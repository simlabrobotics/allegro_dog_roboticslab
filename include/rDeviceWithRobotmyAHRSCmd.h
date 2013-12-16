/* RoboticsLab, Copyright 2008-2011 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

#ifndef __RDEVICEWITHROBOTMYAHRSCMD_H__
#define __RDEVICEWITHROBOTMYAHRSCMD_H__

#define CMD_MYAHRS_BASE				(0x0000)
#define CMD_MYAHRS_DATA				(CMD_MYAHRS_BASE+3)
#define CMD_MYAHRS_DATA_SINGLE		(CMD_MYAHRS_BASE+10)
#define CMD_MYAHRS_DATA_CONTINUOUS	(CMD_MYAHRS_BASE+11)

typedef enum myAHRSDataPort {
	myAHRSData_ATTI = 0,
	myAHRSData_ACC,
	myAHRSData_GYR,
	myAHRSData_MAG,
	myAHRSData_TMP
} myAHRSDataPort;

#endif // __RDEVICEWITHROBOTMYAHRSCMD_H__
