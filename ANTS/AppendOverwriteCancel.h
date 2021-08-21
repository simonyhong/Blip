#pragma once


// CAppendOverwriteCancel dialog

class CAppendOverwriteCancel : public CDialogEx
{
	DECLARE_DYNAMIC(CAppendOverwriteCancel)

public:
	CAppendOverwriteCancel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAppendOverwriteCancel();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };
	int m_Choice;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAppendFile();
	afx_msg void OnBnClickedOverwrite();
};
