	int i;
	if(!UnmapViewOfFile(M_SamplingRate)){ return;/*In case it has already been closed*/} 
	CloseHandle(hFile_M_SamplingRate); 

	//M_AIQueueSize
	if(!UnmapViewOfFile(M_AIQueueSize)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_AIQueueSize); 
	//M_DO_QueueSize
	if(!UnmapViewOfFile(M_DO_QueueSize)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DO_QueueSize); 
	//M_Spike_QueueSize
	if(!UnmapViewOfFile(M_Spike_QueueSize)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Spike_QueueSize); 
	//M_usableTrialIdxInMem
	if(!UnmapViewOfFile(M_usableTrialIdxInMem)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_usableTrialIdxInMem); 
	//M_Ecode_QueueSize
	if(!UnmapViewOfFile(M_Ecode_QueueSize)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Ecode_QueueSize); 
	//M_SamplesPerRead
	if(!UnmapViewOfFile(M_SamplesPerRead)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_SamplesPerRead); 
	//M_SamplingEpoch_us
	if(!UnmapViewOfFile(M_SamplingEpoch_us)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_SamplingEpoch_us); 
	//M_microSecPerPoint
	if(!UnmapViewOfFile(M_microSecPerPoint)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_microSecPerPoint); 
	//M_numNeuralChannels
	if(!UnmapViewOfFile(M_numNeuralChannels)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_numNeuralChannels); 
	//M_numBehaviorChans
	if(!UnmapViewOfFile(M_numBehaviorChans)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_numBehaviorChans); 
	//M_Variable_For_Anything
	if(!UnmapViewOfFile(M_Variable_For_Anything)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Variable_For_Anything); 
	//M_Blip_OFF
	if(!UnmapViewOfFile(M_Blip_OFF)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Blip_OFF); 
	//M_WaveSpanPre_ms
	if(!UnmapViewOfFile(M_WaveSpanPre_ms)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_WaveSpanPre_ms); 
	//M_WaveSpanPost_ms
	if(!UnmapViewOfFile(M_WaveSpanPost_ms)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_WaveSpanPost_ms); 
	//M_MiniRequiredLengthOfwaveForExamination
	if(!UnmapViewOfFile(M_MiniRequiredLengthOfwaveForExamination)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MiniRequiredLengthOfwaveForExamination); 
	//M_LatestTimeStampInMicroS
	if(!UnmapViewOfFile(M_LatestTimeStampInMicroS)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_LatestTimeStampInMicroS); 
	//M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec
	if(!UnmapViewOfFile(M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec); 
	//M_isDAS_Running
	if(!UnmapViewOfFile(M_isDAS_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isDAS_Running); 
	//M_isDAScl_Running
	if(!UnmapViewOfFile(M_isDAScl_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isDAScl_Running); 
	//M_isANTS_Running
	if(!UnmapViewOfFile(M_isANTS_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isANTS_Running); 
	//##############   Raw Data   #####################
	for(i=0;i<MAX_AI_CHAN;i++){
		//M_usableAiIdx[i]
		if(!UnmapViewOfFile(M_usableAiIdx[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_usableAiIdx[i]); 
		//M_AI[i]
		if(!UnmapViewOfFile(M_AI[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_AI[i]); 
		//M_timeStampMicS
		if(!UnmapViewOfFile(M_timeStampMicS[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_timeStampMicS[i]); 
	}
	//M_NumOfFigsUsingThisChan
	if(!UnmapViewOfFile(M_NumOfFigsUsingThisChan)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumOfFigsUsingThisChan); 
	//M_SampTimeArrayInMicS
	if(!UnmapViewOfFile(M_SampTimeArrayInMicS)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_SampTimeArrayInMicS); 
	//M_SampTimeArrayIdx
	if(!UnmapViewOfFile(M_SampTimeArrayIdx)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_SampTimeArrayIdx); 
	//##############   Digital Out    #####################
	//M_DO_Array 
	if(!UnmapViewOfFile(M_DO_Array)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DO_Array); 
	//##############   Digital In    #####################
	//M_DI_Array 
	if(!UnmapViewOfFile(M_DI_Array)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DI_Array); 
	//##############   Behavior Analog Inputs   #####################
	//M_BehaviorSamplingRate
	if(!UnmapViewOfFile(M_BehaviorSamplingRate)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_BehaviorSamplingRate); 	
	//M_firstBehaviorAiIdxInTrial
	if(!UnmapViewOfFile(M_firstBehaviorAiIdxInTrial)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_firstBehaviorAiIdxInTrial); 
	//M_timeStamp4BehaviorAI_MicS
	if(!UnmapViewOfFile(M_timeStamp4BehaviorAI_MicS)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_timeStamp4BehaviorAI_MicS); 
	//##############   Voltage-Time Discriminator Setting   #####################
	//M_preWaveSpanDataSize
	if(!UnmapViewOfFile(M_preWaveSpanDataSize)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_preWaveSpanDataSize); 
	//M_postWaveSpanDataSize
	if(!UnmapViewOfFile(M_postWaveSpanDataSize)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_postWaveSpanDataSize); 
	//M_DataPointsPerWave
	if(!UnmapViewOfFile(M_DataPointsPerWave)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DataPointsPerWave); 
	//M_Threshold
	if(!UnmapViewOfFile(M_Threshold)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Threshold); 
	//##############   Neural Analog Inputs   #####################
	for(i=0;i<NUM_NN_AI_CHAN;i++){
		//##########  Analysing the raw Neural input  #########
		//M_usableSIGi[i]
		if(!UnmapViewOfFile(M_usableSIGi[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_usableSIGi[i]); 
		//##############   ALL wavelets   #####################
		//M_WvEcode[i] 
		if(!UnmapViewOfFile(M_WvEcode[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_WvEcode[i]); 
		//M_firstIdxOfWavelet[i] 
		if(!UnmapViewOfFile(M_firstIdxOfWavelet[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_firstIdxOfWavelet[i]); 
		//M_lengthOfSignal[i] 
		if(!UnmapViewOfFile(M_lengthOfSignal[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_lengthOfSignal[i]); 
		//M_AdjustXToXthreshold[i] //This parameter is to use to shift the wavelet to align the threshold-crossing point (M_AI[x]==threshold) with the VT-discriminator's 0 point.
		if(!UnmapViewOfFile(M_AdjustXToXthreshold[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_AdjustXToXthreshold[i]); 
		//##############   Spike data   #####################
		//M_usableSpikeIdxOfNn1[i]
		if(!UnmapViewOfFile(M_usableSpikeIdxOfNn1[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_usableSpikeIdxOfNn1[i]); 
		//M_usableSpikeIdxOfNn2[i]
		if(!UnmapViewOfFile(M_usableSpikeIdxOfNn2[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_usableSpikeIdxOfNn2[i]); 
		//M_usableSpikeIdxOfNn3[i]
		if(!UnmapViewOfFile(M_usableSpikeIdxOfNn3[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_usableSpikeIdxOfNn3[i]); 
		//M_spikeWaveIdxOfNn1[i] 
		if(!UnmapViewOfFile(M_spikeWaveIdxOfNn1[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spikeWaveIdxOfNn1[i]); 
		//M_spikeWaveIdxOfNn2[i] 
		if(!UnmapViewOfFile(M_spikeWaveIdxOfNn2[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spikeWaveIdxOfNn2[i]); 
		//M_spikeWaveIdxOfNn3[i] 
		if(!UnmapViewOfFile(M_spikeWaveIdxOfNn3[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spikeWaveIdxOfNn3[i]); 
		//M_spikeTimeStampOfNn1[i] 
		if(!UnmapViewOfFile(M_spikeTimeStampOfNn1[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spikeTimeStampOfNn1[i]); 
		//M_spikeTimeStampOfNn2[i] 
		if(!UnmapViewOfFile(M_spikeTimeStampOfNn2[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spikeTimeStampOfNn2[i]); 
		//M_spikeTimeStampOfNn3[i] 
		if(!UnmapViewOfFile(M_spikeTimeStampOfNn3[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spikeTimeStampOfNn3[i]); 
		//M_spike_IdxOfAIOfNn1[i] 
		if(!UnmapViewOfFile(M_spike_IdxOfAIOfNn1[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spike_IdxOfAIOfNn1[i]); 
		//M_spike_IdxOfAIOfNn2[i] 
		if(!UnmapViewOfFile(M_spike_IdxOfAIOfNn2[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spike_IdxOfAIOfNn2[i]); 
		//M_spike_IdxOfAIOfNn3[i] 
		if(!UnmapViewOfFile(M_spike_IdxOfAIOfNn3[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_spike_IdxOfAIOfNn3[i]); 
		//M_numSpikesOfNn1[i] 
		if(!UnmapViewOfFile(M_numSpikesOfNn1[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_numSpikesOfNn1[i]); 
		//M_numSpikesOfNn2[i] 
		if(!UnmapViewOfFile(M_numSpikesOfNn2[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_numSpikesOfNn2[i]); 
		//M_numSpikesOfNn3[i] 
		if(!UnmapViewOfFile(M_numSpikesOfNn3[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_numSpikesOfNn3[i]); 
		//M_firstSpkIdxInTrialOfNn1[i] 
		if(!UnmapViewOfFile(M_firstSpkIdxInTrialOfNn1[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_firstSpkIdxInTrialOfNn1[i]); 
		//M_firstSpkIdxInTrialOfNn2[i] 
		if(!UnmapViewOfFile(M_firstSpkIdxInTrialOfNn2[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_firstSpkIdxInTrialOfNn2[i]); 
		//M_firstSpkIdxInTrialOfNn3[i] 
		if(!UnmapViewOfFile(M_firstSpkIdxInTrialOfNn3[i])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_firstSpkIdxInTrialOfNn3[i]); 
	}
	//##############   Spike data   #####################
	//M_usableWaveIdx
	if(!UnmapViewOfFile(M_usableWaveIdx)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_usableWaveIdx); 
	//##############   VT Boxes   #####################
	//M_Flip_AI_SignalSign
	if(!UnmapViewOfFile(M_Flip_AI_SignalSign)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Flip_AI_SignalSign); 
	//M_HeyDASsv_SomeChanBoxInfoChanged
	if(!UnmapViewOfFile(M_HeyDASsv_SomeChanBoxInfoChanged)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyDASsv_SomeChanBoxInfoChanged); 
	//M_HeyANTS_SomeChanBoxInfoChanged
	if(!UnmapViewOfFile(M_HeyANTS_SomeChanBoxInfoChanged)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyANTS_SomeChanBoxInfoChanged); 
	//M_ChanBoxInfoChanged
	if(!UnmapViewOfFile(M_ChanBoxInfoChanged)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ChanBoxInfoChanged); 
	//M_IsFromHighToLow
	if(!UnmapViewOfFile(M_IsFromHighToLow)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_IsFromHighToLow); 
	//M_numMatchingKinds
	if(!UnmapViewOfFile(M_numMatchingKinds)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_numMatchingKinds); 
	int Ch,Nn;
	for(Ch=0;Ch<NUM_NN_AI_CHAN;Ch++){
		for(Nn=0;Nn<MAX_NNs_PerCHAN;Nn++){
			//M_SelRectBdryLtIdx[][]
			if(!UnmapViewOfFile(M_SelRectBdryLtIdx[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SelRectBdryLtIdx[Ch][Nn]); 
			//M_SelRectBdryRtIdx[][]
			if(!UnmapViewOfFile(M_SelRectBdryRtIdx[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SelRectBdryRtIdx[Ch][Nn]); 
			//M_RejRectBdryLtIdx[][]
			if(!UnmapViewOfFile(M_RejRectBdryLtIdx[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_RejRectBdryLtIdx[Ch][Nn]); 
			//M_RejRectBdryRtIdx[][]
			if(!UnmapViewOfFile(M_RejRectBdryRtIdx[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_RejRectBdryRtIdx[Ch][Nn]); 
			//M_SelRectBdryLftX_IdxInDbl[][]
			if(!UnmapViewOfFile(M_SelRectBdryLftX_IdxInDbl[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SelRectBdryLftX_IdxInDbl[Ch][Nn]); 
			//M_SelRectBdryRhtX_IdxInDbl[][]
			if(!UnmapViewOfFile(M_SelRectBdryRhtX_IdxInDbl[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SelRectBdryRhtX_IdxInDbl[Ch][Nn]); 
			//M_RejRectBdryLftX_IdxInDbl[][]
			if(!UnmapViewOfFile(M_RejRectBdryLftX_IdxInDbl[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_RejRectBdryLftX_IdxInDbl[Ch][Nn]); 
			//M_RejRectBdryRhtX_IdxInDbl[][]
			if(!UnmapViewOfFile(M_RejRectBdryRhtX_IdxInDbl[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_RejRectBdryRhtX_IdxInDbl[Ch][Nn]); 
			//M_SelRectBdryTopYDataCoord[][]
			if(!UnmapViewOfFile(M_SelRectBdryTopYDataCoord[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SelRectBdryTopYDataCoord[Ch][Nn]); 
			//M_SelRectBdryBtmYDataCoord[][]
			if(!UnmapViewOfFile(M_SelRectBdryBtmYDataCoord[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SelRectBdryBtmYDataCoord[Ch][Nn]); 
			//M_RejRectBdryTopYDataCoord[][]
			if(!UnmapViewOfFile(M_RejRectBdryTopYDataCoord[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_RejRectBdryTopYDataCoord[Ch][Nn]); 
			//M_RejRectBdryBtmYDataCoord[][]
			if(!UnmapViewOfFile(M_RejRectBdryBtmYDataCoord[Ch][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_RejRectBdryBtmYDataCoord[Ch][Nn]); 
		}
		//M_numSelRects[Ch]
		if(!UnmapViewOfFile(M_numSelRects[Ch])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_numSelRects[Ch]); 
		//M_numRejRects[Ch]
		if(!UnmapViewOfFile(M_numRejRects[Ch])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_numRejRects[Ch]); 
	}
	//##############   Trials and Event Codes   #####################
	//M_isMOXY_Running
	if(!UnmapViewOfFile(M_isMOXY_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isMOXY_Running); 
	//##############   Trials and Event Codes   #####################
	//M_isTAS_Running
	if(!UnmapViewOfFile(M_isTAS_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isTAS_Running); 
	//M_usableEcodeIdx
	if(!UnmapViewOfFile(M_usableEcodeIdx)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_usableEcodeIdx); 
	//M_numEcodes
	if(!UnmapViewOfFile(M_numEcodes)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_numEcodes); 
	//M_FirstEcodeIdxOfTrial
	if(!UnmapViewOfFile(M_FirstEcodeIdxOfTrial)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_FirstEcodeIdxOfTrial); 
	//M_EventCodes
	if(!UnmapViewOfFile(M_EventCodes)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EventCodes); 
	//M_EventTimes
	if(!UnmapViewOfFile(M_EventTimes)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EventTimes); 
	//M_isThereNewTrial
	if(!UnmapViewOfFile(M_isThereNewTrial)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isThereNewTrial); 
	//M_NnNumberToRecord
	if(!UnmapViewOfFile(M_NnNumberToRecord)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NnNumberToRecord); 
	//##############   Trials in the Memory Map   #####################
	//M_NumTrlsInMAP
	if(!UnmapViewOfFile(M_NumTrlsInMAP)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumTrlsInMAP); 
	//##############   Antidromic Stimulations   #####################
	//M_IsAntidromicStim
	if(!UnmapViewOfFile(M_IsAntidromicStim)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_IsAntidromicStim); 
	//M_WhichAntidromicMode
	if(!UnmapViewOfFile(M_WhichAntidromicMode)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_WhichAntidromicMode); 
	//M_HeyANTS_StimGivenToGatingDIO_X
	if(!UnmapViewOfFile(M_HeyANTS_StimGivenToGatingDIO_X)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyANTS_StimGivenToGatingDIO_X); 
	//M_is_LFP_saving
	if(!UnmapViewOfFile(M_is_LFP_saving)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_is_LFP_saving); 
	//M_HeyMOXY_StimGivenToGatingDIO_X
	if(!UnmapViewOfFile(M_HeyMOXY_StimGivenToGatingDIO_X)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyMOXY_StimGivenToGatingDIO_X); 
	//M_NumOfStimulationChannels
	if(!UnmapViewOfFile(M_NumOfStimulationChannels)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumOfStimulationChannels); 
	//M_StimDigitalOutChannels
	if(!UnmapViewOfFile(M_StimDigitalOutChannels)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StimDigitalOutChannels); 
	//M_StimulationRelayID
	if(!UnmapViewOfFile(M_StimulationRelayID)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StimulationRelayID); 
	//M_CurrIdx_Of_StimGateRelayID
	if(!UnmapViewOfFile(M_CurrIdx_Of_StimGateRelayID)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_CurrIdx_Of_StimGateRelayID); 
	//M_NumOfFocusChannels
	if(!UnmapViewOfFile(M_NumOfFocusChannels)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumOfFocusChannels); 
	//M_isFocusedChannel
	if(!UnmapViewOfFile(M_isFocusedChannel)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isFocusedChannel); 
	//M_InterStimulationInterval
	if(!UnmapViewOfFile(M_InterStimulationInterval)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_InterStimulationInterval); 
	//M_NumOfSamplingsAfterStim
	if(!UnmapViewOfFile(M_NumOfSamplingsAfterStim)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumOfSamplingsAfterStim); 
	//M_TriggerToStimulationLatency
	if(!UnmapViewOfFile(M_TriggerToStimulationLatency)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_TriggerToStimulationLatency); 
	//M_StimulationLatencyBias_InMicroS
	if(!UnmapViewOfFile(M_StimulationLatencyBias_InMicroS)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StimulationLatencyBias_InMicroS); 
	//M_AntidromicTriggeringChanID
	if(!UnmapViewOfFile(M_AntidromicTriggeringChanID)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_AntidromicTriggeringChanID); 
	//M_AntiStimTime
	if(!UnmapViewOfFile(M_AntiStimTime)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_AntiStimTime); 
	//M_EstimatedAntidromicStimPointIdx
	if(!UnmapViewOfFile(M_EstimatedAntidromicStimPointIdx)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EstimatedAntidromicStimPointIdx); 
	//M_RememberedTurnedOnStimRelayID
	if(!UnmapViewOfFile(M_RememberedTurnedOnStimRelayID)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_RememberedTurnedOnStimRelayID); 
	//Zapper
	//M_isZapperLearning
	if(!UnmapViewOfFile(M_isZapperLearning)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isZapperLearning); 
	//M_StartAIidx_ofArtifact_4ANTS
	if(!UnmapViewOfFile(M_StartAIidx_ofArtifact_4ANTS)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StartAIidx_ofArtifact_4ANTS); 
	//M_Zapper
	if(!UnmapViewOfFile(M_Zapper)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Zapper); 
	//##############   Orthodromic Stimulations   #####################
	//M_IsToLearnArtifact
	if(!UnmapViewOfFile(M_IsToLearnArtifact)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_IsToLearnArtifact); 
	//M_IsOrthoStim
	if(!UnmapViewOfFile(M_IsOrthoStim)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_IsOrthoStim); 
	//M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation
	if(!UnmapViewOfFile(M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation); 
	//M_HeyDAS_DoOrthodromicStimulationNOW
	if(!UnmapViewOfFile(M_HeyDAS_DoOrthodromicStimulationNOW)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyDAS_DoOrthodromicStimulationNOW); 
	//M_HeyDASsv_StimGatingRequest
	if(!UnmapViewOfFile(M_HeyDASsv_StimGatingRequest)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyDASsv_StimGatingRequest); 
	//M_EstimatedOrthodromicStimPointIdx
	if(!UnmapViewOfFile(M_EstimatedOrthodromicStimPointIdx)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EstimatedOrthodromicStimPointIdx); 
	//M_OrthodromicRecordingChannel
	if(!UnmapViewOfFile(M_OrthodromicRecordingChannel)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_OrthodromicRecordingChannel); 
