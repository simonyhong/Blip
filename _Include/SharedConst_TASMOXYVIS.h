#if !defined (SHARED_CNST_TAS_MOXY_VIS)
#define SHARED_CNST_TAS_MOXY_VIS

#define MAX_LEN_FILE_NAME 512
#define SIZE_OF_USER_DATA 1000
#define MAX_CHARLEN_SIZE_OF_USER_DATA 20
struct MoxyUserData{
	int m_NumUserData, m_TotNumUserVlaluesToRecord;
	char m_NameArray[SIZE_OF_USER_DATA][MAX_CHARLEN_SIZE_OF_USER_DATA];
	int m_SizeOfName[SIZE_OF_USER_DATA];
	int m_FirstLocationOfUserValue[SIZE_OF_USER_DATA];
	int m_NumOfValues[SIZE_OF_USER_DATA];
	double m_ValuesToRecord[SIZE_OF_USER_DATA*2];
	__int64 m_TimeOfOccurrence[SIZE_OF_USER_DATA];
};


//########### Trial info #########
#define MAX_NUM_ECODE 1000
//########### DIO #########
#define MaxDIO 24
#define MAX_STIM_OBJ 100
//#################  EYE monitor  ####################
#define	DEFAULT_EYE_ID_TO_MONITOR 0

struct PositionWindows{
	double cntrX,cntrY, width,hight;
	double LeftBoundary, UpperBoundary, RightBoundary, LowerBoundary;
	double Old_LeftBoundary, Old_UpperBoundary, Old_RightBoundary, Old_LowerBoundary;
	double angle;
	int m_OnOff;
}; 

//#################   Running-Line Display  #################################################
#define NUM_RL_PANELS   2
#define MAX_NUM_PENS_FOR_RUNNING_LINE 10
#define MAX_NUM_OF_ECODES_IN_ALIAS_TABLE	(MAX_NUM_PENS_FOR_RUNNING_LINE) //The number of ECodes that the user chose to be displayed as Events (in Event names & SDFs in different colors).

#define MAX_NUM_USER_PICKED_VARS 10 //The number of variables that the user chose to see in real time. These variables will be displayed as numbers and as the running line.
#define MAX_LEN_OF_VARIABLE_NAME 64
struct UserPickedVarToDisp{
	int		M_TypeOfVar;
	int		M_FigIdx;
	double	M_Value;
	double	M_PrvVarValue;
	char	M_VarInChar[MAX_LEN_OF_VARIABLE_NAME];
	int		M_DispPosition;
	int		M_ColorOrder;
	BOOL	M_Left0_Or_Right1_axis;
	double	M_MultiplyThisToDisplay;
	double	M_AddThisToDisplay;
	CPen	*M_pPen; 
};

//########### Mostly TAS #########
#define MAX_NUM_OF_FUNCTIONS 1000
#define SETS_OF_USER_VARIABLE_BLOCKS 2
//########### Display #########
#define SIZE_OF_DISP_REQUEST_QUEUE 100
#define	USE_LEFT_AXIS	0
#define	USE_RIGHT_AXIS  1
#define	SHOW_IN_RUNNING_LINE_DISP  1
#define	NO_SHOW_IN_RUNNING_LINE_DISP  0
#define	MAX_CHARS_OF_PIC_FILENAME	64

//###############     User Message    ####################
#define WM_MYMESSAGE_DISP_EVENT_4TAS			(WM_USER + 1)
#define WM_MYMESSAGE_NEW_TRIAL_TO_SAVE			(WM_USER + 2)
#define WM_MYMESSAGE_ANTIDROMIC_STIM_HAPPENED	(WM_USER + 3)
#define WM_MYMESSAGE_SOUND						(WM_USER + 4) //Just sound only
#define WM_MYMESSAGE_AUTO_DIO_ON_OFF			(WM_USER + 5)
#endif