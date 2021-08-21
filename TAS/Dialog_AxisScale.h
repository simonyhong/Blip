#pragma once


// CAxisScale dialog

class CAxisScale : public CDialog
{
	DECLARE_DYNAMIC(CAxisScale)

public:
	CAxisScale(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAxisScale();

// Dialog Data
	//==========> If the program complains that the following has not been decleared. 
	//             Align the header files as follows: #include "stdafx.h"  AND  #include "TAS.h" AND  #include "Dialog_AxisScale.h"
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dMaxY;
	double m_dMinY;
	double m_dMinX;
	double m_dMaxX;
	double m_dMaxY2;
	double m_dMinY2;
};
