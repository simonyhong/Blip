// ActivationOrCollision.cpp : implementation file
//

#include "stdafx.h"
#include "ANTS.h"
#include "ActivationOrCollision.h"
#include "afxdialogex.h"


// CActivationOrCollision dialog

IMPLEMENT_DYNAMIC(CActivationOrCollision, CDialogEx)

CActivationOrCollision::CActivationOrCollision(CWnd* pParent /*=NULL*/)
	: CDialogEx(CActivationOrCollision::IDD, pParent)
{

}

CActivationOrCollision::~CActivationOrCollision()
{
}

void CActivationOrCollision::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CActivationOrCollision, CDialogEx)
END_MESSAGE_MAP()


// CActivationOrCollision message handlers
