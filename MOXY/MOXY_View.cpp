// MOXY_View.cpp : implementation of the CMOXY_View class
//

#include "stdafx.h"
#include "MOXY.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "Dialog_PrefixSuffix.h"
#include "Dialog_TriggerECode.h"
#include "Dialog_AxisScale.h"
#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"
#include "SinTable.h"


#include <afxmt.h>
#include "Mmsystem.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CMOXY_View::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CMOXY_View

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CMOXY_View, CView)

BEGIN_MESSAGE_MAP(CMOXY_View, CView)
	//{{AFX_MSG_MAP(CMOXY_View)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VT_DISCRIMINATOR, Launch_DAS_cl)
	ON_COMMAND(ID_LAUNCH_VIS, On_Launch_VIS)
	ON_COMMAND(ID_LAUNCH_RAS, On_Launch_RAS)
	ON_COMMAND(ID_LAUNCH_ANTS, On_Launch_ANTS)
	ON_COMMAND(ID_LAUNCH_OSCILLOSCOPE, On_Launch_Oscilloscope)
	ON_COMMAND(ID_XCOR, On_Launch_XCOR)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SMALLER_PANNELS, &CMOXY_View::OnSmallerPanels)
	ON_COMMAND(ID_BIGGER_PANNELS, &CMOXY_View::OnBiggerPanels)
	ON_MESSAGE(WM_MYMESSAGE_ANTIDROMIC_STIM_HAPPENED, AntidromicStim_Display_forMOXY)
	ON_MESSAGE(WM_MYMESSAGE_AUTO_DIO_ON_OFF, DIO_AutoOnOff_CallBack)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CLAS, &CMOXY_View::OnCLAS)
	ON_WM_KEYUP()
	ON_COMMAND(ID_CLOSE_BLIP, &CMOXY_View::OnCloseBlip)
	ON_COMMAND(ID_VIEW_RIGHT_CLICK_FIGURE, &CMOXY_View::OnViewRightClickFigure)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RIGHT_CLICK_FIGURE, &CMOXY_View::OnUpdateViewRightClickFigure)
	ON_COMMAND(ID_VIEW_VISIBLE_OBJs, &CMOXY_View::OnView_VisibleObjs)
	ON_COMMAND(ID_VIEW_ANGLE_MONITOR, &CMOXY_View::OnViewAngleMonitor)
	ON_COMMAND(ID_VIEW_CARTESIAN, &CMOXY_View::OnViewCartesian)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CARTESIAN, &CMOXY_View::OnUpdateViewCartesian)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANGLE_MONITOR, &CMOXY_View::OnUpdateViewAngleMonitor)
	ON_COMMAND(ID_VIRTUAL_ELECTRODE, &CMOXY_View::OnVirtualElectrode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CMOXY_View construction/destruction
CMOXY_View::CMOXY_View()
{
	// TODO: add construction m_EventCode here
	m_nColorShow=BLACK_BRUSH;
	m_nKey=0;
	m_FlagVirMemDC=0;
	m_bIsToggleON=0;
	m_bIsON=0;
	m_dHowManyRows=3; m_dSizeOfOneRow=400; m_dHowManyClmns=3; m_dSizeOfOneClmn=600;
	m_isInsidePane=0;
	m_DispEventIdx=0;
	m_isSaving=0;
	m_IsMemMap=0;
	sprintf(m_PreFix,"");sprintf(m_SuFFix,"");
	m_Fig[XY_FIG].m_pBaseOfFileName=m_BaseOfFileName;
	m_Fig[XY_FIG].m_pJustFileName=m_JustFileName;
	m_Fig[XY_FIG].m_pIsSaving=&m_isSaving;
	for(int i=0;i<MAX_GRAPHS;i++)m_Fig[i].m_FigID=i;
	m_NnCounterArrowDrawn=0;
	m_RWDCounterArrowDrawn=0;
	m_BChanArrowDrawn=0;
	for(int i=0; i<NUM_2D_BEHAVIOR_OBJS;i++){m_PrvTopleftX[i]=INT_MAX, m_PrvTopleftY[0]=INT_MAX;}
	m_EyeSquareWidth=6, m_EyeSquareHeight=6;

	////////////////////////////////////
	m_NullPen.CreatePen(PS_NULL, 1,RGB(0,0,0));
	m_RED.CreatePen(PS_SOLID, 1,RGB(255,0,0));
	m_WhitePen2.CreatePen(PS_SOLID, 2,RGB(255,255,255));
	m_RED2.CreatePen(PS_SOLID, 2,RGB(255,0,0));
	m_RED5.CreatePen(PS_SOLID, 5,RGB(255,0,0));
	m_RED8.CreatePen(PS_SOLID, 8,RGB(255,0,0));
	m_GREEN.CreatePen(PS_SOLID, 1,RGB(0,255,0));
	m_BLUE.CreatePen(PS_SOLID, 1,RGB(100,100,255));
	m_BLUE3.CreatePen(PS_SOLID, 3,RGB(100,100,255));
	m_BLUE5.CreatePen(PS_SOLID, 5,RGB(100,100,255));
	m_BLUE8.CreatePen(PS_SOLID, 8,RGB(100,100,255));
	m_GREEN3.CreatePen(PS_SOLID, 3,RGB(0,255,0));
	m_DkGreenPen.CreatePen(PS_SOLID, 1,RGB(0,155,0));
	m_CYAN.CreatePen(PS_SOLID, 1,RGB(0,255,255));
	m_CYAN3.CreatePen(PS_SOLID, 3,RGB(200,255,255));
	m_PINK.CreatePen(PS_SOLID, 1,RGB(255,150,150));
	m_PINK3.CreatePen(PS_SOLID, 3,RGB(255,150,150));
	m_BROWN.CreatePen(PS_SOLID, 1,RGB(240,130,0));
	m_VIOLET.CreatePen(PS_SOLID, 1,RGB(200,90,250));
	m_VIOLET2.CreatePen(PS_SOLID, 2,RGB(200,90,250));
	m_VIOLET3.CreatePen(PS_SOLID, 3,RGB(200,90,250));
	m_GRAY.CreatePen(PS_SOLID, 1,RGB(170,170,170));
	m_GrayPen2.CreatePen(PS_SOLID, 2,RGB(170,170,170));
	m_BlueGRAY.CreatePen(PS_SOLID, 1,RGB(210,210,240));
	m_BLACK.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_BLACK3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_BLACK5.CreatePen(PS_SOLID, 5,RGB(0,0,0));
	/////////////
	//SigmoidColorMix(m_RGB100,MaxRGB100);
	//GaussianColorMix(m_RGB100,MaxRGB100);
	//MyOriginalColorMix(m_RGB100,MaxRGB100);
	CosineColorMix(m_RGB100,MaxRGB100);

	int i,j;
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;
	LogBrh.lbColor=RGB(0,0,0);
	m_XY_BlackBrush.CreateBrushIndirect( &LogBrh );
	m_backgroundScreenPen.CreatePen(PS_SOLID, BACKGROUND_PEN_WIDTH,LogBrh.lbColor);//for non-Stimulus Display program
	//// They are arbitrarily created to be deleted and recreated later.
	//for(i=0;i<MAX_STIM_OBJ;i++){
	//	StimObj[i].m_foreBrushObj.CreateBrushIndirect( &LogBrh[0] );
	//	StimObj[i].m_bkBrushObj.CreateBrushIndirect( &LogBrh[0] );
	//}
	LOGBRUSH LogBrhIO;
	LogBrhIO.lbStyle=BS_SOLID;LogBrhIO.lbColor=RGB(250,0,0);
	m_DIO_ON_Brush.CreateBrushIndirect(&LogBrhIO);//for non-Stimulus Display program
	LogBrhIO.lbColor=RGB(255,255,255);
	m_DIO_backgroundBrush.CreateBrushIndirect( &LogBrhIO );

	m_bIsScrXYforIJ=0;
	/////////////// Parameters For Applications
	m_dVariable1=3;m_dVariable2=187;
	m_dVariable3=94;
	m_pXYFig=&m_Fig[XY_FIG];
	m_pRunLineFig=&m_Fig[FIRST_RUNNINGLINE_FIG];
	m_XY_Monitor_Scale=20;
	m_IsVertSeparatorHeld=0;
	m_IsHorizSeparatorHeld=0;
	m_VerticalRatioByDivider=0.7;
	m_isRunningLineSDF=0;
	m_isMOXY_Running=0;
	m_TotNumUserPickedVariables=0;
	m_PrevTicks=GetTickCount();

	for(i=0;i<MAX_NUM_OF_TIME_KEEPERS;i++){	m_TimerUsage[i]=0;}
	for(i=0;i<MAX_STIM_OBJ;i++)m_pPicture[i]=NULL;
	for(i=0;i<MAX_STIM_OBJ;i++)strcpy(m_SavedPicFileName[i],"");
	/////////////////
	m_futureVariable=50;
	m_UserClickedDIOIdx_ON=0;
	m_prvDate=GetDateString();
	InitializeCriticalSection(&m_CriticalSection);

	
	m_SelectedVisualObjectID=0;
	m_ReuseClickedPos=1;
	m_isRtBUTTON_pressed=0;
	m_FirstTime0ElementErr=1;
	TimerRotator=0; //Get rid of this once the timer bug is fixed.
	m_isFirstMessage=1;
	m_IsToShowFigureOnRightClick=0;
}					


CMOXY_View::~CMOXY_View() 
{
	m_bIsON=0; 
	Sleep(300);
	int i,s;
	for(i=0;i<MAX_STIM_OBJ;i++)	if(m_pPicture[i])m_pPicture[i]->Release(); //Release any memories that hold pictures and their infomation.

	if(m_IsMemMap==1){//In the case MOXY has not declared memory map, there is nothing to close.
		*M_isMOXY_Running=0;
		DestroySharedMemoryArea_4TASVIS();
		CloseTrialMemoryMAP();
		*M_NumTrlsInMAP=0;
		CloseSharedMemoryArea();
	}

	//Is the following necessary? Wouldn't the operating system do it for me?
	for(i=0;i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	m_DC0Mrr.DeleteDC();
	m_bmp0Mirror.DeleteObject();
	m_DIO_ON_Brush.DeleteObject();
	m_XY_BlackBrush.DeleteObject(); m_DIO_backgroundBrush.DeleteObject();
	m_WhiteBrushView.DeleteObject();
	m_backgroundScreenPen.DeleteObject();
	m_NullPen.DeleteObject(); m_WhitePen2.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject(); m_GREEN.DeleteObject();m_DkGreenPen.DeleteObject();m_CYAN.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();m_VIOLET.DeleteObject();
	m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_BLACK.DeleteObject();m_BLACK3.DeleteObject();m_GRAY.DeleteObject();m_GrayPen2.DeleteObject();m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED2.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	for(i=0;i<MAX_EYE_WIND;i++){
		m_PositionWindPens[i].DeleteObject();
	}
	m_font.DeleteObject();
	DeleteCriticalSection(&m_CriticalSection);
}

BOOL CMOXY_View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


UINT TASK_SCHEDULER(LPVOID WinObjPtr) //A global Function
{
	CMOXY_View* pView=(CMOXY_View*) WinObjPtr;
	pView->Monitoring_System() ;
	if(*(pView->M_Blip_OFF)==1){
		*pView->M_CLOSE_VIS=1;
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
	return 0;
}

UINT Running_Line_Display_Thread_1(LPVOID WinObjPtr) //A global Function
{
	CMOXY_View* pView=(CMOXY_View*) WinObjPtr;
	pView->Running_Line_Display(0) ;

	return 0;
}
UINT Running_Line_Display_Thread_2(LPVOID WinObjPtr) //A global Function
{
	CMOXY_View* pView=(CMOXY_View*) WinObjPtr;
	pView->Running_Line_Display(1) ;

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMOXY_View drawing
#define SIZE_OF_BITMAP_X 1
#define SIZE_OF_BITMAP_Y 0.5
void CMOXY_View::OnDraw(CDC* pDC)
{
#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CMOXYDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_isMOXY_Running==0){
		m_bIsON=1;
		VirMemDC();
		Re_measure_ClientView();
		m_Separator_inScreenCoord=(m_savedClientRect.right-m_savedClientRect.left)*0.5; 
		m_HorizontalSeparator_inScreenCoord=(m_savedClientRect.bottom-m_savedClientRect.top)*m_VerticalRatioByDivider; 
		//RootFileOpen();//Try to open a default root file.
		AfxBeginThread( TASK_SCHEDULER, this);
		while(m_isMOXY_Running==0){Sleep(1); if(m_bIsON==0)return; }//Wait until the MOXY finishes the initial work
		AfxBeginThread( Running_Line_Display_Thread_1, this);
		Sleep(20);
		AfxBeginThread( Running_Line_Display_Thread_2, this);
		Sleep(20);
		if(*M_isDAS_Running==0)Launch_DAS_sv();
	}
	else{
		CRect rt;
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			if(m_savedClientRect.right<rt.right ||m_savedClientRect.bottom<rt.bottom){
				if(m_savedClientRect.right>=SIZE_OF_BITMAP_X*m_nScrMaxX ||m_savedClientRect.bottom>=SIZE_OF_BITMAP_Y*m_nScrMaxY){
					int Y=SIZE_OF_BITMAP_Y*m_nScrMaxY;
					char ScreenMsg[1024]; sprintf(ScreenMsg,"Simon says: To save Bitmap memory, the size of the window is currently limited to Horizontal:%d, Vertical:%d\n To change this setting, adjust SIZE_OF_BITMAP_X and SIZE_OF_BITMAP_Y in the program.\n Let me know if you have an easier solution.",(int)(SIZE_OF_BITMAP_X*m_nScrMaxX),Y);
					AfxMessageBox(ScreenMsg);
				}
			}
			m_savedClientRect=rt;
			if(m_Separator_inScreenCoord>m_savedClientRect.right)m_Separator_inScreenCoord=m_savedClientRect.right; 
			m_HorizontalSeparator_inScreenCoord=m_savedClientRect.bottom*m_VerticalRatioByDivider;//Keep the ratio of the position
			ReCalibrateAllFigures();
			RequestDisplayEvent(REDRAW_ALL_FIGURES); 
		}
		else{
			RequestDisplayEvent(REFRESH_SCREEN);
		}
	}
}


void CMOXY_View::Re_measure_ClientView()
{			
	CRect rt;
	GetClientRect(&rt);
	m_savedClientRect=rt;
}

/////////////////////////////////////////////////////////////////////////////
// CMOXY_View printing

BOOL CMOXY_View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMOXY_View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMOXY_View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMOXY_View diagnostics

#ifdef _DEBUG
void CMOXY_View::AssertValid() const
{
	CView::AssertValid();
}

void CMOXY_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMOXYDoc* CMOXY_View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMOXYDoc)));
	return (CMOXYDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMOXY_View message handlers



void CMOXY_View::VirMemDC()
{
	if(m_FlagVirMemDC==1)return;
	//########  Preparing Virtual Window  ##########
	m_FlagVirMemDC=1;
////////////////////////////////
   LOGFONT lf;                        // Used to create the CFont.
   memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
   lf.lfWidth = 6;                   // Width of font
   lf.lfHeight = 13;                  // Request a 20-pixel-high font
   strcpy(lf.lfFaceName, "Times New Roman");//    with face name "Arial".
   m_font.CreateFontIndirect(&lf);    // Create the font.
   m_FontWidth=lf.lfWidth;
//////////////////////////////////////
   SetFont(&m_font);
	
	m_nScrMaxX=GetSystemMetrics(SM_CXSCREEN);
	m_nScrMaxY=GetSystemMetrics(SM_CYSCREEN);
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(255,255,255);	
	m_WhiteBrushView.CreateBrushIndirect( &LogBrh );
	//m_WhiteBrushView.CreateStockObject(WHITE_BRUSH);
	CClientDC aDC(this);
	
	for(int i=0;i<MaxDC;i++){
		m_DC[i].CreateCompatibleDC(&aDC);
		m_bmp[i].CreateCompatibleBitmap(&aDC, m_nScrMaxX*SIZE_OF_BITMAP_X, m_nScrMaxY*SIZE_OF_BITMAP_Y);//CreateCompatibleBitmap() sometimes fails when the video memoey (?) alocating the bitmap does not have enough space to meet the request.
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_WhiteBrushView);
		m_DC[i].SelectObject(&m_font);
		m_DC[i].SetBkColor(RGB(255,255,255));
		//m_DC[i].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
		if(i<NUM_RL_PANELS)m_RunPanel[i].m_pDC=&m_DC[i];
	}
	m_DC0Mrr.CreateCompatibleDC(&aDC);
	m_bmp0Mirror.CreateCompatibleBitmap(&aDC, m_nScrMaxX*SIZE_OF_BITMAP_X, m_nScrMaxY*SIZE_OF_BITMAP_Y);//CreateCompatibleBitmap() sometimes fails when the video memoey (?) alocating the bitmap does not have enough space to meet the request.
	m_DC0Mrr.SelectObject(&m_bmp0Mirror);
	m_DC0Mrr.SelectObject(&m_WhiteBrushView);
	m_DC0Mrr.SelectObject(&m_font);
	m_DC0Mrr.SetBkColor(RGB(255,255,255));
	m_SavedDC=m_DC[0].SaveDC();
	for(int i=0;i<MAX_GRAPHS;i++){
		m_Fig[i].m_pDC=&m_DC[0];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CMOXY_View::Init_Variables()
{	
	int i,j;

	*M_NnNumberToRecord=1; 	m_PrvNnNumberToRecord=*M_NnNumberToRecord;
	*M_JustClicked=0; 
	*M_isRenamed=0;
	M_EyeXsimulalted[0]=-100; M_EyeYsimulalted[0]=-100;M_EyeXsimulalted[1]=-100; M_EyeYsimulalted[1]=-100;
	m_RunPanel[0].m_pEyeX=&M_EyePosX[*M_BehavioralObjectForXYmonitor]; m_RunPanel[0].m_pEyeY=&M_EyePosY[*M_BehavioralObjectForXYmonitor];
	//Pens for the eye windows
	for(i=0;i<MAX_EYE_WIND;i++){
		j=(MaxRGB100-1)-i*(MaxRGB100/MAX_EYE_WIND);
		if(j<0)j=0;
		m_PositionWindPens[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
	}
	M_EyeX_rawData[0]=-100; M_EyeY_rawData[0]=-100;M_EyeX_rawData[1]=-100; M_EyeY_rawData[1]=-100;m_dataEyeXsaved[0]=-100; m_dataEyeYsaved[0]=-100;m_dataEyeXsaved[1]=-100; m_dataEyeYsaved[1]=-100;
	for(i=0;i<MAX_EYE_WIND;i++){
		M_PositionWinds[i].width=0;
		M_PositionWinds[i].hight=0;
		M_PositionWinds[i].cntrX=0;
		M_PositionWinds[i].cntrY=0;
		M_PositionWinds[i].LeftBoundary =M_PositionWinds[i].cntrX-M_PositionWinds[i].width/2.;
		M_PositionWinds[i].RightBoundary=M_PositionWinds[i].cntrX+M_PositionWinds[i].width/2.;
		M_PositionWinds[i].UpperBoundary=M_PositionWinds[i].cntrY+M_PositionWinds[i].hight/2.;
		M_PositionWinds[i].LowerBoundary=M_PositionWinds[i].cntrY-M_PositionWinds[i].hight/2.;
	}
	// If TAS is already running, get the info of the user-defined variables.
	UpdateFileName_StroredLocal_AND_Disp_Name_n_RecodingStatus();
}
//############### MOXY (MOintoring System) ################
void CMOXY_View::Monitoring_System() 
{
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//###### In case there has been some changes of the window size, redraw ######
	CRect rt;
	GetClientRect(&rt);
	m_savedClientRect=rt;
	//#######################################################################
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);// REALTIME_PRIORITY_CLASS (may make the Windows freeze), HIGH_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS 
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);// THREAD_PRIORITY_TIME_CRITICAL  THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_NORMAL  THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_LOWEST  THREAD_PRIORITY_IDLE 
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_CPUfrq);//The frequency of the processor (Tom says it's a one time measure at the booting of the computer)

	// Shared memory area for SIG (DAS)
	CreateSharedMemoryArea();
	// Shared memory area for MOXY/VIS
	CreateSharedMemoryArea_4TASVIS();
	InitUserData();
	*M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation=OpenEvent(EVENT_ALL_ACCESS,TRUE,"EVENT_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation");
	for(int monitorFig=0; monitorFig<NUM_RL_PANELS;monitorFig++){
		Connect_ChannelVariables_ToMemoryMap(monitorFig);
	}
	for(int i=0; i<MAX_DO_NUM-1;i++){M_DO_Array[i]=RELAY_OFF;}//The eighth one is for the antidromic stimulation
	//MOXY needs to run even without DAS-server. Because this the shared variables need to be treated more carefully.
	if(*M_isDAS_Running!=1)for(int i=0; i<MAX_AI_CHAN;i++){*M_usableAiIdx[i]=-1;}
	if(*M_isTAS_Running==1){//TAS is running
		UpdateRwdAmount_AND_Disp_it();
		m_Msg_Q_Idx=*M_Msg_Q_Idx;
	}
	else{ //TAS is NOT running
		*M_Dur_BigRWD=200; UpdateRwdAmount_AND_Disp_it();
		*M_isSaving=0;
		*M_Msg_Q_Idx=0;m_Msg_Q_Idx=*M_Msg_Q_Idx;
		*M_BehavioralObjectForXYmonitor=0; //Assume the first behavioral channel to display on XY-monitor
	}

	m_IsMemMap=1; //now the memory map has been declared.
	Init_Variables();

	{//This is to adjust the position of m_Separator_inScreenCoord to make the XY eyd window square.
		Init_XYwindow();//This is to measure the X dimensions
		m_Separator_inScreenCoord=m_pXYFig->m_nBy-m_pXYFig->m_nTy+2.5*m_FontWidth;//Make the figure more or less square
		Init_XYwindow();//This is to measure the X dimensions
		InitDIO_FIg();
	}
	RecalibratePanes();
	Init_XYwindow();
	Redraw_XYwindow();//Do not use RequestDisplayEvent(). This is because the display loop starts later.
	Read_MOXY_Config();

	DWORD PrvT=GetTickCount(), newT, PrvT4Monitor=GetTickCount(), newT4Monitor;
	int DispIdx=0, DispIdxDist, nextDispIDx, i, tgt_Q_Idx;
	CSingleLock SyncObj_Display(&m_sema); 

	m_isMOXY_Running=1; *M_isMOXY_Running=1; 
	if(m_Msg_Q_Idx>=0 && m_Msg_Q_Idx<SIZE_OF_MSG_Q)m_Msg_Q_Idx=*M_Msg_Q_Idx; else {*M_Msg_Q_Idx=0; m_Msg_Q_Idx=0;}

	//Event handlers
	while(*M_isDAS_Running!=1){if(m_bIsON==0)return; Sleep(100);}//Without this, the IO does not work properly; don't know why.
	char EvName[64];
	for(int i=0; i<MAX_DO_NUM; i++){ 
		sprintf(EvName,EventObjectsName,i);
		m_M_DO_Active[i] =OpenEvent(EVENT_ALL_ACCESS,TRUE,EvName);
		if(m_M_DO_Active[i]==NULL)AfxMessageBox("m_M_DO_Active[i]==NULL");
	}

	//Main Loop 1 (2D-XY monitor; Replication of VIS; all the display requests)
	int timeLapseInMs=0, NumberOfInitialServices=30;
	while(1){
		Sleep(1);
		//Initially, update the parameters once in a while so that even when TAS message was missed, MOXY picks it up soon. 
		if(NumberOfInitialServices>0 && timeLapseInMs++>1000){timeLapseInMs=0; NumberOfInitialServices--; UpdateFileName_StroredLocal_AND_Disp_Name_n_RecodingStatus();UpdateRwdAmount_AND_Disp_it();}

		if(m_bIsON==0 || *M_Blip_OFF==1)break;
		//########## Request from TAS
		if(*M_isTAS_Running==0){*M_Msg_Q_Idx=0; tgt_Q_Idx=*M_Msg_Q_Idx; m_Msg_Q_Idx=tgt_Q_Idx;}
		tgt_Q_Idx=*M_Msg_Q_Idx;
		if(m_Msg_Q_Idx!=tgt_Q_Idx){
			if(m_Msg_Q_Idx>tgt_Q_Idx){
				for(i=m_Msg_Q_Idx;i<SIZE_OF_MSG_Q;i++){
					Which_MOXY_Function(i);
				}
				for(i=0;i<tgt_Q_Idx;i++){
					Which_MOXY_Function(i);
				}
			}
			else{
				for(i=m_Msg_Q_Idx;i<tgt_Q_Idx;i++){
					Which_MOXY_Function(i);
				}
			}
			m_Msg_Q_Idx=tgt_Q_Idx; //Next Idx to implement.
		}
		//########## Replicate VIS
		if(*M_MsgID_fromTAS_toMOXY!=0){
			Which_ReplicatingVIS_Function(*M_MsgID_fromTAS_toMOXY);
			*M_MsgID_fromTAS_toMOXY=0;
		}
		//##########  Eye Position *DOT*
		if((newT=GetTickCount())-PrvT>10){//Draw every 10 ms (100Hz)
			if((*M_JustClicked)){		
				Get_Latest_XYpos();//When RunningLineDisp() is not running, this (user's click) will be the only way to update the eye position.	
			} 	
			PrvT=newT;
			SyncObj_Display.Lock();
			DrawNew_ObjPositions(&objDC);
			SyncObj_Display.Unlock();
		}
		//###########  Take care of Display requests  ##################
		if(DispIdx<=m_DispEventIdx)DispIdxDist=m_DispEventIdx-DispIdx;
		else DispIdxDist=MAX_DISP_REQUESTS-DispIdx+m_DispEventIdx;
		if(DispIdxDist>0){
			for(i=0;i<DispIdxDist; i++){
				if(DispIdx+1<MAX_DISP_REQUESTS)nextDispIDx=DispIdx+1; else nextDispIDx=0;
				if(i+1<DispIdxDist && m_DispEvents[DispIdx][0]==m_DispEvents[nextDispIDx][0]){
					if(m_DispEvents[DispIdx][1]>=1){//The number of arguments.
						if(m_DispEvents[DispIdx][2]==m_DispEvents[nextDispIDx][2]){//The 1st argument
							if(m_DispEvents[DispIdx][1]>=2){
								if(m_DispEvents[DispIdx][3]==m_DispEvents[nextDispIDx][3]){//The 2nd argument
									DispIdx=nextDispIDx; continue;//In case the next dispaly request is the same as the current one, skip the current one.
								}
							}
							else {DispIdx=nextDispIDx; continue;}//In case the next dispaly request is the same as the current one, skip the current one.
						}
					}
					else {DispIdx=nextDispIDx; continue;}//In case the next dispaly request is the same as the current one, skip the current one.
				}
				SyncObj_Display.Lock();
				Display_forMOXY(DispIdx,&objDC);	DispIdx=nextDispIDx;				
				SyncObj_Display.Unlock();
			}
		}
		//########### Turn on an LED of a DIO relay.
		if(*M_HeyMOXY_StimGivenToGatingDIO_X>=0){
			// This has a Thread form because: it needs to turn on and off with a delay in between
			PostMessage(WM_MYMESSAGE_ANTIDROMIC_STIM_HAPPENED, *M_HeyMOXY_StimGivenToGatingDIO_X,100);
			*M_HeyMOXY_StimGivenToGatingDIO_X=-1;
		}
		//########### In case the file number has changed, update the name.
		if(*M_NnNumberToRecord>0 && m_PrvNnNumberToRecord!=*M_NnNumberToRecord){Make_TheBaseOfFileName();}
	}
	m_bIsON=0;
	m_isMOXY_Running=0; *M_isMOXY_Running=0;
}


