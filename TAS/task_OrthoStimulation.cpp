#include "stdafx.h"
#include "Update_Me_TAS_Doc.h"
#include "TAS_View.h"
#include "Task_Constants.h"


void CTASView::InitVariables_OrthoStim()
{
	m_isRepeatTrial=0; //This prevents a error where previous Tasks repeat trial becomes this tasks repeat trial there for skipping some initialization of parameters.
	//Stimulation 
	dur_preStmPeriod=500;
	dur_postStmPeriod=500;
	dur_varStmPeriod=500;
	numStm=0;
	m_pDoc->Randomize();
	
	//Header to write to the file
	m_pDoc->Write_Your_Header_Here_to_Write_on_the_File("This is the Orthodromic stimulation task.");

	//Give the prefix and suffix for the files to save
	m_pDoc->Give_Prefix_Suffix_forFileName("","OST");

	////To display any varialbe in MOXY, give it here.
	//Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Trial#",m_TrialNum, 0, USE_RIGHT_AXIS, NO_SHOW_IN_RUNNING_LINE_DISP,1,1);
	//Give_Nickname_VariableItself_FigIdx_Left0OrRight1Axis_IsToShowRunLine_MultiplyThis_AddThis("Performance",performance, 1, USE_LEFT_AXIS, SHOW_IN_RUNNING_LINE_DISP,1,0);

	//##### In case you want to display Ecodes in the "alias" form in the Running-Line Display, put them under here.
	m_pDoc->ECode_as_Word(TASK_START,"St"); 
	m_pDoc->ECode_as_Word(STIM_CD,"Stim"); 
}

void CTASView::OrthoStim_TaskScheduler()
{
	InitVariables_OrthoStim();
	//background:
	m_pDoc->PvisSetBackColor(m_VIS_BackgroundRed, m_VIS_BackgroundGreen, m_VIS_BackgroundBlue);
	/*############### The actual task starts from here ################*/
	while(m_pDoc->m_IsPlayButtonON){
		m_pDoc->PvisAllOff();                     /* Clear screen */
		m_pDoc->Use_the_REAL_EYE_position(m_EyeID);//The cliking (for eye position) is valid for the rest of the trial.
	//End:
		m_pDoc->EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell();//This properly finishes the previous trial. 
		while(m_pDoc->m_bPause||m_pDoc->m_TaskType!=ORTHO_STIM_TASK){
			if(!m_pDoc->m_IsPlayButtonON||m_pDoc->m_TaskType!=ORTHO_STIM_TASK){  m_IsAntiShakeThreadRunning=0; Sleep(100); return;}//Turn all the threads off first, and end the task.
			Sleep(10); 
		}
	//Start:
		m_pDoc->StartCollectingTrialData_Give_StartECode(TASK_START); //To inform TAS to register the beginning of the trial use this function.
		m_pDoc->Relay_ON_n_DisplayLED(DO_GATING_CHAN1); //Open the channel to let the stimulation go through
	//SetStimVars:
		if(m_DoThisOnlyOnceToInformTheUser==1){m_DoThisOnlyOnceToInformTheUser=0; m_pDoc->Name_and_Value_to_record("Test 1!",123); m_pDoc->Name_and_ArrayOfValues_to_record("Test 2!",m_fixGreen,2);}
		m_pDoc->Give_TrialID_toSave(++numStm); //If you skip this function, the TAS will increase the trial number by 1 every trial.
	//PreStim:
		m_pDoc->SleepAccurate(dur_preStmPeriod,-1); 
	//StimStart:
		m_pDoc->ECode(STIM_CD);
		m_pDoc->OrthodromicStimulation(); //Trigger the stimulation
	//PostStmPeriod:
		Sleep(dur_postStmPeriod); 
		Sleep(Random(dur_varStmPeriod));
	}
	m_pDoc->Relay_OFF_n_DisplayLED(DO_GATING_CHAN1); //Open the channel to let the stimulation go through
}

