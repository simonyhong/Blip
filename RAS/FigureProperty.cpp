// FigureProperty.cpp : implementation file
//

#include "stdafx.h"
#include "RAS.h"
#include "FigureProperty.h"


// CFigureProperty dialog

IMPLEMENT_DYNAMIC(CFigureProperty, CDialog)

CFigureProperty::CFigureProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CFigureProperty::IDD, pParent)
	, m_ChannID(0)
	, m_NeuronID(0)
	, Give_Nickname_VarItself_FigIdx_FrontOrBack_RunLine(0)
	, m_PreAlignPeriod_ms(0)
	, m_dSigma(0)
{

}

CFigureProperty::~CFigureProperty()
{
}

void CFigureProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1_CHANNEL, m_ChannID);
	DDV_MinMaxInt(pDX, m_ChannID, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT2_NEURON, m_NeuronID);
	DDV_MinMaxInt(pDX, m_NeuronID, 0, 2);
	DDX_Text(pDX, IDC_EDIT3_ALIGNMENT, Give_Nickname_VarItself_FigIdx_FrontOrBack_RunLine);
	DDX_Text(pDX, IDC_EDIT4_BEFORE_ALIGNMENT_TIME, m_PreAlignPeriod_ms);
	DDX_Text(pDX, IDC_EDIT5_GAUSSIAN_SIGMA, m_dSigma);
}


BEGIN_MESSAGE_MAP(CFigureProperty, CDialog)
END_MESSAGE_MAP()


// CFigureProperty message handlers
