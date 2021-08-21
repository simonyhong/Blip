// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "XCOR.h"

#include "MainFrm.h"

#include "XCOR_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "XCOR_View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_REMEMBER, &CMainFrame::OnFileRemember)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//if (!m_wndStatusBar.Create(this) ||
	//	!m_wndStatusBar.SetIndicators(indicators,
	//	  sizeof(indicators)/sizeof(UINT)))
	//{
	//	TRACE0("Failed to create status bar\n");
	//	return -1;      // fail to create
	//}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

//To enable the auto position feature of the MainFrame window, follow the 2 steps, (1), (2) 
// (1) Define ROOT_PATH and include the following header:
#define ROOT_PATH "\\_Config_Files\\Window_Position_Files\\Root_XCOR.txt"
#include "RootFunctions_for_MainFrame.h"

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~FWS_ADDTOTITLE; //Do not show "Untitled-" 

  
	//#####  (2) Read root file to position the MainFraim  ##########
	if(RootFileOpen(ROOT_PATH)==1){//Is there a valid root file? 
		//Old format (absolute values)
		if(m_Height_in_screen>=1){
			cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 3; //Specifies the width of the new window.
			cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 4; //Specifies the height of the new window.
			cs.y  = ::GetSystemMetrics(SM_CYSCREEN) / 3; //Specifies the y-coordinate of the upper left corner of the new window. Coordinates are relative to the parent window if the new window is a child window; otherwise coordinates are relative to the screen origin.
			cs.x =  ::GetSystemMetrics(SM_CXSCREEN) / 2; //Specifies the x-coordinate of the upper left corner of the new window. Coordinates are relative to the parent window if the new window is a child window; otherwise coordinates are relative to the screen origin.
		}
		//New format (relative values)
		else{
			//Size
			cs.cy = RoundToClosestInt(::GetSystemMetrics(SM_CYSCREEN)*(m_Height_in_screen)); //Specifies the height of the new window.
			cs.cx = RoundToClosestInt(::GetSystemMetrics(SM_CXSCREEN)*(m_Width_in_screen)); //Specifies the width of the new window.
			//Position
			if(m_Top_in_screen>0.95){AfxMessageBox("m_Top_in_screen>0.95\nAdjusting"); m_Top_in_screen=0.95;} 
			if(m_Left_in_screen>0.95){AfxMessageBox("m_Left_in_screen>0.95\nAdjusting"); m_Left_in_screen=0.95;} 
			cs.y = RoundToClosestInt(::GetSystemMetrics(SM_CYSCREEN)*m_Top_in_screen);// y of the upper left corner of the new window. Coordinates are relative to the parent window if the new window is a child window; otherwise coordinates are relative to the screen origin.
			cs.x = RoundToClosestInt(::GetSystemMetrics(SM_CXSCREEN)*m_Left_in_screen); //x of the upper left corner of the new window. Coordinates are relative to the parent window if the new window is a child window; otherwise coordinates are relative to the screen origin.
		}
	}
	else{//Use the default configuration
		cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 3; //Specifies the width of the new window.
		cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 4; //Specifies the height of the new window.
		cs.y  = ::GetSystemMetrics(SM_CYSCREEN) / 3; //Specifies the y-coordinate of the upper left corner of the new window. Coordinates are relative to the parent window if the new window is a child window; otherwise coordinates are relative to the screen origin.
		cs.x =  ::GetSystemMetrics(SM_CXSCREEN) / 2; //Specifies the x-coordinate of the upper left corner of the new window. Coordinates are relative to the parent window if the new window is a child window; otherwise coordinates are relative to the screen origin.
	}
	//############################################################
	if(cs.cx<80) cs.cx=80; //Set the minimun so that it gets shown even when the user's monitor is small.
	if(cs.cy<76) cs.cy=76; //Set the minimun so that it gets shown even when the user's monitor is small.

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnClose() 
{
	CXCOR_View::m_bIsON=0;
	
	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	

	CFrameWnd::OnClose();
}

