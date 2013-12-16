/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

#ifndef __RDEVICEALLEGRODOGENCODER_H__
#define __RDEVICEALLEGRODOGENCODER_H__

#include "rMath/rMath.h"
using namespace rMath;

#include "rDevice/rDeviceBase.h"

class rDeviceAllegroDogEncoder : public RD_DEVICE_CLASS(Base)
{
	RD_VERSION(1.0)
	RD_AUTHOR(SimLab)
public:
	RD_DECLARE_CTOR(AllegroDogEncoder);
	RD_DECLARE_DTOR(AllegroDogEncoder);

	RD_DECLARE_createDevice;
	RD_DECLARE_initDevice;
	RD_DECLARE_terminateDevice;
	RD_DECLARE_readDeviceValue;
	RD_DECLARE_updateReadValue;

private:
	void printProps();

private:
	int _rt_joint;
	TCHAR _systemDeviceName[MAX_PATH];
	rID _systemDevice;
	rID _sim_joint;
	float* _deviceMem_pos;
	int*   _deviceMem_enc;

	float _q;

	float _gear_ratio;
	float _gear_ratio_inv;
	int _direction;
};

#endif // __RDEVICEALLEGRODOGENCODER_H__
