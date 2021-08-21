// ChooseSourceDir.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "Dialog_ChooseSourceDir.h"
#include "afxdialogex.h"


// CChooseSourceDir dialog

IMPLEMENT_DYNAMIC(CChooseSourceDir, CDialogEx)

CChooseSourceDir::CChooseSourceDir(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChooseSourceDir::IDD, pParent)
	, m_CandidateDirectory(_T(""))
	, m_UseCandidateDirectory(0)//This value controls which radio is active at the begining.
	//, m_LetMeChoose(false)
{
}

CChooseSourceDir::~CChooseSourceDir()
{
}

void CChooseSourceDir::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CandidateDirectory);
	DDX_Radio(pDX, IDC_RADIO1, m_UseCandidateDirectory);//This links the radio button (this one is the only one having the "grup" property activated) to the variable.
}


BEGIN_MESSAGE_MAP(CChooseSourceDir, CDialogEx)
END_MESSAGE_MAP()


// CChooseSourceDir message handlers


