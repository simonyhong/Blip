#pragma once


// CPrefixSuffix dialog

class CPrefixSuffix : public CDialog
{
	DECLARE_DYNAMIC(CPrefixSuffix)

public:
	CPrefixSuffix(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrefixSuffix();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_PrefixSuffix;
};
