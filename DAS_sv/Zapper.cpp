#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "VarDefinitions_SIG.h"
#include "Channel.h"
#include "DAS_View.h"
#include "SinTable.h"
#include "MainFrm_DAS.h"
#include <NIDAQmx.h>
#include "Mmsystem.h" 


//Learning always happens after finding the begining of the artifact using Find_BeginingOfArtifact(), and comes before Zapping (Zapper_Zap()).
//So, the data chAI[p] will always be a raw one not touched by Zapping.
void CDasView::Zapper_Learn(CAiChannel * pCh, double *chAI)
{
	__int64 InitIdx, EndIdx;
	int idx, RemainingLengthToSearch;
	//Search for the begining of the artifact
	if(m_HowManyDataPoints_ToFind_StartOfArtifact > 0){//NEED to search for the begining of the artifact
		Find_BeginingOfArtifact(pCh, chAI);
	}
	//Learning the artifact
	if(m_StartAIidx_toLearnArtifact>=0){//ALLOWED to learn
		InitIdx=m_StartAIidx_toLearnArtifact; 
		//Find the shorter stretch of data between "InitIdx and pCh->m_AiIdx" and "m_NumOfArtifactDataPoints_toLEARN".
		Find_EndAIidx(pCh, InitIdx, EndIdx, m_NumOfArtifactDataPoints_toLEARN, RemainingLengthToSearch);
		m_StartAIidx_toLearnArtifact=EndIdx;
		__int64 p,q;
		if(InitIdx<=EndIdx){//Not the loop end
			for(p=InitIdx, q=ARTIFACT_SIZE-m_NumOfArtifactDataPoints_toLEARN;p<EndIdx;p++, q++){
				m_pArtifact[q]=m_LearnedLevel*m_pArtifact[q] +(1-m_LearnedLevel)*chAI[p];
			}					
		}
		else{//There is the loop of the index somewhere in the time span
			for(p=InitIdx, q=ARTIFACT_SIZE-m_NumOfArtifactDataPoints_toLEARN;p<pCh->m_AIQ_size;p++, q++){
				m_pArtifact[q]=m_LearnedLevel*m_pArtifact[q] +(1-m_LearnedLevel)*chAI[p];
			}					
			for(p=0;p<EndIdx;p++, q++){
				m_pArtifact[q]=m_LearnedLevel*m_pArtifact[q] +(1-m_LearnedLevel)*chAI[p];
			}					
		}
		m_NumOfArtifactDataPoints_toLEARN=RemainingLengthToSearch;
	}
}

void CDasView::Zapper_Zap(CAiChannel * pCh, double *chAI)
{
	__int64 InitIdx, EndIdx;
	int idx, RemainingLengthToSearch;
	//Search for the begining of the artifact
	if(m_HowManyDataPoints_ToFind_StartOfArtifact > 0){//NEED to search for the begining of the artifact
		Find_BeginingOfArtifact(pCh, chAI);
	}
	//Zap the artifact
	if(m_StartAIidx_toZapArtifact>=0){//ALLOWED to Zap
		InitIdx=m_StartAIidx_toZapArtifact; 
		//Find the shorter stretch of data between "InitIdx and pCh->m_AiIdx" and "m_NumOfArtifactDataPoints_toZAP".
		Find_EndAIidx(pCh, InitIdx, EndIdx, m_NumOfArtifactDataPoints_toZAP, RemainingLengthToSearch);
		m_StartAIidx_toZapArtifact=EndIdx;
		__int64 p,q;
		if(InitIdx<=EndIdx){//Not the loop end
			for(p=InitIdx, q=ARTIFACT_SIZE-m_NumOfArtifactDataPoints_toZAP;p<EndIdx;p++, q++){
				chAI[p]-=m_pArtifact[q];
			}					
		}
		else{//There is the loop of the index somewhere in the time span
			for(p=InitIdx, q=ARTIFACT_SIZE-m_NumOfArtifactDataPoints_toZAP;p<pCh->m_AIQ_size;p++, q++){
				chAI[p]-=m_pArtifact[q];
			}					
			for(p=0;p<EndIdx;p++, q++){
				chAI[p]-=m_pArtifact[q];
			}					
		}
		m_NumOfArtifactDataPoints_toZAP=RemainingLengthToSearch;
	}
}

void CDasView::Find_EndAIidx(CAiChannel * pCh, __int64 InitIdx, __int64 &EndIdx, int LengthToSearch, int &RemainingLengthToSearch)
{
	if(InitIdx<pCh->m_AiIdx){//There is NO LOOP of the index somewhere in the time span
		if(pCh->m_AiIdx-InitIdx<=LengthToSearch){EndIdx=pCh->m_AiIdx; RemainingLengthToSearch=LengthToSearch-(EndIdx-InitIdx);}
		else{ EndIdx=InitIdx+LengthToSearch; RemainingLengthToSearch=0;} 
	}
	else{//There is the loop of the index somewhere in the time span
		if(pCh->m_AIQ_size-InitIdx+pCh->m_AiIdx<=LengthToSearch){EndIdx=pCh->m_AiIdx; RemainingLengthToSearch=LengthToSearch-(pCh->m_AIQ_size-InitIdx+pCh->m_AiIdx);}
		else {EndIdx=pCh->qIdx(InitIdx+LengthToSearch); RemainingLengthToSearch=0;} 
	}
}

