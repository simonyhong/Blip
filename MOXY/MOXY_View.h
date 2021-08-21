// MOXY_View.h : interface of the CMOXY_View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOXY_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_MOXY_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//###############################################################################################################################
//----------------------------------------   My_Math.h --------------------------------------------------
#define RAND_RATE       ( (double)rand()/(double)RAND_MAX )
inline	int Random(int N){ if(N>0)return rand()%N; else return 0;}
#define PROBABILITY(P)  (RAND_RATE<(double)P)
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
#define	DISTANCE3D_AB(x1,y1,z1,  x2,y2,z2) ( sqrt(SQDISTANCE_AB_3D(x1,y1,z1,  x2,y2,z2) ) )
#define	MAG_2D_VECTOR(x,y)    sqrt(((x)*(x)+(y)*(y)))
#define PSEUDO_DISTANCE(x1,y1, x,y, x2,y2) (  fabs( (y1-y2)*x + (x2-x1)*y +x1*y2-x2*y1 ) / DISTANCE_AB(x1,y1,x2,y2)  ) /*The distance between a line and a point */
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
#define	SQMAGNITUDE3D(x,y,z)       (     (x)*(x) + (y)*(y)+ (z)*(z)     )
#define	SQDISTANCE_AB_3D(x1,y1,z1,  x2,y2,z2) ( (           ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2))+ ((z1)-(z2))*((z1)-(z2))                )  )
#define INT_DOT_PRODUCT(a1,a2, b1,b2) ( (a1)*(b1) + (a2)*(b2) )
#define INT_CROSS_PRODUCT(a1,a2, b1,b2) ( (a1)*(b2) - (b1)*(a2) )
#define LIMIT_I1I(a) 	{if(a> 1.)a= 1.;	else if(a<-1.)a=-1.;}     

#include <afxmt.h>
#define MsgPosX(pB, X_inDataCrd) (pB->m_dMinX+(X_inDataCrd)*(pB->m_dSpanX))
#define MsgPosY(pB, relY) (pB->m_dMinY+(relY)*(pB->m_dSpanY))
#define ChrPosX(ID,X_inDataCrd) (m_Fig[ID].m_dMinX+(X_inDataCrd)*(m_Fig[ID].m_dSpanX))
#define ChrPosY(ID,relY) (m_Fig[ID].m_dMinY+(relY)*(m_Fig[ID].m_dSpanY))

//#################### XY-Monitor's display modes ############
#define EYE_MONITOR_MODE 0 //EYE-movement monitor
#define CARTESIAN_MODE 1
#define MAX_EYE_MONITOR_ANGLE 80 //EYE-movement monitor
//############################################################

class CMOXYDoc;
class CFigure;

class CMOXY_View : public CView
{
protected: // create from serialization only
	CMOXY_View();
	DECLARE_DYNCREATE(CMOXY_View)
public:
//###############################################  User variables and functions are below  ############################################
	////##############  Shared variables and functions across different tasks #############
	//Some shared functions across different tasks
	void	Init_Variables();
	void	Name_and_Value_to_record(char Name[], double Value);
	void	Name_and_Value_to_record(char Name[], double V1, double V2);
	void	Name_and_ArrayOfValues_to_record(char Name[], int ArrayOfValues[], int N);
	void	Name_and_ArrayOfValues_to_record(char Name[], float ArrayOfValues[], int N);
	void	Name_and_ArrayOfValues_to_record(char Name[], double ArrayOfValues[], int N);
	void	InitUserData();
	//#######################################################################################################################################
	//#####################################  Trials and ECodes ##############################################################################
	int		m_isSaving, m_IsVertSeparatorHeld, m_IsHorizSeparatorHeld, m_isFirstMessage;
	void	Iitialize_UserPicked_Variables_to_Display();

	__int64 m_firstBehaviorAiIdx, m_lastBehaviorAiIdx;
	int		m_numBehaviorChannels,	m_BehaviorSamplingRate;
	__int64	m_StartTimeOfAI, m_EndTimeOfAI;
	void	Serialize(CArchive& ar);
	CString	m_strFileName;

