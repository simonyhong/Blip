// AnalysisDoc.cpp : implementation of the CDasDoc class
//

#include "stdafx.h"
#include "DAS.h"

#include "DAS_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDasDoc

IMPLEMENT_DYNCREATE(CDasDoc, CDocument)

BEGIN_MESSAGE_MAP(CDasDoc, CDocument)
	//{{AFX_MSG_MAP(CDasDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDasDoc construction/destruction

CDasDoc::CDasDoc()
{
	// TODO: add one-time construction code here

}

CDasDoc::~CDasDoc()
{
}

BOOL CDasDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDasDoc serialization

void CDasDoc::Serialize(CArchive& ar)
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
// CDasDoc diagnostics

#ifdef _DEBUG
void CDasDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDasDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDasDoc commands
