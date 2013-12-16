/* RoboticsLab, Copyright 2008-2010 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "control_AllegroDogHome.h"
#include "control_AllegroDogHomeCmd.h"
#include "rPanelManipulatorCmd.h"
#include "rCommon/rParseUtil.h"
#include "rDeviceAllegroDogCANCmd.h"
#include "../rDeviceAllegroDogMotor/rDeviceAllegroDogMotorCmd.h"

control_AllegroDogHome::control_AllegroDogHome(rDC rdc) 
:rControlAlgorithmEx(rdc)
, _dT(0.002)
, _jdof(JDOF)
, _home_state_global(HOME_INIT)
, _home_step_global(0)
{
}

control_AllegroDogHome::~control_AllegroDogHome()
{
}

void control_AllegroDogHome::_servoOn()
{
}

void control_AllegroDogHome::_servoOff()
{
}

void control_AllegroDogHome::_initHome()
{
	if (_home_state_global)
	{
		printf(">control_AllegroDogHome: ALREADY DONE !!!\n");
		return;
	}

	for (int i=0; i<NUM_CAN_CH; i++)
	{
		commandDevice(_can[i], CAN_CMD_DISABLE, 0, 0, -1);
		//Sleep(500);
		commandDevice(_can[i], CAN_CMD_MOTOR_OFF, 0, 0, -1);
		commandDevice(_can[i], CAN_CMD_MOTOR_ON, 0, 0, -1);
		commandDevice(_can[i], CAN_CMD_OPMODE_POSITION, 0, 0, -1);
		commandDevice(_can[i], CAN_CMD_SET_POS_MAX_VELOCITY, _home_speed, 0, -1);
	}
	AllegroDog_MotorCommand_t motorCmd;
	motorCmd.q_des = 0.0f;
	motorCmd.qdot_des = 0.0f;
	motorCmd.tau_des = 0.0f;
	for (int i=0; i<_jdof; i++)
	{
		_home_state[i] = HOME_INIT;
		// write ZERO to motor command
		writeDeviceValue(_motor[i], &motorCmd, sizeof(AllegroDog_MotorCommand_t));
	}
	_home_state_global = HOME_SEARCH_HOME;
	_home_step_global = 1;

	Sleep(2);
	_commandHome(2);
	Sleep(2);
	_commandHome(5);
	Sleep(2);
	_commandHome(8);
	Sleep(2);
	_commandHome(11);
}

void control_AllegroDogHome::_finalizeHome()
{
	//for (int i=0; i<NUM_CAN_CH; i++)
	//{
	////	Sleep(1000);
	////	commandDevice(_can[i], CAN_CMD_RESET_ENC, 0, 0, -1);
	//	//commandDevice(_can[i], CAN_CMD_MOTOR_OFF, 0, 0, -1);
	//	//commandDevice(_can[i], CAN_CMD_OPMODE_CURRENT, 0, 0, -1);
	//	for (int j=0; j<JDOF; j+=2)
	//	{
	//		Sleep(2);
	//		commandDevice(_can[i], CAN_CMD_RESET_ENC, 0, 0, j);
	//	}
	//}
/*	int i=0;
	{
		for (int j=0; j<6; j+=2)
		{
			Sleep(100);
			commandDevice(_can[i], CAN_CMD_RESET_ENC, 0, 0, j);
		}
	}
	i=1;
	{
		for (int j=6; j<JDOF; j+=2)
		{
			Sleep(100);
			commandDevice(_can[i], CAN_CMD_RESET_ENC, 0, 0, j);
		}
	} */
	for (int i=0; i<NUM_CAN_CH; i++)
	{
		Sleep(2);
		commandDevice(_can[i], CAN_CMD_RESET_ENC, 0, 0, -1);
	} 
	printf("Encoder Reset: ALL DONE !!!\n");
	
	/*for (int i=0; i<_jdof; i++)
	{
		commandDevice(_motor[i], ALLEGRODOGMOTOR_CMD_OPMODE_CUR);
	}*/
}

