#pragma once


// CTriggerECode dialog

class CTriggerECode : public CDialog
{
	DECLARE_DYNAMIC(CTriggerECode)

public:
	CTriggerECode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTriggerECode();

// Dialog Data
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_sTriggerEcode;
};
