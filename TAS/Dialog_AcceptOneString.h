#pragma once


// CAcceptOneString dialog

class CAcceptOneString : public CDialog
{
	DECLARE_DYNAMIC(CAcceptOneString)

public:
	CAcceptOneString(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAcceptOneString();

// Dialog Data
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_AcceptOneString;
};
