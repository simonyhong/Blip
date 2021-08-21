// TriggerECode.cpp : implementation file
//
#include "stdafx.h"
#include "TAS.h"
#include "Dialog_TriggerECode.h"


// CTriggerECode dialog

IMPLEMENT_DYNAMIC(CTriggerECode, CDialog)

CTriggerECode::CTriggerECode(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerECode::IDD, pParent)
	, m_sTriggerEcode(_T(""))
{

}

CTriggerECode::~CTriggerECode()
{
}

void CTriggerECode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sTriggerEcode);
}


BEGIN_MESSAGE_MAP(CTriggerECode, CDialog)
END_MESSAGE_MAP()


// CTriggerECode message handlers
