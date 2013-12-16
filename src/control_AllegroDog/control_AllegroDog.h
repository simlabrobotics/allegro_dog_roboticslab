/* RoboticsLab, Copyright 2008-2010 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#ifndef __CONTROL_ALLEGRODOG_H__
#define __CONTROL_ALLEGRODOG_H__

#include <list>
#include "rControlAlgorithm/rControlAlgorithm.h"
#include "rxControlSDK/rxControlSDK.h"
#include "AllegroDogWalk/AllegroDogWalk.h"
#include "rDeviceAllegroDogRemoteCmd.h"

#define JDOF 12
#define MAX_JDOF 15
#define NUM_CAN_CH 2

class REXPORT control_AllegroDog : public rControlAlgorithmEx
{
public:
	control_AllegroDog(rDC rdc);
	~control_AllegroDog();

	virtual void init(int mode = 0);
	virtual void update(const rTime& t);
	virtual void setNominalSystem(const TCHAR* path, const TCHAR* aml, const HTransform& T0, const dVector& q0);
	virtual void setPeriod(const rTime& dT);
	virtual int command(const short& cmd, const int& arg = 0);
	virtual void datanames(vector<string_type>& names, int channel = -1);
	virtual void collect(vector<double>& data, int channel = -1);
	virtual void onSetInterestFrame(const TCHAR* name, const HTransform& T);

private:
	virtual void _estimate();
	virtual void _readDevices();
	virtual void _writeDevices();
	virtual void _reflect();
	virtual void _compute(const rTime& t);

	void _findDevices();
	void _servoOn();
	void _servoOff();

private:
	rTime				_dT;
	rTime				_curTime;

	rID					_remoteTP_BT; // Bluetooth
	// for remote control
	MessageRemoteTP_t	_msgRTP;
	int					_sz2read_msgRTP;

	AllegroDogWalk*		_alg;

	rHANDLE				_motor[JDOF];
	rHANDLE				_enc[JDOF];
	rHANDLE				_arhs;

	dVector				_q;
	dVector				_q_des;
	dVector				_err;
	dVector				_err_dot;
	dVector				_err_old;
	dVector				_qdot;
	dVector				_qdot_des;
	dVector				_tau_des;
	dVector				_cur_des;

	int					_jdof;

	double				_x[4];
	double				_y[4];
	double				_z[4];

	dVector				_kp;
	dVector				_ki;
	dVector				_kd;
	double				_kp_balance;
	double				_kd_balance;
	double				_kp_turn;

	double				_stride_freq;				///> frequency of stride
	double				_stride_step_height;
	double				_stride_step_length;
	double				_stride_step_inc_X;
	double				_stride_step_inc_Z;
	double				_home_position_height;
	double				_home_position_front;
	double				_home_position_hind;
	double				_L0;						///> length of the first link in a leg
	double				_L1;
	double				_L2;
	double				_BW;
	double				_BL;

	float				_rpy[3];

	rHANDLE				_sharedMemory;

	rxFilteredDerivative* _filter_derivative;
	rxFilteredDerivative* _filter_derivative_des;
	double				_filter_cutoff;

	////////////////////////////////////////////////////////
	// for testing motors
	int					_jid_test;
	int					_test_mode;
	enum eTEST_MODE
	{
		eTEST_MODE_WALK = 0,		// walking algorithm
		eTEST_MODE_CONST_TORQUE,	// constant torque
		eTEST_MODE_POSITION_SIN,	// sinusoidal trajectory(position)
		eTEST_MODE_TORQUE_SIN,		// sinusoidal trajectory(torque)
		eTEST_MODE_DISABLE = 0xff,	// disabled
	};
	double				_test_time_start;
	double				_test_traj_amp;
	double				_test_traj_freq;
	dVector				_test_traj_offset;
};

#endif
