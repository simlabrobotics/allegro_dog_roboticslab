/* RoboticsLab, Copyright 2008-2012 SimLab Co., Ltd. All rights reserved.
 *
 * This library is commercial and cannot be redistributed, and/or modified
 * WITHOUT ANY ALLOWANCE OR PERMISSION OF SimLab Co., LTD.
 */

/**
 * @file rPanelManipulatorCmdUtil.h
 * @brief Utility function to access rPanelManipulatorCmd shared memory.
 * @author Sangyup Yi
 * @version 1.0
 * @date 2013/09/11 : version 1.0
 */
#pragma once

//#define _RTX_VER (900)

#include "rPanelManipulatorCmd.h"
#ifdef _RTX_VER
#include "Rtapi.h"
#pragma comment(lib, "rtapi_w32.lib")
#endif

#ifdef _RTX_VER
#define _rCloseHandle			RtCloseHandle 
#define _rCreateEvent			RtCreateEvent 
#define _rSetEvent				RtSetEvent 
#define _rWaitForSingleObject	RtWaitForSingleObject 
#else
#define _rCloseHandle			CloseHandle 
#define _rCreateEvent			CreateEvent 
#define _rSetEvent				SetEvent 
#define _rWaitForSingleObject	WaitForSingleObject 
#endif

static rPanelManipulatorData_t* s_shm = NULL;
static HANDLE s_hShm = NULL;
static HANDLE s_dataUpdateEvent = NULL;

/**
 * Create or get the shared memory.
 */
inline rPanelManipulatorData_t* getrPanelManipulatorCmdMemory()
{
	if (s_shm != NULL)
		return s_shm;
		
#ifndef _RTX_VER
	s_hShm = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, 
		FALSE, 
		TEXT("RoboticsLab::rPanelManipulatorCmd"));
	if (s_hShm == NULL)
	{
		s_hShm = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(rPanelManipulatorData_t),
			TEXT("RoboticsLab::rPanelManipulatorCmd"));
		if (s_hShm == NULL)
			return NULL;
	}
	
	s_shm = (rPanelManipulatorData_t*)MapViewOfFile(
		s_hShm, 
		FILE_MAP_ALL_ACCESS, 
		0, 0, 0);

#else // _RTX_VER
	s_hShm = RtOpenSharedMemory(
		SHM_MAP_WRITE,
		0,
		TEXT("RoboticsLab::rPanelManipulatorCmd"),
		(void**)&s_shm);
	if (s_hShm == NULL)
	{
		s_hShm = RtCreateSharedMemory(
			PAGE_READWRITE,
			0,
			1024*1024,
			TEXT("RoboticsLab::rPanelManipulatorCmd"),
			(void**)&s_shm);
	}
#endif

	if (!s_shm)										// Check if the system is created successfully.
	{
		printf("\n--------------------------------------------------------------------\n");
		printf("[ERROR] Could not get or create shared memory.\n");
	}

	return s_shm;
}

/**
 * Close shared memory.
 */
inline void closerPanelManipulatorCmdMemory()
{
#ifndef _RTX_VER
	if (s_shm)
		UnmapViewOfFile(s_shm);
#endif
	if (s_shm)
		_rCloseHandle(s_hShm);
	s_shm = NULL;
	s_hShm = NULL;
}

/**
 * Wait for data update
 */
inline DWORD waitrPanelManipulatorCmdUpdate(DWORD dwMilliseconds)
{
	if (!s_dataUpdateEvent)
		s_dataUpdateEvent = _rCreateEvent(NULL, FALSE, FALSE, TEXT("RoboticsLab::rPanelManipulatorCmdUpdate"));

	return _rWaitForSingleObject(s_dataUpdateEvent, dwMilliseconds);
}

/**
 * Signal data update
 */
inline BOOL setrPanelManipulatorCmdUpdate()
{
	if (!s_dataUpdateEvent)
		s_dataUpdateEvent = _rCreateEvent(NULL, FALSE, FALSE, TEXT("RoboticsLab::rPanelManipulatorCmdUpdate"));

	return _rSetEvent(s_dataUpdateEvent);
}
