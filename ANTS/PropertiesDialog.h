#pragma once


// CPropertiesDialog dialog

class CPropertiesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPropertiesDialog)

public:
	CPropertiesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertiesDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_Chan_1st;
	int m_Chan_2nd;
	int m_Chan_3rd;
	int m_Chan_4th;
	int m_Chan_5th;
	int m_Chan_6th;
	int m_Chan_7th;
	int m_Chan_8th;
	int m_Chan_9th;
	int m_Chan_10th;
	int m_PostTrigger_1st;
	int m_PostTrigger_2nd;
	int m_PostTrigger_3rd;
	int m_PostTrigger_4th;
	int m_PostTrigger_5th;
	int m_PostTrigger_6th;
	int m_PostTrigger_7th;
	int m_PostTrigger_8th;
	int m_PostTrigger_9th;
	int m_PostTrigger_10th;
	int m_StimulationLatencyBias_InMicroS;
	int m_Chan_11th;
	int m_Chan_12th;
	int m_Chan_13th;
	int m_Chan_14th;
	int m_Chan_15th;
	int m_Chan_16th;
	int m_Chan_17th;
	int m_Chan_18th;
	int m_Chan_19th;
	int m_Chan_20th;
	int m_PostTrigger_11th;
	int m_PostTrigger_12th;
	int m_PostTrigger_13th;
	int m_PostTrigger_14th;
	int m_PostTrigger_15th;
	int m_PostTrigger_16th;
	int m_PostTrigger_17th;
	int m_PostTrigger_18th;
	int m_PostTrigger_19th;
	int m_PostTrigger_20th;
};
