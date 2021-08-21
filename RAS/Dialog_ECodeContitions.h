#pragma once


// CECodeContitions dialog

class CECodeContitions : public CDialogEx
{
	DECLARE_DYNAMIC(CECodeContitions)

public:
	CECodeContitions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CECodeContitions();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_MustExiestECodes;
	CString m_MustNotExiestECodes;
	CString m_OneOfThemMustExiestECodes;
};
