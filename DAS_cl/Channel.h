#if !defined (CHANNEL_CLASS)
#define CHANNEL_CLASS

class CDasView;
class CFigure;
class CAiChannel: public CObject
{
public:
	int		m_IsThisChanUsed;
	int		m_ChanID;
	double	gain; 
	int		m_preWaveSpanDataSize,m_postWaveSpanDataSize; //Since DAS_cl generates the m_preWaveSpanDataSize, and informs DAS_sv to update M_preWaveSpanDataSize, DAS_cl does not need to have M_preWaveSpanDataSize itself.
	////////////////////
	int		m_idx_SpikeNn1,m_idx_SpikeNn2,m_idx_SpikeNn3;
	double *M_AI;
	__int64	*M_timeStampMicS;
	__int64	m_AiIdx;
	__int64	*M_usableAiIdx;
	__int64	*M_AIQueueSize;
	__int64	m_SIGi;
	__int64	*M_usableSIGi;
	double	*M_Threshold;
	int		*M_firstIdxOfWavelet;
	int		*M_WvEcode;
	int		*M_lengthOfSignal;
	double	*M_AdjustXToXthreshold;
	int		*M_Spike_QueueSize;
	int		*M_usableSpikeIdxOfNn1,	*M_usableSpikeIdxOfNn2,	*M_usableSpikeIdxOfNn3;
	int		*M_spikeWaveIdxOfNn1, *M_spikeWaveIdxOfNn2, *M_spikeWaveIdxOfNn3;
	__int64	*M_spikeTimeStampOfNn1, *M_spikeTimeStampOfNn2, *M_spikeTimeStampOfNn3;
	//Trial variables
	int		*M_numSpikesOfNn1,*M_numSpikesOfNn2,*M_numSpikesOfNn3;
	int		*M_firstSpkIdxInTrialOfNn1,*M_firstSpkIdxInTrialOfNn2,*M_firstSpkIdxInTrialOfNn3;
	inline __int64 qIdx(__int64 i)
	{	if(i>=0 && i<*M_AIQueueSize)return i;
		else if(i>=*M_AIQueueSize)return (i-*M_AIQueueSize);
		else return (*M_AIQueueSize+i);//if(i<0) => This happens when the program tries to retrieve the pre-threshold-crossing portion of the wave.
	}

	CDasView *pView;
	CFigure *pFig; //Only one figure is allowed to display the channel's signals
	//###############################  Device context to delete at the end of the program  #################################
	static	CDC	m_ChDC;
	static	CBitmap m_ChBmp;
	static	CPen m_VIOLETLIGHT2,m_GREEN2,m_CYAN2;
	static int m_DiviceContaxtCreatedAlready1,m_DiviceContaxtCreatedAlready2;

	//########################################   VT_DISCRIMINATOR_TASK  ########################################
	CArray<int, int& >m_isVT_Selected; 
	CArray<int, int& >m_SelectedWaveFormsToDisp; 

	int		m_IsThresholdHeld,m_HighToLowCrossing;
	int		m_initDataPointIdx,m_endDataPointIdx,m_DataPointsPerWave,m_MaxNumWaveformsToDisp;

	//Rectangle on Screen (to display boxes)
	CRect	m_SelRectsOnScrn[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectsOnScrn[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];
	//Rectangle in data coordinate (to re-create “m_SelRectsOnScrn”)
	double	m_SelRectBdryLftXDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryRhtXDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON]; //Selection Boxes - in "data" coordinate
	double	m_RejRectBdryLftXDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryRhtXDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON]; //Rejection Boxes - in "data" coordinate
	int		m_numSelRectsTmp[MAX_NNs_PerCHAN];
	int		m_numRejRectsTmp[MAX_NNs_PerCHAN];

	//The VT-Box parameters that the DAS server and client SHARE.
	void	UpdateServerWithVTBoxes();
	int		m_numSelRects[MAX_NNs_PerCHAN];
	int		m_numRejRects[MAX_NNs_PerCHAN];
	//Left and right boundaries of selection Boxes - in "index" coordinate (intger form): Start and End indexes to examine
	int		m_SelRectBdryLtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryRtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];//Left and right boundaries of selection Boxes - in "data index" coordinate (intger form)
	int		m_RejRectBdryLtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryRtIdx[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];//Left and right boundaries of rejection Boxes - in "data index" coordinate (intger form)
	//Left and right boundaries of selection Boxes - in "index" coordinate (in fraction form): Online "line-crossing"
	double	m_SelRectBdryLftX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryRhtX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];//Left and right boundaries of selection Boxes - in "data index" coordinate (in fraction form)
	double	m_RejRectBdryLftX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryRhtX_IdxInDbl[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON];//Left and right boundaries of rejection Boxes - in "data index" coordinate (in fraction form)
	//Selection Boxes - in "data" coordinate: Online "line-crossing"
	double	m_SelRectBdryTopYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_SelRectBdryBtmYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON]; //Selection Boxes - in "data" coordinate
	double	m_RejRectBdryTopYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON],m_RejRectBdryBtmYDataCoord[MAX_NNs_PerCHAN][MAX_VT_RECTs_PER_NEURON]; //Rejection Boxes - in "data" coordinate

	//For the matching process
	int		m_totNumRejected, m_numMatchingKinds; //How many kinds (1, 2 or 3)
	int		m_IsThisRejected[MAX_NNs_PerCHAN];//During the process of matching, use this variable if there has been any rejection in any category
	//
	int		m_hasBoxMoved,m_selBoxColor,m_selectedBoxType,m_selectedIsToAccept,m_selectedBoxID,m_EdgeMoveType;
	int		m_RtClickSelectedBoxType,m_RtClickSelectedBoxIsToAccept,m_RtClickSelectedBoxID;
	void	DrawOneVTbox(int colorType, int isToAccept, int BoxID,  int isToHighlight);
	void	DeleteOneVTBox(int colorType, int isToAccept, int BoxID);
	void	ShowAll_VTBoxes();
	int		WhichBox(CPoint &point, int &colorType,int &isToAccept, int &edgeMoveType);
	void	Remeasure_Box_Positions_DataAnchored();
	void	Remeasure_Box_YPositions_ScreenAnchored();
	void	Remeasure_Box_XY_Positions_ScreenAnchored();
	void	ReRegisterSelBoxes_andNoticeServer();
	void	ReCalibrateRelativeBoxIndex_OneBox(int colorType, int isToAccept, int BoxID);
	void	Redraw_BoxEdge(CPoint &point, int colorType, int isToAccept, int BoxID, int &edgeMoveType);
	void	RedrawDiscriminatorRect(CPoint &point, int colorType, int isToAccept, int BoxID);
	void	RegisterSelBox(int SelColor, int isToAccept, int BoxID);
	int		SearchForDataIdx_closeButBeforeX(float X, int candidateIdx);
	int		SearchForDataIdx_closeButAfterX(float X, int candidateIdx);
	CPoint	m_prvPoint;
	//##########################################################################################################

	void	FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY);
	
	CAiChannel();
	virtual ~CAiChannel();
};



#endif 