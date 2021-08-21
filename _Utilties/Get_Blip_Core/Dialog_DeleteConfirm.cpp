// Dialog_DeleteConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Analysis.h"
#include "Dialog_DeleteConfirm.h"


// Dialog_DeleteConfirm dialog

IMPLEMENT_DYNAMIC(Dialog_DeleteConfirm, CDialogEx)

Dialog_DeleteConfirm::Dialog_DeleteConfirm(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog_DeleteConfirm::IDD, pParent)
	, m_FileExts_toDelete(_T(""))
	, m_Check1(FALSE)
{

}

Dialog_DeleteConfirm::~Dialog_DeleteConfirm()
{
}

void Dialog_DeleteConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FileExts_toDelete);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
}


BEGIN_MESSAGE_MAP(Dialog_DeleteConfirm, CDialogEx)
END_MESSAGE_MAP()


// Dialog_DeleteConfirm message handlers
