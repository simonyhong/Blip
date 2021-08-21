#if !defined (RUNNING_LINE_CLASS)
#define RUNNING_LINE_CLASS

struct CUserPickedVarToDisp
{
public:
	int		m_TypeOfVar;
	int		m_DispPosition;
	int		m_ColorOrder;
	char	m_VarInChar[64];
	BOOL	m_Front0_Or_Back1_axis;
	double	*m_pValue;
	double	*m_pAddThis;
	double	*m_pMultiplyThis;
	double	m_PrvVarValue;
	int		m_DispByRunningLine;
	CPen	*m_pPen; 
};

class CMOXY_View;
class CRunningLineDisplay: public CObject
{
public:
	int		m_RL_FigID;
	int		m_RecordingSourceChanID;
	int		m_IsToSuperImpose;
	void	DispTriggerStatus();
	void	DispSupperimposeStatus();
	void	DispRecordingSource_Chan_Nn();
	CString	m_sTriggerEcode;
	int		m_RL_RecordingSourceNn;

	//###### Basic stuff  ########
	int		m_preWaveSpanDataSize,m_postWaveSpanDataSize;
	void	DisplayUserPickedVariables();
	////////////////////
	double *M_AI;
	__int64	*M_timeStampMicS;//Pointer to the time array
	__int64	*M_LatestTimeStampInMicroS;//The latest AI time stamp
	__int64	m_AiIdx;
	__int64	*M_usableAiIdx;
	__int64	*M_AIQueueSize;
	int		*M_usableSpikeIdxOfNn_RL;
	__int64	m_SIGi;
	__int64	*M_usableSIGi;
	int		*M_firstIdxOfWavelet;
	int		*M_usableWaveIdx;
	__int64	*M_spikeTimeStampOfNn_RL;
	__int64	*M_spike_IdxOfAIOfNn_RL;
	int		*M_Spike_QueueSize;
		int *M_IsTriggeredMode;
		int *M_nTriggerECode;
		int *M_hasTriggered;
	__int64 *M_TriggeredTime_ms;
	int		*M_NumUserPickedVariables;

	CUserPickedVarToDisp m_UserPickedVar[MAX_NUM_USER_PICKED_VARS];
	int		m_numUserPickedVariables;
	double m_prvEyeX, m_prvEyeY, *m_pEyeX, *m_pEyeY, m_PrvRLtime; //The data eye position
	__int64 m_prvXYpos_DispTimeMs;
	CMOXY_View *pView;
	double CRunningLineDisplay::FindIntersactionX_between_One_line_and_AnotherWithTwoPoints(double yEqualsTo, double X1, double Y1, double X2, double Y2);

	//###############################  Device context to delete at the end of the program  #################################
	CDC		*m_pDC;
	static CPen	m_VIOLETLIGHT2;
	static CPen m_rgbPEN1[MAX_NUM_PENS_FOR_RUNNING_LINE],m_rgbPEN2[MAX_NUM_PENS_FOR_RUNNING_LINE],m_rgbPEN3[MAX_NUM_PENS_FOR_RUNNING_LINE], m_White3Pen;
	static int	m_PensHaveBeenCreated,m_PenIDs_alternating[MAX_NUM_PENS_FOR_RUNNING_LINE];
	//#######################################   RunningLine Dispaly  #######################################################
	void	PensForChannel();
	#define MaxRGB100 100
	COLORREF m_RGB100[MaxRGB100];
	void	RunningLineSDF();
	void	Redraw_RunningLineFig();
	int		m_samplesPerMs;
	double	m_y,m_lastSDFY;
	double	m_x,m_lastSDFX;
	CFigure *m_pRLineFig;
	int		m_MaxRunningSDFX, m_DispEyeTraceAtEveryXms;
	int		m_FirstDisp, m_SamplesPerMs;
	__int64 m_TimeOfMinX_ms,m_SDFbeginTime_us;
	CArray<float, float& >m_SDF; 
	CPen	*m_pPenForSDF;
	float	m_fSigma,m_GaussianAmp, m_halfGaussKerMs;
	float	KrVal[1000];
	int		P[1000],NumP;
	void	CreateKer(float sig, float RangeX);
	void	Init_RunningLineSDF_FIG();
	int		idxSpkQ(int i);
	int		idxSDF(int i);
	void	DispSDF(int &SDFi, int endI, CClientDC *pObjDC);
	void	Register_a_variable_to_display(int UserPickedVarID);
	//######################################################################################################################

	CRunningLineDisplay();
	virtual ~CRunningLineDisplay();
};



#endif 