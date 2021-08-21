#pragma once


// CRename_MRIs dialog

class CRename_MRIs : public CDialogEx
{
	DECLARE_DYNAMIC(CRename_MRIs)

public:
	CRename_MRIs(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRename_MRIs();

// Dialog Data
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_Coronal_1_Sagittal_2;
	double m_Multiplication;
	double m_Addition;
};
