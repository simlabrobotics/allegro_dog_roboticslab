#include "AllegroDogWalk.h"
#include "memory.h"
#define _USE_MATH_DEFINES
#include "math.h"
#ifdef WIN32
#define _USE_MATH_DEFINES
#include "math.h"
#endif

AllegroDogWalk::AllegroDogWalk(eDogType type)
{
	_dT = 0.002;
	_dogType = type;
	_motionType = eMotionType_NONE;

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
	_half_of_body_length = BX;
	_half_of_body_width = BY;

	_kp_balance = 0.0;
	_kd_balance = 0.0;
	_kp_turn = 0.0;
	
	memset(_q, 0, sizeof(_q));
	memset(_q_filtered, 0, sizeof(_q_filtered));
	memset(_q_pre, 0, sizeof(_q_pre));
	memset(_q_filtered_pre, 0, sizeof(_q_filtered_pre));
	memset(_qdot, 0, sizeof(_qdot));
	memset(_qdot_filtered, 0, sizeof(_qdot_filtered));
	memset(_qdot_pre, 0, sizeof(_qdot_pre));
	memset(_tau_des, 0, sizeof(_tau_des));
	memset(_q_des, 0, sizeof(_q_des));

	memset(_err_dot, 0, sizeof(_err_dot));
	memset(_err, 0, sizeof(_err));
	memset(_err_old, 0, sizeof(_err_old));

	memset(_x, 0, sizeof(_x));
	memset(_y, 0, sizeof(_y));
	memset(_z, 0, sizeof(_z));
	memset(_x_filtered, 0, sizeof(_x_filtered));
	memset(_y_filtered, 0, sizeof(_y_filtered));
	memset(_z_filtered, 0, sizeof(_z_filtered));
	memset(_x_pre, 0, sizeof(_x_pre));
	memset(_y_pre, 0, sizeof(_y_pre));
	memset(_z_pre, 0, sizeof(_z_pre));
	memset(_x_filtered_pre, 0, sizeof(_x_filtered_pre));
	memset(_y_filtered_pre, 0, sizeof(_y_filtered_pre));
	memset(_z_filtered_pre, 0, sizeof(_z_filtered_pre));

	memset(_xdot, 0, sizeof(_xdot));
	memset(_ydot, 0, sizeof(_ydot));
	memset(_zdot, 0, sizeof(_zdot));
	memset(_xdot_filtered, 0, sizeof(_xdot_filtered));
	memset(_ydot_filtered, 0, sizeof(_ydot_filtered));
	memset(_zdot_filtered, 0, sizeof(_zdot_filtered));
	memset(_xdot_pre, 0, sizeof(_xdot_pre));
	memset(_ydot_pre, 0, sizeof(_ydot_pre));
	memset(_zdot_pre, 0, sizeof(_zdot_pre));
	
	memset(_kp, 0, sizeof(_kp));
	memset(_ki, 0, sizeof(_ki));
	memset(_kd, 0, sizeof(_kd));

	memset(_x_des, 0, sizeof(_x_des));
	memset(_y_des, 0, sizeof(_y_des));
	memset(_z_des, 0, sizeof(_z_des));

	_turningArg = 0;
	
	_runTime = 0.0;
	_strideLength = 0.0;
	_stepHeight = 0.0;
	_turningMagnitude = 0.0;
	_rotationMagnitude= 0.0;

	// Set positions of 4 legs at Homing position.
	_footPosition[0].x= _half_of_body_length;	_footPosition[0].y= _half_of_body_width;	_footPosition[0].z=-_L0-_L1-_L2;
	_footPosition[1].x= _half_of_body_length;	_footPosition[1].y=-_half_of_body_width;	_footPosition[1].z=-_L0-_L1-_L2;
	_footPosition[2].x=-_half_of_body_length;	_footPosition[2].y= _half_of_body_width;	_footPosition[2].z=-_L0-_L1-_L2;
	_footPosition[3].x=-_half_of_body_length;	_footPosition[3].y=-_half_of_body_width;	_footPosition[3].z=-_L0-_L1-_L2;

	// Estimate _q_des at this stage {_q_des should be ZERO here}.
	SolveIK();

	for (int i=0;i<NOL;i++)
	{
		_stopDeepen[i] = _home_position_height;
		_desiredHeight[i] = _home_position_height;
	}
}

AllegroDogWalk::~AllegroDogWalk(void)
{
}

eDogType AllegroDogWalk::GetType()
{
	return _dogType;
}

void AllegroDogWalk::SetTimeInterval(double dT)
{
	_dT = dT;
}

