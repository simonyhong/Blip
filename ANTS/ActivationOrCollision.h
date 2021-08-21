#pragma once


// CActivationOrCollision dialog

class CActivationOrCollision : public CDialogEx
{
	DECLARE_DYNAMIC(CActivationOrCollision)

public:
	CActivationOrCollision(CWnd* pParent = NULL);   // standard constructor
	virtual ~CActivationOrCollision();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
