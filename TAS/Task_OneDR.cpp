
#include "stdafx.h"
#include <math.h>
#include "Update_Me_TAS_Doc.h"
#include "TAS_View.h"
#include "Task_Constants.h"


void CTASView::InitVariables_1DR()
{	
	m_isRepeatTrial=0; //This prevents a error where previous Tasks repeat trial becomes this tasks repeat trial there for skipping some initialization of parameters.
	//1DR 
	m_IsToGiveRandomRWD_1DR=1;
	m_TrialID=0;
	m_howManyDir=2;
	m_FixSize[0]=1; m_FixSize[1]=1;
	m_pDoc->Randomize();

	//Header to write to the file
	m_pDoc->Write_Your_Header_Here_to_Write_on_the_File("This is the 1DR task");
	m_pDoc->Write_Your_Header_Here_to_Write_on_the_File("Left reward:3001; Left no-reward: 3000; Right reward: 3011; Right no-reward: 3010");

	//Set objects with figure names
	m_pDoc->Give_ObjID_and_NameOfPic(0,"BabyMonkey.jpg"); //For Fix (see Task.cpp)
	m_pDoc->Give_ObjID_and_NameOfPic(2,"curious-george-eats-a-bunny.jpg"); //For Tgt (see Task.cpp)

	//Give the prefix and suffix for the files to save
	m_pDoc->Give_Prefix_Suffix_forFileName("R","DR");

	//To display any varialbe in MOXY, give it here.
	m_pDoc->Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Trial#",m_TrialNum, 0, USE_RIGHT_AXIS, NO_SHOW_IN_RUNNING_LINE_DISP,1,1);
	m_pDoc->Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Performance",performance, 1, USE_LEFT_AXIS,SHOW_IN_RUNNING_LINE_DISP,1,0);

	//#####  In case you want to display Ecodes in the "alias" form in the Running-Line Display, put them under here.
	//ECode_as_Word(TASK_START,"St"); 
	m_pDoc->ECode_as_Word(SHOWFIXCD,"OnFx"); 
	//ECode_as_Word(FIXSTARTCD,"FxSt"); 
	m_pDoc->ECode_as_Word(TGTCD,"Tgt"); 
	//ECode_as_Word(SAC_OCCURRED,"Sac"); 
	m_pDoc->ECode_as_Word(RWDCD,"ROn"); 
	m_pDoc->ECode_as_Word(RWDOFFCD,"ROff");
	//To reset the color of Fixation
	m_fixRed[0]=100*(tgtBrightness/10.); m_fixGreen[0]=100*(tgtBrightness/10.); m_fixBlue[0]=100*(tgtBrightness/10.); 
	*m_pDoc->M_Dur_BigRWD=200; m_pDoc->Inform_MOXY_withoutHandshaking("UpdateRwdRect");
}
void CTASView::Reset_OneDRTask()// ==> You can write your own reset function.
{
	//Whenever the task is initiated for the first time, initialize this
	BlockNum=0;
	m_StartNewBlock=1; 
	m_isRepeatTrial=0;
}