void AllegroDogWalk::SetTurningArg(int turningArg)
{
//	_turningArg += turningArg;
}

void AllegroDogWalk::SetSpeedArg(int speedArg)
{
//	_stride_freq += 0.1*speedArg;
//	if (_stride_freq > 2.0) _stride_freq = 2.0;
//	if (_stride_freq < 1.0) _stride_freq = 1.0;
}

void AllegroDogWalk::SetMotionType(int motionType)
{
	_curT = 0;
	_motionType = (eMotionType)motionType;
}

void AllegroDogWalk::SetAngelRef(float row_ref, float pitch_ref, float yaw_ref)
{
	_rpy_Ref[0] = row_ref;
	_rpy_Ref[1] = pitch_ref;
	_rpy_Ref[2] = yaw_ref;
}

void AllegroDogWalk::SetJointPosition(double* q)
{
	memcpy(_q_pre, _q, SIZEOF_VARRAY);
	memcpy(_q, q, SIZEOF_VARRAY);
	memcpy(_q_filtered_pre, _q_filtered, SIZEOF_VARRAY);
	memcpy(_qdot_pre, _qdot, SIZEOF_VARRAY);
	for (int i=0; i<NOL; i++) 
	{
		for (int j=0; j<NOJ; j++) 
		{
			_q_filtered[i][j] = (0.6*_q_filtered[i][j]) + (0.198*_q_pre[i][j]) + (0.198*_q[i][j]);
			_qdot[i][j] = (_q[i][j]-_q_pre[i][j]) / _dT;
			_qdot_filtered[i][j] = (0.6*_qdot_filtered[i][j]) + (0.198*_qdot_pre[i][j]) + (0.198*_qdot[i][j]);
		}
	}
}

void AllegroDogWalk::SetBodyPosture(double roll, double pitch, double yaw, double droll)
{
/*	_pitchAngle = pitch;
	_rollAngle = roll;
	_yawAngle = yaw;
	_dRollAngle = droll; */
	_yawAngle = yaw;
}

