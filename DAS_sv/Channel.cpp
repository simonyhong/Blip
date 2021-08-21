#include "stdafx.h"
#include "VarDefinitions_SIG.h"
#include "Channel.h"
#include "DAS_View.h"
#include <math.h>

CAiChannel::CAiChannel()
{
	gain=10;
	m_idx_SpikeNn1=0; m_idx_SpikeNn2=0; m_idx_SpikeNn3=0;
	m_ThresholdXingSearchIdx=0;//m_ThresholdXingSearchIdx points the next array index to process
	m_numMatchingKinds=0;
	m_HighToLowCrossing=0;
	m_selectedBoxID=-1;
	ChanSamplCounter=0;
	m_newlyRetrivedWaveID=0; 
	m_crntlyDisplayedWaveID=0; m_totNumWavesRetrived=0; 
	m_RefreshAtEveryNWaveforms=7;
	for(int i=0;i<MAX_NNs_PerCHAN;i++){
		m_numSelRects[i]=0;
		m_numRejRects[i]=0;
	}
	m_RtClickSelectedBoxID=-1;
	InitializeCriticalSection(&m_CriticalSection);
}

CAiChannel::~CAiChannel()
{
	DeleteCriticalSection(&m_CriticalSection);
}

UINT SetAntiDO_forXms_Thread(LPVOID WinObjPtr); //A global Function

