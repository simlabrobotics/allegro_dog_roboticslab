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
#include "rDeviceAllegroDogMotor/rDeviceAllegroDogMotorCmd.h"
#include "rPanelManipulatorCmd.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rxSDK/rxSDK.h"
#include "rCommon/rCodeUtil.h"

//#define _SIMULATOR (1)

bool bContact = false;		// Enables/disables contact dynamics.
bool bQuit = false;			// Set this flag true to quit this program.
bool bRun = false;			// Set this flag true to activate the program.
										// See OnKeyRun() function for the details.
const rTime delT = 0.002;

#define JDOF (1)
#define JID  (8) //h:0
#define CID  (JID/6) // comm id

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
//string_type devCAN_name = _T("COMM0");
//string_type devCAN_name = _T("COMM1");
//rxDevice* devMotor = NULL;
//string_type devMotor_name = _T("Mot1");
//rxDevice* devEnc = NULL;
//string_type devEnc_name = _T("Enc1");

string_type devCAN_name[] = {
	_T("COMM0"), 
	_T("COMM1")
};
//string_type devMotor_name[] = {
//	_T("Mot1"),
//	_T("Mot2"),
//	_T("Mot3"),
//	_T("Mot4"),
//	_T("Mot5"),
//	_T("Mot6"),
//	_T("Mot7"),
//	_T("Mot8"),
//	_T("Mot9"),
//	_T("Mot10"),
//	_T("Mot11"),
//	_T("Mot12")
//};
//string_type devEnc_name[] = {
//	_T("Enc1"),
//	_T("Enc2"),
//	_T("Enc3"),
//	_T("Enc4"),
//	_T("Enc5"),
//	_T("Enc6"),
//	_T("Enc7"),
//	_T("Enc8"),
//	_T("Enc9"),
//	_T("Enc10"),
//	_T("Enc11"),
//	_T("Enc12")
//};
int homeArg[] = {
	(0 << 16) | (unsigned short)(1980),
	(1 << 16) | (unsigned short)(-1980),
	(1 << 16) | (unsigned short)(-3815),
	(0 << 16) | (unsigned short)(3990),
	(0 << 16) | (unsigned short)(3815),
	(1 << 16) | (unsigned short)(-3990),
	
	(1 << 16) | (unsigned short)(-1980),
	(0 << 16) | (unsigned short)(1980),
	(0 << 16) | (unsigned short)(3815),
	(1 << 16) | (unsigned short)(-3990),
	(1 << 16) | (unsigned short)(-3815),
	(0 << 16) | (unsigned short)(3990)
};

rPanelManipulatorData_t* pSHM = NULL;

void MyKeyboardHandler(int key, void* data);
void MyControlCallback(rTime time, void* data);
//void SetPositionRelative(float degree_delta);
//void SetPositionAbsolute(float degree);
void ServoOn();
void ServoOff();
void ResetEncoder();
void EmergencyStop();
void Homing();
//void PlotPos(std::vector<double>& data);
//void PlotCur(std::vector<double>& data);
void PlotTimeTick(std::vector<double>& data);
void PrintInstruction();

void MyControlCallback(rTime time, void* data)
{
	//float q;

	if (/*devEnc && */pSHM)
	{
		//devEnc->readDeviceValue(&q, sizeof(float));
		//pSHM->state.slave_state[JID].position = q;
		//pSHM->state.slave_state[JID].velocity = PHYSICS_WORLD->elapsed(rtTimer)*1.0e3;
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
	devCAN = sys->findDevice(devCAN_name[CID]);
	//devMotor = sys->findDevice(devMotor_name);
	//devEnc = sys->findDevice(devEnc_name);
	
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
//	case VK_LEFT:
//		SetPositionRelative(-30.0f);
//		break;
//	case VK_RIGHT:
//		SetPositionRelative(30.0f);
//		break;
//	case VK_HOME:
//		SetPositionAbsolute(0.0f);
//		break;
	case VK_H:
		Homing();
		break;

	case VK_TAB:
		{
			bRun = !bRun;
			if(bRun)
			{
				rActivateWorld();
				ResetEncoder();
				ServoOn();
//				SetPositionRelative(0.0f);
			}
			else
			{
				ServoOff();
				rDeactivateWorld();
			}
		}
		break;
	
	case VK_Q:
		{
			ServoOff();
			rDeactivateWorld();
			bQuit = true;
			//rQuit();
		}
		break;
	}
}

