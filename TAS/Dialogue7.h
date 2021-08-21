#pragma once


// CDialogue7 dialog

class CDialogue7 : public CDialog
{
	DECLARE_DYNAMIC(CDialogue7)

public:
	CDialogue7(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogue7();

// Dialog Data
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_D7Var1;
	double m_D7Var2;
	double m_D7Var3;
	double m_D7Var4;
	double m_D7Var5;
	double m_D7Var6;
	double m_D7Var7;
	double m_D7Var8;
	double m_D7Var9;
	double m_D7Var10;
	afx_msg void OnEnChangeD7var1();
	afx_msg void OnEnChangeD7var2();
	afx_msg void OnEnChangeD7var3();
	afx_msg void OnEnChangeD7var4();
	afx_msg void OnEnChangeD7var5();
	afx_msg void OnEnChangeD7var6();
	afx_msg void OnEnChangeD7var7();
	afx_msg void OnEnChangeD7var8();
	afx_msg void OnEnChangeD7var9();
	afx_msg void OnEnChangeD7var10();
};
