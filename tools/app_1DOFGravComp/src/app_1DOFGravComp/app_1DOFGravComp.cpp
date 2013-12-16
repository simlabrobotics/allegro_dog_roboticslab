/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "stdafx.h"

#include "rMath/rMath.h"
using namespace rMath;

#include "rCommand/rCmdDefine.h"
#include "rCommand/rCmdDevice.h"
#include "rCommand/rCmdManipulator.h"
#include "rDeviceAllegroDogCANCmd.h"
#include "rDeviceAllegroDogCANDef.h"
#include "rPanelManipulatorCmd.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rxSDK/rxSDK.h"
#include "rCommon/rCodeUtil.h"
#include "../src/rDeviceAllegroDogMotor/rDeviceAllegroDogMotorCmd.h"

//#define _SIMULATOR (1)

bool bContact = false;		// Enables/disables contact dynamics.
bool bQuit = false;			// Set this flag true to quit this program.
bool bRun = false;			// Set this flag true to activate the program.
										// See OnKeyRun() function for the details.
const rTime delT = 0.002;

#define JDOF (1)
#define JID  (2)

#ifdef _SIMULATOR
string_type aml_path = _T("models/Etc/AllegroDog/P1DOF_Sim.aml");
#else
string_type aml_path = _T("models/Etc/AllegroDog/P1DOF_RT.aml");
#endif
string_type aml_name = _T("AllegroDog");
HTransform aml_T0;
dVector aml_q0;
rxSystem* sys = NULL;

rID rtTimer = INVALID_RID;
string_type timer_name = _T("RTTimer");
rTimerMode timer_mode = rTimer_MODE_REAL_RTSS;
//rTimerMode timer_mode = rTimer_MODE_REAL_WIN32;

rxDevice* devCAN = NULL;
string_type devCAN_name = _T("COMM0");
rxDevice* devMotor = NULL;
string_type devMotor_name = _T("Mot1");
rxDevice* devEnc = NULL;
string_type devEnc_name = _T("Enc1");

rPanelManipulatorData_t* pSHM = NULL;

void MyKeyboardHandler(int key, void* data);
void MyControlCallback(rTime time, void* data);
void StartControl();
void StopControl();
void ServoOn();
void ServoOff();
void ResetEncoder();
void EmergencyStop();
void PlotPos(std::vector<double>& data);
void PlotCur(std::vector<double>& data);
void PlotTimeTick(std::vector<double>& data);
void PrintInstruction();

