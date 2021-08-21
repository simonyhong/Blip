
#include "stdafx.h"
#include "Update_Me_TAS_Doc.h"
#include "TAS_View.h"
#include "Task_Constants.h"


void CTASView::InitVariables_Saccade()
{
	m_isRepeatTrial=0; //This prevents a error where previous Tasks repeat trial becomes this tasks repeat trial there for skipping some initialization of parameters.
	m_TrialID=0;
	RandTgt=2; //Orderly visit possible target locations
	isFixOn=1;
	m_Dur_Wait_toFIX=1000;
	whereTgt=0;//The index of chosen target location (among a list of locations)
	m_TrialNum=0;

	/*Fixation*/
	m_N_Fix = 1;
	m_FixObjArray[0]=0; //I want to use the first object (0) in VIS to store the fixation properties.
	m_FixX[0]=0; m_FixY[0]=0;
	m_FixSize[0]=1; 
	m_fixRed[0]=100; m_fixGreen[0]=100;	m_fixBlue[0]=100; //The color of the fixation square
	m_Fix_AllON[0]=1;
	m_Fix_AllOFF[0]=0;
	m_pDoc->Randomize();
	
	/*Target*/
	m_N_Tgt= 1;
	m_TgtObjArray[0]=2;
	m_UserTgtSize=1.3;
	m_TgtObjX[0]=0;
	m_TgtObjY[0]=0;
	m_TgtSize[0]=9;
	m_TgtRed[0]=100; m_TgtGreen[0]=100;	m_TgtBlue[0]=100;
	m_Tgt_AllON[0]=1;
	m_Tgt_AllOFF[0]=0;

	*m_pDoc->M_Dur_BigRWD=150; m_pDoc->Inform_MOXY_withoutHandshaking("UpdateRwdRect");

	//The followig is given to give a default setting, in case the user wants to present his own target location.
	m_pDoc->PvisStimLocation(m_N_Tgt, m_TgtObjArray, m_TgtObjX, m_TgtObjY);
	m_pDoc->PvisSetStimColors(m_N_Tgt,m_TgtObjArray,m_TgtRed,m_TgtGreen,m_TgtBlue); /*sets the color of the  foreground and background of the stimulus*/
	m_pDoc->PvisDrawBar(m_N_Tgt, m_TgtObjArray, m_TgtSize, m_TgtSize);  
	m_pDoc->Give_ObjID_and_NameOfPic(0,"BabyMonkey.jpg"); //For Fix (see Task.cpp)
	m_pDoc->Give_ObjID_and_NameOfPic(2,"curious-george-eats-a-bunny.jpg"); //For Tgt (see Task.cpp)

	//Header to write to the file
	m_pDoc->Write_Your_Header_Here_to_Write_on_the_File("This is the Saccade task.");

	//Give the prefix and suffix for the files to save
	m_pDoc->Give_Prefix_Suffix_forFileName("","SC");
	////To display any varialbe in MOXY, give it here.
	//Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Trial#",m_TrialNum, 0, USE_RIGHT_AXIS, NO_SHOW_IN_RUNNING_LINE_DISP,1,1);
	//Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Performance",performance, 1, USE_LEFT_AXIS, SHOW_IN_RUNNING_LINE_DISP,1,0);

	//##### In case you want to display Ecodes in the "alias" form in the Running-Line Display, put them under here.
	m_pDoc->ECode_as_Word(TASK_START,"St"); 
	m_pDoc->ECode_as_Word(SHOWFIXCD,"OnFx"); 
	//m_pDoc->ECode_as_Word(FIXSTARTCD,"FxSt"); 
	m_pDoc->ECode_as_Word(TGTCD,"Tgt"); 
	m_pDoc->ECode_as_Word(SAC_OCCURRED,"Sac"); 
	m_pDoc->ECode_as_Word(RWDCD,"ROn"); 
	m_pDoc->ECode_as_Word(RWDOFFCD,"ROff");
}
void CTASView::Reset_Saccade()// ==> You can write your own reset function.
{
	//Whenever the task is initiated for the first time, initialize this
	BlockNum=0;
	m_TrialNum=0;
	m_isRepeatTrial=0;
}

