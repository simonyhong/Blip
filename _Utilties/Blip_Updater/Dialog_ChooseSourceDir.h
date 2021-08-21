#pragma once


// CChooseSourceDir dialog

class CChooseSourceDir : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseSourceDir)

public:
	CChooseSourceDir(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChooseSourceDir();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_CandidateDirectory;
	int m_UseCandidateDirectory;
	//int m_LetMeChoose;
};
