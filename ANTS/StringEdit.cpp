// StringEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ANTS.h"
#include "StringEdit.h"


// StringEdit dialog

IMPLEMENT_DYNAMIC(StringEdit, CDialog)

StringEdit::StringEdit(CWnd* pParent /*=NULL*/)
	: CDialog(StringEdit::IDD, pParent)
	, m_UserInputString1(_T(""))
{

}

StringEdit::~StringEdit()
{
}

void StringEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRING1, m_UserInputString1);
}


BEGIN_MESSAGE_MAP(StringEdit, CDialog)
END_MESSAGE_MAP()


// StringEdit message handlers