//Signal Gatherer (SIG: Discrimiates the threshold-crossing waves)
void CAiChannel::OnAnalogInput(__int64 usableAiIdx, int ArrayIndex_forUsableAiId) 
{
	EnterCriticalSection(&m_CriticalSection);//Critical Section is done in a instance-specific manner. So only the threads that works on this specific class instance, "pChan->OnAnalogInput()" will be blocked.
	ChanSamplCounter++;

	//##### Automatically trigger a stimulation?
	if((/*(*pView->M_IsToLearnArtifact==1 && *pView->M_IsOrthoStim==1) || */(*M_IsAntidromicStim==1 && *M_WhichAntidromicMode==AUTO_STIM_MODE)) && (*m_pSamplingCounter)==0 && *pView->M_NumOfStimulationChannels>0){
		//How frequently do you want to give an antidromic stimulation? 
		//This was calculated in ANTS by converting the M_PostTriggerPeriod[] which the user has put in (e.g. 1000 ms) in Property Dialog for each Stimulating Channel. 
		//Each stimulating channel can have different M_PostTriggerPeriod[], and the NEXT antidromic stimulation becomes considered ONLY AFTER this period.  
		(*m_pSamplingCounter)=pView->M_NumOfSamplingsAfterStim[*pView->M_CurrIdx_Of_StimGateRelayID];
		
		pView->m_AiChan_M_timeStampMicS=M_timeStampMicS[m_ThresholdXingSearchIdx]; pView->m_nEstimatedAntidromicStimPointIdx=m_ThresholdXingSearchIdx;	pView->m_TriggeringChannel=m_ChanID;	pView->m_ArrayIndexForUsableAiId_handedOverByRequestingChan=ArrayIndex_forUsableAiId;
		if(*M_IsAntidromicStim==1){
			if(pView->m_IsResting==1)SetEvent(pView->m_Ev_WakeUp_ForAntidromicStimulation);
			pView->m_AntidromicJobRequest=1;
		}
		//if(*pView->M_IsOrthoStim==1){
		//	if(pView->m_IsResting==1)SetEvent(*pView->M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation);
		//	*pView->M_HeyDAS_DoOrthodromicStimulationNOW=1;
		//}
	}

	__int64 usableAiIdx_1,DistToSearchForThresholdXing;

	//##### "-*M_MiniRequiredLengthOfwaveForExamination" guarentees that the Time-voltage-discriminator has at least the required data points to determine whether or not the wave meets the user-set requirements.
	//m_ThresholdXingSearchIdx points the next array index to process
	if(usableAiIdx<m_ThresholdXingSearchIdx){//The M_usableAiIdx has returned to the head part of the data.
		DistToSearchForThresholdXing=(__int64)(m_AIQ_size-m_ThresholdXingSearchIdx+usableAiIdx-*M_MiniRequiredLengthOfwaveForExamination); //-*M_MiniRequiredLengthOfwaveForExamination guarentees that the Time-voltage-discriminator has at least the required data points to determine whether or not the wave meets the user-set requirements.
	}	
	else{
		DistToSearchForThresholdXing=(__int64)(usableAiIdx-m_ThresholdXingSearchIdx-*M_MiniRequiredLengthOfwaveForExamination); //-*M_MiniRequiredLengthOfwaveForExamination guarentees that the Time-voltage-discriminator has at least the required data points to determine whether or not the wave meets the user-set requirements.
	}

	//### VT-discriminator ##########
	//m_ThresholdXingSearchIdx points to the next array index for the VTD process 
	//See the documentation "Voltage-Time Discriminator (VTD)- inside" in Blip_Guide
	// (1) Take care of the M_AI[]-array boundary crossing part first
	if(DistToSearchForThresholdXing<0)return;
	if(usableAiIdx<m_ThresholdXingSearchIdx){//The index m_ThresholdXingSearchIdx will cross the boundary of the array
		if(m_HighToLowCrossing==0){
			for(;m_ThresholdXingSearchIdx<m_AIQ_size_1;m_ThresholdXingSearchIdx++){// Because of "[m_ThresholdXingSearchIdx+1]" m_ThresholdXingSearchIdx<m_AIQ_size_1
				if(M_AI[m_ThresholdXingSearchIdx]<=(*M_Threshold) && M_AI[m_ThresholdXingSearchIdx+1]>(*M_Threshold)){
					Register_n_Classify_wavelet(ArrayIndex_forUsableAiId);
				}
				if(--DistToSearchForThresholdXing<0){m_ThresholdXingSearchIdx++; break;}//m_ThresholdXingSearchIdx++ should be done here because m_ThresholdXingSearchIdx needs to point the NEXT one after the "for loop".
			}
			if(DistToSearchForThresholdXing>=0){
				if(m_ThresholdXingSearchIdx!=m_AIQ_size_1){AfxMessageBox("m_ThresholdXingSearchIdx!=m_AIQ_size_1");}
				if(M_AI[m_ThresholdXingSearchIdx]<=(*M_Threshold) && M_AI[0]>(*M_Threshold)){
					Register_n_Classify_wavelet(ArrayIndex_forUsableAiId);
				}
				--DistToSearchForThresholdXing;
			}
		}
		else{
			for(;m_ThresholdXingSearchIdx<m_AIQ_size_1;m_ThresholdXingSearchIdx++){// Because of "[m_ThresholdXingSearchIdx+1]" m_ThresholdXingSearchIdx<m_AIQ_size_1
				if(M_AI[m_ThresholdXingSearchIdx]>(*M_Threshold) && M_AI[m_ThresholdXingSearchIdx+1]<=(*M_Threshold)){
					Register_n_Classify_wavelet(ArrayIndex_forUsableAiId);
				}
				if(--DistToSearchForThresholdXing<0){m_ThresholdXingSearchIdx++; break;}//m_ThresholdXingSearchIdx++ should be done here because m_ThresholdXingSearchIdx needs to point the NEXT one after the "for loop".
			}
			if(DistToSearchForThresholdXing>=0){
				if(m_ThresholdXingSearchIdx!=m_AIQ_size_1){AfxMessageBox("m_ThresholdXingSearchIdx!=m_AIQ_size_1");}
				if(M_AI[m_ThresholdXingSearchIdx]>(*M_Threshold) && M_AI[0]<=(*M_Threshold)){
					Register_n_Classify_wavelet(ArrayIndex_forUsableAiId);
				}
				--DistToSearchForThresholdXing;
			}
		}
		m_ThresholdXingSearchIdx=0;
	}
	// (2) Take care of the rest that is NOT crossing the M_AI[]-array boundary.
	if(DistToSearchForThresholdXing>=0){//This "if()" checks in case the previous "if(usableAiIdx<m_ThresholdXingSearchIdx)" resulted in negative or 0  DistToSearchForThresholdXing.
		usableAiIdx_1=qIdx((__int64)(usableAiIdx-1));
		if(m_HighToLowCrossing==0){
			for(;m_ThresholdXingSearchIdx<usableAiIdx_1;m_ThresholdXingSearchIdx++){
				if(M_AI[m_ThresholdXingSearchIdx]<=(*M_Threshold) && M_AI[m_ThresholdXingSearchIdx+1]>(*M_Threshold)){
					Register_n_Classify_wavelet(ArrayIndex_forUsableAiId);
				}
				if(--DistToSearchForThresholdXing<0){m_ThresholdXingSearchIdx++; break;}//m_ThresholdXingSearchIdx++ should be done here because m_ThresholdXingSearchIdx needs to point the NEXT one after the "for loop".
			}
		}
		else{
			for(;m_ThresholdXingSearchIdx<usableAiIdx_1;m_ThresholdXingSearchIdx++){
				if(M_AI[m_ThresholdXingSearchIdx]>(*M_Threshold) && M_AI[m_ThresholdXingSearchIdx+1]<=(*M_Threshold)){
					Register_n_Classify_wavelet(ArrayIndex_forUsableAiId);
				}
				if(--DistToSearchForThresholdXing<0){m_ThresholdXingSearchIdx++; break;}//m_ThresholdXingSearchIdx++ should be done here because m_ThresholdXingSearchIdx needs to point the NEXT one after the "for loop".
			}
		}
	}
	*M_usableSIGi=qIdx((__int64)(m_ThresholdXingSearchIdx-1));
	//#############
END_SIG_ACQUISION:;
	LeaveCriticalSection(&m_CriticalSection);
}

