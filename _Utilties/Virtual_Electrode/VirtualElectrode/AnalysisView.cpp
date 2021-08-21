// AnalysisView.cpp : implementation of the CAnalysisView class
//

#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CAnalysisView::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CAnalysisView

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CAnalysisView, CView)

BEGIN_MESSAGE_MAP(CAnalysisView, CView)
	//{{AFX_MSG_MAP(CAnalysisView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_2DGraph_III, On2DGraph_III)
	ON_COMMAND(ID_2DGraph_II, On2DGraph_II)
	ON_COMMAND(ID_2DGraph_5, On2DGraph_5)
	ON_COMMAND(ID_2DGraph_4, On2DGraph_4)
	ON_COMMAND(ID_2DGraph_VI, On2DGraph_VI)
	ON_COMMAND(ID_2DGraph_VII, On2DGraph_VII)
	ON_COMMAND(ID_2DGraph_VIII, On2DGraph_VIII)
	ON_COMMAND(ID_2DGraph, On2DGraph_I)
	ON_COMMAND(ID_Parameters, OnChangeParameters)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_SCALE_DELETE_FIGS, &CAnalysisView::OnDeleteFigs)
	ON_COMMAND(ID_EDIT_POINTS, &CAnalysisView::OnEditPoints)
	ON_COMMAND(ID_FILE_SAVE, &CAnalysisView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CAnalysisView::OnFileOpen)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_SAVE_DRAWING, &CAnalysisView::OnFileSaveDrawing)
	ON_COMMAND(ID_SHOW_000, &CAnalysisView::OnShow_Origin)
	ON_UPDATE_COMMAND_UI(ID_SHOW_000, &CAnalysisView::OnUpdateShow_Origin)
	ON_UPDATE_COMMAND_UI(ID_2DGraph_III, &CAnalysisView::OnUpdate2dgraph3)
	ON_UPDATE_COMMAND_UI(ID_2DGraph_4, &CAnalysisView::OnUpdate2dgraph4)
	ON_UPDATE_COMMAND_UI(ID_2DGraph, &CAnalysisView::OnUpdate2dgraph)
	ON_COMMAND(ID_SELECT, &CAnalysisView::OnSelect)
	ON_UPDATE_COMMAND_UI(ID_SELECT, &CAnalysisView::OnUpdateSelect)
	ON_COMMAND(ID_MAKE_LINE_SEGMENTS, &CAnalysisView::OnMakeLineSegments)
	ON_UPDATE_COMMAND_UI(ID_MAKE_LINE_SEGMENTS, &CAnalysisView::OnUpdateMakeLineSegments)
	ON_COMMAND(ID_MAKE_POINTS, &CAnalysisView::OnMakePoints)
	ON_UPDATE_COMMAND_UI(ID_MAKE_POINTS, &CAnalysisView::OnUpdateMakePoints)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_CROSSHAIR, &CAnalysisView::Define_3D_Origin_inMRI_byMouseClick)
	ON_UPDATE_COMMAND_UI(ID_CROSSHAIR, &CAnalysisView::OnUpdate_Locate_3D_Origin_inMRI_byMouseClick)
	ON_COMMAND(ID_CONFIG_DIALOG, &CAnalysisView::OnPitchRollYaw)
	ON_COMMAND(ID_ELECTRODE_COORDINATE, &CAnalysisView::OnUserInput_of_Electrode_Position)
	ON_COMMAND(ID_SHOW_TEXTURE, &CAnalysisView::OnShowTexture)
	ON_UPDATE_COMMAND_UI(ID_SHOW_TEXTURE, &CAnalysisView::OnUpdateShowTexture)
	ON_COMMAND(ID_VIEW_DISTANCE_FROM_ELECTRODE, &CAnalysisView::OnView_DistanceFromElectrode)
	ON_COMMAND(SHOW_DRAWING, &CAnalysisView::OnShowDrawing)
	ON_UPDATE_COMMAND_UI(SHOW_DRAWING, &CAnalysisView::OnUpdateShowDrawing)
	ON_COMMAND(ID_LOCATE_GRID_ORIGIN, &CAnalysisView::OnDefineGridOrigin)
	ON_UPDATE_COMMAND_UI(ID_LOCATE_GRID_ORIGIN, &CAnalysisView::OnUpdateLocateGridOrigin)
	ON_COMMAND(ID_TOOLS_RENAME, &CAnalysisView::OnToolsRenameMRIs)
	ON_COMMAND(ID_BLANK, &CAnalysisView::OnResetFigure)
	ON_COMMAND(ID_SHOW_GRID, &CAnalysisView::OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, &CAnalysisView::OnUpdateShowGrid)
	ON_COMMAND(ID_MARKING, &CAnalysisView::OnMarking)
	ON_UPDATE_COMMAND_UI(ID_MARKING, &CAnalysisView::OnUpdateMarking)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CAnalysisView construction/destruction
