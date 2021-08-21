// DAS_View.h : interface of the CDasView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_DASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

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
#define MsgPosX(pB, relX) (pB->m_dMinX+(relX)*(pB->m_dSpanX))
#define MsgPosY(pB, relY) (pB->m_dMinY+(relY)*(pB->m_dSpanY))


class CDasDoc;
class CFigure;
class CDasView : public CView
{
protected: // create from serialization only
	CDasView();
	DECLARE_DYNCREATE(CDasView)
//-------------------------------------------
public:
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	void	Init_MemoryMapVariables();
	//#######################################################################################################################################
	CAiChannel m_AiChan[MAX_AI_CHAN];
	int		m_numAiChannels;
	int		m_NumAiChannels,m_ArraySize_RawData;
	unsigned char	m_prvDO_Array[MAX_DO_NUM];
	int	m_AntiDromicOutput;
	//################################   Simulated data acquisition  ########################################################################
	void	Simulated_Callback();
	__int64	newVal_simulated;

	//#######################################################################################################################################
	int		m_SaveIdx;
	void	Make_TheBaseOfFileName();
	void	RemeasureFileNameRect();
	void	EditPrefixSuffix();
	int		m_Hist[100];
	int		m_IsThresholdHeld;
	__int64  m_LastTimeDisplayed;
	int		m_NumSelectedWaveformsDisplayed[NUM_NN_AI_CHAN],m_PrvNumSelectedWaveformsDisplayed[NUM_NN_AI_CHAN];
	int		m_NumNotSelectedWaveformsDisplayed[NUM_NN_AI_CHAN];
	int		m_preWaveSpanDataSize, m_postWaveSpanDataSize;
	int		m_SelColor;
	int		qIdxSpike(int i);
	void	DispOneWaveform_FragmentedWave(int WaveID, int ChanID);
	void	DrawWave(int WvID, int ChanID);
	void	RefreshFrame();
	void	Remeasure_ThresholdPositions();

	//######  All about display
	#define	MAX_DISP_REQUESTS 10
	int		m_DispEvents[MAX_DISP_REQUESTS][4],m_DispEventIdx, m_DisplayAcquisionDisruption;
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1, int SomeIntValue2);
	void	RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) ;
	void	DisplayStuff(int RequestIdx, CDC *pObjDC);
	void	DispFileName();

	void	Figure_Channel_Config();
	void	DisconnectFigure_and_StopClientChan(int ChanID);
	void	StopClientChan_and_RemoveFigInfoInClientChan(int ChanID);
	void	UnplugFigureFromChan(CFigure *pFig);
	void	AttachFig_toChan(CFigure *pFig, int ChanID);
	void	LaunchServer();
	//#######################################################################################################################################
	TaskHandle	m_pTaskHandle_AI, m_pTaskHandle_DO;
	//#######################################################################################################################################
	//################   VT_DISCRIMINATOR_TASK  ################
	int		m_WaveIdxToDisp[NUM_NN_AI_CHAN];
	int		m_DataPointsPerWave,m_MaxNumWaveformsToDisp;
	__int64		m_EnteredChanID;
	//#####################  MOUSE  ################################################################################
	int		m_dontDoRubberbending,m_isInsidePane, m_IsValidOutput;
	void	DrawMeasuringLine(float relX, float relY, CFigure *pFig, CClientDC *pDC);
	int		m_paneTopX,m_paneTopY,m_paneBottomX, m_paneBottomY;
	int	m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//##############################################################################################################
	//#####################  File  #################################################################################
	CString m_prvDate,m_Date;
	CString	m_strFileName;
	int		m_isSaving,m_numOfSavedWaves,m_numOfSavedSelectedWaves, m_PrvNnNumberToRecord,m_numOf_ConsecutiveNonSelectedWaves_Saved;
	double	*pM_AI_save, *m_AdjustXToXthreshold_save;

	__int64	m_AIQueueSize_save;
	void	DesignateRecordingChannel(int RecordingChannel);
	int 	m_DataPointsPerWave_save, *m_FirstIdxOfWavelet;
	int 	LocateAndCreateFolder_IfItIsNotAlreayThere();
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	CString GetExecutableName();
	int 	CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FldrName[], CString Date);
	void	SaveWaves(int N);
	int		m_RecordingChanId, m_StopAfterSavingNwaves;
	char	m_ConfigJustFileName[256];
	char	m_FileNameFULL[1024],m_JustFileName[1024],m_FolderName[1024],m_FolderPathWithoutFileName[1024],m_FileNameWithoutEXT[1024], m_PreFix[1024],m_SuFFix[1024], m_BaseOfFileName[1024];
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	int		GetLastFolderName(char *FolderPath, char *FolderName);
	int		GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		wildcmp(char *wild, char *string);
	int		Find_Last_Num_inString(char *GivenString);
	//##############################################################################################################
	//#####################  Statistics  ###########################################################################
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

	//##########################  Divice Context stuff to delete later //##########################
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
	CBrush	m_GrayBrush,m_RedBrush;
	CBrush m_bkBrush;