//void SetPositionRelative(float degree_delta)
//{
//	AllegroDog_MotorCommand_t cmd;
//	cmd.q_des = (float)(degree_delta*DEGREE);
//	cmd.qdot_des = 0.0f;
//	cmd.tau_des = 0.0f;
//	if (devMotor)
//		devMotor->writeDeviceValue(&cmd, sizeof(AllegroDog_MotorCommand_t));
//	Sleep(500);
//	if (devCAN)
//	{
//		devCAN->command(CAN_CMD_SET_POS_MAX_VELOCITY, 2000, NULL, JID);
//		Sleep(100);
//		devCAN->command(CAN_CMD_SET_POS_TARGET_RELATIVE, 0, NULL, JID);
//	}
//}

//void SetPositionAbsolute(float degree)
//{
//	AllegroDog_MotorCommand_t cmd;
//	cmd.q_des = (float)(degree*DEGREE);
//	cmd.qdot_des = 0.0f;
//	cmd.tau_des = 0.0f;
//	if (devMotor)
//		devMotor->writeDeviceValue(&cmd, sizeof(AllegroDog_MotorCommand_t));
//	Sleep(500);
//	if (devCAN)
//	{
//		devCAN->command(CAN_CMD_SET_POS_MAX_VELOCITY, 2000, NULL, JID);
//		Sleep(100);
//		devCAN->command(CAN_CMD_SET_POS_TARGET_ABSOLUTE, 0, NULL, JID);
//	}
//}

void ServoOn()
{
	/*if (devMotor)
		devMotor->command(ALLEGRODOGMOTOR_CMD_OPMODE_POS);*/

	if (devCAN)
		devCAN->command(CAN_CMD_MOTOR_ON, 0, NULL, JID);

	if (devCAN)
		devCAN->command(CAN_CMD_OPMODE_POSITION, 0, NULL, JID);
}

void ServoOff()
{
	if (devCAN)
		devCAN->command(CAN_CMD_MOTOR_OFF, 0, NULL, JID);

	/*if (devMotor)
		devMotor->command(ALLEGRODOGMOTOR_CMD_OPMODE_CUR);*/
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

void Homing()
{
	if (devCAN)
	{
		devCAN->command(CAN_CMD_SET_POS_MAX_VELOCITY, 100, NULL, JID);
		Sleep(100);
		devCAN->command(CAN_CMD_HOMING, homeArg[JID], NULL, JID);
	}
}

void PrintInstruction()
{
	printf("**********************************************\n");
	printf("*             app_homing                     *\n");
	printf("**********************************************\n");
	printf("TAB: Start program\n");
	printf("Q: Quit program\n");
	printf("\n");
	printf("H: Homing\n");
	printf("\n");
	printf("Z: Reset encoder\n");
	printf("SPACE: Emergency stop\n");
//	printf("Left Arrow: Move -30 degree relatively\n");
//	printf("Right Arrow: Move +30 degree relatively\n");
//	printf("HOME: Move to zero positoin\n");
	printf("**********************************************\n");
}

//void PlotPos(std::vector<double>& data)
//{
//	for (int i=0; i<JDOF; i++)
//	{
//		float val;
//		if (devEnc && devEnc->readDeviceValue(&val, 4) > 0)
//			data.push_back(val);
//		else
//			data.push_back(0);
//	}
//}

//void PlotCur(std::vector<double>& data)
//{
//	for (int i=0; i<JDOF; i++)
//	{
//		float val;
//		if (devMotor && devMotor->monitorDeviceValue(&val, 4) > 0)
//			data.push_back(val);
//		else
//			data.push_back(0);
//	}
//}

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
