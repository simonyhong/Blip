// PitchRollYaw.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "PitchRollYaw.h"
#include "afxdialogex.h"


// CPitchRollYaw dialog

IMPLEMENT_DYNAMIC(CPitchRollYaw, CDialogEx)

CPitchRollYaw::CPitchRollYaw(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPitchRollYaw::IDD, pParent)
	, m_GridPitch(0)
	, m_GridRoll(0)
	, m_GridYaw(0)
{

}

CPitchRollYaw::~CPitchRollYaw()
{
}

void CPitchRollYaw::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_GridPitch);
	DDX_Text(pDX, IDC_EDIT2, m_GridRoll);
	DDX_Text(pDX, IDC_EDIT3, m_GridYaw);
}


BEGIN_MESSAGE_MAP(CPitchRollYaw, CDialogEx)
END_MESSAGE_MAP()


// CPitchRollYaw message handlers
