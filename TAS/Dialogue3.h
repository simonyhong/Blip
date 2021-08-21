#pragma once


// CDialogue3 dialog

class CDialogue3 : public CDialog
{
	DECLARE_DYNAMIC(CDialogue3)

public:
	CDialogue3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogue3();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_D3Var1;
	double m_D3Var2;
	double m_D3Var3;
	double m_D3Var4;
	double m_D3Var5;
	double m_D3Var6;
	double m_D3Var7;
	BOOL m_D3Check1;
	BOOL m_D3Check2;
	afx_msg void OnEnChangeD3var1();
	//afx_msg void OnEnChangeD3var2();
	//afx_msg void OnEnChangeD3var3();
	//afx_msg void OnEnChangeD3var4();
	//afx_msg void OnEnChangeD3var5();
	//afx_msg void OnEnChangeD3var6();
	//afx_msg void OnEnChangeD3var7();
	//afx_msg void OnBnClickedD3check1();
	//afx_msg void OnBnClickedD3check2();
};
