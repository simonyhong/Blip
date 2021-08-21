// TAS_Doc.cpp : implementation of the CTASDoc class
//
#include "stdafx.h"
#include <math.h>
#include "TAS.h"
#include "Mmsystem.h" 

#include "Update_Me_TAS_Doc.h"
#include "Dialog_JustDoIt.h"
#include "TAS_View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTASDoc

IMPLEMENT_DYNCREATE(CTASDoc, CDocument)

BEGIN_MESSAGE_MAP(CTASDoc, CDocument)
	//{{AFX_MSG_MAP(CTASDoc)
	ON_COMMAND(ID_FILE_SAVE_TAS, &CTASDoc::OnSaveTrialsToFile)
	ON_COMMAND(ID_FILE_CLOSE_WRITING, &CTASDoc::OnFileCloseWriting)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_TAS, &CTASDoc::OnUpdateOnSaveTrialsToFile)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, &CTASDoc::OnUpdateProceed)
	ON_UPDATE_COMMAND_UI(ID_TASK_SCHEDULER, &CTASView::OnUpdateTaskON)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTASDoc construction/destruction

BOOL CTASDoc::m_IsPlayButtonON;


CTASDoc::CTASDoc()
{
	// TODO: add construction m_EventCode here

	int i,j;
	for(i=0;i<MAX_NUM_OF_FUNCTIONS;i++){	FunctionArray[i]=NULL;}
	m_nKey=0;
	m_bPause=1;
	m_IsPlayButtonON=0;
	m_dHowManyRows=3; m_dSizeOfOneRow=400; m_dHowManyClmns=3; m_dSizeOfOneClmn=600;
	m_BlownUpPaneID=-1;
	m_isInsidePane=0;
	m_DispEventIdx=0;
	m_isSaving=0;
	m_IsMemMap=0;
	sprintf(m_PreFix,"");sprintf(m_SuFFix,"");
	m_NnCounterArrowDrawn=0;
	m_RWDCounterArrowDrawn=0;
	m_PrvTopleftX=0, m_PrvTopleftY=0, m_EyeSquareWidth=6, m_EyeSquareHeight=6;
	m_MessageID=0; m_MessageID_toMOXY=0;
	
	m_bIsScrXYforIJ=0;
	/////////////// Parameters For Applications
	m_dVariable1=3;m_dVariable2=187;
	m_dVariable3=94;
	m_IsSeparatorHeld=0;
	m_IsHorizSeparatorHeld=0;
	m_VerticalRatioByDivider=0.7;
	m_isRunningLineSDF=0;
	m_isTAS_Running=0;

	for(i=0;i<MAX_NUM_OF_TIME_KEEPERS;i++){	m_TimerUsage[i]=0;}
	/////////////////
	m_futureVariable=50;
	m_prvDate=GetDateString();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalSection4Msg);
	InitializeCriticalSection(&m_CriticalSection4Serilize);

	m_isRtBUTTON_pressed=0;
	m_FirstTime0ElementErr=1;
	TimerRotator=0; //Get rid of this once the timer bug is fixed.

	tas_bFistTime=1;
	m_AI_Size_warning=1;
	if(CLOCKS_PER_SEC!=1000)AfxMessageBox("In the program CLOCKS_PER_SEC==1000 is assumed. Currently it is not. Please correct this.");
}

CTASDoc::~CTASDoc()
{
	m_IsPlayButtonON=0;// Not sure, setting this in Doc works. 
		
	//##### Ask the threads to terminate; not sure if it works. #####
	m_IsPlayButtonON=0; 
	for(int i=0; i<30; i++){
		if(m_isTAS_Running==0)break;
		Sleep(10);
	}
	m_isTAS_Running=0;
	DWORD ExitCode=0;
	TerminateThread(m_pTAS_Thread, ExitCode);
	//##########################################

	if(m_IsMemMap==1){//In the case TAS has not declared memory map, there is nothing to close.
		*M_isTAS_Running=0;  *M_Is_TAS_Running_4MOXYVIS=0;
		*M_MsgID_fromTAS_toVIS=0;
		*M_MsgID_fromVIS_toTAS=0;
		*M_MsgID_fromTAS_toMOXY=0;
		*M_MsgID_fromMOXY_toTAS=0;

		CloseMemoryMAP_4TASMOXYVIS();
		CloseTrialMemoryMAP();
		*M_NumTrlsInMAP=0;
		CloseSharedMemoryArea();
	}

	DeleteCriticalSection(&m_CriticalSection);
	DeleteCriticalSection(&m_CriticalSection4Msg);
	DeleteCriticalSection(&m_CriticalSection4Serilize);
}
UINT KILL_THREAD(LPVOID pParam) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) pParam;
			
	while(1){
		if(*(pDoc->M_Blip_OFF)==1){
			pDoc->m_IsPlayButtonON=1;
			int i;
			for(i=0; i<20;i++){
				Sleep(50); //Give some time for the RAS_Main() loop to close, in the case it is running.
				if(pDoc->m_isTAS_Running==0){
					break;
				}
			}
			Sleep(50); //Give some time for the RAS_Main() loop to close, in the case it is running.
			AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
			break;
		}
		Sleep(20);
	}
	return 0;
}

BOOL CTASDoc::OnNewDocument()
{
	if(!CDocument::OnNewDocument())return FALSE;

	if(tas_bFistTime==1){
		POSITION pos =GetFirstViewPosition();
		m_pView=(CTASView*)GetNextView(pos);
		m_pView->m_pDoc=this; 
		Sleep(200);//Just in case TASVIEW has some stuff to do.

		// Shared memory area with DAS
		CreateSharedMemoryArea();

		AfxBeginThread( KILL_THREAD, this); //This is running just to detect the user's commend of "close all Blip windows". This thread is needed to close RAS even when it is Stopped. There must be a better way to do this rather than running a thread all the time. 
		m_pView->OnTaskScheduler();
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CTASDoc diagnostics

#ifdef _DEBUG
void CTASDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTASDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

UINT LOOP_THREAD(LPVOID pParam) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) pParam;
	pDoc->Loop_Thread() ;
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CTASDoc commands
/////////////////////////////////////////////////////////////////////////////////////////////

// Simon says: 
// I may upgrade Blip. The only part you need to upgrade will be: 
//		(1) Update_Me_TAS_Doc.cpp, (2) Update_Me_TAS_Doc.h, (3) Update_Me folder.
// For this reason, I advise not to mass up with these "Update_Me" files, unless you are willing to do things yourself, which is welcome.
// Moreover, it may be a good idea to make your own utility functions in your CTASView class.
void CTASDoc::TAS()
{
	m_isTAS_Running=1; //Start of the thread
	//################
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);// REALTIME_PRIORITY_CLASS (may make the Windows freeze), HIGH_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS 
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);// THREAD_PRIORITY_TIME_CRITICAL  THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_NORMAL  THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_LOWEST  THREAD_PRIORITY_IDLE 
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_CPUfrq);//The frequency of the processor (Tom says it's a one time measure at the booting of the computer)

	if(tas_bFistTime==1){
		//CreateSharedMemoryArea(); //Done in OnNewDocument()
		// Shared memory area for TAS/VIS
		CreateSharedMemoryArea_4TASVIS();
		if(*M_isTAS_Running==1){
			Dialog_JustDoIt JustDoItBox;
			if(JustDoItBox.DoModal()==IDCANCEL) exit(1);
		}

		for(int i=0; i<MAX_DO_NUM-1;i++){M_DO_Array[i]=RELAY_OFF;}//The eighth one is for the antidromic stimulation
		M_DO_Array[DO_STIM_TRIGGER_CHAN]=0;//The channel of index 8 is for the stimulation, which is not via the RELAY in my (Simon's) system.

		//TAS needs to run even without DAS-server. Because this the shared variables need to be treated more carefully.
		if(*M_isDAS_Running!=1)for(int i=0; i<MAX_AI_CHAN;i++){*M_usableAiIdx[i]=-1;}

		m_IsMemMap=1; //now the memory map has been declared.
		*M_NnNumberToRecord=1;
		Give_Prefix_Suffix_forFileName("","");// Incase the user does not give any prefix or sufix for the file to save, the default file name, created by this function here, will be used.
		*M_BehavioralObjectForXYmonitor=0;//In case the user does not explicitly specify the channel to display the XY-monitor.

		if(*M_isMOXY_Running==0){Launch_MOXY();}
		if(*M_Is_VIS_Running==0){Launch_VIS();}

		//Event handlers
		while(*M_isDAS_Running!=1){if(m_isTAS_Running==0)return; Sleep(100);}//Without this, the IO does not work properly; don't know why.
		*M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation=OpenEvent(EVENT_ALL_ACCESS,TRUE,"EVENT_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation");
		char EvName[64]; 	
		for(int i=0; i<MAX_DO_NUM; i++){
			sprintf(EvName,EventObjectsName,i);
			m_M_DO_Active[i] =OpenEvent(EVENT_ALL_ACCESS,TRUE,EvName);
			if(m_M_DO_Active[i]==NULL)AfxMessageBox("m_M_DO_Active[i]==NULL");
		}
		m_M_DI_Active =OpenEvent(EVENT_ALL_ACCESS,TRUE,Event_DI_ObjectsName);

		tas_bFistTime=0;
	}

	*M_NumUserPickedVariables=0;
	AfxBeginThread(LOOP_THREAD, this);

	//*M_TrialID=0;
	*M_usableEcodeIdx=-1; //Not availabe initially.
	*M_usableTrialIdxInMem=-1;
	*M_numEcodes=0;
	(*M_NumTrlsInMAP)=0;
	*M_isThereNewTrial=0;//Nothing to save for now
	for(int i=0; i<MAX_STIM_OBJ;i++)M_Ojb_Switch[i]=0; //Turn off all visual objects
	*M_Dur_BigRWD=200; Inform_MOXY_withoutHandshaking("UpdateRwdRect");//Ask MOXY to update the reward amount display.

	//#######################################
	*M_Msg_Q_Idx=0;
	*M_MsgID_fromVIS_toTAS=0;
	*M_MsgID_fromTAS_toVIS=0;
	*M_MsgID_fromMOXY_toTAS=0;
	*M_MsgID_fromTAS_toMOXY=0;
	*M_isTAS_Running=1; *M_Is_TAS_Running_4MOXYVIS=1; 

	timeBeginPeriod(1); //This one makes the Sleep() function accurate
	while(m_IsPlayButtonON && *M_Blip_OFF==0){
		//  Some essential initializations to be done (you can modify) 
		m_TAS_trialID_tobeSaved=0; m_UserGave_TrlID=0;
		m_Header="";//Clear the header
		m_TimeOfTrialStart=*M_LatestTimeStampInMicroS; *M_numEcodes=0; //This is to suppress the warning at the begining of the trial where "EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell()" calls At_the_End_of_a_Trial_Check_if_MemoryMap_config_meets_the_trials_config();
		*M_isThereNewTrial=0; //Nothing to save yet.
		(*M_MoxyUserDataSetID)=0; 
		m_isSaving=0; *M_isSaving=0;//Not saving
		m_firsVIS_ErrorMsg=1;//By setting this, the TAS gives a warning when VIS is not responding. It does onely once when the flag "m_firsVIS_ErrorMsg" is 1. It turns off right after the error message.
		m_firsMOXY_ErrorMsg=1;//By setting this, the TAS gives a warning when MOXY is not responding. It does onely once when the flag "m_firsMOXY_ErrorMsg" is 1. It turns off right after the error message.
		//m_bPause=1; //Stop!
		init_IdleTimers_to0();
		Iitialize_UserPicked_Variables_to_Display();//When a new task starts, do this to initialize the user-picked variables to display.
		Iitialize_ECode_As_Word();//When a new task starts, do this to initialize the aliases (alternet names of ECodes) to accept task-wide aliases.
		
		//##########  Run the task ###########################
		//##########  Run the task ###########################
		if(FunctionArray[m_TaskType]!=NULL){
			(*m_pView.*FunctionArray[m_TaskType])();
		}
		else{ //Error
			AfxMessageBox("Simon says: The function is not well assigned. Please check your code.");
			break;
		}
		//####################################################
		//####################################################

		Sleep(50);//Let's give some time for the previous program to turn off stuff, befor begining a new one. It may not be necessary.
		if(!m_IsPlayButtonON)break; //This needs to be somewhere in the loop to terminate the thread in the case the program is shutting down.
	}
	timeEndPeriod(1);//match this with 	timeBeginPeriod(1) above 

	m_isSaving=0; *M_isSaving=0;//Not saving
	*M_MsgID_fromTAS_toVIS=0;
	*M_MsgID_fromVIS_toTAS=0;
	*M_MsgID_fromTAS_toMOXY=0;
	*M_MsgID_fromMOXY_toTAS=0;
	*M_isTAS_Running=0; *M_Is_TAS_Running_4MOXYVIS=0;
	//#######################################
	 m_isTAS_Running=0; //End of the thread
}