	//#####################  Running line display ####################
	CRunningLineDisplay m_RunPanel[NUM_RL_PANELS];
	int		m_callingFrom, m_TotNumUserPickedVariables;
	void	Connect_ChannelVariables_ToMemoryMap(int FigID);
	int		m_isRunningLineSDF,m_isMOXY_Running;
	int		m_isInsidePane,	m_firsVIStErrorMsg;
	void	Display_forMOXY(int RequestIdx, CDC *pObjDC);
	int		m_DispEvents[MAX_DISP_REQUESTS][4], m_DispEventIdx;
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue); 
	void	RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2); 
	void	RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2); 
	void	EditTriggerECode(CRunningLineDisplay *pMonitorFig);
	int		Find_Last_PositiveNum_inString(char *GivenString, int &NumToReturn);
	int		Find_Last_Num_inString(char *GivenString, int &NumToReturn);
	void	EditPrefixSuffix();
	void	AcceptOneString(CRunningLineDisplay *pMonitorFig);
	void	Accept_ChannelID_forXY_disp(CRunningLineDisplay *pMonitorFig);
#define MaxNumTimer 20
	int		m_durOfIdle[MaxNumTimer];
	void	init_IdleTimers_to0();
	BOOL	m_IsCatchThreadRunning,m_IsAntiShakeThreadRunning, m_IsAvoidDetactThreadRunning;
	void	Give_ObjID_and_NameOfPic(int ObjID, char Name[]);
	//#####################  Digital IO  #################################################################################
	int		m_DIO_state[MaxDIO],m_whichDIO, m_UserClickedDIOIdx, m_UserClickedDIOIdx_ON;
	void	InitDIO_FIg();
	void	Relay_ON_n_DisplayLED(int whichBIT);
	void	Relay_OFF_n_DisplayLED(int whichBIT);
	void	Relay_ON(int whichBIT);
	void	Relay_OFF(int whichBIT);
	void	Label_DIO(CFigure *pFig, CDC *pDC);

	//#####################  Task Scheduler basics ####################
	__int64 m_CPUfrq, m_newVal, m_lastVal, m_sampleCounter;
	double HighPerformanceTime(__int64 lastVal);
	int		m_PrvNnNumberToRecord;
	void	SleepAccurate(int FixedDur, int variableDur);
	void	Sleep_NOT_Accurate(int FixedDur, int variableDur);
	void	SleepAccurate_1ms();
	//Parallel Threads
	CRITICAL_SECTION 	m_CriticalSection;
	#define MAX_NUM_OF_TIME_KEEPERS 20
	int		m_TimerUsage[MAX_NUM_OF_TIME_KEEPERS];
		
	//###########################  XY  (Eye or any other) Monitor  #############################
