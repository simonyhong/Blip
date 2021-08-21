
#include "stdafx.h"
#include "ANTS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ANTS_Doc.h"
#include "SharedConstants.h"
#include "Channel.h"
#include "Figure.h"
#include "ANTS_View.h"
#include "PropertiesDialog.h"
#include "DialogueBox.h"
#include "StringEdit.h"
#include "PrefixSuffix.h"


void CAntsView::OnEnChangeEditString1()
{
	StringEdit	ParameterBOX;

	ReEnter:
	if(m_CallingFrom==1)ParameterBOX.m_UserInputString1=m_GlobalTitle;
	//else if(m_CallingFrom==2)ParameterBOX.m_UserInputString1=m_pSpotlightFig->m_RecordingChan.NameString;
	else if(m_CallingFrom==3)ParameterBOX.m_UserInputString1=m_Lable_Of_SuperimposeNumber.NameString;
	else if(m_CallingFrom==4)ParameterBOX.m_UserInputString1=m_Lable_Of_TriggerChanNumber.NameString;
	if(ParameterBOX.DoModal()==IDOK){
		if(m_CallingFrom==1)m_GlobalTitle=ParameterBOX.m_UserInputString1;
		else if(m_CallingFrom==2){//Recording Channel
			AfxMessageBox("The specified channel number is out of the maximum (MAX_AI_CHAN-1).");//Some invalid channel number has been entered.
		}
		else if(m_CallingFrom==3){
			char ChanLabel[256];
			strcpy(ChanLabel,ParameterBOX.m_UserInputString1);
			m_EnteredChanID=Find_Last_PositiveNum_inString(ChanLabel);
			if(m_EnteredChanID>=0 && m_EnteredChanID<=6){
				m_Lable_Of_SuperimposeNumber.NameString=ParameterBOX.m_UserInputString1;
				m_Lable_Of_SuperimposeNumber.m_ChanID=m_EnteredChanID; 
				m_numberOfSuperimposeLines=m_EnteredChanID; 
				RequestDisplayEvent(REDRAW_SUPERIMPOSE_NUMBER);
			}
			else{AfxMessageBox("Enter between 1~6"); goto ReEnter;}//Some invalid channel number has been entered.

		}
		else if(m_CallingFrom==4){
			char ChanLabel[256];
			strcpy(ChanLabel,ParameterBOX.m_UserInputString1);
			m_EnteredChanID=Find_Last_PositiveNum_inString(ChanLabel);
			if(m_EnteredChanID>=0 && m_EnteredChanID<NUM_NN_AI_CHAN && M_NumOfFigsUsingThisChan[m_EnteredChanID]>0){
				if(M_NumOfFigsUsingThisChan[m_EnteredChanID]<=0 && m_bIsTo_ANTI_Stimulate==1){//Currently doing antidromic stimulation yet the user enters a wrong triggering channel id.
					AfxMessageBox("Hello there!\nThe specified recording channel is currently not used by DAS-client.\nTry again.");
					goto ReEnter;
				}				
				TriggerChannelName_Register(m_EnteredChanID);
				RequestDisplayEvent(REDRAW_TRIGGER_CHANNEL_NUMBER);
			}
			else{

				if(m_EnteredChanID<0 || m_EnteredChanID>=NUM_NN_AI_CHAN){AfxMessageBox("Hello there!\nThe specified recording channel number is out of the recording range 0~(MAX_AI_CHAN-1).");}//Some invalid channel number has been entered.
				else{AfxMessageBox("Hello there!\nThe specified recording channel is currently not used by DAS-client");}
			}
		}
	}
}

void CAntsView::TriggerChannelName_Register(int TrigChan)
{
	char TrigName[100];
	sprintf(TrigName,"TrigChan: %d ",TrigChan);
	m_Lable_Of_TriggerChanNumber.NameString=TrigName;
	m_Lable_Of_TriggerChanNumber.m_ChanID=TrigChan; 
	*M_AntidromicTriggeringChanID=TrigChan;
}