void AllegroDogWalk::UpdateControl(double time)
{
	_curT += _dT;

	switch (_motionType)
	{
	case eMotionType_HOME:
	//	Motion_HomePosition();
		return;

	case eMotionType_READY:						/// Move the legs to READY stage
		Motion_Ready();
		break;

	case eMotionType_READY_UP:					/// Move the body base up
		Motion_Ready_Up();
		break;

	/// Stop the movement if it does
	/// Set the height desire before calling Move_Up function
	case eMotionType_UP:						
		_turningMagnitude=0.0; 
		if (_strideLength > 0)
		{
			_strideLength -= (_stride_step_inc_X);
			if(_strideLength<(0))
			{
				_strideLength = (0);
				_rotationMagnitude=0.0; //0.02

				_stepHeight -= _stride_step_inc_X;
				if(_stepHeight<(0))
				{
					_stepHeight = (0);
				}
			}
		}
		else
		{
			_strideLength += (_stride_step_inc_X);
			if(_strideLength>(0))
			{
				_strideLength = (0);
				_rotationMagnitude=0.0; //0.02

				_stepHeight -= _stride_step_inc_X;
				if(_stepHeight<(0))
				{
					_stepHeight = (0);
				}
			}
		}
		
		Motion_Stop();
		if (_stepHeight == 0)
		{
			if ((_home_position_height) > -0.49)
			{
				_home_position_height -= 0.015;
				_desiredHeight[0] -= 0.015;
				_desiredHeight[1] -= 0.015;
				_desiredHeight[2] -= 0.015;
				_desiredHeight[3] -= 0.015;
				_stride_step_height -= 0.003;
				_motionType = eMotionType_READY_UP;
			}
		}
		break;

	/// Stop the movement if it does
	/// Set the height desire before calling Move_Down function
	case eMotionType_DN:
		_turningMagnitude=0.0; 
		if (_strideLength > 0)
		{
			_strideLength -= (_stride_step_inc_X);
			if(_strideLength<(0))
			{
				_strideLength = (0);
				_rotationMagnitude=0.0; 

				_stepHeight -= _stride_step_inc_X;
				if(_stepHeight<(0))
				{
					_stepHeight = (0);
				}
			}
		}
		else
		{
			_strideLength += (_stride_step_inc_X);
			if(_strideLength>(0))
			{
				_strideLength = (0);
				_rotationMagnitude=0.0; 

				_stepHeight -= _stride_step_inc_X;
				if(_stepHeight<(0))
				{
					_stepHeight = (0);
				}
			}
		}
		
		Motion_Stop();
		if (_stepHeight == 0)
		{
			if ((_home_position_height) < -0.415)
			{
				_home_position_height += 0.015;
				_desiredHeight[0] += 0.015;
				_desiredHeight[1] += 0.015;
				_desiredHeight[2] += 0.015;
				_desiredHeight[3] += 0.015;
				_stride_step_height += 0.003;
				_motionType = eMotionType_READY;
			}
		}
		break;

	/// Height step increased by height_step unit at every tick
	/// Length step increased by length_step unit at every tick
	/// Turing magnitude estimated via yaw angle
	/// Motion_Walk function calls Oscillator function to calculate the update position at end point of the legs
	case eMotionType_WALK:
		_stepHeight += _stride_step_inc_Z;
		if(_stepHeight > _stride_step_height)
		{
			_stepHeight = _stride_step_height;
		}

		if (_rpy_Ref[2] > 90 || _rpy_Ref[2] < -90)		// Near the 180 or -180 degree !~
		{
			if (_rpy_Ref[2] > 0 && _yawAngle < 0)		// The sign change form (+) to (-)
			{
				_turningMagnitude= -(_kp_turn) * _half_of_body_length * sin((_yawAngle + 180 + 180 - _rpy_Ref[2])*M_PI/180);
			}
			else if (_rpy_Ref[2] < 0 && _yawAngle > 0) // The sign change form (-) to (+)
			{
				_turningMagnitude= -(_kp_turn) * _half_of_body_length * sin((_yawAngle - 180 - 180 - _rpy_Ref[2])*M_PI/180);
			}
			else									   // The sign change doesnt change !~
			{
				_turningMagnitude= -(_kp_turn) * _half_of_body_length * sin((_yawAngle - _rpy_Ref[2])*M_PI/180);
			}
		}
		else
		{
			_turningMagnitude= -(_kp_turn) * _half_of_body_length * sin((_yawAngle - _rpy_Ref[2])*M_PI/180);
		}
		_turningMagnitude=0;

	//	_rotationMagnitude-=_stride_step_inc_X;
	//	if(_rotationMagnitude<=0) _rotationMagnitude=0;

		Motion_Walk();
		_strideLength += _stride_step_inc_X;
		if(_strideLength>=_stride_step_length) _strideLength = _stride_step_length;
		break;

	case eMotionType_RUN:
	//	Motion_Run();
		return;

	/// Height step increased by height_step unit at every tick
	/// Length step increased by length_step unit at every tick
	/// Turing magnitude estimated via yaw angle
	/// In practice, the dog tends to move backward while doing Hoping, we make it move forward a little to compensate
	/// Motion_Hop function calls Oscillator function to calculate the update position at end point of the legs
	case eMotionType_HOP:
		_stepHeight += _stride_step_inc_Z;
		if(_stepHeight > _stride_step_height)
		{
			_stepHeight = _stride_step_height;
		}

		if (_rpy_Ref[2] > 90 || _rpy_Ref[2] < -90)		// Near the 180 or -180 degree !~
		{
			if (_rpy_Ref[2] > 0 && _yawAngle < 0)		// The sign change form (+) to (-)
			{
				_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle + 180 + 180 - _rpy_Ref[2])*M_PI/180);
			}
			else if (_rpy_Ref[2] < 0 && _yawAngle > 0) // The sign change form (-) to (+)
			{
				_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle - 180 - 180 - _rpy_Ref[2])*M_PI/180);
			}
			else									   // The sign change doesnt change !~
			{
				_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle - _rpy_Ref[2])*M_PI/180);
			}
		}
		else
		{
			_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle - _rpy_Ref[2])*M_PI/180);
		}

		_strideLength -= (_stride_step_inc_X);
		if(_strideLength<(100*_stride_step_inc_X /*0*/))
		{
			_strideLength = (100*_stride_step_inc_X /*0*/);
			_rotationMagnitude=0.0; //0.02
		}
		Motion_Hop();
		break;

	/// Height step increased by height_step unit at every tick
	/// Length step increased by length_step unit at every tick
	/// Turing magnitude estimated via yaw angle
	/// In practice, the dog tends to move backward while doing Hoping, we make it move forward a little to compensate
	/// Motion_Hop function calls Oscillator function to calculate the update position at end point of the legs
	case eMotionType_LEFT:
		_stepHeight += _stride_step_inc_Z;
		if(_stepHeight > _stride_step_height)
		{
			_stepHeight = _stride_step_height;
		}

		if (_rpy_Ref[2] < -90)		// Near the 180 or -180 degree !~
		{
			if (_rpy_Ref[2] < 0 && _yawAngle > 0) // The sign change form (-) to (+)
			{
				if ((_yawAngle - 180 - 180 - _rpy_Ref[2]) < -15) 
				{
					_turningMagnitude = 0; 
					_motionType = eMotionType_HOP;
					SetAngelRef(0.0f, 0.0f, _yawAngle);
				}
				else _turningMagnitude = -0.025;
			}
			else									   // The sign change doesnt change !~
			{
				if ((_yawAngle - _rpy_Ref[2]) < -15) 
				{
					_turningMagnitude = 0; 
					_motionType = eMotionType_HOP;
					SetAngelRef(0.0f, 0.0f, _yawAngle); 
				}
				else _turningMagnitude = -0.025;
			}
		}
		else
		{
			if ((_yawAngle - _rpy_Ref[2]) < -15) 
			{
				_turningMagnitude = 0; 
				_motionType = eMotionType_HOP;
				SetAngelRef(0.0f, 0.0f, _yawAngle); 
			}
			else _turningMagnitude = -0.025;
		} 
		_strideLength -= (_stride_step_inc_X);
		if(_strideLength<(100*_stride_step_inc_X))
		{
			_strideLength = (100*_stride_step_inc_X);
		}
		Motion_Hop();
		break;

	/// Height step increased by height_step unit at every tick
	/// Length step increased by length_step unit at every tick
	/// Turing magnitude estimated via yaw angle
	/// In practice, the dog tends to move backward while doing Hoping, we make it move forward a little to compensate
	/// Motion_Hop function calls Oscillator function to calculate the update position at end point of the legs
	case eMotionType_RIGHT:
		_stepHeight += _stride_step_inc_Z;
		if(_stepHeight > _stride_step_height)
		{
			_stepHeight = _stride_step_height;
		}

		if (_rpy_Ref[2] > 90)		// Near the 180 or -180 degree !~
		{
			if (_rpy_Ref[2] > 0 && _yawAngle < 0)		// The sign change form (+) to (-)
			{
				if ((_yawAngle + 180 + 180 - _rpy_Ref[2]) > 15) 
				{
					_turningMagnitude = 0; 
					_motionType = eMotionType_HOP;
					SetAngelRef(0.0f, 0.0f, _yawAngle);
				}
				else _turningMagnitude = 0.025;
			}
			else									   // The sign change doesnt change !~
			{
				if ((_yawAngle - _rpy_Ref[2]) > 15) 
				{
					_turningMagnitude = 0; 
					_motionType = eMotionType_HOP;
					SetAngelRef(0.0f, 0.0f, _yawAngle); 
				}
				else _turningMagnitude = 0.025;
			}
		}
		else
		{
			if ((_yawAngle - _rpy_Ref[2]) > 15) 
			{
				_turningMagnitude = 0; 
				_motionType = eMotionType_HOP;
				SetAngelRef(0.0f, 0.0f, _yawAngle); 
			}
			else _turningMagnitude = 0.025;
		}
		_strideLength -= (_stride_step_inc_X);
		if(_strideLength<(100*_stride_step_inc_X))
		{
			_strideLength = (100*_stride_step_inc_X);
		}
		Motion_Hop();
		break;

	/// Height step increased by height_step unit at every tick
	/// Length step decreased by length_step unit at every tick
	/// Turing magnitude estimated via yaw angle
	/// In practice, the dog tends to move backward while doing Hoping, we make it move forward a little to compensate
	/// Motion_Hop function calls Oscillator function to calculate the update position at end point of the legs
	case eMotionType_BACK:
		_stepHeight += _stride_step_inc_Z;
		if(_stepHeight > _stride_step_height)
		{
			_stepHeight = _stride_step_height;
		}
		_rotationMagnitude-=_stride_step_inc_X;
		if(_rotationMagnitude<=0) _rotationMagnitude=0;

		if (_rpy_Ref[2] > 90 || _rpy_Ref[2] < -90)		// Near the 180 or -180 degree !~
		{
			if (_rpy_Ref[2] > 0 && _yawAngle < 0)		// The sign change form (+) to (-)
			{
				_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle + 180 + 180 - _rpy_Ref[2])*M_PI/180);
			}
			else if (_rpy_Ref[2] < 0 && _yawAngle > 0) // The sign change form (-) to (+)
			{
				_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle - 180 - 180 - _rpy_Ref[2])*M_PI/180);
			}
			else									   // The sign change doesnt change !~
			{
				_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle - _rpy_Ref[2])*M_PI/180);
			}
		}
		else
		{
			_turningMagnitude= -_kp_turn * _half_of_body_length * sin((_yawAngle - _rpy_Ref[2])*M_PI/180);
		}
		_turningMagnitude=0;
		
		Motion_Walk();
		_strideLength -= _stride_step_inc_X;
		if(_strideLength <= -_stride_step_length) _strideLength = -_stride_step_length;
		break;

	/// Length step decreased by length_step unit at every tick
	/// After that, Height step decreased by height_step unit at every tick
	/// In practice, the dog tends to move backward while doing Hoping, we make it move forward a little to compensate
	/// Motion_Stop function calls Oscillator function to calculate the update position at end point of the legs
	case eMotionType_STOP:
		_turningMagnitude=0.0; 
		if (_strideLength > 0)
		{
			_strideLength -= (_stride_step_inc_X);
			if(_strideLength<(0))
			{
				_strideLength = (0);
				_rotationMagnitude=0.0; 
				_stepHeight -= _stride_step_inc_X;
				if(_stepHeight<(0))
				{
					_stepHeight = (0);
				}
			}
		}
		else
		{
			_strideLength += (_stride_step_inc_X);
			if(_strideLength>(0))
			{
				_strideLength = (0);
				_rotationMagnitude=0.0; 
				_stepHeight -= _stride_step_inc_X;
				if(_stepHeight<(0))
				{
					_stepHeight = (0);
				}
			}
		}
		
		Motion_Stop();
		break;

	case eMotionType_NONE:
	default:
		break;
	}

	// Estimate _q_des currently
	SolveIK();

	// PID control
	for (int i=0; i<NOL; i++) {
		for (int j=0; j<NOJ; j++) {
			_err[i][j]=(_q_des[i][j])-(_q[i][j]);
			_err_dot[i][j]=(_err[i][j]-_err_old[i][j])/_dT;
			_tau_des[i][j] =_kp[i][j]*_err[i][j]+ _kd[i][j]*_err_dot[i][j];

			// Limit the max & min of desired torque
			if (_tau_des[i][j] > 8.686) _tau_des[i][j] = 8.686;
			if (_tau_des[i][j] < -8.686) _tau_des[i][j] = -8.686;
			_err_old[i][j]=_err[i][j];
		}
	}
}

