// PrefixSuffix.cpp : implementation file
//

#include "stdafx.h"
#include "TAS.h"
#include "Dialog_PrefixSuffix.h"


// CPrefixSuffix dialog

IMPLEMENT_DYNAMIC(CPrefixSuffix, CDialog)

CPrefixSuffix::CPrefixSuffix(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefixSuffix::IDD, pParent)
	, m_Prefix(_T(""))
	, m_Suffix(_T(""))
	, m_NextNeuronNumber(0)
{

}

CPrefixSuffix::~CPrefixSuffix()
{
}

void CPrefixSuffix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Prefix);
	DDX_Text(pDX, IDC_EDIT2, m_Suffix);
	DDX_Text(pDX, IDC_EDIT3, m_NextNeuronNumber);
}


BEGIN_MESSAGE_MAP(CPrefixSuffix, CDialog)
END_MESSAGE_MAP()


// CPrefixSuffix message handlers
