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
//#include "rPanelManipulatorCmd.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rxSDK/rxSDK.h"
#include "rCommon/rCodeUtil.h"
#include "../control_AllegroDog/control_AllegroDogCmd.h"
#include "../control_AllegroDogHome/control_AllegroDogHomeCmd.h"
#include "../rDeviceAllegroDogMotor/rDeviceAllegroDogMotorCmd.h"


#define USE_CONTROL_PLUGIN

bool bContact = false;		// Enables/disables contact dynamics.
bool bQuit = false;			// Set this flag true to quit this program.
bool bRun = false;			// Set this flag true to activate the program.
										// See OnKeyRun() function for the details.
const rTime delT = 0.002;

#define JDOF (12)

string_type aml_path = _T("models/Etc/AllegroDog/AllegroDogNRL_RT.aml");
string_type aml_name = _T("AllegroDog");
HTransform aml_T0;
dVector aml_q0;
rxSystem* sys = NULL;

rxControlInterface* control_pid = NULL;
string_type control_pid_path = _T("controls/control_AllegroDogNRLRT.xdl");
string_type control_pid_name = _T("AllegroDogCtrl");

rxControlInterface* control_home = NULL;
string_type control_home_path = _T("controls/control_AllegroDogNRLHomeRT.xdl");
string_type control_home_name = _T("AllegroDogHomeCtrl");

rxControlInterface* control_current = NULL; // currently activated controller

rID rtTimer = INVALID_RID;
string_type timer_name = _T("RTTimer");
rTimerMode timer_mode = rTimer_MODE_REAL_RTSS;

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

rxDevice* devGyro = NULL;
string_type devGyro_name = _T("GyroSensor");

rPanelManipulatorData_t* pSHM = NULL;

#ifdef USE_CONTROL_PLUGIN
bool bActivateControlPlugin = true;
#else
bool bActivateControlPlugin = false;
#endif

void MyKeyboardHandler(int key, void* data);
void MyControlCallback(rTime time, void* data);
void ActivateController(rxControlInterface* control);
void ServoOnController(rxControlInterface* control);
void ServoOffController(rxControlInterface* control);
void ResetEncoder();
void ServoOn();			// motor power on
void ServoOff();		// motor power off
void StartControl();	// start periodic current control
void StopControl();		// stop periodic control
void Home();
void MotionReady();
void MotionHop();
void MotionWalk();
void MotionWalkBackward();
void MotionStop();
void MotionTurnLeft();
void MotionTurnRight();
void MotionBaseUp();
void MotionBaseDown();
void MotionSpeedUp();
void MotionSpeedDown();
void PrintInstruction();
void PlotPos(std::vector<double>& data);
void PlotCur(std::vector<double>& data);
void PlotTimeTick(std::vector<double>& data);

