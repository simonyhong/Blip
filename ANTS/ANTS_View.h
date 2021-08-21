// AntsView.h : interface of the CAntsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//###############################################################################################################################
//-----------------------------  Constants --------------------------------------
#define WHOLE_NET     0
#define NO_CONNECTION 1
//#define GRAPH_3D_STOP	0
//#define SIMULATION	1
//#define GRAPH_2D		2
//#define GRAPH_3D		3
//----------------------------------------   My_Math.h --------------------------------------------------
#define RAND_RATE       ( (double)rand()/(double)RAND_MAX )
#define PROBABILITY(P)  (RAND_RATE<(double)P)
#define Random(N)       (rand()%(N))  // N should be an integer.

#define Sq2D(x,y)   ((x)*(x)+(y)*(y))        //Distance from the original to the given vector A
#define Sq3D(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))//Distance from the original to the given vector A
#define M3D(x,y,z)  sqrt((x)*(x)+(y)*(y)+(z)*(z))
#define M2D(x,y)    sqrt(((x)*(x)+(y)*(y)))
#define SQ_DISTANCE_AB_1D(x1,  x2)          ( ((x1)-(x2))*((x1)-(x2))  )
#define DISTANCE_AB_1D(x1,  x2)          sqrt( SQ_DISTANCE_AB_1D(x1,  x2) )
#define SQ_DISTANCE_AB(x1,y1,  x2,y2)          ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) )
#define DISTANCE_AB(x1,y1,  x2,y2) ( sqrt(           SQ_DISTANCE_AB(x1,y1,  x2,y2)                )  )
#define DISTANCE_OA(x1,y1) ( sqrt(   Sq2D(x1,y1)    )  )
#define	SQ_DISTANCE_AB_2D(x1,y1,  x2,y2)          ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) )
inline double CrossProduct(int a1, int a2, int b1, int b2 ){return a1*b2 - b1*a2; }
inline double CrossProduct(float a1, float a2, int b1, int b2 ){return a1*b2 - b1*a2; }
inline double CrossProduct(double a1, double a2, double b1, double b2 ){return a1*b2 - b1*a2; }
inline double DotProduct( int a1, int a2, int b1, int b2 ){return a1*b1 + a2*b2; }
inline double DotProduct( double a1, double a2, double b1, double b2 ){return a1*b1 + a2*b2; }
inline double DotProduct( float a1, float a2, float b1, float b2 ){return a1*b1 + a2*b2; }
inline double MAXab(double a, double b ){if(a>b)return a; else return b; }
inline double MINab(double a, double b ){if(a<b)return a; else return b; }

#define WM_MY_MESSAGE (WM_USER + 1)
#define SimDurationx10    1
#define MsgPosX(pB, relX) (pB->m_dMinX+(relX)*(pB->m_dMaxX-pB->m_dMinX))
#define MsgPosY(pB, relY) (pB->m_dMinY+(relY)*(pB->m_dMaxY-pB->m_dMinY))
#define ChrPosX(ID,relX) (m_Board[ID].m_dMinX+(relX)*(m_Board[ID].m_dMaxX-m_Board[ID].m_dMinX))
#define ChrPosY(ID,relY) (m_Board[ID].m_dMinY+(relY)*(m_Board[ID].m_dMaxY-m_Board[ID].m_dMinY))
#define ContinuousArry 0
#define SplitArry 1

#define MaxFigRow 4
#define MaxFigCln 4
#define MaxFigLyr MAX_NUM_STIM_CHANNELS