#define MaxPen 4
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NULL, m_RED, m_BLUE,m_GREEN, m_GREEN2,m_DkGREEN,m_CYAN1,m_CYAN2, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_ORANGE2, m_VIOLET2,m_VIOLET3,m_VIOLETLIGHT2,m_VIOLETLIGHT1,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY1,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;
	//#############################################################################################

	//Other drawing related ones
	#define MaxRGB100 100
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	int		m_fontHeight,m_fontWidth;
	void	DisplayTitle();
	int m_nScrMaxX, m_nScrMaxY;
	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime;
	BOOL	 m_bShow2D;
	int      m_nKey; //
	int		 m_nColorShow,m_SavedDC;
	float	m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int m_StepByStep;
	double	m_dScaleUpDn;
	#define MaxFigRow 10
	#define MaxFigCln 10
	#define MaxFigLyr 1
	#define MAX_GRAPHS 100
	CFigure m_Fig[MaxFigRow][MaxFigCln][MaxFigLyr];
	CString m_GlobalTitle;
	CRect	m_TitleArea, m_dTitleRect;
	CRect	m_FileNameRect, m_prvFileNameRect;
	CRect	m_savedClientRect;
	CRect	m_CapturRec;
	int		m_numCaptured,m_KeyCode;
	int		m_topOfFigs,m_DoNotErase;
	void	VirMemDC();
	void	Batch_DrawFigureFrames();
	void	Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) ;
	void	Init_Fig_Positions();
	BOOL	m_FlagVirMemDC;
	BOOL	m_bProceed,m_bIsScrXYforIJ;
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
	clock_t m_start,m_finish;
	//////////////////////////////////////////////////////////
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int		MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	void	Re_measure_ClientView();
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	void	Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) ;
	void	Batch_SetFontSize(CFont *pFont);
	void	Start_a_NEW_set_of_figures();
	void	Batch_Prepare4FigFrame();
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	double	DifferenceToRound(double &NumToAdjust, double VariableRange);
	void	AdjustScale(double &minY, double &maxY);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	LinearColorMix(COLORREF RGB_array[],int arraySize);
	void	LinearColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	ColorLegend(CDC *pDC, CFigure *pFig);
	void	FromScrnToDataCoordinate(int x, int y, float &DataX, float &DataY, CFigure *pFig);
	void	FromScrnToDataCoordinate2(int y, double &DataY2, CFigure *pFig);
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	LineCrossingRect(int x1,double y1,int x2,double y2,float recLeft,float recTop,float recRight,float recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	RecalibratePanes();
	void	Show2D_I();
	void	Show2D_II();
	void	DAS_cl_MAIN();
	void	Show2D_IV();
	void	Show2D_V();
	void	Show2D_VI();
	void	Show2D_VII();
	void	Show3D();
	void	OnMyMessage();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	int		WhichFigure(CPoint point);
	void	Figs_Captured();
	void	Hilight_AllCaptured_Figs();
	void	EliminateExcluded();
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCaptured(CFigure *pFig);
//-------------------------------------------

// Attributes
public:
	CDasDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDasView)
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
	virtual ~CDasView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDasView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void On2DGraph_III();
	afx_msg void On2DGraph_II();
	afx_msg void On2DGraph_IV();
	afx_msg void On2DGraph_V();
	afx_msg void On2DGraph_VI();
	afx_msg void On2DGraph_VII();
	afx_msg void On2DGraph_VIII();
	afx_msg void On2DGraph_I();
	afx_msg void OnChangeParameters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIsFromHighToLow();
public:
	afx_msg void OnUpdateIsPD(CCmdUI *pCmdUI);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnSelect0();
public:
	afx_msg void OnUnselect1();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDeleteFigs();
	afx_msg void OnDelete_the_Rest_Figs();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnEnChangeEditString1();
	afx_msg void OnUpdateSelect0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUnselect1(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVtDelete();
	afx_msg void OnSelect1();
	afx_msg void OnSelect2();
	afx_msg void OnUpdateSelect1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSelect2(CCmdUI *pCmdUI);
	afx_msg void OnConfigFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnAdjustScaleToFitData();
	afx_msg void OnDisplayAcquisionDisruption();
	afx_msg void OnUpdateDisplayAcquisionDisruption(CCmdUI *pCmdUI);
	afx_msg void On_FlipSignal();
	afx_msg void On_UpdateViewFlipSignal(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in DAS_View.cpp
inline CDasDoc* CDasView::GetDocument()
   { return (CDasDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
