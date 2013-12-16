/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "rDeviceAllegroDogMotor.h"
#include "rDeviceAllegroDogMotorCmd.h"
#include "rDeviceAllegroDogCANDef.h"
#include "rDeviceAllegroDogCANCmd.h"
#include <float.h>

RD_IMPLE_FACTORY(AllegroDogMotor)

rDeviceAllegroDogMotor::rDeviceAllegroDogMotor()
: _rt_joint(-1)
, _systemDevice(INVALID_RID)
//, _joint(INVALID_RID) // why real motor need this sim_joint
, _deviceMem_pos(NULL)
, _deviceMem_vel(NULL)
, _deviceMem_cur(NULL)
, _deviceMem_cmd(NULL)
, _deviceMem_opmode(NULL)
, _deviceMem_opmode_display(NULL)
, _deviceMem_opstatus(NULL)
, _deviceMem_faultstatus(NULL)
, _modeOfOperation(eOP_MODE_CUR)
, _tau_des(0.0f)
, _cur_des(0.0f)
, _q_des(0.0f)
, _qdot_des(0.0f)
, _torque_const(1.0f)
, _torque_const_inv(1.0f)
, _gear_ratio(1.0f)
, _gear_ratio_inv(1.0f)
, _current_upper(FLT_MAX)
, _current_lower(-FLT_MAX)
, _current_to_pwm(0)
, _direction(1)
{
}

rDeviceAllegroDogMotor::~rDeviceAllegroDogMotor()
{
}

void rDeviceAllegroDogMotor::onCreate(const rDeviceContext& rdc)
{
	RD_DEVICE_CLASS(Base)::onCreate(rdc);
	
	const TCHAR* prop;
	
	// channel
	prop = getProperty(_T("Joint"));
	if (prop)
		_rt_joint = _tstoi(prop);

	// system device name
	prop = getProperty(_T("system device name"));
	if (prop)
		_tcscpy_s(_systemDeviceName, MAX_PATH, prop);
	else
		_systemDeviceName[0] = 0;

	prop = getProperty(_T("torque constant"));
	if (prop)
	{
		_torque_const = (float)_tstof(prop);
		_torque_const_inv = 1.0f / _torque_const;
	}

	prop = getProperty(_T("gear ratio"));
	if (prop)
	{
		_gear_ratio = (float)_tstof(prop);
		_gear_ratio_inv = 1.0f / _gear_ratio;
	}

	prop = getProperty(_T("current upper"));
	if (prop)
		_current_upper = (float)_tstof(prop);

	prop = getProperty(_T("current lower"));
	if (prop)
		_current_lower = (float)_tstof(prop);

	prop = getProperty(_T("current to pwm"));
	if (prop)
		_current_to_pwm = (float)_tstof(prop);

	prop = getProperty(_T("direction"));
	if (prop && prop[0] == _T('-'))
		_direction = -1;
	else
		_direction = 1;
}

void rDeviceAllegroDogMotor::onInit()
{
	RD_DEVICE_CLASS(Base)::onInit();
	
	// find joint
	//_joint = _rdc.m_deviceAPI->getJointID(_rdc.m_robotname, _rdc.m_nodename);
	//comment out because unnecessary

	// find system device and set device buffer
	AllegroDog_DeviceMemory_t* mem = (AllegroDog_DeviceMemory_t*)(_rdc.m_deviceAPI->getMemory(_systemDeviceName));
	_systemDevice             = _rdc.m_deviceAPI->getDeviceID(_rdc.m_robotname, _systemDeviceName);
	_deviceMem_pos            = &(mem->pos_demand[_rt_joint]);
	_deviceMem_vel            = &(mem->vel_demand[_rt_joint]);
	_deviceMem_cur            = &(mem->cur_demand[_rt_joint]);
	_deviceMem_cmd            = &(mem->cmd_demand[_rt_joint]);
	_deviceMem_opmode         = &(mem->mode_of_op[_rt_joint]);
	_deviceMem_opmode_display = &(mem->mode_of_op_disp[_rt_joint]);
	_deviceMem_opstatus       = &(mem->op_status[_rt_joint]);
	_deviceMem_faultstatus    = &(mem->falut_status[_rt_joint]);

	// print out device properties
	printProps();
}

void rDeviceAllegroDogMotor::onTerminate()
{
	RD_DEVICE_CLASS(Base)::onTerminate();
}

int rDeviceAllegroDogMotor::writeDeviceValue(void* buffer, int len, int port)
{
	if (len == sizeof(float))
	{	
		_lock.lock();
		_tau_des = *(float*)buffer;
		_lock.unlock();
		return sizeof(float);
	}
	else if (len == sizeof(AllegroDog_MotorCommand_t))
	{
		_lock.lock();
		_q_des = ((AllegroDog_MotorCommand_t*)buffer)->q_des;
		_qdot_des = ((AllegroDog_MotorCommand_t*)buffer)->qdot_des;
		_tau_des = ((AllegroDog_MotorCommand_t*)buffer)->tau_des;
		_lock.unlock();
		return sizeof(AllegroDog_MotorCommand_t);
	}
	else
		return 0;
}