void AllegroDogWalk::Motion_HomePosition()
{
	memset(_tau_des, 0, sizeof(_tau_des));
}

/// Estimate the z position of the end point of legs {z position increases continously}
void AllegroDogWalk::Motion_Ready()
{
	for (int i=0; i<NOL; i++)
	{
		if(_footPosition[i].z<_home_position_height)	_footPosition[i].z+=0.2*_stride_step_inc_Z;
	}
}

/// Estimate the z position of the end point of legs {z position decreases continously}
void AllegroDogWalk::Motion_Ready_Up()
{
	for (int i=0; i<NOL; i++)
	{
		if(_footPosition[i].z>_home_position_height)	_footPosition[i].z-=0.2*_stride_step_inc_Z;
	}
}

/// Motion_Walk: calls Oscillator function to calculate the update position at end point of the legs
void AllegroDogWalk::Motion_Walk()
{
	_runTime += _dT;
	//ControlDirectionSelfRotationMethod(0);	
	//ControlDirectionCurveMethod(0);
	Oscillator();
	//BalanceModule();
		
	if(_runTime>=(1./_stride_freq))		// reset all to prepare for new phase
	{
		for (int i=0;i<NOL;i++) _stopDeepen[i]=_desiredHeight[i];
		_runTime = 0.;	
	}
}