CAnalysisView::CAnalysisView()
{
	// TODO: add construction code here
	CRect rec(605,0,1300,700);
	m_rectShow3D=rec;
	m_nColorShow=BLACK_BRUSH;
	m_bShow2D=0;  
	m_bFistTime=1;
	m_nKey=0;
	m_nSpeed=0;
	FlagVerScreen=0;
	m_bProceed=0;
	m_bIsToggleON=0;
	m_bIsON=0;
	m_dScaleUpDn=1;
	m_dHowManyRows=MaxFigRow; m_dHowManyClmns=MaxFigCln;
	m_BlownUpPaneID=-1;
	////////////////////////////////////
	m_NULL.CreatePen(PS_NULL, 1,RGB(0,0,0));
	m_RED.CreatePen(PS_SOLID, 1,RGB(255,0,0));
	m_RED3.CreatePen(PS_SOLID, 3,RGB(255,0,0));
	m_RED5.CreatePen(PS_SOLID, 5,RGB(255,0,0));
	m_RED8.CreatePen(PS_SOLID, 8,RGB(255,0,0));
	m_ORANGE2.CreatePen(PS_SOLID, 2,RGB(255,150,0));
	m_GREEN.CreatePen(PS_SOLID, 1,RGB(0,255,0));
	m_BLUE.CreatePen(PS_SOLID, 1,RGB(100,100,255));
	m_BLUE3.CreatePen(PS_SOLID, 3,RGB(100,100,255));
	m_BLUE5.CreatePen(PS_SOLID, 5,RGB(100,100,255));
	m_BLUE8.CreatePen(PS_SOLID, 8,RGB(100,100,255));
	m_GREEN3.CreatePen(PS_SOLID, 3,RGB(0,255,0));
	m_DkGREEN.CreatePen(PS_SOLID, 1,RGB(0,155,0));
	m_CYAN.CreatePen(PS_SOLID, 1,RGB(0,255,255));
	m_CYAN3.CreatePen(PS_SOLID, 3,RGB(200,255,255));
	m_PINK.CreatePen(PS_SOLID, 1,RGB(255,150,150));
	m_PINK3.CreatePen(PS_SOLID, 3,RGB(255,150,150));
	m_BROWN.CreatePen(PS_SOLID, 1,RGB(240,130,0));
	m_VIOLET.CreatePen(PS_SOLID, 1,RGB(200,90,250));
	m_VIOLET2.CreatePen(PS_SOLID, 2,RGB(200,90,250));
	m_VIOLET3.CreatePen(PS_SOLID, 3,RGB(200,90,250));
	m_bkPen.CreatePen(PS_SOLID, 1,RGB(220,220,220));
//	m_GRAY2.CreatePen(PS_SOLID, 3,RGB(200,200,200));
	m_GRAY2.CreatePen(PS_SOLID, 3,RGB(220,220,220));
//	m_GRAY2.CreatePen(PS_SOLID, 3,RGB(230,230,230));
	m_BlueGRAY.CreatePen(PS_SOLID, 1,RGB(210,210,240));
	m_BLACK.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_BLACK3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_BLACK2.CreatePen(PS_SOLID, 2,RGB(0,0,0));
	m_BLACK5.CreatePen(PS_SOLID, 5,RGB(0,0,0));
	int i,j;
	/////////////
	//CosineColorMix(m_RGB60,MaxRGB60);
	CosineColorMix_AxB(m_RGB60,MaxRGB60,BCGYRV_COLORS,BRIGHTNESS_LEVELS-1);
	for(i=0;i<BCGYRV_COLORS;i++){m_RGB60[(BRIGHTNESS_LEVELS-1)*BCGYRV_COLORS+i]=RGB(255*((i+0.5)/(double)BCGYRV_COLORS),255*((i+0.5)/(double)BCGYRV_COLORS),255*((i+0.5)/(double)BCGYRV_COLORS));}
	//GaussianColorMix(m_RGB60,MaxRGB60);
	//MyOriginalColorMix(m_RGB60,MaxRGB60); m_RGB60[0]=RGB(100,100,100);
	MyOriginalColorMix_forGL( m_rGL, m_gGL, m_bGL, MaxRGB60);

	LOGBRUSH LogBrh[MaxPen];
	for(i=0;i<MaxPen;i++){
		j=i*((MaxRGB60-1)/(MaxPen-1));
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB60[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 3,m_RGB60[j]);
		//m_rgbPEN3[i].CreatePen(PS_SOLID, 6,m_RGB60[j]);

		LogBrh[i].lbStyle=BS_SOLID;
		LogBrh[i].lbColor=m_RGB60[j];
		m_ColorBrush[i].CreateBrushIndirect( &LogBrh[i] );
	}
	for(i=0;i<MaxBrush;i++){
		LogBrh[i].lbStyle=BS_SOLID;
		LogBrh[i].lbColor=RGB((int)((255./MaxBrush)*i),(int)((255./MaxBrush)*i),(int)((255./MaxBrush)*i));
		m_Gray[i].CreateBrushIndirect( &LogBrh[i] );
	}

	/////////////// Parameters For Applications
	m_GlobalTitle="NoName";
	m_NewlyLoaded=0; m_DoNotErase=0;
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	m_SelectedItemID=-1; m_numCaptured=0;

	m_NeedsToSave=0; 
	m_toShowNeuralInfo=1;
	m_toShowDrawing=1;
	m_isCoronal_Or_Sagittal=-1;

	//////////
	m_FirstTime0ElementErr=1;
	m_Pic_Loaded=0;
	m_toShowRuller=0;

	///
	CLinedObj::m_pNeedsToSave=&m_NeedsToSave;
	m_RGBID=MaxRGB60-1; m_Selected_Drawing_Idx=-1; m_Selected_MRI_Idx=-1;
	m_OriginDone=0; m_UnderConstructionObjID=-1;
	m_PixelScale_in_mm=0.5;
	m_SelectedDots.SetSize(100); m_SizeOfSelectedDots=100; m_NumSelectedDots=0;
	m_SelectedMarkings.SetSize(100); m_SizeOfSelectedMarkings=100; m_NumSelectedMarkings=0;
	m_SelectedObjs.SetSize(100); m_SizeOfSelectedObjs=100; m_NumSelectedObjs=0;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalSection2);
	
	m_hThread=NULL;
	m_IsTextOnBoard=1; 
	m_Znear=0; m_Zfar=210; 
	m_hEvStopMainThread=NULL;
	m_DispEventIdx=0;m_RequestIdx=0;
	m_isToDispOrigin=1;
	m_selPaneID=-1;
	m_FirstGL=1; 
	m_Saved_Left_MousePointFor3DX=NO_3DMOVE; 	m_Saved_Left_MousePointFor3DY=NO_3DMOVE;  m_Saved_Right_MousePointFor3DX=NO_3DMOVE; 	m_Saved_Right_MousePointFor3DY=NO_3DMOVE;
	m_savedX=-1, m_savedY=-1; m_DrewTmpLine=0; 
	m_CTRL_Pressed=0;
	
	m_GrabedVertex=-1 ;m_GrabbedObjectIdx=-1; m_GrabbedDotIdx=-1; m_GrabbedMarkingIdx=-1; m_CurrentlyEditedObjIdx=-1;
	m_RGBID=MaxPen*0.5;
	m_toRegisterOrign=0; m_toRegisterGridOrign=0;
	m_MRI_Pixel_Dimension_In_mm=0.5;
	m_DrawingLoaded=0;
	m_ShowCrosshairOnce=0;

	m_OriginOfAbscissa_MRIPixelCoordinate_Coronal_JustForSaveAndRead=100; m_OriginOfOrdinate_MRIPixelCoordinate_Coronal_JustForSaveAndRead=100;
	m_OriginOfAbscissa_MRIPixelCoordinate_Sagittal_JustForSaveAndRead=100, m_OriginOfOrdinate_MRIPixelCoordinate_Sagittal_JustForSaveAndRead=100;
	m_NumDrawings=0;
	m_ElecCoordinate_withGridCenterAsOrigin[0]='\0';
	//////////
	m_NnType=-1; 
	m_fovy=10;
	m_Magnification2D=1; m_PrevMag2D=1; m_MinX_corner=0; m_MinZ_corner=0;
	m_xScale3D=1, m_yScale3D=1, m_zScale3D=1;
	m_isToEditObj=0;
	m_DistanceFromElectrode=0.02;
}					


