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

#define TOREAD		1	
#define NOT_TOREAD		0	

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
	char	m_MatchExtStr[100][64];
	int		m_NumExts, m_isToCopyExe;
	void	FindAllFiles(char *New_Blip_Dir, char* searchThisDir, bool searchSubDirs);
	int		Confirm_Deletion();
	//#######################################################################################################################################
    // thread handling
    HANDLE	m_hThread,  m_hEventBasedThread,m_hEvStopMainThread, m_EvDisplayRequest;
    HANDLE	m_hEvKeyPressed, m_hToNextStep;
	HANDLE	m_Events[MAXIMUM_WAIT_OBJECTS]; 
	//#######################################################################################################################################
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################
	int	m_SelectedItemID;
	int	m_CallingFrom;
	CFigure	*m_pSpotlightFig;
	//#######################################################################################################################################
	//#####################  File  #########################################################################################
	void Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	int	GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	CString GetExecutableName();
	char	m_FileName[500],m_FileNameWithoutEXT[500],m_FolderPath[500],m_NewBlip_FolderPath[500],m_JustFileName[100],m_JustFolderName[100],m_GrandParentFolderName[500]; 
	int	GetFile_WholePath_Name_CurPath(BOOL IsToREAD);
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int	GetFolderName(char* FolderPath, char *FolderName);
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
	double	box_muller(double m, double s);
	//###################################################################################

	int		m_fontHeight;
	#define MaxRGB100 100
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	int m_nScrMaxX, m_nScrMaxY;
	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime;
	BOOL	 m_bShow2D;
	int      m_nKey; //
	int		m_NewlyLoaded;
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
	clock_t m_start,m_finish;
	//////////////////////////////////////////////////////////
	CRect	m_savedClientRect;
	void	Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) ;
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	double	DifferenceToRound(double &NumToAdjust, double VariableRange);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	Show2D_I();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	int		Find_Last_Num_inString(char *GivenString);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;
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
	afx_msg void On2DGraph_I();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
};

#ifndef _DEBUG  // debug version in AnalysisView.cpp
inline CAnalysisDoc* CAnalysisView::GetDocument()
   { return (CAnalysisDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