void CMOXY_View::Connect_ChannelVariables_ToMemoryMap(int FigID)
{
	m_RunPanel[FigID].m_RL_FigID=FigID;
	int monitorFigIdx=FigID+FIRST_RUNNINGLINE_FIG; if(monitorFigIdx<0||monitorFigIdx>=MAX_GRAPHS){AfxMessageBox("monitorFigIdx<0||monitorFigIdx>=MAX_GRAPHS"); exit(0);}
	m_RunPanel[FigID].m_pRLineFig=&m_Fig[monitorFigIdx];
	m_Fig[monitorFigIdx].m_pDC=m_RunPanel[FigID].m_pDC;
	m_RunPanel[FigID].pView=this;
	int	recordingChanIdx=m_RunPanel[FigID].m_RecordingSourceChanID;
	m_RunPanel[FigID].PensForChannel();
	//Shared memory map
	m_RunPanel[FigID].M_AI					=M_AI[recordingChanIdx];
	m_RunPanel[FigID].M_timeStampMicS		=M_timeStampMicS[recordingChanIdx];//This is shared across the channels
	m_RunPanel[FigID].M_AIQueueSize			=&M_AIQueueSize[recordingChanIdx];
	m_RunPanel[FigID].M_usableAiIdx			=M_usableAiIdx[recordingChanIdx];
	m_RunPanel[FigID].M_usableSIGi			=M_usableSIGi[recordingChanIdx];
	m_RunPanel[FigID].M_firstIdxOfWavelet	=M_firstIdxOfWavelet[recordingChanIdx];
	m_RunPanel[FigID].M_usableWaveIdx		=&M_usableWaveIdx[recordingChanIdx];
	if(m_RunPanel[FigID].m_RL_RecordingSourceNn==0){
		m_RunPanel[FigID].M_spikeTimeStampOfNn_RL=M_spikeTimeStampOfNn1[recordingChanIdx];
		m_RunPanel[FigID].M_usableSpikeIdxOfNn_RL=M_usableSpikeIdxOfNn1[recordingChanIdx];
		m_RunPanel[FigID].M_spike_IdxOfAIOfNn_RL=M_spike_IdxOfAIOfNn1[recordingChanIdx];
	}
	else if(m_RunPanel[FigID].m_RL_RecordingSourceNn==1){
		m_RunPanel[FigID].M_spikeTimeStampOfNn_RL=M_spikeTimeStampOfNn2[recordingChanIdx];
		m_RunPanel[FigID].M_usableSpikeIdxOfNn_RL=M_usableSpikeIdxOfNn2[recordingChanIdx];
		m_RunPanel[FigID].M_spike_IdxOfAIOfNn_RL=M_spike_IdxOfAIOfNn2[recordingChanIdx];
	}
	else if(m_RunPanel[FigID].m_RL_RecordingSourceNn==2){
		m_RunPanel[FigID].M_spikeTimeStampOfNn_RL=M_spikeTimeStampOfNn3[recordingChanIdx];
		m_RunPanel[FigID].M_usableSpikeIdxOfNn_RL=M_usableSpikeIdxOfNn3[recordingChanIdx];
		m_RunPanel[FigID].M_spike_IdxOfAIOfNn_RL=M_spike_IdxOfAIOfNn3[recordingChanIdx];
	}
	else {AfxMessageBox("Only 3 neurons are supported");}
	m_RunPanel[FigID].M_Spike_QueueSize	=M_Spike_QueueSize;
	m_RunPanel[FigID].M_LatestTimeStampInMicroS	=M_LatestTimeStampInMicroS;
	m_RunPanel[FigID].M_IsTriggeredMode=&M_IsTriggeredMode[FigID];
	m_RunPanel[FigID].M_nTriggerECode=&M_nTriggerECode[FigID];
	m_RunPanel[FigID].M_hasTriggered=&M_hasTriggered[FigID];
	m_RunPanel[FigID].M_TriggeredTime_ms=&M_TriggeredTime_ms[FigID];
	m_RunPanel[FigID].M_NumUserPickedVariables=M_NumUserPickedVariables;
}