void CTASDoc::Loop_Thread()
{	
	int i;
	while(1){
		Sleep(1);
		if(!m_IsPlayButtonON)break;
		//The user-picked variables
		for(i=0;i<*M_NumUserPickedVariables;i++){
			if(     M_VarToDisp[i].M_TypeOfVar==0)M_VarToDisp[i].M_Value=(double)(*(m_UserPickedVar[i].m_pUnsignedCharValue));
			else if(M_VarToDisp[i].M_TypeOfVar==1)M_VarToDisp[i].M_Value=(double)(*(m_UserPickedVar[i].m_pIntValue));
			else if(M_VarToDisp[i].M_TypeOfVar==2)M_VarToDisp[i].M_Value=(double)(*(m_UserPickedVar[i].m_pFloatValue));
			else                                  M_VarToDisp[i].M_Value=        (*(m_UserPickedVar[i].m_pDoubleValue));
		}
	}
}

void CTASDoc::RootFileOpen()
{
	if(tas_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\Config_TAS.txt",m_FileNameFULL);
		sprintf(m_JustFileName,"Config_TAS.txt");
	}
	else if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){return;}

	FILE *fp;
	if((fp=fopen(m_FileNameFULL,"r"))!= NULL){
		fscanf(fp,"%lf",&m_futureVariable );
	}
}

void CTASDoc::Launch_MOXY()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];

	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\MOXY\\x64\\Release\\MOXY.exe", DirStr);
	#else _M_X86  // x32
	Find_Blip_andAddSome("\\MOXY\\Release\\MOXY.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CTASDoc::Launch_VIS()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];

	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\VIS\\x64\\Release\\VIS.exe", DirStr);
	#else _M_X86  // x32
	Find_Blip_andAddSome("\\VIS\\Release\\VIS.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}


void CTASDoc::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
{
	char tmpStr[1024];
	CString ExeDirString=GetExecutableName();
	strcpy(tmpStr,ExeDirString);
	if(wildcmp("*\Blip*",tmpStr)==1){
		RE_Search:;
		if(wildcmp("*\Blip",tmpStr)==1){
			sprintf(FullPath,"%s",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search;}
		strcat(FullPath,AddedFolderName);
	}		
	else AfxMessageBox("Couldn't find Blip folder.");
}


int CTASDoc::GetBigRWD()
{
	return *M_Dur_BigRWD;
}
void CTASDoc::SetBigRWD(int RwdDuration)
{
	*M_Dur_BigRWD=RwdDuration;
	Inform_MOXY_withoutHandshaking("UpdateRwdRect");
}
int CTASDoc::GetTaskType()
{
	return m_TaskType;
}
void CTASDoc::SetTaskType(int task)
{
	m_TaskType=task;
}

//The following function is a thread running in parallel with the reward and more. 
//It terminates either 1) when the eye leaves the window (WindID), which triggers an ECode (pView->ECode(.)), or 2) when the specified time-limit (dur_Wait+dur_variWait) has reached. 
UINT DETECT_ETE_OUT_OF_TGT_MOMENT(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	if(pDoc->WaitUntilEyeIsOut(pDoc->m_WindID_THREAD,pDoc->m_EyeID_THREAD, pDoc->m_dur_Wait_THREAD, pDoc->m_dur_variWait_THREAD)){
		pDoc->ECode(EYE_OUT_OF_TGT);   //CTASDoc uses negative integers for internal ECodes.
	}
	else{
		pDoc->ECode(EYE_STILL_ON_TGT); //CTASDoc uses negative integers for internal ECodes.
	}
	return 0;
}

void CTASDoc::Monitor_EyeOutOfBox_thread(int WindID,int EyeID, int dur_Wait,int dur_variWait)
{
	m_WindID_THREAD=WindID; m_EyeID_THREAD=EyeID, m_dur_Wait_THREAD=dur_Wait; m_dur_variWait_THREAD=dur_variWait; 
	AfxBeginThread(DETECT_ETE_OUT_OF_TGT_MOMENT, this);  
}

//If an event has occured with an ECode, it will be stored.
//In case the ECode is the triggering ECode for the RL, the RL will be notified.
void CTASDoc::ECode(int ecode)
{	
	int index=*M_usableEcodeIdx+1;
	if(index>=*M_Ecode_QueueSize)index=0;
	M_EventCodes[index]=ecode;				
	M_EventTimes[index]=*M_LatestTimeStampInMicroS;
	*M_usableEcodeIdx=index; //Update the index after finishing all the adjustment.
	(*M_numEcodes)++;
	for(int i=0;i<NUM_RL_PANELS;i++){
		if(M_IsTriggeredMode[i]==1 && M_nTriggerECode[i]==ecode){M_hasTriggered[i]=1; M_TriggeredTime_ms[i]=0.001*M_EventTimes[index];}
	}
}

//If an event has occured with an ECode, it will be stored.
//In case the ECode is the triggering ECode for the RL, the RL will be notified.
void CTASDoc::ECode_with_Time(int ecode, __int64 Time)
{	
	int index=*M_usableEcodeIdx+1;
	if(index>=*M_Ecode_QueueSize)index=0;
	M_EventCodes[index]=ecode;				
	M_EventTimes[index]=Time;
	*M_usableEcodeIdx=index; //Update the index after finishing all the adjustment.
	(*M_numEcodes)++;
	for(int i=0;i<NUM_RL_PANELS;i++){
		if(M_IsTriggeredMode[i]==1 && M_nTriggerECode[i]==ecode){M_hasTriggered[i]=1; M_TriggeredTime_ms[i]=0.001*M_EventTimes[index];}
	}
}

//#####  Start a new trial
void CTASDoc::StartCollectingTrialData_Give_StartECode(int ecode)
{

	//Start Time of new trial 
	m_TimeOfTrialStart=*M_LatestTimeStampInMicroS;
	//The user has not assigned the trial ID. 
	m_UserGave_TrlID=0; 
	//ECode
	*M_numEcodes=0;//This line needs to come BEFORE "ECode(ecode)" line
	ECode(ecode);
	*M_FirstEcodeIdxOfTrial=*M_usableEcodeIdx;//This line needs to come AFTER "ECode(ecode)" line
	//User defined values
	(*M_MoxyUserDataSetID)=!(*M_MoxyUserDataSetID);
	m_UserData[*M_MoxyUserDataSetID].m_NumUserData=0; //This line resets the parameters of the user data (created by Name_and_Value_to_record() or Name_and_ArrayOfValues_to_record() ). 
	m_UserData[(*M_MoxyUserDataSetID)].m_TotNumUserVlaluesToRecord=0; 
	m_UserData[(*M_MoxyUserDataSetID)].m_FirstLocationOfUserValue[0]=m_UserData[(*M_MoxyUserDataSetID)].m_TotNumUserVlaluesToRecord;
	M_MoxyUserData[*M_MoxyUserDataSetID].m_NumUserData=0; //This line resets the parameters of the user data made by MOXY.
	M_MoxyUserData[(*M_MoxyUserDataSetID)].m_TotNumUserVlaluesToRecord=0; 
	M_MoxyUserData[(*M_MoxyUserDataSetID)].m_FirstLocationOfUserValue[0]=M_MoxyUserData[(*M_MoxyUserDataSetID)].m_TotNumUserVlaluesToRecord;
	//Behavior AI. 
	*M_firstBehaviorAiIdxInTrial=*M_usableAiIdx[FIRST_2D_BHVR_AI_CH_IDX];
	//Spike Data info. 
	for(int chan=0;chan<*M_numNeuralChannels;chan++){
		*M_firstSpkIdxInTrialOfNn1[chan]=qIdxSpike(*M_usableSpikeIdxOfNn1[chan]+1);
		*M_firstSpkIdxInTrialOfNn2[chan]=qIdxSpike(*M_usableSpikeIdxOfNn2[chan]+1);
		*M_firstSpkIdxInTrialOfNn3[chan]=qIdxSpike(*M_usableSpikeIdxOfNn3[chan]+1);
		*M_numSpikesOfNn1[chan]=0; *M_numSpikesOfNn2[chan]=0; *M_numSpikesOfNn3[chan]=0;
	}
	//This new trial needs to be saved
	*M_isThereNewTrial=1;
}
int CTASDoc::qIdxSpike(int i)
{
	if(i>=0 && i<MAX_SPIKEs) return i;
	else if(i<0) return (MAX_SPIKEs+i);// => This happens when the program tries to retrieve the pre-threshold-crossing portion of the wave.
	else return (i-MAX_SPIKEs);//if(i>MAX_SPIKEs-1)
}
void CTASDoc::Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], unsigned char &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay)
{	
	if((*M_NumUserPickedVariables)-1>=MAX_NUM_USER_PICKED_VARS){AfxMessageBox("Too many variables to display. Check: MAX_NUM_USER_PICKED_VARS"); return;};
	m_UserPickedVar[(*M_NumUserPickedVariables)].m_pUnsignedCharValue=&pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_TypeOfVar=0;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_Value=pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_PrvVarValue=pVariable;
	Register_a_variable_to_display(NickName, FigIdx, Left0_Or_Right1_axis,IsToShowRunningLine, MultiplyThisToDisplay, AddThisToDisplay);
}
void CTASDoc::Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], int &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay)
{	
	if((*M_NumUserPickedVariables)-1>=MAX_NUM_USER_PICKED_VARS){AfxMessageBox("Too many variables to display. Check: MAX_NUM_USER_PICKED_VARS"); return;};
	m_UserPickedVar[(*M_NumUserPickedVariables)].m_pIntValue=&pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_TypeOfVar=1;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_Value=pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_PrvVarValue=pVariable;
	Register_a_variable_to_display(NickName, FigIdx, Left0_Or_Right1_axis,IsToShowRunningLine, MultiplyThisToDisplay, AddThisToDisplay);
}
void CTASDoc::Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], float &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay)
{	
	if((*M_NumUserPickedVariables)-1>=MAX_NUM_USER_PICKED_VARS){AfxMessageBox("Too many variables to display. Check: MAX_NUM_USER_PICKED_VARS"); return;};
	m_UserPickedVar[(*M_NumUserPickedVariables)].m_pFloatValue=&pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_TypeOfVar=2;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_Value=pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_PrvVarValue=pVariable;
	Register_a_variable_to_display(NickName, FigIdx, Left0_Or_Right1_axis,IsToShowRunningLine, MultiplyThisToDisplay, AddThisToDisplay);
}
void CTASDoc::Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis(char NickName[], double &pVariable, int  FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay)
{	
	if((*M_NumUserPickedVariables)-1>=MAX_NUM_USER_PICKED_VARS){AfxMessageBox("Too many variables to display. Check: MAX_NUM_USER_PICKED_VARS"); return;};
	m_UserPickedVar[(*M_NumUserPickedVariables)].m_pDoubleValue=&pVariable;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_TypeOfVar=3;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_PrvVarValue=pVariable;
	Register_a_variable_to_display(NickName, FigIdx, Left0_Or_Right1_axis,IsToShowRunningLine, MultiplyThisToDisplay, AddThisToDisplay);
}
void CTASDoc::Register_a_variable_to_display(char NickName[], int FigIdx, int Left0_Or_Right1_axis, int IsToShowRunningLine,  double MultiplyThisToDisplay, double AddThisToDisplay)
{	
	if(strlen(NickName)>=MAX_LEN_OF_VARIABLE_NAME){AfxMessageBox("Too long name for the variable. Check: MAX_LEN_OF_VARIABLE_NAME"); return;};
	strcpy(M_VarToDisp[(*M_NumUserPickedVariables)].M_VarInChar,NickName);
	M_VarToDisp[(*M_NumUserPickedVariables)].M_FigIdx=FigIdx;
	if(IsToShowRunningLine!=0)M_VarToDisp[(*M_NumUserPickedVariables)].M_ColorOrder=(*M_NumUserPickedVariables)%MAX_NUM_PENS_FOR_RUNNING_LINE;
	else M_VarToDisp[(*M_NumUserPickedVariables)].M_ColorOrder=-1;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_Left0_Or_Right1_axis=Left0_Or_Right1_axis;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_MultiplyThisToDisplay=MultiplyThisToDisplay;
	M_VarToDisp[(*M_NumUserPickedVariables)].M_AddThisToDisplay=AddThisToDisplay;
	(*M_NumUserPickedVariables)++;

	//Inform MOXY to update
	Inform_MOXY_withoutHandshaking("AddUserVar");
}


void CTASDoc::Iitialize_UserPicked_Variables_to_Display()
{	
	(*M_NumUserPickedVariables)=0;

	//Inform MOXY to update
	Inform_MOXY_withoutHandshaking("InitUserVar");
}

void CTASDoc::Randomize()
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
}

void CTASDoc::Give_ObjID_and_NameOfPic(int ObjID, char Name[])
{	
	if(ObjID<0||ObjID>=MAX_STIM_OBJ){AfxMessageBox("ObjID<0||ObjID>=MAX_STIM_OBJ"); return;}
	strcpy(M_PicFileName[ObjID],Name); 
}

void CTASDoc::Iitialize_ECode_As_Word()
{
	(*M_NumEcodesInTable)=0;

}

