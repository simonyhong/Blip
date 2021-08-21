// Dialogue7.cpp : implementation file
//

#include "stdafx.h"
#include "MOS.h"
#include "Dialogue7.h"


// CDialogue7 dialog

IMPLEMENT_DYNAMIC(CDialogue7, CDialog)

CDialogue7::CDialogue7(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue7::IDD, pParent)
	, m_D7Var1(0)
	, m_D7Var2(0)
	, m_D7Var3(0)
	, m_D7Var4(0)
	, m_D7Var5(0)
	, m_D7Var6(0)
	, m_D7Var7(0)
	, m_D7Var8(0)
	, m_D7Var9(0)
	, m_D7Var10(0)
{

}

CDialogue7::~CDialogue7()
{
}

void CDialogue7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_D7Var1, m_D7Var1);
	DDX_Text(pDX, IDC_D7Var2, m_D7Var2);
	DDX_Text(pDX, IDC_D7Var3, m_D7Var3);
	DDX_Text(pDX, IDC_D7Var4, m_D7Var4);
	DDX_Text(pDX, IDC_D7Var5, m_D7Var5);
	DDX_Text(pDX, IDC_D7Var6, m_D7Var6);
	DDX_Text(pDX, IDC_D7Var7, m_D7Var7);
	DDX_Text(pDX, IDC_D7Var8, m_D7Var8);
	DDX_Text(pDX, IDC_D7Var9, m_D7Var9);
	DDX_Text(pDX, IDC_D7Var10, m_D7Var10);
}


BEGIN_MESSAGE_MAP(CDialogue7, CDialog)
	ON_EN_CHANGE(IDC_D7Var1, &CDialogue7::OnEnChangeD7var1)
	ON_EN_CHANGE(IDC_D7Var2, &CDialogue7::OnEnChangeD7var2)
	ON_EN_CHANGE(IDC_D7Var3, &CDialogue7::OnEnChangeD7var3)
	ON_EN_CHANGE(IDC_D7Var4, &CDialogue7::OnEnChangeD7var4)
	ON_EN_CHANGE(IDC_D7Var5, &CDialogue7::OnEnChangeD7var5)
	ON_EN_CHANGE(IDC_D7Var6, &CDialogue7::OnEnChangeD7var6)
	ON_EN_CHANGE(IDC_D7Var7, &CDialogue7::OnEnChangeD7var7)
	ON_EN_CHANGE(IDC_D7Var8, &CDialogue7::OnEnChangeD7var8)
	ON_EN_CHANGE(IDC_D7Var9, &CDialogue7::OnEnChangeD7var9)
	ON_EN_CHANGE(IDC_D7Var10, &CDialogue7::OnEnChangeD7var10)
END_MESSAGE_MAP()


// CDialogue7 message handlers

void CDialogue7::OnEnChangeD7var1()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var2()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var3()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var4()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var5()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var6()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var7()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var8()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var9()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogue7::OnEnChangeD7var10()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}
