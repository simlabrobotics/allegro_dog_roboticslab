/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

#ifndef __RDEVICEALLEGRODOGCAN_H__
#define __RDEVICEALLEGRODOGCAN_H__

#include <map>
using namespace std;

#include "rMath/rMath.h"
using namespace rMath;

#include "rDevice/rDeviceBase.h"

class rDeviceAllegroDogCAN : public RD_DEVICE_CLASS(Base)
{
	RD_VERSION(1.0)
	RD_AUTHOR(SimLab)

public:
	RD_DECLARE_CTOR(AllegroDogCAN);
	RD_DECLARE_DTOR(AllegroDogCAN);

	RD_DECLARE_createDevice;
	RD_DECLARE_initDevice;
	RD_DECLARE_terminateDevice;
	RD_DECLARE_readDeviceValue;
	RD_DECLARE_writeDeviceValue;
	RD_DECLARE_updateWriteValue;
	RD_DECLARE_updateReadValue;
	RD_DECLARE_command;

private:
	void	_initParams();
	void	_openCAN();
	void	_closeCAN();
	void	_startPeriodicCommunication();
	void	_stopPeriodicCommunication();
	void	_motorOn(int drvId = -1);
	void	_motorOff(int drvId = -1);
	void	_homing(int offset, int dir, int drvId = -1, int chId = -1);
	void	_resetEncoder(int drvId = -1);
	void	_queryStatus(int drvId = -1);
	void	_setModeOfOperation(int opmode, int drvId = -1);
	void	_setVelocityInP2PMode(int v1, int v2, int drvId = -1);
	void	_stopSystem();
	void	_confirmModeOfOperation();
	void	_sendMotorCmd(int drvId = -1);
	void	_sendMotorCmdP2PAbsolute(int drvId = -1);
	void	_sendMotorCmdP2PRelative(int drvId = -1);

private:
	int		_bus;			// CAN network id
	int		_jdof;			// dimension of joints
	int		_errCount;		// CAN I/O error count
	bool	_periodicIOOn;

	// bus properties
	int		_TX_QUEUE_SIZE;
	int		_RX_QUEUE_SIZE;
	int		_TX_TIMEOUT;
	int		_RX_TIMEOUT;
	int		_mbxID;
	int		_BASE_ID;
	int		_MAX_BUS;

	// control variables
	AllegroDog_DeviceMemory_t* _vars;

	int _modeOfOperation;
	
	// command buffer
	class CANProtocolBuffer
	{
		rDeviceAllegroDogCAN* _devCAN;
		int _id;
		int _tail;
		int _count;
		rLock _lock;
		int _buf_size;
		unsigned char _buf[1024];

	public:
		CANProtocolBuffer(rDeviceAllegroDogCAN* devCAN, int id);
		void flush();
		void push(unsigned char* data, int len);
		int size();
		void handle();
	};
	map<int, CANProtocolBuffer*> _RxCmdQueue;

	// I/O thread
	friend unsigned int __stdcall ioThread(void* inst);
	uintptr_t			_ioThread;
	bool				_ioThreadRun;
	HANDLE				_ioEvent;
	HANDLE				_ioQuitEvent;
};

#endif // __RDEVICEALLEGRODOGCAN_H__
