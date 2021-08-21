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
#include "Trial.h"

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
	ON_COMMAND(ID_2DGraph_IV, On2DGraph_IV)
	ON_COMMAND(ID_2DGraph_V, On2DGraph_V)
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
	ON_COMMAND(ID_SCALE_DELETE_THE_REST, &CAnalysisView::OnDelete_the_Rest_Figs)
	ON_COMMAND(ID_FILE_SAVE, &CAnalysisView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CAnalysisView::OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT_STRING1, &CAnalysisView::OnEnChangeEditString1)
	ON_COMMAND(ID_KERNEL_SIGMA, &CAnalysisView::OnKernelSigma)
	ON_COMMAND(ID_KERNEL_SIGMA_SMALLER, &CAnalysisView::OnKernelSigmaSmaller)
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
	m_GRAY2.CreatePen(PS_SOLID, 3,RGB(170,170,170));
	m_BlueGRAY.CreatePen(PS_SOLID, 1,RGB(210,210,240));
	m_BLACK.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_BLACK3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_BLACK2.CreatePen(PS_SOLID, 2,RGB(0,0,0));
	m_BLACK5.CreatePen(PS_SOLID, 5,RGB(0,0,0));
	/////////////
	//SigmoidColorMix(m_RGB100,MaxRGB100);
	//GaussianColorMix(m_RGB100,MaxRGB100);
	MyOriginalColorMix(m_RGB100,MaxRGB100);

	int i,j;
	for(i=0;i<MaxPen;i++){
		j=i*((MaxRGB100-1)/(MaxPen-1));
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}
	LOGBRUSH LogBrh[MaxBrush];
	for(i=0;i<MaxBrush;i++){
		LogBrh[i].lbStyle=BS_SOLID;
		LogBrh[i].lbColor=RGB((int)((255./MaxBrush)*i),(int)((255./MaxBrush)*i),(int)((255./MaxBrush)*i));
		m_Gray[i].CreateBrushIndirect( &LogBrh[i] );
	}

	/////////////// Parameters For Applications
	m_GlobalTitle="Trial Viewer";
	m_NewlyLoaded=0; m_DoNotErase=0;
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	m_SelectedItemID=-1; m_numCaptured=0;
	m_IsFirstCallByIconPress=1;
	m_RequestedTrlToDisp=0;
	m_CurrNnChannelID=0, m_CurrNnID=0;
	m_Fig[0][0][0].m_NnID=m_CurrNnID;
	m_Fig[0][0][0].m_ChanID=m_CurrNnChannelID;
	m_Fig[0][0][0].m_Is2ndAxis=1;
	InitializeCriticalSection(&m_CriticalSection);
	m_fSigma=20; 
	
	/////  Statistics  ///////
	m_FirstTime0ElementErr=1;

	///
	m_hThread=NULL;
    m_hEvStopMainThread=NULL;
	m_DispEventIdx=0;m_RequestIdx=0;
	m_SDFArraySizeUsed=0;
}					


CAnalysisView::~CAnalysisView() 
{
	m_bIsON=0;    //In case the thread uses a switch (m_bIsON)
	RequestDisplayEvent(STOP_THREAD);//it uses m_Events[]
	SetEvent(m_hEvStopMainThread);

	// Wait for threads to stop, in case the thread uses a thread handle (e.g., m_hThread).
	if(m_hThread!=NULL){
		WaitForSingleObject(m_hThread,INFINITE);
		CloseHandle(m_hThread);
	}
	if(m_hEventBasedThread!=NULL){
		WaitForSingleObject(m_hEventBasedThread,INFINITE);
		CloseHandle(m_hEventBasedThread);
	}

	//Close Handles
	int i;
	CloseHandle(m_hEvStopMainThread);
	for(i=0; i<MAXIMUM_WAIT_OBJECTS;i++){CloseHandle(m_Events[i]);}

	//In case the thread uses an old switch method to terminate, just wait for a sec.
	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	

	//Dynamic Arrays
	m_SDF.RemoveAll();
	m_Compensation.RemoveAll();

	POSITION aPos;
	aPos = m_TrialList.GetHeadPosition();
	while(aPos){
		delete m_TrialList.GetNext(aPos); //It deletes the entities connected to the list.
	}
	m_TrialList.RemoveAll(); //It deletes the list itself.

	DeleteCriticalSection(&m_CriticalSection);

	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	for(i=0; i<MaxBrush;i++)m_Gray[i].DeleteObject();
	m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen;i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); m_rgbPEN3[i].DeleteObject();
	}
	m_NULL.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject(); m_GREEN.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();
	m_VIOLET.DeleteObject();m_ORANGE2.DeleteObject(); m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_BLACK.DeleteObject();m_BLACK2.DeleteObject();m_BLACK3.DeleteObject();m_bkPen.DeleteObject();m_GRAY2.DeleteObject();
	m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	m_font.DeleteObject();
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


	if(m_bFistTime==1){
		VirMemDC();
		m_hEventBasedThread=CreateThread(NULL,0, EventBased_routines,this,0,NULL); //The thread that takes care of Display requests.
		Sleep(20);
		m_dHowManyClmns=-1;
		OnFileOpen();//Try to open a default root file.
		if(m_dHowManyClmns!=-1)return;//Opening a default root file has been successive.

		//Failed to open the defaultRoot file.
		sprintf(m_FileName,"");
		sprintf(m_JustFileName,"NO root file");
		m_dHowManyClmns=1; m_dHowManyRows=1;
		CClientDC objDC(this);
		CFigure *pFig=&m_Fig[0][0][0]; 
		pFig->m_howManyX=-1; 
		pFig->m_howManyY=-1; //Use the default drawing.
		Start_a_NEW_set_of_figures();
		Re_measure_ClientView();
		RequestDisplayEvent(RESIZING_WIN); //Send a message to redraw
		m_bFistTime=0;
	}
	else{
		CRect rt;
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			RequestDisplayEvent(RESIZING_WIN); //Send a message to redraw
		}
		else{//Just display without changing anything
			RequestDisplayEvent(REFRESH_SCREEN); //Send a message to redraw
		}
	}
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
	m_TitleArea.top=rt.top;	m_TitleArea.bottom=rt.top+m_fontHeight*3; m_topOfFigs=m_TitleArea.bottom;
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
DWORD WINAPI  Graph2D_I(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->m_IsFirstCallByIconPress=1;
	pView->Show2D_I() ;
	return 0;
}

