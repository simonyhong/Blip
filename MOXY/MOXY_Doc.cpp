// MOXY_Doc.cpp : implementation of the CMOXYDoc class
//
#include "stdafx.h"
#include "VarDefinitions.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_Doc.h"
#include "MOXY_View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMOXYDoc

IMPLEMENT_DYNCREATE(CMOXYDoc, CDocument)

BEGIN_MESSAGE_MAP(CMOXYDoc, CDocument)
	//{{AFX_MSG_MAP(CMOXYDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMOXYDoc construction/destruction

CMOXYDoc::CMOXYDoc()
{
	// TODO: add one-time construction m_EventCode here

}

CMOXYDoc::~CMOXYDoc()
{
}

BOOL CMOXYDoc::OnNewDocument()
{
	if(!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

void CMOXYDoc::Serialize(CArchive& ar)
{
	Sleep(01);
}

/////////////////////////////////////////////////////////////////////////////
// CMOXYDoc diagnostics

#ifdef _DEBUG
void CMOXYDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMOXYDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMOXYDoc commands