//Register the wavelet and classify using the VT-BOXes
void CAiChannel::Register_n_Classify_wavelet(int ArrayIndex_forUsableAiId) 
{  
	if(*M_AntidromicTriggeringChanID==m_ChanID && *M_IsAntidromicStim==1 && *M_WhichAntidromicMode==THRESHOLD_STIM_MODE && (*m_pSamplingCounter)==0 && *pView->M_NumOfStimulationChannels>0){
		(*m_pSamplingCounter)=pView->M_NumOfSamplingsAfterStim[*pView->M_CurrIdx_Of_StimGateRelayID];//How frequently do you want to give an antidromic stimulation? 1 sec? 0.5 sec? etc...
		pView->m_AiChan_M_timeStampMicS=M_timeStampMicS[m_ThresholdXingSearchIdx]; pView->m_nEstimatedAntidromicStimPointIdx=m_ThresholdXingSearchIdx;	pView->m_TriggeringChannel=m_ChanID;	pView->m_ArrayIndexForUsableAiId_handedOverByRequestingChan=ArrayIndex_forUsableAiId; //m_ThresholdXingSearchIdx holds the index of M_AI[] at which the wavelet crosses the threshold
		if(pView->m_IsResting==1)SetEvent(pView->m_Ev_WakeUp_ForAntidromicStimulation);
		pView->m_AntidromicJobRequest=1;
	}

	//######  REGISTER the WAVE
	m_initDataPointIdx=qIdx((__int64)(m_ThresholdXingSearchIdx-(*M_preWaveSpanDataSize))); m_endDataPointIdx=qIdx((__int64)(m_ThresholdXingSearchIdx+(*M_postWaveSpanDataSize)));
	M_firstIdxOfWavelet[m_newlyRetrivedWaveID]=m_initDataPointIdx;
	if(m_initDataPointIdx<m_endDataPointIdx)M_lengthOfSignal[m_newlyRetrivedWaveID]=(*M_DataPointsPerWave); //Not fragmented
	else M_lengthOfSignal[m_newlyRetrivedWaveID]=m_AIQ_size-m_initDataPointIdx; //Fragmented
	//Adjust the x-axis so that the threshold crossing x value remains the same across the wavelets
	double a=M_AI[m_ThresholdXingSearchIdx+1]-M_AI[m_ThresholdXingSearchIdx], b=M_AI[m_ThresholdXingSearchIdx]-a*(*M_preWaveSpanDataSize);
	M_AdjustXToXthreshold[m_newlyRetrivedWaveID]=(*M_preWaveSpanDataSize)-((*M_Threshold)-b)/a;

	//######  Discrimiate whether the wave has been selected or not.
	if(m_numMatchingKinds>0){
		CheckWavesAgainstRejectBoxes(m_newlyRetrivedWaveID);
		if(m_totNumRejectedKinds!=m_numMatchingKinds){//In case not all of the rejection boxes have caught the subject wave...
			int WvEcode=CheckWavesAgainstSelcetBoxes(m_newlyRetrivedWaveID);
			M_WvEcode[m_newlyRetrivedWaveID]=WvEcode;
			if(WvEcode!=ECODE_UNSELECT){
				if(*M_AntidromicTriggeringChanID==m_ChanID && *M_IsAntidromicStim==1 && *M_WhichAntidromicMode==VTBOX_STIM_MODE && (*m_pSamplingCounter)==0 && *pView->M_NumOfStimulationChannels>0){
					(*m_pSamplingCounter)=pView->M_NumOfSamplingsAfterStim[*pView->M_CurrIdx_Of_StimGateRelayID];//How frequently do you want to give an antidromic stimulation?
					pView->m_AiChan_M_timeStampMicS=M_timeStampMicS[m_ThresholdXingSearchIdx]; pView->m_nEstimatedAntidromicStimPointIdx=m_ThresholdXingSearchIdx;	pView->m_TriggeringChannel=m_ChanID;	pView->m_ArrayIndexForUsableAiId_handedOverByRequestingChan=ArrayIndex_forUsableAiId;	
					if(pView->m_IsResting==1)SetEvent(pView->m_Ev_WakeUp_ForAntidromicStimulation);
					pView->m_AntidromicJobRequest=1;
				}
				//Spike info.
				if(WvEcode==0){ //Nn 1 (Purple box)
					M_spikeWaveIdxOfNn1[m_idx_SpikeNn1]=m_newlyRetrivedWaveID;
					M_spikeTimeStampOfNn1[m_idx_SpikeNn1]=M_timeStampMicS[m_ThresholdXingSearchIdx];
					M_spike_IdxOfAIOfNn1[m_idx_SpikeNn1]=m_ThresholdXingSearchIdx;
					*M_usableSpikeIdxOfNn1=m_idx_SpikeNn1;
					if(m_idx_SpikeNn1<MAX_SPIKEs-1)m_idx_SpikeNn1++; else m_idx_SpikeNn1=0;
					if((*M_numSpikesOfNn1)<*M_Spike_QueueSize)(*M_numSpikesOfNn1)++; //It is a looped queue; old spikes will be overwritten
				}
				else if(WvEcode==1){ //Nn 2 (Green box)
					M_spikeWaveIdxOfNn2[m_idx_SpikeNn2]=m_newlyRetrivedWaveID;
					M_spikeTimeStampOfNn2[m_idx_SpikeNn2]=M_timeStampMicS[m_ThresholdXingSearchIdx];
					M_spike_IdxOfAIOfNn2[m_idx_SpikeNn2]=m_ThresholdXingSearchIdx;
					*M_usableSpikeIdxOfNn2=m_idx_SpikeNn2;
					if(m_idx_SpikeNn2<MAX_SPIKEs-1)m_idx_SpikeNn2++; else m_idx_SpikeNn2=0;
					if((*M_numSpikesOfNn2)<*M_Spike_QueueSize)(*M_numSpikesOfNn2)++; //It is a looped queue; old spikes will be overwritten
				}
				else if(WvEcode==2){ //Nn 3 (Cyan box)
					M_spikeWaveIdxOfNn3[m_idx_SpikeNn3]=m_newlyRetrivedWaveID;
					M_spikeTimeStampOfNn3[m_idx_SpikeNn3]=M_timeStampMicS[m_ThresholdXingSearchIdx];
					M_spike_IdxOfAIOfNn3[m_idx_SpikeNn3]=m_ThresholdXingSearchIdx;
					*M_usableSpikeIdxOfNn3=m_idx_SpikeNn3;
					if(m_idx_SpikeNn3<MAX_SPIKEs-1)m_idx_SpikeNn3++; else m_idx_SpikeNn3=0;
					if((*M_numSpikesOfNn3)<*M_Spike_QueueSize)(*M_numSpikesOfNn3)++; //It is a looped queue; old spikes will be overwritten
				}
			}
		}
		else{
			M_WvEcode[m_newlyRetrivedWaveID]=ECODE_UNSELECT;
		}
	}
	else{
		M_WvEcode[m_newlyRetrivedWaveID]=ECODE_UNSELECT;
	}
	*M_usableSIGi=m_ThresholdXingSearchIdx;

	(*M_usableWaveIdx)=m_newlyRetrivedWaveID;
	//Update m_newlyRetrivedWaveID counter
	if(m_newlyRetrivedWaveID==MAX_WAVE_PTRs-1)m_newlyRetrivedWaveID=0;
	else m_newlyRetrivedWaveID++; 
	m_totNumWavesRetrived++;
}

