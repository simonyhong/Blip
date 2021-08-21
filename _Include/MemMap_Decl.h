	//########################  Memory Map ##########################
	//Parameters and variables
	HANDLE	hFile_M_SamplingRate;				int  *M_SamplingRate;
	HANDLE	hFile_M_AIQueueSize;			__int64 *M_AIQueueSize;
	HANDLE	hFile_M_WavePointer_QueueSize;		int  *M_WavePointer_QueueSize;
	HANDLE	hFile_M_Spike_QueueSize;			int  *M_Spike_QueueSize;
	HANDLE	hFile_M_DO_QueueSize;				int  *M_DO_QueueSize;
	HANDLE	hFile_M_Ecode_QueueSize;			int  *M_Ecode_QueueSize;
	HANDLE	hFile_M_SamplesPerRead;				int  *M_SamplesPerRead;
	HANDLE	hFile_M_SamplingEpoch_us;			int  *M_SamplingEpoch_us;
	HANDLE	hFile_M_microSecPerPoint;			int  *M_microSecPerPoint;
	HANDLE	hFile_M_numNeuralChannels;			int  *M_numNeuralChannels;
	HANDLE	hFile_M_numBehaviorChans;			int  *M_numBehaviorChans;
	HANDLE	hFile_M_LatestTimeStampInMicroS;	__int64 *M_LatestTimeStampInMicroS;
	HANDLE	hFile_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec; int *M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec;
	HANDLE	hFile_M_usableWaveIdx;	int *M_usableWaveIdx;
	HANDLE	hFile_M_Variable_For_Anything;		int  *M_Variable_For_Anything;
	HANDLE	hFile_M_Blip_OFF;					int  *M_Blip_OFF;
	//################################# Raw Analog Inputs ######################################################################################################
	HANDLE	hFile_M_AI[MAX_AI_CHAN];			 double *M_AI[MAX_AI_CHAN];
	HANDLE	hFile_M_timeStampMicS[MAX_AI_CHAN];	__int64 *M_timeStampMicS[MAX_AI_CHAN];
	HANDLE	hFile_M_usableAiIdx[MAX_AI_CHAN];	__int64 *M_usableAiIdx[MAX_AI_CHAN];
	HANDLE	hFile_M_NumOfFigsUsingThisChan;			int *M_NumOfFigsUsingThisChan;
	HANDLE	hFile_M_SampTimeArrayInMicS;		__int64 *M_SampTimeArrayInMicS;
	HANDLE	hFile_M_SampTimeArrayIdx;				int *M_SampTimeArrayIdx;
	//################################# Neural Analog Channels #################################################################################################
	// Voltage-Time Discriminator Setting
	HANDLE	hFile_M_preWaveSpanDataSize;	int *M_preWaveSpanDataSize;
	HANDLE	hFile_M_postWaveSpanDataSize;	int *M_postWaveSpanDataSize;
	HANDLE	hFile_M_DataPointsPerWave;		int *M_DataPointsPerWave;
	HANDLE	hFile_M_Threshold;				double *M_Threshold;
	//All threshold-crossing wavelets
	HANDLE	hFile_M_WaveSpanPre_ms;			   float *M_WaveSpanPre_ms;
	HANDLE	hFile_M_WaveSpanPost_ms;		   float *M_WaveSpanPost_ms;
	HANDLE	hFile_M_MiniRequiredLengthOfwaveForExamination;	int *M_MiniRequiredLengthOfwaveForExamination;
	HANDLE	hFile_M_firstIdxOfWavelet[NUM_NN_AI_CHAN];		int *M_firstIdxOfWavelet[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_lengthOfSignal[NUM_NN_AI_CHAN];			int *M_lengthOfSignal[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_WvEcode[NUM_NN_AI_CHAN];				int *M_WvEcode[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_AdjustXToXthreshold[NUM_NN_AI_CHAN]; double *M_AdjustXToXthreshold[NUM_NN_AI_CHAN] ;
	//Spikes(selected signallets)
	HANDLE	hFile_M_usableSpikeIdxOfNn1[NUM_NN_AI_CHAN];    int *M_usableSpikeIdxOfNn1[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_usableSpikeIdxOfNn2[NUM_NN_AI_CHAN];    int *M_usableSpikeIdxOfNn2[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_usableSpikeIdxOfNn3[NUM_NN_AI_CHAN];    int *M_usableSpikeIdxOfNn3[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_spikeWaveIdxOfNn1[NUM_NN_AI_CHAN];      int *M_spikeWaveIdxOfNn1[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spikeWaveIdxOfNn2[NUM_NN_AI_CHAN];      int *M_spikeWaveIdxOfNn2[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spikeWaveIdxOfNn3[NUM_NN_AI_CHAN];      int *M_spikeWaveIdxOfNn3[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spikeTimeStampOfNn1[NUM_NN_AI_CHAN];__int64 *M_spikeTimeStampOfNn1[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spikeTimeStampOfNn2[NUM_NN_AI_CHAN];__int64 *M_spikeTimeStampOfNn2[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spikeTimeStampOfNn3[NUM_NN_AI_CHAN];__int64 *M_spikeTimeStampOfNn3[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spike_IdxOfAIOfNn1[NUM_NN_AI_CHAN];__int64 *M_spike_IdxOfAIOfNn1[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spike_IdxOfAIOfNn2[NUM_NN_AI_CHAN];__int64 *M_spike_IdxOfAIOfNn2[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_spike_IdxOfAIOfNn3[NUM_NN_AI_CHAN];__int64 *M_spike_IdxOfAIOfNn3[NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_numSpikesOfNn1[NUM_NN_AI_CHAN];			int *M_numSpikesOfNn1[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_numSpikesOfNn2[NUM_NN_AI_CHAN];			int *M_numSpikesOfNn2[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_numSpikesOfNn3[NUM_NN_AI_CHAN];			int *M_numSpikesOfNn3[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_firstSpkIdxInTrialOfNn1[NUM_NN_AI_CHAN];int *M_firstSpkIdxInTrialOfNn1[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_firstSpkIdxInTrialOfNn2[NUM_NN_AI_CHAN];int *M_firstSpkIdxInTrialOfNn2[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_firstSpkIdxInTrialOfNn3[NUM_NN_AI_CHAN];int *M_firstSpkIdxInTrialOfNn3[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_usableSIGi[NUM_NN_AI_CHAN];			__int64 *M_usableSIGi[NUM_NN_AI_CHAN];
	//Spikes(Displayed/Non-displayed signallets)
	HANDLE	hFile_M_SpotlightChanID_forCLAS;				int *M_SpotlightChanID_forCLAS;
	HANDLE	hFile_M_SelectedWaveFormsToDisp;				int *M_SelectedWaveFormsToDisp;
	HANDLE	hFile_M_NumSelectedWaveformsDisplayed;			int *M_NumSelectedWaveformsDisplayed;
	HANDLE	hFile_M_NonSelectedWaveFormsToDisp;				int *M_NonSelectedWaveFormsToDisp;
	HANDLE	hFile_M_NumNonSelectedWaveformsDisplayed;		int *M_NumNonSelectedWaveformsDisplayed;
	//For VT boxes
	HANDLE	hFile_M_Flip_AI_SignalSign;								int *M_Flip_AI_SignalSign;
	HANDLE	hFile_M_HeyDASsv_SomeChanBoxInfoChanged;				int *M_HeyDASsv_SomeChanBoxInfoChanged;
	HANDLE	hFile_M_HeyANTS_SomeChanBoxInfoChanged;					int *M_HeyANTS_SomeChanBoxInfoChanged;
	HANDLE	hFile_M_ChanBoxInfoChanged;								  int *M_ChanBoxInfoChanged;
	HANDLE	hFile_M_IsFromHighToLow;								  int *M_IsFromHighToLow;
	HANDLE	hFile_M_numMatchingKinds;								  int *M_numMatchingKinds;
	HANDLE	hFile_M_SelRectBdryLtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];int *M_SelRectBdryLtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_SelRectBdryRtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];int *M_SelRectBdryRtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_RejRectBdryLtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];int *M_RejRectBdryLtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_RejRectBdryRtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];int *M_RejRectBdryRtIdx[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_SelRectBdryLftX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_SelRectBdryLftX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_SelRectBdryRhtX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_SelRectBdryRhtX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_RejRectBdryLftX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_RejRectBdryLftX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_RejRectBdryRhtX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_RejRectBdryRhtX_IdxInDbl[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_SelRectBdryTopYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_SelRectBdryTopYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_SelRectBdryBtmYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_SelRectBdryBtmYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_RejRectBdryTopYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_RejRectBdryTopYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_RejRectBdryBtmYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];double *M_RejRectBdryBtmYDataCoord[NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_numSelRects[NUM_NN_AI_CHAN];int *M_numSelRects[NUM_NN_AI_CHAN];
	HANDLE	hFile_M_numRejRects[NUM_NN_AI_CHAN];int *M_numRejRects[NUM_NN_AI_CHAN];
	//################################# Behavior Analog Channels ##############################################################################################
	//Behavior signals use "sparce (1kH)" time stamps provided by (*M_timeStamp4BehaviorAI_MicS)
	HANDLE	hFile_M_BehaviorSamplingRate;		    int *M_BehaviorSamplingRate;
	HANDLE	hFile_M_timeStamp4BehaviorAI_MicS;	__int64 *M_timeStamp4BehaviorAI_MicS;
	HANDLE	hFile_M_firstBehaviorAiIdxInTrial;	__int64 *M_firstBehaviorAiIdxInTrial;
	//################################# Other Parameters ######################################################################################################
	//Indexes and parameters
	HANDLE	hFile_M_isDAS_Running;				int *M_isDAS_Running;
	HANDLE	hFile_M_isDAScl_Running;			int *M_isDAScl_Running;
	HANDLE	hFile_M_isANTS_Running;				int *M_isANTS_Running;
	HANDLE	hFile_M_DO_Array;					unsigned char *M_DO_Array;
	HANDLE	hFile_M_DI_Array;					unsigned char *M_DI_Array;
	//#################################  Task Parameters  ######################################################################################################
	//Event Codes+
	HANDLE	hFile_M_FirstEcodeIdxOfTrial;	int *M_FirstEcodeIdxOfTrial;
	HANDLE	hFile_M_usableEcodeIdx;			int *M_usableEcodeIdx;
	//Ecodes
	HANDLE	hFile_M_numEcodes;		        int *M_numEcodes;
	HANDLE	hFile_M_EventCodes;		        int *M_EventCodes;
	HANDLE	hFile_M_EventTimes;		    __int64 *M_EventTimes;		

	HANDLE	hFile_M_isMOXY_Running;			int *M_isMOXY_Running;
	HANDLE	hFile_M_isTAS_Running;			int *M_isTAS_Running;
	HANDLE	hFile_M_isThereNewTrial;		int *M_isThereNewTrial;
	HANDLE	hFile_M_NnNumberToRecord;		int *M_NnNumberToRecord;
	//################################   TRIALS stored in the MAP   ##########################################################################################
	HANDLE	hFile_M_NumTrlsInMAP;					int *M_NumTrlsInMAP;
	HANDLE	hFile_M_usableTrialIdxInMem;			int *M_usableTrialIdxInMem;
	HANDLE	hFile_M_TrialID[MAX_TRLS_IN_MAP];		int *M_TrialID[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_StartTime[MAX_TRLS_IN_MAP];	__int64 *M_StartTime[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_EndTime[MAX_TRLS_IN_MAP];	__int64 *M_EndTime[MAX_TRLS_IN_MAP];
	//Ecodes
	HANDLE	hFile_M_numEcodesMAP[MAX_TRLS_IN_MAP];	int *M_numEcodesMAP[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_ECodesMAP_size;					int *M_ECodesMAP_size;
	HANDLE	hFile_M_Events_ReMapped;				int *M_Events_ReMapped;
	HANDLE	hFile_M_ECodesMAP[MAX_TRLS_IN_MAP];		int *M_ECodesMAP[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_EventTimesMAP[MAX_TRLS_IN_MAP];	__int64 *M_EventTimesMAP[MAX_TRLS_IN_MAP];		
	//Spikes (in trials stored in the map)
	HANDLE	hFile_M_NumChansHavingSpikes[MAX_TRLS_IN_MAP];		 int *M_NumChansHavingSpikes[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_ChanIDsHavingSpikes[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];		 int *M_ChanIDsHavingSpikes[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	HANDLE	hFile_M_NumNnsInChanHavingSpikes[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_NumNnsInChanHavingSpikes[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	HANDLE	hFile_M_NnIdWithSpk[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN][MAX_NNs_PerCHAN]; int *M_NnIdWithSpk[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN][MAX_NNs_PerCHAN];
	HANDLE	hFile_M_numSpksOfNn1MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_numSpksOfNn1MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	HANDLE	hFile_M_numSpksOfNn2MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_numSpksOfNn2MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	HANDLE	hFile_M_numSpksOfNn3MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_numSpksOfNn3MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	HANDLE	hFile_M_SpkTimeOfNn1MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;__int64 *M_SpkTimeOfNn1MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_SpkTimeOfNn1MAP_ReMapped[MAX_TRLS_IN_MAP];			  int *M_SpkTimeOfNn1MAP_ReMapped[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_SpkTimeOfNn2MAP_ReMapped[MAX_TRLS_IN_MAP];			  int *M_SpkTimeOfNn2MAP_ReMapped[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_SpkTimeOfNn3MAP_ReMapped[MAX_TRLS_IN_MAP];			  int *M_SpkTimeOfNn3MAP_ReMapped[MAX_TRLS_IN_MAP];
	HANDLE	hFile_M_SpkTimeOfNn1MAP_size[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_SpkTimeOfNn1MAP_size[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_SpkTimeOfNn2MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN]; __int64 *M_SpkTimeOfNn2MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_SpkTimeOfNn2MAP_size[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_SpkTimeOfNn2MAP_size[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_SpkTimeOfNn3MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN]; __int64 *M_SpkTimeOfNn3MAP[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;
	HANDLE	hFile_M_SpkTimeOfNn3MAP_size[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];int *M_SpkTimeOfNn3MAP_size[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN] ;
	//################################   Antidromic Stimulations  ##########################################################################################
	HANDLE	hFile_M_IsAntidromicStim;				int *M_IsAntidromicStim;
	HANDLE	hFile_M_WhichAntidromicMode;			int *M_WhichAntidromicMode;
	HANDLE	hFile_M_HeyMOXY_StimGivenToGatingDIO_X;	int *M_HeyMOXY_StimGivenToGatingDIO_X;
	HANDLE	hFile_M_HeyANTS_StimGivenToGatingDIO_X;	int *M_HeyANTS_StimGivenToGatingDIO_X;
	HANDLE	hFile_M_StimDigitalOutChannels;			int *M_StimDigitalOutChannels;
	HANDLE	hFile_M_is_LFP_saving;					int *M_is_LFP_saving;
	//##### All Antidromic Stimulatin channels
	HANDLE	hFile_M_NumOfStimulationChannels;		int *M_NumOfStimulationChannels;
	HANDLE	hFile_M_StimulationRelayID;				int *M_StimulationRelayID;
	HANDLE	hFile_M_CurrIdx_Of_StimGateRelayID;		int *M_CurrIdx_Of_StimGateRelayID;
	//##### Focused Antidromic Stimulatin channels
	HANDLE	hFile_M_NumOfFocusChannels;				int *M_NumOfFocusChannels;
	HANDLE	hFile_M_isFocusedChannel;				int *M_isFocusedChannel;

	HANDLE	hFile_M_InterStimulationInterval;		int *M_InterStimulationInterval;
	HANDLE	hFile_M_NumOfSamplingsAfterStim;		int *M_NumOfSamplingsAfterStim;
	HANDLE	hFile_M_StimulationLatencyBias_InMicroS;int *M_StimulationLatencyBias_InMicroS;
	HANDLE	hFile_M_TriggerToStimulationLatency;			int *M_TriggerToStimulationLatency;
	//*M_AntidromicTriggeringChanID: The neural channel from which the TRIGGERING neural signal is provided. 
	//Curreltly, only one recording channel is supported for antidromic stimulation
	HANDLE	hFile_M_AntidromicTriggeringChanID;		int *M_AntidromicTriggeringChanID; 
	HANDLE	hFile_M_AntiStimTime;					__int64 *M_AntiStimTime;
	HANDLE	hFile_M_EstimatedAntidromicStimPointIdx;				__int64 *M_EstimatedAntidromicStimPointIdx;
	HANDLE	hFile_M_RememberedTurnedOnStimRelayID;	int *M_RememberedTurnedOnStimRelayID;
	//Zapper
	HANDLE	hFile_M_isZapperLearning;				int *M_isZapperLearning;
	HANDLE	hFile_M_StartAIidx_ofArtifact_4ANTS;	__int64 *M_StartAIidx_ofArtifact_4ANTS;
	struct Zapper{
		double M_Artifact[MAX_NUM_STIM_CHANNELS][ARTIFACT_SIZE]; 
		double M_LearnedLevel[MAX_NUM_STIM_CHANNELS];
		int M_StimRelayID[MAX_NUM_STIM_CHANNELS];
		int M_NumGatingRelayIDs;
	};
	HANDLE	hFile_M_Zapper;			Zapper *M_Zapper;//Currently, only one recording channel (*M_AntidromicTriggeringChanID) is used for zapping.
	//################################   Orthodromic Stimulations  ##########################################################################################
	HANDLE	hFile_M_IsToLearnArtifact;					int *M_IsToLearnArtifact;
	HANDLE	hFile_M_IsOrthoStim;						int *M_IsOrthoStim;
	HANDLE	hFile_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation;		HANDLE *M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation;
	HANDLE	hFile_M_HeyDAS_DoOrthodromicStimulationNOW;	int *M_HeyDAS_DoOrthodromicStimulationNOW;
	HANDLE	hFile_M_HeyDASsv_StimGatingRequest;			int *M_HeyDASsv_StimGatingRequest;
	HANDLE	hFile_M_EstimatedOrthodromicStimPointIdx;	__int64 *M_EstimatedOrthodromicStimPointIdx;
	HANDLE	hFile_M_OrthodromicRecordingChannel;		int *M_OrthodromicRecordingChannel;


	