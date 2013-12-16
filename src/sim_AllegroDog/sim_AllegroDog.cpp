/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "stdafx.h"

#include "rMath/rMath.h"
using namespace rMath;

#include "rCommand/rCmdDefine.h"
#include "rCommand/rCmdManipulator.h"
//#include "rPanelManipulatorCmd.h"
#include "rPanelManipulatorCmdUtil.h"
#include "rxSDK/rxSDK.h"
#include "rCommon/rCodeUtil.h"
#include "../control_AllegroDog/control_AllegroDogCmd.h"

#ifdef USING_NAMESPACE_RTERM
#include "rTerm/rTerm.h"
using namespace rTerm;
#endif

bool bContact = true;		// Enables/disables contact dynamics.
bool bQuit = false;			// Set this flag true to quit this program.
bool bRun = false;			// Set this flag true to activate the program.
							// See OnKeyRun() function for the details.

const rTime delT = 0.002;						// Basic time increment.

#define JDOF (12)

string_type aml_path = _T("models/Etc/AllegroDog/AllegroDogNRL_Sim.aml"); //AllegroDogNRL_Sim
string_type aml_name = _T("AllegroDog");
HTransform aml_T0;
dVector aml_q0;
rxSystem* sys = NULL;

rxControlInterface* control_pid = NULL;
string_type control_pid_path = _T("controls/control_AllegroDogNRLSim.xdl");
//string_type control_pid_path = _T("controls/control_AllegroDogNRLJointPIDSim.xdl");
//string_type control_pid_path = _T("controls/control_AllegroDogNRLTunePIDSim.xdl");
string_type control_pid_name = _T("AllegroDogCtrl");

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

void ActivateController(rxControlInterface* control);
void ServoOnController(rxControlInterface* control);
void ServoOffController(rxControlInterface* control);
void MyKeyboardHandler(int key, void* data);
void MyControlCallback(rTime time, void* data);
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

int _tmain(int argc, _TCHAR* argv[])
{
	rCreateWorld(bContact, delT);
	rSetGravity(0, 0, -GRAV_ACC);
	rCreatePlane(0, 0, 1, 0);
	
	sContactDynamicsContext cdc;
	cdc.bPositionFix = true;
	cdc.bBacktracking = true;
	PHYSICS_WORLD->setContactDynamicsContext(cdc);
	printf("creating system..\n");

	aml_T0.r[2]=0.8;
	aml_T0.R.ZRotate(90*DEGREE);
	sys = rCreateSystem(aml_path, aml_name, aml_T0, aml_q0);

	for (int i=0; i<JDOF; i++)
	{
		devMotor[i] = sys->findDevice(devMotor_name[i]);
		devEnc[i] = sys->findDevice(devEnc_name[i]);
	}

	rInitializeEx(true, true);

	if (sys)
	{
		int step = 1;
		control_pid = rCreateController(control_pid_name, sys, step);
		control_pid->setAlgorithmDll(control_pid_path);
		control_pid->setPeriod(step*delT);
		control_pid->setNominalSystem(aml_path, aml_name, aml_T0, aml_q0);
		control_pid->initAlgorithm();

		rID pid_pos = rdaqCreatePlot(_T("position"), eDataPlotType_TimeLine);
		rdaqAddData(pid_pos, control_pid, PLOT_Q);

		rID pid_vel = rdaqCreatePlot(_T("velocity"), eDataPlotType_TimeLine);
		rdaqAddData(pid_vel, control_pid, PLOT_QDOT);

		rID pid_tau = rdaqCreatePlot(_T("torque"), eDataPlotType_TimeLine);
		rdaqAddData(pid_tau, control_pid, PLOT_TORQUE);
	}

	rAddKeyboardHandler(MyKeyboardHandler, NULL);
	rAddControlHandler(MyControlCallback, NULL, 0);

	rID pid_Gyro = rdaqCreatePlot(_T("Gyro"), eDataPlotType_TimeLine);
	rdaqAddData(pid_Gyro, control_pid, PLOT_AHRS);

	pSHM = getrPanelManipulatorCmdMemory();
	PrintInstruction();

	rTime curTime = 0;
	while (!bQuit)									// Program main-loop.
	{
		rUpdate();

		switch (pSHM->cmd.command)
		{
		case CMD_ENABLE:
		case CMD_DISABLE:
			MyKeyboardHandler(VK_TAB, NULL);
			break;
		case CMD_RESET_ENCODER:
			break;
		case CMD_SERVO_ON:
			break;
		case CMD_SERVO_OFF:
			break;
		case CMD_CMD_1:
			break;
		case CMD_CMD_2:
			MotionReady();
			break;
		case CMD_CMD_3:
			MotionHop();
			break;
		case CMD_CMD_4:
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
				ActivateController(control_pid);
				ServoOnController(control_pid);
				rActivateWorld();
			}
			else
			{
				ServoOffController(control_pid);
				ActivateController(NULL);
				rDeactivateWorld();
			}
		}
		break;
	
	case VK_Q:
		{
			ServoOffController(control_pid);
			ActivateController(NULL);
			rDeactivateWorld();
			bQuit = true;
			//rQuit();
		}
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

	case VK_D:
		MotionBaseDown();
		break;

	case VK_U:
		MotionBaseUp();
		break;

	case VK_LEFT:
		MotionTurnLeft();
		break;

	case VK_RIGHT:
		MotionTurnRight();
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
		control->command(CMD_TEST_MODE_WALK);
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

void MyControlCallback(rTime time, void* data)
{
	//printf("MyControlCallback: time=%.3f\n", time);

	if (!pSHM)
		return;

	pSHM->state.time = rGetTime();

	float val;
	for (int i=0; i<JDOF; i++)
	{
		if (devEnc[i] && 0 < devEnc[i]->readDeviceValue(&val, sizeof(float)))
		{
			pSHM->state.slave_state[i].position = val;
		}
	}

	setrPanelManipulatorCmdUpdate();
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
	printf("R: Ready\n");
	printf("U: Body up 1.5(cm)\n");
	printf("D: Body down 1.5(cm)\n");
	printf("S: Stop\n");
	printf("H: Hop\n");
	printf("W: Walk\n");
	printf("B: Walk Backward\n");
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