int rDeviceAllegroDogMotor::monitorDeviceValue(void* buffer, int len, int port)
{
	if (len == sizeof(float) && port == ALLEGRODOGMOTOR_MONITOR_CURRENT) // current demand value limited by its bounding values
	{	
		_lock.lock();
		*(float*)buffer = _cur_des;
		_lock.unlock();
		return sizeof(float);
	}
	else if (len == sizeof(int) && port == ALLEGRODOGMOTOR_MONITOR_OPMODE_DISPLAY) // operation mode to display
	{	
		_lock.lock();
		*(int*)buffer = *_deviceMem_opmode_display;
		_lock.unlock();
		return sizeof(int);
	}
	else if (len == sizeof(int) && port == ALLEGRODOGMOTOR_MONITOR_OP_STATUS) // operation status
	{	
		_lock.lock();
		*(int*)buffer = *_deviceMem_opstatus;
		_lock.unlock();
		return sizeof(int);
	}
	else if (len == sizeof(int) && port == ALLEGRODOGMOTOR_MONITOR_FALUT_STATUS) // fault status
	{	
		_lock.lock();
		*(int*)buffer = *_deviceMem_faultstatus;
		_lock.unlock();
		return sizeof(int);
	}
	else
		return 0;
}

void rDeviceAllegroDogMotor::updateWriteValue(rTime time)
{
	if (_systemDevice == INVALID_RID) return;
	_lock.lock();
	{
		switch (_modeOfOperation)
		{
		case eOP_MODE_POS:
			{
				*_deviceMem_cmd = (int)(_q_des / (2.0f * M_PI) * 4000.0f * _gear_ratio * _direction);
				//*_deviceMem_cmd = 0;
				*_deviceMem_opmode = eOP_MODE_POS;
			}
			break;
		case eOP_MODE_VEL:
			{
				// NOT IMPLEMENTED !!!
				//*_deviceMem_opmode = eOP_MODE_VEL;
			}
			break;
		case eOP_MODE_CUR:
			{
				_cur_des = _tau_des * _torque_const_inv * _gear_ratio_inv * _direction;
				_cur_des = RD_BOUND(_cur_des, _current_lower, _current_upper);
				*_deviceMem_cur = _cur_des;

				////////////////////////////////////////////////////
				// XXX : TEST
				////////////////////////////////////////////////////
		//		if (_channel == 3)
		//			*_deviceMem_cmd = (int)(_cur_des * _current_to_pwm);
		//		else
		//			*_deviceMem_cmd = 0;

				////////////////////////////////////////////////////
				*_deviceMem_cmd = (int)(_cur_des * _current_to_pwm);
		//		*_deviceMem_cmd = 0;
				////////////////////////////////////////////////////
				*_deviceMem_opmode = eOP_MODE_CUR;
			}
			break;
		}
	}
	_lock.unlock();
}

int rDeviceAllegroDogMotor::command(int cmd, int arg, void* udata, int port)
{
	switch (cmd)
	{
	case ALLEGRODOGMOTOR_CMD_OPMODE_POS:
		_modeOfOperation = eOP_MODE_POS;
		break;
	case ALLEGRODOGMOTOR_CMD_OPMODE_VEL:
		_modeOfOperation = eOP_MODE_VEL;
		break;
	case ALLEGRODOGMOTOR_CMD_OPMODE_CUR:
		_modeOfOperation = eOP_MODE_CUR;
		break;

	case ALLEGRODOGMOTOR_CMD_HOME:
		_rdc.m_deviceAPI->commandDevice(_systemDevice, CAN_CMD_HOMING, arg, 0, _rt_joint);
		break;

	case ALLEGRODOGMOTOR_CMD_QUERY_STATUS:
		_rdc.m_deviceAPI->commandDevice(_systemDevice, CAN_CMD_QUERY_STATUS, arg, 0, _rt_joint);
		break;
	}
	return 0;
}

void rDeviceAllegroDogMotor::printProps()
{
	printf("---------- motor(%d) -----------\n", _rt_joint);
	printf("gear ratio = %.3f\n", _gear_ratio);
	printf("torque constant = %.3f (Nm/A)\n", _torque_const);
	printf("current lower = %.3f (A)\n", _current_lower);
	printf("current upper = %.3f (A)\n", _current_upper);
	printf("current to pwm = %.3f (A)\n", _current_to_pwm);
	printf("direction = %s\n", (_direction > 0 ? "+" : "-"));
	//printf("-------------------------------\n");
}
