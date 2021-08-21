#include "stdafx.h"
#include "RAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "ANTS_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "Channel.h"
#include "ANTS_View.h"
#include "SinTable.h"

//CDC		CAiChannel::m_ChDC;
//CBitmap CAiChannel::m_ChBmp;
CPen	CAiChannel::m_VIOLETLIGHT2;
int	CAiChannel::m_DiviceContaxtCreatedAlready=0;


CAiChannel::CAiChannel()
{
	m_idx_Spike=0;
	m_AlignmentEcode=0;
	m_firstConnectionToMap=1;
	m_SIGi=0;//m_SIGi points the next array index to process
	if(m_DiviceContaxtCreatedAlready==0){
		m_DiviceContaxtCreatedAlready=1;
		m_VIOLETLIGHT2.CreatePen(PS_SOLID, 2,RGB(200,0,250));
	}
}

CAiChannel::~CAiChannel()
{
	//m_ChDC.DeleteDC();
	//m_ChBmp.DeleteObject();
	m_VIOLETLIGHT2.DeleteObject();
}


