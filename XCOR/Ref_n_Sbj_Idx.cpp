// EcodeToMark.cpp : implementation file
//

#include "stdafx.h"
#include "XCOR.h"
#include "Dialog_Ref_n_Sbj_Idx.h"
#include "afxdialogex.h"


// CRef_n_Sbj_Indexes dialog

IMPLEMENT_DYNAMIC(CRef_n_Sbj_Indexes, CDialogEx)

CRef_n_Sbj_Indexes::CRef_n_Sbj_Indexes(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRef_n_Sbj_Indexes::IDD, pParent)
	, m_XCOR_RefChan(0)
	, m_XCOR_SbjChan(0)
	, m_Ref_NnID(0)
	, m_Sub_NnID(0)
{

}

CRef_n_Sbj_Indexes::~CRef_n_Sbj_Indexes()
{
}

void CRef_n_Sbj_Indexes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_XCOR_RefChan);
	DDX_Text(pDX, IDC_EDIT2, m_XCOR_SbjChan);
	DDX_Radio(pDX, IDC_RADIO1, m_Ref_NnID);//This links the radio button (this one is the only one having the "grup" property activated) to the variable.
	DDX_Radio(pDX, IDC_RADIO4, m_Sub_NnID);//This links the radio button (this one is the only one having the "grup" property activated) to the variable.
}


BEGIN_MESSAGE_MAP(CRef_n_Sbj_Indexes, CDialogEx)
END_MESSAGE_MAP()


// CRef_n_Sbj_Indexes message handlers