CAnalysisView::~CAnalysisView() 
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){*M_is3D_Running=0; m_isCoronal_Or_Sagittal=-1;}
	m_bIsON=0;    //In case the thread uses a switch (m_bIsON)
	//RequestDisplayEvent(STOP_THREAD);//it uses m_Events[]
	for(int i=0; i<MAXIMUM_WAIT_OBJECTS; i++){m_DispEvents[i][0]=STOP_THREAD;} for(int i=0; i<MAXIMUM_WAIT_OBJECTS; i++){SetEvent(m_Events[i]);}

	// Wait for threads to stop, in case the thread uses a thread handle (e.g., m_hThread).
	//if(m_hThread!=NULL){
	//	WaitForSingleObject(m_hThread,1000);
	//	CloseHandle(m_hThread);
	//}
	if(m_hEventBasedThread!=NULL){
		WaitForSingleObject(m_hEventBasedThread,1000);
		CloseHandle(m_hEventBasedThread);
	}

	//Close Handles
	int i,j;
	//CloseHandle(m_hEvStopMainThread);
	//for(i=0; i<MAXIMUM_WAIT_OBJECTS;i++){CloseHandle(m_Events[i]);}
	//for(i=0; i<MAXIMUM_WAIT_KeyOBJECTS;i++){CloseHandle(M_KeyDownEvents[i]);}

	//In the case the thread uses an old switch method to terminate, just wait for a sec.
	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	

	 
	m_SelectedDots.RemoveAll();
	m_SelectedMarkings.RemoveAll();
	m_SelectedObjs.RemoveAll();
	m_Nn.RemoveAll();
	m_MRI.RemoveAll();
	CloseSharedMemoryArea();

	//##########  Drawing related
	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	for(i=0; i<MaxBrush;i++)m_Gray[i].DeleteObject();

	m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen;i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); //m_rgbPEN3[i].DeleteObject();
		m_ColorBrush[i].DeleteObject();
	}
	m_NULL.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject(); m_GREEN.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();
	m_VIOLET.DeleteObject();m_ORANGE2.DeleteObject(); m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_BLACK.DeleteObject();m_BLACK2.DeleteObject();m_BLACK3.DeleteObject();m_bkPen.DeleteObject();m_GRAY2.DeleteObject();
	m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	m_font.DeleteObject();
	DeleteCriticalSection(&m_CriticalSection);
	DeleteCriticalSection(&m_CriticalSection2);
}