//This function takes from the Dialog Recording, Stimulation channels 
#include "Dialog_Record_Stim_Channels.h"
int CAntsView::Dialog_Record_Stimulation_Channels()
{
	Dialog_Record_Stim_Channels	ParameterBOX;

	ParameterBOX.m_RecordingChan=m_pSpotlightFig->m_RecordingChan.m_ChanID;
	ParameterBOX.m_StimChan=m_pSpotlightFig->m_StimGatingRelay.m_ChanID;
	ParameterBOX.m_IsAntidromicTriggeringChannel=m_pSpotlightFig->m_IsIt_TriggeringRecordingChannel;
	if(ParameterBOX.DoModal()==IDOK){
		char NameString[128], StimName[64];
		//####################   Recording channel
		if(ParameterBOX.m_RecordingChan<0 || ParameterBOX.m_RecordingChan>=NUM_NN_AI_CHAN){
			AfxMessageBox("Hello there\nInvalid Recording channel ID for Triggering.\n");
			return -1;
		}
		else{
			if(m_bIsTo_ANTI_Stimulate==1 && M_NumOfFigsUsingThisChan[ParameterBOX.m_RecordingChan]<=0){
				AfxMessageBox("Hello there!\nThe specified recording channel is currently not used by DAS-client."); 
				return -1;
			}
			m_pSpotlightFig->m_RecordingChan.m_ChanID=ParameterBOX.m_RecordingChan; 
		}

		//####################   Stimulation channel
		if(ParameterBOX.m_StimChan>=DO_FIRST_GATING_CHAN && ParameterBOX.m_StimChan<=DO_LAST_GATING_CHAN){
			if(IsThereAnyFigureHavingThisStimChannel(ParameterBOX.m_StimChan)==0 && m_NumOfStimulationChannels>=MAX_NUM_STIM_CHANNELS-1){//A new (previously non-existing) channel has been added to the stimulating channel. BUT the number of stimulation channels is to its maximum already. 
				AfxMessageBox("m_NumOfStimulationChannels>=MAX_NUM_STIM_CHANNELS-1\n  Too many alredy!!");
				 return -1;
			}
		}
		else{//Invalid stimulation Channel ID
			char MsgTmp[512];
			sprintf(MsgTmp,"Hello there!\n The Stimulation channel number is \n out of the currently supported range.\n %d ~ %d (DO_FIRST_GATING_CHAN ~ DO_LAST_GATING_CHAN)\n See SharedConstants.h for available channel numbers",DO_FIRST_GATING_CHAN,DO_LAST_GATING_CHAN);
			AfxMessageBox(MsgTmp);
			return -1;
		}
		m_pSpotlightFig->m_StimGatingRelay.m_ChanID=ParameterBOX.m_StimChan; 

		MakeRecordStimChanNameString(m_pSpotlightFig);

		return 1;
	}
	else{//ID_CANCEL
		return -1;
	}
}

void CAntsView::MakeRecordStimChanNameString(CFigure* pFig)
{
	char NameString[128];
	if(pFig->m_IsIt_TriggeringRecordingChannel==1)sprintf(NameString,"*TRIG* RecCh:%d StmRly:%d     ",pFig->m_RecordingChan.m_ChanID, pFig->m_StimGatingRelay.m_ChanID);
	else                                       sprintf(NameString,"       RecCh:%d StmRly:%d     "       ,pFig->m_RecordingChan.m_ChanID, pFig->m_StimGatingRelay.m_ChanID);
	pFig->m_RecordingChan.NameString=NameString;

}


