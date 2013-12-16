/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include <process.h>
#include "rDeviceAllegroDogCANCmd.h"
#include "rDeviceAllegroDogCANDef.h"
#include "rDeviceAllegroDogCANAPI.h"
#include "rDeviceAllegroDogCAN.h"

static unsigned int __stdcall ioThread(void* inst)
{
	rDeviceAllegroDogCAN* devCAN = (rDeviceAllegroDogCAN*)inst;

	HANDLE hEvents[2];
	int eventIdx;
	int id;
	int len;
	unsigned char data[8];
	int pos1, pos2;
    
    // Configure events to be listened on
    hEvents[0] = devCAN->_ioQuitEvent;
	hEvents[1] = devCAN->_ioEvent;
	
	while (devCAN->_ioThreadRun)
	{
		eventIdx = WaitForMultipleObjects(sizeof(hEvents)/sizeof(hEvents[0]), hEvents, FALSE, 100); 

		switch (eventIdx)
		{
		case 0:
			return 0;//break;

		case 1:
			{
				while (devCAN->_bus != -1 &&
					0 == CANAPI::canReadMsg(devCAN->_bus, &id, &len, data, FALSE))
				{
					if (devCAN->_periodicIOOn) // XXX: we need to check whether it is encoder packet or not!
					{
						if (len == 8)
						{
							pos1 = CANAPI::parse_position(&data[0]);
							pos2 = CANAPI::parse_position(&data[4]);
							if (id >= 0x10+ID_DEVICE_01 && id <= 0x10+ID_DEVICE_12)
							{
								devCAN->_lock.lock();
								devCAN->_vars->enc_actual[(id-0x11)*2+0] = (pos1 - 5000000);
								devCAN->_vars->enc_actual[(id-0x11)*2+1] = (pos2 - 5000000);
								devCAN->_lock.unlock();
							}
						}
#ifdef _DEBUG
						CANAPI::dump_message(id, len, data);
#endif
					}
					else
					{
						map<int, rDeviceAllegroDogCAN::CANProtocolBuffer*>::iterator buf = devCAN->_RxCmdQueue.find(id);
						if (buf != devCAN->_RxCmdQueue.end())
							((rDeviceAllegroDogCAN::CANProtocolBuffer*)(buf->second))->push(data, len);
#ifdef _DEBUG
						else
							CANAPI::dump_message_str(id, len, data);
#endif
					}
				}
			}
			break;
		}
	}

	return 0;
}

static int getMotorDrvId(int port)
{
	return (int)(port/2) + ID_DEVICE_01;
}

static int getMotorDrvChId(int port)
{
	return (int)(port%2) + 1;
}


///////////////////////////////////////////////////////////////////////////////////////////
// rDeviceAllegroDogCAN::CANProtocolBuffer implementation

rDeviceAllegroDogCAN::CANProtocolBuffer::CANProtocolBuffer(rDeviceAllegroDogCAN* devCAN, int id)
: _devCAN(devCAN), _id(id), _tail(0), _count(0), _buf_size(1024)
{
}

void rDeviceAllegroDogCAN::CANProtocolBuffer::flush()
{
	_tail = 0;
}

void rDeviceAllegroDogCAN::CANProtocolBuffer::push(unsigned char* data, int len)
{
	if (size()+len > _buf_size)
	{
		printf(">CANProtocolBuffer(%d): buffer overflow.\n", _id);
		flush();
		return;
	}
	memcpy(_buf+_tail, data, len);
	_tail += len;

	if (_tail > 0 && _buf[_tail-1] == ';')
		handle();
}

int rDeviceAllegroDogCAN::CANProtocolBuffer::size()
{
	return _tail;
}

void rDeviceAllegroDogCAN::CANProtocolBuffer::handle()
{
#ifdef _DEBUG
	CANAPI::dump_message_str(_id, size(), _buf);
#endif

	if (_buf[0] == 'Q' && _buf[1] == '1' && size() == 8)
	{
		int status1, status2;
		_buf[7] = 0;
		sscanf_s((const char*)&_buf[5], "%x", &status1);
		_buf[5] = 0;
		sscanf_s((const char*)&_buf[3], "%x", &status2);
	
		_devCAN->_lock.lock();
		_devCAN->_vars->op_status[(_id-0x11)*2+0] = status1;
		_devCAN->_vars->op_status[(_id-0x11)*2+1] = status2;
		_devCAN->_lock.unlock();
	}

	flush();
}


