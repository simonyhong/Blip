// ClassicalConditioning.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "DialogueCC.h"


// CClassicalConditioning dialog

IMPLEMENT_DYNAMIC(CClassicalConditioning, CDialog)

CClassicalConditioning::CClassicalConditioning(CWnd* pParent /*=NULL*/)
	: CDialog(CClassicalConditioning::IDD, pParent)
	, m_CCTaskType(0)
	, m_IsAvoidence(0)
	, m_CenterOfAvoidenceBox_X(0)
	, m_CenterOfAvoidenceBox_Y(0)
	, m_SizeOfAvoidanceBox(0)
	, m_CSduration(0)
	, m_RWDduration(0)
	, m_PuffDuration(0)
	, m_FixedPart_ITI(0)
	, m_RandomPart_ITI(0)
{

}

CClassicalConditioning::~CClassicalConditioning()
{
}

void CClassicalConditioning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CCTaskType);
	DDX_Text(pDX, IDC_EDIT2, m_IsAvoidence);
	DDX_Text(pDX, IDC_EDIT3, m_CenterOfAvoidenceBox_X);
	DDX_Text(pDX, IDC_EDIT4, m_CenterOfAvoidenceBox_Y);
	DDX_Text(pDX, IDC_EDIT5, m_SizeOfAvoidanceBox);
	DDX_Text(pDX, IDC_EDIT6, m_CSduration);
	DDX_Text(pDX, IDC_EDIT7, m_RWDduration);
	DDX_Text(pDX, IDC_EDIT8, m_PuffDuration);
	DDX_Text(pDX, IDC_EDIT9, m_FixedPart_ITI);
	DDX_Text(pDX, IDC_EDIT10, m_RandomPart_ITI);
}


BEGIN_MESSAGE_MAP(CClassicalConditioning, CDialog)
END_MESSAGE_MAP()


// CClassicalConditioning message handlers
