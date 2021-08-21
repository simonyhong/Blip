// Dialog_InputOneNum.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "Dialog_InputOneNum.h"
#include "afxdialogex.h"


// Dialog_InputOneNum dialog

IMPLEMENT_DYNAMIC(Dialog_InputOneNum, CDialogEx)

Dialog_InputOneNum::Dialog_InputOneNum(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog_InputOneNum::IDD, pParent)
	, m_GiveOneNum(0)
{

}

Dialog_InputOneNum::~Dialog_InputOneNum()
{
}

void Dialog_InputOneNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_GiveOneNum);
}


BEGIN_MESSAGE_MAP(Dialog_InputOneNum, CDialogEx)
END_MESSAGE_MAP()


// Dialog_InputOneNum message handlers