int CAiChannel::CheckWavesAgainstSelcetBoxes(int waveID)
{
	int startCnt,endCnt,NnID;
	float TopY,BtmY,Left,Rght;
	int i,dataCnt,firstIdxOfWave=M_firstIdxOfWavelet[waveID];

	float x0=M_AdjustXToXthreshold[waveID];
	for(NnID=0;NnID<MAX_NNs_PerCHAN;NnID++){
		if(m_numSelRects[NnID]<1 || m_IsThisRejected[NnID]==1)continue;
		for(i=0;i<m_numSelRects[NnID];i++){
			TopY=m_SelRectBdryTopYDataCoord[NnID][i]; BtmY=m_SelRectBdryBtmYDataCoord[NnID][i]; Left=m_SelRectBdryLftX_IdxInDbl[NnID][i]; Rght=m_SelRectBdryRhtX_IdxInDbl[NnID][i];
			startCnt=m_SelRectBdryLtIdx[NnID][i]; //Because the length of the wave is fixed (unless the user changes), the relative starting index for the box is also fixes. 
			endCnt  =m_SelRectBdryRtIdx[NnID][i];
			if(startCnt+firstIdxOfWave<m_AIQ_size && endCnt+firstIdxOfWave<m_AIQ_size){//"qIdx(firstIdxOfWave+dataCnt)" will NOT run across the border of M_AI[] array
				for(dataCnt=startCnt;dataCnt<endCnt;dataCnt++){
					if(LineCrossingRect(dataCnt+x0,M_AI[firstIdxOfWave+dataCnt],dataCnt+1+x0,M_AI[firstIdxOfWave+dataCnt+1],Left,TopY,Rght,BtmY)){
						goto SEARCH_NEXT_SEL_BOX;//A box has found a matching segment.
					}	
				}	
				goto SEARCH_NEXT_NN;//The segment did not match the box
			}
			else{//"qIdx(firstIdxOfWave+dataCnt)" WILL run across the border of M_AI[] array
				for(dataCnt=startCnt;dataCnt<endCnt;dataCnt++){
					if(LineCrossingRect(dataCnt+x0,M_AI[qIdx((__int64)(firstIdxOfWave+dataCnt))],dataCnt+1+x0,M_AI[qIdx((__int64)(firstIdxOfWave+dataCnt+1))],Left,TopY,Rght,BtmY)){
						goto SEARCH_NEXT_SEL_BOX;//A box has found a matching segment.
					}	
				}	
				goto SEARCH_NEXT_NN;//The segment did not match the box
			}	
			SEARCH_NEXT_SEL_BOX:; //A box has found a matching segment.
		}
		return NnID;//All the boxes of one category had a matching segments.
		SEARCH_NEXT_NN:;
	}
	return ECODE_UNSELECT;//The wavelet has been rejected by all categories.
}


