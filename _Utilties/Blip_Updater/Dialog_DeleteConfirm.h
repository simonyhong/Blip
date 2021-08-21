#pragma once
#include "afxwin.h"


// Dialog_DeleteConfirm dialog

class Dialog_DeleteConfirm : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_DeleteConfirm)

public:
	Dialog_DeleteConfirm(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_DeleteConfirm();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_WarningMessage;
	BOOL m_Check1;
	BOOL m_Check2;
	BOOL m_Check3;
	BOOL m_Check4;
	BOOL m_Check5;
	BOOL m_Check6;
	BOOL m_Check7;
	BOOL m_Check8;
	BOOL m_Check9;
	BOOL m_Check10;
	BOOL m_Check11;
	BOOL m_Check12;
	BOOL m_Check13;
	BOOL m_Check14;
	BOOL m_Check15;
	CString m_SourceDir;
	CString m_DestinationDir;
};