UINT Graph2D_II(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_II() ;
	return 0;
}
UINT Graph2D_III(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_III() ;
	return 0;
}
UINT Graph2D_IV(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_IV() ;
	return 0;
}
UINT Graph2D_V(LPVOID WinObjPtr) //A global Function
{
	CAnalysisView* pView=(CAnalysisView*) WinObjPtr;
	pView->Show2D_V() ;
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


void CAnalysisView::On2DGraph_I() 
{
	    m_hEvStopMainThread=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,NULL/*use name?*/);
		m_hThread=CreateThread(NULL,0,Graph2D_I,this,0,NULL);
		//AfxBeginThread( Graph2D_I, this);
}
void CAnalysisView::On2DGraph_II() 
{
		AfxBeginThread( Graph2D_II, this);
}
void CAnalysisView::On2DGraph_III() 
{
		AfxBeginThread( Graph2D_III, this);
}
void CAnalysisView::On2DGraph_IV() 
{
		AfxBeginThread( Graph2D_IV, this);
}
void CAnalysisView::On2DGraph_V() 
{
		AfxBeginThread( Graph2D_V, this);
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

void CAnalysisView::DisplayRoutine_for_requested_jobs()
{	
	CFigure *pFig=&m_Fig[0][0][0]; 	
	CClientDC objDC(this);
	int nIndex;

	m_RequestIdx=m_DispEventIdx;
	while(1){
		nIndex=WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS,m_Events,FALSE/*wait for all?*/,INFINITE/*timeout*/); 
		EnterCriticalSection(&m_CriticalSection);
		switch(m_DispEvents[m_RequestIdx][0]){
			case REFRESH_SCREEN:
				Re_measure_ClientView();
				objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
				break;
			case RESIZING_WIN:
				Re_measure_ClientView();
				Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
				Batch_Prepare4FigFrame();
				//Display
				Batch_DrawFigureFrames();
				Hilight_AllCaptured_Figs();
				DisplayTitle();
				if(m_IsFirstCallByIconPress==0)Show2D_I();
				break;
			case RENAMING_TITLE:
				DisplayTitle();
				break;
			case RENAMING_FIG:
				pFig->ComputeMappingXY();
				pFig->DispNameOfFigure(&m_DC[0]); 
				pFig->RefreshNameBox(&objDC,&m_DC[0]); 
				break;
			case RESCALING_AXES:
				pFig->ComputeMappingXY();
				int clm, row;
				for(clm=0;clm<m_dHowManyClmns;clm++){
					for(row=0;row<m_dHowManyRows;row++){
						pFig=&m_Fig[row][clm][0];
						if(pFig->m_Captured!=1)continue;
						DrawOneFigure(pFig, 1);
				}	}		
				if(m_IsFirstCallByIconPress==0)Show2D_I();
				break;
			case STOP_THREAD: goto END_OF_FUNCTION;
		}
		//Increase the index
		if(m_RequestIdx<m_DispEventIdx)m_RequestIdx++;
		//else if(m_RequestIdx==m_DispEventIdx)AfxMessageBox("m_RequestIdx==m_DispEventIdx");
		else{
			if(m_RequestIdx+1<MAXIMUM_WAIT_OBJECTS)m_RequestIdx++; else m_RequestIdx=0;
		}
		LeaveCriticalSection(&m_CriticalSection);
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
			pFig->SetMinMaxXY(0,10000,-50,50);pFig->SetLabelsXY("","");
			pFig->Set_2ndMinMaxY(0,400);
			pFig->m_Captured=0;
			pFig->m_Deleted=0;
	}	}
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	RequestDisplayEvent(RESIZING_WIN); //Send a message to redraw
}