//The recommanded number of aliased ecodes is about "MAX_NUM_OF_ECODES_IN_ALIAS_TABLE"
//Alias is supported by the Trigger status display.
void CTASDoc::ECode_as_Word(int ECode, char *Word)
{
	if(strlen(Word)>MAX_CHAR_LENGTH_FOR_ECODE_ALIAS-1){AfxMessageBox("The alias name of the given ECode is too long. Check: MAX_CHAR_LENGTH_FOR_ECODE_ALIAS"); return;}

	if((*M_NumEcodesInTable)<MAX_NUM_OF_ECODES_IN_ALIAS_TABLE){
		M_EcodeTable[(*M_NumEcodesInTable)].m_EcodeItself=ECode;
		strcpy(&M_EcodeTable[(*M_NumEcodesInTable)].M_EcodeAlias[0],Word);
		(*M_NumEcodesInTable)++;
	}
	else{AfxMessageBox("Simon says: You are trying to dispaly too many Ecodes. Check: MAX_NUM_OF_ECODES_IN_ALIAS_TABLE");}
}


void CTASDoc::OnPause() 
{
	m_bPause=!m_bPause;
}

void CTASDoc::OnUpdateProceed(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bPause==1);
}


//#############################################################################################################################################################
//##########################################################   Timing related   ###################################################################################################
//####  !!!!  Do not use either "clock()" or "GetTickCount()" to measure time  !!!  ###########
// Sleep(t) function is accurate: the actual idling time = t+-1ms. For example Sleep(20) would result in 19.9~20.99ms of sleep. 

//Idling function
void CTASDoc::SleepAccurate(int FixedDur, int variableDur)
{
	if(FixedDur<0){AfxMessageBox("FixedDur<0");return;}
	__int64 newVal,lastVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastVal);
	unsigned VariablePart=0, totDelay;
	if(variableDur>0)VariablePart=Random(variableDur);
	totDelay=FixedDur+VariablePart; if(totDelay<1.)return;
	if(totDelay>=ACCURACY_OF_SLEEP_PLUS1){
		while(1){
			Sleep(1);
			if(totDelay-HighPerformanceTime_inMs(lastVal)<ACCURACY_OF_SLEEP)break;
			if(!m_IsPlayButtonON)return;
		}
	}
	//Fine delay
	while(1){
		if(HighPerformanceTime_inMs(lastVal)>totDelay+0.002)break;
		if(!m_IsPlayButtonON)return;
	}
}

//Idling function
void CTASDoc::Sleep_NOT_Accurate(int FixedDur, int variableDur)
{
	if(FixedDur<0){/*AfxMessageBox("FixedDur<0");*/return;}
	Sleep(FixedDur);
	Sleep(Random(variableDur));
}
//Idling function
void CTASDoc::SleepAccurate_1ms()
{
	__int64 lastVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastVal);
	while(1){
		if(HighPerformanceTime_inMs(lastVal)>1.002)break;
		if(!m_IsPlayButtonON)return;
	}
}


//DO NOT use this one without going through IDLE_FUNCTION_0,1,2... threads.
//The TimerID *MUST* have been chosen automatically using the FindAvailableTimer() function. 
//When you use the m_durOfIdle[TimerID], this function needs to be used becaue of "if(m_durOfIdle[TimerID]==0)return;"
void CTASDoc::SleepAccurate_with_TIMER(int TimerID, int variableDur)
{
	__int64 newVal,lastVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastVal);
	unsigned VariablePart=0, totDelay;
	if(variableDur>0)VariablePart=Random(variableDur);
	totDelay=m_durOfIdle[TimerID]+VariablePart; if(totDelay<1.)return;
	if(totDelay>=3){
		while(1){
			Sleep(1);
			if(totDelay-HighPerformanceTime_inMs(lastVal)<2)break;
			if(!m_IsPlayButtonON)return;
			if(m_durOfIdle[TimerID]==0)return;//The mother function of this thread wants the idling function to terminate.
		}
	}
	//Fine delay
	while(1){
		if(HighPerformanceTime_inMs(lastVal)>totDelay+0.001)break;
		if(!m_IsPlayButtonON)return;
		if(m_durOfIdle[TimerID]==0)return;//The mother function of this thread wants the idling function to terminate.
	}
}

double CTASDoc::HighPerformanceTime_inMs(__int64 lastVal)
{
	__int64 newVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&newVal);
	return (1000.0 * (double)(newVal-lastVal)/ m_CPUfrq);
}


void CTASDoc::init_IdleTimers_to0()
{
	for(int i=0;i<MaxNumTimer;i++){m_durOfIdle[i]=0;}
}

UINT IDLE_FUNCTION_0(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(0, -1);
	pDoc->m_durOfIdle[0]=0;
	return 0;
}

UINT IDLE_FUNCTION_1(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(1, -1);
	pDoc->m_durOfIdle[1]=0;
	return 0;
}

UINT IDLE_FUNCTION_2(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(2, -1);
	pDoc->m_durOfIdle[2]=0;
	return 0;
}

UINT IDLE_FUNCTION_3(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(3, -1);
	pDoc->m_durOfIdle[3]=0;
	return 0;
}

UINT IDLE_FUNCTION_4(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(4, -1);
	pDoc->m_durOfIdle[4]=0;
	return 0;
}

UINT IDLE_FUNCTION_5(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(5, -1);
	pDoc->m_durOfIdle[5]=0;
	return 0;
}

UINT IDLE_FUNCTION_6(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(6, -1);
	pDoc->m_durOfIdle[6]=0;
	return 0;
}

UINT IDLE_FUNCTION_7(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(7, -1);
	pDoc->m_durOfIdle[7]=0;
	return 0;
}

UINT IDLE_FUNCTION_8(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(8, -1);
	pDoc->m_durOfIdle[8]=0;
	return 0;
}

UINT IDLE_FUNCTION_9(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(9, -1);
	pDoc->m_durOfIdle[9]=0;
	return 0;
}

UINT IDLE_FUNCTION_10(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(10, -1);
	pDoc->m_durOfIdle[10]=0;
	return 0;
}

UINT IDLE_FUNCTION_11(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(11, -1);
	pDoc->m_durOfIdle[11]=0;
	return 0;
}

UINT IDLE_FUNCTION_12(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(12, -1);
	pDoc->m_durOfIdle[12]=0;
	return 0;
}

UINT IDLE_FUNCTION_13(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(13, -1);
	pDoc->m_durOfIdle[13]=0;
	return 0;
}

UINT IDLE_FUNCTION_14(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(14, -1);
	pDoc->m_durOfIdle[14]=0;
	return 0;
}

UINT IDLE_FUNCTION_15(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(15, -1);
	pDoc->m_durOfIdle[15]=0;
	return 0;
}

UINT IDLE_FUNCTION_16(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(16, -1);
	pDoc->m_durOfIdle[16]=0;
	return 0;
}

UINT IDLE_FUNCTION_17(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(17, -1);
	pDoc->m_durOfIdle[17]=0;
	return 0;
}

UINT IDLE_FUNCTION_18(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(18, -1);
	pDoc->m_durOfIdle[18]=0;
	return 0;
}

UINT IDLE_FUNCTION_19(LPVOID WinObjPtr) //A global Function
{
	CTASDoc* pDoc=(CTASDoc*) WinObjPtr;
	pDoc->SleepAccurate_with_TIMER(19, -1);
	pDoc->m_durOfIdle[19]=0;
	return 0;
}


//#############################################################################################################################################################
//##########################################################   Eye Positions ###################################################################################################
//Sets the position of the CENTER of the window
void CTASDoc::positionBox_pos(int WindID, double X, double Y)
{
	M_PositionWinds[WindID].cntrX=X;
	M_PositionWinds[WindID].cntrY=Y;
}

//Sets the sizes of the window
void CTASDoc::positionBox_size(int WindID, double width, double hight)
{
	M_PositionWinds[WindID].width=width;
	M_PositionWinds[WindID].hight=hight;
	//This is for the eye window on the TaskScheduler side
	M_PositionWinds[WindID].LeftBoundary =M_PositionWinds[WindID].cntrX-width/2.; 
	M_PositionWinds[WindID].RightBoundary=M_PositionWinds[WindID].cntrX+width/2.;
	M_PositionWinds[WindID].UpperBoundary=M_PositionWinds[WindID].cntrY+hight/2.;
	M_PositionWinds[WindID].LowerBoundary=M_PositionWinds[WindID].cntrY-hight/2.;
}

//######################  Check if the eye is in or out of a box ###############
//######################  Check if the eye is in or out of a box ###############
int CTASDoc::WaitUntilEyeIsIn(int WindID,int EyeID, int Dur_Wait,int dur_variWait)
{
	return WaitUntilEyeIsInOrOut(-1, WindID, EyeID, Dur_Wait, dur_variWait);
}
int CTASDoc::WaitUntilEyeIsOut(int WindID,int EyeID, int Dur_Wait,int dur_variWait)
{
	return WaitUntilEyeIsInOrOut( 1, WindID, EyeID, Dur_Wait, dur_variWait);
}