void AllegroDogWalk::Motion_Run()
{
	memset(_tau_des, 0, sizeof(_tau_des));
}

/// Motion_Hop: calls Oscillator function to calculate the update position at end point of the legs
/// Length step decreased by length_step unit at every tick
void AllegroDogWalk::Motion_Hop()
{
	_runTime += _dT;
		
	Oscillator();
	//BalanceModule();
	
	if(_runTime>=(1./_stride_freq))		// reset all to prepare for new phase
	{
		for (int i=0;i<NOL;i++)
			_stopDeepen[i]=_desiredHeight[i];
		_runTime = 0.0;
	}

	_strideLength -= _stride_step_inc_X;
	if(_strideLength<(100*_stride_step_inc_X /*0*/))
	{
		_strideLength = (100*_stride_step_inc_X /*0*/);
		//ControlDirectionSelfRotationMethod(1);
	}
}

/// Motion_Stop: calls Oscillator function to calculate the update position at end point of the legs
/// Length step decreased by length_step unit at every tick
/// After that, Height step decreased by height_step unit at every tick
void AllegroDogWalk::Motion_Stop()
{
	_runTime += _dT;
		
	Oscillator();
	//BalanceModule();
	
	if(_runTime>=(1./_stride_freq))		// reset all to prepare for new phase
	{
		for (int i=0;i<NOL;i++)
			_stopDeepen[i]=_desiredHeight[i];
		_runTime = 0.0;
	}

	if (_strideLength > 0)
	{
		_strideLength -= _stride_step_inc_X;
		if(_strideLength<(0))
		{
			_strideLength = (0);

			_stepHeight -= _stride_step_inc_Z;
			if(_stepHeight<(0))
			{
				_stepHeight = (0);
				_runTime = 0.0;
			}
		}
	}
	else
	{
		_strideLength += _stride_step_inc_X;
		if(_strideLength > (0))
		{
			_strideLength = (0);

			_stepHeight -= _stride_step_inc_Z;
			if(_stepHeight<(0))
			{
				_stepHeight = (0);
				_runTime = 0.0;
			}
		}
	}
}

