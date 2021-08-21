// MOXY_View.cpp : implementation of the CMOXY_View class
//

#include "stdafx.h"
#include "MOXY.h"

#include<stdlib.h>
#include<stdio.h>

#include "Dialog_PrefixSuffix.h"
#include "Dialog_TriggerECode.h"
#include "Dialog_AxisScale.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"

#include "Dialog_AcceptOneString.h"



void CMOXY_View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	char buffer[21];
	switch(nChar){
		case VK_TAB: 
			PostMessage(WM_MYMESSAGE_AUTO_DIO_ON_OFF, 300/*duration*/, 0);
			sprintf(buffer,"User-clicked DO: %d", 0);
			Name_and_Value_to_record(buffer,300);
			break;
		case VK_UP: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				M_EyeY_Gain[*M_BehavioralObjectForXYmonitor]+=0.05;
			}
			else if(HIBYTE(GetKeyState(VK_SHIFT))){
				M_EyeY_Bias[*M_BehavioralObjectForXYmonitor]+=0.2;
			}
			break;
		case VK_DOWN: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				if(M_EyeY_Gain[*M_BehavioralObjectForXYmonitor]>0.05)M_EyeY_Gain[*M_BehavioralObjectForXYmonitor]-=0.05;
			}
			else if(HIBYTE(GetKeyState(VK_SHIFT))){
				M_EyeY_Bias[*M_BehavioralObjectForXYmonitor]-=0.2;
			}
			break;
		case VK_LEFT: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				if(M_EyeX_Gain[*M_BehavioralObjectForXYmonitor]>0.05)M_EyeX_Gain[*M_BehavioralObjectForXYmonitor]-=0.05;
			}
			else if(HIBYTE(GetKeyState(VK_SHIFT))){
				M_EyeX_Bias[*M_BehavioralObjectForXYmonitor]-=0.1;
			}
			break;
		case VK_RIGHT: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				M_EyeX_Gain[*M_BehavioralObjectForXYmonitor]+=0.05;
			}
			else if(HIBYTE(GetKeyState(VK_SHIFT))){
				M_EyeX_Bias[*M_BehavioralObjectForXYmonitor]+=0.1;
			}
			break;		
		case VK_SPACE: 
			break;
		case VK_DELETE: 
			break;
		default:
			break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMOXY_View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar){
		case VK_UP: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				Save_MOXY_Config();
			}
			break;
		case VK_DOWN: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				Save_MOXY_Config();
			}
			break;
		case VK_LEFT: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				Save_MOXY_Config();
			}
			break;
		case VK_RIGHT: 
			if(HIBYTE(GetKeyState(VK_CONTROL))){
				Save_MOXY_Config();
			}
			break;		
		default:
			break;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CMOXY_View::OnAxisScale(CFigure *pFig) 
{
	CAxisScale	ParameterBOX;
	
	ParameterBOX.m_dMinX  = pFig->m_dMinX;
	ParameterBOX.m_dMaxX  = pFig->m_dMaxX;
	ParameterBOX.m_dMinY  = pFig->m_dMinY;
	ParameterBOX.m_dMaxY  = pFig->m_dMaxY;
	ParameterBOX.m_dMinY2 = pFig->m_dMinY2;
	ParameterBOX.m_dMaxY2 = pFig->m_dMaxY2;

	if(ParameterBOX.DoModal()==IDOK){
		pFig->m_dMinX =ParameterBOX.m_dMinX;
		pFig->m_dMaxX =ParameterBOX.m_dMaxX;
		pFig->m_dMinY =ParameterBOX.m_dMinY;
		pFig->m_dMaxY =ParameterBOX.m_dMaxY;
		pFig->m_dMinY2=ParameterBOX.m_dMinY2;
		pFig->m_dMaxY2=ParameterBOX.m_dMaxY2;
	}
}

void CMOXY_View::EditTriggerECode(CRunningLineDisplay *pMonitorFig)
{
	CTriggerECode	TriggerEditBox;

	TriggerEditBox.m_sTriggerEcode=pMonitorFig->m_sTriggerEcode;
	if(TriggerEditBox.DoModal()==IDOK){
		char TriggerECode[256];
		CString ChanStr=TriggerEditBox.m_sTriggerEcode;ChanStr.Trim();
		strcpy(TriggerECode,ChanStr);
		pMonitorFig->m_sTriggerEcode=TriggerECode;
		if(strcmp("",TriggerECode)==0||strcmp("None",TriggerECode)==0){
			*(pMonitorFig->M_IsTriggeredMode)=0; return;
		}
		int i, found=0;
		for(i=0;i<(*M_NumEcodesInTable);i++){
			if(strcmp(&M_EcodeTable[i].M_EcodeAlias[0],TriggerECode)==0){
				*(pMonitorFig->M_nTriggerECode)=M_EcodeTable[i].m_EcodeItself; found=1; break;
			}
		}
		if(found==1){*(pMonitorFig->M_IsTriggeredMode)=1; return;}
		int TmpID;
		if(Find_Last_PositiveNum_inString(TriggerECode, TmpID)==-1){pMonitorFig->m_sTriggerEcode="None"; return;}
		*(pMonitorFig->M_nTriggerECode)=TmpID;
		*(pMonitorFig->M_IsTriggeredMode)=1;
	}
}


