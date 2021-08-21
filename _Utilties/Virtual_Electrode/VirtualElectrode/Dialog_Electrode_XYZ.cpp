// Dialog_Electrode_XYZ.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "Dialog_Electrode_XYZ.h"
#include "afxdialogex.h"


// Dialog_Electrode_XYZ dialog

IMPLEMENT_DYNAMIC(Dialog_Electrode_XYZ, CDialogEx)

Dialog_Electrode_XYZ::Dialog_Electrode_XYZ(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dialog_Electrode_XYZ::IDD, pParent)
	, m_X(0)
	, m_Y(0)
	, m_Z(0)
{

}

Dialog_Electrode_XYZ::~Dialog_Electrode_XYZ()
{
}

void Dialog_Electrode_XYZ::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ELECT_X, m_X);
	DDX_Text(pDX, IDC_IDC_ELECT_Y, m_Y);
	DDX_Text(pDX, IDC_IDC_ELECT_Z, m_Z);
}


BEGIN_MESSAGE_MAP(Dialog_Electrode_XYZ, CDialogEx)
END_MESSAGE_MAP()


// Dialog_Electrode_XYZ message handlers
