
#include "stdafx.h"
#include "Update_Me_TAS_Doc.h"
#include "TAS_View.h"
#include "Task_Constants.h"


//########  Most the global functions here  #######

UINT CATCH_WIND(LPVOID WinObjPtr) //A global Function
{
	CTASView* pView=(CTASView*) WinObjPtr;
	if(pView->m_IsCatchThreadRunning==1){//See if there is any pre existing thread running already.
		AfxMessageBox("pView->m_IsCatchThreadRunning!=0");
	}
	pView->m_IsCatchThreadRunning=1; //Now it is running 
	pView->catchWin_Thread();
	pView->m_IsCatchThreadRunning=0; //Now it is off 
	return 0;
}

UINT ANTI_SHAKING(LPVOID WinObjPtr) //A global Function
{
	CTASView* pView=(CTASView*) WinObjPtr;
	if(pView->m_IsAntiShakeThreadRunning==1){//See if there is any pre existing thread running already.
		AfxMessageBox("pView->m_IsAntiShakeThreadRunning!=0");
	}
	pView->m_IsAntiShakeThreadRunning=1; //Now it is running 
	pView->anti_shaking_Thread();
	pView->m_IsAntiShakeThreadRunning=0; //Now it is off 
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CTASView::InitVariables()
{
	// Which one do you want to start at the begining? 
	m_pDoc->m_TaskType=SCC_TASK;

	m_TrialID=0;
	m_EyeID=0;  //This number should be:  0<= m_EyeID <= NUM_2D_BEHAVIOR_OBJS-1 
	
	/*Fixation*/
	m_N_Fix = 1;
	m_FixObjArray[0]=0; m_FixObjArray[1]=1;
	m_FixX[0]=0; m_FixX[1]=0;
	m_FixY[0]=0; m_FixX[1]=0;
	m_fixRed[0]=100;m_fixRed[1]=255;
	m_fixGreen[0]=100;m_fixGreen[1]=255;
	m_fixBlue[0]=100;m_fixBlue[1]=0;
	m_Fix_AllON[0]=1;m_Fix_AllON[1]=0;
	m_Fix_AllOFF[0]=0;m_Fix_AllOFF[1]=0;

	/*Target*/
	m_N_Tgt= 1;
	m_UserTgtSize=1.3;
	m_TgtObjArray[0]=2;m_TgtObjArray[1]=3;m_TgtObjArray[2]=4;m_TgtObjArray[3]=5;
	m_TgtObjX[0]=0;m_TgtObjX[1]=-40;m_TgtObjX[2]=0;m_TgtObjX[3]=0;
	m_TgtObjY[0]=0;m_TgtObjY[1]=  0;m_TgtObjY[2]=0;m_TgtObjY[3]=0;
	m_TgtSize[0]=9;m_TgtSize[1]=9;m_TgtSize[2]=9;m_TgtSize[3]=9;
	m_TgtRed[0]=100;m_TgtRed[1]=255;m_TgtRed[2]=100;m_TgtRed[3]=100;
	m_TgtGreen[0]=100;m_TgtGreen[1]=255;m_TgtGreen[2]=100;m_TgtGreen[3]=100;
	m_TgtBlue[0]=100;m_TgtBlue[1]=0;m_TgtBlue[2]=100;m_TgtBlue[3]=100;
	m_Tgt_AllON[0]=1;m_Tgt_AllON[1]=1;m_Tgt_AllON[2]=1;m_Tgt_AllON[3]=1;
	m_Tgt_AllOFF[0]=0;m_Tgt_AllOFF[1]=0;m_Tgt_AllOFF[2]=0;m_Tgt_AllOFF[3]=0;

	//Background brightness
	m_VIS_BackgroundRed= 0,  m_VIS_BackgroundBlue=0, m_VIS_BackgroundGreen=0;

	isBigReward=1, ConsecutiveNonReward=0, ConsecutiveReward=0;
	m_isRepeatTrial=0, direction=0;

	TrialCount=0;
	m_RwdProbability=50;

	#define INITIAL_RWD_SIZE 300

	dur_smlRWD=0;
	iti=2000;
	dur_MinITI=3000;
	RandomizeITI=1;
	dur_LengthOfRandITI=1000;
	m_FixWinSize = 3;
	m_TgtWinSize = 4;
	tgtBrightness=10;
	usertgtX=20, usertgtY=0; 
	tgt_ViewAngle=10;
	m_Dur_Wait_toFIX=1000;
	dur_toleratedPeriod4Fix=300;// Now there is no tolerated period after the eye gets into the fixation window. 
	m_MinDurOfFix=750;  
	dur_randDur4Fix=500;
	m_forTgt_Dur_ofWait_toInitSaccade=500;
	dur_waitScd2HitTgt=200;
	dur_GazeHold=500;
	dur_randDur4TgtGazeHold=200;
	rewSide=1;
	TrialNumInOneBlock=24;
	m_TrialNum=0;
	BlockNum=1;
	timeOutAfterIncorrect=0;
	timeOut=5;
	dur_TurnOffDelay=0;
	is_Antishaking=0;
	shaking=0;
	is_antiShakeBeep=0;
	is_CheckShaking=0;
	spinWithBeep=1;
	m_DurOfOff=300;

	correct=0, incorrect=0;
	wrongSacDir=0, catchWndON=0;
	performance=100, performanceSHORT=100, performanceLONG=100;

	noTaskSeconds=0,noTaskMin=0;

	m_IsCatchThreadRunning=0;m_IsAntiShakeThreadRunning=0; //To make sure that there no pre-existing thread running.
}

void CTASView::SetAndOpenFixWnd()
{
	SetAnd_OpenClose_FixWnd(BOX_ON);
}
void CTASView::CloseFixWnd()
{
	SetAnd_OpenClose_FixWnd(BOX_OFF);
}

void CTASView::SetAnd_OpenClose_FixWnd(int ON_or_OFF)
{
	if(m_pDoc->m_TaskType==ONEDR_TASK && direction==rewSide){
		m_pDoc->positionBox_pos(posBOX1, m_FixX[0], m_FixY[0]);      // specifies the position (in 10ths of degree) of the designated window. m_FixX & FIxY should be LONG here
		m_pDoc->positionBox_size(posBOX1, m_FixWinSize+1.5, m_FixWinSize+1.5);  // Specifies the size of a window. Size is speicifed as length from center of window to ONE SIDE.
		m_pDoc->positionBox_OnOff( posBOX1,  ON_or_OFF);							 // Turns window on/off
	}
	m_pDoc->positionBox_pos(posBOX0, m_FixX[0], m_FixY[0]);        // specifies the position (in 10ths of degree) of the designated window. m_FixX & FIxY should be LONG here
	m_pDoc->positionBox_size(posBOX0, m_FixWinSize, m_FixWinSize);		// Specifies the size of a window. Size is speicifed as length from center of window to ONE SIDE.
	m_pDoc->positionBox_OnOff( posBOX0,  ON_or_OFF);							// Turns window on/off
}

void CTASView::CorrectTrial()
{
	correct=1;
	m_isRepeatTrial=0;
}

void CTASView::NoFixTrial()
{
	incorrect=1;
	if(m_pDoc->m_TaskType==ONEDR_TASK)m_isRepeatTrial=1;
}

void CTASView::FailedSaccade()
{
	incorrect=1;
	if(m_pDoc->m_TaskType==ONEDR_TASK)m_isRepeatTrial=1;
	PostMessage(WM_MYMESSAGE_SOUND, 300,2);
}

void CTASView::SetWrongDirFlg()
{
	wrongSacDir=1;
	catchWndON=0; //Once the catch window catches a wrong saccade, there is no need to do it any more in a trial.
}


void CTASView::setCatchWnd()
{
 	wrongSacDir=0;

	//"catchWndON" does not need to be at the end of this function. 
	//What matters the most is that the catch eye window coordinates are well set: done by "WaitUntilEyeIsInOrOut()" which comes after this switch is on.
	catchWndON=1;

	m_pDoc->positionBox_pos(posBOX4, (-m_TgtObjX[0]), m_TgtObjY[0]);      
	m_pDoc->positionBox_size(posBOX4, 0.2*(*m_pDoc->M_PosFigMaxX- *m_pDoc->M_PosFigMinX),  0.9*(*m_pDoc->M_PosFigMaxY- *m_pDoc->M_PosFigMinY));
	m_pDoc->positionBox_OnOff(posBOX4,  BOX_ON);
}

void CTASView::clearCatchWnd()
{
 	wrongSacDir=0;
	catchWndON=0;
	m_pDoc->positionBox_size(posBOX4, 0 ,  0);
	m_pDoc->positionBox_OnOff(posBOX4,  BOX_OFF);
}

void CTASView::Sound_RwdTone()
{
	PostMessage(WM_MYMESSAGE_SOUND, (*m_pDoc->M_Dur_BigRWD),1);
}

void CTASView::clearShakingFlags()
{
	if(spinWithBeep){
		shaking=0;
		is_CheckShaking=0;
	}
	//SleepAccurate("SpinDly",  0, 3000); 
}


void CTASView::SetShaking_flag()
{
	if(is_antiShakeBeep && shaking==0)PostMessage(WM_MYMESSAGE_SOUND, NULL,3);

	//dur_TurnOffDelay=1000*30; 
	shaking=1;
	is_CheckShaking=0;
}



/*
 *	Catch Eye Window
 */
void CTASView::catchWin_Thread() 
{	
	wait4CheckCmmnd:
		while(1 != catchWndON){//Idle loop //Use a better method later  
			Sleep(1);if(!m_pDoc->m_IsPlayButtonON||m_IsCatchThreadRunning==0)return;
		} 
	ChckEye:
		while(1==catchWndON){ //When the user request monitoring of the eye with the catch-eye box (in this case posBOX4, continuously monitor it.
			if(m_pDoc->IsEyeIn(posBOX4,m_EyeID))goto SetWrongSacFlg; // If the eye gets into the catch window,
			Sleep(1); //Give the computer a break, 1ms.
			if(!m_pDoc->m_IsPlayButtonON||m_IsCatchThreadRunning==0)return;
		}
		goto wait4CheckCmmnd;
	SetWrongSacFlg:
		SetWrongDirFlg();
		goto wait4CheckCmmnd;
}



/*
 *	Anti-shaking
 */
void CTASView::anti_shaking_Thread() 
{	
	wait4AntiShakeCmmnd:
		while(1 != is_Antishaking){Sleep(1);if(!m_pDoc->m_IsPlayButtonON||m_IsAntiShakeThreadRunning==0)return;} //Use a better method later  
	ChckShaking:
		while(1 != is_CheckShaking){Sleep(1);if(!m_pDoc->m_IsPlayButtonON||m_IsAntiShakeThreadRunning==0)return;} //Use a better method later  
	ChckShakeRELAY:
		while(1==is_Antishaking){
			if(m_pDoc->WaitUntilSignalOn(INPUT_FROM_RELAY,signalSource,1))goto SetShakingFlg; 
		}
	SetShakingFlg:
		SetShaking_flag();
		goto ChckShaking; /*Going to ChckShaking is not to check the shaking for some time (e.g. 30 s) until the next trial.*/
}



