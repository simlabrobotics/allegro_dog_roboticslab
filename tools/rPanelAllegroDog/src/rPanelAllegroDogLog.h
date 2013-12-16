#pragma once

enum eLogTarget
{
	eLOG_TARGET_OUTPUT = 0,
	eLOG_TARGET_DEBUG,
	eLOG_TARGET_PARAM
};

enum eLogType
{
	eLOG_INFO = 0,				///< Information.
	eLOG_WARN,					///< Warning.
	eLOG_ERROR,					///< Error messages.
	eLOG_OTHER					///< Other messages.
};

void WriteLog(eLogTarget etarget, eLogType etype, bool bringFrontWnd, bool mb, TCHAR* fmt, ...);
void ClearLog(eLogTarget etarget);
