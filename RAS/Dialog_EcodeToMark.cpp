// EcodeToMark.cpp : implementation file
//

#include "stdafx.h"
#include "RAS.h"
#include "Dialog_EcodeToMark.h"
#include "afxdialogex.h"


// CEcodeToMark dialog

IMPLEMENT_DYNAMIC(CEcodeToMark, CDialogEx)

CEcodeToMark::CEcodeToMark(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEcodeToMark::IDD, pParent)
	, m_EcodeToMark(0)
	, m_0_TheFirstRadioEnabled(0)
{

}

CEcodeToMark::~CEcodeToMark()
{
}

void CEcodeToMark::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EcodeToMark);
	DDX_Radio(pDX, IDC_RADIO1, m_0_TheFirstRadioEnabled);//This links the radio button (this one is the only one having the "grup" property activated) to the variable.
}


BEGIN_MESSAGE_MAP(CEcodeToMark, CDialogEx)
END_MESSAGE_MAP()


// CEcodeToMark message handlers
