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


//This message-based one is NOT used currently
//This message called function somehow makes DAS_sv STOP (during the entire period of execution of this function, no matter how long) during its Sleep() period.
//The time between the Message to call this function and actual execution of this function is about 52micS and quite consistent.
//This is used to trigger the "STIMULATOR".
//LRESULT CDasView::SetAntiDO_forXms(WPARAM pCommandingChan, LPARAM AIidx)
//{
//	CAiChannel *pAiChan=(CAiChannel *)pCommandingChan;
//	SetAntiDO_forXms_Function(pAiChan->M_timeStampMicS[AIidx], AIidx);
//	return NULL;
//}


//This one constantly checks the request of antidromic stimulation when it is woken up.
//If there is no request for a while (defind by relax_after_x_sec) the loop goes to a "relax" mode where it waits for an EVENT.
//This is faster than the Event-based calls and has less jitter in time. 
void CDasView::Anti_Ortho_Stimulation_Loop()
{
	#define RELAX_AFTER_X_SEC 120
	clock_t m_start,m_finish;
	m_start=clock();
	double duration;	
	__int64 LoopCounter=0; 
	m_IsResting=0;
	m_AntidromicJobRequest=0;
	*M_HeyDAS_DoOrthodromicStimulationNOW=0;

	int	nEventIdx;
	HANDLE Events[3]; 
	Events[0]= m_EvStopProcess;
	Events[1]= m_Ev_WakeUp_ForAntidromicStimulation;
	Events[2]= *M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation;
	#ifdef ZAPPER
	m_StimGatingRelayID=-1;
	#endif

	while(m_bIsON==1 && *M_Blip_OFF==0){
		//Antidromic stimulation (the gating of antidromic stimulation is done automatically by taking turns among available channels)
		if(m_AntidromicJobRequest==1){
			m_AntidromicJobRequest=0;
			Set_AntiDO_forXms_Function(m_AiChan_M_timeStampMicS, m_nEstimatedAntidromicStimPointIdx, m_TriggeringChannel, m_ArrayIndexForUsableAiId_handedOverByRequestingChan);
			m_start=clock();//Restart the time counting.
			LoopCounter=0; 
			m_IsResting=0; //No time to relax.
		}
		//Orthodromic stimulation
		else if(*M_HeyDAS_DoOrthodromicStimulationNOW==1){
			Set_OrthoDO_forXms_Function();
			*M_HeyDAS_DoOrthodromicStimulationNOW=0;
			m_start=clock();//Restart the time counting.
			ResetEvent(*M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation); //In the case m_IsResting==1 already, *M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation does not get reset by WaitForMultipleObjects(). So we need to reset it manually; though it does not make a real difference because WaitForMultipleObjects() will be visited one more time for no reason. 
			LoopCounter=0; 
			m_IsResting=0; //No time to relax.
		}
		//Orthodromic gating (orthodromic stimulation gating channel is selected and initiated by the user.)
		else if(*M_HeyDASsv_StimGatingRequest==1){
			*M_HeyDASsv_StimGatingRequest=0;
			*M_RememberedTurnedOnStimRelayID=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID];  //*M_RememberedTurnedOnStimRelayID is used to turn off later even when the user has changed the configuration.
			M_DO_Array[ *M_RememberedTurnedOnStimRelayID ]=RELAY_ON; //The NEXT gating signal for the stimulation signal via the RELAY.
			DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
			m_start=clock();//Restart the time counting.
			LoopCounter=0; 
			m_IsResting=0; //No time to relax.
		}

		if(m_IsResting==0){//Working!!
			if(LoopCounter++>1000){//If there has been no job request for 1000 loops, check time.
				LoopCounter=0; //This is done to check time every once in a while.
				m_finish=clock();	duration=((double)(m_finish-m_start))/CLOCKS_PER_SEC; //Measure the time since the last job request.
				if(duration>RELAX_AFTER_X_SEC)m_IsResting=1;   //If the time since the last job request is more than a set amount (RELAX_AFTER_X_SEC), start to relax.
			}
		}
		else{//Resting: wait for an EVENT to wake up.
			nEventIdx=WaitForMultipleObjects(3,Events,FALSE/*wait for all?*/,INFINITE/*timeout*/);
			m_IsResting=0; //Woken up.
			if(nEventIdx==0)break;//return;
		}
	}
	m_bIsON=0;

	//####### Turn ALL RELAY OFF
	for(int i=0;i<*M_NumOfStimulationChannels; i++){	
		M_DO_Array[ M_StimDigitalOutChannels[i] ]=RELAY_OFF; //The current gating signal for the stimulation signal via the RELAY.
	}
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
}


