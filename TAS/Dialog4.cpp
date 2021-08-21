// Dialouge4.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "Dialogue4.h"


// CDialogue4 dialog

IMPLEMENT_DYNAMIC(CDialogue4, CDialog)

CDialogue4::CDialogue4(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue4::IDD, pParent)
	, m_D4Var1(0)
	, m_D4Var2(0)
	, m_D4Var3(0)
{

}

CDialogue4::~CDialogue4()
{
}

void CDialogue4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_D4Var1, m_D4Var1);
	DDX_Text(pDX, IDC_D4Var2, m_D4Var2);
	DDX_Text(pDX, IDC_D4Var3, m_D4Var3);
}


BEGIN_MESSAGE_MAP(CDialogue4, CDialog)
	ON_EN_CHANGE(IDC_D4Var1, &CDialogue4::OnEnChangeD4var1)
	ON_EN_CHANGE(IDC_D4Var2, &CDialogue4::OnEnChangeD4var2)
	ON_EN_CHANGE(IDC_D4Var3, &CDialogue4::OnEnChangeD4var3)
END_MESSAGE_MAP()


// CDialogue4 message handlers

void CDialogue4::OnEnChangeD4var1()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue4::OnEnChangeD4var2()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue4::OnEnChangeD4var3()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}
