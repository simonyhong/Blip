
//########  Properties of the visual object (Memory Map) ###########
	HANDLE hFile_M_cntrX;						double *M_cntrX; //1
	HANDLE hFile_M_cntrY;						double *M_cntrY; //2
	HANDLE hFile_M_width;						double *M_width; //3
	HANDLE hFile_M_hight;						double *M_hight; //4
	HANDLE hFile_M_angle;						double *M_angle; //5
	HANDLE hFile_M_Obj_Shape;					int *M_Obj_Shape; //6
	HANDLE hFile_M_rgb;							COLORREF *M_rgb;//7
	HANDLE hFile_M_Ojb_Switch;					int *M_Ojb_Switch;//8
	HANDLE hFile_M_PicFileName[MAX_STIM_OBJ];	char *M_PicFileName[MAX_STIM_OBJ];//9
	HANDLE hFile_M_PhotoDiodeFlash_PosX;		int *M_PhotoDiodeFlash_PosX;
	HANDLE hFile_M_PhotoDiodeFlash_PosY;		int *M_PhotoDiodeFlash_PosY;
	HANDLE hFile_M_PhotoDiodeFlash_PosWidth;	int *M_PhotoDiodeFlash_PosWidth;
	HANDLE hFile_M_PhotoDiodeFlash_PosHeight;	int *M_PhotoDiodeFlash_PosHeight;
	HANDLE hFile_M_backgroundScreenRGB;		COLORREF *M_backgroundScreenRGB;
	//######## Position Related ################################
	HANDLE	hFile_M_PosFigMinX;					double	*M_PosFigMinX; 
	HANDLE	hFile_M_PosFigMaxX;					double	*M_PosFigMaxX; 
	HANDLE	hFile_M_PosFigMinY;					double	*M_PosFigMinY; 
	HANDLE	hFile_M_PosFigMaxY;					double	*M_PosFigMaxY; 
	HANDLE	hFile_M_EyeX_rawData;				double	*M_EyeX_rawData; 
	HANDLE	hFile_M_EyeY_rawData;				double	*M_EyeY_rawData; 
	HANDLE	hFile_M_EyeX_Bias;					double	*M_EyeX_Bias; 
	HANDLE	hFile_M_EyeY_Bias;					double	*M_EyeY_Bias; 
	HANDLE	hFile_M_EyeX_Gain;					double	*M_EyeX_Gain; 
	HANDLE	hFile_M_EyeY_Gain;					double	*M_EyeY_Gain; 
	HANDLE	hFile_M_EyePosX;					double	*M_EyePosX; 
	HANDLE	hFile_M_EyePosY;					double	*M_EyePosY; 	
	HANDLE	hFile_M_EyeXsimulalted;				double	*M_EyeXsimulalted; 	
	HANDLE	hFile_M_EyeYsimulalted;				double	*M_EyeYsimulalted; 	
	#define MAX_EYE_WIND 10
	HANDLE	hFile_M_PositionWinds;	    PositionWindows *M_PositionWinds;
	HANDLE	hFile_M_JustClicked;					int *M_JustClicked; 
	HANDLE	hFile_M_WindToDisplayOrNot;				int *M_WindToDisplayOrNot; 
	HANDLE	hFile_M_BehavioralObjectForXYmonitor;	int *M_BehavioralObjectForXYmonitor; 
	//######## DIO LED ##########################################
	#define NUM_DIO_LEDS 24
	HANDLE hFile_M_DIO_State;					int *M_DIO_State;
	//######## Message between TAS, MOXY & VIS ################################
	HANDLE hFile_M_Is_VIS_Running;				int *M_Is_VIS_Running;
	HANDLE hFile_M_Is_TAS_Running_4MOXYVIS;		int *M_Is_TAS_Running_4MOXYVIS;
	HANDLE hFile_M_N;							int *M_N;
	HANDLE hFile_M_VisObjIDs;					int *M_VisObjIDs;
	HANDLE hFile_M_UserClickedVisObjIDs;		int *M_UserClickedVisObjIDs;
	HANDLE hFile_M_UserClickedN;				int *M_UserClickedN;
	HANDLE hFile_M_CLOSE_VIS;					int *M_CLOSE_VIS;
	#define MAX_LEN_OF_STR_MSG 100
	#define SIZE_OF_MSG_Q 50
	HANDLE hFile_M_MsgID_fromVIS_toTAS;			int *M_MsgID_fromVIS_toTAS;
	HANDLE hFile_M_MsgID_fromTAS_toVIS;			int *M_MsgID_fromTAS_toVIS;
	HANDLE hFile_M_MsgID_fromTAS_toMOXY;		int *M_MsgID_fromTAS_toMOXY;
	HANDLE hFile_M_MsgID_fromMOXY_toTAS;		int *M_MsgID_fromMOXY_toTAS;
	HANDLE hFile_M_Msg_from_MOXY_to_VIS;		int *M_Msg_from_MOXY_to_VIS;
	HANDLE hFile_M_StringMsg_fromTAS_toVIS;		char *M_StringMsg_fromTAS_toVIS;
	HANDLE hFile_M_StringMsg_fromVIS_toTAS;		char *M_StringMsg_fromVIS_toTAS;
	HANDLE hFile_M_StringMsg_fromTAS_toMOXY;	char *M_StringMsg_fromTAS_toMOXY;
	HANDLE hFile_M_StringMsg_fromMOXY_toTAS;	char *M_StringMsg_fromMOXY_toTAS;

