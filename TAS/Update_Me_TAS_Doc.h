// Update_Me_TAS_Doc.h : interface of the CTASDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAS_VISDOC_H__4E7AB3C6_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_TAS_VISDOC_H__4E7AB3C6_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//###############################################################################################################################
//----------------------------------------   My_Math  --------------------------------------------------
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
//----------------------------------------  End of My_Math --------------------------------------------------

#include "SharedConstants.h"
#include "SharedConst_TASMOXYVIS.h"
#include <afxmt.h>

//########### File #########
#define TOREAD		1	
#define NOT_TOREAD	0	
#define APPEND_FILE		0	
#define OVERWRITE_FILE	1	

//########### Eye #########
#define EYE_OUT_OF_TGT		(-1) //CTASDoc uses negative integers for internal ECodes.
#define EYE_STILL_ON_TGT	(-2) //CTASDoc uses negative integers for internal ECodes.

//########### DIO #########
#define DIO_ON					1
#define DIO_OFF					0
//##########################
#define NO_ECODE			INT_MAX

#define ON_SIGNAL_RISE 1
#define ON_SIGNAL_FALL 2

//########### Escape conditions #########
#define BeforeEscapeConditions_GiveDurationInMs(Duration) {double TASdur=Duration; __int64 lastVal; QueryPerformanceCounter((LARGE_INTEGER*)&lastVal); double timeleft=TASdur-m_pDoc->HighPerformanceTime_inMs(lastVal);while(timeleft>0.2){
#define AfterEscapeConditions	timeleft=TASdur-m_pDoc->HighPerformanceTime_inMs(lastVal); if(timeleft>ACCURACY_OF_SLEEP_PLUS1)Sleep(1); else m_pDoc->SleepAccurate_1ms();	}	}

//########### User-picked variables #########
struct UserPickedVar{
	int		m_TypeOfVar;
	unsigned char *m_pUnsignedCharValue;
	int		*m_pIntValue;
	float	*m_pFloatValue;
	double	*m_pDoubleValue;
};

class CUserData
{
public:
	int m_NumUserData, m_SizeOfUserData, m_TotNumUserVlaluesToRecord;
	CArray<CString, CString& >m_NameArray;
	CArray<int, int& >m_SizeOfName, m_FirstLocationOfUserValue, m_NumOfValues;
	CArray<double, double& >m_ValuesToRecord;
	CArray<__int64, __int64& >m_TimeOfOccurrence;
	void Name_and_ArrayOfValues_to_Record(char Name[], int    ArrayOfValues[], int N);
	void Name_and_ArrayOfValues_to_Record(char Name[], float  ArrayOfValues[], int N);
	void Name_and_ArrayOfValues_to_Record(char Name[], double ArrayOfValues[], int N);
	void	Serialize(CArchive& ar);
	CUserData();
	virtual ~CUserData();
};