int _tmain(int argc, _TCHAR* argv[])
{
	rCreateWorld(bContact, delT, rTimer_MODE_MANUAL);
	rSetGravity(0, 0, -GRAV_ACC);
	rtTimer = rCreateTimer(timer_name, timer_mode, delT);
	sys = rCreateRTSystem(aml_path, aml_name, aml_T0, aml_q0, rtTimer);
	devCAN[0] = sys->findDevice(devCAN_name[0]);
	devCAN[1] = sys->findDevice(devCAN_name[1]);
	for (int i=0; i<JDOF; i++)
	{
		devMotor[i] = sys->findDevice(devMotor_name[i]);
		devEnc[i] = sys->findDevice(devEnc_name[i]);
	//	devMotor[i]->command(ALLEGRODOGMOTOR_CMD_OPMODE_CUR, 0, NULL, i);
	}
	devGyro = sys->findDevice(devGyro_name);
	//assert(devGyro && _T("Gyro sensor not found."));

//	rInitializeEx(false, true);
	rInitialize();

	if (sys && bActivateControlPlugin)
	{
		int step = 1;
		
		control_pid = rCreateController(control_pid_name, step*delT, sys, rtTimer);
		control_pid->setAlgorithmDll(control_pid_path);
		control_pid->setPeriod(step*delT);
		control_pid->setNominalSystem(aml_path, aml_name, aml_T0, aml_q0);
		control_pid->initAlgorithm();
		control_pid->deactivate();

		/*rID pid_pos = rdaqCreatePlot(_T("position"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_pos, control_pid, PLOT_Q);*/

		/*rID pid_vel = rdaqCreatePlot(_T("velocity"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_vel, control_pid, PLOT_QDOT);*/

		/*rID pid_tau = rdaqCreatePlot(_T("torque"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_tau, control_pid, PLOT_TORQUE);*/

		/*rID pid_gyro = rdaqCreatePlot(_T("gyro"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_gyro, control_pid, PLOT_AHRS);*/

		step = 50;
		control_home = rCreateController(control_home_name, step*delT, sys, rtTimer);
		control_home->setAlgorithmDll(control_home_path);
		control_home->setPeriod(step*delT);
		control_home->setNominalSystem(aml_path, aml_name, aml_T0, aml_q0);
		control_home->initAlgorithm();
		control_home->deactivate();
	}
	else
	{
		rID pid_pos = rdaqCreatePlot(_T("Joint Position"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_pos, PlotPos);

		rID pid_cur = rdaqCreatePlot(_T("Current"), eDataPlotType_TimeLine, rtTimer);
		rdaqAddData(pid_cur, PlotCur);
	}

	// plot timer tick
	/*rID plot_t = rdaqCreatePlot(_T("Timer"), eDataPlotType_TimeLine, rtTimer);
	rdaqAddData(plot_t, PlotTimeTick);*/

	rAddKeyboardHandler(MyKeyboardHandler, NULL);
	rAddControlHandler(MyControlCallback, NULL, 0.1, rtTimer);

	pSHM = getrPanelManipulatorCmdMemory();
	PrintInstruction();

	//rRun();
	rTime curTime = 0;

	while (!bQuit)									// Program main-loop.
	{
		rUpdate();
		curTime = rGetTime(rtTimer);
		rSetTime(R_DEFAULT_TIMER, curTime);

		switch (pSHM->cmd.command)
		{
		case CMD_ENABLE: // rPanelAllegroDog ENABLE button
			ServoOff();
		//	ActivateController(control_pid);
		//	ServoOnController(control_pid);
			StartControl();
			ServoOn();
			ActivateController(control_pid);
			ServoOnController(control_pid);
			break;
		case CMD_DISABLE: // rPanelAllegroDog STOP button
			ServoOff();
			StopControl();
			ServoOffController(control_pid);
			ActivateController(NULL);
			break;
		/*case CMD_RESET_ENCODER:
			break;
		case CMD_SERVO_ON:
			break;
		case CMD_SERVO_OFF:
			break;*/
		case CMD_CMD_1: // Homing
			Home();
			break;
		case CMD_CMD_2: // Ready
			MotionReady();
			break;
		case CMD_CMD_3: // Hop
			MotionHop();
			break;
		case CMD_CMD_4: // Walk
			MotionWalk();
			break;
		case CMD_CMD_5: // Walk Backward
			MotionWalkBackward();
			break;
		case CMD_CMD_6:
			MotionStop();
			break;
		case CMD_CMD_7: // Turn Left
			MotionTurnLeft();
			break;
		case CMD_CMD_8: // Turn Right
			MotionTurnRight();
			break;
		case CMD_CMD_9: // Base Up
			MotionBaseUp();
			break;
		case CMD_CMD_10: // Base Down
			MotionBaseDown();
			break;
		case CMD_CMD_11: // Speed Up
			MotionSpeedUp();
			break;
		case CMD_CMD_12: // Speed Down
			MotionSpeedDown();
			break;
		case CMD_EXIT:
			MyKeyboardHandler(VK_Q, NULL);
			break;
		}

		pSHM->cmd.command = CMD_NULL;
		Sleep(10);
	}

	DESTROY_PHYSICS_WORLD();
	DESTROY_DATA_ACQUISITION();
	closerPanelManipulatorCmdMemory();

	return 0;
}

void MyKeyboardHandler(int key, void* data)
{
	switch (key)
	{
	case VK_TAB:
		{
			bRun = !bRun;
			if(bRun)
			{
				if (devGyro) devGyro->on();
				rActivateWorld();
			}
			else
			{
				ServoOff();
				StopControl();
				ServoOffController(control_pid);
				ServoOffController(control_home);
				ActivateController(NULL);
				if (devGyro) devGyro->off();
				rDeactivateWorld();
			}
		}
		break;
	
	case VK_Q:
		{
			ServoOff();
			StopControl();
			ServoOffController(control_pid);
			ServoOffController(control_home);
			ActivateController(NULL);
			if (devGyro) devGyro->off();
		//	if (devGyro) devGyro->release();
			rDeactivateWorld();
		//	rDestroyTimerEvent(rtTimer);
		//	rDestroyTimer(rtTimer);
			bQuit = true;
		//	rQuit();
		}
		break;

	case VK_LEFT:
		MotionTurnLeft();
		printf("LEFT arrow: Turn left 15 degree.\n");
		break;

	case VK_RIGHT:
		MotionTurnRight();
		printf("RIGHT arrow: Turn right 15 degree.\n");
		break;

	case VK_PRIOR:
		MotionSpeedUp();
		break;

	case VK_NEXT:
		MotionSpeedDown();
		break;

	case VK_R:
		MotionReady();
		break;

	case VK_W:
		MotionWalk();
		break;

	case VK_B:
		MotionWalkBackward();
		break;

	case VK_S:
		MotionStop();
		break;
		
	case VK_H:
		MotionHop();
		break;

	case VK_E:
		ResetEncoder();
		break;

	case VK_HOME:
		Home();
		break;

	case VK_D:
		MotionBaseDown();
		break;

	case VK_U:
		MotionBaseUp();
		break;
	}
}

void ActivateController(rxControlInterface* control)
{
	if (control_current == control)
		return;

	if (control_current && control_current != control)
	{
		control_current->command(RCMD_DEACTIVATE_CONTROL);
		control_current->deactivate();
	}

	if (control)
	{
		control->activate();
		control->command(RCMD_ACTIVATE_CONTROL);
	}

	control_current = control;
}

void ServoOnController(rxControlInterface* control)
{
	if (control)
		control->command(RESERVED_CMD_SERVO_ON);
}

void ServoOffController(rxControlInterface* control)
{
	if (control)
		control->command(RESERVED_CMD_SERVO_OFF);
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

	if (!bActivateControlPlugin)
	{
		for (int i=0; i<JDOF; i++)
		{
			if (devMotor[i])
			{
				val = pSHM->cmd.slave_command[i].torque;
				devMotor[i]->writeDeviceValue(&val, sizeof(float));
			}
		}
	}
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

	if (control_pid)
	{
		control_pid->command(CMD_TEST_MODE_WALK);
	}
}

void ServoOff()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_MOTOR_OFF, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_MOTOR_OFF, 0, NULL, JDOF);
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

