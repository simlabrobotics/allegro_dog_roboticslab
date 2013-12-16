/* RoboticsLab, Copyright 2008-2010 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#ifndef __CONTROL_ALLEGRODOGHOME_H__
#define __CONTROL_ALLEGRODOGHOME_H__

#include "rControlAlgorithm/rControlAlgorithm.h"
#include "rxControlSDK/rxControlSDK.h"

#define JDOF 12
#define MAX_JDOF 15
#define NUM_CAN_CH 2

class REXPORT control_AllegroDogHome : public rControlAlgorithmEx
{
public:
	control_AllegroDogHome(rDC rdc);
	~control_AllegroDogHome();

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
	void _initHome();
	void _finalizeHome();
	void _commandHome(int jid);

private:
	rTime				_dT;
	rTime				_curTime;
	int					_jdof;

	rHANDLE				_motor[JDOF];
	rHANDLE				_enc[JDOF];
	rHANDLE				_can[NUM_CAN_CH];

	dVector				_q;

	int					_home_speed;
	int					_home_direction[JDOF];
	int					_home_offset[JDOF];
	int					_home_offset_tune[JDOF];
	enum eHomeState
	{
		HOME_INIT = 0,
		HOME_SUCCESS = 1,
		HOME_FAIL = 2,
		HOME_SEARCH_HOME = 4,
		HOME_HOME_SEARCHED = 5,
		HOME_SEARCH_HOME_OFF = 8,
		HOME_SEARCH_OFFSET = 9
	};
	eHomeState			_home_state[JDOF];
	eHomeState			_home_state_global;
	int					_home_step_global;

	rHANDLE				_sharedMemory;
};

#endif // __CONTROL_ALLEGRODOGHOME_H__