class CTASView;
class CTASDoc : public CDocument
{
protected: // create from serialization only
	CTASDoc();
	DECLARE_DYNCREATE(CTASDoc)

protected: 
	int		m_TAS_trialID_tobeSaved, m_UserGave_TrlID; //This is used at the END of the trial to save the trial #.
// Attributes
public:
	CTASView *m_pView;
	int m_bPause;
	int		m_MessageID, m_MessageID_toMOXY;
	static BOOL	 m_IsPlayButtonON;
	BOOL	tas_bFistTime;
	void	TAS();
	int		GetBigRWD();
	void	SetBigRWD(int RwdDuration);
	int		GetTaskType();
	void	SetTaskType(int RwdDuration);
	//Declare the function array to hold the addresses of functions
	void (CTASView::*FunctionArray[MAX_NUM_OF_FUNCTIONS])();
	CWinThread* m_pTAS_Thread;
	__int64	m_TimeOfTrialStart;
	int		m_AI_Size_warning;
	void	Give_TrialID_toSave(int TrlID){m_TAS_trialID_tobeSaved=TrlID; m_UserGave_TrlID=1;}
	void	OrthodromicStimulation();
	void	SetOrthoStimGatingRelay__DoThisAtLeast1msBeforeCallingOrthodromicStimulation(int StmDO);
	int		GetIndexOfSimChannel(int StimDO);
	//#####################  Digital IO  #################################################################################
	void	Relay_ON_n_DisplayLED(int whichBIT);
	void	Relay_OFF_n_DisplayLED(int whichBIT);
	void	Relay_ON(int whichBIT);
	void	Relay_OFF(int whichBIT);
	void	SendOut_M_DO_Array(); //Set m_pDoc->M_DO_Array[] first and use SendOut_M_DO_Array() to send out DO message
	//#####################  Task Scheduler basics ####################
	__int64 m_CPUfrq, m_newVal, m_lastVal, m_sampleCounter;
	double HighPerformanceTime_inMs(__int64 lastVal);
	int		m_TaskType, m_PrvNnNumberToRecord;
	void	SleepAccurate(int FixedDur, int variableDur);
	void	Sleep_NOT_Accurate(int FixedDur, int variableDur);
	void	SleepAccurate_1ms();
	void	SleepAccurate_with_TIMER(int Timer_ID, int variableDur);
	int		FindAvailableTimer();
	CRITICAL_SECTION 	m_CriticalSection, m_CriticalSection4Msg, m_CriticalSection4Serilize;
	#define MAX_NUM_OF_TIME_KEEPERS 20
	int		m_TimerUsage[MAX_NUM_OF_TIME_KEEPERS];
	#define MaxNumTimer 20
	int		m_durOfIdle[MaxNumTimer];
	void	init_IdleTimers_to0();
	void	Give_ObjID_and_NameOfPic(int ObjID, char Name[]);
	//#####################################  Trials and ECodes ##############################################################################
	void	ECode(int);
	void	ECode_with_Time(int ecode, __int64 Time);
	float	InterpretAliasIntoEcode(char *Alias);
	void	StartCollectingTrialData_Give_StartECode(int ecode);
	int		m_isSaving, m_IsSeparatorHeld, m_IsHorizSeparatorHeld;
	void	Iitialize_ECode_As_Word();
	void	ECode_as_Word(int ECode, char *Word);
	void	Iitialize_UserPicked_Variables_to_Display();

	__int64 m_firstBehaviorAiIdx, m_lastBehaviorAiIdx;
	int		m_numBehaviorChannels,	m_BehaviorSamplingRate;
	__int64	m_StartTimeOfAI_MicS, m_EndTimeOfAI_MicS;
	CString	m_strFileName;

	//#####################  Running line display ####################
	int		m_callingFrom;
	void	Loop_Thread();
	int		m_isRunningLineSDF,m_isTAS_Running;
	int		m_isInsidePane,	m_firsVIS_ErrorMsg,	m_firsMOXY_ErrorMsg;
	int		m_DispEvents[SIZE_OF_DISP_REQUEST_QUEUE][4], m_DispEventIdx;
	UserPickedVar	m_UserPickedVar[MAX_NUM_USER_PICKED_VARS];
	void	Register_a_variable_to_display(char NickName[], int FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay);
	void	Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], int &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay);
	void	Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], unsigned char &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay);
	void	Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], float &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay);
	void	Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], double &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay);
	//###########################  Eye Monitor  #############################
