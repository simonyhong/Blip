
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
#include "SinTable.h"
#include "MainFrm.h"


//Using the information in figures, this function 
//    (1) registers the stimulation channels in M_StimDigitalOutChannels[i]; 
//    (2) figures out stimulation-recording pairs for saving purpose 
//    (3) Tags figures that are eligible for display
//For the valid stimulation channels: This function goes through ALL the figures in ANTS and addes the STIMULATION channels that the Figures have, but not the "M_StimDigitalOutChannels[ ]", which "Property" has as a list.
//    Also if M_StimDigitalOutChannels[] has it but no Figure is using the STIMULATION channel, that channel will be removed. 
void CAntsView::Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures()
{				
	int i, idx, lyr, row, clm, foundIdx, ChID;
	CFigure *pFig;

	//##################################################################
	//Remove Stimulation channels that are not used
	for(i=m_NumOfStimulationChannels-1; i>=0; i--){
		foundIdx=-1;
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					if(pFig->m_Deleted==0 && pFig->m_StimGatingRelay.m_ChanID==M_StimDigitalOutChannels[i]){//Find the i that has the same index info of the stimulation channel as the one by the figure.
						foundIdx=i; goto NextStimCH;
					}				
				}
			}
		}
		NextStimCH:
		if(foundIdx==-1){
			if(M_StimDigitalOutChannels[i]>=DO_FIRST_GATING_CHAN && M_StimDigitalOutChannels[i]<=DO_LAST_GATING_CHAN)	Relay_OFF(M_StimDigitalOutChannels[i]);//In case it is active, turn off the gating signals.
			//Fill the unused one by shifting everthing above.
			for(idx=i;idx<m_NumOfStimulationChannels-1;idx++){
				M_StimDigitalOutChannels[idx]=M_StimDigitalOutChannels[idx+1];
				m_StimulationChannels[idx]=m_StimulationChannels[idx+1]; 
				M_InterStimulationInterval[idx]     =M_InterStimulationInterval[idx+1];     
				m_PostStimPeriod[idx]  =m_PostStimPeriod[idx+1];  
			}
			//Delete the last one.
			M_StimDigitalOutChannels[m_NumOfStimulationChannels-1]	=-1;
			m_StimulationChannels[m_NumOfStimulationChannels-1]		=-1; 
			M_InterStimulationInterval[m_NumOfStimulationChannels-1]=1000;     
			m_PostStimPeriod[m_NumOfStimulationChannels-1]			=1000;  
			m_NumOfStimulationChannels--; 
			if(m_NumOfStimulationChannels<0){AfxMessageBox("m_NumOfStimulationChannels<0");m_NumOfStimulationChannels=0;}
			*M_NumOfStimulationChannels=m_NumOfStimulationChannels;		
		}
	}
	//##################################################################
	//Add stimulation channels that the figures have but are not in the channel
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				ChID=pFig->m_StimGatingRelay.m_ChanID;
				if(ChID==-1 || pFig->m_Deleted==1)continue;
				foundIdx=-1;
				for(i=0;i<m_NumOfStimulationChannels;i++){
					if(ChID==M_StimDigitalOutChannels[i]){//Find the i that has the same index info of the stimulation channel as the one by the figure.
						foundIdx=i; goto NextStimCH2;
					}				
				}
				NextStimCH2:
				if(foundIdx==-1 && m_NumOfStimulationChannels<MAX_NUM_STIM_CHANNELS-1 && ChID>=DO_FIRST_GATING_CHAN && ChID<=DO_LAST_GATING_CHAN){
					M_StimDigitalOutChannels[m_NumOfStimulationChannels]=ChID;
					m_StimulationChannels[m_NumOfStimulationChannels]=ChID; 
					M_InterStimulationInterval[m_NumOfStimulationChannels]=1000;	
					m_PostStimPeriod[m_NumOfStimulationChannels]=1000;
					AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(m_NumOfStimulationChannels);//How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
					m_NumOfStimulationChannels++;
					*M_NumOfStimulationChannels=m_NumOfStimulationChannels;
				}
			}
		}
	}

	//##################################################################
	//For save and disply purpose,
	//register the recording-stimulation channel pairs that are valid:
	//		(1)attached to proper stimulation channels (2) pFig->m_Deleted!=1 (3) valid and used channel (4) no duplication.
	//		(5) pFig->m_toSave==1 is considered only for saving purpose, not for display.
	int Recording_StimulationCh, StimIdx, FoundStimCh;
	m_numRecording_Stim_PairsToSave=0;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr]; pFig->m_IsToDisplay=0;
				FoundStimCh=0;for(StimIdx=0;StimIdx<m_NumOfStimulationChannels;StimIdx++)if(M_StimDigitalOutChannels[StimIdx]==pFig->m_StimGatingRelay.m_ChanID){FoundStimCh=1;break;} if(FoundStimCh==0)continue; //Has a un-registered stimulation channel.
				if(pFig->m_Deleted!=1 && pFig->m_RecordingChan.m_ChanID>=0 && pFig->m_RecordingChan.m_ChanID<NUM_NN_AI_CHAN && M_NumOfFigsUsingThisChan[pFig->m_RecordingChan.m_ChanID]>0){
					pFig->m_IsToDisplay=1; //Display this
					if(pFig->m_toSave==1){ //Display & Save this figure's signal
						for(idx=0; idx<m_numRecording_Stim_PairsToSave; idx++)if(m_pFigForSave[idx]==pFig)goto NEXT_FIG_TO_SEARCH;//Duplicate
						m_pFigForSave[m_numRecording_Stim_PairsToSave]=pFig;
						m_numRecording_Stim_PairsToSave++;
					}
				}
				NEXT_FIG_TO_SEARCH:;
			}
		}
	}

	//##################################################################
	//Give a warning that one of the figures has a stimulation channel that is out of the supported range.
	int OutOfRangeChannelNumber=0;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				ChID=pFig->m_StimGatingRelay.m_ChanID;
				if(ChID<DO_FIRST_GATING_CHAN || ChID>DO_LAST_GATING_CHAN){
					OutOfRangeChannelNumber=1;
				}
			}
		}
	}
	if(OutOfRangeChannelNumber==1){
		char MsgTmp[512];
		sprintf(MsgTmp,"Hello there!\nOne of the Stimulation channel number is \n out of the currently supported range.\n %d ~ %d (DO_FIRST_GATING_CHAN ~ DO_LAST_GATING_CHAN)\n See SharedConstants.h for available channel numbers",DO_FIRST_GATING_CHAN,DO_LAST_GATING_CHAN);
		AfxMessageBox(MsgTmp);
	}
}


