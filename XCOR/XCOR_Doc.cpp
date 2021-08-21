// XCORDoc.cpp : implementation of the CXCORDoc class
//

#include "stdafx.h"
#include "XCOR.h"

#include "XCOR_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXCORDoc

IMPLEMENT_DYNCREATE(CXCORDoc, CDocument)

BEGIN_MESSAGE_MAP(CXCORDoc, CDocument)
	//{{AFX_MSG_MAP(CXCORDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXCORDoc construction/destruction

CXCORDoc::CXCORDoc()
{
	// TODO: add one-time construction code here

}

CXCORDoc::~CXCORDoc()
{
}

BOOL CXCORDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXCORDoc serialization

void CXCORDoc::Serialize(CArchive& ar)
{
//	int TrlIdxInMap=*m_pView->M_usableTrialIdxInMem;
	int i, numEcode;
	//Reading
	if (!ar.IsStoring())
	{

	}
}

/////////////////////////////////////////////////////////////////////////////
// CXCORDoc diagnostics

#ifdef _DEBUG
void CXCORDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXCORDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXCORDoc commands