void control_AllegroDogHome::_commandHome(int jid)
{
	RASSERT(jid >= 0 && jid < JDOF);
	commandDevice(_motor[jid], ALLEGRODOGMOTOR_CMD_OPMODE_POS);
	commandDevice(_motor[jid], ALLEGRODOGMOTOR_CMD_HOME, (_home_direction[jid] << 16) | (unsigned short)(_home_offset[jid] + _home_offset_tune[jid]));
}

void control_AllegroDogHome::_findDevices()
{
	for (int i=0; i<_jdof; i++)
	{
		TCHAR devname[32];

		_stprintf_s(devname, 32, _T("Mot%d"), i + 1);
		_motor[i] = findDevice(devname);

		_stprintf_s(devname, 32, _T("Enc%d"), i + 1);
		_enc[i] = findDevice(devname);
	}
	for (int i=0; i<NUM_CAN_CH; i++)
	{
		TCHAR devname[32];

		_stprintf_s(devname, 32, _T("COMM%d"), i);
		_can[i] = findDevice(devname);
	}
}

void control_AllegroDogHome::init(int mode)
{
	const TCHAR* prop = NULL;
	float* val = NULL;
	int* ival = NULL;

	prop = getProperty(_T("JDOF"));
	if (prop)
		_jdof = parse_atoi(prop);

	_findDevices();

	_q.resize(_jdof);
	_q.zero();

	val = new float[_jdof];
	ival = new int[_jdof];
	
	prop = getProperty(_T("dT"));
	if (prop)
		_dT = parse_atof(prop);

	prop = getProperty(_T("speed"));
	if (prop)
		_home_speed = _ttoi(prop);
	else
		_home_speed = 200;

	prop = getProperty(_T("direction"));
	if (prop)
	{
		for (int i=0; i<_jdof; i++)
			ival[i] = 0;
		parse_ivector(ival, _jdof, prop);
		for (int i=0; i<_jdof; i++)
			_home_direction[i] = (ival[i] == 0 ? ival[0] : ival[i]);
	}

	prop = getProperty(_T("offset"));
	if (prop)
	{
		for (int i=0; i<_jdof; i++)
			ival[i] = 0;
		parse_ivector(ival, _jdof, prop);
		for (int i=0; i<_jdof; i++)
			_home_offset[i] = (ival[i] == 0 ? ival[0] : ival[i]);
	}

	prop = getProperty(_T("offset_tune"));
	if (prop)
	{
		for (int i=0; i<_jdof; i++)
			ival[i] = 0;
		parse_ivector(ival, _jdof, prop);
		for (int i=0; i<_jdof; i++)
			_home_offset_tune[i] = (ival[i] == 0 ? ival[0] : ival[i]);
	}

	delete [] val;

	////////////////////////////////////////////////////
	_sharedMemory = findDevice(_T("SharedMemory"));
}

void control_AllegroDogHome::update(const rTime& t)
{
	_curTime = t;
	rControlAlgorithm::update(t);
}

void control_AllegroDogHome::setNominalSystem(const TCHAR* path, const TCHAR* aml, const HTransform& T0, const dVector& q0)
{
}

void control_AllegroDogHome::setPeriod(const rTime& dT)
{
	_dT = dT;
}

void control_AllegroDogHome::_readDevices()
{
	float val;
	int ival;
	for (int i=0; i<JDOF; i++)
	{
		if (_enc[i] != INVALID_RHANDLE)
		{
			if (0 < readDeviceValue(_enc[i], &val, 4))
				_q[i] = (double)val;
		}

		if (_motor[i] != INVALID_RHANDLE)
		{
			if (0 < monitorDeviceValue(_motor[i], &ival, 4, ALLEGRODOGMOTOR_MONITOR_OP_STATUS))
			{
				_home_state[i] = (eHomeState)((ival & 0xf0) >> 4);
			}
		}
	}
}

