// MY_NEW_FIX_TASK.cpp : implementation file
//

#include "stdafx.h"
#include "MOS.h"
#include "MY_NEW_FIX_TASK.h"
#include "afxdialogex.h"


// CMY_NEW_FIX_TASK dialog

IMPLEMENT_DYNAMIC(CMY_NEW_FIX_TASK, CDialogEx)

CMY_NEW_FIX_TASK::CMY_NEW_FIX_TASK(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMY_NEW_FIX_TASK::IDD, pParent)
	, myNewFixSize(0)
{

}

CMY_NEW_FIX_TASK::~CMY_NEW_FIX_TASK()
{
}

void CMY_NEW_FIX_TASK::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myNewFixSize);
}


BEGIN_MESSAGE_MAP(CMY_NEW_FIX_TASK, CDialogEx)
END_MESSAGE_MAP()


// CMY_NEW_FIX_TASK message handlers