#define MAX_VISIBLE_OBJs 10
	HANDLE hFile_M_IsVisibleObj;				BOOL *M_IsVisibleObj;
//########  MOXY to TAS  ##########################################
	HANDLE hFile_M_Dur_BigRWD;					int *M_Dur_BigRWD;
	HANDLE hFile_M_MoxyUserDataSetID;			int *M_MoxyUserDataSetID;
	HANDLE hFile_M_MoxyUserData;				MoxyUserData *M_MoxyUserData;
	//########  Ecode & alias  ##########################################
	#define MAX_CHAR_LENGTH_FOR_ECODE_ALIAS 32
	struct EcodeTable{
		int		m_EcodeItself;
		int		m_ColorOrder;
		char	M_EcodeAlias[MAX_CHAR_LENGTH_FOR_ECODE_ALIAS]; 
	};
	HANDLE	hFile_M_EcodeTable;							EcodeTable *M_EcodeTable;
	HANDLE	hFile_M_NumEcodesInTable;					int *M_NumEcodesInTable;
	HANDLE	hFile_M_IsTriggeredMode;					int *M_IsTriggeredMode;
	HANDLE	hFile_M_nTriggerECode;						int *M_nTriggerECode;
	HANDLE	hFile_M_hasTriggered;						int *M_hasTriggered;
	HANDLE	hFile_M_TriggeredTime_ms;				__int64 *M_TriggeredTime_ms;

	//########  File Names and Status  ##########################################
	HANDLE	hFile_M_PreFix;							char *M_PreFix;
	HANDLE	hFile_M_SuFFix;							char *M_SuFFix;
	HANDLE	hFile_M_BaseOfFileName;					char *M_BaseOfFileName;
	HANDLE	hFile_M_JustFileName;					char *M_JustFileName;
	HANDLE	hFile_M_isSaving;						int *M_isSaving;
	HANDLE	hFile_M_isRenamed;						int *M_isRenamed;
	//########  User Picked Variables  ##########################################
	HANDLE hFile_M_VarToDisp;							UserPickedVarToDisp *M_VarToDisp;
	HANDLE hFile_M_NumUserPickedVariables;				int *M_NumUserPickedVariables;
	HANDLE hFile_M_VarToDispLineEnd;					int *M_VarToDispLineEnd;
	HANDLE hFile_M_VarRECT;								CRect *M_VarRECT;
	
	//########  Message queue from TAS to MOXY
	HANDLE hFile_M_Msg_Q_Idx;							int *M_Msg_Q_Idx;
	HANDLE hFile_M_StrQ_fromTAS_toMOXY[SIZE_OF_MSG_Q];	char *M_StrQ_fromTAS_toMOXY[SIZE_OF_MSG_Q];
	HANDLE hFile_M_VisObjID_Q_fromTAS_toMOXY[SIZE_OF_MSG_Q];	int *M_VisObjID_Q_fromTAS_toMOXY[SIZE_OF_MSG_Q];
	HANDLE hFile_M_N_Q_fromTAS_toMOXY[SIZE_OF_MSG_Q];	int *M_N_Q_fromTAS_toMOXY[SIZE_OF_MSG_Q];


