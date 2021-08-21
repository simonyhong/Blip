// CAxisScale.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "Dialog_AxisScale.h"


// CAxisScale dialog

IMPLEMENT_DYNAMIC(CAxisScale, CDialog)

CAxisScale::CAxisScale(CWnd* pParent /*=NULL*/)
	: CDialog(CAxisScale::IDD, pParent)
	, m_dMaxY(0)
	, m_dMinY(0)
	, m_dMinX(0)
	, m_dMaxX(0)
	, m_dMaxY2(0)
	, m_dMinY2(0)
{

}

CAxisScale::~CAxisScale()
{
}

void CAxisScale::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAX_Y, m_dMaxY);
	DDX_Text(pDX, IDC_MIN_Y, m_dMinY);
	DDX_Text(pDX, IDC_MIN_X, m_dMinX);
	DDX_Text(pDX, IDC_MAX_X, m_dMaxX);
	DDX_Text(pDX, IDC_EDIT1, m_dMaxY2);
	DDX_Text(pDX, IDC_EDIT3, m_dMinY2);
}


BEGIN_MESSAGE_MAP(CAxisScale, CDialog)
END_MESSAGE_MAP()


// CAxisScale message handlers