void CAnalysisView::Show2D_I() 
{
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CTrial *pTrial;
	char toPrint[64];
	int i,j, FirstLocation;
	int *pData;
	__int64 TimeOfOccur;
	double userValue;
	
	//####################################################################
	//Read the file only once. This happens only when the user clicks the read-file icon.
	if(m_IsFirstCallByIconPress==1){
		if(GetFile_WholePath_Name_CurPath(TOREAD)==0)return;
		GetFolderName(m_FolderPath,m_JustFolderName);
		if(wildcmp("*.rst",m_JustFileName)==0){AfxMessageBox("This is not a *.rst file"); return;}
		m_IsFirstCallByIconPress=0;
		CFile theFile;
		if(theFile.Open(m_FileName, CFile::modeRead | CFile::shareDenyNone)== FALSE){
			return;
		};
		CArchive archive(&theFile, CArchive::load);

		//Convert Date to number
		char Date6Digit[64];
		if(wildcmp("norm_*", m_JustFileName)==0 && Find_first_6Digit_unintrupted_number(Date6Digit,m_JustFileName)==1){
			m_FileYearMonthDate=Convert_6digitDate_ToNumber(Date6Digit);
		}
		else{//Assume that the file has a new format.
			m_FileYearMonthDate=991231;
		}

		//Header of the file.
		m_NumTrialsInList=0;
		CArray<char, char& > m_Header;
		char dumpChar[64];
		int SizeOfData, SizeOfChar=sizeof(char), SizeOfInt=sizeof(int), TrialID;
		if(m_FileYearMonthDate>111216){ //If the file was made after Dec. 16, 2011: it has the "header".
 			archive.Read(&SizeOfData,SizeOfInt);
			m_Header.SetSize(SizeOfData+5);
 			if(m_FileYearMonthDate<120105){//Old type
				archive.Read(&(m_Header[0]),(SizeOfData+3)); 
			}
			else{
				archive.Read(&(m_Header[0]),(SizeOfData-1));
 				archive.Read(dumpChar,1);
			}
		}
	
		//Clean the Trial first
		POSITION aPos;
		aPos = m_TrialList.GetHeadPosition();
		while(aPos){delete m_TrialList.GetNext(aPos); }//It deletes the entities connected to the list.
		m_TrialList.RemoveAll(); //It deletes the list itself.
		m_NumTrialsInList=0;

		//Now do the stuff for CTrial class
		while( archive.Read(&TrialID,SizeOfInt)!=0 ){
			pTrial=new CTrial();
			m_TrialList.AddTail(pTrial);
			m_pTrial[m_NumTrialsInList]=pTrial;
			pTrial->Serialize(archive, this);
			m_NumTrialsInList++;
		}
		archive.Close();
		theFile.Close();
		m_RequestedTrlToDisp=0;
		InitSDF(m_Fig[0][0][0].m_dMaxX-m_Fig[0][0][0].m_dMinX);;
	}
	//####################################################################
	if(m_NumTrialsInList==0){AfxMessageBox("Empty file."); return;}

	pTrial=m_pTrial[m_RequestedTrlToDisp];
	sprintf(toPrint,"Trial #: %d ",m_RequestedTrlToDisp+1);
	pFig->m_Title.NameString=toPrint;
	pFig->Frame_Graph2D(&m_DC[0],1);
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5+1.5*m_fontHeight, m_JustFileName);
	//pFig->PutText(MsgPosX(pFig,0.2),MsgPosY(pFig,1.05),toPrint,&m_DC[0]);
	
	//#####  ECode
	m_initTime=pTrial->m_EventTimeInMs[0];
	int TotEvent=pTrial->m_numEventCodes,RGBidx;
	for(i=0; i<TotEvent; i++){
		RGBidx=(MaxRGB100-1)*((i+TotEvent*0.5*(i%2))/TotEvent); 
		if(RGBidx>=MaxRGB100)RGBidx=RGBidx-MaxRGB100;
		m_DC[0].SetTextColor(m_RGB100[RGBidx]);
		pFig->PutIntValWithPicxelBias(pTrial->m_EventTimeInMs[i]-m_initTime,MsgPosY(pFig,0.98),(i%2)*m_fontHeight,pTrial->m_EventCode[i], &m_DC[0]);
	}

	//#####  User data
	int NumUserVal, TotUserData=pTrial->m_NumUserData;
	for(i=0; i<TotUserData; i++){
		TimeOfOccur=pTrial->m_TimeOfOccurrenceInMs[i]-m_initTime;
		//Color
		RGBidx=(MaxRGB100-1)*((i+TotUserData*0.5*(i%2))/TotUserData); 
		if(RGBidx>=MaxRGB100)RGBidx=RGBidx-MaxRGB100;
		m_DC[0].SetTextColor(m_RGB100[RGBidx]);
		//Name
		pFig->PutText(TimeOfOccur,MsgPosY(pFig,0.90-0.025*i),pTrial->m_NameArray[i],&m_DC[0]);
		//Values
		FirstLocation=pTrial->m_FirstLocationOfValue[i];
		NumUserVal=pTrial->m_NumOfValues[i];
		for(j=0; j<NumUserVal; j++){
			userValue=pTrial->m_ValuesToRecord[FirstLocation+j];
			pFig->PutVal_p2(TimeOfOccur,MsgPosY(pFig,0.85-0.05*j-0.025*i),userValue, &m_DC[0]);
		}
	}
	m_DC[0].SetTextColor(m_BnW100[0]);

	//#####  Spikes
	int *pSpk=NULL;
	int numSpks, Nn,spk;
	int initX,endX;
	if(pTrial->m_numChans>0){
		m_DC[0].SelectObject(m_BLACK);
		CChannel *pChan;
		for(i=0;i<pTrial->m_numChans;i++){
			pChan=pTrial->m_pChan[i];
			if(pChan->m_ChanID==pFig->m_ChanID){
				for(Nn=0;Nn<pChan->m_numNns;Nn++){
					if(pChan->m_NnID[Nn]==pFig->m_NnID){
						numSpks=pChan->m_numSpikesOfNn[pFig->m_NnID];
						pSpk=&(pChan->m_SpikeTimesNn_ms[pFig->m_NnID][0]);
						for(spk=0; spk<numSpks; spk++){
							pFig->SpikeTic(pSpk[spk]-m_initTime, MsgPosY(pFig,0.), &m_DC[0]);
						}
						break;
					}
				}
				break;
			}
		}
	}
	if(pSpk!=NULL){
		//Calculate the SDF using the given spike array.
		// pFig->m_dMinX == m_SDF[0]
		ComputeSDF(pSpk, numSpks, (int)(m_initTime+pFig->m_dMinX));
		//Display
		double x;
		initX=pFig->m_dMinX;
		m_DC[0].SelectObject(&m_DkGREEN); 
		pFig->MoveTo2nd(initX, m_SDF[0],&m_DC[0]);// pFig->m_dMinX=m_SDF[0]
		for(i=0, x=initX; i<m_SDFArraySizeUsed;i++, x+=1){
			pFig->LineTo2nd(x, m_SDF[i],&m_DC[0]);
		}	
	}

	//####  Analog data
	for(i=0;i<pTrial->m_numBehaviorChannels;i++){
		if	   (i==0){pData=&pTrial->m_BehavioralAI0[0]; m_DC[0].SelectObject(m_RED); }
		else if(i==1){pData=&pTrial->m_BehavioralAI1[0]; m_DC[0].SelectObject(m_BLUE);}
		else break;
		initX=pFig->m_dMinX;
		if(initX<0)initX=0; if(initX>pTrial->m_numDataPointsOfBehaviorAI)initX=pTrial->m_numDataPointsOfBehaviorAI;
		endX=pFig->m_dMaxX;
		if(endX<0)endX=0; if(endX>pTrial->m_numDataPointsOfBehaviorAI)endX=pTrial->m_numDataPointsOfBehaviorAI;
		if(m_FileYearMonthDate>120205){
			pFig->Graph2D(initX,pData[initX]*0.001,9, &m_DC[0]);
			for(j=initX;j<endX;j++){
				pFig->Graph2D(j,pData[j]*0.001,2, &m_DC[0]);
			}
		}
		else{//Old format
			pFig->Graph2D(initX,pData[initX]*0.020,9, &m_DC[0]);
			for(j=initX;j<endX;j++){
				pFig->Graph2D(j,pData[j]*0.020,2, &m_DC[0]);
			}
		}
	}

	/////////////////////
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].RestoreDC(nSaveDC); 
}