void CAiChannel::CheckWavesAgainstRejectBoxes(int waveID)
{
	int startCnt,endCnt,NnID;
	float TopY,BtmY,Left,Rght;
	int i,dataCnt,firstIdxOfWave=M_firstIdxOfWavelet[waveID];

	float x0=M_AdjustXToXthreshold[waveID];
	m_totNumRejectedKinds=0;
	for(NnID=0;NnID<MAX_NNs_PerCHAN;NnID++){
		m_IsThisRejected[NnID]=0;//default assumption, not rejected
		if(m_numRejRects[NnID]<1)continue;
		for(i=0;i<m_numRejRects[NnID];i++){
			TopY=m_RejRectBdryTopYDataCoord[NnID][i]; BtmY=m_RejRectBdryBtmYDataCoord[NnID][i]; Left=m_RejRectBdryLftX_IdxInDbl[NnID][i]; Rght=m_RejRectBdryRhtX_IdxInDbl[NnID][i];
			startCnt=m_RejRectBdryLtIdx[NnID][i];
			endCnt  =m_RejRectBdryRtIdx[NnID][i];
			if(startCnt+firstIdxOfWave<m_AIQ_size && endCnt+firstIdxOfWave<m_AIQ_size){//"qIdx(firstIdxOfWave+dataCnt)" will NOT run across the border of M_AI[] array
				for(dataCnt=startCnt;dataCnt<endCnt;dataCnt++){
					if(LineCrossingRect(dataCnt+x0,M_AI[firstIdxOfWave+dataCnt],dataCnt+1+x0,M_AI[firstIdxOfWave+dataCnt+1],Left,TopY,Rght,BtmY)){
						m_IsThisRejected[NnID]=1; m_totNumRejectedKinds++; goto END_OF_ONE_REJ_BOX; 
			}	}	}
			else{//"qIdx(firstIdxOfWave+dataCnt)" WILL run across the border of M_AI[] array
				for(dataCnt=startCnt;dataCnt<endCnt;dataCnt++){
					if(LineCrossingRect(dataCnt+x0,M_AI[qIdx((__int64)(firstIdxOfWave+dataCnt))],dataCnt+1+x0,M_AI[qIdx((__int64)(firstIdxOfWave+dataCnt+1))],Left,TopY,Rght,BtmY)){
						m_IsThisRejected[NnID]=1; m_totNumRejectedKinds++; goto END_OF_ONE_REJ_BOX; 
			}	}	}	
		}
		END_OF_ONE_REJ_BOX:;
	}
}

