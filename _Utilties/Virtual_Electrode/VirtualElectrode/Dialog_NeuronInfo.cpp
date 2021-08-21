// NeuronInfo_Dialog.cpp : implementation file
//

#include "stdafx.h"
#include "Analysis.h"
#include "Dialog_NeuronInfo.h"
#include "afxdialogex.h"


// CNeuronInfo dialog

IMPLEMENT_DYNAMIC(CNeuronInfo, CDialogEx)

CNeuronInfo::CNeuronInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNeuronInfo::IDD, pParent)
	, m_NeuronName(_T(""))
	, m_Note(_T(""))
	, m_Type(0)
	, m_Variable1(0)
	, m_Variable2(0)
	, m_Variable3(0)
	, m_Variable4(0)
	, m_Variable5(0)
	, m_Variable6(0)
	, m_Variable7(0)
	, m_Variable8(0)
	, m_Variable9(0)
	, m_Variable10(0)
	, m_Varuable11(0)
	, m_Variable12(0)
{

}

CNeuronInfo::~CNeuronInfo()
{
}

void CNeuronInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_NeuronName);
	DDV_MaxChars(pDX, m_NeuronName, 64);
	DDX_Text(pDX, IDC_EDIT2, m_Note);
	DDV_MaxChars(pDX, m_Note, 1024);
	DDX_Text(pDX, IDC_EDIT3, m_Type);
	DDX_Text(pDX, IDC_EDIT4, m_Variable1);
	DDX_Text(pDX, IDC_EDIT5, m_Variable2);
	DDX_Text(pDX, IDC_EDIT6, m_Variable3);
	DDX_Text(pDX, IDC_EDIT7, m_Variable4);
	DDX_Text(pDX, IDC_EDIT8, m_Variable5);
	DDX_Text(pDX, IDC_EDIT9, m_Variable6);
	DDX_Text(pDX, IDC_EDIT10, m_Variable7);
	DDX_Text(pDX, IDC_EDIT11, m_Variable8);
	DDX_Text(pDX, IDC_EDIT12, m_Variable9);
	DDX_Text(pDX, IDC_EDIT13, m_Variable10);
}


BEGIN_MESSAGE_MAP(CNeuronInfo, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CNeuronInfo::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CNeuronInfo message handlers


void CNeuronInfo::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.



	// TODO:  Add your control notification handler code here
}
