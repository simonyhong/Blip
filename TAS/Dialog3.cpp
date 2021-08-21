// Dialogue3.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "Dialogue3.h"


// CDialogue3 dialog

IMPLEMENT_DYNAMIC(CDialogue3, CDialog)

CDialogue3::CDialogue3(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue3::IDD, pParent)
	, m_D3Var1(0)
	//, m_D3Var2(0)
	//, m_D3Var3(0)
	//, m_D3Var4(0)
	//, m_D3Var5(0)
	//, m_D3Var6(0)
	//, m_D3Var7(0)
	//, m_D3Check1(FALSE)
	//, m_D3Check2(FALSE)
{

}

CDialogue3::~CDialogue3()
{
}

void CDialogue3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_D3Var1, m_D3Var1);
	//DDX_Text(pDX, IDC_D3Var2, m_D3Var2);
	//DDX_Text(pDX, IDC_D3Var3, m_D3Var3);
	//DDX_Text(pDX, IDC_D3Var4, m_D3Var4);
	//DDX_Text(pDX, IDC_D3Var5, m_D3Var5);
	//DDX_Text(pDX, IDC_D3Var6, m_D3Var6);
	//DDX_Text(pDX, IDC_D3Var7, m_D3Var7);
	//DDX_Check(pDX, IDC_D3CHECK1, m_D3Check1);
	//DDX_Check(pDX, IDC_D3CHECK2, m_D3Check2);
}


BEGIN_MESSAGE_MAP(CDialogue3, CDialog)
	ON_EN_CHANGE(IDC_D3Var1, &CDialogue3::OnEnChangeD3var1)
	//ON_EN_CHANGE(IDC_D3Var2, &CDialogue3::OnEnChangeD3var2)
	//ON_EN_CHANGE(IDC_D3Var3, &CDialogue3::OnEnChangeD3var3)
	//ON_EN_CHANGE(IDC_D3Var4, &CDialogue3::OnEnChangeD3var4)
	//ON_EN_CHANGE(IDC_D3Var5, &CDialogue3::OnEnChangeD3var5)
	//ON_EN_CHANGE(IDC_D3Var6, &CDialogue3::OnEnChangeD3var6)
	//ON_EN_CHANGE(IDC_D3Var7, &CDialogue3::OnEnChangeD3var7)
	//ON_BN_CLICKED(IDC_D3CHECK1, &CDialogue3::OnBnClickedD3check1)
	//ON_BN_CLICKED(IDC_D3CHECK2, &CDialogue3::OnBnClickedD3check2)
END_MESSAGE_MAP()


// CDialogue3 message handlers

void CDialogue3::OnEnChangeD3var1()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

//void CDialogue3::OnEnChangeD3var2()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnEnChangeD3var3()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnEnChangeD3var4()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnEnChangeD3var5()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnEnChangeD3var6()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnEnChangeD3var7()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnBnClickedD3check1()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
//
//void CDialogue3::OnBnClickedD3check2()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}
