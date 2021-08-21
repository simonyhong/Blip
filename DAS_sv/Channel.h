#if !defined (CHANNEL_CLASS)
#define CHANNEL_CLASS

class CDasView;

class CAiChannel: public CObject
{
public:
	int		m_ChanID;
	double	gain; 
	////////////////////
	int		m_idx_SpikeNn1,m_idx_SpikeNn2,m_idx_SpikeNn3;
	__int64	m_AIQ_size, m_AIQ_size_1;
	double *M_AI;
	__int64	*M_timeStampMicS;
	__int64	m_AiIdx;
	__int64	*M_usableAiIdx;
	__int64	*M_AIQueueSize;
	__int64	m_ThresholdXingSearchIdx;
	__int64	*M_usableSIGi;

	int	*M_preWaveSpanDataSize;
	int	*M_postWaveSpanDataSize;
	int	*M_DataPointsPerWave;
	double	*M_Threshold;

	int		*M_firstIdxOfWavelet;
	int		*M_WvEcode;
	int		*M_MiniRequiredLengthOfwaveForExamination;
	int		*M_lengthOfSignal;
	int		*M_NumDisplayedWaveformsOnScreen;
	double	*M_AdjustXToXthreshold;
	int		*M_usableWaveIdx;
	int		*M_Spike_QueueSize;
	int		*M_usableSpikeIdxOfNn1,	*M_usableSpikeIdxOfNn2,	*M_usableSpikeIdxOfNn3;
	int		*M_spikeWaveIdxOfNn1, *M_spikeWaveIdxOfNn2, *M_spikeWaveIdxOfNn3;
	__int64	*M_spikeTimeStampOfNn1, *M_spikeTimeStampOfNn2, *M_spikeTimeStampOfNn3;
	__int64	*M_spike_IdxOfAIOfNn1, *M_spike_IdxOfAIOfNn2, *M_spike_IdxOfAIOfNn3;
	//Trial variables
	int		*M_numSpikesOfNn1,*M_numSpikesOfNn2,*M_numSpikesOfNn3;
	int		*M_firstSpkIdxInTrialOfNn1,*M_firstSpkIdxInTrialOfNn2,*M_firstSpkIdxInTrialOfNn3;
	//Antidromic variables
	int		*M_AntidromicTriggeringChanID, *M_IsAntidromicStim, *M_WhichAntidromicMode;

	int		*m_pSamplingCounter;
	__int64		ChanSamplCounter;

	CDasView *pView;
	CRITICAL_SECTION 	m_CriticalSection;
	//###############################  Device context to delete at the end of the program  #################################

	//########################################   VT_DISCRIMINATOR_TASK  ########################################
	int		m_HighToLowCrossing;
	int		m_HowManyTimesCalled2DispWaveform;
	int		m_newlyRetrivedWaveID,m_crntlyDisplayedWaveID,m_totNumWavesRetrived;
	int		m_initDataPointIdx,m_endDataPointIdx,m_RefreshAtEveryNWaveforms;

	//The VT-Box parameters that the server DAS and client DAS share
	int		m_numSelRects[MAX_NNs_PerCHAN];
	int		m_numRejRects[MAX_NNs_PerCHAN];
	void	UpdateVTBoxes();
	//Left and right boundaries of selection Boxes - in "index" coordinate (intger form): Start and End indexes to examine
	int		m_SelRectBdryLtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryRtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];
	int		m_RejRectBdryLtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryRtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];
	//Left and right boundaries of selection Boxes - in "index" coordinate (in fraction form): Online "line-crossing"
	double	m_SelRectBdryLftX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryRhtX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];
	double	m_RejRectBdryLftX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryRhtX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];
	//Selection Boxes - in "data" coordinate: Online "line-crossing"
	double	m_SelRectBdryTopYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryBtmYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];
	double	m_RejRectBdryTopYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryBtmYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON]; 

	//For the matching process
	int		m_totNumRejectedKinds, m_numMatchingKinds; //How many kinds (1, 2 or 3)
	int		m_IsThisRejected[MAX_NNs_PerCHAN];//During the process of matching, use this variable if there has been any rejection in any category
	//
	int		m_hasBoxMoved,m_selBoxColor,m_selectedBoxType,m_selectedIsToAccept,m_selectedBoxID,m_EdgeMoveType;
	int		m_RtClickSelectedBoxType,m_RtClickSelectedBoxIsToAccept,m_RtClickSelectedBoxID;
	int		CheckWavesAgainstSelcetBoxes(int waveID);
	void	CheckWavesAgainstRejectBoxes(int waveID);
	void	MiniRequiredLengthOfwaveForExamination();
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	LineCrossingRect(double x1,double y1,double x2,double y2,float recLeft,float recTop,float recRight,float recBottom);


	inline __int64 qIdx(__int64 i)
	{	if(i>=0 && i<m_AIQ_size)return i;
		else if(i>m_AIQ_size_1)return (i-m_AIQ_size);
		else return (m_AIQ_size+i);//if(i<0) => This happens when the program tries to retrieve the pre-threshold-crossing portion of the wave.
	}
	//##########################################################################################################

	void	OnAnalogInput(__int64 usableAiIdx, int ArrayIndex_forUsableAiId); 
	void	Register_n_Classify_wavelet(int	ArrayIndex_forUsableAiId); 
	//######################################################################################################################

	CAiChannel();
	virtual ~CAiChannel();
};



#endif 