#define ONE_OVER_ROOT2 0.7071
int CTASView::scc_setVariables()
{
	int em_EventCode=2000;
	isBigReward=1; //It's always big reward

	// Set the location of the Target 
	if(RandTgt==0){ //Fixed target
		m_TgtObjX[0]=usertgtX;
		m_TgtObjY[0]=usertgtY;
		/*Determine the direction*/
		if     (m_TgtObjX[0]>0  && m_TgtObjY[0]==0)em_EventCode = 2000;
		else if(m_TgtObjX[0]>0  && m_TgtObjY[0]>0 )em_EventCode = 2001;
		else if(m_TgtObjX[0]==0 && m_TgtObjY[0]>0 )em_EventCode = 2002;	
		else if(m_TgtObjX[0]<0  && m_TgtObjY[0]>0 )em_EventCode = 2003;
		else if(m_TgtObjX[0]<0  && m_TgtObjY[0]==0)em_EventCode = 2004;	
		else if(m_TgtObjX[0]<0  && m_TgtObjY[0]<0 )em_EventCode = 2005;
		else if(m_TgtObjX[0]==0 && m_TgtObjY[0]<0 )em_EventCode = 2006;	
		else if(m_TgtObjX[0]>0  && m_TgtObjY[0]<0 )em_EventCode = 2007;
	}	
	else{
		if(RandTgt==1) whereTgt=Random(8);
		else if(RandTgt==2) {whereTgt++; if(whereTgt>7)whereTgt=0;}
		switch(whereTgt){
		case 0:
			em_EventCode = 2000;
			m_TgtObjX[0]=tgt_ViewAngle; 
			m_TgtObjY[0]=0;
			break;
		case 1:
			em_EventCode = 2001;
			m_TgtObjX[0]= tgt_ViewAngle*ONE_OVER_ROOT2; 
			m_TgtObjY[0]= tgt_ViewAngle*ONE_OVER_ROOT2;
			break;
		case 2:
			em_EventCode = 2002;
			m_TgtObjX[0]=0; 
			m_TgtObjY[0]=tgt_ViewAngle;
			break;
		case 3:
			em_EventCode = 2003;
			m_TgtObjX[0]=-tgt_ViewAngle*ONE_OVER_ROOT2; 
			m_TgtObjY[0]= tgt_ViewAngle*ONE_OVER_ROOT2;
			break;
		case 4:
			em_EventCode = 2004;
			m_TgtObjX[0]=-tgt_ViewAngle; 
			m_TgtObjY[0]=0;
			break;
		case 5:
			em_EventCode = 2005;
			m_TgtObjX[0]=-tgt_ViewAngle*ONE_OVER_ROOT2; 
			m_TgtObjY[0]=-tgt_ViewAngle*ONE_OVER_ROOT2;
			break;
		case 6:
			em_EventCode = 2006;
			m_TgtObjX[0]=0; 
			m_TgtObjY[0]=-tgt_ViewAngle;
			break;
		case 7:
			em_EventCode = 2007;
			m_TgtObjX[0]= tgt_ViewAngle*ONE_OVER_ROOT2; 
			m_TgtObjY[0]=-tgt_ViewAngle*ONE_OVER_ROOT2;
			break;
		}
	}
	m_Dur_RWD=0;
	for(int i=0;i<m_N_Tgt;i++){m_TgtSize[i]=m_UserTgtSize; }

	return em_EventCode;
}