#define XY_MONITOR 1
#define MAX_EYE_WIND 10
	double	m_prvEyePosX[NUM_2D_BEHAVIOR_OBJS], m_prvEyePosY[NUM_2D_BEHAVIOR_OBJS];
	int		m_PrvTopleftX, m_PrvTopleftY, m_EyeSquareWidth, m_EyeSquareHeight;
	double m_MinXplusSome, m_MaxXminusSome, m_MinYplusSome, m_MaxYminusSome;

	double	m_dataEyeXsaved[2], m_dataEyeYsaved[2]; //The data eye position
	int		XY_Left, XY_Right, XY_Up, XY_Down;//XY window display position
	void	Get_Latest_XYpos();
	int		IsEyeIn(int WindID,int EyeID);
	void	Use_the_REAL_EYE_position(int EyeID);
	void	Monitor_EyeOutOfBox_thread(int WindID,int EyeID, int dur_Wait,int dur_variWait);
	int		m_WindID_THREAD,m_EyeID_THREAD, m_dur_Wait_THREAD, m_dur_variWait_THREAD;//These variables are used in Monitor_EyeOutOfBox_thread()

	//#####################  File  #################################################################################
	CString m_Header;
	int  m_LengthOfHeader;
	CString m_prvDate,m_Date;
	char	m_FileNameFULL[1024],m_JustFileName[1024], m_PreFix[1024],m_SuFFix[1024], m_BaseOfFileName[1024],m_FolderPathWithoutFileName[1024],m_LastPartOfFolderName[1024],m_FileNameWithoutEXT[1024];
	double	m_futureVariable;
	void	RootFileOpen();
	void	Launch_MOXY();
	void	Launch_VIS();
	void	Find_Blip_andAddSome(char AddedFolderName[], char FullPath[]);
	void	EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell();
	int		LocateAndCreateFolder_IfItIsNotAlreayThere();
	CString GetDateString();
	CString	GetExecutableName();
	int		CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FolderName[], CString Date);
	void	Make_TheBaseOfFileName_usingInfoInLocalVariables();
	void	Make_TheBaseOfFileName_usingInfoInMemoryMap();
	void	Add_NnNumber_toFileName();
	void	Give_Prefix_Suffix_forFileName(char prefix[], char suffix[]);
	void	Serialize_Header();
	void	Serialize(CArchive& ar, int TrlIdxInMap, int UserDataSetID_for_SaveFunctionOnly, int BehaviorSamplingRate, __int64 StartTimeOfAI_MicS, __int64 EndTimeOfAI_MicS, int NumBehaviorChannels, __int64 firstBehaviorAiIdx, __int64 lastBehaviorAiIdx);
	void	Write_Your_Header_Here_to_Write_on_the_File(char *header);
	void	Name_and_ArrayOfValues_to_record(char Name[], double ArrayOfValues[], int N);
	void	Name_and_ArrayOfValues_to_record(char Name[], float ArrayOfValues[], int N);
	void	Name_and_ArrayOfValues_to_record(char Name[], int ArrayOfValues[], int N);
	void	Name_and_Value_to_record(char Name[], double Value);
	void	Name_and_Value_to_record(char Name[], double V1, double V2);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9);
	void	Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9, double V10);	
	int		GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int		GetLastFolderName(char *FolderPath, char *LastFolderName);
	int		GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	int		Find_Last_Num_inString(char *GivenString, int &NumToReturn);
	int		Find_Last_PositiveNum_inString(char *GivenString, int &NumToReturn);
	void	Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany);
	int		wildcmp(char *wild, char *string);
	
	//###########################  User defined variables  #############################
	int		m_UserDataSetID_for_SaveFunctionOnly;
	CUserData m_UserData[SETS_OF_USER_VARIABLE_BLOCKS]; //MOXY has a separate user data memories; SETS_OF_USER_VARIABLE_BLOCKS sets alternating just like m_UserData[] in TAS.

	//##############################################################################################################
	//##########################################  Shared Memory ####################################################
	#include "MemMap_Decl.h"
	int		m_IsMemMap;
	void	CreateSharedMemoryArea();
	void	CloseSharedMemoryArea();
	void	CloseTrialMemoryMAP();
	int		qIdxSpike(int i);
	#include "MemMap_Decl_TASMOXYVIS.h"
	void CreateSharedMemoryArea_4TASVIS(void);
	void CloseMemoryMAP_4TASMOXYVIS(void);
	void	At_the_End_of_a_Trial_Check_if_MemoryMap_config_meets_the_trials_config();
	HANDLE	m_M_DO_Active[MAX_DO_NUM];
	HANDLE	m_M_DI_Active;
	//############   Communication with VIS & MOXY  ############
	void	Inform_MOXY(char *message); //The same as "Inform_MOXY_withoutHandshaking()"
	void	Inform_MOXY_withoutHandshaking(char *message);// Warning: this one does not do "Hand-shake" with MOXY. For this reason, if you manipulate the same drawing objects rapidly in succession, the result may not be as you expect. So use different objects for rapid sequential manipulations, such as different eye windows.
	void	Send_Msg_to_MOXY_and_Wait_untilFinishes(char *message, int MaxWait_inMS);
	void	PvisAllOff();
	void	PvisSetBackColor(int red, int green, int blue);
	void	PvisSetStimColors(int N, int ObjArray[], int Red[], int Green[], int Blue[]);
	void	PvisStimLocation(int N, int ObjArray[], double X[], double Y[]);
	void	PvisDrawBar(int N, int ObjArray[], double width[], double hight[]);
	void	PvisDrawDisk(int N, int ObjArray[], double width[], double hight[]);
	void	PvisSwitchStim_with_ECODE(int N, int ObjArray[], int Switch[], int ECODE);
	void	PvisSwitchStim_with_ECODE_atThresholdcrossingInChX(int N, int ObjArray[], int Switch[], int ECODE, int ChX);
	void	Drop_myEcode_at_threshold_crossingInChX(int myEcode, int ChX, int RiseOrFall, int dur_Wait);
	void	PvisDrawPic(int N, int ObjArray[], double width[], double hight[]);

	void	Send_Msg_to_VIS_and_Wait_untilFinishes(char *message);
	void	Msg_from_TAS_to_MOXY(char *message);
	int		WaitUntilEyeIsIn(int WindID,int EyeID,int dur_Wait,int dur_variWait);
	int		WaitUntilEyeIsOut(int WindID,int EyeID,int dur_Wait,int dur_variWait);
	int		WaitUntilEyeIsInOrOut(int InOrOut,int WindID,int EyeID,int dur_Wait,int dur_variWait);
	int		WaitUntilSignalOn(int whatToCheck, int signalSource, int dur_Wait);
	int		TimerRotator; //Get rid of this once the timer bug is fixed.
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
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);

	//###########################   Mouse  ###########################
	int		m_selPaneID; //This is for the mouse manipulation
	float	m_paneTopX,m_paneTopY,m_paneBottomX, m_paneBottomY;

	//####################  Other drawing-related stuff  ######################
	int m_nScrMaxX, m_nScrMaxY;
	int	m_FontWidth;
	int      m_nKey; //
	int		m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int		m_BlownUpPaneID,m_NnCounterArrowDrawn,m_RWDCounterArrowDrawn;
	#define MAX_GRAPHS 4
	void	Init_Fig_Positions();
	BOOL	m_bIsScrXYforIJ;
	double m_dVariable1, m_dVariable2, m_dVariable3;
	void	Display_Centralized();
	void	positionBox_pos(int WindID, double X, double Y);
	void	positionBox_size(int WindID, double width, double hight);
	void	positionBox_OnOff(int WindID, int Switch);
	void	DisplayLED(int whichBIT, int IsOn);
	//##############################################################################################################
	int		m_isRtBUTTON_pressed;
	void	Randomize(); //randomizes the random number sequence
	int		m_Separator_inScreenCoord,m_HorizontalSeparator_inScreenCoord;
	double	m_VerticalRatioByDivider;
	void	RecalibratePanes();
	void	Auto_Fig_Settings(int howManyRows, int sizeOfOneRow, int howManyClmns, int sizeOfOneClmn, double scale) ;
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix(int RedArray[],int GreenArray[],int BlueArray[], int arraySize); 
	void	Show2D_II();
	void	OnMyMessage();
	double	Rectify(double Value){if(Value>0.)return Value; else return 0.;}
	CTASView * CTASDoc::GetView();
	void	TAS_TaskScheduler(CTASView *pView);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTASDoc)
	public:
	virtual BOOL OnNewDocument();
	//virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTASDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTASDoc)
	//}}AFX_MSG

public:
	afx_msg void OnSaveTrialsToFile();
	afx_msg void OnFileCloseWriting();
	afx_msg void OnUpdateOnSaveTrialsToFile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProceed(CCmdUI *pCmdUI);
	afx_msg void OnPause();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAS_VISDOC_H__4E7AB3C6_4618_11D3_8364_C9075A873F60__INCLUDED_)
