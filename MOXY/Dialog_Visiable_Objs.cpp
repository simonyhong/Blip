// Dialog_Visiable_Objs.cpp : implementation file
//

#include "stdafx.h"
#include "MOXY.h"
#include "Dialog_Visiable_Objs.h"
#include "afxdialogex.h"


// Dialog_Visiable_Objs dialog

IMPLEMENT_DYNAMIC(Dialog_Visiable_Objs, CDialogEx)

Dialog_Visiable_Objs::Dialog_Visiable_Objs(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog_Visiable_Objs::IDD, pParent)
	, m_IsVisible_1(FALSE)
	, m_IsVisible_2(FALSE)
	, m_IsVisible_3(FALSE)
	, m_IsVisible_4(FALSE)
	, m_IsVisible_5(FALSE)
	, m_IsVisible_6(FALSE)
	, m_IsVisible_7(FALSE)
	, m_IsVisible_8(FALSE)
	, m_IsVisible_9(FALSE)
	, m_IsVisible_10(FALSE)
{

}

Dialog_Visiable_Objs::~Dialog_Visiable_Objs()
{
}

void Dialog_Visiable_Objs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_IsVisible_1);
	DDX_Check(pDX, IDC_CHECK2, m_IsVisible_2);
	DDX_Check(pDX, IDC_CHECK3, m_IsVisible_3);
	DDX_Check(pDX, IDC_CHECK4, m_IsVisible_4);
	DDX_Check(pDX, IDC_CHECK5, m_IsVisible_5);
	DDX_Check(pDX, IDC_CHECK6, m_IsVisible_6);
	DDX_Check(pDX, IDC_CHECK7, m_IsVisible_7);
	DDX_Check(pDX, IDC_CHECK8, m_IsVisible_8);
	DDX_Check(pDX, IDC_CHECK9, m_IsVisible_9);
	DDX_Check(pDX, IDC_CHECK10, m_IsVisible_10);
}


BEGIN_MESSAGE_MAP(Dialog_Visiable_Objs, CDialogEx)
END_MESSAGE_MAP()


// Dialog_Visiable_Objs message handlers