BOOL CAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

DWORD WINAPI  EventBased_routines(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	for(int i=0; i<MAXIMUM_WAIT_OBJECTS;i++)pView->m_Events[i]=CreateEvent(NULL,FALSE,FALSE,NULL);
	pView->DisplayRoutine_for_requested_jobs() ;
	return 0;
}
UINT Graph2D_I(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->_1st_ThreeD_Anatomy() ;
	return 0;
}
UINT Graph2D_III(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_III() ;
	return 0;
}
UINT Graph2D_4(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_4() ;
	return 0;
}

UINT Graph2D_II(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->_2nd_InteractiveMRI() ;
	return 0;
}

UINT Graph2D_5(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_5() ;
	return 0;
}

UINT Graph2D_VI(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_VI() ;
	return 0;
}
UINT Graph2D_VII(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_VII() ;
	return 0;
}
UINT Graph2D_VIII(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_VIII() ;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAnalysisView drawing

void CAnalysisView::OnDraw(CDC* pDC)
{
#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_isCoronal_Or_Sagittal==FROM_3D){
		SetEvent(M_KeyDownEvents[3]);
		return;
	}
	if(m_bFistTime==1){
		VirMemDC();
		CreateSharedMemoryArea();
		m_hEventBasedThread=CreateThread(NULL,0, EventBased_routines,this,0,NULL); //The thread that takes care of Display requests.
		Sleep(20);
		m_dHowManyClmns=-1;
		OnFileOpen();//Try to open a default root file.
		if(m_dHowManyClmns!=-1){
			if(strlen(M_MsgToNewWindow)!=0){
				if(wildcmp("Coronal*",M_MsgToNewWindow)==1){M_MsgToNewWindow[0] ='\0';AfxBeginThread( Graph2D_III, this);}
				else if(wildcmp("Sagittal*",M_MsgToNewWindow)==1){M_MsgToNewWindow[0] ='\0';AfxBeginThread( Graph2D_4, this);}
				else if(wildcmp("3D*",M_MsgToNewWindow)==1){M_MsgToNewWindow[0] ='\0';AfxBeginThread( Graph2D_I, this);}
				M_MsgToNewWindow[0] ='\0';
			}
			m_bFistTime=0;
		}
		else{//Failed to open a default root file => implement the programs own version.
			sprintf(m_FileName,"");
			sprintf(m_JustFileName,"NO root file");
			m_dHowManyClmns=MaxFigCln; 
			Re_measure_ClientView();
			Start_a_NEW_set_of_figures();
			if(strlen(M_MsgToNewWindow)!=0){
				if(wildcmp("Coronal*",M_MsgToNewWindow)==1){M_MsgToNewWindow[0] ='\0';AfxBeginThread( Graph2D_III, this);}
				else if(wildcmp("Sagittal*",M_MsgToNewWindow)==1){M_MsgToNewWindow[0] ='\0';AfxBeginThread( Graph2D_4, this);}
				else if(wildcmp("3D*",M_MsgToNewWindow)==1){M_MsgToNewWindow[0] ='\0';AfxBeginThread( Graph2D_I, this);}
				M_MsgToNewWindow[0] ='\0';
			}
			m_bFistTime=0;
		}
	}
	else{
		CRect rt;
		CClientDC objDC(this);
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			RequestDisplayEvent(RESIZING_WIN); //Send a message to redraw
		}
		else{//Just display without changing anything
			objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY); m_DrewTmpLine=0;
		}
	}
}

int CAnalysisView::Launch_Coronal_Or_Sagittal(char ModeName[])
{
	strcpy(M_MsgToNewWindow,ModeName);

	char DirStr[1024];
	CString St=	GetExecutableName();
	strcpy(DirStr,St);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
	return result;
}

void CAnalysisView::Re_measure_ClientView()
{			
	CRect rt;
	GetClientRect(&rt);
	if(m_bFistTime==1){
		CFont* pFont = m_DC[0].GetCurrentFont();
		LOGFONT LogFont;
		pFont->GetLogFont(&LogFont);
		m_fontHeight=LogFont.lfHeight;
	}
	m_TitleArea.top=rt.top;	m_TitleArea.bottom=rt.top+m_fontHeight*2; m_topOfFigs=m_TitleArea.bottom;
	m_TitleArea.left=rt.left; m_TitleArea.right=rt.right;
	m_dSizeOfOneRow =(rt.bottom-m_topOfFigs-ViewMargin4FigsAlongEdges)/m_dHowManyRows;  if(m_dSizeOfOneRow <=0)m_dSizeOfOneRow=0;
	m_dSizeOfOneClmn=(rt.right-rt.left-ViewMargin4FigsAlongEdges)/m_dHowManyClmns; if(m_dSizeOfOneClmn<=0)m_dSizeOfOneClmn=0;
	m_savedClientRect=rt;m_savedClientRect.top=m_topOfFigs;
}


/////////////////////////////////////////////////////////////////////////////
// CAnalysisView printing

BOOL CAnalysisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAnalysisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAnalysisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAnalysisView diagnostics

#ifdef _DEBUG
void CAnalysisView::AssertValid() const
{
	CView::AssertValid();
}

void CAnalysisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnalysisDoc* CAnalysisView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnalysisDoc)));
	return (CAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAnalysisView message handlers



void CAnalysisView::On2DGraph_I() 
{
	    //m_hEvStopMainThread=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,NULL/*use name?*/);
		//m_hThread=CreateThread(NULL,0,Graph2D_I,this,0,NULL);
		AfxBeginThread( Graph2D_I, this);
}
void CAnalysisView::On2DGraph_II() 
{
		AfxBeginThread( Graph2D_II, this);
}
void CAnalysisView::On2DGraph_III() 
{
		AfxBeginThread( Graph2D_III, this);
}
void CAnalysisView::On2DGraph_5() 
{
		AfxBeginThread( Graph2D_5, this);
}
void CAnalysisView::On2DGraph_4() 
{
		AfxBeginThread( Graph2D_4, this);
}
void CAnalysisView::On2DGraph_VI() 
{
		AfxBeginThread( Graph2D_VI, this);
}
void CAnalysisView::On2DGraph_VII() 
{
		AfxBeginThread( Graph2D_VII, this);
}
void CAnalysisView::On2DGraph_VIII() 
{
		AfxBeginThread( Graph2D_VIII, this);
}

void CAnalysisView::VirMemDC()
{
	//########  Preparing Virtual Window  ##########
	if(FlagVerScreen==1){
		//Clear screen
		//m_DC[0].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
		return;
	}
	FlagVerScreen=1;
////////////////////////////////
   LOGFONT lf;                        // Used to create the CFont.
   memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
   lf.lfWidth = 6;                   // Width of font
   lf.lfHeight = 12;                  // Request a 20-pixel-high font
   strcpy(lf.lfFaceName, "Times New Roman");    //    with face name "Arial".
   m_font.CreateFontIndirect(&lf);    // Create the font.
   // Use the font to paint a control. This code assumes
   // a control named IDC_TEXT1 in the dialog box.
   SetFont(&m_font);
//////////////////////////////////////
	
	m_nScrMaxX=GetSystemMetrics(SM_CXSCREEN);
	m_nScrMaxY=GetSystemMetrics(SM_CYSCREEN);
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(220,220,220);	
	m_bkBrush.CreateBrushIndirect( &LogBrh );
	//m_bkBrush.CreateStockObject(WHITE_BRUSH);
	CClientDC aDC(this);
	
	for(int i=0;i<MaxDC;i++){
		m_DC[i].CreateCompatibleDC(&aDC);
		m_bmp[i].CreateCompatibleBitmap(&aDC, m_nScrMaxX, m_nScrMaxY);
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_bkBrush);
		m_DC[i].SelectObject(&m_font);
		m_DC[i].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
	}
	m_SavedDC=m_DC[0].SaveDC();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################  Display Event Handler #######################################
void CAnalysisView::RequestDisplayEvent(int Event) 
{
	RequestDisplayEvent(Event, 0, NULL,  NULL);
}
void CAnalysisView::RequestDisplayEvent(int Event,  int SomeIntValue) 
{
	RequestDisplayEvent(Event, 1, SomeIntValue,  NULL); 
}
void CAnalysisView::RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2) 
{
	RequestDisplayEvent(Event, 2, SomeIntValue1, SomeIntValue2);
}
void CAnalysisView::RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) 
{	
	m_DispEvents[m_DispEventIdx][0]=Event;
	m_DispEvents[m_DispEventIdx][1]=NumVariables;
	for(int i=0; i<NumVariables;i++){
		if(i==0)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue1; 
		else if(i==1)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue2; 
	}
	SetEvent(m_Events[m_DispEventIdx]);
	if(m_DispEventIdx+1<MAXIMUM_WAIT_OBJECTS)m_DispEventIdx++; else m_DispEventIdx=0;
}
//In case there is a need to centralize the dispalying processes, embed this this function in a host thread
void CAnalysisView::DisplayRoutine_for_requested_jobs()
{	
	CFigure *pFig=&m_Fig[0][0][0]; 	
	CClientDC objDC(this);
	int nIndex;

	m_RequestIdx=m_DispEventIdx;
	while(1){
		nIndex=WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS,m_Events,FALSE/*wait for all?*/,INFINITE/*timeout*/); 
		if(*M_isCoronal_Running==0 && *M_isSagittal_Running==0 && *M_is3D_Running==0)continue;
		switch(m_DispEvents[m_RequestIdx][0]){
			case RESIZING_WIN:
				if(m_Pic_Loaded==1 && m_isCoronal_Or_Sagittal!=FROM_3D){
					m_DC[0].FillSolidRect(m_savedClientRect,RGB(220,220,220));
					Init_Figure_AFTER_MRI_loading();
					Redraw_Everything();
				}
				else{
					m_DC[0].FillSolidRect(m_savedClientRect,RGB(220,220,220));
					objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY); m_DrewTmpLine=0;
				}
				break;
			case RENAMING_TITLE:
				DisplayTitle();
				break;
			case RENAMING_FIG:
				pFig->Prepare4FigFrame();
				pFig->Disp_Electrode_Position(&m_DC[0]); 
				pFig->RefreshNameBox(&objDC,&m_DC[0]); 
				break;
			case RESCALING_AXES:
				pFig->Prepare4FigFrame();
				pFig->Frame_Graph2D(&m_DC[0],1); 
				pFig->RefreshFrame(&objDC,&m_DC[0]); 
				Hilight_OneCapturedFig(pFig); 
				break;
			case STOP_THREAD: goto END_OF_FUNCTION;
		}
		//Increase the index
		if(m_RequestIdx<m_DispEventIdx)m_RequestIdx++;
		//else if(m_RequestIdx==m_DispEventIdx)AfxMessageBox("m_RequestIdx==m_DispEventIdx");
		else{
			if(m_RequestIdx+1<MAXIMUM_WAIT_OBJECTS)m_RequestIdx++; else m_RequestIdx=0;
		}
	}
	END_OF_FUNCTION:;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