//Re-register the minimum required length of the wave to meet the requirement of the examination with "boxes".
void CAiChannel::MiniRequiredLengthOfwaveForExamination()
{
	int startCnt,endCnt,NnID;
	int i,BiggestIdx=0;//"0" will be the length when there is no box.

	for(NnID=0;NnID<MAX_NNs_PerCHAN;NnID++){
		if(m_numSelRects[NnID]<1)continue;
		for(i=0;i<m_numSelRects[NnID];i++){
			endCnt  =m_SelRectBdryRtIdx[NnID][i];
			if(BiggestIdx<endCnt)BiggestIdx=endCnt;
		}
	}
	for(NnID=0;NnID<MAX_NNs_PerCHAN;NnID++){
		if(m_numRejRects[NnID]<1)continue;
		for(i=0;i<m_numRejRects[NnID];i++){
			endCnt  =m_RejRectBdryRtIdx[NnID][i];
			if(BiggestIdx<endCnt)BiggestIdx=endCnt;
		}
	}
	*M_MiniRequiredLengthOfwaveForExamination=BiggestIdx;
}
//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CAiChannel::LineCrossingRect(double x1,double y1,double x2,double y2,float recLeft,float recTop,float recRight,float recBottom)
{
#define CCW(ax, ay, bx, by, cx, cy ) ((cy-ay)*(bx-ax)>(by-ay)*(cx-ax))
	if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recLeft,recTop,recRight,recTop,x1,y1)!=CCW(recLeft,recTop,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recRight,recBottom)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recRight,recBottom,recRight,recTop,x1,y1)!=CCW(recRight,recBottom,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recBottom)!=CCW(x1,y1,x2,y2,recRight,recBottom))&&(CCW(recLeft,recBottom,recRight,recBottom,x1,y1)!=CCW(recLeft,recBottom,recRight,recBottom,x2,y2))
		)return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recLeft,recBottom))&&(CCW(recLeft,recTop,recLeft,recBottom,x1,y1)!=CCW(recLeft,recTop,recLeft,recBottom,x2,y2))
		)
		return 1;
	else return 0;
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CAiChannel::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
{
#define CCW(ax, ay, bx, by, cx, cy ) ((cy-ay)*(bx-ax)>(by-ay)*(cx-ax))
	if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recLeft,recTop,recRight,recTop,x1,y1)!=CCW(recLeft,recTop,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recRight,recBottom)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recRight,recBottom,recRight,recTop,x1,y1)!=CCW(recRight,recBottom,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recBottom)!=CCW(x1,y1,x2,y2,recRight,recBottom))&&(CCW(recLeft,recBottom,recRight,recBottom,x1,y1)!=CCW(recLeft,recBottom,recRight,recBottom,x2,y2))
		)return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recLeft,recBottom))&&(CCW(recLeft,recTop,recLeft,recBottom,x1,y1)!=CCW(recLeft,recTop,recLeft,recBottom,x2,y2))
		)
		return 1;
	else return 0;
}
   

