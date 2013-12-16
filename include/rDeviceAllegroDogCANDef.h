#ifndef __RDEVICEALLEGRODOGCANDEF_H__
#define __RDEVICEALLEGRODOGCANDEF_H__

#include "canDefs.h"

#define MAX_DOF 12

typedef struct tagDeviceMemory_AllegroDog
{
	float pos_actual[MAX_DOF];
	float vel_actual[MAX_DOF];
	float cur_actual[MAX_DOF];
	float pos_demand[MAX_DOF];
	float vel_demand[MAX_DOF];
	float cur_demand[MAX_DOF];
	int   enc_actual[MAX_DOF];
	int   cmd_demand[MAX_DOF]; // motor driver(Robocube Tech) command
	int   mode_of_op[MAX_DOF]; // mode of operation
	int   mode_of_op_disp[MAX_DOF]; // mode of operation display
	int   op_status[MAX_DOF];
	int   falut_status[MAX_DOF];
} AllegroDog_DeviceMemory_t;

enum eAllegroDog_OP_MODE
{
	eOP_MODE_POS = CONTROL_MODE_POSITION, // position servo
	eOP_MODE_VEL = CONTROL_MODE_VELOCITY, // velocity servo
	eOP_MODE_CUR = CONTROL_MODE_CURRENT  // current servo
};

#endif // __RDEVICEALLEGRODOGCANDEF_H__