void CAnalysisView::Start_a_NEW_set_of_figures()
{			
	int i,j;
	CFigure *pFig;
	m_numCaptured=0;
	Batch_SetFontSize(&m_DC[0]);
	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->SetMinMaxXY(0,1000,-100,100);pFig->SetLabelsXY("","");
			pFig->m_Captured=0;
			pFig->m_Deleted=0;
	}	}
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	RequestDisplayEvent(RESIZING_WIN); //Send a message to redraw
}

void CAnalysisView::Manual_Fig_Labels_n_MinMaxXY() 
{

}

void CAnalysisView::Tics_n_Lines(CFigure *pFig, int HowManyTicsX, int HowManyTicsY) 
{
#define TICK_STEPX(pB) ((pB->m_dMaxX-pB->m_dMinX)/(pB->m_howManyX-1))
#define TICK_STEPY(pB) ((pB->m_dMaxY-pB->m_dMinY)/(pB->m_howManyY-1))
	int i;
	float TickStepX, TickStepY;

	pFig->m_howManyX=HowManyTicsX; TickStepX=TICK_STEPX(pFig);
	for(i=0;i<pFig->m_howManyX;i++){pFig->m_IsLabeledTicX[i]=1;pFig->m_XTics[i]=pFig->m_dMinX+i*TickStepX;} 
	pFig->m_howManyY=HowManyTicsY; TickStepY=TICK_STEPY(pFig);
	for(i=0;i<pFig->m_howManyY;i++){pFig->m_IsLabeledTicY[i]=1;pFig->m_YTics[i]=pFig->m_dMinY+i*TickStepY;} 
}

void CAnalysisView::Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) 
{
#define F2FY 5
#define F2FX 5
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->m_normTx=scale*((j+1)*F2FX+    j*(sizeOfOneClmn-F2FX));  pFig->m_normTy=scale*((i+1)*F2FY+    i*(sizeOfOneRow-F2FY)+m_topOfFigs);   
			pFig->m_normBx=scale*((j+1)*F2FX+(j+1)*(sizeOfOneClmn-F2FX));  pFig->m_normBy=scale*((i+1)*F2FY+(i+1)*(sizeOfOneRow-F2FY)+m_topOfFigs); 
			pFig-> m_LateralMargin=20;
			pFig->SetFig_Position_in_ClientView(pFig->m_normTx,pFig->m_normTy,pFig->m_normBx,pFig->m_normBy);

			pFig->m_BlownUpTx=200; pFig->m_BlownUpTy=5; pFig->m_BlownUpBx=820; pFig->m_BlownUpBy=605; pFig->m_LateralMargin=20;
		}	
	}
}
void CAnalysisView::Batch_SetFontSize(CDC* pDC) 
{
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->SetFontSize(pDC);
		}	
	}
}
void CAnalysisView::Batch_Prepare4FigFrame() 
{
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->Prepare4FigFrame();
		}	
	}
}

void CAnalysisView::Batch_DrawFigureFrames() 
{	
	CClientDC objDC(this);
	m_DC[0].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY); 
	m_DoNotErase=1;//This is to inform the Labeling part not to erase previous site, which may not correspond to new position due to resize.
	int i,j;
	CFigure *pFig;
	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			if(pFig->m_Deleted==0)pFig->Frame_Graph2D(&m_DC[0],1); 
		}
	}
	//m_DC[0].SetBkColor(RGB(220, 220, 220));
	//m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5, m_JustFileName);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
}


// Writes a string sText with a size which fits perfectly in a rectangle rFont
// Font already attached with device context pDC is scaled
// This does not work for default font of the device contex
void CAnalysisView::FontInRect(CDC *pDC, CString sText, CRect &rFont)
{
	// Gets current font details
	CFont* pFont = pDC->GetCurrentFont();
	if(pFont==NULL)return;
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);

	// Scales the font to fit in the rectangle
	int a=pDC->GetDeviceCaps(LOGPIXELSY);
	LogFont.lfHeight = -72 * Divide(rFont.Height(), pDC->GetDeviceCaps(LOGPIXELSY));
	LogFont.lfWidth = -72 * Divide(Divide(rFont.Width() , pDC->GetDeviceCaps(LOGPIXELSX)), sText.GetLength());

	// Create a font of calculated size
	CFont Font;
	Font.CreateFontIndirect(&LogFont);

	// Rechecks the string width and height
	pFont = pDC->SelectObject(&Font);
	CSize Size = pDC->GetTextExtent(sText);
	pDC->SelectObject(pFont);
	Font.DeleteObject();

	// Fine tunes the font width and height
	LogFont.lfWidth = (int) (LogFont.lfWidth * rFont.Width() / Size.cx);
	LogFont.lfHeight = (int) (LogFont.lfHeight * rFont.Height() / Size.cy);

	// Create a font with accurate size and displays string
	Font.CreateFontIndirect(&LogFont);
	pFont = pDC->SelectObject(&Font);
	pDC->DrawText(sText, rFont, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pFont);
	Font.DeleteObject();
}