class CFigure;
class CAntsView : public CView
{
protected: // create from serialization only
	CAntsView();
	DECLARE_DYNCREATE(CAntsView)
//-------------------------------------------
public:
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	HANDLE	m_M_DO_Active[MAX_DO_NUM];
	//#######################################################################################################################################
	CAiChannel AiChan;
	double	*m_pM_AI;
	__int64	*pM_AIQueueSize;
	//#######################################################################################################################################
	HANDLE m_EvStopProcess;
	HANDLE m_hThread;
	//#####################  Application starts from HERE  ##################################################################################
	int		m_NumOfStimulationChannels, m_StimulationChannels[MAX_NUM_STIM_CHANNELS], m_SavedPostStimPeriod[MAX_NUM_STIM_CHANNELS], m_PostStimPeriod[MAX_NUM_STIM_CHANNELS], m_IsFasterStm, m_DisplayedStimLayer;
	int		m_numRecording_Stim_PairsToSave; CFigure *m_pFigForSave[MaxFigRow*MaxFigCln*MaxFigLyr];
	ClickableLabel m_LayerTab[MAX_NUM_STIM_CHANNELS], m_Lable_Of_SuperimposeNumber, m_Lable_Of_TriggerChanNumber;
	int		m_numberOfSuperimposeLines;
	void	Draw_Disp_Tabs_and_DispStimLayer(int StimLayer, int toDisplayTabs, int toDisplayLayer);
	void	Draw_Disp_SuperimposeNumber(int layer, int IstoDisplayNow);
	void	Draw_TriggerChanNumber(int layer, int IstoDisplayNow);
	void	Draw_Tabs(int layer, int IstoDisplayNow);
	void	Minimum_RequiredTime_toTrigger(int layer, int IstoDisplayNow);
	void	DrawGlobalTitle(int layer, int IstoDisplayNow);
	void	TriggerChannelName_Register(int TrigChan);
	void	BatchDraw_LayerContents(int IsToDisplayNow);
	void	Meassure_SuperimposeNumber();
	void	Draw_Text_OfMeasuringLine(CFigure* pFig, int LineID, int X);
	void	Update_and_Draw_MeasuringLine(CFigure* pFig, int X);
	void	Update_and_Draw_MeasuringLine(CFigure* pFig);
	void	Meassure_TriggerChanNumber();
	int		IsThereAnyFigureHavingThisStimChannel(int Chan);
	void	AssessConfiguration();
	int		DoesTriggeringChannel_HaveThisStimChannel(int Chan);
	int		IsIt_a_ValidStimChannel_Registered(int ChID);
	void	ConfigureLayerButtons();
	void	DisplayStuff(int RequestIdx, CDC *pObjDC);
	int		m_HeldLineIdx;
	int		m_StimulationLatencyBias_InMicroS;
	int		m_EnteredChanID;
	void	Recalibrate_Panes_and_MeasuringLineRectPositions();
	void	Init_Stim_and_Recording_Channles();
	void	DisplayLFP_AroundStimulation(int StimChan, CDC *pObjDC);
	void	Set_StimLatency_and_PostStimPeriod();
	int		qIdx(int i);
	void	UpdateScales();
#define	MAX_DISP_REQUESTS 100
	int		m_DispEvents[MAX_DISP_REQUESTS][4],m_DispEventIdx;
	void	DrawAllMeasuringLines_NoDisplayNow(CFigure *pFig);
	void	InitMeasuringLineParameters_and_DrawLines();
	void	Make_TheBaseOfFileName(int IsToSave);
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1, int SomeIntValue2);
	void	RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2); 
	void	EditPrefixSuffix();
	void	MakeRecordStimChanNameString(CFigure* pFig);
	long long m_OrthoStimIdx;
	void	Relay_ON(int whichBIT);
	void	Relay_OFF(int whichBIT);
	void	UnHilight_OneCapturedFig(CFigure *pFig);
	void	AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(int StimChanDOIdx);
	int		GetIndexOfSimChannel(int StimDO);
	void	RectOfLabel_inTitleArea(float relX, int shiftToRightBy, int LengthOfLabel, CRect &rect);
	void	RectOfLayerTabs(float left, int LengthOfLabel, CRect &rect);
	void	Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
	int		Dialog_Record_Stimulation_Channels();
	ClickableLabel m_FileName;
	int		ShortestDistanceFrom_A_to_B(int A, int B, int SizeOfLoopedQueue);
	CRITICAL_SECTION m_CriticalSection4Save;
	//################################# Zapper ##############################################################################################
	void	Disp_ZapStatus();
	void	Disp_LearnedLevel(int FigID);
	void	Zapper_MakeListOfGatingRelayIDs();
	void	Update_LearnedLevel(int RecordingChannel, int GatingRelayID, double LearnedLevel);
	CRect	m_ZapRect;
	
	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################################
	int		m_SelectedItemID;
	int		m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//#######################################################################################################################################
	//#####################  File  ##########################################################################################################
	CString m_prvDate,m_Date;
	CString	m_strFileName;
	int		m_is_LFP_saving,m_numOfSavedTrials,m_PrvNnNumberToRecord, m_LFP_Start_Xms_toSave, m_LFP_End_Xms_toSave, m_is_firstTimeOrthoStimulation_IntervalSetting, m_IsContinuousRecordingMode, m_FirstRecordingInSequence, m_LastIndexOfLFP;
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	char	m_ConfigJustFileName[256];
	char	m_FileNameFULL[1024],m_JustFileName[1024],m_FolderName[1024],m_FolderPathWithoutFileName[1024],m_FileNameWithoutEXT[1024], m_PreFix[1024],m_SuFFix[1024],m_ActivationOrCollision[1024], m_BaseOfFileName[1024];
	int		GetLastFolderName(char *FolderPath, char *FolderName);
	int		GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		wildcmp(char *wild, char *string);
	int		Find_Last_PositiveNum_inString(char *GivenString);
	int		Find_Last_Num_inString(char *GivenString, int &NumToReturn);
	void	Save_Antidromic_StimulationSignal(int CurrentStimulationChannel);
	void	Save_ORTHO_StimulationSignal(int CurrentStimulationChannel);
	int 	LocateAndCreateFolder_IfItIsNotAlreayThere();
	CString GetExecutableName();
	int 	CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FldrName[], CString Date);
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	void	RedrawFileName(int layer, int IstoDisplayNow);

	//#######################################################################################################################################
	//#####################  Statistics  ####################################################################################################
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int	MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	int		m_FirstTime0ElementErr;
	double	Mean(double *Array, int N);
	double	Mean(float *Array, int N);
	double	Mean(int *Array, int N);
	double	SD_GivenM(double *Array, int N,double mean);
	double	SD_GivenM(float *Array, int N,double mean);
	double	SD_GivenM(int *Array, int N,double mean);
	double	Mean_OnlyPositive(double *Array, int N);
	double	SD_GivenM_OnlyPositive(double *Array, int N,double mean);
	void	Gaussian_WhiteNoise_0to1(double noise[], int numSamples, double Mean, double Sig);
	double	GenHist_fromMinimum(double Data[],int dataSize, int Hist[], int binNum, double binWidth);
	int		Select_Action(float *pi, int N);
	double	box_muller(double m, double s);

	//#####################  Divice Context stuff to delete later //#####################
