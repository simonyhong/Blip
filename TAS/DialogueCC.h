#pragma once


// CClassicalConditioning dialog

class CClassicalConditioning : public CDialog
{
	DECLARE_DYNAMIC(CClassicalConditioning)

public:
	CClassicalConditioning(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClassicalConditioning();

// Dialog Data
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_CCTaskType;
	int m_IsAvoidence;
	int m_CenterOfAvoidenceBox_X;
	int m_CenterOfAvoidenceBox_Y;
	int m_SizeOfAvoidanceBox;
	int m_CSduration;
	int m_RWDduration;
	int m_PuffDuration;
	int m_FixedPart_ITI;
	int m_RandomPart_ITI;
};