void CDasView::Set_AntiDO_forXms_Function(__int64 AiChan_M_timeStampMicS, __int64 TriggerSig_AIidx, int TriggeringChannel, int ArrayIndexForUsableAiId_handedOverByRequestingChan)
{
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);// THREAD_PRIORITY_TIME_CRITICAL  THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_NORMAL  THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_LOWEST  THREAD_PRIORITY_IDLE 
	__int64 newVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&newVal); 
	int timeLeftInMicS=M_TriggerToStimulationLatency[*M_CurrIdx_Of_StimGateRelayID]-((newVal*m_1000000perCPU-m_beginValInMicS)-AiChan_M_timeStampMicS);
	if(timeLeftInMicS>*M_StimulationLatencyBias_InMicroS)SleepAccurate_inMicS(timeLeftInMicS-*M_StimulationLatencyBias_InMicroS);//This one has an accuracy of a few microS.

	//################ ON (Trigger) ################
	M_DO_Array[DO_STIM_TRIGGER_CHAN]=1;//Since this is for the stimulator not for the relay, do not use RELAY_ON (==0). 
	//Now on
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use

	//################ Inform other programs ################
	//Deduce the indexes of the triggerring event: 
	//Since the M_usableAiIdx[] is set synchronously across all the channels at each iteration of data acquision by EveryNCallback(), 
	//knowing which index of a channel corresponds to the triggering signal can be used to deduce the indexes of other channels 
	//by first calculating the index difference between the last available index and the triggering index: M_usableAiIdx[TriggeringChannel]-TriggerSig_AIidx
	//and applying this difference to other channels. 
	//Since there is a possibility that M_usableAiIdx[] may have changed since the retrival of the data, m_UsableAiIdx_atTheTimeOfDataRetrival[ArrayIndexForUsableAiId_handedOverByRequestingChan][ChanID] is used instead. 
	//Inform ANTS, first part
	int DifferenceOfIdxFromLatestAvailableIdx, usableAiIdx_ofTriggerChan=m_UsableAiIdx_atTheTimeOfDataRetrival[ArrayIndexForUsableAiId_handedOverByRequestingChan][TriggeringChannel];
	if(TriggerSig_AIidx <= usableAiIdx_ofTriggerChan){ //This is the ordinary case
		DifferenceOfIdxFromLatestAvailableIdx=usableAiIdx_ofTriggerChan-TriggerSig_AIidx;
	}
	else{ //If the index of the last available data point is behind of the triggering index found...
		DifferenceOfIdxFromLatestAvailableIdx=(M_AIQueueSize[TriggeringChannel]-TriggerSig_AIidx) + usableAiIdx_ofTriggerChan;
	}
	if(DifferenceOfIdxFromLatestAvailableIdx<0 || DifferenceOfIdxFromLatestAvailableIdx>=M_AIQueueSize[TriggeringChannel])AfxMessageBox("DifferenceOfIdxFromLatestAvailableIdx<0 || DifferenceOfIdxFromLatestAvailableIdx>=M_AIQueueSize[TriggeringChannel]");
	for(int ChanID=0; ChanID<NUM_NN_AI_CHAN; ChanID++){
		M_EstimatedAntidromicStimPointIdx[ChanID]=AiChan[ChanID].qIdx(m_UsableAiIdx_atTheTimeOfDataRetrival[ArrayIndexForUsableAiId_handedOverByRequestingChan][ChanID]-DifferenceOfIdxFromLatestAvailableIdx);
	}

