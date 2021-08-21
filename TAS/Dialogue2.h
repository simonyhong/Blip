#pragma once


// CDialogue2 dialog

class CDialogue2 : public CDialog
{
	DECLARE_DYNAMIC(CDialogue2)

public:
	CDialogue2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogue2();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_D2Var1;
	double m_D2Var2;
	double m_D2Var3;
	afx_msg void OnEnChangeD2var1();
	afx_msg void OnEnChangeD2var2();
	afx_msg void OnEnChangeD2var3();
};
