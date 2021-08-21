// AppendOverwriteCancel.cpp : implementation file
//

#include "stdafx.h"
#include "ANTS.h"
#include "AppendOverwriteCancel.h"
#include "afxdialogex.h"


// CAppendOverwriteCancel dialog

IMPLEMENT_DYNAMIC(CAppendOverwriteCancel, CDialogEx)

CAppendOverwriteCancel::CAppendOverwriteCancel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppendOverwriteCancel::IDD, pParent)
{

}

CAppendOverwriteCancel::~CAppendOverwriteCancel()
{
}

void CAppendOverwriteCancel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAppendOverwriteCancel, CDialogEx)
	ON_BN_CLICKED(IDC_APPEND_FILE, &CAppendOverwriteCancel::OnBnClickedAppendFile)
	ON_BN_CLICKED(IDC_OVERWRITE, &CAppendOverwriteCancel::OnBnClickedOverwrite)
END_MESSAGE_MAP()


// CAppendOverwriteCancel message handlers


#include "VarDefinitions.h"
void CAppendOverwriteCancel::OnBnClickedAppendFile()
{
	m_Choice=APPEND_FILE;
	CDialog::OnOK();
}


void CAppendOverwriteCancel::OnBnClickedOverwrite()
{
	m_Choice=OVERWRITE_FILE;
	CDialog::OnOK();
}