void CTASView::OneDR_Task_Scheduler()
{		
	__int64 savedTime;
	InitVariables_1DR();
	ParallelProgramsHere:
		AfxBeginThread( CATCH_WIND, this);  //(8)Wrong-saccade-catching Eye window  
		AfxBeginThread( ANTI_SHAKING, this);//Anti-shaking mechanism
	background:
		m_pDoc->PvisSetBackColor(m_VIS_BackgroundRed, m_VIS_BackgroundGreen, m_VIS_BackgroundBlue);
	/*############### The actual task loop is here ################*/
	while(m_pDoc->m_IsPlayButtonON){
			m_pDoc->PvisAllOff();                     /* Clear screen */
			m_pDoc->Use_the_REAL_EYE_position(m_EyeID);//The cliking (for eye position) is valid for the rest of the trial.
		//End:
			m_pDoc->EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell();//This properly finishes the previous trial. 
			if(m_pDoc->m_bPause){
				Reset_OneDRTask();//You can delete this, or make your own.
			}
			while(m_pDoc->m_bPause||m_pDoc->m_TaskType!=ONEDR_TASK){
				if(!m_pDoc->m_IsPlayButtonON||m_pDoc->m_TaskType!=ONEDR_TASK){m_IsCatchThreadRunning=0; m_IsAntiShakeThreadRunning=0; Sleep(100); return;}//Turn all the threads off first, and end the task.
				Sleep(10); 
			}
		//Start:
			m_pDoc->StartCollectingTrialData_Give_StartECode(TASK_START); //To inform TAS to register the beginning of the trial use this function.
		//variables:
			m_pDoc->ECode(setVariables_OneDR()); /* return value becomes an Em_EventCode*/
			m_pDoc->Give_TrialID_toSave(m_TrialNum); //If you skip this function, the TAS will increase the trial number by 1 every trial.
		//setcolor:
			m_pDoc->PvisSetStimColors(m_N_Fix,m_FixObjArray,m_fixRed,m_fixGreen,m_fixBlue); /*sets the color of the  foreground and background of the stimulus*/
		//timeOutPeriod:
			m_pDoc->SleepAccurate(dur_TurnOffDelay, -1); 
		//setFix:
			m_pDoc->PvisStimLocation(m_N_Fix, m_FixObjArray, m_FixX, m_FixY);
		//drawFix:
			//PvisDrawPic(m_N_Fix, m_FixObjArray, m_FixSize, m_FixSize);  
			m_pDoc->PvisDrawDisk(m_N_Fix, m_FixObjArray, m_FixSize, m_FixSize);  
		//setTgtLocation:
  			m_pDoc->PvisStimLocation(m_N_Tgt, m_TgtObjArray, m_TgtObjX, m_TgtObjY);
		//TgtColor:
			m_pDoc->PvisSetStimColors(m_N_Tgt, m_TgtObjArray,m_TgtRed,m_TgtGreen,m_TgtBlue); /*sets the color of the  foreground and background of the stimulus*/
		//drawTgtbar:
			//PvisDrawPic(m_N_Tgt, m_TgtObjArray, m_TgtSize, m_TgtSize);  
			m_pDoc->PvisDrawBar(m_N_Tgt, m_TgtObjArray, m_TgtSize, m_TgtSize);  
		//pre_task_null_period: 
			if(m_DoThisOnlyOnceToInformTheUser==1){m_DoThisOnlyOnceToInformTheUser=0; m_pDoc->Name_and_Value_to_record("Test 1!",123); m_pDoc->Name_and_ArrayOfValues_to_record("Test 2!",m_fixGreen,2);}
			Sleep(1000);
		//showFix:
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Fix_AllON, SHOWFIXCD);        
			//Use the following code if you would like to use a photo-diode to detact the onset of physical display.
			//m_pDoc->PvisSwitchStim_with_ECODE_atThresholdcrossingInChX(m_N_Fix, m_FixObjArray, m_Fix_AllON, SHOWFIXCD,FIRST_OTHER_CH_DIX); //Use this function if you would like to use a photo-diode to detact the onset of the physical display.
		//opnFixWnd:
			m_pDoc->ECode(65);
			SetAndOpenFixWnd();	
		//waitfix:
			m_pDoc->ECode(66);
			BeforeEscapeConditions_GiveDurationInMs(m_Dur_Wait_toFIX);
			//#########  Describe your escape conditions below  ######### 
			if(m_pDoc->IsEyeIn(posBOX0,m_EyeID)==1){
				savedTime=*m_pDoc->M_LatestTimeStampInMicroS; //in case this is a real fixation not just drifting. 
				if(m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,dur_toleratedPeriod4Fix,-1)==0)goto keepfx;
			}
			//#########  End of your escape conditions ################## 
			AfterEscapeConditions
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Fix_AllOFF, NO_ECODE);
			CloseFixWnd(); 
			goto nofx;
		keepfx:
			m_pDoc->ECode_with_Time(FIXSTARTCD,savedTime);
			if(m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,m_MinDurOfFix-dur_toleratedPeriod4Fix,dur_randDur4Fix)){m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Fix_AllOFF, NO_ECODE);CloseFixWnd(); goto failedSaccade;}
		//offfix:
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, m_Fix_AllOFF, NO_ECODE);
			if(m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,1,-1)){CloseFixWnd(); goto failedSaccade;}//Check the eye position for the last time
		//showTgt:
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Tgt, m_TgtObjArray, m_Tgt_AllON, TGTCD);
			//Use the following code if you would like to use a photo-diode to detact the onset of physical display.
			//m_pDoc->PvisSwitchStim_with_ECODE_atThresholdcrossingInChX(m_N_Tgt, m_TgtObjArray, m_Tgt_AllON, TGTCD, FIRST_OTHER_CH_DIX); //Use this function if you would like to use a photo-diode to detact the onset of the physical display.
