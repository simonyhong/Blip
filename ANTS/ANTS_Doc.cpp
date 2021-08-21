// AntsDoc.cpp : implementation of the CAntsDoc class
//

#include "stdafx.h"
#include "ANTS.h"

#include "ANTS_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAntsDoc

IMPLEMENT_DYNCREATE(CAntsDoc, CDocument)

BEGIN_MESSAGE_MAP(CAntsDoc, CDocument)
	//{{AFX_MSG_MAP(CAntsDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntsDoc construction/destruction

CAntsDoc::CAntsDoc()
{
	// TODO: add one-time construction code here

}

CAntsDoc::~CAntsDoc()
{
}

BOOL CAntsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAntsDoc serialization

void CAntsDoc::Serialize(CArchive& ar)
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
// CAntsDoc diagnostics

#ifdef _DEBUG
void CAntsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAntsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAntsDoc commands