int CAnalysisView::Convert_6digitDate_ToNumber(char *DateString) 
{	
	char date[64];
	int DateInNum;
	strcpy(date,DateString);
	Copy_fromNth_HowMany(date,DateString,5,2);
	DateInNum=atoi(date)*10000;
	Copy_fromNth_HowMany(date,DateString,3,2);
	DateInNum+=atoi(date);
	Copy_fromNth_HowMany(date,DateString,1,2);
	DateInNum+=atoi(date)*100;
	m_FileYearMonthDate=DateInNum;
	return DateInNum;
}

int CAnalysisView::Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany) 
{	
	strcpy(Dest,Source);
	strrev(Dest);
	Dest[strlen(Dest)-(Nth-1)]='\0';
	strrev(Dest);
	Dest[HowMany]='\0';
	return 0;
}

int CAnalysisView::Find_first_6Digit_unintrupted_number(char *SixDigitString, char *NameString) 
{	
	int numDigits,Start, endI, i=0;
	endI=strlen(NameString);
	Re_Search:;
	numDigits=0; Start=i;
	for(i=Start;i<endI;i++){
		if(NameString[i]<'0' || NameString[i]>'9'){i++; goto Re_Search;}
		numDigits++;
		if(numDigits==6){
			Copy_fromNth_HowMany(SixDigitString,NameString,Start+1,6);
			break;
		}
	}

	if(numDigits==6)return 1;
	else  return 0;//Failed to Find
}

void CAnalysisView::Show2D_II() 
{	
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CFigure *pFig2=&m_Fig[1][0][0];
	CFigure *pFig3=&m_Fig[0][1][0]; 
	CFigure *pFig4=&m_Fig[1][1][0];
	pFig->Frame_Graph2D(&m_DC[0],1);
	pFig2->Frame_Graph2D(&m_DC[0],1);
	pFig3->Frame_Graph2D(&m_DC[0],1);
	pFig4->Frame_Graph2D(&m_DC[0],1);
	
	InitGraphics();
	ResizeGraphics();
	for(double z=0; z>-20; z-=0.1){
		DrawGraphics(z);
	}

	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}

void CAnalysisView::Show2D_III() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CFigure *pFig2=&m_Fig[1][0][0];
	pFig->Frame_Graph2D(&m_DC[0],1);
	pFig2->Frame_Graph2D(&m_DC[0],1);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	
	int R,G,B;
	double  SqDist,redCenter=0.8;
	COLORREF RGB;
#define MaxI 100.
	double RedKer[(int)MaxI],GrnKer[(int)MaxI],BluKer[(int)MaxI];
	for(int i=0; i<MaxI; i++){
		//########  RED
		SqDist=SQ_DISTANCE_AB_1D(i,MaxI*redCenter);
		if(i<MaxI*redCenter)RedKer[i]=(255/(1+exp(-(((double)i)/MaxI-0.6)/0.03)));
		else RedKer[i]=255*exp(-SqDist/(MaxI*30.));
		m_DC[0].SelectObject(m_RED);
		if(i>0){pFig->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]); }

		//########  GREEN
		SqDist=SQ_DISTANCE_AB_1D(i,MaxI*0.55);
		GrnKer[i]=255*exp(-SqDist/(MaxI*10.));
		m_DC[0].SelectObject(m_GREEN);
		if(i>0){pFig->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);}
		
		//########  BLUE
		SqDist=SQ_DISTANCE_AB_1D(i,MaxI*0.30);
		BluKer[i]=255*exp(-SqDist/(MaxI*6.));
		m_DC[0].SelectObject(m_BLUE);
		if(i>0){pFig->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);}
	}
	for(int i=0; i<MaxI; i++){
		pFig->FillSolidRect(i,160,6,50,RGB(RedKer[i], 0, 0), &m_DC[0]);
		pFig->FillSolidRect(i,120,6,50,RGB(0, GrnKer[i], 0), &m_DC[0]);
		pFig->FillSolidRect(i,80 ,6,50,RGB(0, 0, BluKer[i]), &m_DC[0]);
		pFig->FillSolidRect(i,40 ,6,50,RGB(RedKer[i], GrnKer[i], BluKer[i]), &m_DC[0]);
	}
	int I[(int)MaxI];
	m_DC[0].SelectObject(m_GRAY2);
	for(int i=0; i<MaxI; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		if(i>0)pFig2->Graph2D(i-1,I[i-1],i,I[i],2, &m_DC[0]);
	}		
	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	pFig->PutText(MsgPosX(pFig,0), MsgPosY(pFig,1.03 ), "My original mix. This considers luminance as a factor",&m_DC[0]);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;	
}


