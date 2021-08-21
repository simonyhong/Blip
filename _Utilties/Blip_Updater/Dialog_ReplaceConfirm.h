#pragma once


// CReplaceConfirm dialog

class CReplaceConfirm : public CDialogEx
{
	DECLARE_DYNAMIC(CReplaceConfirm)

public:
	CReplaceConfirm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReplaceConfirm();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
