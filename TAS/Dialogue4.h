#pragma once


// CDialogue4 dialog

class CDialogue4 : public CDialog
{
	DECLARE_DYNAMIC(CDialogue4)

public:
	CDialogue4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogue4();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_D4Var1;
	double m_D4Var2;
	double m_D4Var3;
	afx_msg void OnEnChangeD4var1();
	afx_msg void OnEnChangeD4var2();
	afx_msg void OnEnChangeD4var3();
};
