// Dialogue5.cpp : implementation file
//

#include "stdafx.h"
#include "MOS.h"
#include "Dialogue5.h"


// CDialogue5 dialog

IMPLEMENT_DYNAMIC(CDialogue5, CDialog)

CDialogue5::CDialogue5(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue5::IDD, pParent)
	, m_D5Var1(0)
	, m_D5Var2(0)
{

}

CDialogue5::~CDialogue5()
{
}

void CDialogue5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_D5Var1, m_D5Var1);
	DDX_Text(pDX, IDC_D5Var2, m_D5Var2);
}


BEGIN_MESSAGE_MAP(CDialogue5, CDialog)
	ON_EN_CHANGE(IDC_D5Var1, &CDialogue5::OnEnChangeD5var1)
	ON_EN_CHANGE(IDC_D5Var2, &CDialogue5::OnEnChangeD5var2)
END_MESSAGE_MAP()


// CDialogue5 message handlers

void CDialogue5::OnEnChangeD5var1()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue5::OnEnChangeD5var2()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}
