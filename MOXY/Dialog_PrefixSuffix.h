#pragma once


// CPrefixSuffix dialog

class CPrefixSuffix : public CDialog
{
	DECLARE_DYNAMIC(CPrefixSuffix)

public:
	CPrefixSuffix(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrefixSuffix();

// Dialog Data
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Prefix;
	CString m_Suffix;
	int m_NextNeuronNumber;
};
