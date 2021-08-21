#include "stdafx.h"
#include "TAS.h"
#include "Update_Me_TAS_Doc.h"
#include "TAS_View.h"
#include "Task_Constants.h"

void CTASView::InitVariables_MyNewFixTask()
{	
	m_isRepeatTrial=0; //This prevents a error where previous Tasks repeat trial becomes this tasks repeat trial there for skipping some initialization of parameters.
	/*Fixation*/
	m_N_Fix = 1;
	m_FixObjArray[0]=0; //I want to use the first object (0) in VIS to store the fixation properties.
	m_FixX[0]=0; m_FixY[0]=0;
	m_FixSize[0]=1; 
	m_fixRed[0]=100; m_fixGreen[0]=100;	m_fixBlue[0]=100; //The color of the fixation square
	m_Fix_AllON[0]=1;
	m_Fix_AllOFF[0]=0;
	m_MinDurOfFix=50;
	m_FixWinSize=15;
	m_Dur_Wait_toFIX=2000;
	m_Dur_RWD=500; *m_pDoc->M_Dur_BigRWD=500;
	m_TrialID=0;
	m_FixAtCenter0_RandFix1_UserXY2=0; //By default, the center.

	//Set objects with figure names
	m_pDoc->Give_ObjID_and_NameOfPic(0,"BabyMonkey.jpg"); //For Fix (see Task.cpp)
	m_pDoc->Give_ObjID_and_NameOfPic(2,"curious-george-eats-a-bunny.jpg"); //For Tgt (see Task.cpp)

	//Header to write to the file
	m_pDoc->Write_Your_Header_Here_to_Write_on_the_File("This is the fixation task");
	m_pDoc->Write_Your_Header_Here_to_Write_on_the_File("The monkey gets juice just by fixating on the dot on the screen");

	//Give the prefix and suffix for the files to save
	m_pDoc->Give_Prefix_Suffix_forFileName("","Fx");
	//To display any varialbe in MOXY, give it here.
	m_pDoc->Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Trial#",m_TrialID, 0, USE_RIGHT_AXIS, NO_SHOW_IN_RUNNING_LINE_DISP,1 ,0);
	//To use a photo instead of a bar, give a name of a bmp file
	m_pDoc->Give_ObjID_and_NameOfPic(0,"BabyMonkey.jpg"); //For Fix (see Task.cpp)
}

