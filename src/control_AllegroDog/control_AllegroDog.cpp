/* RoboticsLab, Copyright 2008-2010 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "control_AllegroDog.h"
#include "control_AllegroDogCmd.h"
#include "rPanelManipulatorCmd.h"
#include "rCommon/rParseUtil.h"
#include "float.h"
#include "rDeviceWithRobotmyAHRSCmd.h"
#include "../rDeviceAllegroDogMotor/rDeviceAllegroDogMotorCmd.h"

control_AllegroDog::control_AllegroDog(rDC rdc) 
:rControlAlgorithmEx(rdc)
, _dT(0.002)
, _jdof(0)
, _alg(NULL)
, _filter_derivative(NULL)
, _filter_derivative_des(NULL)
, _filter_cutoff(10.0)
, _jid_test(0)
, _test_mode(eTEST_MODE_WALK) // eTEST_MODE_DISABLE
, _test_time_start(0)
, _test_traj_amp(0)
, _test_traj_freq(0)
{
}

control_AllegroDog::~control_AllegroDog()
{
	if (_alg)
		delete _alg;

	if (_filter_derivative) delete _filter_derivative;
	if (_filter_derivative_des) delete _filter_derivative_des;
}

void control_AllegroDog::_servoOn()
{
}

void control_AllegroDog::_servoOff()
{
}

void control_AllegroDog::_findDevices()
{
	for (int i=0; i<_jdof; i++)
	{
		TCHAR devname[32];

		_stprintf(devname, _T("Mot%d"), i + 1);
		_motor[i] = findDevice(devname);

		_stprintf(devname, _T("Enc%d"), i + 1);
		_enc[i] = findDevice(devname);
	}

	_arhs = findDevice(_T("Gyro_Sensor"));
}

void control_AllegroDog::init(int mode)
{
	_alg = CreateAllegroDogWalk();
	assert(_alg);

	_jdof = JDOF;

	_findDevices();

	_q.resize(_jdof);
	_err.resize(_jdof);
	_err_dot.resize(_jdof);
	_err_old.resize(_jdof);
	_qdot.resize(_jdof);
	_q_des.resize(_jdof);
	_qdot_des.resize(_jdof);
	_cur_des.resize(_jdof);
	_tau_des.resize(_jdof);
	_test_traj_offset.resize(_jdof);
	
	_q.zero();
	_err.zero();
	_err_dot.zero();
	_err_old.zero();
	_qdot.zero();
	_cur_des.zero();
	_tau_des.zero();
	_q_des.zero();
	_qdot_des.zero();
	_test_traj_offset.zero();

	memset(_x, 0, sizeof(_x[0])*4);
	memset(_y, 0, sizeof(_y[0])*4);
	memset(_z, 0, sizeof(_z[0])*4);

	const TCHAR* prop = NULL;
	float* val = NULL;

	val = new float[_jdof];
	prop = getProperty(_T("dT"));
	if (prop)
		_dT = parse_atof(prop);

	_kp.resize(_jdof);
	_kp.zero();
	_ki.resize(_jdof);
	_ki.zero();
	_kd.resize(_jdof);
	_kd.zero();
	_kp_balance = 0.0;
	_kd_balance = 0.0;
	_kp_turn = 0.0;

	_remoteTP_BT	= findDevice(_T("RemoteTP_BT"));
	_sz2read_msgRTP = sizeof(MessageRemoteTP_t);

	prop = getProperty(_T("Kp"));
	if (prop)
	{
		for (int i=0; i<_jdof; i++)
			val[i] = FLT_MAX;
		parse_vector(val, _jdof, prop);
		for (int i=0; i<_jdof; i++)
			_kp[i] = (val[i] == FLT_MAX ? val[0] : val[i]);
	}

	prop = getProperty(_T("Ki"));
	if (prop)
	{
		for (int i=0; i<_jdof; i++)
			val[i] = FLT_MAX;
		parse_vector(val, _jdof, prop);
		for (int i=0; i<_jdof; i++)
			_ki[i] = (val[i] == FLT_MAX ? val[0] : val[i]);
	}

	prop = getProperty(_T("Kd"));
	if (prop)
	{
		for (int i=0; i<_jdof; i++)
			val[i] = FLT_MAX;
		parse_vector(val, _jdof, prop);
		for (int i=0; i<_jdof; i++)
			_kd[i] = (val[i] == FLT_MAX ? val[0] : val[i]);
	}

	prop = getProperty(_T("Kp Balance"));
	if (prop)
		_kp_balance = parse_atof(prop);

	prop = getProperty(_T("Kd Balance"));
	if (prop)
		_kd_balance = parse_atof(prop);

	prop = getProperty(_T("Kp Turn"));
	if (prop)
		_kp_turn = parse_atof(prop);

	_stride_freq = STRIDE_FREQ;
	_stride_step_height = STRIDE_STEP_HEIGHT;
	_stride_step_length = STRIDE_STEP_LENGTH;
	_stride_step_inc_X = INC_POS_X;
	_stride_step_inc_Z = INC_POS_Z;
	_home_position_height = SP_HEIGHT;
	_home_position_front = SP_FRONT;
	_home_position_hind = SP_HIND;
	_L0 = L0;
	_L1 = L1;
	_L2 = L2;
	_BL = BX*2.0; // multiply 2.0 because BX is the half of the body length
	_BW = BY*2.0;

	prop = getProperty(_T("Stride Step Length"));
	if (prop)
		_stride_step_length = parse_atof(prop);

	prop = getProperty(_T("Stride Step Height"));
	if (prop)
		_stride_step_height = parse_atof(prop);

	prop = getProperty(_T("Stride Frequency"));
	if (prop)
		_stride_freq = parse_atof(prop);
	
	prop = getProperty(_T("Stride Step Increment X"));
	if (prop)
		_stride_step_inc_X = parse_atof(prop);

	prop = getProperty(_T("Stride Step Increment Z"));
	if (prop)
		_stride_step_inc_Z = parse_atof(prop);

	prop = getProperty(_T("Body Length"));
	if (prop)
		_BL = parse_atof(prop);

	prop = getProperty(_T("Body Width"));
	if (prop)
		_BW = parse_atof(prop);

	prop = getProperty(_T("L0"));
	if (prop)
		_L0 = parse_atof(prop);

	prop = getProperty(_T("L1"));
	if (prop)
		_L1 = parse_atof(prop);

	prop = getProperty(_T("L2"));
	if (prop)
		_L2 = parse_atof(prop);

	prop = getProperty(_T("Home Position Height"));
	if (prop)
		_home_position_height = parse_atof(prop);

	prop = getProperty(_T("Home Position Front"));
	if (prop)
		_home_position_front = parse_atof(prop);

	prop = getProperty(_T("Home Position Hind"));
	if (prop)
		_home_position_hind = parse_atof(prop);


	delete [] val;

	memset(_rpy, 0, sizeof(_rpy[0])*3);

	if (_alg)
	{
		_alg->SetTimeInterval(_dT);
		_alg->SetGains(_kp.array, _ki.array, _kd.array);
		_alg->SetBalanceGain(_kp_balance, _kd_balance);
		_alg->SetTurnGain(_kp_turn);
		_alg->SetKinematics(_L0, _L1, _L2, _BW, _BL);
		_alg->SetWalkingParameters(_stride_step_length, _stride_step_height, _stride_freq, _stride_step_inc_X, _stride_step_inc_Z);
		_alg->SetDesiredHomePosition(_home_position_height, _home_position_front, _home_position_hind);
	}

	////////////////////////////////////////////////////
	_sharedMemory = findDevice(_T("SharedMemory"));

	// initialize shared memory with initial values
	/*rPanelManipulatorData_t buf;
	if (0 < readDeviceValue(_sharedMemory, &buf, sizeof(buf)))
	{
		writeDeviceValue(_sharedMemory, &buf, sizeof(buf));
	}*/

	if (_jdof > 0)
	{
		double t_cutoff = 1.0 / _filter_cutoff;
		_filter_derivative = new rxFilteredDerivative(_dT, t_cutoff, _q);
		_filter_derivative_des = new rxFilteredDerivative(_dT, t_cutoff, _q_des);
	}

	_test_mode = eTEST_MODE_WALK;
}

