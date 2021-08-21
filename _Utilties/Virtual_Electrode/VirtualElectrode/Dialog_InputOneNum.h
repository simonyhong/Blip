#pragma once


// Dialog_InputOneNum dialog

class Dialog_InputOneNum : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_InputOneNum)

public:
	Dialog_InputOneNum(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_InputOneNum();

// Dialog Data
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_GiveOneNum;
};