//When DAS attempts to find the beginning of the artifact, the signals in the channel around that time has not been zapped. 
//So, using m_ExpectedTimeOfARTIFACT_MinusHalfMs as the begining point to search for the artifact is valid.
void CDasView::Find_BeginingOfArtifact(CAiChannel * pCh, double *chAI)
{
#define ARTIFACT_SURGE 0.2
	__int64 InitIdx, EndIdx;
	int idx, RemainingLengthToSearch;
	//Search for the begining of the artifact
	if(m_HowManyDataPoints_ToFind_StartOfArtifact > 0){//NEED to search for the begining of the artifact
		InitIdx=m_ExpectedTimeOfARTIFACT_MinusHalfMs; //When DAS attempts to find the beginning of the artifact, the signals in the channel around that time has not been zapped. So, using m_ExpectedTimeOfARTIFACT_MinusHalfMs as the beginning point to search for the artifact is valid.
		//Find the shorter stretch of data between "InitIdx and pCh->m_AiIdx" and "m_HowManyDataPoints_ToFind_StartOfArtifact".
		Find_EndAIidx(pCh, InitIdx, EndIdx, m_HowManyDataPoints_ToFind_StartOfArtifact, RemainingLengthToSearch);
		m_HowManyDataPoints_ToFind_StartOfArtifact=RemainingLengthToSearch;			
		//Find the artifact surge
		if(InitIdx<EndIdx){//Not the loop end
			for(idx=InitIdx; idx<EndIdx-2; idx++){
				if((chAI[idx]-chAI[idx+2])>ARTIFACT_SURGE || (chAI[idx]-chAI[idx+2])<-ARTIFACT_SURGE){
					m_HowManyDataPoints_ToFind_StartOfArtifact=0;//Found, no need to search any further.
					m_StartAIidx_toLearnArtifact=idx;
					m_StartAIidx_toZapArtifact=idx;
					*M_StartAIidx_ofArtifact_4ANTS=idx;
					return;
				}
			}
		}
		else{//There is the loop of the index somewhere in the time span
			for(idx=InitIdx; idx<pCh->m_AIQ_size-2; idx++){
				if((chAI[idx]-chAI[idx+2])>ARTIFACT_SURGE || (chAI[idx]-chAI[idx+2])<-ARTIFACT_SURGE){
					m_HowManyDataPoints_ToFind_StartOfArtifact=0;//Found, no need to search any further.
					m_StartAIidx_toLearnArtifact=idx;
					m_StartAIidx_toZapArtifact=idx;
					*M_StartAIidx_ofArtifact_4ANTS=idx;
					return;
				}
			}
			for(idx=0; idx<EndIdx-2; idx++){
				if((chAI[idx]-chAI[idx+2])>ARTIFACT_SURGE || (chAI[idx]-chAI[idx+2])<-ARTIFACT_SURGE){
					m_HowManyDataPoints_ToFind_StartOfArtifact=0;//Found, no need to search any further.
					m_StartAIidx_toLearnArtifact=idx;
					m_StartAIidx_toZapArtifact=idx;
					*M_StartAIidx_ofArtifact_4ANTS=idx;
					return;
				}
			}
		}
		//If the artifact was not found even after searching the whole allowed span, abort learning or zapping.
		if(m_HowManyDataPoints_ToFind_StartOfArtifact<=0){
			m_NumOfArtifactDataPoints_toLEARN=0;
			m_NumOfArtifactDataPoints_toZAP=0;
		}
	}
}

void CDasView::Zapper__Antidromic_Stimulation_Given_from(int StimGatingRelayID, int ExpectedTimeOfStim_AIidx)
{
	int numStoredIDs,p;
	Zapper *pZapper;
	pZapper=&M_Zapper[*M_AntidromicTriggeringChanID]; //Only one recording channel is allowed to act on the stimulation; change the architecture if you want multiple recording channels to handle the stimulation.
	numStoredIDs=pZapper->M_NumGatingRelayIDs;
	if(numStoredIDs>0){
		for(p=0;p<numStoredIDs;p++){ //Find the stimulation channel
			if(pZapper->M_StimRelayID[p] == StimGatingRelayID){//If found
				m_pArtifact=pZapper->M_Artifact[p];
				m_LearnedLevel=pZapper->M_LearnedLevel[p];
				//To find the begining of the artifact
				m_ExpectedTimeOfARTIFACT_MinusHalfMs=ExpectedTimeOfStim_AIidx-0.0005*DATA_SAMPLING_RATE;
				m_HowManyDataPoints_ToFind_StartOfArtifact=0.002*DATA_SAMPLING_RATE;
				//Where is the artifact?
				m_StartAIidx_toLearnArtifact=-1;
				m_StartAIidx_toZapArtifact=-1;
				//Is it to Learn the Artifact?
				if(*M_isZapperLearning==1){
					//To learn the artifact
					m_NumOfArtifactDataPoints_toLEARN=ARTIFACT_SIZE;
				}
				//Always Zap!!
				m_NumOfArtifactDataPoints_toZAP=ARTIFACT_SIZE;
				break;
			}
		}
		if(p>=numStoredIDs){
			AfxMessageBox("The relay ID is not found.");//Not found
		}
	}
}