void control_AllegroDog::update(const rTime& t)
{
	_curTime = t;
	rControlAlgorithm::update(t);
}

void control_AllegroDog::setNominalSystem(const TCHAR* path, const TCHAR* aml, const HTransform& T0, const dVector& q0)
{
}

void control_AllegroDog::setPeriod(const rTime& dT)
{
	_dT = dT;
}

void control_AllegroDog::_readDevices()
{
	float val;
	for (int i=0; i<JDOF; i++)
	{
		if (_enc[i] != INVALID_RHANDLE)
		{
			if (0 < readDeviceValue(_enc[i], &val, 4))
				_q[i] = (double)val;
		}

		if (_motor[i] != INVALID_RHANDLE)
		{
			if (0 < monitorDeviceValue(_motor[i], &val, 4))
				_cur_des[i] = (double)val;
		}
	}

	if (_arhs != INVALID_RHANDLE)
	{
		readDeviceValue(_arhs, _rpy, 3*sizeof(float), myAHRSData_ACC);  // myAHRSData_ATTI
	}

	// read message from bluetooth
	if(_remoteTP_BT != INVALID_RHANDLE)
	{
		int szRead = readDeviceValue(_remoteTP_BT, &_msgRTP, _sz2read_msgRTP, DEVPORT_FETCH_MESSAGE);
		if(szRead == _sz2read_msgRTP && szRead > 0)
		{
#ifdef DEBUG_COMM
			printf("type : %d, size : %d, cmd : %d, data : %d\n", _msgRTP.type, _msgRTP.size, _msgRTP.cmd, _msgRTP.data);
#endif
			if(_msgRTP.type == RTP_TYPE_COMMAND_EVENT)
			{
				switch(_msgRTP.cmd)
				{
				case RTP_CMD_CMD1:
					command(CMD_MOTION_READY);
					break;

				case RTP_CMD_CMD2:
					break;

				case RTP_CMD_CMD3:
					command(CMD_MOTION_RUN);
					break;

				case RTP_CMD_CMD4:
					command(CMD_MOTION_STOP);
					break;

				case RTP_CMD_CMD5:
					command(CMD_MOTION_TURNL);
					break;

				case RTP_CMD_CMD6:
					command(CMD_MOTION_TURNR);
					break;

				case RTP_CMD_CMD7:
					break;

				case RTP_CMD_CMD8:
					command(CMD_SPEED_DN);
					break;

				case RTP_CMD_CMD9:
					command(CMD_SPEED_UP);
					break;
				}
			}
		}
	} // bluetooth
}

