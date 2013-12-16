/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */
// rTermUtil.h
#ifndef __RTERMUTIL_H__
#define __RTERMUTIL_H__

namespace rTermUtil
{

	unsigned long startPhysicsExt(const char* physicsname);
	unsigned long startRobotExt(const char* robotname);
	unsigned long startControlExt(const char* controlname);
	bool stopExtensionByName(const char* extname);
	bool setExtensionParam(unsigned long hExtension, unsigned short ptype, int nparam, float fparam, const char* szparam);
	bool startSimulation(unsigned long hPhysics);
	bool stopSimulation(unsigned long hPhysics);
	bool resetSimulation(unsigned long hPhysics);
	bool setGravity(unsigned long hPhysics, float x, float y, float z);
	bool setCollision(unsigned long hPhysics, int robotid, bool enable);
	bool setBodyCollision(unsigned long hPhysics, int robotid, bool enable, const char* bodyname);
	bool createPlane(unsigned long hPhysics, float x, float y, float z, float d);
	bool activateSystem(unsigned long hPhysics, int robotid);
	bool deactivateSystem(unsigned long hPhysics, int robotid);
	bool activateEnvironment(unsigned long hPhysics, int envid);
	bool deactivateEnvironment(unsigned long hPhysics, int envid);
	unsigned long queryRobot(unsigned long hPhysics, const char* robotname);
	unsigned long queryJoint(unsigned long hPhysics, unsigned long robotid, const char* jointname);
	unsigned long queryBody(unsigned long hPhysics, unsigned long robotid, const char* bodyname);
	bool moveRobot(unsigned long hPhysics, int robotid, const double HT[]);
	bool setJointPos(unsigned long hPhysics, int robotid, const double q0[], int dof);
	bool bindControl(unsigned long hRobot, unsigned long hControl);
	bool unbindControl(unsigned long hRobot, unsigned long hControl);
	bool enableRobot(unsigned long hRobot);
	bool disableRobot(unsigned long hRobot);
	bool servoOnRobot(unsigned long hRobot);
	bool enableDevice(unsigned long hRobot, unsigned long hDevice);
	bool disableDevice(unsigned long hRobot, unsigned long hDevice);
	bool queryDevice(unsigned long hRobot, const char* dname, unsigned long& hDevice);
	bool getDeviceProperty(unsigned long hRobot, unsigned long hDevice, const char* pname, char* prop, int len);
	bool readDevicefv(unsigned long hRobot, unsigned long hDevice, int port, int dim, float* values);
	bool writeDevicefv(unsigned long hRobot, unsigned long hDevice, int port, int dim, float* values);
	bool activateControl(unsigned long hControl);
	bool deactivateControl(unsigned long hControl);
	bool setControlParam(unsigned long hControl, unsigned short ptype, int nparam, float fparam, const char* szparam);
	bool setControlInitMode(unsigned long hControl, int mode);
	bool setControlSystem(unsigned long hRobot, int systemid);
	bool setControlPeriod(unsigned long hControl, float period);
	bool setControlAlgorithm(unsigned long hControl, const char* filepath);
	bool setControlNominalPath(unsigned long hControl, const char* path);
	bool setControlNominalFilename(unsigned long hControl, const char* filename);
	bool setControlInitialJointPos(unsigned long hControl, const double q0[], int dof);
	bool setControlInitialBaseHT(unsigned long hControl, const double HT[]);
	bool sendControlCommand(unsigned long hRobot, unsigned short cmd, unsigned long arg);
	bool sendKeyEvent(unsigned long hRobot, unsigned short cmd);
	bool setTimerModeProgramAuto(int timestep);
	bool setTimerModeManual(int timestep);
	bool setTimerModeReal();
	bool setTimeResetCurrent(int time);
	bool setTimeAdvanceTimeStep(int timestep);
	void clear();

	void waitKeyInput();

}


#endif // __RTERMUTIL_H__
