#pragma once


// CFigureProperty dialog

class CFigureProperty : public CDialog
{
	DECLARE_DYNAMIC(CFigureProperty)

public:
	CFigureProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFigureProperty();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_ChannID;
	// It needs to be between 1 and 3
	int m_NeuronID;
	int Give_Nickname_VarItself_FigIdx_FrontOrBack_RunLine;
	int m_PreAlignPeriod_ms;
	double m_dSigma;
};
