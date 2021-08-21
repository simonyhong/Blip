#pragma once


// Dialog_Electrode_XYZ dialog

class Dialog_Electrode_XYZ : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Electrode_XYZ)

public:
	Dialog_Electrode_XYZ(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_Electrode_XYZ();

// Dialog Data
	enum { IDD = IDD_ELECTRODE_XYZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_X;
	double m_Y;
	double m_Z;
};
