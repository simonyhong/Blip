// Dialogue6.cpp : implementation file
//

#include "stdafx.h"
#include "MOS.h"
#include "Dialogue6.h"


// CDialogue6 dialog

IMPLEMENT_DYNAMIC(CDialogue6, CDialog)

CDialogue6::CDialogue6(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue6::IDD, pParent)
	, m_D6Var1(0)
	, m_IsToGiveRandomRWD_NULL(0)
	, m_RandRWD_Probability(0)
{

}

CDialogue6::~CDialogue6()
{
}

void CDialogue6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_D6Var1, m_D6Var1);
	DDX_Text(pDX, IDC_EDIT1, m_IsToGiveRandomRWD_NULL);
	DDX_Text(pDX, IDC_EDIT2, m_RandRWD_Probability);
}


BEGIN_MESSAGE_MAP(CDialogue6, CDialog)
	ON_EN_CHANGE(IDC_D6Var1, &CDialogue6::OnEnChangeD6var1)
END_MESSAGE_MAP()


// CDialogue6 message handlers

void CDialogue6::OnEnChangeD6var1()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}
