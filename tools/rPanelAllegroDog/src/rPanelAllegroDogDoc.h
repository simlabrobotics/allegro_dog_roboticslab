
// rPanelAllegroDogDoc.h : interface of the CrPanelAllegroDogDoc class
//


#pragma once

class CrPanelAllegroDogDoc : public CDocument
{
protected: // create from serialization only
	CrPanelAllegroDogDoc();
	DECLARE_DYNCREATE(CrPanelAllegroDogDoc)

// Attributes
public:
	int _graphSamplingPeriod;
	int _graphSamplingDuration;
	BOOL _graphQ[JDOF];
	BOOL _graphQdot[JDOF];
	BOOL _graphTau[JDOF];
	BOOL _graphErrCount;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CrPanelAllegroDogDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


