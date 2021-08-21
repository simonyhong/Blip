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
	, m_WarningMessage(_T(""))
	, m_Check1(FALSE)
	, m_Check2(FALSE)
	, m_Check3(FALSE)
	, m_Check4(FALSE)
	, m_Check5(FALSE)
	, m_Check6(FALSE)
	, m_Check7(FALSE)
	, m_Check8(FALSE)
	, m_Check9(FALSE)
	, m_Check10(FALSE)
	, m_Check11(FALSE)
	, m_Check12(FALSE)
	, m_Check13(FALSE)
	, m_Check14(FALSE)
	, m_Check15(FALSE)
	, m_SourceDir(_T(""))
	, m_DestinationDir(_T(""))
{

}

Dialog_DeleteConfirm::~Dialog_DeleteConfirm()
{
}

void Dialog_DeleteConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT1, m_WarningMessage);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	DDX_Check(pDX, IDC_CHECK8, m_Check8);
	DDX_Check(pDX, IDC_CHECK9, m_Check9);
	DDX_Check(pDX, IDC_CHECK10, m_Check10);
	DDX_Check(pDX, IDC_CHECK11, m_Check11);
	DDX_Check(pDX, IDC_CHECK12, m_Check12);
	DDX_Check(pDX, IDC_CHECK13, m_Check13);
	DDX_Check(pDX, IDC_CHECK14, m_Check14);
	DDX_Text(pDX, IDC_EDIT1, m_SourceDir);
	DDX_Text(pDX, IDC_EDIT2, m_DestinationDir);
	DDX_Check(pDX, IDC_CHECK15, m_Check15);
}


BEGIN_MESSAGE_MAP(Dialog_DeleteConfirm, CDialogEx)
END_MESSAGE_MAP()


// Dialog_DeleteConfirm message handlers
