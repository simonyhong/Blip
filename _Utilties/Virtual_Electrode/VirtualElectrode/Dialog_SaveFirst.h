#pragma once


// Dialog_SaveFirst dialog

class Dialog_SaveFirst : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_SaveFirst)

public:
	Dialog_SaveFirst(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_SaveFirst();

// Dialog Data
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
