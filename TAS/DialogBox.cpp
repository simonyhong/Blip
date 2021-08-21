// DialogueBox.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "DialogueBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogueBox dialog


CDialogueBox::CDialogueBox(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogueBox::IDD, pParent)
	, m_Variable4(0)
	, m_Variable5(0)
	, m_Variable6(0)
	, m_Variable7(0)
	, m_Variable8(0)
	, m_Variable9(0)
	, m_Variable10(0)
	, m_Variable11(0)
	, m_Variable12(0)
	, m_Variable13(0)
	, m_Variable14(0)
	, m_Variable15(0)
	, m_Variable16(0)
	, m_Variable17(0)
	, m_Variable18(0)
	, m_Variable19(0)
	, m_Variable20(0)
	, m_Variable21(0)
	, m_Variable22(0)
	, m_Check1(FALSE)
	, m_Check2(FALSE)
	, m_Check3(FALSE)
	, m_Variable23(0)
{
	//{{AFX_DATA_INIT(CDialogueBox)
	m_dVariable1 = 0.0;
	m_dVariable2 = 0.0;
	m_dVariable3 = 0.0;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_TaskType=0;
}


void CDialogueBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogueBox)
	DDX_Text(pDX, IDC_Variable_1, m_dVariable1);
	DDX_Text(pDX, IDC_Variable_2, m_dVariable2);
	DDX_Text(pDX, IDC_Variable_3, m_dVariable3);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_Variable_4, m_Variable4);
	DDX_Text(pDX, IDC_Variable_5, m_Variable5);
	DDX_Text(pDX, IDC_Variable_6, m_Variable6);
	DDX_Text(pDX, IDC_Variable_7, m_Variable7);
	DDX_Text(pDX, IDC_Variable_8, m_Variable8);
	DDX_Text(pDX, IDC_Variable_9, m_Variable9);
	DDX_Text(pDX, IDC_Variable_10, m_Variable10);
	DDX_Text(pDX, IDC_Variable_11, m_Variable11);
	DDX_Text(pDX, IDC_Variable_12, m_Variable12);
	DDX_Text(pDX, IDC_Variable_13, m_Variable13);
	DDX_Text(pDX, IDC_Variable_14, m_Variable14);
	DDX_Text(pDX, IDC_Variable_15, m_Variable15);
	DDX_Text(pDX, IDC_Variable_16, m_Variable16);
	DDX_Text(pDX, IDC_Variable_17, m_Variable17);
	DDX_Text(pDX, IDC_Variable_18, m_Variable18);
	DDX_Text(pDX, IDC_Variable_19, m_Variable19);
	DDX_Text(pDX, IDC_Variable_20, m_Variable20);
	DDX_Text(pDX, IDC_Variable_21, m_Variable21);
	DDX_Text(pDX, IDC_Variable_22, m_Variable22);
	DDX_Text(pDX, IDC_Variable_23, m_Variable23);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);

	DDX_Control(pDX, IDC_COMBO1, m_TaskCombo);

}



BEGIN_MESSAGE_MAP(CDialogueBox, CDialog)
	//{{AFX_MSG_MAP(CDialogueBox)
	ON_EN_CHANGE(IDC_Variable_1, OnChangeVariable1)
	ON_EN_CHANGE(IDC_Variable_2, OnChangeVariable2)
	ON_EN_CHANGE(IDC_Variable_3, OnChangeVariable3)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_Variable_4, &CDialogueBox::OnEnChangeVariable4)
	ON_EN_CHANGE(IDC_Variable_5, &CDialogueBox::OnEnChangeVariable5)
	ON_EN_CHANGE(IDC_Variable_6, &CDialogueBox::OnEnChangeVariable6)
	ON_EN_CHANGE(IDC_Variable_7, &CDialogueBox::OnEnChangeVariable7)
	ON_EN_CHANGE(IDC_Variable_8, &CDialogueBox::OnEnChangeVariable8)
	ON_EN_CHANGE(IDC_Variable_9, &CDialogueBox::OnEnChangeVariable9)
	ON_EN_CHANGE(IDC_Variable_10, &CDialogueBox::OnEnChangeVariable10)
	ON_EN_CHANGE(IDC_Variable_11, &CDialogueBox::OnEnChangeVariable11)
	ON_EN_CHANGE(IDC_Variable_12, &CDialogueBox::OnEnChangeVariable12)
	ON_EN_CHANGE(IDC_Variable_13, &CDialogueBox::OnEnChangeVariable13)
	ON_EN_CHANGE(IDC_Variable_14, &CDialogueBox::OnEnChangeVariable14)
	ON_EN_CHANGE(IDC_Variable_15, &CDialogueBox::OnEnChangeVariable15)
	ON_EN_CHANGE(IDC_Variable_16, &CDialogueBox::OnEnChangeVariable16)
	ON_EN_CHANGE(IDC_Variable_17, &CDialogueBox::OnEnChangeVariable17)
	ON_EN_CHANGE(IDC_Variable_18, &CDialogueBox::OnEnChangeVariable18)
	ON_EN_CHANGE(IDC_Variable_19, &CDialogueBox::OnEnChangeVariable19)
	ON_EN_CHANGE(IDC_Variable_20, &CDialogueBox::OnEnChangeVariable20)
	ON_EN_CHANGE(IDC_Variable_21, &CDialogueBox::OnEnChangeVariable21)
	ON_EN_CHANGE(IDC_Variable_22, &CDialogueBox::OnEnChangeVariable22)
	ON_BN_CLICKED(IDC_CHECK1, &CDialogueBox::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDialogueBox::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CDialogueBox::OnBnClickedCheck3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDialogueBox::OnCbnSelchangeTaskCombo)
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


//void CDialogueBox::OnStnClickedV2()
//{
//	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
//}

void CDialogueBox::OnEnChangeVariable4()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable5()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable6()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable7()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable8()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable9()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable10()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable11()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable12()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable13()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable14()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable15()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable16()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable17()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable18()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable19()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable20()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable21()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnEnChangeVariable22()
{
	UpdateData(TRUE); //It accepts the value entered in the dialog to its attached variable.
}

void CDialogueBox::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler m_EventCode here
}

void CDialogueBox::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler m_EventCode here
}

void CDialogueBox::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler m_EventCode here
}


#include "Task_Constants.h"
BOOL CDialogueBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//If you would like to sort the menu, change the "property" of the combo box
	m_TaskCombo.AddString("1DR");
	m_TaskCombo.AddString("Saccade");	
	m_TaskCombo.AddString("My New Fix");	
	m_TaskCombo.AddString("Orthodromic Stim");
	m_TaskCombo.SetCurSel( m_TaskType );//It helps display the curretly selected item
	//To display the listed members: In the dialog layout designer, while designing the dialog, click the "down arrow" on the combobox. You can then drag down on the bottom of the combobox's outline to increase its height.

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDialogueBox::OnCbnSelchangeTaskCombo()
{
	CString strSelected;
	int index = m_TaskCombo.GetCurSel();

	if( index != CB_ERR )
	{
		m_TaskCombo.GetLBText(index, strSelected);
		if     (strSelected=="1DR"        )m_TaskType=ONEDR_TASK;
		else if(strSelected=="Saccade"    )m_TaskType=SCC_TASK;
		else if(strSelected=="My New Fix" )m_TaskType=MY_NEW_FIX_TASK;
		else if(strSelected=="Orthodromic Stim" )m_TaskType=ORTHO_STIM_TASK;
	}
}