/// repeated motion of x-, y-, z- position to generate desired trajectory of legs
void AllegroDogWalk::Oscillator()
{
	//start of oscillator

	//motion of y- position of legs
	_footPosition[0].y = _half_of_body_width+_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);
	_footPosition[1].y =-_half_of_body_width-_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);
	_footPosition[2].y = _half_of_body_width+_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);
	_footPosition[3].y =-_half_of_body_width-_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);

	//motion of x- position of legs
	_footPosition[0].x = _home_position_front - (_strideLength+_turningMagnitude)*cos(2.*M_PI*_stride_freq*_runTime);
	_footPosition[1].x = _home_position_front + (_strideLength-_turningMagnitude)*cos(2.*M_PI*_stride_freq*_runTime);
	_footPosition[2].x = _home_position_hind  + (_strideLength+_turningMagnitude)*cos(2.*M_PI*_stride_freq*_runTime);
	_footPosition[3].x = _home_position_hind  - (_strideLength-_turningMagnitude)*cos(2.*M_PI*_stride_freq*_runTime);

	//motion of z- position of legs
	//to accomodate with inclination, desired height of each leg is different

	if(_runTime<=(0.5/_stride_freq))
	{
		_footPosition[0].z = _desiredHeight[0] + _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
		_footPosition[1].z = _desiredHeight[1] - _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
		_footPosition[2].z = _desiredHeight[2] - _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
		_footPosition[3].z = _desiredHeight[3] + _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
	} 
	if(_runTime>(0.5/_stride_freq))
	{
		_footPosition[0].z = _desiredHeight[0] + _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
		_footPosition[1].z = _desiredHeight[1] - _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
		_footPosition[2].z = _desiredHeight[2] - _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
		_footPosition[3].z = _desiredHeight[3] + _stepHeight*sin(2.*M_PI*_stride_freq*_runTime)	;
	}

	double adapt_gain1,adapt_gain2;
	//suppose to be 1 and 1, if these values should be tuned in real application, they will be added to .XDL
	adapt_gain1=0.07; // 0.07
	adapt_gain2=0.07;

	_stopDeepen[0] = _desiredHeight[0] + adapt_gain1*_half_of_body_length*sin(_rollAngle) - adapt_gain2*_half_of_body_width*sin(_pitchAngle);
	_stopDeepen[1] = _desiredHeight[1] - adapt_gain1*_half_of_body_length*sin(_rollAngle) - adapt_gain2*_half_of_body_width*sin(_pitchAngle);
	_stopDeepen[2] = _desiredHeight[2] + adapt_gain1*_half_of_body_length*sin(_rollAngle) + adapt_gain2*_half_of_body_width*sin(_pitchAngle);
	_stopDeepen[3] = _desiredHeight[3] - adapt_gain1*_half_of_body_length*sin(_rollAngle) + adapt_gain2*_half_of_body_width*sin(_pitchAngle);
	
	for (int i=0;i<NOL;i++)
	{
		if (_stopDeepen[i] < _desiredHeight[i]-0.005)
			_stopDeepen[i]=_desiredHeight[i]-0.005;

		if (_footPosition[i].z <= _stopDeepen[i])
			_footPosition[i].z = _stopDeepen[i];	//sliding of foot
	}
}

