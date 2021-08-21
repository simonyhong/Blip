// MY_NEW_FIX_TASK.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "Dialog_MY_NEW_FIX_TASK.h"
#include "afxdialogex.h"


// CMY_NEW_FIX_TASK dialog

IMPLEMENT_DYNAMIC(CMY_NEW_FIX_TASK, CDialogEx)

CMY_NEW_FIX_TASK::CMY_NEW_FIX_TASK(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMY_NEW_FIX_TASK::IDD, pParent)
	, myNewFixSize(0)
	, m_FixAtCenter0_RandFix1_UserXY2(0)
	, m_Dur_Wait_toFIX(0)
	, m_MinDurOfFix(0)
{

}

CMY_NEW_FIX_TASK::~CMY_NEW_FIX_TASK()
{
}

void CMY_NEW_FIX_TASK::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myNewFixSize);
	DDX_Text(pDX, IDC_HOW_TO_GENERATE_XY, m_FixAtCenter0_RandFix1_UserXY2);
	DDV_MinMaxInt(pDX, m_FixAtCenter0_RandFix1_UserXY2, 0, 2);
	DDX_Text(pDX, IDC_WAIT_TO_FIXATE, m_Dur_Wait_toFIX);
	DDV_MinMaxInt(pDX, m_Dur_Wait_toFIX, 0, INT_MAX);
	DDX_Text(pDX, IDC_MINI_DUR_FIX, m_MinDurOfFix);
	DDV_MinMaxInt(pDX, m_MinDurOfFix, 0, INT_MAX);
}


BEGIN_MESSAGE_MAP(CMY_NEW_FIX_TASK, CDialogEx)
END_MESSAGE_MAP()


// CMY_NEW_FIX_TASK message handlers