void control_AllegroDog::_writeDevices()
{
	AllegroDog_MotorCommand_t cmd;
	for (int i=0; i<JDOF; i++)
	{
		cmd.q_des = (float)_q_des[i];
		cmd.qdot_des = (float)_qdot_des[i];
		cmd.tau_des = (float)_tau_des[i];
		if (i==0 || i==3 || i==6 || i==9)						// Le Added
		{
			if (_q[i] > M_PI/10) cmd.tau_des = (float) 0.0;
			else if (_q[i] < -M_PI/10) cmd.tau_des = (float) 0.0;
		}
		if (i==2 || i==5 || i==8 || i==11)						// Le Added
		{
			if (_q[i] > M_PI*3/4) cmd.tau_des = (float) 0.0;
			else if (_q[i] < -M_PI*3/4) cmd.tau_des = (float) 0.0;
		}
		else
		{
			if (_q[i] > M_PI/2) cmd.tau_des = (float) 0.0;
			else if (_q[i] < -M_PI/2) cmd.tau_des = (float) 0.0;
		}
		if (_motor[i] != INVALID_RID)
		{
			if (eTEST_MODE_WALK == _test_mode)
				writeDeviceValue(_motor[i], &(cmd.tau_des), sizeof(float));
			else
				writeDeviceValue(_motor[i], &cmd, sizeof(AllegroDog_MotorCommand_t));
		}
	}
}

