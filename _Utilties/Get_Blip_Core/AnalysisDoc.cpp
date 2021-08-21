// AnalysisDoc.cpp : implementation of the CAnalysisDoc class
//

#include "stdafx.h"
#include "Analysis.h"

#include "AnalysisDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnalysisDoc

IMPLEMENT_DYNCREATE(CAnalysisDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnalysisDoc, CDocument)
	//{{AFX_MSG_MAP(CAnalysisDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalysisDoc construction/destruction

CAnalysisDoc::CAnalysisDoc()
{
	// TODO: add one-time construction code here

}

CAnalysisDoc::~CAnalysisDoc()
{
}

BOOL CAnalysisDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAnalysisDoc serialization

void CAnalysisDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CAnalysisDoc diagnostics

#ifdef _DEBUG
void CAnalysisDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnalysisDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnalysisDoc commands