//When the infomation of the "client's" VT box changes, it updates on the server side
void CAiChannel::UpdateVTBoxes()
{
	int Nn, Rct;

	m_numMatchingKinds=pView->M_numMatchingKinds[m_ChanID];
	if(*M_WhichAntidromicMode!=AUTO_STIM_MODE){if(m_numMatchingKinds>0)*M_WhichAntidromicMode=VTBOX_STIM_MODE; else *M_WhichAntidromicMode=THRESHOLD_STIM_MODE;}
	for(Nn=0;Nn<MAX_NNs_PerCHAN;Nn++){
		m_numSelRects[Nn]=pView->M_numSelRects[m_ChanID][Nn];
		m_numRejRects[Nn]=pView->M_numRejRects[m_ChanID][Nn];
		for(Rct=0;Rct<m_numSelRects[Nn];Rct++){
			m_SelRectBdryLtIdx[Nn][Rct]			=pView->M_SelRectBdryLtIdx[m_ChanID][Nn][Rct];
			m_SelRectBdryRtIdx[Nn][Rct]			=pView->M_SelRectBdryRtIdx[m_ChanID][Nn][Rct];//Left and right boundaries of selection Boxes - in "data index" coordinate (intger form)
			m_SelRectBdryLftX_IdxInDbl[Nn][Rct]	=pView->M_SelRectBdryLftX_IdxInDbl[m_ChanID][Nn][Rct];
			m_SelRectBdryRhtX_IdxInDbl[Nn][Rct]	=pView->M_SelRectBdryRhtX_IdxInDbl[m_ChanID][Nn][Rct];//Left and right boundaries of selection Boxes - in "data index" coordinate (in fraction form)
			m_SelRectBdryTopYDataCoord[Nn][Rct]	=pView->M_SelRectBdryTopYDataCoord[m_ChanID][Nn][Rct];
			m_SelRectBdryBtmYDataCoord[Nn][Rct]	=pView->M_SelRectBdryBtmYDataCoord[m_ChanID][Nn][Rct]; //Selection Boxes - in "data" coordinate
		}
		for(Rct=0;Rct<m_numRejRects[Nn];Rct++){
			m_RejRectBdryLtIdx[Nn][Rct]			=pView->M_RejRectBdryLtIdx[m_ChanID][Nn][Rct];
			m_RejRectBdryRtIdx[Nn][Rct]			=pView->M_RejRectBdryRtIdx[m_ChanID][Nn][Rct];//Left and right boundaries of rejection Boxes - in "data index" coordinate (intger form)
			m_RejRectBdryLftX_IdxInDbl[Nn][Rct]	=pView->M_RejRectBdryLftX_IdxInDbl[m_ChanID][Nn][Rct];
			m_RejRectBdryRhtX_IdxInDbl[Nn][Rct]	=pView->M_RejRectBdryRhtX_IdxInDbl[m_ChanID][Nn][Rct];//Left and right boundaries of rejection Boxes - in "data index" coordinate (in fraction form)
			m_RejRectBdryTopYDataCoord[Nn][Rct]	=pView->M_RejRectBdryTopYDataCoord[m_ChanID][Nn][Rct];
			m_RejRectBdryBtmYDataCoord[Nn][Rct]	=pView->M_RejRectBdryBtmYDataCoord[m_ChanID][Nn][Rct];//Rejection Boxes - in "data" coordinate
		}
	}
	m_HighToLowCrossing=pView->M_IsFromHighToLow[m_ChanID];
	(*M_preWaveSpanDataSize) =TIME_TO_DATA_POINTS(pView->M_WaveSpanPre_ms[m_ChanID]);
	(*M_postWaveSpanDataSize)=TIME_TO_DATA_POINTS(pView->M_WaveSpanPost_ms[m_ChanID]);
	(*M_DataPointsPerWave)=(*M_postWaveSpanDataSize)+(*M_preWaveSpanDataSize);
	MiniRequiredLengthOfwaveForExamination();//Re-register the minimum required length of the wave to meet the requirement of the examination with "boxes".
	pView->M_ChanBoxInfoChanged[m_ChanID]=0;
}

