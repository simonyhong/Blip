// AcceptOneNum.cpp : implementation file
//

#include "stdafx.h"
#include "RAS.h"
#include "AcceptOneNum.h"


// CAcceptOneNum dialog

IMPLEMENT_DYNAMIC(CAcceptOneNum, CDialog)

CAcceptOneNum::CAcceptOneNum(CWnd* pParent /*=NULL*/)
	: CDialog(CAcceptOneNum::IDD, pParent)
	, m_dNumberVariable(0)
{

}

CAcceptOneNum::~CAcceptOneNum()
{
}

void CAcceptOneNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dNumberVariable);
}


BEGIN_MESSAGE_MAP(CAcceptOneNum, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnAcceptNum)
END_MESSAGE_MAP()


// CAcceptOneNum message handlers


void CAcceptOneNum::OnAcceptNum() 
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}