int CAntsView::IsIt_a_ValidStimChannel_Registered(int ChID)
{
	for(int i=0; i<m_NumOfStimulationChannels; i++){
		if(M_StimDigitalOutChannels[i]==ChID)return 1;//Valid channel
	}
	return 0;//Couldn't find the channel
}

int CAntsView::IsThereAnyFigureHavingThisStimChannel(int Chan)
{				
	int lyr, row, clm;
	CFigure *pFig;

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(pFig->m_Deleted==0 && pFig->m_StimGatingRelay.m_ChanID==Chan){//Find the i that has the same index info of the stimulation channel as the one by the figure.
					return 1;
	}	}	}	}
	return 0;
}

//Find figures in the TRIGgerLayer and set the M_TriggerToStimulationLatency[] for them using the valid, and registered stimulation channels in M_StimDigitalOutChannels[]. 
//But, since the pTrigFig needs to have the prioriety, if there are other figures bearing the same stimulating channel as the pTrigFig, use the measuring line 0 of the pTrigFig to set the M_TriggerToStimulationLatency[].
//In the case there are more valid, and registered stimulation channels than the ones represented by the figures in the TRIGerLayer, use other layers to set the M_TriggerToStimulationLatency[] for stimulation channels.
void CAntsView::Set_StimLatency_and_PostStimPeriod()
{
	int lyr, row, clm, StimChanDOIdx, StimChanDOIdxForTRIGgerFig, ChanToSearch;
	int foundCh, i, j, StimChOfFig, firstTime=1;
	CFigure *pFig;

	for(i=0; i<m_NumOfStimulationChannels; i++){
		ChanToSearch=M_StimDigitalOutChannels[i];
		foundCh=0;
		//Set the
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					StimChOfFig=pFig->m_StimGatingRelay.m_ChanID;
					if(pFig->m_Deleted==0 && pFig->m_RecordingChan.m_ChanID!=*M_AntidromicTriggeringChanID && DoesTriggeringChannel_HaveThisStimChannel(StimChOfFig)){//Does the triggering channel has this stimulating channel.
						continue;
					}
					if(pFig->m_IsToDisplay==1 && StimChOfFig==ChanToSearch){
						StimChanDOIdx=GetIndexOfSimChannel(StimChOfFig);
						M_TriggerToStimulationLatency[StimChanDOIdx]=(int)(1000*fabs(pFig->m_MeasuringLineTime[0])); //1000 is there to convert to Micro Second
						AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(StimChanDOIdx);//How frequently do you want the antidromic stimulation to be triggered? For example, 1000ms? or 500ms?
						foundCh=1; goto END_OfSEARCH;
					}
				}
			}
		}
		END_OfSEARCH:;
		if(foundCh==0 && firstTime==1){firstTime=0; AssessConfiguration();}
	}
}


int CAntsView::DoesTriggeringChannel_HaveThisStimChannel(int Chan)
{				
	int lyr, row, clm;
	CFigure *pFig;

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(pFig->m_Deleted==0 && pFig->m_RecordingChan.m_ChanID==*M_AntidromicTriggeringChanID && pFig->m_StimGatingRelay.m_ChanID==Chan){
					return 1;
	}	}	}	}
	return 0;
}