#define XY_MONITOR 1
	CFigure	*m_pXYFig;
	double m_prvEyePosX[NUM_2D_BEHAVIOR_OBJS], m_prvEyePosY[NUM_2D_BEHAVIOR_OBJS];
	int		m_PrvTopleftX[NUM_2D_BEHAVIOR_OBJS], m_PrvTopleftY[NUM_2D_BEHAVIOR_OBJS], m_EyeSquareWidth, m_EyeSquareHeight;
	double m_MinXplusSome, m_MaxXminusSome, m_MinYplusSome, m_MaxYminusSome; 

	double	m_XY_Monitor_Scale;
	double	m_dataEyeXsaved[2], m_dataEyeYsaved[2]; //The data eye position
	int		XY_Left, XY_Right, XY_Up, XY_Down;//XY window display position
	int		m_XY_DisplayMode;
	void	Init_XYwindow();
	void	Get_Latest_XYpos();
	void	DrawNew_ObjPositions(CDC *pDC);
	int		IsEyeIn(int WindID,int EyeID);
	void	HasEyeMoved(int EyeID);
	void	Monitor_EyeOutOfBox_thread(int WindID,int EyeID, int dur_Wait,int dur_variWait);
	int		m_WindID_THREAD,m_EyeID_THREAD, m_dur_Wait_THREAD, m_dur_variWait_THREAD;//These variables are used in Monitor_EyeOutOfBox_thread()
	//###########################   Replicating VIS  ###########################
	HBITMAP m_PicHBmp;	
	BITMAP	m_PicBmp;  	
	HDC		m_PicHDC;   
	int		m_Msg_Q_Idx, m_IsToShowFigureOnRightClick;
	unsigned  m_PrevTicks;
	int		m_topleftX[MAX_STIM_OBJ],m_topleftY[MAX_STIM_OBJ],m_width_inPixels[MAX_STIM_OBJ],m_height_inPixels[MAX_STIM_OBJ];
	void	Which_MOXY_Function(int messageID);
	void	Which_ReplicatingVIS_Function(int messageID);
	void	Msg_to_TAS(int messageID);
	void	PvisAllOff_ReplicationOfVIS();
	void	PvisSetBackColor_ReplicationOfVIS();
	void	PvisDrawBar_ReplicationOfVIS();
	void	PvisDrawDisk_ReplicationOfVIS();
	void	PvisSwitchStim_ReplicationOfVIS();
	void	PvisDrawPic_ReplicationOfVIS();
	void	PvisShowStimAtHereNOW_ReplicationOfVIS();
	void	PvisShowFigAtHereNOW_ReplicationOfVIS();
	void	PvisHideStimAtHereNOW_ReplicationOfVIS();
	//#####################  Picturs  ################################
	int		LoadPictureFile(LPCTSTR szFile, LPPICTURE &pPicture);
	LPPICTURE m_pPicture[MAX_STIM_OBJ];
	char	m_SavedPicFileName[MAX_STIM_OBJ][MAX_CHARS_OF_PIC_FILENAME];

	//###########################   Mouse  ###########################
	int		m_selPaneID; //This is for the mouse manipulation
	float	m_paneTopX,m_paneTopY,m_paneBottomX, m_paneBottomY;
	void	FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY);
	int		m_SelectedVisualObjectID, m_RightClickX,m_RightClickY, m_PrvRightClickX,m_PrvRightClickY, m_ReuseClickedPos;

	//############   Communication with VIS & MOXY   ############
	void	PvisShowStimAtHereNOW(int N, int fixObj[], double X[], double Y[]);
	void	PvisHideStimAtHereNOW(int N, int fixObj[]);

	int		WaitUntilEyeIsIn(int WindID,int EyeID,int dur_Wait,int dur_variWait);
	int		WaitUntilEyeIsOut(int WindID,int EyeID,int dur_Wait,int dur_variWait);
	int		WaitUntilEyeIsInOrOut(int InOrOut,int WindID,int EyeID,int dur_Wait,int dur_variWait);
	int		TimerRotator; //Get rid of this once the timer bug is fixed.
	void	Register_a_User_Picked_Variable();
	//##############################################################################################################
	//##########################################  Shared Memory ####################################################
	#include "MemMap_Decl.h"
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	void	CloseTrialMemoryMAP();

	#include "MemMap_Decl_TASMOXYVIS.h"
	void CreateSharedMemoryArea_4TASVIS(void);
	void OpenSharedMemory_4TASVIS(void);
	void DestroySharedMemoryArea_4TASVIS(void);

	HANDLE	m_M_DO_Active[MAX_DO_NUM];
	int		m_IsMemMap;
	int		qIdxSpike(int i);
	//#############################################  Syncronization #############################################
	CSemaphore m_sema;

	//####################################  Statistics  ##############################################################
	int		m_FirstTime0ElementErr;
	double	Mean(double *Array, int N);
	double	Mean(float *Array, int N);
	double	Mean(int *Array, int N);
	double	SD_GivenM(double *Array, int N,double mean);
	double	SD_GivenM(float *Array, int N,double mean);
	double	SD_GivenM(int *Array, int N,double mean);
	double	Mean_OnlyPositive(double *Array, int N);
	double	SD_GivenM_OnlyPositive(double *Array, int N,double mean);
	void	Gaussian_WhiteNoise_0to1(double noise[], int numSamples);
	double	GenHist(double Data[],int dataSize, int Hist[], int binNum, double binWidth);
	int		Select_Action(float *pi, int N);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int		MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);

	//####################  Divice Context stuff that needs to be erased when the program ends  ######################
	CDC m_DC[MaxDC],m_DC0Mrr;
	CBitmap m_bmp[MaxDC],m_bmp0Mirror;
