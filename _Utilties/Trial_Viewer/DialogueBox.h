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

// Dialog Data
	//{{AFX_DATA(CDialogueBox)
	enum { IDD = IDD_DIALOG1 };
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

	// Generated message map functions
	//{{AFX_MSG(CDialogueBox)
	afx_msg void OnChangeVariable1();
	afx_msg void OnChangeVariable2();
	afx_msg void OnChangeVariable3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	double m_dMinimumY;
	double m_dMaxY2;
	double m_dMinY2;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGUEBOX_H__BDF90581_4374_11D7_8366_00E098722BAB__INCLUDED_)
