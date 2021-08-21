// XCOR_View.h : interface of the CXCOR_View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCORVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_XCORVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

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
inline int BiggerOne(int a, int b ){if(a>b)return a; else return b; }
inline __int64 BiggerOne_int64(__int64 a, __int64 b ){if(a>b)return a; else return b; }
#define RemoveForwardSlashAtEnd(Dir) if(Dir[strlen(Dir) - 1] == '\\') Dir[strlen(Dir) - 1] = '\0'; //Remove the forward slash.
#define AddFrwdSlashAtEndIfNotHave(Dir)    if(Dir[strlen(Dir) - 1] != '\\') strcat(Dir,"\\"); //Add the forward slash.

#define WM_MY_MESSAGE (WM_USER + 1)
#define SimDurationx10    1
#define MsgPosX(pB, relX) (pB->m_dMinX+(relX)*(pB->m_dSpanX))
#define MsgPosY(pB, relY) (pB->m_dMinY+(relY)*(pB->m_dSpanY))

struct	RowClm
{
	int row,clm,depth;
};

class CFigure;
class CTrial;
class CXCOR_View : public CView
{
protected: // create from serialization only
	CXCOR_View();
	DECLARE_DYNCREATE(CXCOR_View)
//-------------------------------------------
public:
	//###########################################################################################################################
	CArray<double, double& >m_Raster[HOW_MANY_TRLS_TO_DISP]; 
	int		m_SizeOfRaster[HOW_MANY_TRLS_TO_DISP], m_NumSpikesInRow[HOW_MANY_TRLS_TO_DISP], m_RasterIdx, m_NumOfDisplayedRasterRows, m_AutoCorrelation;
	CArray<double, double& >m_XCOR, m_TmpXCOR; 
	int		m_x, m_XCOR_RefSpkIdx, m_XCOR_SbjSpk_StartIdx, m_XCOR_SbjSpk_EndIdx, m_SbjSpkIdx, m_XCOR_RefChan, m_XCOR_SbjChan, m_XCOR_RefNn, m_XCOR_SbjNn, m_NumWeightings, m_UserReqest_ChangeXOR_Ker;
	int		m_PreSpikeTimeWnd, m_PostSpikeTimeWnd, *m_pUsableSpikeIdx_RefNn, *m_pUsableSpikeIdx_SubNn, m_FullSizeXCOR_ms, m_XcorSize_ofPreRefSpike, m_XcorSize_ofPostRefSpike, m_XcorSize_ofPreRefSpike_inMicS, m_XcorSize_ofPostRefSpike_inMicS;
	__int64	*m_pSpikeTimeStampOf_RefNn, *m_pSpikeTimeStampOf_SubNn, m_StartTimeWindow_MicS_SbjSpk, m_EndTimeWindow_MicS_SbjSpk;
	int		m_FoundStart, m_FoundEnd, m_Ref_AndOr_SbjChnaged;
	__int64* AddressOf_SpkIndexOfNn(int ChanID, int NnID);
	int		idxSpkQ(int i);
	void	Set_XCOR_range_and_Kernel();
	void	FindFirstIdxOfSbjSpike();
	void	Reset_Ref_Sbj_IDXes();

	//#####################  Read RST  ######################################################################################################
	int		Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany); 
	int		Convert_6digitDate_ToNumber(char *DateString);
	int		m_FileYearMonthDate;
	char	m_FolderPathRST[1024];
	//#######################################################################################################################################
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	//#######################################################################################################################################
	int		m_PrvSpkTimeOfNn1MAP_ReMapped[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	int		m_PrvSpkTimeOfNn2MAP_ReMapped[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	int		m_PrvSpkTimeOfNn3MAP_ReMapped[MAX_TRLS_IN_MAP][NUM_NN_AI_CHAN];
	int		m_PrvEvent_ReMapped[MAX_TRLS_IN_MAP];
	int		m_NumTrialsInMap_InThisProgram,FirstTimeMsg, m_IsFirstTimeRasterPlot;
	int		m_ProcessedUsableTrialIdxInMem, m_usableTrialIdxInMem;
	int		m_Stop_Or_Pause;
	int		Find_Last_Num_inString(char *GivenString, int &NumToReturn);
	void	RedrawAllContent();
	void	Draw_OneFig_andContent(int row, int clm) ;
	void	Draw_OneFig_andContent(CFigure *pFig);
	void	SetECodeConditions_Dialog();
	int		m_AlignmentEcode;

	//Gaussian Kernel
	float	m_fSigma,m_GaussianAmp, m_HalfKernel_ms, m_FullKernel_ms;
	float	KrVal[1000];
	int		P[1000],NumP;
	void	CreateKer(float sig, float RangeX);
	//#######################################################################################################################################
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int		MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	//######################  Display loop  #################################################################################################
	#define	MAX_DISP_REQUESTS 10
	int		m_DispEvents[MAX_DISP_REQUESTS][4],m_DispEventIdx,  m_isDisplayerRunning;
	void	DisplayStuff(int RequestIdx, CDC *pObjDC);
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1, int SomeIntValue2);
	void	RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) ;
	void	DispForResizing(CDC *pObjDC);
	void	DispForRescalingAxes(CDC *pObjDC);
	//#############################################  Syncronization #############################################
	CSemaphore m_sema;

	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################################
	int	m_SelectedItemID;
	int	m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//#######################################################################################################################################
	//#####################  File  ##########################################################################################################
	CString	GetExecutableName();
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	int FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[]);
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	char m_FileName[1024],m_JustFileName[1024],m_FolderPath[1024],m_FileNameWithoutEXT[1024], m_JustFolderName[1024];
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int GetFile_WholePath_WithInitialFolder_Name(BOOL IsToREAD, char *TitleOfDialog, char *initialFolder, char *initialFileName);
	int		wildcmp(char *wild, char *string);
	int	GetFolderName(char* FolderPath, char *FolderName);
	//#######################################################################################################################################
	//#####################  Statistics  ####################################################################################################
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

	//#####################  Divice Context stuff to delete later //#########################################################################
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
	CBrush m_bkBrush;