#define MaxDC MAX_NUM_STIM_CHANNELS
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
	CBrush	m_GrayBrush, m_RedBrush;
	CBrush m_bkBrush;
#define MaxPen 6
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NULL_pen, m_RED, m_BLUE, m_GREEN,m_DkGREEN,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_ORANGE2;
	CPen	m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;
	//###################################################################################
	int m_FontWidth, m_FontHeight;
	#define MaxRGB100 100
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	int m_nScrMaxX, m_nScrMaxY;
	int		m_fontHeight;
	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime, m_bFistTime2;
	BOOL	 m_bShow2D;
	int      m_nKey; //
	int      m_nSpeed; //
	int		 m_nColorShow,m_SavedDC;
	int		m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int		m_BlownUpPaneID;
	int m_isOrthodromicStimulation;
	double	m_dScaleUpDn;
	BOOL m_bIsTo_ANTI_Stimulate;
	CFigure m_Fig[MaxFigRow][MaxFigCln][MaxFigLyr];
	CString m_GlobalTitle;
	CRect	m_TitleArea, m_dTitleRect;
	int		m_topOfFigs,m_DoNotErase;
	int		m_NewlyLoaded;
	int		m_SelecFig_row,m_SelecFig_clm,m_SelecFig_dpt;
	void	VirMemDC();
	void	Batch_DrawFigureFrames(int toDisplay);
	void	Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) ;
	void	Init_Fig_Positions();
	BOOL	m_FlagVirMemDC;
	BOOL	m_bProceed,m_bIsScrXYforIJ;
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
	clock_t m_start,m_finish;
	//////////////////////////////////////////////////////////
	CRect	m_savedClientRect;
	void	Re_measure_ClientView();
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	void	Batch_FigPosition_Settings(double scale) ;
	void	Batch_SetFontSize(CDC* pDC);
	void	Start_a_NEW_set_of_figures();
	void	Batch_Prepare4FigFrame();
	int		m_dHowManyRows[MaxFigLyr], m_dSizeOfOneRow[MaxFigLyr], m_dHowManyClmns[MaxFigLyr], m_dSizeOfOneClmn[MaxFigLyr], m_dHowManyLayers;
	double	DifferenceToRound(double &NumToAdjust, double VariableRange);
	void	AdjustScale(double &minY, double &maxY);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	LinearColorMix(COLORREF RGB_array[],int arraySize);
	void	LinearColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	ColorLegend(CDC *pDC, CFigure *pFig);
	void	FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY, CFigure *pFig);
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	ANTS_main();
	void	Show2D_V();
	void	Show2D_VI();
	void	Show2D_VII();
	void	Show2D_VIII();
	void	Show3D();
	void	OnMyMessage();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	void	WhichFigure(CPoint point);
	void	Figs_Captured();
	void	Hilight_AllCaptured_Figs();
	void	EliminateExcluded();
	void	DrawOneFigure(CFigure *pFig, int isToHighlight);
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCaptured(CFigure *pFig);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;