void control_AllegroDogHome::_writeDevices()
{
	if (HOME_SEARCH_HOME == _home_state_global)
	{
		/*for (int i=0; i<_jdof; i++)
		{
			commandDevice(_motor[i], ALLEGRODOGMOTOR_CMD_QUERY_STATUS);
		}*/
		for (int i=0; i<NUM_CAN_CH; i++)
		{
			commandDevice(_can[i], CAN_CMD_QUERY_STATUS, 0, 0, -1);
		}
	}
}

void control_AllegroDogHome::_reflect()
{
}

void control_AllegroDogHome::_compute(const double& t)
{
	if (HOME_SEARCH_HOME == _home_state_global)
	{
		if (_home_step_global == 1 &&
			_home_state[2] == HOME_SUCCESS && 
			_home_state[5] == HOME_SUCCESS &&
			_home_state[8] == HOME_SUCCESS &&
			_home_state[11] == HOME_SUCCESS &&
			_home_state[1] == HOME_INIT &&
			_home_state[10] == HOME_INIT)
		{
			printf(">control_AllegroDogHome: done(2, 5, 8, 10) !!!\n");

			Sleep(2);
			_commandHome(1);
			Sleep(2);
			_commandHome(10);
			_home_step_global++;
		}
		else if (_home_step_global == 2 &&
				 _home_state[1] == HOME_SUCCESS && 
				 _home_state[10] == HOME_SUCCESS &&
				 _home_state[4] == HOME_INIT &&
				 _home_state[7] == HOME_INIT)
		{
			printf(">control_AllegroDogHome: done(1, 10) !!!\n");

			Sleep(2);
			_commandHome(4);
			Sleep(2);
			_commandHome(7);
			_home_step_global++;
		}
		else if (_home_step_global == 3 &&
				 _home_state[4] == HOME_SUCCESS &&
				 _home_state[7] == HOME_SUCCESS &&
				 _home_state[0] == HOME_INIT &&
				 _home_state[9] == HOME_INIT)
		{
			printf(">control_AllegroDogHome: done(4, 7) !!!\n");

			Sleep(2);
			_commandHome(0);
			Sleep(2);
			_commandHome(9);
			_home_step_global++;
		}
		else if (_home_step_global == 4 &&
				 _home_state[0] == HOME_SUCCESS &&
				 _home_state[9] == HOME_SUCCESS &&
				 _home_state[3] == HOME_INIT &&
				 _home_state[6] == HOME_INIT)
		{
			printf(">control_AllegroDogHome: done(0, 9) !!!\n");

			Sleep(2);
			_commandHome(3);
			Sleep(2);
			_commandHome(6);
			_home_step_global++;
		}
		else if (_home_step_global == 5 &&
				 _home_state[3] == HOME_SUCCESS && 
				 _home_state[6] == HOME_SUCCESS)
		{
			printf(">control_AllegroDogHome: done(3, 6) !!!\n");
			printf(">control_AllegroDogHome: ALL DONE !!!\n");
			_home_state_global = HOME_SUCCESS;
			_finalizeHome();
		}
		else
		{
			for (int i=0; i<_jdof; i++)
			{
				if (HOME_FAIL == _home_state[i])
				{
					printf(">control_AllegroDogHome: more than one joint failed to home(%d).\n", i);
					_home_state_global = HOME_FAIL;
					break;
				}
			}
		}
	}
}

void control_AllegroDogHome::_estimate()
{
}

int control_AllegroDogHome::command(const short& cmd, const int& arg)
{
	switch (cmd)
	{
	case CMD_HOME:
		_initHome();
		break;

	case RCMD_ACTIVATE_CONTROL:
		break;

	case RESERVED_CMD_SERVO_ON:
		_servoOn();
		break;

	case RESERVED_CMD_SERVO_OFF:
		_servoOff();
		break;
	}

	return 0;
}

void control_AllegroDogHome::datanames(vector<string_type>& names, int channel)
{
}

void control_AllegroDogHome::collect(vector<double>& data, int channel)
{
}

void control_AllegroDogHome::onSetInterestFrame(const TCHAR* name, const HTransform& T)
{
}

rControlAlgorithm* CreateControlAlgorithm(rDC& rdc)
{
	return new control_AllegroDogHome(rdc);
}