void CAnalysisView::Show2D_IV() 
{  
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CFigure *pFig2=&m_Fig[1][0][0];
	pFig->Frame_Graph2D(&m_DC[0],1);
	pFig2->Frame_Graph2D(&m_DC[0],1);
	
	#define MaxI 100.
	int R,G,B;
	double  SqDist,Ker;
	double RedKer[(int)MaxI],GrnKer[(int)MaxI],BluKer[(int)MaxI];
	COLORREF RGB;
	double startRED=MaxI*0.7, endRED=MaxI*1   ,sigSqRED=300 ;
	double startGRN=MaxI*0.30, endGRN=MaxI*0.70,sigSqGRN=300 ;
	double startBLU=MaxI*0,  endBLU=MaxI*0.30,sigSqBLU=300 ;
	//########  RED
	for(int i=0; i<MaxI; i++){
		m_DC[0].SelectObject(m_RED);
		if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			SqDist=SQ_DISTANCE_AB_1D(startRED, i);
		else if(i>=startRED && i<endRED)//When the i is somewhere inside the plateau (maximum) of a kernel
			SqDist=0;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			SqDist=SQ_DISTANCE_AB_1D(endRED, i);
		Ker= exp(-SqDist/sigSqRED);
		RedKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]); }
		//########  GREEN
		m_DC[0].SelectObject(m_GREEN);
		if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			SqDist=SQ_DISTANCE_AB_1D(startGRN, i);
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			SqDist=0;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			SqDist=SQ_DISTANCE_AB_1D(endGRN, i);
		Ker= exp(-SqDist/sigSqGRN);
		GrnKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);}
		//########  BLUE
		m_DC[0].SelectObject(m_BLUE);
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			SqDist=SQ_DISTANCE_AB_1D(startBLU, i);
		else if(i>=startBLU && i<endBLU)//When the i is somewhere inside the plateau (maximum) of a kernel
			SqDist=0;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			SqDist=SQ_DISTANCE_AB_1D(endBLU, i);
		Ker= exp(-SqDist/sigSqBLU);
		BluKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);}
	}		
	
	/////////////////////
	for(int i=0; i<MaxI; i++){
		pFig->FillSolidRect(i,160,6,50,RGB((int)RedKer[i], 0, 0), &m_DC[0]);
		pFig->FillSolidRect(i,120,6,50,RGB(0, (int)GrnKer[i], 0), &m_DC[0]);
		pFig->FillSolidRect(i,80 ,6,50,RGB(0, 0, (int)BluKer[i]), &m_DC[0]);
		pFig->FillSolidRect(i,40 ,6,50,RGB((int)RedKer[i], (int)GrnKer[i], (int)BluKer[i]), &m_DC[0]);
	}		
	int I[(int)MaxI];
	m_DC[0].SelectObject(m_GRAY2);
	for(int i=0; i<MaxI; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		if(i>0)pFig2->Graph2D(i-1,I[i-1],i,I[i],2, &m_DC[0]);
	}		
	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	pFig->PutText(MsgPosX(pFig,0), MsgPosY(pFig,1.03 ), "More colorful mix using Gaussian kernels",&m_DC[0]);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}