#define MaxBrush 10
	CBrush	m_DIO_ON_Brush;
	CBrush	m_XY_BlackBrush, m_DIO_backgroundBrush;
	CBrush	m_WhiteBrushView;
	CPen	m_backgroundScreenPen;//for non-Stimulus Display program's own Display
	CPen	m_NullPen, m_WhitePen2, m_RED, m_BLUE, m_GREEN,m_DkGreenPen,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK3,m_GRAY,m_GrayPen2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED2, m_BLUE3, m_GREEN3;
	CPen	m_PositionWindPens[MAX_EYE_WIND];
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;

	//####################  Other drawing-related stuff  ######################
	#define MaxRGB100 100
	COLORREF m_RGB100[MaxRGB100], m_BnW100[MaxRGB100];
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	int m_nScrMaxX, m_nScrMaxY;
	int	m_FontWidth;
	static BOOL	 m_bIsON;
	int      m_nKey; //
	int		 m_nColorShow,m_SavedDC;
	int		m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int		m_NnCounterArrowDrawn,m_RWDCounterArrowDrawn, m_BChanArrowDrawn;
	BOOL	m_bIsToggleON;
	CRect	m_savedClientRect;
	#define MAX_GRAPHS 4
	CFigure m_Fig[MAX_GRAPHS], *m_pRunLineFig;
	void	VirMemDC();
	void	Init_Fig_Positions();
	BOOL	m_FlagVirMemDC;
	BOOL	m_bIsScrXYforIJ;
	double m_dVariable1, m_dVariable2, m_dVariable3;
	void	SetFontSize(CFigure *);
	void	positionBox_pos(int WindID, double X, double Y);
	void	positionBox_size(int WindID, double width, double hight);
	void	positionBox_OnOff(int WindID, int Switch);
	void	Turn_PositionBox_OnOff(int WindID, int Switch);
	void	ReCalibrateAllFigures();
	void	Redraw_XYwindow();
	void	Redraw_DIO_Fig();
	void	Draw_User_RtClick_Pos();
	void	Draw_IncreaseArrow(int &Variable, ClickableLabel &Label, int isToHighlight, CDC *pObjDC);
	void	Draw_DecreaseArrow(int &Variable, ClickableLabel &Label, int isToHighlight, CDC *pObjDC);
	void	RemeasureRWDRect(int Dur_BigRWD);
	void	Remeasure_BehavioralObj_Rect(int BehavioralObj);
	void	UpdateRwdAmount_AND_Disp_it();
	//#####################  File  #################################################################################
	CString m_prvDate,m_Date;
	char	m_FileNameFULL[1024],m_JustFileName[1024],m_FolderPathWithoutFileName[1024],m_LastPartOfFolderName[1024],m_FileNameWithoutEXT[1024], m_PreFix[1024],m_SuFFix[1024], m_BaseOfFileName[1024];
	double	m_futureVariable;
	void	UpdateFileName_StroredLocal_AND_Disp_Name_n_RecodingStatus();
	void	RootFileOpen();
	int		GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		GetLastFolderName(char *FolderPath, char *LastFolderName);
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	int		wildcmp(char *wild, char *string);
	void	RemakeFileName();
	void	Make_TheBaseOfFileName();
	CString	GetExecutableName();
	CString GetDateString();
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	int		FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[]);
	void	Launch_DAS_sv();
	void	Save_MOXY_Config();
	void	Read_MOXY_Config();
	//##############################################################################################################
	int		m_isRtBUTTON_pressed;
	void	Re_measure_ClientView();
	int		m_Separator_inScreenCoord,m_HorizontalSeparator_inScreenCoord;
	double	m_VerticalRatioByDivider;
	void	RecalibratePanes();
	void	Auto_Fig_Settings(int howManyRows, int sizeOfOneRow, int howManyClmns, int sizeOfOneClmn, double scale) ;
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	Show2D_II();
	void	Monitoring_System();
	void	Running_Line_Display(int RL_FigID);
	void	OnAxisScale(CFigure *pFig);
	void	OnMyMessage();
	double	Rectify(double Value){if(Value>0.)return Value; else return 0.;}
	int		WhichPane(CPoint point);

//-------------------------------------------

// Attributes
public:
	CMOXYDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMOXY_View)
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
	virtual ~CMOXY_View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMOXY_View)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void Launch_DAS_cl();
	afx_msg void On_Launch_VIS();
	afx_msg void On_Launch_RAS();
	afx_msg void On_Launch_ANTS();
	afx_msg void On_Launch_Oscilloscope();
	afx_msg void OnCLAS();
	afx_msg void On_Launch_XCOR();
	afx_msg LRESULT AntidromicStim_Display_forMOXY(WPARAM, LPARAM);
	afx_msg LRESULT DIO_AutoOnOff_CallBack(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSmallerPanels();
	afx_msg void OnBiggerPanels();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCloseBlip();
	afx_msg void OnViewRightClickFigure();
	afx_msg void OnUpdateViewRightClickFigure(CCmdUI *pCmdUI);
	afx_msg void OnView_VisibleObjs();
	afx_msg void OnViewAngleMonitor();
	afx_msg void OnViewCartesian();
	afx_msg void OnUpdateViewCartesian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewAngleMonitor(CCmdUI *pCmdUI);
	afx_msg void OnVirtualElectrode();
};

#ifndef _DEBUG  // debug version in MOXY_View.cpp
inline CMOXYDoc* CMOXY_View::GetDocument()
   { return (CMOXYDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOXY_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
