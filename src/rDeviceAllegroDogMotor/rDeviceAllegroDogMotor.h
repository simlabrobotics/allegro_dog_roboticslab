/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

#ifndef __RDEVICEALLEGRODOGMOTOR_H__
#define __RDEVICEALLEGRODOGMOTOR_H__

#include "rMath/rMath.h"
using namespace rMath;

#include "rDevice/rDeviceBase.h"

class rDeviceAllegroDogMotor : public RD_DEVICE_CLASS(Base)
{
	RD_VERSION(1.0)
	RD_AUTHOR(SimLab)
public:
	RD_DECLARE_CTOR(AllegroDogMotor);
	RD_DECLARE_DTOR(AllegroDogMotor);;

	RD_DECLARE_createDevice;
	RD_DECLARE_initDevice;
	RD_DECLARE_terminateDevice;
	RD_DECLARE_writeDeviceValue;
	RD_DECLARE_monitorDeviceValue;
	RD_DECLARE_updateWriteValue;
	RD_DECLARE_command;

private:
	void printProps();

private:
	int _rt_joint;
	TCHAR _systemDeviceName[MAX_PATH];
	rID _systemDevice;
	//rID _joint;// not necessary
	float* _deviceMem_pos;
	float* _deviceMem_vel;
	float* _deviceMem_cur;
	int*   _deviceMem_cmd;
	int*   _deviceMem_opmode;
	int*   _deviceMem_opmode_display;
	int*   _deviceMem_opstatus;
	int*   _deviceMem_faultstatus;

	int _modeOfOperation;

	float _tau_des; // user input desired torque.
	float _cur_des; // desired current limited by current lower and upper value.
	float _q_des; // user input desired position in radian
	float _pos_des; // desired
	float _qdot_des;

	float _current_upper;
	float _current_lower;
	float _torque_const;
	float _torque_const_inv;
	float _gear_ratio;
	float _gear_ratio_inv;
	float _current_to_pwm;
	int _direction;
};
#endif