//////////// Stimulation block//////////////////
//	if(!m_Dir && m_isRWD){
//			SleepAccurate(70,-1);
//		StimStart:
//			ECode(STIM_CD);
//			Relay_ON_n_DisplayLED(DO_STIM_CHAN); //Trigger the stimulation
//		StimDur:
//			Sleep(1); //The duration of the trigger signal
//		StimEnd:
//			Relay_OFF_n_DisplayLED(DO_STIM_CHAN);
//	}
//////////////////////////////
		//opnTgtwnd:
			CloseFixWnd();
			OneDR_Set_and_OpenOrClose_TgtWnd(BOX_ON);
			if(shaking==1)goto offTgt;
		//waitInitSac:
			if(!m_pDoc->WaitUntilEyeIsOut(posBOX0,m_EyeID,m_forTgt_Dur_ofWait_toInitSaccade,-1)){OneDR_Set_and_OpenOrClose_TgtWnd(0); goto failedSaccade;} // If the eye does not leave the fixation window...   !!!
		//GoingToTgt:
			m_pDoc->ECode(SAC_OCCURRED);
			if(m_N_Tgt==1 && fabs((float)m_TgtObjX[0])>1)setCatchWnd(); 
			BeforeEscapeConditions_GiveDurationInMs(dur_waitScd2HitTgt);
			//#########  Describe your escape conditions below  ######### 
				if(wrongSacDir==1){clearCatchWnd(); OneDR_Set_and_OpenOrClose_TgtWnd(0); goto failedSaccade; }
				if(m_pDoc->IsEyeIn(posBOX2,m_EyeID)==1){clearCatchWnd();goto gazeHold;}
			//#########  End of your escape conditions ################## 
			AfterEscapeConditions
			clearCatchWnd(); 
			OneDR_Set_and_OpenOrClose_TgtWnd(BOX_OFF); 
			goto LooksAway;
		gazeHold:
			if(m_pDoc->WaitUntilEyeIsOut(posBOX2,m_EyeID,dur_GazeHold, dur_randDur4TgtGazeHold)){OneDR_Set_and_OpenOrClose_TgtWnd(0); goto failedSaccade;}
			if(1==shaking)goto offTgt;
		//######### RWD ############
		m_pDoc->Monitor_EyeOutOfBox_thread(posBOX2,m_EyeID, 1000, 0);//It is a thread running in parallel with the reward and more. It terminates either 1) when the eye leaves the target window, which triggers an ECode, EYE_OUT_OF_TGT, or 2) when the specified time-limit (e.g. 2000ms) has reached. 
		//rwd:
			m_pDoc->Relay_ON_n_DisplayLED(RWD);
			m_pDoc->ECode(RWDCD);
		//setRWDtoneFlg:
			Sound_RwdTone();
		//rwddur:
			m_pDoc->SleepAccurate(m_Dur_RWD,-1);
		//##########################
		//rwdoff:         
			m_pDoc->ECode(RWDOFFCD);
			m_pDoc->Relay_OFF_n_DisplayLED(RWD);
		correctfix:
			CorrectTrial();
			m_pDoc->ECode(CORRECTCD);
			goto offTgt;
		nofx:
			m_pDoc->ECode(NOFIX);
			NoFixTrial();
			goto offTgt; 
		LooksAway:
			m_pDoc->ECode(AWAY_LOOK_TRL);
			FailedSaccade();
			goto offTgt; 
		failedSaccade:
			m_pDoc->ECode(FAILED_SAC);
			FailedSaccade();
		offTgt:
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Tgt, m_TgtObjArray, m_Tgt_AllOFF, NO_ECODE);
			OneDR_Set_and_OpenOrClose_TgtWnd(0); 
		//postTaskDur: 
			m_pDoc->Sleep_NOT_Accurate((int)(dur_MinITI/2), dur_LengthOfRandITI);
		//RandomRWD:
			//Give a random RWD during the ITI period.
			//But only after a correct trial. Otherwise the monkey may be sleeping or ideling.
			if(m_IsToGiveRandomRWD_1DR && correct &&PROBABILITY(0.1)){
				m_pDoc->Relay_ON_n_DisplayLED(RWD);
				m_pDoc->ECode(RND_RWDCD);
				m_pDoc->SleepAccurate(300,-1);
				m_pDoc->ECode(RND_RWDOFFCD);
				m_pDoc->Relay_OFF_n_DisplayLED(RWD);
				m_pDoc->Sleep_NOT_Accurate((int)(dur_MinITI/2), dur_LengthOfRandITI);//Give more time
			}
	}
}

