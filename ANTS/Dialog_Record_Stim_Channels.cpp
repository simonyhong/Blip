// Dialog_Record_Stim_Channels.cpp : implementation file
//

#include "stdafx.h"
#include "ANTS.h"
#include "Dialog_Record_Stim_Channels.h"
#include "afxdialogex.h"


// Dialog_Record_Stim_Channels dialog

IMPLEMENT_DYNAMIC(Dialog_Record_Stim_Channels, CDialogEx)

Dialog_Record_Stim_Channels::Dialog_Record_Stim_Channels(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog_Record_Stim_Channels::IDD, pParent)
	, m_RecordingChan(0)
	, m_StimChan(0)
	, m_IsAntidromicTriggeringChannel(FALSE)
{

}

Dialog_Record_Stim_Channels::~Dialog_Record_Stim_Channels()
{
}

void Dialog_Record_Stim_Channels::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_RecordingChan);
	DDX_Text(pDX, IDC_EDIT2, m_StimChan);
	DDX_Check(pDX, IDC_CHECK_Trigger, m_IsAntidromicTriggeringChannel);
}


BEGIN_MESSAGE_MAP(Dialog_Record_Stim_Channels, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_Trigger, &Dialog_Record_Stim_Channels::OnBnClickedCheck1)
END_MESSAGE_MAP()


// Dialog_Record_Stim_Channels message handlers


void Dialog_Record_Stim_Channels::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}
