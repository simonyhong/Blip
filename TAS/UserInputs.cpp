// TAS_View.cpp : implementation of the CTASView class
//

#include "stdafx.h"
#include "TAS.h"
#include "TAS_View.h"
#include "Task_Constants.h"
#include "Update_Me_TAS_Doc.h"

#include "DialogueBox.h"
#include "Dialogue2.h"
#include "Dialogue3.h"


//The Main Dialog Box
void CTASView::OnChangeParameters() 
{
	CDialogueBox	ParameterBOX;
	int RwdDuration=m_pDoc->GetBigRWD();//Not sure why VC++ wouldn't take "*m_pDoc->M_Dur_BigRWD"
	int taskType  =m_pDoc->GetTaskType();
	
	//sprintf(ParameterBOX.CaptionText,"Found!"); //How can I display the caption of the Dialog?
	int	PrvTask=taskType;
	ParameterBOX.m_TaskType  = taskType;
	ParameterBOX.m_dVariable2  = TrialNumInOneBlock;
	ParameterBOX.m_dVariable3  = m_RwdProbability;
	ParameterBOX.m_Variable4  = dur_smlRWD;
	ParameterBOX.m_Variable5  = RwdDuration;
	ParameterBOX.m_Variable6  = m_FixWinSize;
	ParameterBOX.m_Variable7  = m_TgtWinSize;
	ParameterBOX.m_Variable8  = tgt_ViewAngle;
	ParameterBOX.m_Variable9  = m_MinDurOfFix;
	ParameterBOX.m_Variable10 = dur_randDur4Fix;
	ParameterBOX.m_Variable11 = dur_toleratedPeriod4Fix;
	ParameterBOX.m_Variable12 = m_forTgt_Dur_ofWait_toInitSaccade;
	ParameterBOX.m_Variable13 = dur_waitScd2HitTgt;
	ParameterBOX.m_Variable14 = dur_GazeHold;
	ParameterBOX.m_Variable15 = dur_randDur4TgtGazeHold;
	ParameterBOX.m_Variable16 = dur_MinITI;
	ParameterBOX.m_Variable17 = dur_LengthOfRandITI;
	ParameterBOX.m_Variable18 = timeOut;
	ParameterBOX.m_Variable19 = m_DurOfOff;
	ParameterBOX.m_Variable20 = usertgtX;
	ParameterBOX.m_Variable21 = usertgtY;
	ParameterBOX.m_Variable22 = m_UserTgtSize;
	ParameterBOX.m_Variable23 = tgtBrightness;

	ParameterBOX.m_Check1 = timeOutAfterIncorrect;
	ParameterBOX.m_Check2 = is_Antishaking;
	ParameterBOX.m_Check3 = is_antiShakeBeep;

	if(ParameterBOX.DoModal()==IDOK){
		taskType				=ParameterBOX.m_TaskType ; m_pDoc->SetTaskType(taskType);
		TrialNumInOneBlock		=ParameterBOX.m_dVariable2 ;
		m_RwdProbability		=ParameterBOX.m_dVariable3 ;
		dur_smlRWD				=ParameterBOX.m_Variable4 ;
		RwdDuration				=ParameterBOX.m_Variable5; 	
		if(RwdDuration<0)RwdDuration=0; m_pDoc->SetBigRWD(RwdDuration);
		m_pDoc->Inform_MOXY("UpdateRwdRect");//Ask MOXY to update the reward amount display.
		m_FixWinSize				=ParameterBOX.m_Variable6 ;
		m_TgtWinSize				=ParameterBOX.m_Variable7 ;
		tgt_ViewAngle			=ParameterBOX.m_Variable8 ;
		m_MinDurOfFix		=ParameterBOX.m_Variable9 ;
		dur_randDur4Fix			=ParameterBOX.m_Variable10;
		dur_toleratedPeriod4Fix	=ParameterBOX.m_Variable11;
		m_forTgt_Dur_ofWait_toInitSaccade		=ParameterBOX.m_Variable12;
		dur_waitScd2HitTgt		=ParameterBOX.m_Variable13;
		dur_GazeHold			=ParameterBOX.m_Variable14;
		dur_randDur4TgtGazeHold	=ParameterBOX.m_Variable15;
		dur_MinITI				=ParameterBOX.m_Variable16;
		dur_LengthOfRandITI		=ParameterBOX.m_Variable17;
		timeOut					=ParameterBOX.m_Variable18;
		m_DurOfOff				=ParameterBOX.m_Variable19;
		usertgtX				=ParameterBOX.m_Variable20;
		usertgtY				=ParameterBOX.m_Variable21;
		m_UserTgtSize				=ParameterBOX.m_Variable22;
		tgtBrightness			=ParameterBOX.m_Variable23;

		timeOutAfterIncorrect	=ParameterBOX.m_Check1;
		is_Antishaking			=ParameterBOX.m_Check2;
		is_antiShakeBeep		=ParameterBOX.m_Check3;
	}
}

// 1DR
void CTASView::On_1DR() 
{
	CDialogue3	ParameterBOX;
	
	ParameterBOX.m_D3Var1  = m_howManyDir;
	//ParameterBOX.m_D3Var2  = unexpectedAteveryX;
	//ParameterBOX.m_D3Var3  = 0;//useDifferentColors;
	//ParameterBOX.m_D3Var4= ?;
	//ParameterBOX.m_D3Var5  = 0;//TgtType;
	//ParameterBOX.m_D3Var6= ?;
	//ParameterBOX.m_D3Var7  = 0;//changeAfterXcorrect;
	//ParameterBOX.m_D3Check1 = 0;//choiceDR;
	//ParameterBOX.m_D3Check2 = 0;//mendatoryChoice;

	if(ParameterBOX.DoModal()==IDOK){
		m_howManyDir			=ParameterBOX.m_D3Var1;
		//unexpectedAteveryX	=ParameterBOX.m_D3Var2;
		//useDifferentColors	=ParameterBOX.m_D3Var3;
		//?					=ParameterBOX.m_D3Var4;
		//TgtType				=ParameterBOX.m_D3Var5;
		//?					=ParameterBOX.m_D3Var6;
		//changeAfterXcorrect	=ParameterBOX.m_D3Var7;
		//choiceDR			=ParameterBOX.m_D3Check1;
		//mendatoryChoice		=ParameterBOX.m_D3Check2;
	}
}

// Saccade
void CTASView::On_Saccade() 
{
	CDialogue2	ParameterBOX;
	
	ParameterBOX.m_D2Var1  = RandTgt;
	ParameterBOX.m_D2Var2  = isFixOn;

	if(ParameterBOX.DoModal()==IDOK){
		RandTgt	=ParameterBOX.m_D2Var1 ;
		if(RandTgt>2){RandTgt=0; AfxMessageBox("RandTgt>2");}
		isFixOn	=ParameterBOX.m_D2Var2 ;
	}
}


