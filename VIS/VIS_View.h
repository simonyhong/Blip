// VIS_View.h : interface of the CVISView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

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

#include "SharedConstants.h"
#include "SharedConst_TASMOXYVIS.h"
class CFigure;
class CVISView : public CView
{
protected: // create from serialization only
	CVISView();
	DECLARE_DYNCREATE(CVISView)
//-------------------------------------------
public:
	int		m_topleftX_inPixel[MAX_STIM_OBJ],m_topleftY_inPixel[MAX_STIM_OBJ],m_width_inPixels[MAX_STIM_OBJ],m_height_inPixels[MAX_STIM_OBJ];
	clock_t	m_StartTime;
	unsigned  m_PrevTicks;
	void	WhichFunction(int messageID);
	void	Which_MOXY_Function();
	void	PvisAllOff_implement();
	void	PvisSetBackColor_implement();
	void	PvisDrawBar_implement();
	void	PvisDrawDisk_implement();
	void	PvisSwitchStim_implement();
	void	PvisDrawPic_implement();
	void	PvisShowStimAtHereNOW_implement();
	void	PvisShowFigAtHereNOW_implement();
	void	PvisHideStimAtHereNOW_implement();
	//#######################################################################################################################################
	//#####################  Picturs  #########################################################################################
	int	LoadPictureFile(LPCTSTR szFile, LPPICTURE &pPicture);
	LPPICTURE m_pPicture[MAX_STIM_OBJ];
	char	m_SavedPicFileName[MAX_STIM_OBJ][MAX_CHARS_OF_PIC_FILENAME];

	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################
	int	m_SelectedItemID;
	int	m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//#######################################################################################################################################
	//#####################  File  #########################################################################################
	CString	GetExecutableName();
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	char m_FileName[1024],m_JustFileName[1024],m_FolderPath[1024],m_FileNameWithoutEXT[1024];
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		wildcmp(char *wild, char *string);
	int		m_FileOpenSucceeded;
	//#######################################################################################################################################
	//##########################################  Shared Memory  ##############################################################
	double	m_ScreenSize_inDegrees_4VIS;
	void	RootFileOpen();
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	#include "MemMap_Decl_TASMOXYVIS.h"
	void Msg_to_TAS(int messageID);
	void CreateSharedMemoryArea_4TASVIS(void);
	void CloseMemoryMAP_4TASMOSVIS(void);
	int		m_IsMemMap;
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
	//Some tools
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int	MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);

	//#####################  Divice Context stuff to delete later //#####################
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
#define MaxBrush 10
	CBrush m_bkBrush;
#define MaxPen 4
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen],m_rgbPEN3[MaxPen];
	CPen	m_NULL, m_RED, m_BLUE, m_GREEN,m_DkGREEN,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_ORANGE2;
	CPen	m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;
	CBrush	m_backgroundBrush;//for non-Stimulus Display program's own Display
	CPen	m_backgroundScreenPen;//for non-Stimulus Display program's own Display
	//###################################################################################

	HBITMAP m_PicHBmp;	
	BITMAP	m_PicBmp;  	
	HDC		m_PicHDC;   
	int		m_fontHeight;
	void	DisplayTitle();
	#define MaxRGB100 100
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	int m_nScrMaxX, m_nScrMaxY;
	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime;
	int      m_nKey; //
	int		 m_nColorShow,m_SavedDC;
	CRect	m_rectShow3D;
	CFigure m_TwoD1,m_TwoD2,m_TwoD3,m_TwoD4, m_TwoD5, m_TwoD6, m_TwoD7, m_TwoD8, m_TwoD9, m_TwoD10;
	CFigure m_TwoD11, m_TwoD12, m_TwoD13,m_TwoD14,m_TwoD15,m_TwoD16,m_TwoD17,m_TwoD18,m_TwoD19;
	#define MAX_GRAPHS 100
	CFigure m_3D1,m_3D2,m_3D3, m_TwoD101,m_TwoD102,m_TwoD103, m_TwoD104,m_Board[MAX_GRAPHS];
	#define MaxFigRow 10
	#define MaxFigCln 10
	#define MaxFigLyr 3
	CFigure m_Figure;
	int		m_NewlyLoaded;
	void	VirMemDC();
	BOOL	FlagVerScreen;
	void	SetFontSize(CFigure *);
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
	clock_t m_start,m_finish;
	//////////////////////////////////////////////////////////
	CRect	m_savedClientRect;
	void	Re_measure_ClientView();
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	void	FigPosition_Settings() ;
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	LinearColorMix(COLORREF RGB_array[],int arraySize);
	void	LinearColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	ColorLegend(CDC *pDC, CFigure *pFig);
	void	FromScrnToDataCoordinate(int x, int y, float &DataX, float &DataY, CFigure *pFig);
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	VIsual_Stimuli_generator();
	void	Show3D();
	void	OnMyMessage();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	void	WhichFigure(CPoint point);
	void	Figs_Captured();
	void	Hilight_AllCaptured_Figs();
	void	DrawOneFigure(CFigure *pFig, int isToHighlight);
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCaptured(CFigure *pFig);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;

	CThreeD ThD;
//-------------------------------------------

// Attributes
public:
	CVISDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVISView)
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
	virtual ~CVISView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVISView)
	afx_msg void On3DGraph();
	afx_msg void On2DGraph_I();
	afx_msg void OnUpdate3DGraph(CCmdUI* pCmdUI);
	afx_msg void OnClearScreen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCalibrateScreen();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnSmallerPanels();
public:
	afx_msg void OnBiggerPanels();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // debug version in VISView.cpp
inline CVISDoc* CVISView::GetDocument()
   { return (CVISDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