void AllegroDogWalk::BalanceModule()
{
	//balance module
	float limit_angle;
	limit_angle=20*DEG2RAD;
	//_kp_balance=-0.173;
	//_kd_balance=-0.005;
	
	for (int i=0;i<NOL;i++)
	{			
		if (abs(_desiredAngle[i][0]) < limit_angle)
			_footPosition[i].y += _kp_balance*_rollAngle + _kd_balance*_dRollAngle;
	}
}

void AllegroDogWalk::ControlDirectionSelfRotationMethod(int left_right)
{
	//rotate method1: robot rotates around itselt
	//set GmagR negative to turn right, set GmagR positive to turn left
	if(left_right==0)	_rotationMagnitude=0.0;	
	if(left_right==1)	_rotationMagnitude=0.01;
	if(left_right==-1)	_rotationMagnitude=-0.01;

	_footPosition[0].y = _half_of_body_width+_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);
	_footPosition[1].y =-_half_of_body_width-_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);
	_footPosition[2].y = _half_of_body_width+_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);
	_footPosition[3].y =-_half_of_body_width-_rotationMagnitude*sin((M_PI)*_stride_freq*_runTime);

}

void AllegroDogWalk::ControlDirectionCurveMethod(double desired_angle)
{
	_turningMagnitude=-0.003;
}

void AllegroDogWalk::SolveFK()
{
	for (int i=0; i<NOL; i++)
	{
		_x_pre[i] = _x[i];
		_y_pre[i] = _y[i];
		_z_pre[i] = _z[i];
		_x_filtered_pre[i] = _x_filtered[i];
		_y_filtered_pre[i] = _y_filtered[i];
		_z_filtered_pre[i] = _z_filtered[i];
	}

	_x[0] =0.0;
	_y[0] =0.0;
	_z[0] =0.0;

	_x[1] =0.0;
	_y[1] =0.0;
	_z[1] =0.0;

	_x[2] =0.0;
	_y[2] =0.0;
	_z[2] =0.0;

	_x[3] =0.0;
	_y[3] =0.0;
	_z[3] =0.0;

	for (int i=0; i<NOL; i++)
	{
		_x_filtered[i] = (0.6f*_x_filtered[i]) + (0.198f*_x_pre[i]) + (0.198f*_x[i]);
		_y_filtered[i] = (0.6f*_y_filtered[i]) + (0.198f*_y_pre[i]) + (0.198f*_y[i]);
		_z_filtered[i] = (0.6f*_z_filtered[i]) + (0.198f*_z_pre[i]) + (0.198f*_z[i]);

		_xdot_pre[i] = _xdot[i];
		_ydot_pre[i] = _ydot[i];
		_zdot_pre[i] = _zdot[i];

		_xdot[i] = (_x_filtered[i] - _x_filtered_pre[i]) / _dT;
		_ydot[i] = (_y_filtered[i] - _y_filtered_pre[i]) / _dT;
		_zdot[i] = (_z_filtered[i] - _z_filtered_pre[i]) / _dT;

		_xdot_filtered[i] = (0.6f*_xdot_filtered[i]) + (0.198f*_xdot_pre[i]) + (0.198f*_xdot[i]);
		_ydot_filtered[i] = (0.6f*_ydot_filtered[i]) + (0.198f*_ydot_pre[i]) + (0.198f*_ydot[i]);
		_zdot_filtered[i] = (0.6f*_zdot_filtered[i]) + (0.198f*_zdot_pre[i]) + (0.198f*_zdot[i]);
	}
}

