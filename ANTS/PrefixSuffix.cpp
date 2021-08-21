// PrefixSuffix.cpp : implementation file
//

#include "stdafx.h"
#include "ANTS.h"
#include "PrefixSuffix.h"


// CPrefixSuffix dialog

IMPLEMENT_DYNAMIC(CPrefixSuffix, CDialog)

CPrefixSuffix::CPrefixSuffix(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefixSuffix::IDD, pParent)
	, m_PrefixSuffix(_T(""))
{

}

CPrefixSuffix::~CPrefixSuffix()
{
}

void CPrefixSuffix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_PrefixSuffix);
}


BEGIN_MESSAGE_MAP(CPrefixSuffix, CDialog)
END_MESSAGE_MAP()


// CPrefixSuffix message handlers
