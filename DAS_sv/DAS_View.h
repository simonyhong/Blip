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
class CDasView : public CView
{
protected: // create from serialization only
	CDasView();
	DECLARE_DYNCREATE(CDasView)
//-------------------------------------------
public:
	//############################ MEMORY MAP ###############################################################################################
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	void	Init_MemoryMapVariables();
	int		IsToCreate;
	//############################ NI board #################################################################################################
	int		CountBoards();
	void	ReportSummaryOfBoards();
	char	m_BoardName[32][512];
	int		m_BoardIdxUsed;
	int		m_NumOfBoards;
	//########################### Threads & synchronization #################################################################################
	HANDLE m_EvStopProcess, m_Ev_WakeUp_ForAntidromicStimulation;
	HANDLE m_hThread, m_hSimulatedDAQmxThread, m_hDigitalOutThread, m_hCheckToEnd;
	void	Simulated_DAQmx();
	HANDLE m_DO_Active[MAX_DO_NUM];
	HANDLE m_DI_Active;
	//#######################################################################################################################################
	CArray<double, double& >m_dRawData; //Raw data array to read in.
	CAiChannel AiChan[MAX_AI_CHAN];
	int		m_AiChanID[MAX_AI_CHAN];
#define MAX_USABLE_AI_IDX_ARRAY 200
	int		m_NumAiChannels,m_ArraySize_RawData, m_ArrayIndex_forUsableAiId;
	int		m_UsableAiIdx_atTheTimeOfDataRetrival[MAX_USABLE_AI_IDX_ARRAY][NUM_NN_AI_CHAN]; //This is used to identify "M_usableAiIdx" of each channel at one point of time. This is used to conveniently identify the index of data when a stimulation happens.

	//#######################################################################################################################################
	__int64 m_idx_Spike;
	int		m_SamplingCounter;
	void	SleepAccurate_inMicS(int FixedDur);
	double	HighPerformanceTime(__int64 lastVal);
	CRITICAL_SECTION 	m_CriticalSectionDAQ;
	void	DigitalOut();
	//#########################################  Antidromic Function  ########################################################################
	void	Set_AntiDO_forXms_Function(__int64 AiChan_M_timeStampMicS, __int64 AIidx, int TriggeringChannel, int ArrayIndexForUsableAiId_handedOverByRequestingChan);
	void	Set_OrthoDO_forXms_Function();
	__int64 m_AiChan_M_timeStampMicS;
	int		m_nEstimatedAntidromicStimPointIdx, m_TriggeringChannel;
	void	Anti_Ortho_Stimulation_Loop();
	void	Check_to_End_DASsv();
	int		m_AntidromicJobRequest,	m_IsResting;
	int		m_ArrayIndexForUsableAiId_handedOverByRequestingChan;
	// Zapper
	int		m_StimGatingRelayID;
	__int64 m_ExpectedTimeOfStim_AIidx, m_StartAIidx_toLearnArtifact, m_StartAIidx_toZapArtifact, m_ExpectedTimeOfARTIFACT_MinusHalfMs;
	int		m_HowManyDataPoints_ToFind_StartOfArtifact, m_NumOfArtifactDataPoints_toLEARN, m_NumOfArtifactDataPoints_toZAP;
	double *m_pArtifact, m_LearnedLevel;
	void	Zapper__Antidromic_Stimulation_Given_from( int StimGatingRelayID, int StartAIidx_ToFind_StartOfArtifact);
	void	Find_EndAIidx(CAiChannel * pCh, __int64 InitIdx, __int64 &EndIdx, int LengthToSearch, int &RemainingLengthToSearch);
	void	Find_BeginingOfArtifact(CAiChannel * pCh, double *chAI);
	void	Zapper_Learn(CAiChannel * pCh, double *chAI);
	void	Zapper_Zap(CAiChannel * pCh, double *chAI);
	//#######################################################################################################################################
	TaskHandle	m_pTaskHandle_AI, m_pTaskHandle_DO, m_pTaskHandle_DI;
	signed long		m_SamplesRead[MAX_DI_NUM], m_BytesPerSample[MAX_DI_NUM];
	//#######################################################################################################################################
	//################   VT_DISCRIMINATOR_TASK  ################
	__int64 m_CPUfrq, m_newVal, m_beginVal, m_beginValInMicS;
	double	m_1000000perCPU;
	int		m_initDataPointIdx,m_endDataPointIdx,m_RefreshAtEveryNWaveforms;
	//#####################  File  #########################################################################################
	CString	GetExecutableName();
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	int		FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[]);
	char m_FileName[1024],m_JustFileName[1024],m_FolderPath[1024],m_FileNameWithoutEXT[1024];
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		wildcmp(char *wild, char *string);
	void	SaveThresholds();
	void	ReadThresholds();
	//#####################  Statistics  ###########################################################################

	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime;
	BOOL	m_bProceed;
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	LineCrossingRect(int x1,double y1,int x2,double y2,float recLeft,float recTop,float recRight,float recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	DAS_sv_main();
	void	Show2D_II();
	void	Show2D_III();
	void	Show2D_IV();
	void	Show2D_V();
	void	Show2D_VI();
	void	Show2D_VII();
	void	Show3D();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
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
	afx_msg void On2DGraph_I();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnChangeOfChanConfig(WPARAM, LPARAM);
	afx_msg LRESULT SetDO_forXms(WPARAM, LPARAM);
};

#ifndef _DEBUG  // debug version in DAS_View.cpp
inline CDasDoc* CDasView::GetDocument()
   { return (CDasDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DASVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
