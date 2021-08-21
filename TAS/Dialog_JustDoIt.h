#pragma once


// Dialog_JustDoIt dialog

class Dialog_JustDoIt : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_JustDoIt)

public:
	Dialog_JustDoIt(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_JustDoIt();

// Dialog Data
	enum { IDD = IDD_DIALOG15 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