///////////////////////////////////////////////////////////////////////////////////////////
// rDeviceAllegroDogCAN implementation

RD_IMPLE_FACTORY(AllegroDogCAN)

rDeviceAllegroDogCAN::rDeviceAllegroDogCAN()
: _TX_QUEUE_SIZE(TX_QUEUE_SIZE)
, _RX_QUEUE_SIZE(RX_QUEUE_SIZE)
, _TX_TIMEOUT(TX_TIMEOUT)
, _RX_TIMEOUT(RX_TIMEOUT)
, _mbxID(mbxID)
, _BASE_ID(BASE_ID)
, _MAX_BUS(MAX_BUS)
, _jdof(-1)
, _errCount(0)
, _periodicIOOn(false)
, _ioThread(0)
, _ioThreadRun(false)
, _ioEvent(NULL)
, _ioQuitEvent(NULL)
, _modeOfOperation(eOP_MODE_CUR)
{
	_bus = -1;
	_vars = NULL;
}

rDeviceAllegroDogCAN::~rDeviceAllegroDogCAN()
{
	if (_ioEvent) CloseHandle(_ioEvent);
	if (_ioQuitEvent) CloseHandle(_ioQuitEvent);
}

void rDeviceAllegroDogCAN::onCreate(const rDeviceContext& rdc)
{
	RD_DEVICE_CLASS(Base)::onCreate(rdc);

	_ioEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	_ioQuitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	for (int i=0; i<MAX_DOF; i++)
		_RxCmdQueue.insert(
			map<int, CANProtocolBuffer*>::value_type(
				0x10+ID_DEVICE_01+i, 
				new CANProtocolBuffer(this, 0x10+ID_DEVICE_01+i)));
}

void rDeviceAllegroDogCAN::onInit()
{
	RD_DEVICE_CLASS(Base)::onInit();
	_vars = (AllegroDog_DeviceMemory_t*)_rdc.m_deviceAPI->getMemory(name);
	memset(_vars, 0, sizeof(AllegroDog_DeviceMemory_t));
	_initParams();
	_openCAN();
}

void rDeviceAllegroDogCAN::onTerminate()
{
	_stopSystem();
	_closeCAN();
	map<int, CANProtocolBuffer*>::iterator itr = _RxCmdQueue.begin();
	while (itr != _RxCmdQueue.end())
	{
		delete (itr->second);
		itr++;
	}
	_RxCmdQueue.clear();
	RD_DEVICE_CLASS(Base)::onTerminate();
}

int rDeviceAllegroDogCAN::readDeviceValue(void* buffer, int len, int port)
{
	if (!_vars)
		return 0;

	if (len == sizeof(AllegroDog_DeviceMemory_t))
	{
		_lock.lock();
		memcpy(buffer, _vars, sizeof(AllegroDog_DeviceMemory_t));
		_lock.unlock();
		return sizeof(AllegroDog_DeviceMemory_t);
	}
	else
		return 0;
}

int rDeviceAllegroDogCAN::writeDeviceValue(void* buffer, int len, int port)
{
	if (!_vars)
		return 0;

	if (len == sizeof(AllegroDog_DeviceMemory_t))
	{
		_lock.lock();
		memcpy(_vars, buffer, sizeof(AllegroDog_DeviceMemory_t));
		_lock.unlock();
		return sizeof(AllegroDog_DeviceMemory_t);
	}
	else
		return 0;
}

void rDeviceAllegroDogCAN::updateWriteValue(rTime time)
{
	SetEvent(_ioEvent);
}

void rDeviceAllegroDogCAN::updateReadValue(rTime time)
{
	if (_periodicIOOn)
	{
		//_confirmModeOfOperation();
		_sendMotorCmd();
	}
}