UINT IDLE_FUNCTION_0(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_1(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_2(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_3(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_4(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_5(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_6(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_7(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_8(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_9(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_10(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_11(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_12(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_13(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_14(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_15(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_16(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_17(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_18(LPVOID WinObjPtr); //A global Function
UINT IDLE_FUNCTION_19(LPVOID WinObjPtr); //A global Function

//Choose an unused Timer automatically. For this it uses a "Critical Section" 
//Be careful with m_TimerUsage[.] and m_durOfIdle[.]
int CTASDoc::FindAvailableTimer()
{
	int timerToUse=-1;
	//############  Critical Section  #############
	EnterCriticalSection(&m_CriticalSection);
	for(int i=0;i<MAX_NUM_OF_TIME_KEEPERS;i++){
		if(i==TimerRotator)continue; //TimerRotator is used because of a misterious bug (likely a of visual studio 2010). 
		if(m_TimerUsage[i]==0){
			if(m_durOfIdle[i]!=0){
				AfxMessageBox("m_durOfIdle[i]!=0");//The timers need to be 0 at the start of idling to make sure that the previous thread using the timer has ended, before launching another thread using the same timer. 
			}
			m_TimerUsage[i]=1;//
			timerToUse=i;
			TimerRotator=i; //Get rid of this once the timer bug is fixed.
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalSection);
	//#############################################
	return timerToUse;
}
int CTASDoc::WaitUntilEyeIsInOrOut(int InOrOut, int WindID,int EyeID, int dur_Wait,int dur_variWait)
{
	if(dur_Wait<0){
		AfxMessageBox("dur_Wait cannot be negative"); return 0;
	}
	//Time keeping
	int TimeKeeperID=FindAvailableTimer();//TimeKeeperID MUST be obtained using FindAvailableTimer() function.
	if(TimeKeeperID==-1){AfxMessageBox("There is no available Timer left."); return -10000;}
	int VariablePart=0;
	if(dur_variWait>0)VariablePart=Random(dur_variWait);
	if(m_durOfIdle[TimeKeeperID]!=0){
		AfxMessageBox("m_durOfIdle[TimeKeeperID]!=0");//The timers need to be 0 at the start of idling to make sure that the previous thread using the timer has ended, before launching another thread using the same timer. 
	}
	m_durOfIdle[TimeKeeperID]=dur_Wait+VariablePart;
	switch(TimeKeeperID){
		case 0:	AfxBeginThread( IDLE_FUNCTION_0,  this); break;
		case 1:	AfxBeginThread( IDLE_FUNCTION_1,  this); break;
		case 2:	AfxBeginThread( IDLE_FUNCTION_2,  this); break;
		case 3:	AfxBeginThread( IDLE_FUNCTION_3,  this); break;
		case 4:	AfxBeginThread( IDLE_FUNCTION_4,  this); break;
		case 5:	AfxBeginThread( IDLE_FUNCTION_5,  this); break;
		case 6:	AfxBeginThread( IDLE_FUNCTION_6,  this); break;
		case 7:	AfxBeginThread( IDLE_FUNCTION_7,  this); break;
		case 8:	AfxBeginThread( IDLE_FUNCTION_8,  this); break;
		case 9:	AfxBeginThread( IDLE_FUNCTION_9,  this); break;
		case 10:AfxBeginThread( IDLE_FUNCTION_10, this); break;
		case 11:AfxBeginThread( IDLE_FUNCTION_11, this); break;
		case 12:AfxBeginThread( IDLE_FUNCTION_12, this); break;
		case 13:AfxBeginThread( IDLE_FUNCTION_13, this); break;
		case 14:AfxBeginThread( IDLE_FUNCTION_14, this); break;
		case 15:AfxBeginThread( IDLE_FUNCTION_15, this); break;
		case 16:AfxBeginThread( IDLE_FUNCTION_16, this); break;
		case 17:AfxBeginThread( IDLE_FUNCTION_17, this); break;
		case 18:AfxBeginThread( IDLE_FUNCTION_18, this); break;
		case 19:AfxBeginThread( IDLE_FUNCTION_19, this); break;
	}

	//Check the eye position
	float LeftBoundary, UpperBoundary, RightBoundary, LowerBoundary;
	LeftBoundary=M_PositionWinds[WindID].LeftBoundary; UpperBoundary=M_PositionWinds[WindID].UpperBoundary; RightBoundary=M_PositionWinds[WindID].RightBoundary; LowerBoundary=M_PositionWinds[WindID].LowerBoundary;
	int EyeInOrOut=0;
	int AiChanID=FIRST_2D_BHVR_AI_CH_IDX+EyeID*2;

	do{
		if((*M_JustClicked)==1 ||*M_usableAiIdx[AiChanID]<0 ){//The user just clicked the screen. The cliking is valid for the rest of the trial.
			//Sleep(1);//==>Do not delete this, until the problem of "release" version of the program not detecting the user-click is resolved.
			if(M_EyeXsimulalted[EyeID]>LeftBoundary && M_EyeXsimulalted[EyeID]<RightBoundary && M_EyeYsimulalted[EyeID]>LowerBoundary && M_EyeYsimulalted[EyeID]<UpperBoundary){
				if(InOrOut==-1){EyeInOrOut=1; break;}//The eye is inside of the window
			}
			else{
				if(InOrOut== 1){EyeInOrOut=1; break;}//The eye is outside of the window
			}
		}
		else{//Real data
			//Get the eye position first
			Get_Latest_XYpos();
			if(M_EyeX_rawData[EyeID]>LeftBoundary && M_EyeX_rawData[EyeID]<RightBoundary && M_EyeY_rawData[EyeID]>LowerBoundary && M_EyeY_rawData[EyeID]<UpperBoundary){
				if(InOrOut==-1){EyeInOrOut=1; break;}//The eye is inside of the window
			}
			else{
				if(InOrOut== 1){EyeInOrOut=1; break;}//The eye is outside of the window
			}
		}
		if(!m_IsPlayButtonON)break;
	}while(m_durOfIdle[TimeKeeperID]>0);//Run until m_durOfIdle[TimeKeeperID]==0

	if(EyeInOrOut==0){
		m_durOfIdle[TimeKeeperID]=0;
	}
	m_durOfIdle[TimeKeeperID]=0;//Terminate the time-keeping process even when the intended eye movement did not happen till the end of the timeout.
	m_TimerUsage[TimeKeeperID]=0; //##### !!!! This needs to be here NOT in the timer itself because the timer (IDLE_FUNCTION_1,2,3...) is being used even when it has timed out!!!! #####

	return EyeInOrOut; 
}

//This function does not call Get_Latest_XYpos() to get the accurate recent eye position, because it does not need that accuracy.
void CTASDoc::Use_the_REAL_EYE_position(int EyeID)
{
	int AiChanID=FIRST_2D_BHVR_AI_CH_IDX+EyeID*2;
	if(*M_usableAiIdx[AiChanID]<0)return;//The DAS is not running: use the user clicked coordinates.

	(*M_JustClicked)=0; //Use the real eye position
	m_dataEyeXsaved[EyeID]=M_EyeX_rawData[EyeID]; m_dataEyeYsaved[EyeID]=M_EyeY_rawData[EyeID];//Small deviation will be ignored
}

//Check the eye position relative to the eye window
int CTASDoc::IsEyeIn(int WindID,int EyeID)
{
	float LeftBoundary=M_PositionWinds[WindID].LeftBoundary, UpperBoundary=M_PositionWinds[WindID].UpperBoundary, RightBoundary=M_PositionWinds[WindID].RightBoundary, LowerBoundary=M_PositionWinds[WindID].LowerBoundary;
	int IsEyeIn;
	if(*M_JustClicked && ((*M_BehavioralObjectForXYmonitor)==EyeID)){//The user just clicked the screen. The cliking is valid for the rest of the trial.
		if(M_EyeXsimulalted[EyeID]>LeftBoundary && M_EyeXsimulalted[EyeID]<RightBoundary && M_EyeYsimulalted[EyeID]>LowerBoundary && M_EyeYsimulalted[EyeID]<UpperBoundary){
			IsEyeIn=1;//The eye is inside of the window
		}
		else{
			IsEyeIn=0;//The eye is outside of the window
		}
	}
	else{//Real data
		//Get the eye position first
		Get_Latest_XYpos();
		if(M_EyeX_rawData[EyeID]>LeftBoundary && M_EyeX_rawData[EyeID]<RightBoundary && M_EyeY_rawData[EyeID]>LowerBoundary && M_EyeY_rawData[EyeID]<UpperBoundary){
			IsEyeIn=1;//The eye is inside of the window
		}
		else{
			IsEyeIn=0;//The eye is outside of the window
		}
	}

	return IsEyeIn; 
}

//Conceptually, a behavioral AI channel == FIRST_2D_BHVR_AI_CH_IDX+ BehavioralObj*2.  
//Dimentions of that behavior channel(BehavioralObj) are: (X, Y,...)==(FIRST_2D_BHVR_AI_CH_IDX+BehavioralObj*2+0, FIRST_2D_BHVR_AI_CH_IDX+BehavioralObj*2+1,...)
//Make sure that this is the same as the one in TAS
void CTASDoc::Get_Latest_XYpos()
{
	int AiChanID;
	int MaxI=NUM_2D_BEHAVIOR_OBJS;if(MaxI>NUM_2D_BEHAVIOR_OBJS)MaxI=NUM_2D_BEHAVIOR_OBJS;
	for(int i=0; i<MaxI; i++){
		AiChanID=FIRST_2D_BHVR_AI_CH_IDX+i*2;
		if(((*M_JustClicked)&&(i==(*M_BehavioralObjectForXYmonitor))) ||*M_usableAiIdx[AiChanID]<0 ){//The user just clicked the screen. The cliking is valid for the rest of the trial.
			M_EyePosX[i]=M_EyeXsimulalted[i]; 
			M_EyePosY[i]=M_EyeYsimulalted[i];
		}
		else{
			M_EyeX_rawData[i]=(SCALE_XY_POS*(*(M_AI[AiChanID  ]+*M_usableAiIdx[AiChanID  ]))  +M_EyeX_Bias[i])*M_EyeX_Gain[i];
			M_EyeY_rawData[i]=(SCALE_XY_POS*(*(M_AI[AiChanID+1]+*M_usableAiIdx[AiChanID+1]))  +M_EyeY_Bias[i])*M_EyeY_Gain[i];
			M_EyePosX[i]=M_EyeX_rawData[i]; 
			M_EyePosY[i]=M_EyeY_rawData[i];
		}
	}
}


//#############################################################################################################################################################
//##########################################################   File related  ###################################################################################################

UINT SaveOneTrial(LPVOID Ptr)
{	
	CTASDoc* pDoc=(CTASDoc*) Ptr;
	EnterCriticalSection(&(pDoc->m_CriticalSection4Serilize));
	//######

	CFile theFile;
	if(*pDoc->M_isRenamed==1){//MOXY has triggered this to make TAS to update the name of the file to save.
		pDoc->Make_TheBaseOfFileName_usingInfoInMemoryMap();
		sprintf(pDoc->m_FileNameFULL,"%s\\%s",pDoc->m_FolderPathWithoutFileName,pDoc->m_JustFileName);
		rename(pDoc->m_strFileName,pDoc->m_FileNameFULL);
		pDoc->m_strFileName=pDoc->m_FileNameFULL;
	}
	theFile.Open(pDoc->m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive archive(&theFile, CArchive::store);

	pDoc->Serialize(archive, *pDoc->M_usableTrialIdxInMem, pDoc->m_UserDataSetID_for_SaveFunctionOnly, pDoc->m_BehaviorSamplingRate, pDoc->m_StartTimeOfAI_MicS, pDoc->m_EndTimeOfAI_MicS, pDoc->m_numBehaviorChannels, pDoc->m_firstBehaviorAiIdx, pDoc->m_lastBehaviorAiIdx);
	
	archive.Close();
	theFile.Close();
	//######
	LeaveCriticalSection(&(pDoc->m_CriticalSection4Serilize));
	return 1;
}

void CTASDoc::EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell()
{
	//Check if MemoryMap config meets the trial's configuration.
	At_the_End_of_a_Trial_Check_if_MemoryMap_config_meets_the_trials_config();
	if(*M_isThereNewTrial==0)return;//There is nothing to save.

	int TrlIdxInMap;
	int IsToCreate,FirstTimeMsg;
	//int a=	GetFileSize(M_StartTime[0],  NULL);
	if(*M_NumTrlsInMAP<MAX_TRLS_IN_MAP){
		TrlIdxInMap=*M_NumTrlsInMAP;
		IsToCreate=1,FirstTimeMsg=1;
		//Create a new trial data in memory MAP
		do{//This dummy while loop is given to mimic the loop that RAS has.
		#include "MemMap_Open_Trials.h"
		}while(0);
		(*M_NumTrlsInMAP)++;
	}
	else{
		//Over-write on an existing oldest trial entity in memory
		TrlIdxInMap=*M_usableTrialIdxInMem+1;
		if(TrlIdxInMap>=MAX_TRLS_IN_MAP)TrlIdxInMap=0;
		if(*M_numEcodes>M_ECodesMAP_size[TrlIdxInMap]){
			//if(!UnmapViewOfFile(hView_M_ECodesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			//CloseHandle(hFile_M_ECodesMAP[TrlIdxInMap]); 
			//if(!UnmapViewOfFile(hView_M_EventTimesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			//CloseHandle(hFile_M_EventTimesMAP[TrlIdxInMap]); 

			//Re-Create a new Ecode & ETime area in memory MAP
			IsToCreate=2;//2 means Re-Create
			#include "MemMap_Open_Ecodes.h"
		}
		int ch, sizeOfMap;
		char NameOfMap[256];
		IsToCreate=2;//2 means Re-Create
		for(ch=0;ch<NUM_NN_AI_CHAN;ch++){
			if(*M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch]<*M_numSpikesOfNn1[ch]){
				#include "MemMap_Open_SpikesNn1.h"
			}
			if(*M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch]<*M_numSpikesOfNn2[ch]){
				#include "MemMap_Open_SpikesNn2.h"
			}
			if(*M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch]<*M_numSpikesOfNn3[ch]){
				#include "MemMap_Open_SpikesNn3.h"
			}
		}
	}
	int	i,ChID, Nn,endI;
	__int64 tmpTime=0;
	__int64 *pSpk, *pMSpk;
	if(m_UserGave_TrlID==0)m_TAS_trialID_tobeSaved++;
	*M_TrialID[TrlIdxInMap]=m_TAS_trialID_tobeSaved;
	//Event codes etc.
	for(ChID=0;ChID<*M_numNeuralChannels;ChID++){
		if(*M_usableSpikeIdxOfNn1[ChID]>=0 && tmpTime<*(M_spikeTimeStampOfNn1[ChID]+*M_usableSpikeIdxOfNn1[ChID])){
			tmpTime=*(M_spikeTimeStampOfNn1[ChID]+*M_usableSpikeIdxOfNn1[ChID]);
		}
		if(*M_usableSpikeIdxOfNn2[ChID]>=0 && tmpTime<*(M_spikeTimeStampOfNn2[ChID]+*M_usableSpikeIdxOfNn2[ChID])){
			tmpTime=*(M_spikeTimeStampOfNn2[ChID]+*M_usableSpikeIdxOfNn2[ChID]);
		}
		if(*M_usableSpikeIdxOfNn3[ChID]>=0 && tmpTime<*(M_spikeTimeStampOfNn3[ChID]+*M_usableSpikeIdxOfNn3[ChID])){
			tmpTime=*(M_spikeTimeStampOfNn3[ChID]+*M_usableSpikeIdxOfNn3[ChID]);
		}
	}
	*M_StartTime[TrlIdxInMap]=M_EventTimes[*M_FirstEcodeIdxOfTrial];
	*M_EndTime[TrlIdxInMap]  =BiggerOne_int64(tmpTime,M_EventTimes[*M_FirstEcodeIdxOfTrial+*M_numEcodes-1]);
	*M_numEcodesMAP[TrlIdxInMap]=*M_numEcodes;
	int		*pECode=M_ECodesMAP[TrlIdxInMap] ;
	__int64 *pETime=M_EventTimesMAP[TrlIdxInMap],k;
	for(i=0;i<*M_numEcodes;i++){
		pECode[i]=M_EventCodes[*M_FirstEcodeIdxOfTrial+i];
		pETime[i]=M_EventTimes[*M_FirstEcodeIdxOfTrial+i];
	}
	//Spikes
	int NumSpks, firstSpkIdx, numChanHavingSpikes=0,firstNeuronInChan;
	for(ChID=0;ChID<*M_numNeuralChannels;ChID++){
		firstNeuronInChan=1;
		*M_NumNnsInChanHavingSpikes[TrlIdxInMap][ChID]=0;
		for(Nn=0;Nn<3;Nn++){
			//First neuron of the channel
			if(Nn==0){     NumSpks=*M_numSpikesOfNn1[ChID]; *M_numSpksOfNn1MAP[TrlIdxInMap][ChID]=NumSpks; if(NumSpks<1)continue; firstSpkIdx=*M_firstSpkIdxInTrialOfNn1[ChID]; pSpk=M_SpkTimeOfNn1MAP[TrlIdxInMap][ChID]; pMSpk=(M_spikeTimeStampOfNn1[ChID]+firstSpkIdx);}
			else if(Nn==1){NumSpks=*M_numSpikesOfNn2[ChID]; *M_numSpksOfNn2MAP[TrlIdxInMap][ChID]=NumSpks; if(NumSpks<1)continue; firstSpkIdx=*M_firstSpkIdxInTrialOfNn2[ChID]; pSpk=M_SpkTimeOfNn2MAP[TrlIdxInMap][ChID]; pMSpk=(M_spikeTimeStampOfNn2[ChID]+firstSpkIdx);}
			else {         NumSpks=*M_numSpikesOfNn3[ChID]; *M_numSpksOfNn3MAP[TrlIdxInMap][ChID]=NumSpks; if(NumSpks<1)continue; firstSpkIdx=*M_firstSpkIdxInTrialOfNn3[ChID]; pSpk=M_SpkTimeOfNn3MAP[TrlIdxInMap][ChID]; pMSpk=(M_spikeTimeStampOfNn3[ChID]+firstSpkIdx);}
			if(firstNeuronInChan==1){*M_ChanIDsHavingSpikes[TrlIdxInMap][numChanHavingSpikes]=ChID; numChanHavingSpikes++; firstNeuronInChan=0;}
			*M_NnIdWithSpk[TrlIdxInMap][ChID][*M_NumNnsInChanHavingSpikes[TrlIdxInMap][ChID]]=Nn;
			(*M_NumNnsInChanHavingSpikes[TrlIdxInMap][ChID])++;
			//err=GetLastError();
			if(NumSpks>(*M_Spike_QueueSize)-2){
				//(1) Error message: MAX_SPIKEs.
				if(NumSpks>(*M_Spike_QueueSize))AfxMessageBox("NumSpks>(*M_Spike_QueueSize)?\n This should not happen, because DAS_sv does not keep more than MAX_SPIKEs");
				else AfxMessageBox("There have been too many spikes (>MAX_SPIKEs) in this trial.\n The spikes occurred later part of the trial have neen truncated. \n To avoid this situation, you can either shorten the duration of the recording for each trial, or increase MAX_SPIKEs, or avoid junk signals if it was a mistake.");
			}
			if(firstSpkIdx+NumSpks<(*M_Spike_QueueSize)){
				for(i=0;i<NumSpks;i++){
					//k=pMSpk[i];
					//pSpk[i]=k;
					pSpk[i]=pMSpk[i];
				}
			}
			else{
				endI=(*M_Spike_QueueSize)-firstSpkIdx;
				for(i=0;i<endI;i++){
					//k=pMSpk[i];
					//pSpk[i]=k;
					pSpk[i]=pMSpk[i];
				}
				endI=NumSpks-endI;
				if(Nn==0){     pSpk=&M_SpkTimeOfNn1MAP[TrlIdxInMap][ChID][i]; pMSpk=(M_spikeTimeStampOfNn1[ChID]+0);}
				else if(Nn==1){pSpk=&M_SpkTimeOfNn2MAP[TrlIdxInMap][ChID][i]; pMSpk=(M_spikeTimeStampOfNn2[ChID]+0);}
				else {         pSpk=&M_SpkTimeOfNn3MAP[TrlIdxInMap][ChID][i]; pMSpk=(M_spikeTimeStampOfNn3[ChID]+0);}
				for(i=0;i<endI;i++){
					pSpk[i]=pMSpk[i];
				}
			}
		}
	}
	*M_NumChansHavingSpikes[TrlIdxInMap]=numChanHavingSpikes;

	//Analog Signals: This needs to be done even when DAS is not running so that Saving function knows what's going on.
	m_numBehaviorChannels=*M_numBehaviorChans;
	m_firstBehaviorAiIdx=*M_firstBehaviorAiIdxInTrial;
	m_lastBehaviorAiIdx=*M_usableAiIdx[FIRST_2D_BHVR_AI_CH_IDX];
	m_BehaviorSamplingRate=*M_BehaviorSamplingRate;
	if(m_firstBehaviorAiIdx>0 && m_lastBehaviorAiIdx>0){
		m_StartTimeOfAI_MicS=M_timeStamp4BehaviorAI_MicS[m_firstBehaviorAiIdx];//Currently, all Behavior Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
		m_EndTimeOfAI_MicS  =M_timeStamp4BehaviorAI_MicS[m_lastBehaviorAiIdx];//Currently, all Behavior Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
	}
	else{
		m_StartTimeOfAI_MicS=-1;//Currently, all Behavior Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
		m_EndTimeOfAI_MicS  =-1;//Currently, all Behavior Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
	}
	*M_usableTrialIdxInMem=TrlIdxInMap;
	*M_isThereNewTrial=0;

	//This part needs to come after doing all the stuff above.
	//This is because the SaveOneTrial() is a parallel thread that runns with the next trial.
	m_UserDataSetID_for_SaveFunctionOnly=(*M_MoxyUserDataSetID);
	if(m_isSaving==1){
		AfxBeginThread( SaveOneTrial, this);
	}
	m_firsVIS_ErrorMsg=1;//By setting this, the TAS gives a warning when VIS is not responding. It does only once when the flag "m_firsVIS_ErrorMsg" is 1. It turns off right after the error message.
}

void CTASDoc::At_the_End_of_a_Trial_Check_if_MemoryMap_config_meets_the_trials_config()
{
#define MARGIN_FOR_SAVE_FUNCTION 0.1 //100 ms; This time is introduced to make sure that the computer has time to write before the queue is overwritten by the new AI signals in the new trial.
	if(m_AI_Size_warning==1 && (*M_LatestTimeStampInMicroS-m_TimeOfTrialStart)*0.000001 > MAX_TIMESPAN_IN_SEC_STORED_IN_Q - MARGIN_FOR_SAVE_FUNCTION){
		char Msg[512]; 
		sprintf(Msg,"Size of AI queue (%d sec)<Timespan of trial(%0.1f sec). \n Consider increasing MAX_TIMESPAN_IN_SEC_STORED_IN_Q in \"SharedConstants.h\", if the behavioral data is important.\n You can suppress this message by setting \"m_pDoc->m_AI_Size_warning=0\".", MAX_TIMESPAN_IN_SEC_STORED_IN_Q, (float)((*M_LatestTimeStampInMicroS-m_TimeOfTrialStart)*0.000001) );
		AfxMessageBox(Msg);
	}

	//MAX_SPIKEs => Taken care of at (1) Error message: MAX_SPIKEs, in "EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell()".
	//This is an urgent situation if happened.

	//ECODE_Q_SIZE
	if(*M_numEcodes>ECODE_Q_SIZE){
		char Msg[512]; 
		sprintf(Msg,"Too many ECodes(=%d) have been used. Increase ECODE_Q_SIZE(=%d) in \"SharedConstants.h\" Or, consider using Name_and_Value_to_record()or Name_and_ArrayOfValues_to_record()", *M_numEcodes,ECODE_Q_SIZE);
		AfxMessageBox(Msg);
	}
}


void CTASDoc::Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany) 
{	
	strcpy(Dest,Source);
	strrev(Dest);
	Dest[strlen(Dest)-(Nth-1)]='\0';
	strrev(Dest);
	Dest[HowMany]='\0';
}

#include "Dialog_AppendOverwriteCancel.h"
void CTASDoc::OnSaveTrialsToFile()
{
	if(m_isTAS_Running==0)return;
	if(m_isSaving==1){//Turn it off!! It has the same effect as pushing the "close" button from the menu.
		m_isSaving=0; *M_isSaving=0; 
		Inform_MOXY_withoutHandshaking("UpdateFileDisp");
		return;
	}

	Make_TheBaseOfFileName_usingInfoInMemoryMap();
	int result=LocateAndCreateFolder_IfItIsNotAlreayThere();
	if(result==-1){//Failed to create or open a folder.
		if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,m_BaseOfFileName)==0)return;
	}
	GetLastFolderName(m_FolderPathWithoutFileName,m_LastPartOfFolderName);
	m_strFileName=m_FileNameFULL;

	//#########  Examine if there is an existing file with the same name.  #########
	FILE *fp;
	fp=fopen(m_FileNameFULL,"r");
	if(fp!= NULL){
		fclose(fp);//First, close the file.
		CAppendOverwriteCancel SaveChoice;
		if(SaveChoice.DoModal()==IDCANCEL){
			return;
		}
		else if(SaveChoice.m_Choice==OVERWRITE_FILE){
			//Backup the old one.
			int length=strlen(m_FileNameFULL);
			char tmpName[1024];
			Copy_fromNth_HowMany(tmpName, m_FileNameFULL, 0, length-3);
			strcat(tmpName,"bak");
			remove(tmpName);//If there is an existing *.bak file, delete it first.
			rename(m_FileNameFULL,tmpName);
			//Open a new file
			fp=fopen(m_FileNameFULL,"w");fclose(fp);
			Serialize_Header();//Write a header.
		}
		//else{} //The default action is to append.
	}
	else{//New file
		Serialize_Header();
	}
	//##############################################################################

	*M_isThereNewTrial=0; //Nothing to save for now
	m_isSaving=1; *M_isSaving=1;
	Inform_MOXY_withoutHandshaking("UpdateFileDisp");
}

int CTASDoc::LocateAndCreateFolder_IfItIsNotAlreayThere()
{
	CString Date = GetDateString();	

	char tmpStr[1024],FldrName[1024];
	CString ExeDirString=GetExecutableName();
	strcpy(tmpStr,ExeDirString);
	if(wildcmp("*\Blip*",tmpStr)==1){
		RE_Search1:;
		if(wildcmp("*\Blip",tmpStr)==1){
			sprintf(FldrName,"%s",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search1;}
		sprintf(FldrName,"%s\\_Data_Recorded\\%s",FldrName,Date);
		//SetCurrentDirectory(FldrName);
		return CreateFolder_IfNotAlreayThere_Also_MakeFileName(FldrName, Date);
	}		

	return -1;//Failed to open or create a folder
}
CString CTASDoc::GetDateString()
{
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;  // Get the current time from the operating system.
	CTime CurrTime( osBinaryTime ); //  CTime from C run-time time
	CString Date = CurrTime.Format( "%m%d%y" );	
	return Date;
}

CString CTASDoc::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

int CTASDoc::CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FldrName[], CString Date)
{	
	int FileResult=CreateDirectory(FldrName,NULL);//Returns a non zero number if succesfull.
	if(FileResult==0){
		FileResult=GetLastError();
		if(FileResult!=183){
			AfxMessageBox("Unable to automatically open/create a folder to store data");//183 means the folder is there already.
			return -1;
		}
	}
	if(m_prvDate!=Date){
		m_prvDate=m_Date; *M_NnNumberToRecord=1;
		//Since the file sequence number has been changed remake the file name. 
		Add_NnNumber_toFileName();
	}//When the date changes, the file sequence resets.
	strcpy(m_FolderPathWithoutFileName,FldrName);
	sprintf(m_FileNameFULL,"%s\\%s",m_FolderPathWithoutFileName,m_JustFileName);
	return 0;
}
// This is used to initialize the name of the file to store, reflecting the initial seting given in the task.
void CTASDoc::Make_TheBaseOfFileName_usingInfoInLocalVariables()
{
	m_Date = GetDateString();	
	strcpy(m_BaseOfFileName,m_PreFix);
	strcat(m_BaseOfFileName,m_Date); 
	//strcat(m_BaseOfFileName,"_");
	strcat(m_BaseOfFileName,m_SuFFix);
	strcpy(M_BaseOfFileName,m_BaseOfFileName);
	Add_NnNumber_toFileName();//<- This one has it:	Inform_MOXY_withoutHandshaking("UpdateFileDisp");
}

//This is used to formuated the name of the file to store in case the user has changed prefix & surfix using the dialog in MOXY.
void CTASDoc::Make_TheBaseOfFileName_usingInfoInMemoryMap()
{
	strcpy(m_PreFix,M_PreFix); 
	strcpy(m_SuFFix,M_SuFFix);
	Make_TheBaseOfFileName_usingInfoInLocalVariables();
	*M_isRenamed=0;// Reset MOXY's message.
}

void CTASDoc::Add_NnNumber_toFileName()
{	
	m_PrvNnNumberToRecord=*M_NnNumberToRecord;
	char NnStr[10]; itoa(*M_NnNumberToRecord,NnStr,10);
	sprintf(m_JustFileName,"%s%s.rst",m_BaseOfFileName,NnStr);
	strcpy(M_JustFileName,m_JustFileName);

	Inform_MOXY_withoutHandshaking("UpdateFileDisp");
}

void CTASDoc::Give_Prefix_Suffix_forFileName(char prefix[], char suffix[])
{	
	strcpy(m_PreFix,prefix);	strcpy(M_PreFix,prefix);
	strcpy(m_SuFFix,suffix);	strcpy(M_SuFFix,suffix);
	Make_TheBaseOfFileName_usingInfoInLocalVariables();
	Inform_MOXY_withoutHandshaking("UpdateFileDisp");
}

//The user clicked the "Close" menu.
void CTASDoc::OnFileCloseWriting()
{
	m_isSaving=0; *M_isSaving=0;
	Inform_MOXY_withoutHandshaking("UpdateFileDisp");
}

void CTASDoc::OnUpdateOnSaveTrialsToFile(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isSaving==1);
}


void CTASDoc::Serialize_Header()
{
	CFile theFile;
	theFile.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive archive(&theFile, CArchive::store);

	archive << m_LengthOfHeader;
	archive << m_Header;

	archive.Close();
	theFile.Close();
}

char GlobalMsg[1024];
UINT MessageThread(LPVOID MessageStr)
{
	AfxMessageBox((char*) MessageStr);
	 return 0;
}

//Caution: 
// 1) Use only the local variables as much as possible. For example, in the case SaveOneTrial() is called by the task before the previous SaveOneTrial() is in progress, using non-local variables may affect the currently saving variables. 
// 2) Do not use AfxMessageBox(). This will hault the saving until the user clicks it away. Instead use a thread such as MessageThread();
void CTASDoc::Serialize(CArchive& ar, int TrlIdxInMap, int UserDataSetID_for_SaveFunctionOnly, int BehaviorSamplingRate, __int64 StartTimeOfAI_MicS, __int64 EndTimeOfAI_MicS, int NumBehaviorChannels, __int64 firstBehaviorAiIdx, __int64 lastBehaviorAiIdx)
{
	int i,j, numEcode, numDataSets;
	MoxyUserData *pMoxyData;
		
	// Now do the stuff for CTrial class
	if( ar.IsStoring() ){
        ar << *M_TrialID[TrlIdxInMap];
        ar << *M_StartTime[TrlIdxInMap];
        ar << *M_EndTime[TrlIdxInMap];
		//###### ECODES
        ar << *M_numEcodesMAP[TrlIdxInMap];
		numEcode=*M_numEcodesMAP[TrlIdxInMap];
		ar.Write(M_ECodesMAP[TrlIdxInMap],numEcode*sizeof(int));
		ar.Write(M_EventTimesMAP[TrlIdxInMap],numEcode*sizeof(long long));
		//###### USER VALUES
		ar << m_UserData[UserDataSetID_for_SaveFunctionOnly].m_NumUserData+ M_MoxyUserData[UserDataSetID_for_SaveFunctionOnly].m_NumUserData;
		m_UserData[UserDataSetID_for_SaveFunctionOnly].Serialize(ar);
		pMoxyData=&(M_MoxyUserData[UserDataSetID_for_SaveFunctionOnly]);
		numDataSets=pMoxyData->m_NumUserData;
		for(i=0; i<numDataSets; i++){
			ar << pMoxyData->m_SizeOfName[i];
			ar.Write(pMoxyData->m_NameArray[i],pMoxyData->m_SizeOfName[i]*sizeof(char));
			ar << pMoxyData->m_NumOfValues[i];
			ar.Write(&(pMoxyData->m_ValuesToRecord[pMoxyData->m_FirstLocationOfUserValue[i]]),pMoxyData->m_NumOfValues[i]*sizeof(double));
			ar << pMoxyData->m_TimeOfOccurrence[i];
		}
		//####### Spikes
		int	numChanHavingSpikes=*M_NumChansHavingSpikes[TrlIdxInMap];
		ar << numChanHavingSpikes;
		int	ChanCnt, ChanID,NnCnt, NumSpks,numNnsInChanHavingSpikes,chanIdxWithSpk,NnID;
		__int64* pSpk;
		for(ChanCnt=0;ChanCnt<numChanHavingSpikes;ChanCnt++){
			chanIdxWithSpk=*M_ChanIDsHavingSpikes[TrlIdxInMap][ChanCnt];
			ar << chanIdxWithSpk;
			numNnsInChanHavingSpikes=*M_NumNnsInChanHavingSpikes[TrlIdxInMap][chanIdxWithSpk];
			ar << numNnsInChanHavingSpikes;
			for(NnCnt=0;NnCnt<numNnsInChanHavingSpikes;NnCnt++){
				NnID=*M_NnIdWithSpk[TrlIdxInMap][chanIdxWithSpk][NnCnt];
				if(NnID==0){     NumSpks=*M_numSpksOfNn1MAP[TrlIdxInMap][chanIdxWithSpk]; pSpk=M_SpkTimeOfNn1MAP[TrlIdxInMap][chanIdxWithSpk];}
				else if(NnID==1){NumSpks=*M_numSpksOfNn2MAP[TrlIdxInMap][chanIdxWithSpk]; pSpk=M_SpkTimeOfNn2MAP[TrlIdxInMap][chanIdxWithSpk];}
				else {			 NumSpks=*M_numSpksOfNn3MAP[TrlIdxInMap][chanIdxWithSpk]; pSpk=M_SpkTimeOfNn3MAP[TrlIdxInMap][chanIdxWithSpk];}
				ar << NnID;// neuron ID
				ar << NumSpks;
				if(NumSpks<1){
					sprintf(GlobalMsg, "Trl:%d, Ch:%d/Nn:%d: NumSpks==%d?",TrlIdxInMap, chanIdxWithSpk,NnID,NumSpks);			
					AfxBeginThread( MessageThread, GlobalMsg);
				}
				ar.Write(pSpk,NumSpks*sizeof(long long));
			}
		}

		//####### Analog Input regarding the behavior
		//if(NumBehaviorChannels<1 || firstBehaviorAiIdx<0)return;
		double *pAI;
		int endI,numDataPointsOfBehaviorAI;
		ar << BehaviorSamplingRate;
		ar << StartTimeOfAI_MicS;
		ar << EndTimeOfAI_MicS;
		ar << NumBehaviorChannels;
		if(lastBehaviorAiIdx>=firstBehaviorAiIdx){// Equal "=" is added in the case where there was no Analog input.
			if(firstBehaviorAiIdx<0 || lastBehaviorAiIdx<0){numDataPointsOfBehaviorAI=0;}
			else{
				numDataPointsOfBehaviorAI=lastBehaviorAiIdx-firstBehaviorAiIdx;//Currently, all Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
				//Check the validity
				if(StartTimeOfAI_MicS>EndTimeOfAI_MicS){
					sprintf(GlobalMsg, "StartTimeOfAI_MicS>EndTimeOfAI_MicS");			
					AfxBeginThread( MessageThread, GlobalMsg);
				}
				else if(StartTimeOfAI_MicS==EndTimeOfAI_MicS && numDataPointsOfBehaviorAI!=0)AfxMessageBox("StartTimeOfAI_MicS==EndTimeOfAI_MicS && numDataPointsOfBehaviorAI!=0");
				if(fabs(numDataPointsOfBehaviorAI-BehaviorSamplingRate*0.000001*(EndTimeOfAI_MicS-StartTimeOfAI_MicS))>20){
					sprintf(GlobalMsg, "abs(numDataPointsOfBehaviorAI-BehaviorSamplingRate*0.000001*(EndTimeOfAI_MicS-StartTimeOfAI_MicS))>20");			
					AfxBeginThread( MessageThread, GlobalMsg);
				}
			}
			//Save AI
			for(ChanID=0;ChanID<NumBehaviorChannels;ChanID++){
				ar << ChanID;
				ar << numDataPointsOfBehaviorAI;
				if(numDataPointsOfBehaviorAI>0){
					pAI=M_AI[FIRST_2D_BHVR_AI_CH_IDX+ChanID];
					for(i=firstBehaviorAiIdx;i<lastBehaviorAiIdx;i++){
						ar << (int)((SCALE_XY_POS*1000)*pAI[i]); //The AI signal is amplified by 1000 times of the signal used in BLIP to conver it to "int", and maintain the accuracy.
					}
				}
			}
		}
		else{
			if(firstBehaviorAiIdx<0 || lastBehaviorAiIdx<0){numDataPointsOfBehaviorAI=0;}
			else{
				numDataPointsOfBehaviorAI=M_AIQueueSize[FIRST_2D_BHVR_AI_CH_IDX]-firstBehaviorAiIdx+lastBehaviorAiIdx;//Currently, all Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
				//Here "M_AIQueueSize[FIRST_2D_BHVR_AI_CH_IDX]-firstBehaviorAiIdx" became negative.
				//
				//Check the validity
				if(StartTimeOfAI_MicS>EndTimeOfAI_MicS){
					sprintf(GlobalMsg, "StartTimeOfAI_MicS>EndTimeOfAI_MicS");			
					AfxBeginThread( MessageThread, GlobalMsg);
				}
				else if(StartTimeOfAI_MicS==EndTimeOfAI_MicS && numDataPointsOfBehaviorAI!=0)AfxMessageBox("StartTimeOfAI_MicS==EndTimeOfAI_MicS && numDataPointsOfBehaviorAI!=0");
				if(fabs(numDataPointsOfBehaviorAI-BehaviorSamplingRate*0.000001*(EndTimeOfAI_MicS-StartTimeOfAI_MicS))>20){
					sprintf(GlobalMsg, "abs(numDataPointsOfBehaviorAI-BehaviorSamplingRate*0.000001*(EndTimeOfAI_MicS-StartTimeOfAI_MicS))>20");			
					AfxBeginThread( MessageThread, GlobalMsg);
				}
			}
			//Save AI
			endI=M_AIQueueSize[FIRST_2D_BHVR_AI_CH_IDX];
			for(ChanID=0;ChanID<NumBehaviorChannels;ChanID++){
				ar << ChanID;
				ar << numDataPointsOfBehaviorAI;
				if(numDataPointsOfBehaviorAI>0){
					pAI=M_AI[FIRST_2D_BHVR_AI_CH_IDX+ChanID];
					for(i=firstBehaviorAiIdx;i<endI;i++){
						ar << (int)((SCALE_XY_POS*1000)*pAI[i]); //The AI signal is amplified by 1000 times of the signal used in BLIP to conver it to "int", and maintain the accuracy.
					}
					for(i=0;i<lastBehaviorAiIdx;i++){
						ar << (int)((SCALE_XY_POS*1000)*pAI[i]); //The AI signal is amplified by 1000 times of the signal used in BLIP to conver it to "int", and maintain the accuracy.
					}
				}
			}
		}
	}
}

void CTASDoc::Write_Your_Header_Here_to_Write_on_the_File(char *header)
{
	if(strlen(m_Header)==0){//Initial part of the Header.
		m_Header.Format("BLIP Version: %0.3lf; ",BLIP_VERSION);
		m_Header+="Date recorded: "+GetDateString()+"; ";
		m_Header+=header; m_Header+="; ";
	}
	else{
		m_Header+=header; m_Header+="; ";
	}
	m_LengthOfHeader=m_Header.GetLength()+1;
}

void CTASDoc::Name_and_ArrayOfValues_to_record(char Name[], double ArrayOfValues[], int N) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_ArrayOfValues_to_record(char Name[], float ArrayOfValues[], int N) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_ArrayOfValues_to_record(char Name[], int ArrayOfValues[], int N) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}

void CUserData::Name_and_ArrayOfValues_to_Record(char Name[], int ArrayOfValues[], int N)
{
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CUserData::Name_and_ArrayOfValues_to_Record(char Name[], float ArrayOfValues[], int N)
{
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CUserData::Name_and_ArrayOfValues_to_Record(char Name[], double ArrayOfValues[], int N)
{
	#include "Name_and_ArrayOfValues_to_Record.h"
}

void CTASDoc::Name_and_Value_to_record(char Name[], double value) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=1; 
	double ArrayOfValues[1]={value};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=2; 
	double ArrayOfValues[2]={V1, V2};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}

void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=3; 
	double ArrayOfValues[3]={V1, V2, V3};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=4; 
	double ArrayOfValues[4]={V1, V2, V3, V4};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=5; 
	double ArrayOfValues[5]={V1, V2, V3, V4, V5};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=6; 
	double ArrayOfValues[6]={V1, V2, V3, V4, V5, V6};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=7; 
	double ArrayOfValues[7]={V1, V2, V3, V4, V5, V6, V7};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=8; 
	double ArrayOfValues[8]={V1, V2, V3, V4, V5, V6, V7, V8};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=9; 
	double ArrayOfValues[9]={V1, V2, V3, V4, V5, V6, V7, V8, V9};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}
void CTASDoc::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9, double V10) 
{	
	m_UserData[(*M_MoxyUserDataSetID)].m_TimeOfOccurrence[m_UserData[(*M_MoxyUserDataSetID)].m_NumUserData]=*M_LatestTimeStampInMicroS;

	int N=10; 
	double ArrayOfValues[10]={V1, V2, V3, V4, V5, V6, V7, V8, V9, V10};
	m_UserData[(*M_MoxyUserDataSetID)].Name_and_ArrayOfValues_to_Record(Name, ArrayOfValues, N);
}

//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPathWithoutFileName & m_FileNameFULL
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPathWithoutFileName
int CTASDoc::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
{	
	CFileDialog dialog(IsToREAD, "rst", initialFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.rst)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){return 0;}
	sprintf(m_FileNameFULL,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPathWithoutFileName,m_FileNameFULL);
	m_FolderPathWithoutFileName[strlen(m_FileNameFULL)-strlen(m_JustFileName)]='\0';
	
	//Extract file name without extension (.txt)
	#define dotSizePlus1 2 //The size of the identifier ".txt"
	char strbuf[1000];
	int i,sizeofChar,foundExt=0;
	sizeofChar=strlen(m_JustFileName);
	for(i=0;i<sizeofChar;i++){
		strncpy_s(strbuf,dotSizePlus1,m_JustFileName+i,_TRUNCATE);
		if(strcmp(".", strbuf)==0){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileNameFULL);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileNameFULL)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CTASDoc::GetLastFolderName(char* FolderPath, char *LastFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first letter
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	strncpy_s(FldrName,i+1,&tmpStr[0],_TRUNCATE);
	_strrev(FldrName);
	strcpy_s(LastFolderName,100,FldrName);
	return 1;
}

//This one discards the parent folder name out of the full pathway "FolderPath"
int CTASDoc::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first letter
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	int a=strlen(tmpStr)-(i+1);
	if(a<=0){
		AfxMessageBox("Couldn't find Blip folder."); 
		exit(1);
	} 
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
int CTASDoc::Find_Last_Num_inString(char *GivenString, int &NumToReturn)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0, sign=0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			if(*p=='-')sign=1; else sign=0;
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return -1;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	return 0;
}

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
//In case it does not find a number, it give a waning and does not register anything.
int CTASDoc::Find_Last_PositiveNum_inString(char *GivenString, int &NumToReturn)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		char strbuf[1000];
		_snprintf(strbuf, 999, "The name does not contain a number, or an alias of an ECode", GivenString);
		AfxMessageBox(strbuf);
		return -1;
	}
	NumToReturn=num;
	return 0;
}

int CTASDoc::wildcmp(char *wild, char *string) 
{
  char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if(*wild == '*') {
      if(!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } 
	else if((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } 
	else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  int a=strlen(wild);
  if(a==0)return 1;
  return !*wild;
}

//##################################################   CUserData  ###########################################################################################################
//##################################################   CUserData  ###########################################################################################################
//######### User defined Data to write to file #########
CUserData::CUserData()
{
	m_SizeOfUserData=10;
	m_NameArray.SetSize(m_SizeOfUserData);
	m_SizeOfName.SetSize(m_SizeOfUserData);
	m_FirstLocationOfUserValue.SetSize(m_SizeOfUserData);
	m_NumOfValues.SetSize(m_SizeOfUserData);
	m_ValuesToRecord.SetSize(m_SizeOfUserData);
	m_TimeOfOccurrence.SetSize(m_SizeOfUserData);
	m_NumUserData=0; m_TotNumUserVlaluesToRecord=0; m_FirstLocationOfUserValue[0]=m_TotNumUserVlaluesToRecord;
}
CUserData::~CUserData()
{
	//////
	m_NameArray.RemoveAll();
	m_SizeOfName.RemoveAll();
	m_ValuesToRecord.RemoveAll();
	m_NumOfValues.RemoveAll();
	m_FirstLocationOfUserValue.RemoveAll();
	m_TimeOfOccurrence.RemoveAll();
}

void CUserData::Serialize(CArchive& ar)
{
	int i,j;
	for(i=0; i<m_NumUserData; i++){
		ar << m_SizeOfName[i];
		ar << m_NameArray[i];
		ar << m_NumOfValues[i];
		for(j=0; j<m_NumOfValues[i]; j++){
			ar << m_ValuesToRecord[m_FirstLocationOfUserValue[i]+j];
		}
		ar << m_TimeOfOccurrence[i];
	}
}


//#############################################################################################################################################################
//##########################################################   Signal IO   ###################################################################################################
//This one checks if the signal comming from the NI input port (signalSource) matches "whatToCheck".
int CTASDoc::WaitUntilSignalOn(int whatToCheck, int signalSource, int dur_Wait) 
{
	//Time keeping
	int TimeKeeperID=FindAvailableTimer();//TimeKeeperID MUST be obtained using FindAvailableTimer() function.
	if(m_durOfIdle[TimeKeeperID]!=0)AfxMessageBox("m_durOfIdle[TimeKeeperID]!=0");//The timers need to be 0 at the start of idling to make sure that the previous thread using the timer has ended, before launching another thread using the same timer. 
	m_durOfIdle[TimeKeeperID]=dur_Wait;
	switch(TimeKeeperID){
		case 0:	AfxBeginThread( IDLE_FUNCTION_0,  this); break;
		case 1:	AfxBeginThread( IDLE_FUNCTION_1,  this); break;
		case 2:	AfxBeginThread( IDLE_FUNCTION_2,  this); break;
		case 3:	AfxBeginThread( IDLE_FUNCTION_3,  this); break;
		case 4:	AfxBeginThread( IDLE_FUNCTION_4,  this); break;
		case 5:	AfxBeginThread( IDLE_FUNCTION_5,  this); break;
		case 6:	AfxBeginThread( IDLE_FUNCTION_6,  this); break;
		case 7:	AfxBeginThread( IDLE_FUNCTION_7,  this); break;
		case 8:	AfxBeginThread( IDLE_FUNCTION_8,  this); break;
		case 9:	AfxBeginThread( IDLE_FUNCTION_9,  this); break;
		case 10:AfxBeginThread( IDLE_FUNCTION_10, this); break;
		case 11:AfxBeginThread( IDLE_FUNCTION_11, this); break;
		case 12:AfxBeginThread( IDLE_FUNCTION_12, this); break;
		case 13:AfxBeginThread( IDLE_FUNCTION_13, this); break;
		case 14:AfxBeginThread( IDLE_FUNCTION_14, this); break;
		case 15:AfxBeginThread( IDLE_FUNCTION_15, this); break;
		case 16:AfxBeginThread( IDLE_FUNCTION_16, this); break;
		case 17:AfxBeginThread( IDLE_FUNCTION_17, this); break;
		case 18:AfxBeginThread( IDLE_FUNCTION_18, this); break;
		case 19:AfxBeginThread( IDLE_FUNCTION_19, this); break;
	}

	while(m_durOfIdle[TimeKeeperID]){//Run until m_durOfIdle[TimeKeeperID]==0
		Sleep(1);//Do something here.
		//if()return 1; //TwhatToCheck == signalSource
		if(!m_IsPlayButtonON)break;
	}
	m_durOfIdle[TimeKeeperID]=0;//Terminate the time-keeping process even when the intended eye movement did not happen till the end of the timeout.
	m_TimerUsage[TimeKeeperID]=0; //##### !!!! This needs to be here NOT in the timer itself because the timer (IDLE_FUNCTION_1,2,3...) is being used even when it has timed out!!!! #####

	return 0;
}

void CTASDoc::Relay_ON_n_DisplayLED(int whichBIT)
{
	Relay_ON(whichBIT);
	DisplayLED(whichBIT, DIO_ON);
}
void CTASDoc::Relay_OFF_n_DisplayLED(int whichBIT)
{
	Relay_OFF(whichBIT);
	DisplayLED(whichBIT, DIO_OFF);
}

void CTASDoc::SendOut_M_DO_Array() //Use "m_pDoc->M_DO_Array[Digital Output Channel number here] = 1 or 0;" to set digital bits before calling this function.
{
	if(!SetEvent(m_M_DO_Active[0])){ //Setting any m_M_DO_Active[] (from m_M_DO_Active[0] ~ from m_M_DO_Active[MAX_DO_NUM-1]) will make DAS send out the entire M_DO_Array[]. 
		AfxMessageBox("SetEvent() failed");
	}
}

void CTASDoc::Relay_ON(int whichBIT)
{
	if(whichBIT>=MAX_DO_NUM){AfxMessageBox("whichBIT>=MAX_DO_NUM"); return;}
	M_DO_Array[whichBIT]=RELAY_ON; //0 means on because NI acts as a voltage sink for the relay (70-ODC5 Grayhill)
	if(!SetEvent(m_M_DO_Active[whichBIT])){
		AfxMessageBox("SetEvent() failed");
	}
}
void CTASDoc::Relay_OFF(int whichBIT)
{
	if(whichBIT>=MAX_DO_NUM){AfxMessageBox("whichBIT>=MAX_DO_NUM"); return;}
	M_DO_Array[whichBIT]=RELAY_OFF; //1 means off because NI acts as a voltage sink for the relay (70-ODC5 Grayhill)
	if(!SetEvent(m_M_DO_Active[whichBIT])){
		AfxMessageBox("SetEvent() failed");
	}
}
void CTASDoc::OrthodromicStimulation()
{
	if(*M_isANTS_Running==0|| *M_IsOrthoStim==0)AfxMessageBox("Please turn on ANTS. And, put it on Orthodromic mode (->).");
	if(!SetEvent(*M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation)){AfxMessageBox("SetEvent() failed");}
	*M_HeyDAS_DoOrthodromicStimulationNOW=1; //Now request DAS_sv a job
	
	//Just in case
	*M_IsOrthoStim=1;
	*M_IsAntidromicStim=0; 
}
void CTASDoc::SetOrthoStimGatingRelay__DoThisAtLeast1msBeforeCallingOrthodromicStimulation(int StimDO)
{
	if(*M_isANTS_Running==0|| *M_IsOrthoStim==0)AfxMessageBox("Please turn on ANTS. And, put it on Orthodromic mode (->).");
	int StimChanIdxCounter=GetIndexOfSimChannel(StimDO);
	if(StimChanIdxCounter==-1){char MsgBuff[128]; sprintf(MsgBuff,"Currently, ANTS do not have that stimulation channel (%d) registered. Please check \"Antidromic Property Dialog\".",StimDO); AfxMessageBox(MsgBuff); return;}
	if(!SetEvent(*M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation)){AfxMessageBox("SetEvent() failed");}
	*M_CurrIdx_Of_StimGateRelayID=StimChanIdxCounter;//This is the index of M_StimDigitalOutChannels[].
	*M_HeyDASsv_StimGatingRequest=1;//Now request DAS_sv a job
	DisplayLED(StimDO, DIO_ON);
	
	//Just in case
	*M_IsOrthoStim=1;
	*M_IsAntidromicStim=0; 
}

int CTASDoc::GetIndexOfSimChannel(int StimDO) 
{
	int i;
	for(i=0;i<*M_NumOfStimulationChannels;i++){
		if(StimDO==M_StimDigitalOutChannels[i]){ //M_StimDigitalOutChannels[] channel list is maintained by ANTS using the "Antidromic Property" Dialog.
			return i;
		}
	}
	return -1;
}

//#############################################################################################################################################################
//##########################################################    Memory MAP   ##################################################################################
void CTASDoc::CreateSharedMemoryArea()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
	#include "MemMap_Open_DAS.h" 
}


void CTASDoc::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}

void CTASDoc::CloseTrialMemoryMAP()
{
	int	NumTrialsInMap=*M_NumTrlsInMAP;
	#include "MemMap_Close_Trials.h" 
}

////////////////////////////////////////////////////////////

void CTASDoc::CreateSharedMemoryArea_4TASVIS()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
#include "MemMap_Open_TASMOXYVIS.h"
}    

void CTASDoc::CloseMemoryMAP_4TASMOXYVIS()
{
	#include "MemMap_Close_TASMOXYVIS.h"
}


//#############################################################################################################################################################
//##########################################################   Statistics & usful functions   ###################################################################################################
#include "Statistics.h" 
//#############################################################################################################################################################
//##########################################################   COLOR MIX   ###################################################################################################
void CTASDoc::MyOriginalColorMix(int RedArray[],int GreenArray[],int BlueArray[], int arraySize) 
{	
	int R,G,B;
	double SqDist;
	COLORREF RGB;
	for(int i=0; i<arraySize; i++){
		//########  RED
		RedArray[i]=(255/(1+exp(-(((double)i)/arraySize-0.6)/0.03)));
		//########  GREEN
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.55);
		GreenArray[i]=255*exp(-SqDist/(arraySize*10.));
		//########  BLUE
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.30);
		BlueArray[i]=255*exp(-SqDist/(arraySize*6.));
	}
}

//#############################################################################################################################################################
//##########################################################   Communication & Replication of VIS   ###################################################################################################
#define REPLICATE_VIS
//########################  Communication with VIS  ################################
void CTASDoc::Send_Msg_to_VIS_and_Wait_untilFinishes(char *message)
{
	EnterCriticalSection(&m_CriticalSection4Msg);
	*M_MsgID_fromVIS_toTAS=0; //Reinitiating the communication. Only one message at a time is allowed.

	++m_MessageID; if(m_MessageID>=INT_MAX)m_MessageID=0;
	m_firsVIS_ErrorMsg=1;
	int ite=0;
	while(*M_MsgID_fromTAS_toVIS!=0){
		ite++; 
		if(ite>3000){
			ite=0; 
			if(m_firsVIS_ErrorMsg==1){AfxMessageBox("VIS is not responding?"); m_firsVIS_ErrorMsg=0;}
		}
		Sleep(1);
		if(m_IsPlayButtonON ==0|| *M_Is_VIS_Running==0)break; //When the VIS is not running do not wait to clear previous message.
	}
	strcpy(M_StringMsg_fromTAS_toVIS, message);   
	*M_MsgID_fromTAS_toVIS=m_MessageID;

	//Now wait until VIS is done.
	clock_t start=clock();
	m_firsVIS_ErrorMsg=1;
	while(1){
		if(*M_MsgID_fromVIS_toTAS!=0){
			if(*M_MsgID_fromVIS_toTAS==m_MessageID){
				*M_MsgID_fromVIS_toTAS=0;
				break;
			}
			else{
				AfxMessageBox("Received mismatched message ID from VIS");
				*M_MsgID_fromVIS_toTAS=0;
			}
		}
		// clock() is used instead of Sleep() to make this loop more responsive.
		if((clock()-start)>3000|| m_firsVIS_ErrorMsg==0){ 
			if(m_firsVIS_ErrorMsg==1){AfxMessageBox("Is VIS turned on?"); m_firsVIS_ErrorMsg=0;}
			Sleep(2);
		}
		if(m_IsPlayButtonON==0/*||m_bPause==1*/){ break;}
	}
	LeaveCriticalSection(&m_CriticalSection4Msg);
}

void CTASDoc::Send_Msg_to_MOXY_and_Wait_untilFinishes(char *message, int MaxWait_inMs)
{
	if(*M_isMOXY_Running==0)return;
	EnterCriticalSection(&m_CriticalSection4Msg);
	*M_MsgID_fromMOXY_toTAS=0; //Reinitiating the communication. Only one message at a time is allowed.

	strcpy(M_StringMsg_fromTAS_toMOXY, message);   
	++m_MessageID_toMOXY; if(m_MessageID_toMOXY>=INT_MAX)m_MessageID_toMOXY=0;
	*M_MsgID_fromTAS_toMOXY=m_MessageID_toMOXY;

	//Now wait until MOXY is done.
	clock_t start=clock();
	while(1){
		if(*M_MsgID_fromMOXY_toTAS!=0){
			if(*M_MsgID_fromMOXY_toTAS==m_MessageID_toMOXY){
				*M_MsgID_fromMOXY_toTAS=0;
				break;
			}
			else{
				AfxMessageBox("Received mismatched message ID from MOXY");
				*M_MsgID_fromMOXY_toTAS=0;
				break;
			}
		}
		// clock() is used instead of Sleep() to make this loop more responsive.
		if((clock()-start)>MaxWait_inMs){ 
			Sleep(2);
			if(*M_isMOXY_Running==0)break;
		}
		if(m_IsPlayButtonON==0/*||m_bPause==1*/){ break;}
	}
	LeaveCriticalSection(&m_CriticalSection4Msg);
}

void CTASDoc::Inform_MOXY(char *message)// Warning: this one does not do "Hand-shake" with MOXY. For this reason, if you manipulate the same drawing objects rapidly in succession, the result appearing on MOXY may not be as you expect. So use different objects for rapid sequential manipulations, such as different eye windows.
{
	Inform_MOXY_withoutHandshaking(message);
}

void CTASDoc::Inform_MOXY_withoutHandshaking(char *message)// Warning: this one does not do "Hand-shake" with MOXY. For this reason, if you manipulate the same drawing objects rapidly in succession, the result appearing on MOXY may not be as you expect. So use different objects for rapid sequential manipulations, such as different eye windows.
{
	strcpy(M_StrQ_fromTAS_toMOXY[*M_Msg_Q_Idx], message);   
	if(*M_Msg_Q_Idx+1<SIZE_OF_MSG_Q)(*M_Msg_Q_Idx)++; 
	else *M_Msg_Q_Idx=0;
}

//########################  Message Functions  ################################
//###################### Clear screen
void CTASDoc::PvisAllOff()                      
{	
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisAllOff"); //The message is to inform the VIS which function this one is requesting.
#ifdef REPLICATE_VIS
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisAllOff",100);
#endif
}
//###################### Set Background Color (For VIS & MOXY)
void CTASDoc::PvisSetBackColor(int red, int green, int blue)                    
{
	*M_backgroundScreenRGB=RGB(red, green, blue);
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisSetBackColor"); //The message is to inform the VIS which function this one is requesting.

#ifdef REPLICATE_VIS
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisSetBackColor",100);
#endif
}

//###################### Set the Color of the visual stimuli  (For VIS & MOXY)
void CTASDoc::PvisSetStimColors(int N, int ObjArray[], int Red[], int Green[], int Blue[])
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++)M_rgb[ObjArray[i]]=RGB(Red[i],Green[i],Blue[i]); 
	//This one is not necessary to ask the VIS to do anything.
	//It is used directly at the paining stage, such as in "FillSolidRect_ReportBackTopLeftXY()".
}

//###################### Set the positions   (For VIS & MOXY)
void CTASDoc::PvisStimLocation(int N, int ObjArray[], double X[], double Y[])
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++)M_cntrX[ObjArray[i]]=X[i]; 
	for(i=0;i<N;i++)M_cntrY[ObjArray[i]]=Y[i]; 
}

//###################### Draw Bar (For VIS & MOXY)
void CTASDoc::PvisDrawBar(int N, int ObjArray[], double width[], double hight[])
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++){
		M_VisObjIDs[i]=ObjArray[i]; 
	}
	//for(i=0;i<N;i++)M_angle[ObjArray[i]]=angle[i]; 
	for(i=0;i<N;i++)M_width[ObjArray[i]]=width[i]; 
	for(i=0;i<N;i++)M_hight[ObjArray[i]]=hight[i]; 
	//for(i=0;i<N;i++)M_Obj_Shape[ObjArray[i]]=contrast[i]; 
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisDrawBar"); //The message is to inform the VIS which function this one is requesting.

