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
#include "rPanelManipulatorCmdUtil.h"
#include "rxSDK/rxSDK.h"
#include "rCommon/rCodeUtil.h"
#include "rCommon/rLock.h"
#include "control_AllegroDog/control_AllegroDogCmd.h"
#include "rDeviceAllegroDogMotor/rDeviceAllegroDogMotorCmd.h"

//#define _SIMULATOR (1)

bool bContact = false;		// Enables/disables contact dynamics.
bool bQuit = false;			// Set this flag true to quit this program.
bool bRun = false;			// Set this flag true to activate the program.
										// See OnKeyRun() function for the details.
const rTime delT = 0.002;

#define JDOF (12)

#ifdef _SIMULATOR
string_type aml_path = _T("models/Etc/AllegroDog/AllegroDogNRL_Sim_e.aml");
#else
string_type aml_path = _T("models/Etc/AllegroDog/AllegroDogNRL_RT.aml");
#endif
string_type aml_name = _T("AllegroDog");
HTransform aml_T0;
dVector aml_q0;
rxSystem* sys = NULL;

rxControlInterface* control_pid = NULL;
#ifdef _SIMULATOR
string_type control_pid_path = _T("controls/control_AllegroDogNRLJointPIDSim.xdl");
#else
string_type control_pid_path = _T("controls/control_AllegroDogNRLJointPIDRT.xdl");
#endif
string_type control_pid_name = _T("AllegroDogCtrl");

rID rtTimer = INVALID_RID;
string_type timer_name = _T("RTTimer");

#ifdef _SIMULATOR
rTimerMode timer_mode = rTimer_MODE_REAL_WIN32;
#else
rTimerMode timer_mode = rTimer_MODE_REAL_RTSS;
#endif

rxDevice* devCAN[2] = {NULL, NULL};
string_type devCAN_name[2] = {_T("COMM0"), _T("COMM1")};
rxDevice* devMotor[JDOF] = {NULL, };
string_type devMotor_name[JDOF] = {
	_T("Mot1"),
	_T("Mot2"),
	_T("Mot3"),
	_T("Mot4"),
	_T("Mot5"),
	_T("Mot6"),
	_T("Mot7"),
	_T("Mot8"),
	_T("Mot9"),
	_T("Mot10"),
	_T("Mot11"),
	_T("Mot12")
};
rxDevice* devEnc[JDOF] = {NULL, };
string_type devEnc_name[JDOF] = {
	_T("Enc1"),
	_T("Enc2"),
	_T("Enc3"),
	_T("Enc4"),
	_T("Enc5"),
	_T("Enc6"),
	_T("Enc7"),
	_T("Enc8"),
	_T("Enc9"),
	_T("Enc10"),
	_T("Enc11"),
	_T("Enc12")
};

rPanelManipulatorData_t* pSHM = NULL;

void MyKeyboardHandler(int key, void* data);
void MyControlCallback(rTime time, void* data);
void ActivateController(rxControlInterface* control);
void ServoOnController(rxControlInterface* control);
void ServoOffController(rxControlInterface* control);
void StartControl();
void StopControl();
void ServoOn();
void ServoOff();
void ResetEncoder();
void EmergencyStop();
void PlotCur(std::vector<double>& data);
void PlotTimeTick(std::vector<double>& data);
void PrintInstruction();

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

#ifndef _SIMULATOR
	devCAN[0] = sys->findDevice(devCAN_name[0]);
	devCAN[1] = sys->findDevice(devCAN_name[1]);
#endif
	for (int i=0; i<JDOF; i++)
	{
		devMotor[i] = sys->findDevice(devMotor_name[i]);
		devEnc[i] = sys->findDevice(devEnc_name[i]);
	}
	
#ifdef _SIMULATOR
	rInitializeEx(true, true);
#else
	rInitialize();
