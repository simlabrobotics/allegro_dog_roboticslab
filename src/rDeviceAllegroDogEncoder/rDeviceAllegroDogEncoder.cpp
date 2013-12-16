/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "rDeviceAllegroDogEncoder.h"
#include "rDeviceAllegroDogCANDef.h"

RD_IMPLE_FACTORY(AllegroDogEncoder)

rDeviceAllegroDogEncoder::rDeviceAllegroDogEncoder()
: _rt_joint(-1)
, _systemDevice(INVALID_RID)
, _sim_joint(INVALID_RID)
, _deviceMem_pos(NULL)
, _deviceMem_enc(NULL)
, _q(0.0f)
, _gear_ratio(1.0f)
, _gear_ratio_inv(1.0f)
, _direction(1)
{
}

rDeviceAllegroDogEncoder::~rDeviceAllegroDogEncoder()
{
}

void rDeviceAllegroDogEncoder::onCreate(const rDeviceContext& rdc)
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

	prop = getProperty(_T("gear ratio"));
	if (prop)
	{
		_gear_ratio = (float)_tstof(prop);
		_gear_ratio_inv = 1.0f / _gear_ratio;
	}

	prop = getProperty(_T("direction"));
	if (prop && prop[0] == _T('-'))
		_direction = -1;
	else
		_direction = 1;
}

void rDeviceAllegroDogEncoder::onInit()
{
	RD_DEVICE_CLASS(Base)::onInit();
	
	// find joint
	_sim_joint = _rdc.m_deviceAPI->getJointID(_rdc.m_robotname, _rdc.m_nodename);

	// find system device and set device buffer
	AllegroDog_DeviceMemory_t* mem = (AllegroDog_DeviceMemory_t*)(_rdc.m_deviceAPI->getMemory(_systemDeviceName));
	_systemDevice = _rdc.m_deviceAPI->getDeviceID(_rdc.m_robotname, _systemDeviceName);
	_deviceMem_pos = &(mem->pos_actual[_rt_joint]);
	_deviceMem_enc = &(mem->enc_actual[_rt_joint]);

	// print out device properties
	printProps();
}

void rDeviceAllegroDogEncoder::onTerminate()
{
	RD_DEVICE_CLASS(Base)::onTerminate();
}

int rDeviceAllegroDogEncoder::readDeviceValue(void* buffer, int len, int port)
{
	if (len == sizeof(float))
	{	
		_lock.lock();
		*(float*)buffer = _q;
		_lock.unlock();
		return sizeof(float);
	}
	else
		return 0;
}

void rDeviceAllegroDogEncoder::updateReadValue(rTime time)
{
	if (_systemDevice == INVALID_RID) return;
	_lock.lock();
	{
		int enc = _deviceMem_enc[0] * _direction;
		_q = (float)(enc) * _gear_ratio_inv * 2.0f * M_PI /4000.0f;
		_deviceMem_pos[0] = _q;

		if (_sim_joint != INVALID_RID)
			_rdc.m_deviceAPI->setJointPositionKinematic(_sim_joint, &_q);
	}
	_lock.unlock();
}

void rDeviceAllegroDogEncoder::printProps()
{
	printf("---------- encoder(%d) -----------\n", _rt_joint);
	printf("gear ratio = %.3f\n", _gear_ratio);
	//printf("encoder lower limit = %d (ticks)\n", _encoder_lower);
	//printf("encoder upper limit = %d (ticks)\n", _encoder_upper);
	printf("direction = %s\n", (_direction > 0 ? "+" : "-"));
	//printf("-------------------------------\n");
}
