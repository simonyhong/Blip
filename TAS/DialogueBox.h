#if !defined(AFX_DIALOGUEBOX_H__BDF90581_4374_11D7_8366_00E098722BAB__INCLUDED_)
#define AFX_DIALOGUEBOX_H__BDF90581_4374_11D7_8366_00E098722BAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogueBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogueBox dialog

class CDialogueBox : public CDialog
{
// Construction
public:
	CDialogueBox(CWnd* pParent = NULL);   // standard constructor

	CComboBox	m_TaskCombo;


// Dialog Data
	//{{AFX_DATA(CDialogueBox)
	enum { IDD = IDD_DIALOG1 };
	int		m_TaskType;
	double	m_dVariable1;
	double	m_dVariable2;
	double	m_dVariable3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogueBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDialogueBox)
	afx_msg void OnChangeVariable1();
	afx_msg void OnChangeVariable2();
	afx_msg void OnChangeVariable3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	double m_Variable4;
	double m_Variable5;
	double m_Variable6;
	double m_Variable7;
	double m_Variable8;
	double m_Variable9;
	double m_Variable10;
	double m_Variable11;
	double m_Variable12;
	double m_Variable13;
	double m_Variable14;
	double m_Variable15;
	double m_Variable16;
	double m_Variable17;
	double m_Variable18;
	double m_Variable19;
	double m_Variable20;
	double m_Variable21;
	double m_Variable22;
	double m_Variable23;
	BOOL m_Check1;
	BOOL m_Check2;
	BOOL m_Check3;
	afx_msg void OnEnChangeVariable4();
	afx_msg void OnEnChangeVariable5();
	afx_msg void OnEnChangeVariable6();
	afx_msg void OnEnChangeVariable7();
	afx_msg void OnEnChangeVariable8();
	afx_msg void OnEnChangeVariable9();
	afx_msg void OnEnChangeVariable10();
	afx_msg void OnEnChangeVariable11();
	afx_msg void OnEnChangeVariable12();
	afx_msg void OnEnChangeVariable13();
	afx_msg void OnEnChangeVariable14();
	afx_msg void OnEnChangeVariable15();
	afx_msg void OnEnChangeVariable16();
	afx_msg void OnEnChangeVariable17();
	afx_msg void OnEnChangeVariable18();
	afx_msg void OnEnChangeVariable19();
	afx_msg void OnEnChangeVariable20();
	afx_msg void OnEnChangeVariable21();
	afx_msg void OnEnChangeVariable22();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnCbnSelchangeTaskCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGUEBOX_H__BDF90581_4374_11D7_8366_00E098722BAB__INCLUDED_)