int CTASView::setVariables_OneDR()
{
	int em_EventCode = 3000,i;
	int MaxSameAction;
	double Performance111;

	if(m_pDoc->m_TaskType==ONEDR_TASK){
		if(m_StartNewBlock==1 || !m_isRepeatTrial){ //NOT a repeat trial due to a previous error trial.
			m_TrialNum++;
			if(m_StartNewBlock==0 && m_TrialNum>TrialNumInOneBlock){
				m_StartNewBlock=1;
			}
			if(m_StartNewBlock==1){
				m_StartNewBlock=0;
				BlockNum++; m_TrialNum=0; rewSide=BlockNum%m_howManyDir;
			}		
			//Set direction and reward
			if(m_RwdProbability==50){
				MaxSameAction=3+Random(2); // 0 (LEFT) or 1 (RIGHT)
				direction=Random(m_howManyDir); // 0 (LEFT) or 1 (RIGHT)	 
				if(rewSide==direction){
					isBigReward=1;
				}
				else{
					isBigReward=0;
				}
				if(isBigReward){ConsecutiveReward++; ConsecutiveNonReward=0;}
				else{ConsecutiveNonReward++;  ConsecutiveReward=0;}
				if(m_howManyDir==2){
					if(ConsecutiveNonReward>MaxSameAction){direction=!direction; isBigReward=1; ConsecutiveNonReward=0; ConsecutiveReward++;}
					if(ConsecutiveReward>MaxSameAction){direction=!direction; isBigReward=0; ConsecutiveReward=0; ConsecutiveNonReward++;}
				}
			}
			else{
				if(PROBABILITY((double)m_RwdProbability/100.)){//Reward trial
					direction=rewSide; 
					isBigReward=1;
				}
				else {//No reward trial
					SelectAgain:	direction=Random(m_howManyDir);
					if(m_howManyDir>1 && direction==rewSide)goto SelectAgain;
					isBigReward=0;
				}
			}
			m_N_Tgt=1;
		}
		for(i=0;i<m_N_Tgt;i++){
			m_TgtRed[i]=100*(tgtBrightness/10.); m_TgtGreen[i]=100*(tgtBrightness/10.); m_TgtBlue[i]=100*(tgtBrightness/10.); 
		}
		for(i=0;i<m_N_Tgt;i++){m_TgtSize[i]=m_UserTgtSize; }
		em_EventCode = 3000+m_pDoc->m_TaskType*100+direction*10+isBigReward;  //right big
	}
	switch(direction){
	case 1:  //Right
		m_TgtObjX[0]=tgt_ViewAngle; 
		m_TgtObjY[0]=0;
		break;
	case 0:  //Left
		m_TgtObjX[0]=-tgt_ViewAngle; 
		m_TgtObjY[0]=0;
		break;
	case 2:  //Up
		m_TgtObjX[0]=0; 
		m_TgtObjY[0]=tgt_ViewAngle;
		break;
	case 3:  //Down
		m_TgtObjX[0]=0; 
		m_TgtObjY[0]=-tgt_ViewAngle;
		break;
	}

	m_N_Fix=1;

	if(dur_MinITI<1000)dur_MinITI=1000; 
	//SleepAccurate("itidly",    (int)(dur_MinITI/2),  0); // = (dur_MinITI/2)+ 0
	if(isBigReward)	{
		m_Dur_RWD=(*m_pDoc->M_Dur_BigRWD);
	}
	else   {
		m_Dur_RWD=dur_smlRWD;
	}

	///////////////////////////////////////
	#define OFF_TASK_MIN 1
	performanceSHORT     +=0.1  *((100-performanceSHORT     )*correct+(0-performanceSHORT     )*incorrect);
	performanceLONG  +=0.01*((100-performanceLONG  )*correct+(0-performanceLONG  )*incorrect);	
	performance=performanceSHORT/2+performanceLONG/2;
	if(timeOutAfterIncorrect==1&&correct==0 &&timeOut>0 ){dur_TurnOffDelay=1000*timeOut; 	printf("TmOut\n"); noTaskSeconds+=timeOut; noTaskMin=noTaskSeconds/60;}
	else if(is_Antishaking==1 && shaking==1){dur_TurnOffDelay=1000*timeOut; 	printf("TmOut(shaking)\n"); noTaskSeconds+=timeOut;  shaking=0;}
	else {dur_TurnOffDelay=0; shaking=0;}
	correct=0; incorrect=0;

	m_isRWD=isBigReward;
	m_Dir=direction;

	return em_EventCode;
}

void CTASView::OneDR_Set_and_OpenOrClose_TgtWnd(int ON_or_OFF)
{
	int	secondWndSize;

	if(isBigReward)secondWndSize=m_TgtWinSize+1;
	else                 secondWndSize=0;
	m_pDoc->positionBox_pos(posBOX2, m_TgtObjX[0], m_TgtObjY[0]);            // specifies the position (in 10ths of degree) of the designated window. m_TgtObjX & m_TgtObjY should be LONG here
	m_pDoc->positionBox_pos(posBOX3, m_TgtObjX[0], m_TgtObjY[0]);    
	m_pDoc->positionBox_size(posBOX2, m_TgtWinSize, m_TgtWinSize);  // Specifies the size of a window. Size is speicifed as length from center of window to ONE SIDE.
	m_pDoc->positionBox_size(posBOX3, secondWndSize, secondWndSize);  
	m_pDoc->positionBox_OnOff( posBOX2,  ON_or_OFF);                  // Turns window on/off
	m_pDoc->positionBox_OnOff( posBOX3,  ON_or_OFF);
}




