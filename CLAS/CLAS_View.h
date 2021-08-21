// CLAS_View.h : interface of the CCLAS_View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_CLASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

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

class CAiChannel: public CObject
{
public:
	double *M_AI;
	__int64	*M_usableAiIdx;
	int		*M_firstIdxOfWavelet;
	int		*M_WvEcode;
	int		*M_lengthOfSignal;
	double	*M_AdjustXToXthreshold;

	int	*M_DataPointsPerWave;

	CAiChannel(){};
	virtual ~CAiChannel(){};
};

class CFigure;
class CCLAS_View : public CView
{
protected: // create from serialization only
	CCLAS_View();
	DECLARE_DYNCREATE(CCLAS_View)
//-------------------------------------------
public:
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	
	CAiChannel AiChan[MAX_AI_CHAN];
	
	//#######################################################################################################################################
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int	MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	
	//######  All about display
	#define	MAX_DISP_REQUESTS 10
	int		m_DispEvents[MAX_DISP_REQUESTS][3],m_DispEventIdx;
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1);
	void	RequestDisplayEvent(int Event,  int SomeIntValue1, int SomeIntValue2);
	void	DisplayStuff(int DispEvent, CFigure *pFig);
	void	DisplayStuff(int RequestIdx, CDC *pObjDC);
	CFigure *m_pFig;
	__int64  m_LastTimeDisplayed;
	int		m_NumSelectedWaveformsDisplayed[NUM_NN_AI_CHAN],m_PrvNumSelectedWaveformsDisplayed[NUM_NN_AI_CHAN];
	int		m_NumNotSelectedWaveformsDisplayed[NUM_NN_AI_CHAN];
	void	DrawWave(int WvID, int ChanID);
	void	DispOneWaveform_FragmentedWave(int WaveID, int ChanID);
	void	RefreshFrame();

	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################
	int	m_SelectedItemID;
	int	m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//#######################################################################################################################################
	//#####################  File  #########################################################################################
	CString	GetExecutableName();
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);	char m_FileName[1024],m_JustFileName[1024],m_FolderPath[1024],m_FileNameWithoutEXT[1024];
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		wildcmp(char *wild, char *string);
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
	int		Select_Action(float *pi, int N);
	double	box_muller(double m, double s);

	//##########################  Divice Context stuff to delete later //##########################
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
	CBrush	m_GrayBrush,m_RedBrush;
	CBrush m_bkBrush;
#define MaxPen 4
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NullPen, m_RED, m_BLUE,m_GREEN, m_GREEN2,m_DkGREEN,m_CYAN1,m_CYAN2, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_ORANGE2, m_VIOLET2,m_VIOLET3,m_VIOLETLIGHT2,m_VIOLETLIGHT1,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY1,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;
	//#############################################################################################

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
	CFigure m_TwoD1,m_TwoD2,m_TwoD3,m_TwoD4, m_TwoD5, m_TwoD6, m_TwoD7, m_TwoD8, m_TwoD9, m_TwoD10;
	CFigure m_TwoD11, m_TwoD12, m_TwoD13,m_TwoD14,m_TwoD15,m_TwoD16,m_TwoD17,m_TwoD18,m_TwoD19;
	#define MAX_GRAPHS 100
	CFigure m_TwoD101,m_TwoD102,m_TwoD103, m_TwoD104,m_Board[MAX_GRAPHS];
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
	BOOL	m_bIsScrXYforIJ;
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
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
	void	EliminateExcluded();
	void	DrawOneFigure(CFigure *pFig, int isToHighlight);
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCaptured(CFigure *pFig);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;

//-------------------------------------------

// Attributes
public:
	CCLAS_Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLAS_View)
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
	virtual ~CCLAS_View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCLAS_View)
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
};

#ifndef _DEBUG  // debug version in CLAS_View.cpp
inline CCLAS_Doc* CCLAS_View::GetDocument()
   { return (CCLAS_Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
