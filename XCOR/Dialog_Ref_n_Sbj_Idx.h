#pragma once


// CRef_n_Sbj_Indexes dialog

class CRef_n_Sbj_Indexes : public CDialogEx
{
	DECLARE_DYNAMIC(CRef_n_Sbj_Indexes)

public:
	CRef_n_Sbj_Indexes(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRef_n_Sbj_Indexes();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_XCOR_RefChan, m_XCOR_SbjChan;
	int m_Ref_NnID, m_Sub_NnID;
};
