#pragma once


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
	CString m_FileExts_toDelete;
	BOOL m_Check1;
	afx_msg void OnBnClickedCheck1();
};