void CTASView::MyNewFixTask()
{		
	InitVariables_MyNewFixTask();
	m_pDoc->PvisSetBackColor(m_VIS_BackgroundRed, m_VIS_BackgroundGreen, m_VIS_BackgroundBlue);
	m_pDoc->PvisAllOff(); // Clear screen
	/*############### The actual task loop is here ################*/
	while(m_pDoc->m_IsPlayButtonON){
			m_pDoc->Use_the_REAL_EYE_position(0/*Eye ID*/);//The cliking (for eye position) is valid for the rest of the trial.
		//End:
			m_pDoc->EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell();//This properly finishes the previous trial. 
			while(m_pDoc->m_bPause||m_pDoc->m_TaskType!=MY_NEW_FIX_TASK){
				if(!m_pDoc->m_IsPlayButtonON||m_pDoc->m_TaskType!=MY_NEW_FIX_TASK){return;}
				Sleep(10); 
			}
		//Start:
			m_pDoc->StartCollectingTrialData_Give_StartECode(TASK_START); //To inform TAS to register the beginning of the trial use this function.
			m_pDoc->Give_TrialID_toSave(++m_TrialNum); //If you skip this function, the TAS will increase the trial number by 1 every trial.
		//setFix:
			m_pDoc->PvisSetStimColors(m_N_Fix,m_FixObjArray,m_fixRed,m_fixGreen,m_fixBlue); //sets the color of the stimulus
			if     (m_FixAtCenter0_RandFix1_UserXY2==0){ m_FixX[0]=0; m_FixY[0]=0;}
			else if(m_FixAtCenter0_RandFix1_UserXY2==1){ m_FixX[0]=Random((int)(2*tgt_ViewAngle))-tgt_ViewAngle; m_FixY[0]=Random((int)(2*tgt_ViewAngle))-tgt_ViewAngle;}
			else{ m_FixX[0]=m_pDoc->M_cntrX[0]; m_FixY[0]=m_pDoc->M_cntrY[0];} //Assume the object 0 is used for expressing the user's right click.
			m_pDoc->PvisStimLocation(m_N_Fix, m_FixObjArray, m_FixX, m_FixY);
			m_pDoc->PvisDrawPic(m_N_Fix, m_FixObjArray, m_FixSize, m_FixSize);  
			//m_pDoc->PvisDrawBar(m_N_Fix, m_FixObjArray, m_FixSize, m_FixSize);  
		//pre_task_null_period: 
			if(m_DoThisOnlyOnceToInformTheUser==1){m_DoThisOnlyOnceToInformTheUser=0; m_pDoc->Name_and_Value_to_record("Test 1!",123); m_pDoc->Name_and_ArrayOfValues_to_record("Test 2!",m_fixGreen,2);}
			Sleep(1000);//Use "Sleep()" function only when the timing is not critical.
		//showFix:
			myFix_Switch[0]=1;
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, myFix_Switch, SHOWFIXCD);        
		//opnFixWnd:
			m_pDoc->positionBox_pos(posBOX0, m_FixX[0], m_FixY[0]);        // specifies the position (in 10ths of degree) of the designated window. m_FixX & FIxY should be LONG here
			m_pDoc->positionBox_size(posBOX0, m_FixWinSize, m_FixWinSize);		// Specifies the size of a window. Size is speicifed as length from center of window to ONE SIDE.
			m_pDoc->positionBox_OnOff(posBOX0,  BOX_ON);							// Turns window on/off
		//waitfix:
			//Total 20 (from 0~19) Timers are available. You can use anyone at a time, as long as you know that the timer is not used by anyother function at the same time.
			if(!m_pDoc->WaitUntilEyeIsIn(posBOX0/*Eye window ID*/,0/*EYE ID*/,m_Dur_Wait_toFIX /*How long*/,-1/*variable duration*/))goto nofx;
		//keepfx:
			m_pDoc->ECode(FIXSTARTCD);
			if(m_pDoc->WaitUntilEyeIsOut(posBOX0,0,m_MinDurOfFix,-1))goto failedSaccade;
		////offfix:
		//	myFix_Switch[0]=0;
		//	m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, myFix_Switch, NO_ECODE);
		//	m_pDoc->positionBox_OnOff(posBOX0,  BOX_OFF);							// Turns window on/off
		//######### RWD ############
		//rwd:
			m_pDoc->ECode(RWDCD);
			m_pDoc->Relay_ON_n_DisplayLED(RWD);
		//RWDtone:
			PostMessage(WM_MYMESSAGE_SOUND, *m_pDoc->M_Dur_BigRWD,1/*tone*/);
		//rwddur:
			m_pDoc->SleepAccurate(*m_pDoc->M_Dur_BigRWD,-1/*variable duration*/);//Use this function whenever timing is critical. But this function is "expensive" in terms of computing.
		//##########################
						//offfix:
							myFix_Switch[0]=0;
							m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, myFix_Switch, NO_ECODE);
							m_pDoc->positionBox_OnOff(posBOX0,  BOX_OFF);							// Turns window on/off
		//rwdoff:         
			m_pDoc->ECode(RWDOFFCD);
			m_pDoc->Relay_OFF_n_DisplayLED(RWD);
		correctfix:
			m_pDoc->ECode(CORRECTCD);
			goto closeEyeWnd;
		nofx:
			m_pDoc->ECode(NOFIX);
			goto offfixIfHasnt; 
		failedSaccade:
			PostMessage(WM_MYMESSAGE_SOUND, 300/*duration*/,2/*Error beep*/);
			m_pDoc->ECode(FAILED_SAC);
		offfixIfHasnt:
			myFix_Switch[0]=0;
			m_pDoc->PvisSwitchStim_with_ECODE(m_N_Fix, m_FixObjArray, myFix_Switch, NO_ECODE);
			m_pDoc->positionBox_OnOff(posBOX0,  BOX_OFF);							// Turns window on/off
		closeEyeWnd:
			m_pDoc->Sleep_NOT_Accurate(1000/*fixed duration*/, 500/*variable duration*/);
	}
}

//Dialog
#include "Dialog_MY_NEW_FIX_TASK.h"
void CTASView::OnMyNewFix()
{		
	CMY_NEW_FIX_TASK FixSizeDialog;

	FixSizeDialog.myNewFixSize=m_FixSize[0];
	FixSizeDialog.m_FixAtCenter0_RandFix1_UserXY2=m_FixAtCenter0_RandFix1_UserXY2;
	FixSizeDialog.m_Dur_Wait_toFIX=m_Dur_Wait_toFIX;
	FixSizeDialog.m_MinDurOfFix=m_MinDurOfFix;
	if(FixSizeDialog.DoModal()==IDOK){
		m_FixSize[0]=FixSizeDialog.myNewFixSize;
		m_FixAtCenter0_RandFix1_UserXY2=FixSizeDialog.m_FixAtCenter0_RandFix1_UserXY2;
		m_Dur_Wait_toFIX=FixSizeDialog.m_Dur_Wait_toFIX;
		m_MinDurOfFix=FixSizeDialog.m_MinDurOfFix;
	}
}