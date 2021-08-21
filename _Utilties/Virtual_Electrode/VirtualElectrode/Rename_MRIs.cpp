// Rename_MRIs.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "Rename_MRIs.h"
#include "afxdialogex.h"


// CRename_MRIs dialog

IMPLEMENT_DYNAMIC(CRename_MRIs, CDialogEx)

CRename_MRIs::CRename_MRIs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRename_MRIs::IDD, pParent)
	, m_Coronal_1_Sagittal_2(0)
	, m_Multiplication(0)
	, m_Addition(0)
{

}

CRename_MRIs::~CRename_MRIs()
{
}

void CRename_MRIs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_Coronal_1_Sagittal_2);
	DDX_Text(pDX, IDC_EDIT1, m_Multiplication);
	DDX_Text(pDX, IDC_EDIT2, m_Addition);
}


BEGIN_MESSAGE_MAP(CRename_MRIs, CDialogEx)
END_MESSAGE_MAP()


// CRename_MRIs message handlers