void CAnalysisView::Show2D_V() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CFigure *pFig2=&m_Fig[1][0][0];
	CFigure *pFig3=&m_Fig[0][1][0]; 
	CFigure *pFig4=&m_Fig[1][1][0];
	pFig->Frame_Graph2D(&m_DC[0],1);
	pFig2->Frame_Graph2D(&m_DC[0],1);
	pFig3->Frame_Graph2D(&m_DC[0],1);
	pFig4->Frame_Graph2D(&m_DC[0],1);
	
	#define MaxI 100.
	double RedKer[(int)MaxI],GrnKer[(int)MaxI],BluKer[(int)MaxI];
	LinearColorMix_adjusted(m_RGB100,MaxRGB100);
	int R,G,B;
	double  SqDist,Ker, slope1=1/0.35,slope2=1/0.15, redB=1-slope2*0.65,grnB=0,bluB=0, redBB=1-slope2*0.35,grnBB=slope1,bluBB=1+slope2*0.35;
	COLORREF RGB;
	double startRED=MaxI*0.65, endRED=MaxI*1.0 ;
	double startGRN=MaxI*0.35, endGRN=MaxI*0.65;
	double startBLU=MaxI*0.0, endBLU=MaxI*0.35;
	//########  RED
	for(int i=0; i<MaxI; i++){
		m_DC[0].SelectObject(m_RED);
		if(i<0.5*MaxI)Ker=0;
		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope2*(i/(float)MaxI)+redB;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=1;
		RedKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]); }
		//########  GREEN
		m_DC[0].SelectObject(m_GREEN);
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=0;
		else if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope1*(i/(float)MaxI)+grnB;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope1*(i/(float)MaxI)+grnBB;
		GrnKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);}
		//########  BLUE
		m_DC[0].SelectObject(m_BLUE);
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope1*(i/(float)MaxI)+bluB;
		else if(i>=startBLU && i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*MaxI)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope2*(i/(float)MaxI)+bluBB;
		else 
			Ker=0;
		BluKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);}
	}		
	
	/////////////////////
	float I[(int)MaxI];
	float Ideal[(int)MaxI];
	float lowerBnd=28, higherBnd=54, adjFctr[(int)MaxI];
	m_DC[0].SelectObject(m_GRAY2);
	for(int i=0; i<MaxI; i++){
		Ideal[i]=lowerBnd+i*(higherBnd/(float)MaxI);
		if(i>0)pFig2->Graph2D(i-1,Ideal[i-1],i,Ideal[i],2, &m_DC[0]);
	}
	for(int i=0; i<MaxI; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		adjFctr[i]=Divide(Ideal[i],(float)I[i]);
		if(i>0)pFig2->Graph2D(i-1,I[i-1],i,I[i],2, &m_DC[0]);
	}		
	/////////////////////
	for(int i=0; i<MaxI; i++){
		pFig->FillSolidRect(i,160,6,50,RGB((int)RedKer[i], 0, 0), &m_DC[0]);
		pFig->FillSolidRect(i,120,6,50,RGB(0, (int)GrnKer[i], 0), &m_DC[0]);
		pFig->FillSolidRect(i,80 ,6,50,RGB(0, 0, (int)BluKer[i]), &m_DC[0]);
		pFig->FillSolidRect(i,40 ,6,50,RGB((int)RedKer[i], (int)GrnKer[i], (int)BluKer[i]), &m_DC[0]);
	}		
	/////////////////////
	//Adjust
	/////////////////////
#define BASEADJUST 0.3
	float howMuchAdjust; //How much percentage to adjsut?
	float prevBlue,a,K;
	for(int i=MaxI-1; i>=0; i--){
		howMuchAdjust=BASEADJUST+BASEADJUST*((MaxI-i)/(float)MaxI);
		//howMuchAdjust=BASEADJUST;
		RedKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(RedKer[i]<0)RedKer[i]=0; else if(RedKer[i]>255)RedKer[i]=255;
		GrnKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(GrnKer[i]<0)GrnKer[i]=0; else if(GrnKer[i]>255)GrnKer[i]=255;
		BluKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(BluKer[i]<0)BluKer[i]=0; else if(BluKer[i]>255)BluKer[i]=255;
		
		if(i==endBLU){prevBlue=BluKer[i]; a=prevBlue/(7.*i);K=6*prevBlue/(float)(7.);}
		if(i<endBLU){
			BluKer[i]=prevBlue;
		}

		if(i<MaxI-1){
			m_DC[0].SelectObject(m_RED);
			pFig3->Graph2D(i,RedKer[i],i-1,RedKer[i+1],2, &m_DC[0]);pFig4->Graph2D(i,RedKer[i],i+1,RedKer[i+1],2, &m_DC[0]); 
			m_DC[0].SelectObject(m_GREEN);
			pFig3->Graph2D(i,GrnKer[i],i+1,GrnKer[i+1],2, &m_DC[0]);pFig4->Graph2D(i,GrnKer[i],i+1,GrnKer[i+1],2, &m_DC[0]);
			m_DC[0].SelectObject(m_BLUE);
			pFig3->Graph2D(i,BluKer[i],i+1,BluKer[i+1],2, &m_DC[0]);pFig4->Graph2D(i,BluKer[i],i+1,BluKer[i+1],2, &m_DC[0]);
		}
	}		
	m_DC[0].SelectObject(m_GRAY2);
	for(int i=0; i<MaxI; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		if(i>0)pFig4->Graph2D(i-1,I[i-1],i,I[i],2, &m_DC[0]);
	}		
	for(int i=0; i<MaxI; i++){
		pFig3->FillSolidRect(i,160,6,50,RGB((int)RedKer[i], 0, 0), &m_DC[0]);
		pFig3->FillSolidRect(i,120,6,50,RGB(0, (int)GrnKer[i], 0), &m_DC[0]);
		pFig3->FillSolidRect(i,80 ,6,50,RGB(0, 0, (int)BluKer[i]), &m_DC[0]);
		pFig3->FillSolidRect(i,40 ,6,50,RGB((int)RedKer[i], (int)GrnKer[i], (int)BluKer[i]), &m_DC[0]);
	}		
	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	pFig->PutText(MsgPosX(pFig,0), MsgPosY(pFig,1.03 ), "More colorful mix using Gaussian kernels",&m_DC[0]);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}
