#pragma once
#include "afxwin.h"


// Dialog_Record_Stim_Channels dialog

class Dialog_Record_Stim_Channels : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Record_Stim_Channels)

public:
	Dialog_Record_Stim_Channels(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_Record_Stim_Channels();

// Dialog Data
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_RecordingChan;
	int m_StimChan;
	afx_msg void OnBnClickedCheck1();
	BOOL m_IsAntidromicTriggeringChannel;
};