#define TRIGGER_TTL_PLATEAU_inMS 1
	#ifdef ZAPPER
	//Inform the EveryNCallback to look for an artifact.
	m_ExpectedTimeOfStim_AIidx=TriggerSig_AIidx+M_TriggerToStimulationLatency[*M_CurrIdx_Of_StimGateRelayID]/(*M_microSecPerPoint);//This variable will be used at the EveryNCallback to predict the artifact.
	m_StimGatingRelayID=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; 
	Zapper__Antidromic_Stimulation_Given_from(m_StimGatingRelayID, m_ExpectedTimeOfStim_AIidx);
	#endif
	//Inform ANTS, second part
	*M_HeyANTS_StimGivenToGatingDIO_X=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; 
	//Inform TAS to display this event
	*M_HeyMOXY_StimGivenToGatingDIO_X=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; 
	Sleep(TRIGGER_TTL_PLATEAU_inMS);//This one does not have to be accurate. (This is the plateau period of the TTL trigger pulse)

	//################ OFF (Trigger) ################
	M_DO_Array[DO_STIM_TRIGGER_CHAN]=0;//Since this is for the stimulator not for the relay, do not use RELAY_OFF (==1). 
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use

	//#### Switch the gating signal sometime after stimulation designated by M_InterStimulationInterval[].
	//The gating signal (which opens the stimulation channel for the stimulation signal to go through) ends roughly 100 ms before the end of the delay period designated by M_InterStimulationInterval[].
	//This is to make sure that the current gating is off before any following stimulation signal.
	Sleep((int)Rectify(M_InterStimulationInterval[ *M_CurrIdx_Of_StimGateRelayID]-(100+TRIGGER_TTL_PLATEAU_inMS)));
	if(*M_RememberedTurnedOnStimRelayID<0){AfxMessageBox("*M_RememberedTurnedOnStimRelayID<0"); return;}
	M_DO_Array[ *M_RememberedTurnedOnStimRelayID ]=RELAY_OFF; //The current GATING signal for the stimulation signal via the RELAY.
	if(*M_NumOfStimulationChannels==0)return;
	int AttemptCount=0;
	NEXT_STIM_CHAN://In case there is a focused channel (only a subset of the channels are stimulated), and if the index chosen is not one of those in focus, choose the next one and see if that is.
	if(*M_CurrIdx_Of_StimGateRelayID+1<*M_NumOfStimulationChannels)(*M_CurrIdx_Of_StimGateRelayID)++; else *M_CurrIdx_Of_StimGateRelayID=0; //Take turns 
	if(*M_NumOfFocusChannels>0){
		if(AttemptCount++>MAX_NUM_STIM_CHANNELS){
			AfxMessageBox("AttemptCount++>MAX_NUM_STIM_CHANNELS"); return;
		}
		if(M_isFocusedChannel[*M_CurrIdx_Of_StimGateRelayID]==0)goto NEXT_STIM_CHAN; //Exclude those that are not on focus.
	}
	if(*M_CurrIdx_Of_StimGateRelayID>=0 && *M_CurrIdx_Of_StimGateRelayID<MAX_NUM_STIM_CHANNELS && M_StimDigitalOutChannels[*M_CurrIdx_Of_StimGateRelayID]>=DO_FIRST_GATING_CHAN && M_StimDigitalOutChannels[*M_CurrIdx_Of_StimGateRelayID]<=DO_LAST_GATING_CHAN){
		*M_RememberedTurnedOnStimRelayID=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID];  //*M_RememberedTurnedOnStimRelayID is used to turn off later even when the user has changed the configuration.
		M_DO_Array[ *M_RememberedTurnedOnStimRelayID ]=RELAY_ON; //The NEXT gating signal for the stimulation signal via the RELAY.
		DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
	}
}

