// TAS_View.cpp : implementation of the CTASView class
//

#include "stdafx.h"
#include "TAS.h"
#include <math.h>

#include "Update_Me_TAS_Doc.h"
#include "TAS_View.h"
#include "Task_Constants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTASView
IMPLEMENT_DYNCREATE(CTASView, CView)

BEGIN_MESSAGE_MAP(CTASView, CView)
	//{{AFX_MSG_MAP(CTASView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_Parameters, OnChangeParameters)
	ON_COMMAND(ID_Functions, On_Saccade)
	ON_COMMAND(Quick_Function, On_1DR)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_TASK_SCHEDULER, OnTaskScheduler)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_MYMESSAGE_AUTO_DIO_ON_OFF, DIO_AutoOnOff_CallBack)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_MY_NEW_FIX, &CTASView::OnMyNewFix)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_MESSAGE(WM_MYMESSAGE_SOUND, BeepCallBack)
#include "Leave_me_inside_of_this_MESSAGE_MAP.h"
END_MESSAGE_MAP()
#include "Leave_me_at_the_bottom_of_this_MESSAGE_MAP.h"

/////////////////////////////////////////////////////////////////////////////


// CTASView construction/destruction
CTASView::CTASView()
{
	m_DoThisOnlyOnceToInformTheUser=1;
	m_SelectedVisualObjectID=0;
}					

CTASView::~CTASView() 
{

}


/////////////////////////////////////////////////////////////////////////////
BOOL CTASView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTASView drawing
void CTASView::OnDraw(CDC* pDC)
{
	#include "Starting_TAS.h" //<- Leave me alone

}
/////////////////////////////////////////////////////////////////////////////
// CTASView diagnostics ==> Leave these guys alone.
#ifdef _DEBUG
void CTASView::AssertValid() const
{
	CView::AssertValid();
}
void CTASView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CTASDoc* CTASView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTASDoc)));
	return (CTASDoc*)m_pDocument;
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////






// CTASView message handlers
///////////////////////////////////////////////////////////////////////////////////////////////////////
//############### TAS (TAsk Scheduler) ################
void CTASView::TaskScheduler() 
{
	//Give your functions below, just the name & ID. 
	//The function should be "void FunctionName()" type:
	Give_the_name_of_your_task_function_and_its_ID_here( OneDR_Task_Scheduler,		ONEDR_TASK		);
	Give_the_name_of_your_task_function_and_its_ID_here( Saccade_Task_Scheduler,	SCC_TASK		);
	Give_the_name_of_your_task_function_and_its_ID_here( MyNewFixTask,				MY_NEW_FIX_TASK	);
	Give_the_name_of_your_task_function_and_its_ID_here( OrthoStim_TaskScheduler,	ORTHO_STIM_TASK	);

	// Do you have anyting to initialize?
	InitVariables(); 
	
	//################   RUN   ########################//
	AfxBeginThread(TAS_THREAD, this);//<- Leave me alone. This implements the Memory Map variables (e.g. M_isTAS_Running) and runs your task program. For this reason, you need to access these variables after calling this TAS_THREAD.
}


//An example of message passing. You can modify this.
// This is in a Thread form, because it needs to turn on and off with a delay in between without stopping the whole program just for this.
LRESULT CTASView::BeepCallBack(WPARAM duration, LPARAM BeepID) 
{
	switch(BeepID){
	case 1:
		m_pDoc->Relay_ON_n_DisplayLED(CORRECT_BEEP);
		m_pDoc->SleepAccurate(duration,-1); 
		m_pDoc->Relay_OFF_n_DisplayLED(CORRECT_BEEP);
		break;
	case 2:
		m_pDoc->ECode(ERR_BEEP);
		m_pDoc->Relay_ON_n_DisplayLED(WRONG_BEEP);
		m_pDoc->SleepAccurate(duration,-1); 
		m_pDoc->Relay_OFF_n_DisplayLED(WRONG_BEEP);
		break;
	case 3:
		m_pDoc->Relay_ON_n_DisplayLED(CORRECT_BEEP);m_pDoc->Relay_ON_n_DisplayLED(RWD);m_pDoc->ECode(RWDCD);
		m_pDoc->SleepAccurate(duration,-1); 
		m_pDoc->Relay_OFF_n_DisplayLED(CORRECT_BEEP);m_pDoc->Relay_OFF_n_DisplayLED(RWD);m_pDoc->ECode(RWDOFFCD);		break;
	}
	return NULL;
}

void CTASView::OnUpdateTaskON(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_pDoc->m_IsPlayButtonON==1);
}

// This is in a Thread form because: it needs to turn on and off with a delay in between
LRESULT CTASView::DIO_AutoOnOff_CallBack(WPARAM duration, LPARAM DIO_ID) 
{
	m_pDoc->Relay_ON_n_DisplayLED(DIO_ID);
	m_pDoc->SleepAccurate(duration,-1); 
	m_pDoc->Relay_OFF_n_DisplayLED(DIO_ID);
	return NULL;
}

void CTASView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	KeyDown(nChar);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Pressing the tab key opens the channel RWD. Note: the same functions is in MOXY.
// To accept the key input, TAS window needs to be highlighted (by clicking it).
void CTASView::KeyDown(UINT nChar)
{
	switch(nChar){
		case VK_TAB: 
			PostMessage(WM_MYMESSAGE_AUTO_DIO_ON_OFF, 300/*duration*/,RWD);
			m_pDoc->Name_and_Value_to_record("Reward by Tab_key (ms)",300);
			break;
		case VK_SPACE: 
			break;
		case VK_DELETE: 
			break;
		default:
			break;
	}
}
