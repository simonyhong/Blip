#pragma once


// CDialogue5 dialog

class CDialogue5 : public CDialog
{
	DECLARE_DYNAMIC(CDialogue5)

public:
	CDialogue5(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogue5();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_D5Var1;
	double m_D5Var2;
	afx_msg void OnEnChangeD5var1();
	afx_msg void OnEnChangeD5var2();
};
