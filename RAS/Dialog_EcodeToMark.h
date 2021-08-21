#pragma once


// CEcodeToMark dialog

class CEcodeToMark : public CDialogEx
{
	DECLARE_DYNAMIC(CEcodeToMark)

public:
	CEcodeToMark(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEcodeToMark();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_EcodeToMark;
	int m_0_TheFirstRadioEnabled;
};
