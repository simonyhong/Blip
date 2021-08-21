// VISDoc.cpp : implementation of the CVISDoc class
//

#include "stdafx.h"
#include "VIS.h"

#include "VIS_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVISDoc

IMPLEMENT_DYNCREATE(CVISDoc, CDocument)

BEGIN_MESSAGE_MAP(CVISDoc, CDocument)
	//{{AFX_MSG_MAP(CVISDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVISDoc construction/destruction

CVISDoc::CVISDoc()
{
	// TODO: add one-time construction code here

}

CVISDoc::~CVISDoc()
{
}

BOOL CVISDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVISDoc serialization

void CVISDoc::Serialize(CArchive& ar)
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
// CVISDoc diagnostics

#ifdef _DEBUG
void CVISDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVISDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVISDoc commands