int rDeviceAllegroDogCAN::command(int cmd, int arg, void* udata, int port)
{
	switch (cmd)
	{
	case CAN_CMD_MOTOR_ON:
		{
			if (port >= 0 && port < _jdof)
				_motorOn(getMotorDrvId(port));
			else
				_motorOn();
		}
		break;

	case CAN_CMD_MOTOR_OFF:
		{
			if (port >= 0 && port < _jdof)
				_motorOff(getMotorDrvId(port));
			else
				_motorOff();
		}
		break;

	case CAN_CMD_HOMING:
		{
			int offset = (short)(arg & 0x0000ffff);
			int direction = (short)((arg & 0xffff0000) >> 16);
			if (port >= 0 && port < _jdof)
				_homing(offset, direction, getMotorDrvId(port), getMotorDrvChId(port));
			else
				_homing(offset, direction);
		}
		break;

	case CAN_CMD_RESET_ENC:
		{
			if (port >= 0 && port < _jdof)
				_resetEncoder(getMotorDrvId(port));
			else
				_resetEncoder();
		}
		break;

	case CAN_CMD_QUERY_STATUS:
		{
			if (port >= 0 && port < _jdof)
				_queryStatus(getMotorDrvId(port));
			else
				_queryStatus();
		}
		break;

	case CAN_CMD_ENABLE:
		{
			_startPeriodicCommunication();
		}
		break;

	case CAN_CMD_DISABLE:
		{
			_stopPeriodicCommunication();
		}
		break;

	case CAN_CMD_OPMODE_POSITION:
		{
			if (port >= 0 && port < _jdof)
				_setModeOfOperation(eOP_MODE_POS, getMotorDrvId(port));
			else
				_setModeOfOperation(eOP_MODE_POS);
		}
		break;

	case CAN_CMD_OPMODE_VELOCITY:
		{
			if (port >= 0 && port < _jdof)
				_setModeOfOperation(eOP_MODE_VEL, getMotorDrvId(port));
			else
				_setModeOfOperation(eOP_MODE_VEL);
		}
		break;

	case CAN_CMD_OPMODE_CURRENT:
		{
			if (port >= 0 && port < _jdof)
				_setModeOfOperation(eOP_MODE_CUR, getMotorDrvId(port));
			else
				_setModeOfOperation(eOP_MODE_CUR);
		}
		break;

	case CAN_CMD_SET_POS_TARGET_RELATIVE:
		{
			if (port >= 0 && port < _jdof)
				_sendMotorCmdP2PRelative(getMotorDrvId(port));
			else
				_sendMotorCmdP2PRelative();
		}
		break;

	case CAN_CMD_SET_POS_TARGET_ABSOLUTE:
		{
			if (port >= 0 && port < _jdof)
				_sendMotorCmdP2PAbsolute(getMotorDrvId(port));
			else
				_sendMotorCmdP2PAbsolute();
		}
		break;

	case CAN_CMD_SET_POS_MAX_VELOCITY:
		{
			if (port >= 0 && port < _jdof)
				_setVelocityInP2PMode(arg, arg, getMotorDrvId(port));
			else
				_setVelocityInP2PMode(arg, arg);
		}
		break;

	case CAN_CMD_STOP_SYS:
		{
			_stopSystem();
		}
		break;

	case CAN_CMD_UPDATE_READ:
		updateWriteValue(0);
		break;

	case CAN_CMD_UPDATE_WRITE:
		updateReadValue(0);
		break;
	}
	return 0;
}

void rDeviceAllegroDogCAN::_openCAN()
{
	if (_ioThread)
		return;

	printf(">CAN%d: open\n", _bus);
	if (_bus == -1 || CANAPI::initCAN(_bus))
	{
		printf(">CAN%d: failed to initialize CAN.\n", _bus);
		_enabled = false;
		return;
	}

	// start listening CAN frames
	ResetEvent(_ioQuitEvent);
	_ioThreadRun = true;
	_ioThread = _beginthreadex(NULL, 0, ioThread, (void*)this, 0, NULL);
	printf(">CAN%d: start listening can frames.\n", _bus);

	printf(">CAN%d: initialized successfully.\n", _bus);

	printf(">CAN%d: system init.\n", _bus);
	if (_bus == -1 ||
		CANAPI::command_system_init(_bus, ID_DEVICE_ALL))
	{
		printf(">CAN%d: failed to initialize CAN communication parameters.\n", _bus);
		_enabled = false;
		_closeCAN();
		return;
	}

	printf(">CAN%d: set response address.\n", _bus);
	if (_bus == -1 ||
		CANAPI::command_set_respond_address(_bus, ID_DEVICE_ALL))
	{
		printf(">CAN%d: failed to set response address.\n", _bus);
		_enabled = false;
		_closeCAN();
		return;
	}
}

