#pragma once


// CDialogue6 dialog

class CDialogue6 : public CDialog
{
	DECLARE_DYNAMIC(CDialogue6)

public:
	CDialogue6(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogue6();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_D6Var1;
	afx_msg void OnEnChangeD6var1();
	int m_IsToGiveRandomRWD_NULL;
	double m_RandRWD_Probability;
};
