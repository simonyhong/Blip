#pragma once


// CPitchRollYaw dialog

class CPitchRollYaw : public CDialogEx
{
	DECLARE_DYNAMIC(CPitchRollYaw)

public:
	CPitchRollYaw(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPitchRollYaw();

// Dialog Data
	enum { IDD = IDD_PITCH_ROLL_YAW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_GridPitch;
	double m_GridRoll;
	double m_GridYaw;
};