void CAnalysisView::RefreshEverything() 
{	
	VirMemDC();
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	Manual_Fig_Labels_n_MinMaxXY();
	Batch_DrawFigureFrames();
}


//Connect two spots on the screen with an arrow
void CAnalysisView::DrawArrow(double rx1, double ry1, double rx2, double ry2, BOOL Is_Arrow, CFigure *pFig, CClientDC *pObjDC, CDC *pDC)
{
	pFig->Graph2D(rx1,ry1,rx2,ry2,2,pDC);pFig->Graph2D(rx1,ry1,rx2,ry2,2,pObjDC);
	if(Is_Arrow==0)return;

	double MainDirX,MainDirY, LeftDirX,LeftDirY,RightDirX,RightDirY, N;
	int x1,y1,x2,y2;
	pFig->FromDataToScreenXY(rx1,ry1, x1,y1);
	pFig->FromDataToScreenXY(rx2,ry2, x2,y2);

	MainDirX=x2-x1; MainDirY=y2-y1;

	//Right-side arrow wing
	RightDirX=(double)(COS165*MainDirX - SIN165*MainDirY);
	RightDirY=(double)(SIN165*MainDirX + COS165*MainDirY);
	N=(double)M2D(RightDirX, RightDirY);//Magnitude of the direction vector as a Normalization factor.
	RightDirX/=N; RightDirY/=N; 	//Normalize the direction vector	
	RightDirX*=15; RightDirY*=15;   //Multiply the length
	pDC->MoveTo(x2,y2); pDC->LineTo(RightDirX+x2,RightDirY+y2); pObjDC->MoveTo(x2,y2); pObjDC->LineTo(RightDirX+x2,RightDirY+y2);

	//Left-side arrow wing
	LeftDirX =(double)( COS165*MainDirX + SIN165*MainDirY);
	LeftDirY =(double)(-SIN165*MainDirX + COS165*MainDirY);
	N=(double)M2D(LeftDirX, LeftDirY);//Magnitude of the direction vector as a Normalization factor.
	LeftDirX/=N; LeftDirY/=N; 	//Normalize the direction vector
	LeftDirX*=15; LeftDirY*=15; //Multiply the length
	pDC->MoveTo(x2,y2); pDC->LineTo(LeftDirX+x2,LeftDirY+y2); pObjDC->MoveTo(x2,y2); pObjDC->LineTo(LeftDirX+x2,LeftDirY+y2);
}

int CAnalysisView::Rounded_to_Nearest_Int(double Val)
{
	int RoundedUp,RoundedDown;
	if(Val>0){
		RoundedUp=(int)(Val+0.5);
		RoundedDown=(int)(Val);
	}
	else{
		RoundedUp=(int)(Val);
		RoundedDown=(int)(Val-0.5);
	}
	if(fabs(Val-RoundedUp)>fabs(Val-RoundedDown))return RoundedDown;
	else return RoundedUp;
}

void CAnalysisView::OnUpdate2dgraph3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isCoronal_Or_Sagittal==CORONAL);
}

void CAnalysisView::OnUpdate2dgraph4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isCoronal_Or_Sagittal==SAGITTAL);
}


void CAnalysisView::OnUpdate2dgraph(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isCoronal_Or_Sagittal==FROM_3D);
}


void CAnalysisView::OnSelect()
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){ return;}
	if(m_isCoronal_Or_Sagittal!=CORONAL && m_isCoronal_Or_Sagittal!=SAGITTAL){AfxMessageBox("Hello there:\n Run Coronal/Sagittal program first."); return;}

	m_EditMode=SELECT_OBJ;
	ProperlyFinish();
}


void CAnalysisView::OnUpdateSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_EditMode==SELECT_OBJ);
}


void CAnalysisView::OnMakeLineSegments()
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){ return;}
	if(m_isCoronal_Or_Sagittal!=CORONAL && m_isCoronal_Or_Sagittal!=SAGITTAL){AfxMessageBox("Hello there:\n Run Coronal/Sagittal program first."); return;}

	m_EditMode=MAKE_LINE_SEG;
	m_TYPE=LINE_TYPE;
	UnMarkObjects_and_RedrawEveryting();
}


void CAnalysisView::OnUpdateMakeLineSegments(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_EditMode==MAKE_LINE_SEG);
}

void CAnalysisView::OnMakePoints()
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){ return;}
	if(m_isCoronal_Or_Sagittal!=CORONAL && m_isCoronal_Or_Sagittal!=SAGITTAL){AfxMessageBox("Hello there:\n Run Coronal/Sagittal program first."); return;}

	m_EditMode=MAKE_DOTS;
	m_TYPE=DOT_TYPE;
	UnMarkObjects_and_RedrawEveryting();
	ProperlyFinish();
}


void CAnalysisView::OnUpdateMakePoints(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_EditMode==MAKE_DOTS);
}


