// AnalysisView.h : interface of the CAnalysisView class
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

#define RemoveForwardSlashAtEnd(Dir) if(Dir[strlen(Dir) - 1] == '\\') Dir[strlen(Dir) - 1] = '\0'; //Remove the forward slash.
#define AddFrwdSlashAtEndIfNotHave(Dir)    if(Dir[strlen(Dir) - 1] != '\\') strcat(Dir,"\\"); //Add the forward slash.

class CTrial;
class CFigure;
class CAnalysisView : public CView
{
protected: // create from serialization only
	CAnalysisView();
	DECLARE_DYNCREATE(CAnalysisView)
//-------------------------------------------
public:
	//#####################  Application starts from HERE  ##################################################################################
	CTypedPtrList<CObList, CTrial*> m_TrialList;
	CTrial* m_pTrial[3000];
	int		m_IsFirstCallByIconPress, m_IsEditOkayed;
	int m_NumTrialsInList, m_RequestedTrlToDisp;
	int		m_CurrNnChannelID, m_CurrNnID, m_FileYearMonthDate;
	int		Convert_6digitDate_ToNumber(char *DateString);
	int		Copy_fromNth_HowMany(char *Dest, char *Source, int N1, int HowMany);
	int		Find_first_6Digit_unintrupted_number(char *SixDigitString, char *NameString) ;
	void	ChannelID();
	void	NnID();
	//##########################  SDF  ########################################################################################################
	#define GAUSS_SUM 1000
	CArray<double, double& >m_SDF, m_Compensation; 
	__int64 m_initTime;
	int		m_SDFArraySize, m_SDFArraySizeUsed;
	double m_GaussianAmp, m_fSigma, m_halfGaussKerMs, m_fullGaussianMs;
	float	m_KrVal[20000];
	int		m_P[20000],NumP;
	void	CreateKer(float sig, float RangeX);
	void	ComputeSDF(int SpkTime[], int SizeOfSpkArray, int TimeToSubtract);
	void InitSDF(int SizeOfSDF);
	//#######################################################################################################################################
    // thread handling
    HANDLE	m_hThread,  m_hEventBasedThread,m_hEvStopMainThread, m_EvDisplayRequest;
    HANDLE	m_hEvKeyPressed, m_hToNextStep;
	HANDLE	m_Events[MAXIMUM_WAIT_OBJECTS]; 
	int		m_RequestIdx;
	int		m_DispEvents[MAXIMUM_WAIT_OBJECTS][4], m_DispEventIdx;
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue); 
	void	RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2); 
	void	RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2); 
	void	DisplayRoutine_for_requested_jobs();
	//#######################################################################################################################################
	void CAnalysisView::DrawArrow(double rx1, double ry1, double rx2, double ry2, BOOL Is_Arrow, CFigure *pBd0, CClientDC *pObjDC, CDC *pDC);
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int	MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################
	int	m_SelectedItemID;
	int	m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//#######################################################################################################################################
	//#####################  File  #########################################################################################
	void FromA_findB_addC_giveD(char FromA[], char *findB, char addC[], char giveD[]);
	void FromExePath_findA_addB_giveC(char *A, char addB[], char giveC[]);
	int	GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	CString GetExecutableName();
	char	m_FileName[500],m_FileNameWithoutEXT[500],m_FolderPath[500],m_JustFileName[100],m_JustFolderName[100],m_GrandParentFolderName[500]; 
	int	GetFile_WholePath_Name_CurPath(BOOL IsToREAD);
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int	GetFolderName(char* FolderPath, char *FolderName);
	int		wildcmp(char *wild, char *string);
	//#######################################################################################################################################
	//#####################  IPC  #########################################################################################
	HANDLE hFile;
	int *hView, *aux; 
	void CreateSharedMemoryArea(void);
	void OpenSharedMemory(void);
	void DestroySharedMemoryArea (void);
	void WriteOnSharedMemory(int data, int position);
	void ReadFromSharedMemory(int *data, int position);
	//#######################################################################################################################################
	//#####################  Statistics  #########################################################################################
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
	double	box_muller(double m, double s);

	//#####################  Divice Context stuff to delete later //#####################
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
#define MaxBrush 10
	CBrush	m_Gray[MaxBrush];
	CBrush m_bkBrush;
