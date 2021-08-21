#pragma once


// CRenameDialog dialog

class CRenameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRenameDialog)

public:
	CRenameDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRenameDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