#include "AcceptOneDouble.h"
void CAnalysisView::Define_3D_Origin_inMRI_byMouseClick()
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){ return;}
	if(m_isCoronal_Or_Sagittal!=CORONAL && m_isCoronal_Or_Sagittal!=SAGITTAL){AfxMessageBox("Hello there:\n Run Coronal/Sagittal program first."); return;}

	m_toRegisterOrign=1;
	UnMarkObjects_and_RedrawEveryting();

	//Display the MRI of depth 0
	int found=0, i;
	for(i=0;i<m_NumMRIs;i++){
		if(m_MRI[i].m_Depth==0){
			found=1;
			char tmp[100]; strcpy(tmp,m_MRI[i].m_JustMRI_FileName);
			//m_Selected_MRI_Idx=i;
			break; 
		}
	}
	if(found==1){
		if(m_isCoronal_Or_Sagittal==CORONAL ){*M_ElectrodeTopY_3DAtlas-=*M_ElectrodeTipY_3DAtlas; *M_ElectrodeTipY_3DAtlas=0;} //Move the tip of the electrode to the Y plane containing the origin
		if(m_isCoronal_Or_Sagittal==SAGITTAL){*M_ElectrodeTopX_3DAtlas-=*M_ElectrodeTipX_3DAtlas; *M_ElectrodeTipX_3DAtlas=0;} //Move the tip of the electrode to the X plane containing the origin
		InformOtherProgramsToUpdate();
		SetEvent(M_KeyDownEvents[1]);			
		Sleep(50); //Let the main while(1) function take care of the rest.
	}
	else{
		m_toRegisterOrign=0;
		AfxMessageBox("Could not find the MRI that represents the image depth 0");
		return;
	}

	CAcceptOneDouble Dlg;
	Dlg.m_DoubleValue=m_MRI_Pixel_Dimension_In_mm;
	if(Dlg.DoModal()==IDOK){
		if(Dlg.m_DoubleValue<=0){AfxMessageBox("Pixel dimension should be a positive number."); m_toRegisterOrign=0; return;}
		m_MRI_Pixel_Dimension_In_mm=Dlg.m_DoubleValue;
		AfxMessageBox("Hello:\n In the following, please left-mouse-click the origin of the 3D in the MR image.");

	}
	else{
		m_toRegisterOrign=0;
	}
}

void CAnalysisView::OnUpdate_Locate_3D_Origin_inMRI_byMouseClick(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_toRegisterOrign==1);
}

void CAnalysisView::OnDefineGridOrigin()
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){ return;}
	if(m_isCoronal_Or_Sagittal!=CORONAL && m_isCoronal_Or_Sagittal!=SAGITTAL){AfxMessageBox("Hello there:\n Run Coronal/Sagittal program first."); return;}
	
	AfxMessageBox("Hello:\n In the following, please left-mouse-click the origin of the GRID in the MR image.");
	m_toRegisterGridOrign=1;
	UnMarkObjects_and_RedrawEveryting();
	Disply_MRI_and_ColorLegend();
}

void CAnalysisView::OnUpdateLocateGridOrigin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_toRegisterGridOrign==1);
}

void CAnalysisView::OnShow_Origin()
{
	m_isToDispOrigin=!m_isToDispOrigin;
	RequestDisplayEvent(RESIZING_WIN);
	SetEvent(M_KeyDownEvents[1]);	
}

void CAnalysisView::OnUpdateShow_Origin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isToDispOrigin==1);
}

void CAnalysisView::OnShowTexture()
{
	if(m_isCoronal_Or_Sagittal==-1){AfxMessageBox("Hello there:\n Run Coronal/Sagittal/3D program first."); return;}

	m_toShowNeuralInfo=!m_toShowNeuralInfo;
	if(m_isCoronal_Or_Sagittal==FROM_3D)SetEvent(M_KeyDownEvents[1]);
	else Redraw_Everything();
}

void CAnalysisView::OnUpdateShowTexture(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_toShowNeuralInfo==1);
}

void CAnalysisView::OnShowDrawing()
{
	if(m_isCoronal_Or_Sagittal==-1){AfxMessageBox("Hello there:\n Run Coronal/Sagittal/3D program first."); return;}

	m_toShowDrawing=!m_toShowDrawing;
	if(m_isCoronal_Or_Sagittal==FROM_3D)SetEvent(M_KeyDownEvents[1]);
	else Redraw_Everything();
}

void CAnalysisView::OnUpdateShowDrawing(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_toShowDrawing==1);
}




void CAnalysisView::OnShowGrid()
{
	m_toShowRuller=!m_toShowRuller;
	Redraw_Everything();
}


void CAnalysisView::OnUpdateShowGrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_toShowRuller==1);
}


void CAnalysisView::OnMarking()
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){ return;}
	if(m_isCoronal_Or_Sagittal!=CORONAL && m_isCoronal_Or_Sagittal!=SAGITTAL){AfxMessageBox("Hello there:\n Run Coronal/Sagittal program first."); return;}

	m_EditMode=MAKE_MARKINGS;
	m_TYPE=MARKING_TYPE;
	UnMarkObjects_and_RedrawEveryting();
	ProperlyFinish();
}


void CAnalysisView::OnUpdateMarking(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_EditMode==MAKE_MARKINGS);
}