void rDeviceAllegroDogCAN::_closeCAN()
{
	if (!_ioThread)
		return;
	
	_ioThreadRun = false;
	//SetEvent(_ioEvent);
	SetEvent(_ioQuitEvent);
	WaitForSingleObject((HANDLE)_ioThread, INFINITE);
	CloseHandle((HANDLE)_ioThread);
	_ioThread = 0;
	printf(">CAN%d: stoped listening CAN frames.\n", _bus);

	printf(">CAN%d: close\n", _bus);
	if (_bus == -1 || CANAPI::freeCAN(_bus))
	{
		printf(">CAN%d: failed to free CAN.\n", _bus);
		return;
	}

	printf(">CAN%d: closed.\n", _bus);
}

void rDeviceAllegroDogCAN::_startPeriodicCommunication()
{
	_lock.lock();
	{
		printf(">CAN%d: set response address.\n", _bus);
		if (_bus == -1 ||
			CANAPI::command_set_respond_address(_bus, ID_DEVICE_ALL))
		{
			printf(">CAN%d: failed to set response address.\n", _bus);
			_periodicIOOn = false;
			//_enabled = false;
			//_closeCAN();
		}
		else
		{
			printf(">CAN%d: activate p mode\n", _bus);
			if (_bus == -1 ||
				CANAPI::command_activate_pmode(_bus, ID_DEVICE_ALL))
			{
				printf(">CAN%d: failed to activate p mode.\n", _bus);
				_periodicIOOn = false;
				//_enabled = false;
			}
			else
				_periodicIOOn = true;
		}
	}
	_lock.unlock();
}

void rDeviceAllegroDogCAN::_stopPeriodicCommunication()
{
	_lock.lock();
	{
		printf(">CAN%d: deactivate p mode.\n", _bus);
		if (_bus == -1 ||
			CANAPI::command_deactivate_pmode(_bus, ID_DEVICE_ALL))
		{
			printf(">CAN%d: failed to deactivate p mode.\n", _bus);
			//_enabled = false;
		}
		_periodicIOOn = false;
	}
	_lock.unlock();
}

