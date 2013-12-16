/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
#include "stdafx.h"

#include "rCommand/rCmdDefine.h"
#include "rxSDK/rxSDK.h"
#include "rCommon/rCodeUtil.h"
#include "rPanelManipulatorCmdUtil.h"

bool bQuit = false;			// Set this flag true to quit this program.
bool bRun = false;			// Set this flag true to activate the program.
										// See OnKeyRun() function for the details.
const rTime delT = 0.001;
rPanelManipulatorData_t* pSHM = NULL;

void MyKeyboardHandler(int key, void* data);
void MyControlCallback(rTime time, void* data);
void SetupDAQ();
void PlotEnc(std::vector<double>& data);

int _tmain(int argc, _TCHAR* argv[])
{
	rCreateWorld(false, delT, rTimer_MODE_MANUAL);

	pSHM = getrPanelManipulatorCmdMemory();
	
	startPlot("-ip 127.0.0.1 -port 5158", false);
	rMakeNetwork(1, 5158);
	rInitialize();
	
	rAddKeyboardHandler(MyKeyboardHandler, NULL);
	SetupDAQ();
	
	rActivateWorld();
	bRun = true;

	while (!bQuit)
	{
		waitrPanelManipulatorCmdUpdate(50);
		rSetTime(pSHM->state.time);
		rUpdate();
	}

	DESTROY_PHYSICS_WORLD();
	DESTROY_DATA_ACQUISITION();
	closerPanelManipulatorCmdMemory();
	return 0;
}

void SetupDAQ()
{
	rID pid_q = rdaqCreatePlot(_T("joint angles"), eDataPlotType_TimeLine);
	rID did_q = rdaqAddData(pid_q, PlotEnc);
	std::vector<string_type> datanames;
	datanames.push_back(_T("q[0]"));
	datanames.push_back(_T("q[1]"));
	datanames.push_back(_T("q[2]"));
	rdaqSetDataNames(did_q, datanames);
	
}

void PlotEnc(std::vector<double>& data)
{
	if (!pSHM) return;
	data.push_back(pSHM->state.slave_state[0].position);
	data.push_back(pSHM->state.slave_state[1].position);
	data.push_back(pSHM->state.slave_state[2].position);
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
				rActivateWorld();
			}
			else
			{
				rDeactivateWorld();
			}
		}
		break;
	
	case VK_Q:
		{
			rDeactivateWorld();
			bQuit = true;
			//rQuit();
		}
		break;
	}
}
