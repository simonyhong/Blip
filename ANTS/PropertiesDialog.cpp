// PropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ANTS.h"
#include "PropertiesDialog.h"
#include "afxdialogex.h"


// CPropertiesDialog dialog

IMPLEMENT_DYNAMIC(CPropertiesDialog, CDialogEx)

CPropertiesDialog::CPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPropertiesDialog::IDD, pParent)
	, m_Chan_1st(0)
	, m_Chan_2nd(0)
	, m_Chan_3rd(0)
	, m_Chan_4th(0)
	, m_Chan_5th(0)
	, m_Chan_6th(0)
	, m_Chan_7th(0)
	, m_Chan_8th(0)
	, m_Chan_9th(0)
	, m_Chan_10th(0)
	, m_PostTrigger_1st(0)
	, m_PostTrigger_2nd(0)
	, m_PostTrigger_3rd(0)
	, m_PostTrigger_4th(0)
	, m_PostTrigger_5th(0)
	, m_PostTrigger_6th(0)
	, m_PostTrigger_7th(0)
	, m_PostTrigger_8th(0)
	, m_PostTrigger_9th(0)
	, m_PostTrigger_10th(0)
	, m_StimulationLatencyBias_InMicroS(0)
	, m_Chan_11th(0)
	, m_Chan_12th(0)
	, m_Chan_13th(0)
	, m_Chan_14th(0)
	, m_Chan_15th(0)
	, m_Chan_16th(0)
	, m_Chan_17th(0)
	, m_Chan_18th(0)
	, m_Chan_19th(0)
	, m_Chan_20th(0)
	, m_PostTrigger_11th(0)
	, m_PostTrigger_12th(0)
	, m_PostTrigger_13th(0)
	, m_PostTrigger_14th(0)
	, m_PostTrigger_15th(0)
	, m_PostTrigger_16th(0)
	, m_PostTrigger_17th(0)
	, m_PostTrigger_18th(0)
	, m_PostTrigger_19th(0)
	, m_PostTrigger_20th(0)
{

}

CPropertiesDialog::~CPropertiesDialog()
{
}

void CPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_Chan_1st);
	DDX_Text(pDX, IDC_EDIT3, m_Chan_2nd);
	DDX_Text(pDX, IDC_EDIT4, m_Chan_3rd);
	DDX_Text(pDX, IDC_EDIT5, m_Chan_4th);
	DDX_Text(pDX, IDC_EDIT6, m_Chan_5th);
	DDX_Text(pDX, IDC_EDIT7, m_Chan_6th);
	DDX_Text(pDX, IDC_EDIT8, m_Chan_7th);
	DDX_Text(pDX, IDC_EDIT9, m_Chan_8th);
	DDX_Text(pDX, IDC_EDIT10, m_Chan_9th);
	DDX_Text(pDX, IDC_EDIT11, m_Chan_10th);
	DDX_Text(pDX, IDC_EDIT12, m_PostTrigger_1st);
	DDX_Text(pDX, IDC_EDIT13, m_PostTrigger_2nd);
	DDX_Text(pDX, IDC_EDIT14, m_PostTrigger_3rd);
	DDX_Text(pDX, IDC_EDIT15, m_PostTrigger_4th);
	DDX_Text(pDX, IDC_EDIT16, m_PostTrigger_5th);
	DDX_Text(pDX, IDC_EDIT17, m_PostTrigger_6th);
	DDX_Text(pDX, IDC_EDIT18, m_PostTrigger_7th);
	DDX_Text(pDX, IDC_EDIT19, m_PostTrigger_8th);
	DDX_Text(pDX, IDC_EDIT20, m_PostTrigger_9th);
	DDX_Text(pDX, IDC_EDIT21, m_PostTrigger_10th);
	DDX_Text(pDX, IDC_EDIT1, m_StimulationLatencyBias_InMicroS);
	DDX_Text(pDX, IDC_EDIT22, m_Chan_11th);
	DDX_Text(pDX, IDC_EDIT23, m_Chan_12th);
	DDX_Text(pDX, IDC_EDIT24, m_Chan_13th);
	DDX_Text(pDX, IDC_EDIT25, m_Chan_14th);
	DDX_Text(pDX, IDC_EDIT26, m_Chan_15th);
	DDX_Text(pDX, IDC_EDIT27, m_Chan_16th);
	DDX_Text(pDX, IDC_EDIT28, m_Chan_17th);
	DDX_Text(pDX, IDC_EDIT29, m_Chan_18th);
	DDX_Text(pDX, IDC_EDIT30, m_Chan_19th);
	DDX_Text(pDX, IDC_EDIT31, m_Chan_20th);
	DDX_Text(pDX, IDC_EDIT32, m_PostTrigger_11th);
	DDX_Text(pDX, IDC_EDIT33, m_PostTrigger_12th);
	DDX_Text(pDX, IDC_EDIT34, m_PostTrigger_13th);
	DDX_Text(pDX, IDC_EDIT35, m_PostTrigger_14th);
	DDX_Text(pDX, IDC_EDIT36, m_PostTrigger_15th);
	DDX_Text(pDX, IDC_EDIT37, m_PostTrigger_16th);
	DDX_Text(pDX, IDC_EDIT38, m_PostTrigger_17th);
	DDX_Text(pDX, IDC_EDIT39, m_PostTrigger_18th);
	DDX_Text(pDX, IDC_EDIT40, m_PostTrigger_19th);
	DDX_Text(pDX, IDC_EDIT41, m_PostTrigger_20th);
}


BEGIN_MESSAGE_MAP(CPropertiesDialog, CDialogEx)
END_MESSAGE_MAP()


// CPropertiesDialog message handlers
