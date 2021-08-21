#if !defined (CHANNEL_CLASS)
#define CHANNEL_CLASS

class CRASView;
class CFigure;
class CAiChannel: public CObject
{
public:
	int		m_chanID;
	double	gain; 
	int		m_firstConnectionToMap;
	////////////////////
	int		m_idx_Spike;
	double *M_AI;
	__int64	*M_timeStampMicS;//Pointer to the time array
	__int64	*M_LatestTimeStampInMicroS;//The latest AI time stamp
	__int64	m_AiIdx;
	__int64	*M_usableAiIdx;
	__int64	*M_AIQueueSize;
	int		*M_usableSpikeIdxOfNn1;
	__int64	m_SIGi;
	__int64	*M_usableSIGi;
	int		*M_firstIdxOfWavelet;
	int		*M_usableWaveIdx;
	int		*M_spikeWaveIdxOfNn1;
	__int64	*M_spikeTimeStampOfNn1;
	int		*M_Spike_QueueSize;

	int		m_AlignmentEcode;
	double m_prvEyeX, m_prvEyeY, *m_pEyeX, *m_pEyeY; //The data eye position
	__int64 m_prvXYpos_DispTimeMs;
	CRASView *pView;
	CFigure *pFig;
	int		m_isValidFig;

	//###############################  Device context to delete at the end of the program  #################################
	//static CDC m_ChDC;
	//static CBitmap m_ChBmp;
	static CPen	m_VIOLETLIGHT2;
	static int m_DiviceContaxtCreatedAlready;
	//################################   SDF (just for verification) #######################################################
	void	ContinuousSDF();
	int		m_samplesPerMs;
	double	m_y,m_lastSDFY;
	int		m_x,m_lastSDFX;
	double	m_MinRunningSDFY;
	int		m_MaxRunningSDFX;

	__int64 m_TimeOfMinX_ms,m_SDFbeginTime_us;
	CArray<float, float& >m_SDF; 
	float	m_fSigma,m_GaussianAmp, m_RangeX_ms;
	float	KrVal[1000];
	int		P[1000],NumP;
	void	CreateKer(float sig, float RangeX);
	void	ConvolutionWhole(int initI, int endI);
	void	ConvolutionFragmented(int initI, int endI);
	int		idxSpkQ(int i);
	int		DispSDF(int initI, int endI, __int64 initItime, CClientDC *pObjDC);
	//######################################################################################################################

	CAiChannel();
	virtual ~CAiChannel();
};



#endif 