void MyControlCallback(rTime time, void* data)
{
	static const float L = 0.0825f;//0.165f;
	static const float M = 0.1f;//0.8f;
	float torque;
	float q;
	
	devEnc->readDeviceValue(&q, sizeof(float));
	torque = M*9.81*L*sin(q);
	//printf("Torque %.3f\n",torque);
	if (devMotor)
	{
		//torque = 0.0f;
		devMotor->writeDeviceValue(&torque, sizeof(float));
	}
	if (devEnc && pSHM)
	{
		devEnc->readDeviceValue(&q, sizeof(float));
		pSHM->state.slave_state[JID].position = q;
		pSHM->state.slave_state[JID].velocity = PHYSICS_WORLD->elapsed(rtTimer)*1.0e3;
		pSHM->state.time = rGetTime(rtTimer);
		setrPanelManipulatorCmdUpdate();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	rCreateWorld(bContact, delT, rTimer_MODE_MANUAL);
	rSetGravity(0.0f, 0.0f, (float)-GRAV_ACC);
	rtTimer = rCreateTimer(timer_name, timer_mode, delT, rIO_MODE_SW);
#ifdef _SIMULATOR
	sys = rCreateSystem(aml_path, aml_name, aml_T0, aml_q0, rtTimer);
#else
	sys = rCreateRTSystem(aml_path, aml_name, aml_T0, aml_q0, rtTimer);
#endif
	devCAN = sys->findDevice(devCAN_name);
	devMotor = sys->findDevice(devMotor_name);
	devEnc = sys->findDevice(devEnc_name);
	
	//rInitializeEx(false, true);
	rInitialize();
	
	//// plot joint position
	//rID pid_pos = rdaqCreatePlot(_T("Position"), eDataPlotType_TimeLine, rtTimer);
	//rdaqAddData(pid_pos, PlotPos);

	//// plot commanded current
	//rID pid_cur = rdaqCreatePlot(_T("Current"), eDataPlotType_TimeLine, rtTimer);
	//rdaqAddData(pid_cur, PlotCur);

	//// plot timer tick
	//rID plot_t = rdaqCreatePlot(_T("Timer"), eDataPlotType_TimeLine, rtTimer);
	//rdaqAddData(plot_t, PlotTimeTick);

	pSHM = getrPanelManipulatorCmdMemory();

	rAddKeyboardHandler(MyKeyboardHandler, NULL);
	rAddControlHandler(MyControlCallback, NULL, delT, rtTimer);
	PrintInstruction();

	rTime curTime = 0;

	while (!bQuit)									// Program main-loop.
	{
		rUpdate();
		curTime = rGetTime(rtTimer);
		rSetTime(R_DEFAULT_TIMER, curTime);
		Sleep(10);
	}

	DESTROY_PHYSICS_WORLD();
	DESTROY_DATA_ACQUISITION();
	closerPanelManipulatorCmdMemory();

	printf("\n\nPress any key to exit...");
	getchar();
	return 0;
}

void MyKeyboardHandler(int key, void* data)
{
	switch (key)
	{
	case VK_W:
		break;
	case VK_S:
		break;
	case VK_E:
		break;
	case VK_D:
		break;
	case VK_R:
		break;
	case VK_F:
		break;
	case VK_T:
		break;
	case VK_G:
		break;
	case VK_Z:
		ResetEncoder();
		break;
	case VK_SPACE:
		EmergencyStop();
		break;

	case VK_TAB:
		{
			bRun = !bRun;
			if(bRun)
			{
				rActivateWorld();
				ServoOn();
				StartControl();
			}
			else
			{
				ServoOff();
				StopControl();
				rDeactivateWorld();
			}
		}
		break;
	
	case VK_Q:
		{
			ServoOff();
			StopControl();
			rDeactivateWorld();
			bQuit = true;
			//rQuit();
		}
		break;
	}
}

void StartControl()
{
	if (devCAN)
		devCAN->command(CAN_CMD_ENABLE, 0, NULL, JID);
}

void StopControl()
{
	if (devCAN)
		devCAN->command(CAN_CMD_DISABLE, 0, NULL, JID);
}

void ServoOn()
{
	if (devMotor)
		devMotor->command(ALLEGRODOGMOTOR_CMD_OPMODE_CUR);

	if (devCAN)
	{
		devCAN->command(CAN_CMD_MOTOR_ON, 0, NULL, JID);
		devCAN->command(CAN_CMD_OPMODE_CURRENT, 0, NULL, JID);
	}
}

void ServoOff()
{
	if (devCAN)
		devCAN->command(CAN_CMD_MOTOR_OFF, 0, NULL, JID);
}

void ResetEncoder()
{
	if (devCAN)
		devCAN->command(CAN_CMD_RESET_ENC, 0, NULL, JID);
}

void EmergencyStop()
{
	if (devCAN)
		devCAN->command(CAN_CMD_STOP_SYS, 0, NULL, JID);
}

void PrintInstruction()
{
	printf("**********************************************\n");
	printf("*             app_1DOFGravComp               *\n");
	printf("**********************************************\n");
	printf("TAB: Start program\n");
	printf("Q: Quit program\n");
	printf("\n");
	printf("Z: Reset encoder\n");
	printf("SPACE: Emergency stop\n");
	printf("**********************************************\n");
}

void PlotPos(std::vector<double>& data)
{
	for (int i=0; i<JDOF; i++)
	{
		float val;
		if (devEnc && devEnc->readDeviceValue(&val, 4) > 0)
			data.push_back(val);
		else
			data.push_back(0);
	}
}

void PlotCur(std::vector<double>& data)
{
	for (int i=0; i<JDOF; i++)
	{
		float val;
		if (devMotor && devMotor->monitorDeviceValue(&val, 4) > 0)
			data.push_back(val);
		else
			data.push_back(0);
	}
}

void PlotTimeTick(std::vector<double>& data)
{
	static bool bCheckPriority = false;
	if (!bCheckPriority)
	{
		int priority = GetThreadPriority(GetCurrentThread());
		printf("thread priority for RTSS thread : %d\n", priority);

		bCheckPriority = true;

		if (FALSE == SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL))
		{
			printf("SetThreadPriority() returns an error! %d\n", GetLastError());
		}
		else
		{
			priority = GetThreadPriority(GetCurrentThread());
			printf("thread priority for RTSS thread : %d\n", priority);
		}

		unsigned long pclass = GetPriorityClass(GetCurrentProcess());
		printf("priority class : %d\n", pclass);
	}

	rTime elapsed = PHYSICS_WORLD->elapsed(rtTimer);
	rTime elapsed2 = PHYSICS_WORLD->elapsed2(rtTimer);
	data.push_back(elapsed*1.0e3);
	data.push_back(elapsed2*1.0e3);
}
