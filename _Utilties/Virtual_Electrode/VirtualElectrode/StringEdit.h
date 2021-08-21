#pragma once


// StringEdit dialog

class StringEdit : public CDialog
{
	DECLARE_DYNAMIC(StringEdit)

public:
	StringEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~StringEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_UserInputString1;
};