void CAntsView::EditPrefixSuffix()
{
	CPrefixSuffix	PrefixSuffix;
	char TmpStr[256];
	sprintf(TmpStr,"Prefix: %s",m_PreFix);
	PrefixSuffix.m_PrefixSuffix=TmpStr;
	if(PrefixSuffix.DoModal()==IDOK){
		PrefixSuffix.m_PrefixSuffix.Replace("Prefix:",""); PrefixSuffix.m_PrefixSuffix.TrimLeft();
		strcpy(m_PreFix,PrefixSuffix.m_PrefixSuffix);
	}
	sprintf(TmpStr,"Suffix: %s",m_SuFFix);
	PrefixSuffix.m_PrefixSuffix=TmpStr;
	if(PrefixSuffix.DoModal()==IDOK){
		PrefixSuffix.m_PrefixSuffix.Replace("Suffix:",""); PrefixSuffix.m_PrefixSuffix.TrimLeft();
		strcpy(m_SuFFix,PrefixSuffix.m_PrefixSuffix);
	}
	sprintf(TmpStr,"Neuron #: %d",*M_NnNumberToRecord);
	PrefixSuffix.m_PrefixSuffix=TmpStr;
	if(PrefixSuffix.DoModal()==IDOK){
		int TmpID;
		strcpy(TmpStr,PrefixSuffix.m_PrefixSuffix);
		if(Find_Last_Num_inString(TmpStr, TmpID)==-1)return;
		*M_NnNumberToRecord=TmpID;
	}
	Make_TheBaseOfFileName(m_is_LFP_saving);
}


void CAntsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//if(m_nShowMode==GRAPH_3D){ThD.KeyDown(nChar); goto ToEND;}
	m_KeyCode=nChar;
	switch(nChar){
		case VK_DELETE:
			OnDeleteFigs();
			break;
		default: break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CAntsView::OnSetPrePostPeriods_of_OrthodromicStimulationLFP_ToSave()
{
	StringEdit	ParameterBOX;
	char orthoChar[64];
	//Receive m_LFP_Start_Xms_toSave from the user
	sprintf(orthoChar,"Record from, in ms  (e.g. -500):%d",m_LFP_Start_Xms_toSave);
	ParameterBOX.m_UserInputString1=orthoChar;
	if(ParameterBOX.DoModal()==IDOK){
		char UserNumber[256];
		strcpy(UserNumber,ParameterBOX.m_UserInputString1);
		if(Find_Last_Num_inString(UserNumber,m_LFP_Start_Xms_toSave)==-1)return;
	}
	//Receive m_LFP_End_Xms_toSave from the user
	sprintf(orthoChar,"Record to, in ms  (e.g. 500):%d",m_LFP_End_Xms_toSave);
	ParameterBOX.m_UserInputString1=orthoChar;
	if(ParameterBOX.DoModal()==IDOK){
		char UserNumber[256];
		strcpy(UserNumber,ParameterBOX.m_UserInputString1);
		if(Find_Last_Num_inString(UserNumber,m_LFP_End_Xms_toSave)==-1)return;
	}
	m_is_firstTimeOrthoStimulation_IntervalSetting=0;
}


void CAntsView::OnProperties()
{
	int StimGatingRelayID[MAX_NUM_STIM_CHANNELS], InterStimulationInterval[MAX_NUM_STIM_CHANNELS];
	for(int i=0; i<MAX_NUM_STIM_CHANNELS; i++){
		StimGatingRelayID[i]		=M_StimDigitalOutChannels[i];
		InterStimulationInterval[i]	=M_InterStimulationInterval[i];
	}

	CPropertiesDialog PropertyBox;
	if(MAX_NUM_STIM_CHANNELS>=1	)PropertyBox.m_Chan_1st		=M_StimDigitalOutChannels[0];	PropertyBox.m_PostTrigger_1st	=M_InterStimulationInterval[0];
	if(MAX_NUM_STIM_CHANNELS>=2	)PropertyBox.m_Chan_2nd		=M_StimDigitalOutChannels[1];	PropertyBox.m_PostTrigger_2nd	=M_InterStimulationInterval[1];
	if(MAX_NUM_STIM_CHANNELS>=3	)PropertyBox.m_Chan_3rd		=M_StimDigitalOutChannels[2];	PropertyBox.m_PostTrigger_3rd	=M_InterStimulationInterval[2];
	if(MAX_NUM_STIM_CHANNELS>=4	)PropertyBox.m_Chan_4th		=M_StimDigitalOutChannels[3];	PropertyBox.m_PostTrigger_4th	=M_InterStimulationInterval[3];
	if(MAX_NUM_STIM_CHANNELS>=5	)PropertyBox.m_Chan_5th		=M_StimDigitalOutChannels[4];	PropertyBox.m_PostTrigger_5th	=M_InterStimulationInterval[4];
	if(MAX_NUM_STIM_CHANNELS>=6	)PropertyBox.m_Chan_6th		=M_StimDigitalOutChannels[5];	PropertyBox.m_PostTrigger_6th	=M_InterStimulationInterval[5];
	if(MAX_NUM_STIM_CHANNELS>=7	)PropertyBox.m_Chan_7th		=M_StimDigitalOutChannels[6];	PropertyBox.m_PostTrigger_7th	=M_InterStimulationInterval[6];
	if(MAX_NUM_STIM_CHANNELS>=8	)PropertyBox.m_Chan_8th		=M_StimDigitalOutChannels[7];	PropertyBox.m_PostTrigger_8th	=M_InterStimulationInterval[7];
	if(MAX_NUM_STIM_CHANNELS>=9 )PropertyBox.m_Chan_9th		=M_StimDigitalOutChannels[8];	PropertyBox.m_PostTrigger_9th	=M_InterStimulationInterval[8];
	if(MAX_NUM_STIM_CHANNELS>=10)PropertyBox.m_Chan_10th	=M_StimDigitalOutChannels[9];	PropertyBox.m_PostTrigger_10th	=M_InterStimulationInterval[9];
	if(MAX_NUM_STIM_CHANNELS>=11)PropertyBox.m_Chan_11th	=M_StimDigitalOutChannels[10];	PropertyBox.m_PostTrigger_11th	=M_InterStimulationInterval[10];  
	if(MAX_NUM_STIM_CHANNELS>=12)PropertyBox.m_Chan_12th	=M_StimDigitalOutChannels[11];	PropertyBox.m_PostTrigger_12th	=M_InterStimulationInterval[11];
	if(MAX_NUM_STIM_CHANNELS>=13)PropertyBox.m_Chan_13th	=M_StimDigitalOutChannels[12];	PropertyBox.m_PostTrigger_13th	=M_InterStimulationInterval[12];
	if(MAX_NUM_STIM_CHANNELS>=14)PropertyBox.m_Chan_14th	=M_StimDigitalOutChannels[13];	PropertyBox.m_PostTrigger_14th	=M_InterStimulationInterval[13];
	if(MAX_NUM_STIM_CHANNELS>=15)PropertyBox.m_Chan_15th	=M_StimDigitalOutChannels[14];	PropertyBox.m_PostTrigger_15th	=M_InterStimulationInterval[14];
	if(MAX_NUM_STIM_CHANNELS>=16)PropertyBox.m_Chan_16th	=M_StimDigitalOutChannels[15];	PropertyBox.m_PostTrigger_16th	=M_InterStimulationInterval[15];
	if(MAX_NUM_STIM_CHANNELS>=17)PropertyBox.m_Chan_17th	=M_StimDigitalOutChannels[16];	PropertyBox.m_PostTrigger_17th	=M_InterStimulationInterval[16];
	if(MAX_NUM_STIM_CHANNELS>=18)PropertyBox.m_Chan_18th	=M_StimDigitalOutChannels[17];	PropertyBox.m_PostTrigger_18th	=M_InterStimulationInterval[17];
	if(MAX_NUM_STIM_CHANNELS>=19)PropertyBox.m_Chan_19th	=M_StimDigitalOutChannels[18];	PropertyBox.m_PostTrigger_19th	=M_InterStimulationInterval[18];
	if(MAX_NUM_STIM_CHANNELS>=20)PropertyBox.m_Chan_20th	=M_StimDigitalOutChannels[19];	PropertyBox.m_PostTrigger_20th	=M_InterStimulationInterval[19];
															 
	PropertyBox.m_StimulationLatencyBias_InMicroS	=*M_StimulationLatencyBias_InMicroS;
	if(PropertyBox.DoModal()==IDOK){
		int NumOfStimulationChannels=0;
		if(MAX_NUM_STIM_CHANNELS>=1	){StimGatingRelayID[0 ]=PropertyBox.m_Chan_1st ; InterStimulationInterval[0 ]=PropertyBox.m_PostTrigger_1st ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=2	){StimGatingRelayID[1 ]=PropertyBox.m_Chan_2nd ; InterStimulationInterval[1 ]=PropertyBox.m_PostTrigger_2nd ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=3	){StimGatingRelayID[2 ]=PropertyBox.m_Chan_3rd ; InterStimulationInterval[2 ]=PropertyBox.m_PostTrigger_3rd ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=4	){StimGatingRelayID[3 ]=PropertyBox.m_Chan_4th ; InterStimulationInterval[3 ]=PropertyBox.m_PostTrigger_4th ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=5	){StimGatingRelayID[4 ]=PropertyBox.m_Chan_5th ; InterStimulationInterval[4 ]=PropertyBox.m_PostTrigger_5th ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=6	){StimGatingRelayID[5 ]=PropertyBox.m_Chan_6th ; InterStimulationInterval[5 ]=PropertyBox.m_PostTrigger_6th ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=7	){StimGatingRelayID[6 ]=PropertyBox.m_Chan_7th ; InterStimulationInterval[6 ]=PropertyBox.m_PostTrigger_7th ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=8	){StimGatingRelayID[7 ]=PropertyBox.m_Chan_8th ; InterStimulationInterval[7 ]=PropertyBox.m_PostTrigger_8th ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=9 ){StimGatingRelayID[8 ]=PropertyBox.m_Chan_9th ; InterStimulationInterval[8 ]=PropertyBox.m_PostTrigger_9th ; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=10){StimGatingRelayID[9 ]=PropertyBox.m_Chan_10th; InterStimulationInterval[9 ]=PropertyBox.m_PostTrigger_10th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=11){StimGatingRelayID[10]=PropertyBox.m_Chan_11th; InterStimulationInterval[10]=PropertyBox.m_PostTrigger_11th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=12){StimGatingRelayID[11]=PropertyBox.m_Chan_12th; InterStimulationInterval[11]=PropertyBox.m_PostTrigger_12th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=13){StimGatingRelayID[12]=PropertyBox.m_Chan_13th; InterStimulationInterval[12]=PropertyBox.m_PostTrigger_13th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=14){StimGatingRelayID[13]=PropertyBox.m_Chan_14th; InterStimulationInterval[13]=PropertyBox.m_PostTrigger_14th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=15){StimGatingRelayID[14]=PropertyBox.m_Chan_15th; InterStimulationInterval[14]=PropertyBox.m_PostTrigger_15th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=16){StimGatingRelayID[15]=PropertyBox.m_Chan_16th; InterStimulationInterval[15]=PropertyBox.m_PostTrigger_16th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=17){StimGatingRelayID[16]=PropertyBox.m_Chan_17th; InterStimulationInterval[16]=PropertyBox.m_PostTrigger_17th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=18){StimGatingRelayID[17]=PropertyBox.m_Chan_18th; InterStimulationInterval[17]=PropertyBox.m_PostTrigger_18th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=19){StimGatingRelayID[18]=PropertyBox.m_Chan_19th; InterStimulationInterval[18]=PropertyBox.m_PostTrigger_19th; NumOfStimulationChannels++;}
		if(MAX_NUM_STIM_CHANNELS>=20){StimGatingRelayID[19]=PropertyBox.m_Chan_20th; InterStimulationInterval[19]=PropertyBox.m_PostTrigger_20th; NumOfStimulationChannels++;}
		int ChanCount=0, i;
		for(int i=0;i<NumOfStimulationChannels;i++){
			if(StimGatingRelayID[i]>=DO_FIRST_GATING_CHAN && StimGatingRelayID[i]<=DO_LAST_GATING_CHAN){
				m_StimulationChannels[ChanCount]=StimGatingRelayID[i];
				if(InterStimulationInterval[i]<MIMIMUM_POST_ANTI_DELAY)m_PostStimPeriod[i]=MIMIMUM_POST_ANTI_DELAY; else m_PostStimPeriod[ChanCount]=InterStimulationInterval[i];
				if(++ChanCount==MAX_NUM_STIM_CHANNELS)break;
			}
		}
		*M_CurrIdx_Of_StimGateRelayID=0;//Reset this just in case the counter is bigger than *M_NumOfStimulationChannels
		*M_NumOfStimulationChannels=ChanCount;
		m_NumOfStimulationChannels=ChanCount;
		for(i=0;i<m_NumOfStimulationChannels;i++){
			M_StimDigitalOutChannels[i]       =m_StimulationChannels[i];
			M_InterStimulationInterval[i]=m_PostStimPeriod[i];	
			AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(i); //How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
		}

		*M_StimulationLatencyBias_InMicroS=PropertyBox.m_StimulationLatencyBias_InMicroS;
		m_StimulationLatencyBias_InMicroS=PropertyBox.m_StimulationLatencyBias_InMicroS;
		Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();

		#ifdef Zapper
		//Make a list of Stimulation Gating Relay IDs for M_Artifact[][]; do the same, right after loading a templete file containing similar info.
		Zapper_MakeListOfGatingRelayIDs();
		#endif
	}
}

void CAntsView::Zapper_MakeListOfGatingRelayIDs() 
{
	int i,j,p, found, IDtoFind, numStoredIDs;
	Zapper *pZapper;
	for(i=0;i<m_NumOfStimulationChannels;i++){
		IDtoFind=m_StimulationChannels[i];
		for(j=0;j<NUM_NN_AI_CHAN;j++){
			found=0; 
			pZapper=&(M_Zapper[j]);
			numStoredIDs=pZapper->M_NumGatingRelayIDs;
			for(p=0;p<numStoredIDs;p++){
				if(pZapper->M_StimRelayID[p] == IDtoFind){
					found=1; break;
				}
			}
			if(found==0){//Add
				pZapper->M_StimRelayID[pZapper->M_NumGatingRelayIDs]=IDtoFind;
				pZapper->M_NumGatingRelayIDs++;
			}
		}
	}
}

void CAntsView::Update_LearnedLevel(int RecordingChannel, int GatingRelayID, double LearnedLevel) 
{
	int i, numStoredIDs;
	Zapper *pZapper;
	pZapper=&(M_Zapper[RecordingChannel]);
	numStoredIDs=pZapper->M_NumGatingRelayIDs;
	for(i=0;i<numStoredIDs;i++){
		if(pZapper->M_StimRelayID[i] == GatingRelayID){
			pZapper->M_LearnedLevel[i]=LearnedLevel; return;
		}
	}
	AfxMessageBox("The Rating Realy ID is not found");
}

void CAntsView::OnChangeParameters() 
{
	CDialogueBox	ParameterBOX;

	ParameterBOX.m_dVariable1=m_dMaximumX;
	ParameterBOX.m_dVariable2=m_dMinimumX;
	ParameterBOX.m_dVariable3=m_dMaximumY;
	ParameterBOX.m_dMinimumY=m_dMinimumY;
	if(ParameterBOX.DoModal()==IDOK){
		if(ParameterBOX.m_dVariable1<=ParameterBOX.m_dVariable2||ParameterBOX.m_dVariable3<=ParameterBOX.m_dMinimumY){AfxMessageBox("m_dMaximumX<=m_dMinimumX OR m_dMaximumY<=m_dMinimumY ==> Try again", MB_OK); return;}
		else{
			m_dMaximumX=ParameterBOX.m_dVariable1;
			m_dMinimumX=ParameterBOX.m_dVariable2;
			m_dMaximumY=ParameterBOX.m_dVariable3;
			m_dMinimumY=ParameterBOX.m_dMinimumY;
			UpdateScales();	
		}
	}
}

void CAntsView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

