#pragma once


// CAcceptOneNum dialog

class CAcceptOneNum : public CDialog
{
	DECLARE_DYNAMIC(CAcceptOneNum)

public:
	CAcceptOneNum(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAcceptOneNum();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDialogueBox)
	afx_msg void OnAcceptNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	double m_dNumberVariable;
};