void ResetEncoder()
{
	if (devCAN[0])
		devCAN[0]->command(CAN_CMD_RESET_ENC, 0, NULL, JDOF);
	if (devCAN[1])
		devCAN[1]->command(CAN_CMD_RESET_ENC, 0, NULL, JDOF);
}

void Home()
{
	ActivateController(control_home);
	if (control_home)
		control_home->command(CMD_HOME);
}

void MotionReady()
{
	//ActivateController(control_pid);
	if (control_pid)
	{
		control_pid->command(CMD_TEST_MODE_WALK);
		control_pid->command(CMD_MOTION_READY);
	}
}

void MotionHop()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_MOTION_HOP);
}

void MotionWalk()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_MOTION_RUN);
}

void MotionWalkBackward()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_MOTION_BACK);
}

void MotionStop()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_MOTION_STOP);
}

void MotionTurnLeft()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_MOTION_TURNL);
}

void MotionTurnRight()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_MOTION_TURNR);
}

void MotionBaseUp()
{
	//ActivateController(control_pid);
	if (control_pid)
	{
		control_pid->command(CMD_MOTION_UP);
		printf("Up the body 1.5 cm.\n");
	}
}

void MotionBaseDown()
{
	//ActivateController(control_pid);
	if (control_pid)
	{
		control_pid->command(CMD_MOTION_DN);
		printf("Down the body 1.5 cm.\n");
	}
}

void MotionSpeedUp()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_SPEED_UP);
}

void MotionSpeedDown()
{
	//ActivateController(control_pid);
	if (control_pid)
		control_pid->command(CMD_SPEED_DN);
}

void PrintInstruction()
{
	printf("**********************************************\n");
	printf("*             app_AllegroDog                 *\n");
	printf("**********************************************\n");
	printf("TAB: Start program\n");
	printf("Q: Quit program\n");
	printf("\n");
	printf("HOME: Homing\n");
	printf("E: Reset Encoder\n");
	printf("\n");
	printf("R: Ready\n");
	printf("U: Body up 1.5(cm)\n");
	printf("D: Body down 1.5(cm)\n");
	printf("H: Hop\n");
	printf("W: Walk\n");
	printf("S: Stop\n");
	printf("LEFT arrow: Turn left 15 degree.\n");
	printf("RIGHT arrow: Turn right 15 degree.\n");
	printf("Page Up: Speed up.\n");
	printf("Page Down: Speed down.\n");
	printf("**********************************************\n");
}

void PlotPos(std::vector<double>& data)
{
	for (int i=0; i<JDOF; i++)
	{
		float val;
		if (devEnc[i] && devEnc[i]->readDeviceValue(&val, 4) > 0)
			data.push_back(val*RADIAN);
		else
			data.push_back(0);
	}
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