void control_AllegroDog::_reflect()
{
}

void control_AllegroDog::_compute(const double& t)
{
	switch (_test_mode)
	{
	case eTEST_MODE_WALK:
		{
			if (!_alg) break;
			_alg->SetJointPosition(_q.array);
			_alg->SetBodyPosture(_rpy[0], _rpy[1], _rpy[2], 0.0f);
			_alg->UpdateControl(t);
			_alg->GetFKResult(_x, _y, _z);
			_alg->GetJointDesiredPosition(_q_des.array);
			_alg->GetJointTorque(_tau_des.array);
		}
		break;
	case eTEST_MODE_CONST_TORQUE:
		{
			_tau_des.zero();
			_tau_des[_jid_test] = 200.0f / 800.0f;
		}
		break;
	case eTEST_MODE_POSITION_SIN:
		{
			_q_des = _test_traj_offset;
			_q_des[_jid_test] += _test_traj_amp * sin(2.0*M_PI*_test_traj_freq*(_curTime-_test_time_start)) * sin(2.0*M_PI*_test_traj_freq*(_curTime-_test_time_start));
		}
		break;
	case eTEST_MODE_TORQUE_SIN:
		{
			// NOT IMPLEMENTED !!!
			_tau_des.zero();
		}
		break;
	default:
		_tau_des.zero();
		break;
	}
}

void control_AllegroDog::_estimate()
{
	if (_filter_derivative)
		_filter_derivative->compute(_q, _qdot);
	if (_filter_derivative_des)
		_filter_derivative_des->compute(_q_des, _qdot_des);
}

