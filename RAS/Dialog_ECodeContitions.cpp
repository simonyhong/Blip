// ECodeContitions.cpp : implementation file
//

#include "stdafx.h"
#include "RAS.h"
#include "Dialog_ECodeContitions.h"
#include "afxdialogex.h"


// CECodeContitions dialog

IMPLEMENT_DYNAMIC(CECodeContitions, CDialogEx)

CECodeContitions::CECodeContitions(CWnd* pParent /*=NULL*/)
	: CDialogEx(CECodeContitions::IDD, pParent)
	, m_MustExiestECodes(_T(""))
	, m_MustNotExiestECodes(_T(""))
	, m_OneOfThemMustExiestECodes(_T(""))
{

}

CECodeContitions::~CECodeContitions()
{
}

void CECodeContitions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MustExiestECodes);
	DDX_Text(pDX, IDC_EDIT2, m_MustNotExiestECodes);
	DDX_Text(pDX, IDC_EDIT3, m_OneOfThemMustExiestECodes);
}


BEGIN_MESSAGE_MAP(CECodeContitions, CDialogEx)
END_MESSAGE_MAP()


// CECodeContitions message handlers
