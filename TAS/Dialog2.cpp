// Dialogue2.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "Dialogue2.h"


// CDialogue2 dialog

IMPLEMENT_DYNAMIC(CDialogue2, CDialog)

CDialogue2::CDialogue2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue2::IDD, pParent)
	, m_D2Var1(0)
	, m_D2Var2(0)
	, m_D2Var3(0)
{

}

CDialogue2::~CDialogue2()
{
}

void CDialogue2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_D2Var1, m_D2Var1);
	DDX_Text(pDX, IDC_D2Var2, m_D2Var2);
	DDX_Text(pDX, IDC_D2Var3, m_D2Var3);
}


BEGIN_MESSAGE_MAP(CDialogue2, CDialog)
	ON_EN_CHANGE(IDC_D2Var1, &CDialogue2::OnEnChangeD2var1)
	ON_EN_CHANGE(IDC_D2Var2, &CDialogue2::OnEnChangeD2var2)
	ON_EN_CHANGE(IDC_D2Var3, &CDialogue2::OnEnChangeD2var3)
END_MESSAGE_MAP()


// CDialogue2 message handlers



void CDialogue2::OnEnChangeD2var1()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue2::OnEnChangeD2var2()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue2::OnEnChangeD2var3()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}