//In Antidromic Stimulation DAS_sv automatically sets on/off gating relays using the table made by ANTS. 
//The relay gating for Ortodromic Stimulation is set by DAS_sv at the request of TAS (*M_HeyDASsv_StimGatingRequest=1). 
void CDasView::Set_OrthoDO_forXms_Function()
{
	//################ ON (Trigger) ################
	M_DO_Array[DO_STIM_TRIGGER_CHAN]=1;//Since this is for the stimulator not for the relay, do not use RELAY_ON (==0). 
	//Now on
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
	//## Inform
#define TRIGGER_TTL_PLATEAU_inMS 1
	#ifdef ZAPPER
	//Inform the EveryNCallback to look for an artifact.
	m_ExpectedTimeOfStim_AIidx=TriggerSig_AIidx+M_TriggerToStimulationLatency[*M_CurrIdx_Of_StimGateRelayID]/(*M_microSecPerPoint);//This variable will be used at the EveryNCallback to predict the artifact.
	m_StimGatingRelayID=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; 
	Zapper__Antidromic_Stimulation_Given_from(m_StimGatingRelayID, m_ExpectedTimeOfStim_AIidx); //Now act EveryNCallback()!!
	#endif

	//Inform ANTS, first part
	__int64 newVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&newVal); 
	int EstimatedIncreaseOfIDX=((__int64)(newVal*m_1000000perCPU-m_beginValInMicS)-*M_LatestTimeStampInMicroS + *M_StimulationLatencyBias_InMicroS)/(*M_microSecPerPoint);
	for(int ChanID=0; ChanID<NUM_NN_AI_CHAN; ChanID++){
		//m_ArrayIndex_forUsableAiId[] is not used by the orthodromic stimulation. This is because, unlike the antidromic stimulation, which has some time delay between the triggering signal and actual stimulation, orthodromic stimulation becomes given as soon as the request comes.
		M_EstimatedOrthodromicStimPointIdx[ChanID]=*M_usableAiIdx[ChanID] +EstimatedIncreaseOfIDX;//Time in microS. 
	}
	//Inform ANTS, second part
	*M_HeyANTS_StimGivenToGatingDIO_X=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; 
	//Inform TAS to display this event
	*M_HeyMOXY_StimGivenToGatingDIO_X=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; 
	Sleep(TRIGGER_TTL_PLATEAU_inMS);//This one does not have to be accurate. (This is the plateau period of the TTL trigger pulse)

	//################ OFF (Trigger) ################
	M_DO_Array[DO_STIM_TRIGGER_CHAN]=0;//Since this is for the stimulator, not for the relay, do not use RELAY_OFF (==1). 
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
}

#ifdef EVENT_BASED_LOOP
//  ===> This function assumes that the anti- and ortho-dromic stimulations do not happen simultaneously. 
//       They both have Sleep() functions as part of this routine, which makes this routine non-responsive for other job requests.
//       So, if you are trying to do anti- and ortho-dromic stimulations at the same time, separate one of the functions as a thread.
void CDasView::Antidromic_Stimulation_EventBased()
{
	int	nEventIdx;
	HANDLE Events[3]; 
	Events[0]= m_EvStopProcess;
	Events[1]= m_Ev_WakeUp_ForAntidromicStimulation;
	Events[2]= *M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation;
	#ifdef ZAPPER
	m_StimGatingRelayID=-1;
	#endif
	
	while(1){
		nEventIdx=WaitForMultipleObjects(3,Events,FALSE/*wait for all?*/,INFINITE/*timeout*/);
		if(nEventIdx==0)break;//return;
		
		//Caution: Antidromic and Orthodromic functions have Sleep() function in them. This means 1) these routines should NOT be used at the same time 2) any other low latency Digital Output requst should NOT use this function.
		if(nEventIdx==1){
			Set_AntiDO_forXms_Function(m_AiChan_M_timeStampMicS, m_nEstimatedAntidromicStimPointIdx, m_TriggeringChannel);
		}
		else if(nEventIdx==2){
			m_nEstimatedAntidromicStimPointIdx=AiChan[*M_OrthodromicRecordingChannel].m_ThresholdXingSearchIdx;
			m_AiChan_M_timeStampMicS=AiChan[*M_OrthodromicRecordingChannel].M_timeStampMicS[m_nEstimatedAntidromicStimPointIdx];
			Set_OrthoDO_forXms_Function();
		}
	}
	
	//Turn ALL RELAY OFF
	for(int i=0;i<*M_NumOfStimulationChannels; i++){	
		M_DO_Array[ M_StimDigitalOutChannels[i] ]=RELAY_OFF; //The current gating signal for the stimulation signal via the RELAY.
	}
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
}
#endif