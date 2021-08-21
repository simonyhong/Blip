// AcceptOneString.cpp : implementation file
//

#include "stdafx.h"
#include "MOXY.h"
#include "Dialog_AcceptOneString.h"


// CAcceptOneString dialog

IMPLEMENT_DYNAMIC(CAcceptOneString, CDialog)

CAcceptOneString::CAcceptOneString(CWnd* pParent /*=NULL*/)
	: CDialog(CAcceptOneString::IDD, pParent)
	, m_AcceptOneString(_T(""))
{

}

CAcceptOneString::~CAcceptOneString()
{
}

void CAcceptOneString::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_AcceptOneString);
}


BEGIN_MESSAGE_MAP(CAcceptOneString, CDialog)
END_MESSAGE_MAP()


// CAcceptOneString message handlers
