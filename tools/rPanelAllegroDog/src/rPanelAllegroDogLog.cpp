#include "stdafx.h"
#include "MainFrm.h"
#include "rPanelAllegroDog.h"
#include "rPanelAllegroDogLog.h"

void WriteLog(eLogTarget etarget, eLogType etype, bool bringFrontWnd, bool mb, TCHAR* fmt, ...)
{
	CMainFrame* frameWnd = (CMainFrame*)theApp.GetMainWnd();
	va_list arg;
    va_start(arg, fmt);
	frameWnd->WriteLog(etarget, etype, bringFrontWnd, mb, fmt, arg); 
	va_end(arg);
}

void ClearLog(eLogTarget etarget)
{
	CMainFrame* frameWnd = (CMainFrame*)theApp.GetMainWnd();
	frameWnd->ClearLog(etarget);
}
