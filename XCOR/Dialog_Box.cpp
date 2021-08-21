// DialogueBox.cpp : implementation file
//

#include "stdafx.h"
#include "XCOR.h"
#include "Dialog_Box.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogueBox dialog


CDialogueBox::CDialogueBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogueBox::IDD, pParent)
	, m_dMinimumY(0)
{
	//{{AFX_DATA_INIT(CDialogueBox)
	m_dVariable1 = 0.0;
	m_dVariable2 = 0.0;
	m_dVariable3 = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogueBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogueBox)
	DDX_Text(pDX, IDC_Variable_1, m_dVariable1);
	DDX_Text(pDX, IDC_Variable_2, m_dVariable2);
	DDX_Text(pDX, IDC_Variable_3, m_dVariable3);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MinimumY, m_dMinimumY);
}


BEGIN_MESSAGE_MAP(CDialogueBox, CDialog)
	//{{AFX_MSG_MAP(CDialogueBox)
	ON_EN_CHANGE(IDC_Variable_1, OnChangeVariable1)
	ON_EN_CHANGE(IDC_Variable_2, OnChangeVariable2)
	ON_EN_CHANGE(IDC_Variable_3, OnChangeVariable3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogueBox message handlers

void CDialogueBox::OnChangeVariable1() 
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnChangeVariable2() 
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnChangeVariable3() 
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}