void rDeviceAllegroDogCAN::_motorOn(int drvId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: motor on.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			printf(">CAN%d: all motor on.\n", _bus);

			int rtn = 0;
			if (_bus == 0)
			{
				rtn |= CANAPI::command_motor_on(_bus, ID_DEVICE_01);
				rtn |= CANAPI::command_motor_on(_bus, ID_DEVICE_02);
				rtn |= CANAPI::command_motor_on(_bus, ID_DEVICE_03);
			}
			else if (_bus == 1)
			{
				rtn |= CANAPI::command_motor_on(_bus, ID_DEVICE_04);
				rtn |= CANAPI::command_motor_on(_bus, ID_DEVICE_05);
				rtn |= CANAPI::command_motor_on(_bus, ID_DEVICE_06);
			}
			if (0 != rtn)
			{
				printf(">CAN%d: failed to set all motor on.\n", _bus);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			printf(">CAN%d: motor on(%d).\n", _bus, drvId);

			if (0 != CANAPI::command_motor_on(_bus, drvId))
			{
				printf(">CAN%d: failed to set motor on(%d).\n", _bus, drvId);
			}
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_motorOff(int drvId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: motor off.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			printf(">CAN%d: all motor off.\n", _bus);

			int rtn = 0;
			if (_bus == 0)
			{
				rtn |= CANAPI::command_motor_off(_bus, ID_DEVICE_01);
				rtn |= CANAPI::command_motor_off(_bus, ID_DEVICE_02);
				rtn |= CANAPI::command_motor_off(_bus, ID_DEVICE_03);
			}
			else if (_bus == 1)
			{
				rtn |= CANAPI::command_motor_off(_bus, ID_DEVICE_04);
				rtn |= CANAPI::command_motor_off(_bus, ID_DEVICE_05);
				rtn |= CANAPI::command_motor_off(_bus, ID_DEVICE_06);
			}
			if (0 != rtn)
			{
				printf(">CAN%d: failed to set all motor off.\n", _bus);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			printf(">CAN%d: motor off(%d).\n", _bus, drvId);

			if (0 != CANAPI::command_motor_off(_bus, drvId))
			{
				printf(">CAN%d: failed to set motor off(%d).\n", _bus, drvId);
			}
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_homing(int offset, int dir, int drvId, int chId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: homing.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			printf(">CAN%d: homing\n", _bus);

			if (0 != CANAPI::command_home(_bus, ID_DEVICE_ALL, 1, 1, dir, dir, offset, offset))
			{
				printf(">CAN%d: failed for homing.\n", _bus);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			printf(">CAN%d: homing(%d:%d, %d, %d).\n", _bus, drvId, chId, dir, offset);

			int enable1 = (chId == 1 || chId < 0) ? 1 : 0;
			int enable2 = (chId == 2 || chId < 0) ? 1 : 0;
			if (0 != CANAPI::command_home(_bus, drvId, enable1, enable2, dir, dir, offset, offset))
			{
				printf(">CAN%d: failed to homing(%d).\n", _bus, drvId);
			}
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_resetEncoder(int drvId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: reset encoder.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			printf(">CAN%d: reset all encoder.\n", _bus);

			if (0 != CANAPI::command_reset_encoder(_bus, ID_DEVICE_ALL))
			{
				printf(">CAN%d: failed to reset all encoder.\n", _bus);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			printf(">CAN%d: reset encoder(%d).\n", _bus, drvId);

			if (0 != CANAPI::command_reset_encoder(_bus, drvId))
			{
				printf(">CAN%d: failed to reset encoder(%d).\n", _bus, drvId);
			}
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_queryStatus(int drvId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: query status.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	int result = 0;

	if (drvId < 0) // all motor drives
	{
		//printf(">CAN%d: query status.\n", _bus);
		_lock.lock();
		{
			result = CANAPI::command_query_status(_bus, ID_DEVICE_ALL);
		}
		_lock.unlock();
		if (0 != result)
			printf(">CAN%d: failed to query status.\n", _bus);
	}
	else
	{
		//printf(">CAN%d: query status(%d).\n", _bus, drvId);
		_lock.lock();
		{
			result = CANAPI::command_query_status(_bus, drvId);
		}
		_lock.unlock();
		if (0 != result)
			printf(">CAN%d: failed to query status(%d).\n", _bus, drvId);
	}
}

void rDeviceAllegroDogCAN::_setModeOfOperation(int opmode, int drvId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: set mode of operation.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			printf(">CAN%d: set mode of operation(mode=%d).\n", _bus, opmode);

			if (0 != CANAPI::command_set_mode_of_operation(_bus, ID_DEVICE_ALL, opmode, opmode))
			{
				printf(">CAN%d: failed to set mode of operation(mode=%d).\n", _bus, opmode);
			}
			else
			{
				for (int i=0; i<_jdof; i++)
					_vars->mode_of_op_disp[i] = eOP_MODE_CUR;
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			printf(">CAN%d: set mode of operation(%d, mode=%d).\n", _bus, drvId, opmode);

			if (0 != CANAPI::command_set_mode_of_operation(_bus, ID_DEVICE_ALL, opmode, opmode))
			{
				printf(">CAN%d: failed to set mode of operation(%d, mode=%d).\n", _bus, drvId, opmode);
			}
			else
			{
				_vars->mode_of_op_disp[drvId-ID_DEVICE_01*2+0] = eOP_MODE_CUR;
				_vars->mode_of_op_disp[drvId-ID_DEVICE_01*2+1] = eOP_MODE_CUR;
			}
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_setVelocityInP2PMode(int v1, int v2, int drvId)
{
	if (_bus == -1 || !_enabled)
	{
		printf(">CAN%d: set velocity in P2P mode.\n", _bus);
		printf("\tfailed. CAN is not initialized.\n");
		return;
	}

	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			printf(">CAN%d: set velocity in P2P mode.\n", _bus);

			if (0 != CANAPI::command_set_p2p_velocity(_bus, ID_DEVICE_ALL, v1, v2))
			{
				printf(">CAN%d: failed to set velocity in P2P mode.\n", _bus);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			printf(">CAN%d: set velocity in P2P mode(%d).\n", _bus, drvId);

			if (0 != CANAPI::command_set_p2p_velocity(_bus, ID_DEVICE_ALL, v1, v2))
			{
				printf(">CAN%d: failed to set velocity in P2P mode(%d).\n", _bus, drvId);
			}
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_stopSystem()
{
	_stopPeriodicCommunication();
	_motorOff();
}

void rDeviceAllegroDogCAN::_sendMotorCmd(int drvId)
{
	int rtn;

	if (!_vars) return;
	
	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			if (_bus == 0)
			{
				rtn = CANAPI::write_control_cmds(_bus, ID_DEVICE_01, _vars->cmd_demand[0],  _vars->cmd_demand[1]);
				rtn = CANAPI::write_control_cmds(_bus, ID_DEVICE_02, _vars->cmd_demand[2],  _vars->cmd_demand[3]);
				rtn = CANAPI::write_control_cmds(_bus, ID_DEVICE_03, _vars->cmd_demand[4],  _vars->cmd_demand[5]);
			}
			else if (_bus == 1)
			{
				rtn = CANAPI::write_control_cmds(_bus, ID_DEVICE_04, _vars->cmd_demand[6],  _vars->cmd_demand[7]);
				rtn = CANAPI::write_control_cmds(_bus, ID_DEVICE_05, _vars->cmd_demand[8],  _vars->cmd_demand[9]);
				rtn = CANAPI::write_control_cmds(_bus, ID_DEVICE_06, _vars->cmd_demand[10], _vars->cmd_demand[11]);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			CANAPI::write_control_cmds(_bus, drvId, _vars->cmd_demand[2*(drvId-ID_DEVICE_01)+0],  _vars->cmd_demand[2*(drvId-ID_DEVICE_01)+1]);
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_sendMotorCmdP2PAbsolute(int drvId)
{
	int rtn;

	if (!_vars) return;
	
	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			if (_bus == 0)
			{
				rtn = CANAPI::command_set_p2p_position_absolute(_bus, ID_DEVICE_01, _vars->cmd_demand[0],  _vars->cmd_demand[1]);
				rtn = CANAPI::command_set_p2p_position_absolute(_bus, ID_DEVICE_02, _vars->cmd_demand[2],  _vars->cmd_demand[3]);
				rtn = CANAPI::command_set_p2p_position_absolute(_bus, ID_DEVICE_03, _vars->cmd_demand[4],  _vars->cmd_demand[5]);
			}
			else if (_bus == 1)
			{
				rtn = CANAPI::command_set_p2p_position_absolute(_bus, ID_DEVICE_04, _vars->cmd_demand[6],  _vars->cmd_demand[7]);
				rtn = CANAPI::command_set_p2p_position_absolute(_bus, ID_DEVICE_05, _vars->cmd_demand[8],  _vars->cmd_demand[9]);
				rtn = CANAPI::command_set_p2p_position_absolute(_bus, ID_DEVICE_06, _vars->cmd_demand[10], _vars->cmd_demand[11]);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			CANAPI::command_set_p2p_position_absolute(_bus, drvId, _vars->cmd_demand[2*(drvId-ID_DEVICE_01)+0],  _vars->cmd_demand[2*(drvId-ID_DEVICE_01)+1]);
		}
		_lock.unlock();
	}	
}

void rDeviceAllegroDogCAN::_sendMotorCmdP2PRelative(int drvId)
{
	int rtn;

	if (!_vars) return;
	
	if (drvId < 0) // all motor drives
	{
		_lock.lock();
		{
			if (_bus == 0)
			{
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_01, 1,  _vars->cmd_demand[0]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_01, 2,  _vars->cmd_demand[1]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_02, 1,  _vars->cmd_demand[2]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_02, 2,  _vars->cmd_demand[3]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_03, 1,  _vars->cmd_demand[4]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_03, 2,  _vars->cmd_demand[5]);
			}
			else if (_bus == 1)
			{
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_04, 1,  _vars->cmd_demand[6]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_04, 2,  _vars->cmd_demand[7]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_05, 1,  _vars->cmd_demand[8]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_05, 2,  _vars->cmd_demand[9]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_06, 1,  _vars->cmd_demand[10]);
				rtn = CANAPI::command_set_p2p_position_relative(_bus, ID_DEVICE_06, 2,  _vars->cmd_demand[11]);
			}
		}
		_lock.unlock();
	}
	else
	{
		_lock.lock();
		{
			CANAPI::command_set_p2p_position_relative(_bus, drvId, 1, _vars->cmd_demand[2*(drvId-ID_DEVICE_01)+0]);
			CANAPI::command_set_p2p_position_relative(_bus, drvId, 2, _vars->cmd_demand[2*(drvId-ID_DEVICE_01)+1]);
		}
		_lock.unlock();
	}
}

void rDeviceAllegroDogCAN::_confirmModeOfOperation()
{
	int index_start, index_end;
	int index_0, index_1;
	int opmode_0, opmode_1;
	
	if (!_vars) return;

	if (_bus == 0)
	{
		index_start = ID_DEVICE_01;
		index_end   = ID_DEVICE_03;
	}
	else if (_bus == 1)
	{
		index_start = ID_DEVICE_04;
		index_end   = ID_DEVICE_06;
	}
	else
		return;

	_lock.lock();
	{
		for (int i=index_start; i<=index_end; i++)
		{
			index_0 = 2*(i-index_start);
			index_1 = 2*(i-index_start)+1;
			if (_vars->mode_of_op[index_0] != _vars->mode_of_op_disp[index_0] ||
				_vars->mode_of_op[index_1] != _vars->mode_of_op_disp[index_1])
			{
				printf(">CAN%d: set mode of operation(%d)\n", _bus, i);
				opmode_0 = _vars->mode_of_op[index_0];
				opmode_1 = _vars->mode_of_op[index_1];
				if (opmode_0 < 0 ||
					opmode_1 < 0 ||
					0 != CANAPI::command_set_mode_of_operation(_bus, i, opmode_0, opmode_1))
				{
					printf(">CAN%d: failed. set mode of operation(%d)\n", _bus, i);
				}
				else
				{
					_vars->mode_of_op_disp[index_0] = _vars->mode_of_op[index_0];
					_vars->mode_of_op_disp[index_1] = _vars->mode_of_op[index_1];
				}
			}
		}
	}
	_lock.unlock();
}

void rDeviceAllegroDogCAN::_initParams()
{
	const TCHAR* prop;

	// read bus parameters
	prop = getProperty(_T("TX_QUEUE_SIZE"));
	if (prop)
		_TX_QUEUE_SIZE = _tstoi(prop);
	
	prop = getProperty(_T("RX_QUEUE_SIZE"));
	if (prop)
		_RX_QUEUE_SIZE = _tstoi(prop);
	
	prop = getProperty(_T("TX_TIMEOUT"));
	if (prop)
		_TX_TIMEOUT = _tstoi(prop);
	
	prop = getProperty(_T("RX_TIMEOUT"));
	if (prop)
		_RX_TIMEOUT = _tstoi(prop);
	
	prop = getProperty(_T("mbxID"));
	if (prop)
		_mbxID = _tstoi(prop);
	
	prop = getProperty(_T("BASE_ID"));
	if (prop)
		_BASE_ID = _tstoi(prop);

	prop = getProperty(_T("MAX_BUS"));
	if (prop)
		_MAX_BUS = _tstoi(prop);

	prop = getProperty(_T("BUS"));
	if (prop)
		_bus = _tstoi(prop);

	prop = getProperty(_T("JDOF"));
	if (prop)
		_jdof = _tstoi(prop);

	_errCount = 0;
}