#ifdef REPLICATE_VIS
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=N;
	for(i=0;i<N;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=ObjArray[i]; 
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisDrawBar",100);
#endif
}

//###################### Draw Disk (For VIS & MOXY)
void CTASDoc::PvisDrawDisk(int N, int ObjArray[], double width[], double hight[])
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++){
		M_VisObjIDs[i]=ObjArray[i]; 
	}
	for(i=0;i<N;i++)M_width[ObjArray[i]]=width[i]; 
	for(i=0;i<N;i++)M_hight[ObjArray[i]]=hight[i]; 
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisDrawDisk"); //The message is to inform the VIS which function this one is requesting.

#ifdef REPLICATE_VIS
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=N;
	for(i=0;i<N;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=ObjArray[i]; 
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisDrawDisk",100);
#endif
}

//###################### Draw Picture (For VIS & MOXY)
void CTASDoc::PvisDrawPic(int N, int ObjArray[], double width[], double hight[])
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++)M_VisObjIDs[i]=ObjArray[i]; 
	//for(i=0;i<N;i++)M_angle[ObjArray[i]]=angle[i]; 
	for(i=0;i<N;i++)M_width[ObjArray[i]]=width[i]; 
	for(i=0;i<N;i++)M_hight[ObjArray[i]]=hight[i]; 
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisDrawPic"); //The message is to inform the VIS which function this one is requesting.