void CAnalysisView::Show2D_VI() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CFigure *pFig2=&m_Fig[1][0][0];
	CFigure *pFig3=&m_Fig[0][1][0]; 
	CFigure *pFig4=&m_Fig[1][1][0];
	pFig->Frame_Graph2D(&m_DC[0],1);
	pFig2->Frame_Graph2D(&m_DC[0],1);
	pFig3->Frame_Graph2D(&m_DC[0],1);
	pFig4->Frame_Graph2D(&m_DC[0],1);
	
	#define MaxI 100.
	double RedKer[(int)MaxI],GrnKer[(int)MaxI],BluKer[(int)MaxI];
	LinearColorMix_adjusted(m_RGB100,MaxRGB100);
	int R,G,B;
	double  SqDist,Ker, slope1=1/0.35,slope2=1/0.15, redB=1-slope2*0.65,grnB=0,bluB=0, redBB=1-slope2*0.35,grnBB=slope1,bluBB=1+slope2*0.35;
	COLORREF RGB;
	double startRED=MaxI*0.65, endRED=MaxI*1.0 ;
	double startGRN=MaxI*0.35, endGRN=MaxI*0.65;
	double startBLU=MaxI*0.0, endBLU=MaxI*0.35;
	//########  RED
	for(int i=0; i<MaxI; i++){
		m_DC[0].SelectObject(m_RED);
		if(i<0.5*MaxI)Ker=0;
		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=sin(PIover2*((i-0.5*MaxI)/(0.15*MaxI)));
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=1;
		RedKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,RedKer[i-1],i,RedKer[i],2, &m_DC[0]); }
		//########  GREEN
		m_DC[0].SelectObject(m_GREEN);
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=0;
		else if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope1*(i/(float)MaxI)+grnB;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope1*(i/(float)MaxI)+grnBB;
		GrnKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,GrnKer[i-1],i,GrnKer[i],2, &m_DC[0]);}
		//########  BLUE
		m_DC[0].SelectObject(m_BLUE);
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope1*(i/(float)MaxI)+bluB;
		else if(i>=startBLU && i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*MaxI)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=cos(PIover2*((i-endBLU)/(0.15*MaxI)));
		else 
			Ker=0;
		BluKer[i]=255*Ker;
		if(i>0){pFig->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);pFig2->Graph2D(i-1,BluKer[i-1],i,BluKer[i],2, &m_DC[0]);}
	}		
	
	/////////////////////
	float I[(int)MaxI];
	float Ideal[(int)MaxI];
	float lowerBnd=28, higherBnd=54, adjFctr[(int)MaxI];
	m_DC[0].SelectObject(m_GRAY2);
	for(int i=0; i<MaxI; i++){
		Ideal[i]=lowerBnd+i*(higherBnd/(float)MaxI);
		if(i>0)pFig2->Graph2D(i-1,Ideal[i-1],i,Ideal[i],2, &m_DC[0]);
	}
	for(int i=0; i<MaxI; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		adjFctr[i]=Divide(Ideal[i],(float)I[i]);
		if(i>0)pFig2->Graph2D(i-1,I[i-1],i,I[i],2, &m_DC[0]);
	}		
	/////////////////////
	for(int i=0; i<MaxI; i++){
		pFig->FillSolidRect(i,160,6,50,RGB((int)RedKer[i], 0, 0), &m_DC[0]);
		pFig->FillSolidRect(i,120,6,50,RGB(0, (int)GrnKer[i], 0), &m_DC[0]);
		pFig->FillSolidRect(i,80 ,6,50,RGB(0, 0, (int)BluKer[i]), &m_DC[0]);
		pFig->FillSolidRect(i,40 ,6,50,RGB((int)RedKer[i], (int)GrnKer[i], (int)BluKer[i]), &m_DC[0]);
	}		
	/////////////////////
	//Adjust
	/////////////////////
#define BASEADJUST 0.3
	float howMuchAdjust; //How much percentage to adjsut?
	float prevBlue,a,K;
	for(int i=MaxI-1; i>=0; i--){
		howMuchAdjust=BASEADJUST+BASEADJUST*((MaxI-i)/(float)MaxI);
		//howMuchAdjust=BASEADJUST;
		RedKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(RedKer[i]<0)RedKer[i]=0; else if(RedKer[i]>255)RedKer[i]=255;
		GrnKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(GrnKer[i]<0)GrnKer[i]=0; else if(GrnKer[i]>255)GrnKer[i]=255;
		BluKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(BluKer[i]<0)BluKer[i]=0; else if(BluKer[i]>255)BluKer[i]=255;
		
		if(i==endBLU){prevBlue=BluKer[i]; a=prevBlue/(7.*i);K=6*prevBlue/(float)(7.);}
		if(i<endBLU){
			BluKer[i]=prevBlue;
		}

		if(i<MaxI-1){
			m_DC[0].SelectObject(m_RED);
			pFig3->Graph2D(i,RedKer[i],i-1,RedKer[i+1],2, &m_DC[0]);pFig4->Graph2D(i,RedKer[i],i+1,RedKer[i+1],2, &m_DC[0]); 
			m_DC[0].SelectObject(m_GREEN);
			pFig3->Graph2D(i,GrnKer[i],i+1,GrnKer[i+1],2, &m_DC[0]);pFig4->Graph2D(i,GrnKer[i],i+1,GrnKer[i+1],2, &m_DC[0]);
			m_DC[0].SelectObject(m_BLUE);
			pFig3->Graph2D(i,BluKer[i],i+1,BluKer[i+1],2, &m_DC[0]);pFig4->Graph2D(i,BluKer[i],i+1,BluKer[i+1],2, &m_DC[0]);
		}
	}		
	m_DC[0].SelectObject(m_GRAY2);
	for(int i=0; i<MaxI; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		if(i>0)pFig4->Graph2D(i-1,I[i-1],i,I[i],2, &m_DC[0]);
	}		
	for(int i=0; i<MaxI; i++){
		pFig3->FillSolidRect(i,160,6,50,RGB((int)RedKer[i], 0, 0), &m_DC[0]);
		pFig3->FillSolidRect(i,120,6,50,RGB(0, (int)GrnKer[i], 0), &m_DC[0]);
		pFig3->FillSolidRect(i,80 ,6,50,RGB(0, 0, (int)BluKer[i]), &m_DC[0]);
		pFig3->FillSolidRect(i,40 ,6,50,RGB((int)RedKer[i], (int)GrnKer[i], (int)BluKer[i]), &m_DC[0]);
	}		
	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	pFig->PutText(MsgPosX(pFig,0), MsgPosY(pFig,1.03 ), "More colorful mix using Gaussian kernels",&m_DC[0]);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}


