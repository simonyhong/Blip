// AcceptOneDouble.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "AcceptOneDouble.h"
#include "afxdialogex.h"


// CAcceptOneDouble dialog

IMPLEMENT_DYNAMIC(CAcceptOneDouble, CDialogEx)

CAcceptOneDouble::CAcceptOneDouble(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAcceptOneDouble::IDD, pParent)
	, m_DoubleValue(0)
{

}

CAcceptOneDouble::~CAcceptOneDouble()
{
}

void CAcceptOneDouble::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_DoubleValue);
}


BEGIN_MESSAGE_MAP(CAcceptOneDouble, CDialogEx)
END_MESSAGE_MAP()


// CAcceptOneDouble message handlers
