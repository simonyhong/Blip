#pragma once


// CAcceptOneDouble dialog

class CAcceptOneDouble : public CDialogEx
{
	DECLARE_DYNAMIC(CAcceptOneDouble)

public:
	CAcceptOneDouble(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAcceptOneDouble();

// Dialog Data
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_DoubleValue;
};
