
// rPanelAllegroDogDoc.cpp : implementation of the CrPanelAllegroDogDoc class
//

#include "stdafx.h"
#include "rPanelAllegroDog.h"

#include "rPanelAllegroDogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CrPanelAllegroDogDoc

IMPLEMENT_DYNCREATE(CrPanelAllegroDogDoc, CDocument)

BEGIN_MESSAGE_MAP(CrPanelAllegroDogDoc, CDocument)
END_MESSAGE_MAP()


// CrPanelAllegroDogDoc construction/destruction

CrPanelAllegroDogDoc::CrPanelAllegroDogDoc()
{
	_graphSamplingPeriod = theAppParam.nGraphSamplingPeriod; // msec
	_graphSamplingDuration = theAppParam.nGraphSamplingDuration; // msec
	for (int i=0; i<JDOF; i++)
	{
		_graphQ[i] = TRUE;
		_graphQdot[i] = FALSE;
		_graphTau[i] = FALSE;
	}
	_graphErrCount = FALSE;
}

CrPanelAllegroDogDoc::~CrPanelAllegroDogDoc()
{
}

BOOL CrPanelAllegroDogDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CrPanelAllegroDogDoc serialization

void CrPanelAllegroDogDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CrPanelAllegroDogDoc diagnostics

#ifdef _DEBUG
void CrPanelAllegroDogDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CrPanelAllegroDogDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CrPanelAllegroDogDoc commands
