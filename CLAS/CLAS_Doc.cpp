// CLAS_Doc.cpp : implementation of the CCLAS_Doc class
//

#include "stdafx.h"
#include "CLAS.h"

#include "CLAS_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLAS_Doc

IMPLEMENT_DYNCREATE(CCLAS_Doc, CDocument)

BEGIN_MESSAGE_MAP(CCLAS_Doc, CDocument)
	//{{AFX_MSG_MAP(CCLAS_Doc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLAS_Doc construction/destruction

CCLAS_Doc::CCLAS_Doc()
{
	// TODO: add one-time construction code here

}

CCLAS_Doc::~CCLAS_Doc()
{
}

BOOL CCLAS_Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCLAS_Doc serialization

void CCLAS_Doc::Serialize(CArchive& ar)
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
// CCLAS_Doc diagnostics

#ifdef _DEBUG
void CCLAS_Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCLAS_Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCLAS_Doc commands
