#pragma once


// Dialog_Visiable_Objs dialog

class Dialog_Visiable_Objs : public CDialogEx
{
	DECLARE_DYNAMIC(Dialog_Visiable_Objs)

public:
	Dialog_Visiable_Objs(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dialog_Visiable_Objs();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_IsVisible_1;
	BOOL m_IsVisible_2;
	BOOL m_IsVisible_3;
	BOOL m_IsVisible_4;
	BOOL m_IsVisible_5;
	BOOL m_IsVisible_6;
	BOOL m_IsVisible_7;
	BOOL m_IsVisible_8;
	BOOL m_IsVisible_9;
	BOOL m_IsVisible_10;
};
