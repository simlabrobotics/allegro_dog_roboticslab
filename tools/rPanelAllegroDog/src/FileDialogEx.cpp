// FileDialogEx.cpp : implementation file
//

#include "stdafx.h"
#include "FileDialogEx.h"


// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}

CFileDialogEx::~CFileDialogEx()
{
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
END_MESSAGE_MAP()



// CFileDialogEx message handlers



void CFileDialogEx::OnOK()
{
	CFileDialog::OnOK();
}

void CFileDialogEx::OnFolderChange()
{
	m_strFolderName = GetFolderPath();

	CFileDialog::OnFolderChange();
}
