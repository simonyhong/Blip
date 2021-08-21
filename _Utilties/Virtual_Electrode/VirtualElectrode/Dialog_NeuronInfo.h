#pragma once


// CNeuronInfo dialog

class CNeuronInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CNeuronInfo)

public:
	CNeuronInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNeuronInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_NeuronName;
	CString m_Note;
	int m_Type;
	double m_Variable1;
	double m_Variable2;
	double m_Variable3;
	double m_Variable4;
	double m_Variable5;
	double m_Variable6;
	double m_Variable7;
	double m_Variable8;
	double m_Variable9;
	double m_Variable10;
	afx_msg void OnEnChangeEdit1();
	double m_Varuable11;
	double m_Variable12;
};