#endif

	if (sys)
	{
		int step = 1;
		
		control_pid = rCreateController(control_pid_name, step*delT, sys, rtTimer);
		control_pid->setAlgorithmDll(control_pid_path);
		control_pid->setPeriod(step*delT);
		control_pid->setNominalSystem(aml_path, aml_name, aml_T0, aml_q0);
		control_pid->initAlgorithm();
		control_pid->deactivate();

#ifdef _SIMULATOR
		rID pid_pos = rdaqCreatePlot(_T("position"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_pos, control_pid, PLOT_Q);

		rID pid_vel = rdaqCreatePlot(_T("velocity"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_vel, control_pid, PLOT_QDOT);

		rID pid_tau = rdaqCreatePlot(_T("torque"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_tau, control_pid, PLOT_TORQUE);
#endif
	}
	
	// plot commanded current
	/*rID pid_cur = rdaqCreatePlot(_T("Current"), eDataPlotType_TimeLine, rtTimer);
	rdaqAddData(pid_cur, PlotCur);*/

	// plot timer tick
	/*rID plot_t = rdaqCreatePlot(_T("Timer"), eDataPlotType_TimeLine, rtTimer);
	rdaqAddData(plot_t, PlotTimeTick);*/

	rAddKeyboardHandler(MyKeyboardHandler, NULL);
	rAddControlHandler(MyControlCallback, NULL, delT, rtTimer);
	PrintInstruction();

	pSHM = getrPanelManipulatorCmdMemory();
	rTime curTime = 0;

	while (!bQuit)									// Program main-loop.
	{
		rUpdate();
		curTime = rGetTime(rtTimer);
		rSetTime(R_DEFAULT_TIMER, curTime);

		switch (pSHM->cmd.command)
		{
		case CMD_GO:
			if (control_pid)
				control_pid->command(RCMD_CHANGE_TASK);
			break;
		}
		pSHM->cmd.command = CMD_NULL;

		Sleep(10);
	}

	DESTROY_PHYSICS_WORLD();
	DESTROY_DATA_ACQUISITION();
	closerPanelManipulatorCmdMemory();

	printf("\n\nPress any key to exit...");
	getchar();
	return 0;
}

void MyControlCallback(rTime time, void* data)
{
	if (!pSHM)
		return;

	pSHM->state.time = rGetTime(rtTimer);

	float val;
	
	for (int i=0; i<JDOF; i++)
	{
		if (devEnc[i] && 0 < devEnc[i]->readDeviceValue(&val, sizeof(float)))
			pSHM->state.slave_state[i].position = val;
		
		if (devMotor[i] && devMotor[i]->monitorDeviceValue(&val, sizeof(float)))
			pSHM->state.slave_state[i].torque = val;
	}

	setrPanelManipulatorCmdUpdate();
}

void MyKeyboardHandler(int key, void* data)
{
	switch (key)
	{
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
				ServoOff();
				ActivateController(control_pid);
				ServoOnController(control_pid);
				ServoOn();
				StartControl();
			}
			else
			{
				ServoOff();
				StopControl();
				ServoOffController(control_pid);
				ActivateController(NULL);
				rDeactivateWorld();
			}
		}
		break;
	
	case VK_Q:
		{
			ServoOff();
			StopControl();
			ServoOffController(control_pid);
			ActivateController(NULL);
			rDeactivateWorld();
			bQuit = true;
			//rQuit();
		}
		break;
	}
}

void ActivateController(rxControlInterface* control)
{
	if (control_pid && control_pid != control)
	{
		control_pid->command(RCMD_DEACTIVATE_CONTROL);
		control_pid->deactivate();
	}

	if (control)
	{
		control->activate();
		control->command(RCMD_ACTIVATE_CONTROL);
	}
}

void ServoOnController(rxControlInterface* control)
{
	if (control_pid && control_pid != control)
		control_pid->command(RESERVED_CMD_SERVO_OFF);

	if (control)
		control->command(RESERVED_CMD_SERVO_ON);
}

void ServoOffController(rxControlInterface* control)
{
	if (control)
		control->command(RESERVED_CMD_SERVO_OFF);
}

void StartControl()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_ENABLE, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_ENABLE, 0, NULL, JDOF);
}

void StopControl()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_DISABLE, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_DISABLE, 0, NULL, JDOF);
}

void ServoOn()
{
	for (int i=0; i<JDOF; i++)
	{
		if (devMotor[i])
			devMotor[i]->command(ALLEGRODOGMOTOR_CMD_OPMODE_CUR);
	}

	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_MOTOR_ON, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_MOTOR_ON, 0, NULL, JDOF);

	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_OPMODE_CURRENT, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_OPMODE_CURRENT, 0, NULL, JDOF);
}

void ServoOff()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_MOTOR_OFF, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_MOTOR_OFF, 0, NULL, JDOF);
}

void ResetEncoder()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_RESET_ENC, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_RESET_ENC, 0, NULL, JDOF);
}

void EmergencyStop()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_STOP_SYS, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_STOP_SYS, 0, NULL, JDOF);
}

void PrintInstruction()
{
	printf("**********************************************\n");
	printf("*             app_rPanelJog                  *\n");
	printf("**********************************************\n");
	printf("TAB: Start program\n");
	printf("Q: Quit program\n");
	printf("\n");
	printf("Z: Reset encoder\n");
	printf("SPACE: Emergency stop\n");
	printf("**********************************************\n");
}

void PlotCur(std::vector<double>& data)
{
	for (int i=0; i<JDOF; i++)
	{
		float val;
		if (devMotor[i] && devMotor[i]->monitorDeviceValue(&val, 4) > 0)
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