void CMOXY_View::EditPrefixSuffix()
{
	CPrefixSuffix	PrefixSuffix;
	char TmpStr[256];
	PrefixSuffix.m_Prefix=m_PreFix;
	PrefixSuffix.m_Suffix=m_SuFFix;
	PrefixSuffix.m_NextNeuronNumber=*M_NnNumberToRecord;
	if(PrefixSuffix.DoModal()==IDOK){
		strcpy(m_PreFix,PrefixSuffix.m_Prefix);
		strcpy(m_SuFFix,PrefixSuffix.m_Suffix);
		*M_NnNumberToRecord=PrefixSuffix.m_NextNeuronNumber;
	}
	Make_TheBaseOfFileName(); //This requests re-display of the name
	*M_isRenamed=1;//This is to inform the TAS in case it is already saving to a file.
}


#include "Dialog_AcceptOneString.h"
void CMOXY_View::AcceptOneString(CRunningLineDisplay *pMonitorFig)
{
	CAcceptOneString	OneString;
	char Source[256];
	if(m_callingFrom==0)sprintf(Source,"Chan: %d",pMonitorFig->m_RecordingSourceChanID);
	else if(m_callingFrom==1)sprintf(Source,"Nn: %d",pMonitorFig->m_RL_RecordingSourceNn);

	OneString.m_AcceptOneString+=Source;
	if(OneString.DoModal()==IDOK){
		strcpy(Source,OneString.m_AcceptOneString);
		int TmpID;
		if(Find_Last_PositiveNum_inString(Source, TmpID)==-1)return;
		if(m_callingFrom==0){
			if(TmpID>=0 && TmpID<NUM_NN_AI_CHAN)pMonitorFig->m_RecordingSourceChanID=TmpID;
			else AfxMessageBox("TmpID<0 || TmpID>=NUM_NN_AI_CHAN");
		}
		else if(m_callingFrom==1){
			if(TmpID>=0 && TmpID<3)pMonitorFig->m_RL_RecordingSourceNn=TmpID;
			else AfxMessageBox("TmpID<0 || TmpID>=3");
		}
	}
}

void CMOXY_View::Accept_ChannelID_forXY_disp(CRunningLineDisplay *pMonitorFig)
{
	CAcceptOneString	OneString;
	char Source[256];
	sprintf(Source,"Behavioral Chan: %d",pMonitorFig->m_RecordingSourceChanID);
	OneString.m_AcceptOneString+=Source;

	if(OneString.DoModal()==IDOK){
		strcpy(Source,OneString.m_AcceptOneString);
		int TmpID;
		if(Find_Last_PositiveNum_inString(Source, TmpID)==-1)return;
		if(m_callingFrom==0){
			if(TmpID>=0 && TmpID<NUM_NN_AI_CHAN)pMonitorFig->m_RecordingSourceChanID=TmpID;
			else AfxMessageBox("TmpID<0 || TmpID>=NUM_NN_AI_CHAN");
		}
		else if(m_callingFrom==1){
			if(TmpID>=0 && TmpID<3)pMonitorFig->m_RL_RecordingSourceNn=TmpID;
			else AfxMessageBox("TmpID<0 || TmpID>=3");
		}
	}
}

#include "Dialog_Visiable_Objs.h"
void CMOXY_View::OnView_VisibleObjs()
{
	Dialog_Visiable_Objs	VisibleObjs;
	VisibleObjs.m_IsVisible_1 =M_IsVisibleObj[0];
	VisibleObjs.m_IsVisible_2 =M_IsVisibleObj[1];
	VisibleObjs.m_IsVisible_3 =M_IsVisibleObj[2];
	VisibleObjs.m_IsVisible_4 =M_IsVisibleObj[3];
	VisibleObjs.m_IsVisible_5 =M_IsVisibleObj[4];
	VisibleObjs.m_IsVisible_6 =M_IsVisibleObj[5];
	VisibleObjs.m_IsVisible_7 =M_IsVisibleObj[6];
	VisibleObjs.m_IsVisible_8 =M_IsVisibleObj[7];
	VisibleObjs.m_IsVisible_9 =M_IsVisibleObj[8];
	VisibleObjs.m_IsVisible_10=M_IsVisibleObj[9];
	if(VisibleObjs.DoModal()==IDOK){
		M_IsVisibleObj[0]=VisibleObjs.m_IsVisible_1 ;
		M_IsVisibleObj[1]=VisibleObjs.m_IsVisible_2 ;
		M_IsVisibleObj[2]=VisibleObjs.m_IsVisible_3 ;
		M_IsVisibleObj[3]=VisibleObjs.m_IsVisible_4 ;
		M_IsVisibleObj[4]=VisibleObjs.m_IsVisible_5 ;
		M_IsVisibleObj[5]=VisibleObjs.m_IsVisible_6 ;
		M_IsVisibleObj[6]=VisibleObjs.m_IsVisible_7 ;
		M_IsVisibleObj[7]=VisibleObjs.m_IsVisible_8 ;
		M_IsVisibleObj[8]=VisibleObjs.m_IsVisible_9 ;
		M_IsVisibleObj[9]=VisibleObjs.m_IsVisible_10;
	}
}