#ifdef REPLICATE_VIS
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=N;
	for(i=0;i<N;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=ObjArray[i]; 
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisDrawPic",100);
#endif
}

//###################### Switch ON/OFF (For VIS & MOXY)
void CTASDoc::PvisSwitchStim_with_ECODE(int N, int ObjArray[], int Switch[], int Ecode)
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++)M_VisObjIDs[i]=ObjArray[i]; 
	for(i=0;i<N;i++){
		M_Ojb_Switch[ObjArray[i]]=Switch[i]; 
	}
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisSwitchStim"); //The message is to inform the VIS which function this one is requesting.
	if(Ecode!=NO_ECODE)ECode(Ecode);

#ifdef REPLICATE_VIS
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=N;
	for(i=0;i<N;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=ObjArray[i]; 
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisSwitchStim",100);
#endif
}

void CTASDoc::PvisSwitchStim_with_ECODE_atThresholdcrossingInChX(int N, int ObjArray[], int Switch[], int Ecode, int ChX)
{
	int i;
	*M_N=N;
	for(i=0;i<N;i++)M_VisObjIDs[i]=ObjArray[i]; 
	for(i=0;i<N;i++){
		M_Ojb_Switch[ObjArray[i]]=Switch[i]; 
	}
	//request the job
	Send_Msg_to_VIS_and_Wait_untilFinishes("PvisSwitchStim"); //The message is to inform the VIS which function this one is requesting.
	//See when it gets physically implemented.
	if(Ecode!=NO_ECODE)Drop_myEcode_at_threshold_crossingInChX(Ecode, ChX, ON_SIGNAL_RISE, 100);

#ifdef REPLICATE_VIS
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=N;
	for(i=0;i<N;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=ObjArray[i]; 
	Send_Msg_to_MOXY_and_Wait_untilFinishes("PvisSwitchStim",100);
#endif
}

void CTASDoc::Drop_myEcode_at_threshold_crossingInChX(int myEcode, int ChX, int RiseOrFall, int dur_Wait)
{
	if(ChX<0 || ChX>=MAX_AI_CHAN){AfxMessageBox("ChX<0 || ChX>=MAX_AI_CHAN"); return;}
	if(dur_Wait<0){
		AfxMessageBox("dur_Wait cannot be negative"); return;
	}
	//Time keeping
	int TimeKeeperID=FindAvailableTimer();//TimeKeeperID MUST be obtained using FindAvailableTimer() function.
	if(TimeKeeperID==-1){AfxMessageBox("There is no available Timer left."); return;}
	if(m_durOfIdle[TimeKeeperID]!=0){
		AfxMessageBox("m_durOfIdle[TimeKeeperID]!=0");//The timers need to be 0 at the start of idling to make sure that the previous thread using the timer has ended, before launching another thread using the same timer. 
	}
	m_durOfIdle[TimeKeeperID]=dur_Wait;
	switch(TimeKeeperID){
		case 0:	AfxBeginThread( IDLE_FUNCTION_0,  this); break;
		case 1:	AfxBeginThread( IDLE_FUNCTION_1,  this); break;
		case 2:	AfxBeginThread( IDLE_FUNCTION_2,  this); break;
		case 3:	AfxBeginThread( IDLE_FUNCTION_3,  this); break;
		case 4:	AfxBeginThread( IDLE_FUNCTION_4,  this); break;
		case 5:	AfxBeginThread( IDLE_FUNCTION_5,  this); break;
		case 6:	AfxBeginThread( IDLE_FUNCTION_6,  this); break;
		case 7:	AfxBeginThread( IDLE_FUNCTION_7,  this); break;
		case 8:	AfxBeginThread( IDLE_FUNCTION_8,  this); break;
		case 9:	AfxBeginThread( IDLE_FUNCTION_9,  this); break;
		case 10:AfxBeginThread( IDLE_FUNCTION_10, this); break;
		case 11:AfxBeginThread( IDLE_FUNCTION_11, this); break;
		case 12:AfxBeginThread( IDLE_FUNCTION_12, this); break;
		case 13:AfxBeginThread( IDLE_FUNCTION_13, this); break;
		case 14:AfxBeginThread( IDLE_FUNCTION_14, this); break;
		case 15:AfxBeginThread( IDLE_FUNCTION_15, this); break;
		case 16:AfxBeginThread( IDLE_FUNCTION_16, this); break;
		case 17:AfxBeginThread( IDLE_FUNCTION_17, this); break;
		case 18:AfxBeginThread( IDLE_FUNCTION_18, this); break;
		case 19:AfxBeginThread( IDLE_FUNCTION_19, this); break;
	}

	int HasRisenOrFallen=0;
	double *pAI=M_AI[ChX], TH=M_Threshold[ChX];
	__int64 *pUsableAiIdx=M_usableAiIdx[ChX];

	do{
		if(*M_usableAiIdx[ChX]<0 ){
			AfxMessageBox("*M_usableAiIdx[ChX]<0");
			break;
		}
		if(RiseOrFall==ON_SIGNAL_RISE){
			if(pAI[*pUsableAiIdx]>TH){
				HasRisenOrFallen=1;
				break;
			}
		}
		else{
			if(pAI[*pUsableAiIdx]<TH){
				HasRisenOrFallen=1;
				break;
			}
		}
		if(!m_IsPlayButtonON){break; }
		Sleep(1);
	}while(m_durOfIdle[TimeKeeperID]>0);//Run until m_durOfIdle[TimeKeeperID]==0


	m_durOfIdle[TimeKeeperID]=0;//Terminate the time-keeping process even when the intended eye movement did not happen till the end of the timeout.
	m_TimerUsage[TimeKeeperID]=0; //##### !!!! This needs to be here NOT in the timer itself because the timer (IDLE_FUNCTION_1,2,3...) is being used even when it has timed out!!!! #####

	if(HasRisenOrFallen==1)ECode(myEcode);
	else{
		ECode(-myEcode);
		AfxMessageBox("The signal rise/fall has not been detected during the wait period.\n Check the signal or the threshold of the channel.");
	}
}

//###################### For MOXY #############################################################
//###################### Position Window Display (For MOXY)
void CTASDoc::positionBox_OnOff(int WindID, int Switch)//On and off of the window
{
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=1;
	for(int i=0;i<1;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=WindID; 
	M_PositionWinds[WindID].m_OnOff=Switch;
	Inform_MOXY_withoutHandshaking("PosBox_OnOff");
}

//###################### Display DIO LED (For MOXY)
void CTASDoc::DisplayLED(int whichBIT, int IsOn)
{
	if(whichBIT<0 || whichBIT>=NUM_DIO_LEDS){AfxMessageBox("whichBIT<0 || whichBIT>=NUM_DIO_LEDS"); return;}
	*M_N_Q_fromTAS_toMOXY[*M_Msg_Q_Idx]=1;
	for(int i=0;i<1;i++)M_VisObjID_Q_fromTAS_toMOXY[*M_Msg_Q_Idx][i]=whichBIT; 
	M_DIO_State[whichBIT]=IsOn;
	Inform_MOXY_withoutHandshaking("DisplayLED");
}






