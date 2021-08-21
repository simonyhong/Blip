#include "stdafx.h"
#include "XCOR.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "XCOR_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "XCOR_View.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "Dialog_Box.h"
#include "StringEdit.h"
#include "AcceptOneNum.h"
#include "Trial.h"
#include "VarDefinitionsSDF.h"


void CXCOR_View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CXCOR_View::OnEnChangeEditString1()
{
	StringEdit	ParameterBOX;
	char Source[1024]; 

	if(m_CallingFrom==1)ParameterBOX.m_UserInputString1=m_GlobalTitle;
	m_IsEditOkayed=0;
	if(ParameterBOX.DoModal()==IDOK){
		m_IsEditOkayed=1;
		if(m_CallingFrom==1)m_GlobalTitle=ParameterBOX.m_UserInputString1;
	}
}

#include "Dialog_Ref_n_Sbj_Idx.h"
void CXCOR_View::Set_Ref_and_Sbj_Indexes()
{
	CRef_n_Sbj_Indexes	ParameterBOX;
	ParameterBOX.m_XCOR_RefChan=m_pSpotlightFig->m_XCOR_Label.m_AssociatedVar;
	ParameterBOX.m_Ref_NnID		=m_pSpotlightFig->m_XCOR_Label.status;
	ParameterBOX.m_XCOR_SbjChan=m_pSpotlightFig->m_XCOR_Label.m_Var2;
	ParameterBOX.m_Sub_NnID		=m_pSpotlightFig->m_XCOR_Label.status2;
	m_IsEditOkayed=0;
	m_Ref_AndOr_SbjChnaged=0;
	if(ParameterBOX.DoModal()==IDOK){
		char tmpStr[64];
		m_IsEditOkayed=1;
		m_pSpotlightFig->m_XCOR_Label.m_AssociatedVar	=ParameterBOX.m_XCOR_RefChan;
		m_pSpotlightFig->m_XCOR_Label.status			=ParameterBOX.m_Ref_NnID;
		m_pSpotlightFig->m_XCOR_Label.m_Var2			=ParameterBOX.m_XCOR_SbjChan;
		m_pSpotlightFig->m_XCOR_Label.status2			=ParameterBOX.m_Sub_NnID;
		if(m_XCOR_RefChan !=ParameterBOX.m_XCOR_RefChan){
			m_Ref_AndOr_SbjChnaged=1;
			m_XCOR_RefChan	=ParameterBOX.m_XCOR_RefChan;
		}
		if(m_XCOR_RefNn	!=ParameterBOX.m_Ref_NnID){
			m_Ref_AndOr_SbjChnaged=1;
			m_XCOR_RefNn =ParameterBOX.m_Ref_NnID;
		}
		if(m_XCOR_SbjChan !=ParameterBOX.m_XCOR_SbjChan){
			m_Ref_AndOr_SbjChnaged=1;
			m_XCOR_SbjChan	=ParameterBOX.m_XCOR_SbjChan;
		}
		if(m_XCOR_SbjNn !=ParameterBOX.m_Sub_NnID){
			m_Ref_AndOr_SbjChnaged=1;
			m_XCOR_SbjNn =ParameterBOX.m_Sub_NnID;
		}
	}
}