/// Solve IK to get desired angular angle
void AllegroDogWalk::SolveIK()
{
	int i;
	double s1[4], c1[4], s3[4], c3[4];
	double s4[4], c4[4], alpha4[4],alpha5[4];
	double temp[4];
	double sign1[NOL] = { -1.0, -1.0, 1.0, 1.0};
	double sign2[NOL] = { -1.0, -1.0, 1.0, 1.0};
	CPosition p[4];
	
	for (i=0;i<NOL;i++)
		p[i].z=_footPosition[i].z;

	p[0].x=_footPosition[0].x-_half_of_body_length; p[0].y=_footPosition[0].y-_half_of_body_width;
	p[1].x=_footPosition[1].x-_half_of_body_length; p[1].y=_footPosition[1].y+_half_of_body_width;
	p[2].x=_footPosition[2].x+_half_of_body_length; p[2].y=_footPosition[2].y-_half_of_body_width;
	p[3].x=_footPosition[3].x+_half_of_body_length; p[3].y=_footPosition[3].y+_half_of_body_width;

	for (i=0;i<NOL;i++)
	{
		_desiredAngle[i][0]=atan2(p[i].y,-p[i].z);
		s1[i]=sin(_desiredAngle[i][0]);
		c1[i]=cos(_desiredAngle[i][0]);
	
		c3[i]=(p[i].x*p[i].x + (p[i].z/c1[i]+_L0)*(p[i].z/c1[i]+_L0) -_L1*_L1-_L2*_L2)/(2*_L1*_L2);
		s3[i] = sqrt(1-c3[i]*c3[i]);
		_desiredAngle[i][2]=sign1[i]* atan2(s3[i], c3[i]);

		s4[i]=(_L2*s3[i])/sqrt((p[i].z/c1[i]-_L0)*(p[i].z/c1[i]-_L0)+p[i].x*p[i].x);
		c4[i]=(_L1+_L2*c3[i])/sqrt((p[i].z/c1[i]-_L0)*(p[i].z/c1[i]-_L0)+p[i].x*p[i].x);
		alpha4[i]=sign2[i]*atan2(s4[i], c4[i]);
		
		temp[i]=-_L0-p[i].z/c1[i];
		alpha5[i]=atan2(-p[i].x,temp[i]);
		_desiredAngle[i][1]=(alpha5[i]-alpha4[i]);

		_q_des[i][0] = _desiredAngle[i][0];
		_q_des[i][1] = _desiredAngle[i][1];
		_q_des[i][2] = _desiredAngle[i][2];
	}
}
void AllegroDogWalk::GetJointDesiredPosition(double* qd)
{
	memcpy(qd, _q_des, SIZEOF_VARRAY);
}
void AllegroDogWalk::GetJointTorque(double* tau)
{
	memcpy(tau, _tau_des, SIZEOF_VARRAY);
}

void AllegroDogWalk::GetFKResult(double x[4], double y[4], double z[4])
{
	memcpy(x, _x, sizeof(_x[0])*NOL);
	memcpy(y, _y, sizeof(_y[0])*NOL);
	memcpy(z, _z, sizeof(_z[0])*NOL);
}

void AllegroDogWalk::SetJointDesiredPosition(double* q)
{
	memcpy(_q_des, q, SIZEOF_VARRAY);
}

void AllegroDogWalk::SetGains(double* kp, double* ki, double* kd)
{
	memcpy(_kp, kp, SIZEOF_VARRAY);
	memcpy(_ki, ki, SIZEOF_VARRAY);
	memcpy(_kd, kd, SIZEOF_VARRAY);
}

void AllegroDogWalk::SetBalanceGain(double kp, double kd)
{
	_kp_balance = kp;
	_kd_balance = kd;
}

void AllegroDogWalk::SetTurnGain(double kp)
{
	_kp_turn = kp;
}

void AllegroDogWalk::SetKinematics(double l0, double l1, double l2, double bw, double bl)
{
	_L0 = l0;
	_L1 = l1;
	_L2 = l2;
	_half_of_body_width = bw*0.5;
	_half_of_body_length = bl*0.5;

	_footPosition[0].x= _half_of_body_length;	_footPosition[0].y= _half_of_body_width;	_footPosition[0].z=-_L0-_L1-_L2;
	_footPosition[1].x= _half_of_body_length;	_footPosition[1].y=-_half_of_body_width;	_footPosition[1].z=-_L0-_L1-_L2;
	_footPosition[2].x=-_half_of_body_length;	_footPosition[2].y= _half_of_body_width;	_footPosition[2].z=-_L0-_L1-_L2;
	_footPosition[3].x=-_half_of_body_length;	_footPosition[3].y=-_half_of_body_width;	_footPosition[3].z=-_L0-_L1-_L2;

	SolveIK();
}

void AllegroDogWalk::SetWalkingParameters(double stride_length, double stride_height, double stride_freq, double stride_inc_X, double stride_inc_Z)
{
	_stride_freq = stride_freq;
	_stride_step_height = stride_height;
	_stride_step_length = stride_length;
	_stride_step_inc_X = stride_inc_X;
	_stride_step_inc_Z = stride_inc_Z;
}

void AllegroDogWalk::SetDesiredHomePosition(double height, double front, double hind)
{
	_home_position_height = height;
	_home_position_front = front;
	_home_position_hind = hind;

	for (int i=0;i<NOL;i++)
	{
		_stopDeepen[i] = _home_position_height;
		_desiredHeight[i] = _home_position_height;
	}
}

ALLEGRODOGWALK_EXTERN_C_BEGIN

AllegroDogWalk* CreateAllegroDogWalk()
{
	return new AllegroDogWalk(eDogType_AllegroDog);
}

ALLEGRODOGWALK_EXTERN_C_END