// This is in a Thread form because:
// it needs to turn on and off with a delay in between
LRESULT CMOXY_View::AntidromicStim_Display_forMOXY(WPARAM WhichDO, LPARAM lParam) 
{
	RequestDisplayEvent(DIO_ON,DO_STIM_TRIGGER_CHAN);  
	RequestDisplayEvent(DIO_ON,WhichDO);  
	Sleep(lParam);
	RequestDisplayEvent(DIO_OFF,DO_STIM_TRIGGER_CHAN); 
	RequestDisplayEvent(DIO_OFF,WhichDO); 
	return NULL;
}


void CMOXY_View::RequestDisplayEvent(int Event) 
{
	RequestDisplayEvent(Event, 0, NULL,  NULL);
}
void CMOXY_View::RequestDisplayEvent(int Event,  int SomeIntValue) 
{
	RequestDisplayEvent(Event, 1, SomeIntValue,  NULL); 
}
void CMOXY_View::RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2) 
{
	RequestDisplayEvent(Event, 2, SomeIntValue1, SomeIntValue2);
}
void CMOXY_View::RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) 
{	
	m_DispEvents[m_DispEventIdx][0]=Event;
	m_DispEvents[m_DispEventIdx][1]=NumVariables;
	for(int i=0; i<NumVariables;i++){
		if(i==0)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue1; 
		else if(i==1)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue2; 
	}
	if(m_DispEventIdx+1<MAX_DISP_REQUESTS)m_DispEventIdx++; else m_DispEventIdx=0;
}