void CTASView::Saccade_Task_Scheduler()
{
	__int64 savedTime;
	InitVariables_Saccade();
	ParallelProgramsHere:
		AfxBeginThread( CATCH_WIND, this);  //(8)Wrong-saccade-catching Eye window  
		AfxBeginThread( ANTI_SHAKING, this);//Anti-shaking mechanism
	background:
		m_pDoc->PvisSetBackColor(m_VIS_BackgroundRed, m_VIS_BackgroundGreen, m_VIS_BackgroundBlue);
	/*############### The actual task starts from here ################*/
	while(m_pDoc->m_IsPlayButtonON){
		m_pDoc->PvisAllOff();                     /* Clear screen */
		m_pDoc->Use_the_REAL_EYE_position(m_EyeID);//The cliking (for eye position) is valid for the rest of the trial.
	//End:
		m_pDoc->EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell();//This properly finishes the previous trial. 
		if(m_pDoc->m_bPause){
			Reset_Saccade();//You can delete this, or make your own.
		}
		while(m_pDoc->m_bPause||m_pDoc->m_TaskType!=SCC_TASK){
			if(!m_pDoc->m_IsPlayButtonON||m_pDoc->m_TaskType!=SCC_TASK){m_IsCatchThreadRunning=0; m_IsAntiShakeThreadRunning=0; Sleep(100); return;}//Turn all the threads off first, and end the task.
			Sleep(10); 
		}
	//Start:
		m_pDoc->StartCollectingTrialData_Give_StartECode(TASK_START); //To inform TAS to register the beginning of the trial use this function.
	//scc_variables:
		m_pDoc->ECode(scc_setVariables());
		m_pDoc->Give_TrialID_toSave(++m_TrialNum); //If you skip this function, the TAS will increase the trial number by 1 every trial.
	//scc_setcolor:
		m_pDoc->PvisSetStimColors(m_N_Fix,m_FixObjArray,m_fixRed,m_fixGreen,m_fixBlue); /*sets the color of the  foreground and background of the stimulus*/
	//scc_itidly:
		if(m_DoThisOnlyOnceToInformTheUser==1){m_DoThisOnlyOnceToInformTheUser=0; m_pDoc->Name_and_Value_to_record("Test 1!",123); m_pDoc->Name_and_ArrayOfValues_to_record("Test 2!",m_fixGreen,2);}

		if(0==isFixOn)goto scc_setTgt; 
	//scc_setFix:
		m_pDoc->PvisStimLocation(m_N_Fix, m_FixObjArray, m_FixX, m_FixY);
		m_pDoc->PvisDrawBar(m_N_Fix, m_FixObjArray, m_FixSize, m_FixSize);  
	//scc_showFix:
		m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Fix_AllON, SHOWFIXCD);        
	//scc_opnFixWnd:
		SetAndOpenFixWnd();	
	//scc_waitfix:
		m_pDoc->ECode(66);
		BeforeEscapeConditions_GiveDurationInMs(m_Dur_Wait_toFIX);
		//#########  Describe your escape conditions below  ######### 
		if(m_pDoc->IsEyeIn(posBOX0,m_EyeID)==1){
			savedTime=*m_pDoc->M_LatestTimeStampInMicroS; //in case this is a real fixation not just drifting. 
			if(m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,dur_toleratedPeriod4Fix,-1)==0)goto scc_keepfx;
		}
		//#########  End of your escape conditions ################## 
		AfterEscapeConditions
		CloseFixWnd(); 
		goto nofx;
	scc_keepfx:
		m_pDoc->ECode_with_Time(FIXSTARTCD,savedTime);
		if(m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,m_MinDurOfFix-dur_toleratedPeriod4Fix,dur_randDur4Fix)){CloseFixWnd(); goto scc_failedSac;}
	//scc_offfix:
		m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Fix_AllOFF, NO_ECODE);
		CloseFixWnd(); 
	scc_setTgt:
		m_pDoc->PvisStimLocation(m_N_Tgt, m_TgtObjArray, m_TgtObjX, m_TgtObjY);
	//scc_TgtColor:
		m_pDoc->PvisSetStimColors(m_N_Tgt,m_TgtObjArray,m_TgtRed,m_TgtGreen,m_TgtBlue); /*sets the color of the  foreground and background of the stimulus*/
	//scc_drawTgt:
		m_pDoc->PvisDrawBar(m_N_Tgt, m_TgtObjArray, m_TgtSize, m_TgtSize);  
		//m_pDoc->PvisDrawPic(m_N_Tgt, m_TgtObjArray, m_TgtSize, m_TgtSize);  
		//m_pDoc->PvisDrawPic(m_N_Fix, m_FixObjArray, m_FixSize, m_FixSize);  
	//scc_showTgt:
		m_pDoc->PvisSwitchStim_with_ECODE(m_N_Tgt, m_TgtObjArray, m_Tgt_AllON, TGTCD);
	//scc_opnTgtwnd:
		OneDR_Set_and_OpenOrClose_TgtWnd(BOX_ON);
	//scc_waitInitSac:
		if(1==isFixOn && !m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,m_forTgt_Dur_ofWait_toInitSaccade,-1)){OneDR_Set_and_OpenOrClose_TgtWnd(0); goto scc_failedSac;} // If the eye does not leave the fixation window...   !!!
	//scc_GoingToTgt:
		m_pDoc->ECode(SAC_OCCURRED);
		if(1==wrongSacDir){OneDR_Set_and_OpenOrClose_TgtWnd(0); goto scc_failedSac;}
		if(!m_pDoc->WaitUntilEyeIsIn(posBOX2,m_EyeID,dur_waitScd2HitTgt,-1)){OneDR_Set_and_OpenOrClose_TgtWnd(0); goto scc_failedSac;} 
	//scc_gazeHold:
		if(!m_pDoc->WaitUntilEyeIsOut(posBOX2,m_EyeID,dur_GazeHold, dur_randDur4TgtGazeHold)){OneDR_Set_and_OpenOrClose_TgtWnd(0); goto scc_correctTrial;} 
		goto scc_failedSac;
	scc_correctTrial:
		m_Dur_RWD=(*m_pDoc->M_Dur_BigRWD);
	//######### RWD ############
	//rwd:
		m_pDoc->ECode(RWDCD);
		m_pDoc->Relay_ON_n_DisplayLED(RWD);
	//setRWDtoneFlg:
		Sound_RwdTone();
	//rwddur:
		m_pDoc->SleepAccurate(m_Dur_RWD,-1);
	//##########################
	//rwdoff:         
		m_pDoc->ECode(RWDOFFCD);
		m_pDoc->Relay_OFF_n_DisplayLED(RWD);
	//correctfix:
		CorrectTrial();
		m_pDoc->ECode(CORRECTCD);
		goto offTgt;
	nofx:
		m_pDoc->ECode(NOFIX);
		NoFixTrial();
		goto offTgt; 
	scc_failedSac:
		m_pDoc->ECode(FAILED_SAC);
		FailedSaccade();
	offTgt:
		m_pDoc->PvisSwitchStim_with_ECODE(m_N_Tgt, m_TgtObjArray, m_Tgt_AllOFF, NO_ECODE);
	//offFix:
		m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Tgt_AllOFF, NO_ECODE);
	//postTaskDur: 
		m_pDoc->SleepAccurate((int)(dur_MinITI/2), dur_LengthOfRandITI);
	}
}