//-------------------------------------------

// Attributes
public:
	CAntsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAntsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAntsView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void On2DGraph_IV();
	afx_msg void On2DGraph_V();
	afx_msg void On2DGraph_VI();
	afx_msg void On2DGraph_VII();
	afx_msg void On2DGraph_VIII();
	afx_msg void OnAutoStim();
	afx_msg void OnThresholdMode();
	afx_msg void On_VTBOX_mode();
	afx_msg void OnUpdateAutoStim(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateThresholdMode(CCmdUI *pCmdUI);
	//afx_msg void OnUpdate_VTBOX_mode(CCmdUI *pCmdUI);
	afx_msg void OnChangeParameters();
	afx_msg void OnFunctions();
	afx_msg void OnProceed();
	afx_msg void OnSpeedUp();
	afx_msg void OnSlowDown();
	afx_msg void OnQuickFunction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIs_OrthodromicStimulation();
	afx_msg void OnUpdateIsOrthodromicStimulation(CCmdUI *pCmdUI);
public:
	afx_msg void OnIs_AntidromicStimulation();
public:
	afx_msg void OnUpdateIsToAntidromicallyStimulate(CCmdUI *pCmdUI);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDeleteFigs();
	afx_msg void OnDelete_the_Rest_Figs();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnEnChangeEditString1();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnConfigFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnFileClose();
	afx_msg void OnProperties();
	afx_msg void OnFasterStm();
	afx_msg void OnUpdateFasterStm(CCmdUI *pCmdUI);
	afx_msg void OnFileContinuousRecordingMode();
	afx_msg void OnUpdateFileContinuousRecordingMode(CCmdUI *pCmdUI);
	afx_msg void OnSetPrePostPeriods_of_OrthodromicStimulationLFP_ToSave();
	afx_msg void OnSaveLFP();
	afx_msg void OnUpdateSaveLFP(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in AntsView.cpp
inline CAntsDoc* CAntsView::GetDocument()
   { return (CAntsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
