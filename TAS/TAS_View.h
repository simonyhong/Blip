// TAS_View.h : interface of the CTASView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAS_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_TAS_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTASDoc;

class CTASView : public CView
{
protected: // create from serialization only
	CTASView();
	DECLARE_DYNCREATE(CTASView)
public:
	//##############  Your new task here! #############

	//##############  Sample Program #############
	void	InitVariables_MyNewFixTask();
	void	MyNewFixTask();
	int		myFix_Switch[2], m_FixAtCenter0_RandFix1_UserXY2;
	//##############  1DR #############
	void Reset_OneDRTask();
	int		m_isRWD, m_Dir, m_IsToGiveRandomRWD_1DR;
	int		m_howManyDir,unexpectedAteveryX;
	void	InitVariables_1DR();
	int		setVariables_OneDR();
	void	OneDR_Task_Scheduler();
	void	OneDR_Set_and_OpenOrClose_TgtWnd(int ON_or_OFF);

	//##############  Saccade task #############
	void Reset_Saccade();
	int		m_SelectedVisualObjectID, m_RightClickX,m_RightClickY, m_PrvRightClickX,m_PrvRightClickY;
	int		whereTgt;
	int		RandTgt,isFixOn;
	void	InitVariables_Saccade();
	void	Saccade_Task_Scheduler();
	int		scc_setVariables();
	//###############  Orthodromic Stimulation  ##############
	int dur_preStmPeriod,dur_varStmPeriod,dur_postStmPeriod,numStm;
	void	InitVariables_OrthoStim();
	void	OrthoStim_TaskScheduler();


	////##############  Shared variables and functions across different tasks (you can modify, delete) #############
	int	m_EyeID;
	int	m_TrialID,m_StartNewBlock;
	int correct,incorrect,wrongSacDir,catchWndON;
	float performance, performanceSHORT, performanceLONG;
	int  noTaskSeconds, noTaskMin, dur_Spin;
	int isBigReward, ConsecutiveNonReward, ConsecutiveReward;
	int m_isRepeatTrial, direction;
	int	TrialCount,dur_wait4BttnPress;
	int	m_RwdProbability,m_Dur_RWD, dur_smlRWD;
	int	rl,iti,dur_MinITI,RandomizeITI,dur_LengthOfRandITI,m_FixWinSize,m_TgtWinSize,tgtBrightness;
	double usertgtX,usertgtY,tgt_ViewAngle;
	int	m_Dur_Wait_toFIX,dur_toleratedPeriod4Fix,m_MinDurOfFix,dur_randDur4Fix,dur_waitScd2HitTgt,m_forTgt_Dur_ofWait_toInitSaccade,dur_GazeHold,dur_randDur4TgtGazeHold;
	int rewSide,TrialNumInOneBlock,m_TrialNum,BlockNum,timeOutAfterIncorrect, timeOut;
	int dur_TurnOffDelay,is_Antishaking,shaking,is_antiShakeBeep,is_CheckShaking,spinWithBeep;
	int signalSource;
	int m_DoThisOnlyOnceToInformTheUser;

	/*Fixation*/
	int m_N_Fix;
	int m_FixObjArray[2], m_fixRed[2],m_fixGreen[2],m_fixBlue[2],m_Fix_AllON[2],m_Fix_AllOFF[2];
	double m_FixSize[2];
	double m_FixX[2], m_FixY[2];

	/*Target*/
	int m_N_Tgt, m_TgtObjArray[4],m_TgtRed[4], m_TgtGreen[4], m_TgtBlue[4],m_Tgt_AllON[4],m_Tgt_AllOFF[4];
	double m_TgtObjX[4], m_TgtObjY[4], m_TgtSize[4], m_UserTgtSize;

	 //Background brightness
	int m_VIS_BackgroundRed, m_VIS_BackgroundBlue, m_VIS_BackgroundGreen;
	int m_DurOfOff;

	//Some basic functions across different tasks
	void	InitVariables();
	void	SetAndOpenFixWnd();
	void	CloseFixWnd();
	void	SetAnd_OpenClose_FixWnd(int ON_or_OFF);
	void	CorrectTrial();
	void	NoFixTrial();
	void	FailedSaccade();
	void	SetWrongDirFlg();
	void	setCatchWnd();
	void	clearCatchWnd();
	void	Sound_RwdTone();
	void	clearShakingFlags();
	void	SetShaking_flag();


	//#######################################################################################################################################
	//Parallel Threads
	BOOL	m_IsCatchThreadRunning,m_IsAntiShakeThreadRunning, m_IsAvoidDetactThreadRunning;
	void	AvoidenceWin_Thread();
	void	catchWin_Thread();
	void	anti_shaking_Thread();
	void	KeyDown(UINT nChar);
	
//#######################################################################################################################################
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTASView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTASView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CTASView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void On2DGraph_II();
	afx_msg void OnChangeParameters();
	afx_msg void On_Saccade();
	afx_msg void On_1DR();
	afx_msg LRESULT DIO_AutoOnOff_CallBack(WPARAM, LPARAM);
	afx_msg LRESULT BeepCallBack(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTaskScheduler();
	afx_msg void OnUpdateTaskON(CCmdUI *pCmdUI);
	afx_msg void OnMyNewFix();
#include "Leave_me_at_the_bottom_of_this_class.h"
};

#ifndef _DEBUG  // debug version in TAS_View.cpp
inline CTASDoc* CTASView::GetDocument()
   { return (CTASDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAS_VISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