void CAnalysisView::Show2D_VII() 
{
}


void CAnalysisView::Show2D_VIII()
{ 

}


void CAnalysisView::Manual_Fig_Labels_n_MinMaxXY() 
{
	//////////////////////////////////////////
	m_Board[0 ]. SetMinMaxXY(0,8000,0,1000);	m_Board[0 ].SetLabelsXY("","0  : ");	
	m_Board[1 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[1 ].SetLabelsXY("","1  : ");	
	m_Board[2 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[2 ].SetLabelsXY("","2  : ");	
	m_Board[3 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[3 ].SetLabelsXY("","3  : ");	
	m_Board[4 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[4 ].SetLabelsXY("","4  : ");	
	m_Board[5 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[5 ].SetLabelsXY("","5  : ");	
	m_Board[6 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[6 ].SetLabelsXY("","6  : ");	
	m_Board[7 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[7 ].SetLabelsXY("","7  : ");	
	m_Board[8 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[8 ].SetLabelsXY("","8  : ");	
	m_Board[9 ]. SetMinMaxXY(0,500,0,1	  );	m_Board[9 ].SetLabelsXY("","9  : ");	

	int i,brdIdx;
	float stepX,stepY;
	CFigure *pFig;
	
	m_Board[0].m_howManyX=11; m_Board[0].m_howManyY=11; 
	
	//Batch labeling
	for(brdIdx=0;brdIdx<MAX_GRAPHS;brdIdx++){
		pFig=&m_Board[brdIdx];
		if(pFig->m_howManyX>0){
			stepX=(pFig->m_dMaxX-pFig->m_dMinX)/(pFig->m_howManyX-1);
			for(i=0;i<pFig->m_howManyX+1;i++){
				if(i%1==0){pFig->m_IsLabeledTicX[i]=1;pFig->m_XTics[i]=pFig->m_dMinX+stepX*i;}
				else pFig->m_IsLabeledTicY[i]=0;//Do not label
			} 
		}
		if(pFig->m_howManyY>0){
			stepY=(pFig->m_dMaxY-pFig->m_dMinY)/(pFig->m_howManyY-1);
			for(i=0;i<pFig->m_howManyY+1;i++){
				if(i%1==0){pFig->m_IsLabeledTicY[i]=1;pFig->m_YTics[i]=pFig->m_dMinY+stepY*i;}
				else pFig->m_IsLabeledTicY[i]=0;//Do not label
			} 
		}
	}

	//An example of "individual" labeling
	pFig=&m_Board[1];
	pFig->m_howManyY=4; 
	pFig->m_IsLabeledTicY[0]=1; pFig->m_YTics[0]=0;
	pFig->m_IsLabeledTicY[1]=1; pFig->m_YTics[1]=0.5;
	pFig->m_IsLabeledTicY[2]=1; pFig->m_YTics[2]=0.8;
	pFig->m_IsLabeledTicY[3]=1; pFig->m_YTics[3]=1;
}

void CAnalysisView::Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) 
{
#define TICK_STEPX(pB) ((pB->m_dMaxX-pB->m_dMinX)/(pB->m_howManyX-1))
#define TICK_STEPY(pB) ((pB->m_dMaxY-pB->m_dMinY)/(pB->m_howManyY-1))
	int i;
	float TickStepX, TickStepY;

	pBrd->m_howManyX=HowManyTicsX; TickStepX=TICK_STEPX(pBrd);
	for(i=0;i<pBrd->m_howManyX;i++){pBrd->m_IsLabeledTicX[i]=1;pBrd->m_XTics[i]=pBrd->m_dMinX+i*TickStepX;} 
	pBrd->m_howManyY=HowManyTicsY; TickStepY=TICK_STEPY(pBrd);
	for(i=0;i<pBrd->m_howManyY;i++){pBrd->m_IsLabeledTicY[i]=1;pBrd->m_YTics[i]=pBrd->m_dMinY+i*TickStepY;} 
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
			pFig->ComputeMappingXY();
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
	m_DC[0].SetBkColor(RGB(220, 220, 220));
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5, m_JustFileName);
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
	LogFont.lfHeight = -72 * rFont.Height() / pDC->GetDeviceCaps(LOGPIXELSY);
	LogFont.lfWidth = -72 * rFont.Width() / pDC->GetDeviceCaps(LOGPIXELSX) / sText.GetLength();

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



void CAnalysisView::OnKernelSigma()
{
	if(m_fSigma*1.5<2000)m_fSigma=(int)(m_fSigma*1.5);
	else return;
	InitSDF(m_Fig[0][0][0].m_dMaxX-m_Fig[0][0][0].m_dMinX);;
	Show2D_I();
}


void CAnalysisView::OnKernelSigmaSmaller()
{
	if(m_fSigma*.666>2)m_fSigma=(int)(m_fSigma*.666);
	else return;
	InitSDF(m_Fig[0][0][0].m_dMaxX-m_Fig[0][0][0].m_dMinX);;
	Show2D_I();
}
