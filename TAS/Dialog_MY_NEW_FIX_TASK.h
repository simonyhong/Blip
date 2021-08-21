#pragma once


// CMY_NEW_FIX_TASK dialog

class CMY_NEW_FIX_TASK : public CDialogEx
{
	DECLARE_DYNAMIC(CMY_NEW_FIX_TASK)

public:
	CMY_NEW_FIX_TASK(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMY_NEW_FIX_TASK();

// Dialog Data
	enum { IDD = IDD_DIALOG13 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double myNewFixSize;
	int m_FixAtCenter0_RandFix1_UserXY2;
	int m_Dur_Wait_toFIX;
	int m_MinDurOfFix;
};