void CMOXY_View::Display_forMOXY(int RequestIdx, CDC *pObjDC)
{
	CFigure *pFig;

	switch(m_DispEvents[RequestIdx][0]){
		case DISP_POSITION_BOX:
			Turn_PositionBox_OnOff(m_DispEvents[RequestIdx][2], m_DispEvents[RequestIdx][3]);
			break;
		case DIO_ON:
			pFig=&m_Fig[DIO_FIG];
			pObjDC->SelectObject(&m_RED);
			pObjDC->SelectObject(&m_DIO_ON_Brush);
			pFig->Graph2D( DIO_DISP_X_POSITION(m_DispEvents[RequestIdx][2]), DIO_DISP_Y_POSITION(m_DispEvents[RequestIdx][2]),33, pObjDC);
			break;
		case DIO_OFF:
			pFig=&m_Fig[DIO_FIG];
			pObjDC->SelectObject(&m_RED);
			pObjDC->SelectObject(&m_DIO_backgroundBrush);
			pFig->Graph2D( DIO_DISP_X_POSITION(m_DispEvents[RequestIdx][2]), DIO_DISP_Y_POSITION(m_DispEvents[RequestIdx][2]),33, pObjDC);
			break;
		case REDRAW_ALL_FIGURES:
			//Redraw the DIO
			Redraw_DIO_Fig();			
			//Redraw the XY position window
			Redraw_XYwindow();
			pObjDC->BitBlt(0,0,m_Separator_inScreenCoord,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
			//Redraw the Running-line display
			if(m_isRunningLineSDF==1){
				for(int i=0;i<NUM_RL_PANELS;i++){
					m_RunPanel[i].Redraw_RunningLineFig();
				}
			}
			m_bIsScrXYforIJ=0;
			break;
		case REDRAW_RUNNINGLINE_FIG:
			m_RunPanel[m_DispEvents[RequestIdx][2]].Redraw_RunningLineFig();
			break;
		case REDRAW_XY_WINDOW_FIG:
			Redraw_XYwindow();
			break;
		case REDRAW_DIO_FIG:
			Redraw_DIO_Fig();
			break;
		case TOGGLE_TRIGGER_STATUS:
			m_RunPanel[m_DispEvents[RequestIdx][2]].DispTriggerStatus();
			break;
		case DISP_RECORDING_SOURCE:
			m_RunPanel[m_DispEvents[RequestIdx][2]].DispRecordingSource_Chan_Nn();
			break;
		case DISP_LABELS_OF_USER_PICKED_VAR:
			m_RunPanel[m_DispEvents[RequestIdx][2]].DisplayUserPickedVariables();
			break;
		case TOGGLE_SUPERIMPOSITION_STATUS:
			m_RunPanel[m_DispEvents[RequestIdx][2]].DispSupperimposeStatus();
			break;
		case REFRESH_SCREEN:
			//First, refresh the left part of the m_Separator_inScreenCoord (i.e. the DIO pannel, XY eye position window)
			pObjDC->BitBlt(0,0,m_Separator_inScreenCoord,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
			//Draw the running line dispaly figures.
			for(int i=0;i<NUM_RL_PANELS;i++){
				m_RunPanel[i].m_pRLineFig->RefreshFrame(pObjDC,m_RunPanel[i].m_pDC);
			}
			break;
		case DRAW_USER_RIGHT_CLICK_POS://This one is not used currently, because the user-clicked stimulus is shown in the XY_WINDOW directly.
			Draw_User_RtClick_Pos();
			break;
		case REFRESH_XY_TITLE_AREA:
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		case DECREASE_NEURON_NUM:
			Draw_DecreaseArrow(m_NnCounterArrowDrawn,m_pXYFig->m_FileName,m_DispEvents[RequestIdx][2],&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_FileName);
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		case INCREASE_NEURON_NUM:
			Draw_IncreaseArrow(m_NnCounterArrowDrawn,m_pXYFig->m_FileName,m_DispEvents[RequestIdx][2],&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_FileName);
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		case DECREASE_RWD:
			Draw_DecreaseArrow(m_RWDCounterArrowDrawn,m_pXYFig->m_RwdAmount,m_DispEvents[RequestIdx][2],&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_RwdAmount);
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		case INCREASE_RWD:
			Draw_IncreaseArrow(m_RWDCounterArrowDrawn,m_pXYFig->m_RwdAmount,m_DispEvents[RequestIdx][2],&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_RwdAmount);
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		case UPDATE_RWD:
			Draw_IncreaseArrow(m_RWDCounterArrowDrawn,m_pXYFig->m_RwdAmount,0,&m_DC[0]);
			Draw_DecreaseArrow(m_RWDCounterArrowDrawn,m_pXYFig->m_RwdAmount,0,&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_RwdAmount);
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		case DECREASE_B_CHAN:
			Draw_DecreaseArrow(m_BChanArrowDrawn,m_pXYFig->m_BehavioralObj,m_DispEvents[RequestIdx][2],&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_BehavioralObj);
			m_pXYFig->RefreshFrame(pObjDC,&m_DC[0]);
			break;
		case INCREASE_B_CHAN:
			Draw_IncreaseArrow(m_BChanArrowDrawn,m_pXYFig->m_BehavioralObj,m_DispEvents[RequestIdx][2],&m_DC[0]);
			m_pXYFig->DisplayLabel(m_pXYFig->m_BehavioralObj);
			m_pXYFig->RefreshFrame(pObjDC,&m_DC[0]);
			break;
		case DISP_FILE_NAME_AND_RECORDING_STATUS:
			m_pXYFig->DisplayFileName_and_RecordingStatus(this);
			m_pXYFig->RefreshXYTitleArea(pObjDC);
			break;
		default:
			break;
	}
}

void CMOXY_View::Draw_DecreaseArrow(int &Variable, ClickableLabel &Label, int isToHighlight, CDC *pObjDC) 
{
	CPen *pOldPen;
	CBrush *pOldBrush;
	//Delete the previous arrow
	pOldPen=m_DC[0].SelectObject(&m_NullPen);
	m_DC[0].SelectStockObject(WHITE_BRUSH);
	m_DC[0].Rectangle(Label.PrvWholeRect);
	
	if(isToHighlight==0){
		m_DC[0].SelectObject(&m_GrayPen2);
		Variable=1;
	}
	else{
		m_DC[0].SelectObject(&m_RED2);
		Variable=-1;
	}

	//Draw a new one
	m_DC[0].MoveTo(Label.ControlRect.left,0.5*(Label.ControlRect.top+Label.ControlRect.bottom));
	m_DC[0].LineTo(0.5*(Label.ControlRect.left+Label.ControlRect.right),Label.ControlRect.bottom);
	m_DC[0].LineTo(Label.ControlRect.right,0.5*(Label.ControlRect.top+Label.ControlRect.bottom));

	m_DC[0].SelectObject(pOldPen);
	Label.PrvWholeRect=Label.StringRect;
}

void CMOXY_View::Draw_IncreaseArrow(int &Variable, ClickableLabel &Label,  int isToHighlight, CDC *pObjDC) 
{
	CPen *pOldPen;
	//Delete the previous arrow
	pOldPen=m_DC[0].SelectObject(&m_NullPen);
	m_DC[0].SelectStockObject(WHITE_BRUSH);
	m_DC[0].Rectangle(Label.PrvWholeRect);

	if(isToHighlight==0){
		m_DC[0].SelectObject(&m_GrayPen2);
		Variable=0;
	}
	else{
		m_DC[0].SelectObject(&m_RED2);
		Variable=1;
	}

	//Draw a new one
	m_DC[0].MoveTo(Label.ControlRect.left,0.5*(Label.ControlRect.top+Label.ControlRect.bottom));
	m_DC[0].LineTo(0.5*(Label.ControlRect.left+Label.ControlRect.right),Label.ControlRect.top);
	m_DC[0].LineTo(Label.ControlRect.right,0.5*(Label.ControlRect.top+Label.ControlRect.bottom));

	m_DC[0].SelectObject(pOldPen);
	Label.PrvWholeRect=Label.StringRect;
}


void CMOXY_View::ReCalibrateAllFigures() 
{
	//Recofigure the DIO
	InitDIO_FIg();			
	//Recofigure the XY position window
	Init_XYwindow();
	//Recofigure the Running-line display
	for(int i=0;i<NUM_RL_PANELS;i++)	m_RunPanel[i].Init_RunningLineSDF_FIG();
}

void CMOXY_View::Draw_User_RtClick_Pos() 
{
	CClientDC objDC(this);
	CPen *pOldpen=(CPen*)objDC.SelectStockObject(BLACK_PEN);
	objDC.SetROP2(R2_NOTXORPEN);
	//Erase the previous one
	objDC.MoveTo(m_PrvRightClickX-m_pXYFig->m_FontHeight,m_PrvRightClickY); 	objDC.LineTo(m_PrvRightClickX+m_pXYFig->m_FontHeight,m_PrvRightClickY); 
	objDC.MoveTo(m_PrvRightClickX,m_PrvRightClickY-m_pXYFig->m_FontHeight); 	objDC.LineTo(m_PrvRightClickX,m_PrvRightClickY+m_pXYFig->m_FontHeight); 
	//Draw a new one
//	objDC.SelectStockObject(WHITE_PEN);
	objDC.MoveTo(m_RightClickX-m_pXYFig->m_FontHeight,m_RightClickY); 	objDC.LineTo(m_RightClickX+m_pXYFig->m_FontHeight,m_RightClickY); 
	objDC.MoveTo(m_RightClickX,m_RightClickY-m_pXYFig->m_FontHeight); 	objDC.LineTo(m_RightClickX,m_RightClickY+m_pXYFig->m_FontHeight); 

	m_PrvRightClickX=m_RightClickX; m_PrvRightClickY=m_RightClickY;
	objDC.SelectObject(pOldpen);
	objDC.SetROP2(R2_COPYPEN);	
}

void CMOXY_View::SetFontSize(CFigure* pFig)
{
	// Get the sizes of the font
	//CFont* pFont = m_DC[0].GetCurrentFont(); This SOMETIMES returns an incorrect pointer.
	LOGFONT LogFont;
	m_font.GetLogFont(&LogFont);
	pFig->m_FontWidth=LogFont.lfWidth;
	pFig->m_FontHeight=LogFont.lfHeight;
}

//This maps a screen point (x,y) to a data point (i,j) on the selected graph.
void CMOXY_View::FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY) 
{
	if(m_XY_DisplayMode==CARTESIAN_MODE){
		DataX=(m_maxPaneX-m_minPaneX)*(((double)(x   -m_paneTopX))/(m_paneBottomX-m_paneTopX))+m_minPaneX;	
		DataY=(m_maxPaneY-m_minPaneY)*(((double)(m_paneBottomY-y))/(m_paneBottomY-m_paneTopY))+m_minPaneY;
	}
	else{
		DataX=m_pXYFig->FromScreenToDataX_VisReplica(x);
		DataY=m_pXYFig->FromScreenToDataY_VisReplica(y);
	}
}

void CMOXY_View::OnBiggerPanels()
{
	if(m_XY_Monitor_Scale<200.&&m_XY_Monitor_Scale>=10.){
		if(!(m_XY_DisplayMode==EYE_MONITOR_MODE && m_XY_Monitor_Scale>=MAX_EYE_MONITOR_ANGLE))m_XY_Monitor_Scale+=10;	
	}
	if(m_XY_Monitor_Scale<10.)m_XY_Monitor_Scale+=1;	
	m_bIsScrXYforIJ=0;
	Init_XYwindow();
	RequestDisplayEvent(REDRAW_XY_WINDOW_FIG);
}

void CMOXY_View::OnSmallerPanels()
{
	if(m_XY_Monitor_Scale>10.)m_XY_Monitor_Scale-=10;	
	else if(m_XY_Monitor_Scale>1.)m_XY_Monitor_Scale-=1;	
	m_bIsScrXYforIJ=0;
	Init_XYwindow();
	RequestDisplayEvent(REDRAW_XY_WINDOW_FIG); 
}

//It calculates the constant of the Gaussian kernel.
double CMOXY_View::Kernel_2D(double KerSum, double Sigma, double RangeX)
{
	double sigSq=Sigma*Sigma, rangeXSq=RangeX*RangeX, SqDist, Sum; 

	int p,q, IniP,FinP, IniQ,FinQ; 
	IniP=0-RangeX; FinP=0+RangeX;	IniQ=0-RangeX; FinQ=0+RangeX;
	Sum=0; 
	for(p=IniP;p<=FinP;p++){
		for(q=IniQ;q<=FinQ;q++){
			if((SqDist=SQ_DISTANCE_AB(0,0, p,q))>rangeXSq)continue;
			Sum += exp(-SqDist/sigSq);
	}	}
	return (KerSum/Sum);  //KerSum=Kernel*SIG(exp[.])
}

//It calculates the constant of the Gaussian kernel.
double CMOXY_View::Kernel_1D(double KerSum, double Sigma, double RangeX)
{
	double sigSq=Sigma*Sigma, rangeXSq=RangeX*RangeX, SqDist, Sum; 

	int p, IniP,FinP; 
	IniP=0-RangeX; FinP=0+RangeX;
	Sum=0; 
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(0,p))>rangeXSq)continue;
		Sum += exp(-SqDist/sigSq);
	}	
	return (KerSum/Sum);  //KerSum=KerAmp*SIG(exp[.])
}