#define MaxPen 4
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NULL, m_RED, m_BLUE, m_GREEN,m_DkGREEN,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_ORANGE2;
	CPen	m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;
	//###################################################################################

	//###################################################################################
	//OpenGL
	HDC     m_hDC;
	HGLRC   m_hRC;
	void	DrawGraphics(double z);
	void	ResizeGraphics();
	void	InitGraphics();
	void	SetupPixelFormat();

	//###################################################################################
	CRITICAL_SECTION 	m_CriticalSection;
	int		m_fontHeight;
	void	DisplayTitle();
	#define MaxRGB100 100
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
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
	BOOL m_bIsToggleON;
	CRect	 m_rectShow3D;
	CFigure m_TwoD1,m_TwoD2,m_TwoD3,m_TwoD4, m_TwoD5, m_TwoD6, m_TwoD7, m_TwoD8, m_TwoD9, m_TwoD10;
	CFigure m_TwoD11, m_TwoD12, m_TwoD13,m_TwoD14,m_TwoD15,m_TwoD16,m_TwoD17,m_TwoD18,m_TwoD19;
	#define MAX_GRAPHS 100
	CFigure m_3D1,m_3D2,m_3D3, m_TwoD101,m_TwoD102,m_TwoD103, m_TwoD104,m_Board[MAX_GRAPHS];
	#define MaxFigRow 10
	#define MaxFigCln 10
	#define MaxFigLyr 3
	CFigure m_Fig[MaxFigRow][MaxFigCln][MaxFigLyr];
	CString m_GlobalTitle;
	CRect	m_TitleArea, m_dTitleRect;
	int		m_topOfFigs,m_DoNotErase;
	int		m_NewlyLoaded;
	int		m_SelecFig_row,m_SelecFig_clm,m_SelecFig_dpt;
	void	VirMemDC();
	void	Batch_DrawFigureFrames();
	void	Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) ;
	void	Init_Fig_Positions();
	BOOL	FlagVerScreen;
	BOOL	m_bProceed,m_bIsScrXYforIJ;
	/////////////// For the Dialogue Box
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY, m_dMinimumY2, m_dMaximumY2;
	clock_t m_start,m_finish;
	//////////////////////////////////////////////////////////
	CRect	m_savedClientRect;
	void	Re_measure_ClientView();
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	void	Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) ;
	void	Batch_SetFontSize(CDC* pDC);
	void	Start_a_NEW_set_of_figures();
	void	Batch_Prepare4FigFrame();
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	void	Manual_Fig_Labels_n_MinMaxXY();
	double	DifferenceToRound(double &NumToAdjust, double VariableRange);
	void	AdjustScale(double &minY, double &maxY);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	LinearColorMix(COLORREF RGB_array[],int arraySize);
	void	LinearColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	ColorLegend(CDC *pDC, CFigure *pFig);
	void	FromScrnToDataCoordinate(int x, int y, float &DataX, float &DataY, CFigure *pFig);
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	Show2D_I();
	void	Show2D_II();
	void	Show2D_III();
	void	Show2D_IV();
	void	Show2D_V();
	void	Show2D_VI();
	void	Show2D_VII();
	void	Show2D_VIII();
	void	Show3D();
	void	CommentI(CDC* pDC);
	void	CommentII(CDC* pDC);
	void	CommentIII(CDC* pDC);
	void	CommentIV(CDC* pDC);
	void	CommentV(CDC* pDC);
	void	OnMyMessage();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	void	WhichFigure(CPoint point);
	void	Figs_Captured();
	void	Hilight_AllCaptured_Figs();
	int		Find_Last_Num_inString(char *GivenString);
	int		Find_Last_Num_inString(char *GivenString, int &NumToReturn);
	void	EliminateExcluded();
	void	DrawOneFigure(CFigure *pFig, int isToHighlight);
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCaptured(CFigure *pFig);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;
	void	RefreshEverything();
	inline void Randomize(){
		time_t seconds;
		time(&seconds);
		srand((unsigned int) seconds);
	}
//-------------------------------------------

// Attributes
public:
	CAnalysisDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalysisView)
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
	virtual ~CAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAnalysisView)
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
	afx_msg void OnKernelSigma();
	afx_msg void OnKernelSigmaSmaller();
};

#ifndef _DEBUG  // debug version in AnalysisView.cpp
inline CAnalysisDoc* CAnalysisView::GetDocument()
   { return (CAnalysisDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