int control_AllegroDog::command(const short& cmd, const int& arg)
{
	switch (cmd)
	{
	case CMD_TEST_MODE_WALK:
		_test_mode = eTEST_MODE_WALK;
		break;

	case CMD_TEST_MODE_CONST_TORQUE:
		_test_mode = eTEST_MODE_CONST_TORQUE;
		break;

	case CMD_TEST_MODE_POSITION_SIN:
		_test_mode = eTEST_MODE_POSITION_SIN;
		_test_traj_amp = 1.0;
		_test_traj_freq = 0.5;
		_test_time_start = _curTime;
		_test_traj_offset = _q;
		break;

	case CMD_TEST_MODE_TORQUE_SIN:
		_test_mode = eTEST_MODE_TORQUE_SIN;
		_test_time_start = _curTime;
		_test_traj_offset = _q;
		break;

	case CMD_TEST_MODE_DISABLE:
		_test_mode = eTEST_MODE_DISABLE;
		break;

	case CMD_MOTION_READY:
		if (_alg) _alg->SetMotionType(eMotionType_READY);
		break;

	case CMD_MOTION_DN:
		if (_alg)
		{
			_alg->SetMotionType(eMotionType_DN);
		}
		break;

	case CMD_MOTION_UP:
		if (_alg)
		{
			_alg->SetMotionType(eMotionType_UP);
		}
		break;

	case CMD_MOTION_RUN:
		if (_alg) _alg->SetMotionType(eMotionType_WALK);
		if (_alg) _alg->SetAngelRef(_rpy[0], _rpy[1], _rpy[2]);
		break;

	case CMD_MOTION_BACK:
		if (_alg) _alg->SetMotionType(eMotionType_BACK);
		if (_alg) _alg->SetAngelRef(_rpy[0], _rpy[1], _rpy[2]);
		break;

	case CMD_MOTION_STOP:
		if (_alg) _alg->SetMotionType(eMotionType_STOP);
		break;

	case CMD_MOTION_HOP:
		if (_alg) _alg->SetMotionType(eMotionType_HOP);
		if (_alg) _alg->SetAngelRef(_rpy[0], _rpy[1], _rpy[2]);
		break;

	case CMD_MOTION_TURNL:
		if (_alg) _alg->SetMotionType(eMotionType_LEFT);
		if (_alg) _alg->SetAngelRef(_rpy[0], _rpy[1], _rpy[2]);
		break;

	case CMD_MOTION_TURNR:
		if (_alg) _alg->SetMotionType(eMotionType_RIGHT);
		if (_alg) _alg->SetAngelRef(_rpy[0], _rpy[1], _rpy[2]);
		break;

	case CMD_SPEED_UP:
	//	if (_alg) _alg->SetSpeedArg(1);
		break;

	case CMD_SPEED_DN:
	//	if (_alg) _alg->SetSpeedArg(-1);
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

void control_AllegroDog::datanames(vector<string_type>& names, int channel)
{
	TCHAR tmp[32];

	switch (channel)
	{
	case PLOT_Q:
			for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("q%d(deg)"), i);
				names.push_back(tmp);
			}

			for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("q_des%d(deg)"), i);
				names.push_back(tmp);
			}
		break;

	case PLOT_QDOT:
		{
			for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("qdot%d(deg/sec)"), i);
				names.push_back(tmp);
			}

			for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("qdot_des%d(deg/sec)"), i);
				names.push_back(tmp);
			}
		}
		break;

	case PLOT_TORQUE:
		{
			for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("tau_des%d(Nm)"), i);
				names.push_back(tmp);
			}

			for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("cur_des%d(A)"), i);
				names.push_back(tmp);
			}

			/*for (int i=0; i<_jdof; i++) 
			{
				_stprintf_s(tmp, 32, _T("Power%d"), i);
				names.push_back(tmp);
			}*/
		}
		break;

	case PLOT_AHRS:
		{
			for (int i=0; i<6; i++) 
			{
				_stprintf_s(tmp, 32, _T("ARHS%d(deg)"), i);
				names.push_back(tmp);
			}
		}
		break;
	}
}

void control_AllegroDog::collect(vector<double>& data, int channel)
{
	switch (channel)
	{
	case PLOT_Q:
		{
			for (int i=0; i<_jdof; i++) 
				data.push_back(_q[i]*RADIAN);

			for (int i=0; i<_jdof; i++) 
				data.push_back(_q_des[i]*RADIAN);
			//data.push_back(_q[2]*RADIAN);
			//data.push_back(_q_des[2]*RADIAN);
		}
		break;

	case PLOT_QDOT:
		{
			for (int i=0; i<_jdof; i++) 
				//data.push_back(_x[i]); _qdot
				data.push_back(_qdot[i]*RADIAN); 

			for (int i=0; i<_jdof; i++) 
				//data.push_back(_x[i]); _qdot
				data.push_back(_qdot_des[i]*RADIAN); 
		}
		break;

	case PLOT_TORQUE:
		{
			for (int i=0; i<_jdof; i++) 
				data.push_back(_tau_des[i]);

			for (int i=0; i<_jdof; i++) 
				data.push_back(_cur_des[i]);

			/*for (int i=0; i<_jdof; i++) 
				data.push_back(_tau_des[i]*_qdot[i]);*/
		}
		break;

	case PLOT_AHRS:
		{
			data.push_back(_rpy[0]);
			data.push_back(_rpy[1]);
			data.push_back(_rpy[2]);
		}
		break;
	}
}

void control_AllegroDog::onSetInterestFrame(const TCHAR* name, const HTransform& T)
{
}

rControlAlgorithm* CreateControlAlgorithm(rDC& rdc)
{
	return new control_AllegroDog(rdc);
}