//It generates the Gaussian kernel and its field.
int CMOXY_View::MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX)
{
	double KerAmp=Kernel_1D(KerSum, Sigma, RangeX);	
	double SqDist,sigmaSq=Sigma*Sigma,rangeXSq=RangeX*RangeX;
	int i=0,x, NoX=0;
	int IniX=i-RangeX, FinX=i+RangeX;
	for(x=IniX;x<=FinX;x++){
		if((SqDist=SQ_DISTANCE_AB_1D(i,x))>(float)rangeXSq)continue;
		KrVal[NoX]= KerAmp*exp(-SqDist/sigmaSq);
		KerFieldX[NoX]=x;  NoX++;
		if(NoX>sizeOfKer){AfxMessageBox("NoX>=sizeOfKer", MB_OK); return NoX;}
	}
	
	CFigure *pFig=&m_Fig[0];
	pFig->Graph2D(KerFieldX[0],KrVal[0],9,&m_DC[0]);
	for(i=0; i<NoX; i++){
		pFig->Graph2D(KerFieldX[i],KrVal[i],2,&m_DC[0]);
	}
	return NoX;
}

void CMOXY_View::OnCloseBlip()
{
	if(m_IsMemMap==1){
		*M_Blip_OFF=1;
		Sleep(1);
	}
}

void CMOXY_View::OnViewRightClickFigure()
{
	m_IsToShowFigureOnRightClick=!m_IsToShowFigureOnRightClick;
}

void CMOXY_View::OnUpdateViewRightClickFigure(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsToShowFigureOnRightClick==1);
}

void CMOXY_View::OnViewAngleMonitor()
{
	m_XY_DisplayMode=EYE_MONITOR_MODE;
	if(m_XY_Monitor_Scale>MAX_EYE_MONITOR_ANGLE)m_XY_Monitor_Scale=MAX_EYE_MONITOR_ANGLE;
	Save_MOXY_Config();
	ReCalibrateAllFigures();
	RequestDisplayEvent(REDRAW_ALL_FIGURES); 
}

void CMOXY_View::OnViewCartesian()
{
	m_XY_DisplayMode=CARTESIAN_MODE;
	Save_MOXY_Config();
	ReCalibrateAllFigures();
	RequestDisplayEvent(REDRAW_ALL_FIGURES); 
}

void CMOXY_View::OnUpdateViewCartesian(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_XY_DisplayMode==CARTESIAN_MODE);
}

void CMOXY_View::OnUpdateViewAngleMonitor(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_XY_DisplayMode==EYE_MONITOR_MODE);
}