#define MaxPen 4
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NULL, m_RED, m_BLUE, m_GREEN,m_DkGREEN,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_White2, m_ORANGE2, m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;

	//############################  Basic Stuff #############################################################################################
#define MaxRGB100 100
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown,m_Black;
	int		m_fontHeight;
	int m_nScrMaxX, m_nScrMaxY;
	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime;
	BOOL	 m_bShow2D;
	int      m_nKey; //
	int      m_nSpeed; //
	int		 m_nColorShow,m_SavedDC;
	int		m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int		m_BlownUpPaneID;
	int m_StepByStep;
	double	m_dScaleUpDn;
	BOOL m_IsToDisplayMarks;
	CFigure m_TwoD1,m_TwoD2,m_TwoD3,m_TwoD4, m_TwoD5, m_TwoD6, m_TwoD7, m_TwoD8, m_TwoD9, m_TwoD10;
	CFigure m_TwoD11, m_TwoD12, m_TwoD13,m_TwoD14,m_TwoD15,m_TwoD16,m_TwoD17,m_TwoD18,m_TwoD19;
	#define MAX_GRAPHS 100
	CFigure m_3D1,m_3D2,m_3D3, m_TwoD101,m_TwoD102,m_TwoD103, m_TwoD104;
	#define MaxFigRow 10
	#define MaxFigCln 10
	#define MaxFigLyr 1
	CFigure m_Fig[MaxFigRow][MaxFigCln][MaxFigLyr];
	CString m_GlobalTitle;
	CRect	m_TitleArea, m_dTitleRect;
	void	DisplayTitle();
	int		m_topOfFigs,m_DoNotErase;
	int		m_NewlyLoaded;
	void	VirMemDC();
	void	Batch_DrawFigureFrames(int isToUnveil);
	void	Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) ;
	void	Init_Fig_Positions();
	BOOL	FlagVerScreen;
	BOOL	m_bIsScrXYforIJ;
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
	CRect	m_savedClientRect;
	void	Re_measure_ClientView();
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	void	Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) ;
	void	Batch_SetFontSize(CDC* pDC);
	void	Start_a_NEW_set_of_figures();
	void	Batch_Prepare4FigFrame();
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	void	AdjustScale(double &minY, double &maxY);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	ColorLegend(CDC *pDC, CFigure *pFig);
	void	FromScrnToDataCoordinate(int x, int y, float &DataX, float &DataY, CFigure *pFig);
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	Show2D_I();
	void	XCOR_Main();
	void	Show2D_III();
	void	Show2D_IV();
	void	Show2D_VI();
	void	Show2D_VII();
	void	Show2D_VIII();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	void	WhichFigure(CPoint point);
	void	Figs_Captured();
	void	Hilight_AllCaptured_Figs();
	void	DrawOneFigure(CFigure *pFig, int isToHighlight);
	void	EliminateExcluded();
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCapturedFig(CFigure *pFig);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;
	void	Set_Ref_and_Sbj_Indexes();
	int		m_IsEditOkayed;

//-------------------------------------------

// Attributes
public:
	CXCORDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCOR_View)
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
	virtual ~CXCOR_View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CXCOR_View)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void On_Start_AFRESH();
	afx_msg void OnRasterPlot();
	afx_msg void OnStop_or_Pause();
	afx_msg void OnChangeParameters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
public:
	afx_msg void OnIsToDisplayMarks();
public:
	afx_msg void OnUpdateIsToDisplayMarks(CCmdUI *pCmdUI);
	afx_msg void CheckRasterPlot(CCmdUI *pCmdUI);
	afx_msg void Check_Stop_or_Pause(CCmdUI *pCmdUI);
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
};

#ifndef _DEBUG  // debug version in XCORView.cpp
inline CXCORDoc* CXCOR_View::GetDocument()
   { return (CXCORDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCORVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
