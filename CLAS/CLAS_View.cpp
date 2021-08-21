// CLAS_View.cpp : implementation of the CCLAS_View class
//

#include "stdafx.h"
#include "CLAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "CLAS_Doc.h"
#include "Figure.h"
#include "CLAS_View.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "DialogueBox.h"
#include "StringEdit.h"
#include "Mmsystem.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CCLAS_View::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CCLAS_View

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CCLAS_View, CView)

BEGIN_MESSAGE_MAP(CCLAS_View, CView)
	//{{AFX_MSG_MAP(CCLAS_View)
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
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_SCALE_DELETE_FIGS, &CCLAS_View::OnDeleteFigs)
	ON_COMMAND(ID_SCALE_DELETE_THE_REST, &CCLAS_View::OnDelete_the_Rest_Figs)
	ON_COMMAND(ID_FILE_SAVE, &CCLAS_View::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CCLAS_View::OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT_STRING1, &CCLAS_View::OnEnChangeEditString1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CCLAS_View construction/destruction
CCLAS_View::CCLAS_View()
{
	// TODO: add construction code here
	m_nColorShow=BLACK_BRUSH;
	m_bShow2D=0;  
	m_bFistTime=1;
	m_nKey=0;
	m_nSpeed=0;
	FlagVerScreen=0;
	m_bIsToggleON=0;
	m_bIsON=0;
	m_dScaleUpDn=1;
	m_dHowManyRows=MaxFigRow; m_dHowManyClmns=MaxFigCln;
	m_BlownUpPaneID=-1;
	m_DispEventIdx=0;
	////////////////////////////////////
	m_NullPen.CreatePen(PS_NULL, 1,RGB(0,0,0));
	m_RED.CreatePen(PS_SOLID, 1,RGB(255,0,0));
	m_RED3.CreatePen(PS_SOLID, 3,RGB(255,0,0));
	m_RED5.CreatePen(PS_SOLID, 5,RGB(255,0,0));
	m_RED8.CreatePen(PS_SOLID, 8,RGB(255,0,0));
	m_ORANGE2.CreatePen(PS_SOLID, 2,RGB(255,150,0));
	m_GREEN.CreatePen(PS_SOLID,1 ,RGB(0,255,0));
	m_GREEN2.CreatePen(PS_SOLID,2 ,RGB(0,255,0));
	m_BLUE.CreatePen(PS_SOLID, 1,RGB(100,100,255));
	m_BLUE3.CreatePen(PS_SOLID, 3,RGB(100,100,255));
	m_BLUE5.CreatePen(PS_SOLID, 5,RGB(100,100,255));
	m_BLUE8.CreatePen(PS_SOLID, 8,RGB(100,100,255));
	m_GREEN3.CreatePen(PS_SOLID, 3,RGB(0,255,0));
	m_DkGREEN.CreatePen(PS_SOLID, 1,RGB(0,155,0));
	m_CYAN1.CreatePen(PS_SOLID, 1,RGB(0,255,255));
	m_CYAN2.CreatePen(PS_SOLID, 2,RGB(0,255,255));
	m_CYAN3.CreatePen(PS_SOLID, 3,RGB(200,255,255));
	m_PINK.CreatePen(PS_SOLID, 1,RGB(255,150,150));
	m_PINK3.CreatePen(PS_SOLID, 3,RGB(255,150,150));
	m_BROWN.CreatePen(PS_SOLID, 1,RGB(240,130,0));
	m_VIOLET.CreatePen(PS_SOLID, 1,RGB(200,90,250));
	m_VIOLET2.CreatePen(PS_SOLID, 2,RGB(200,90,250));
	m_VIOLET3.CreatePen(PS_SOLID, 3,RGB(200,90,250));
	m_VIOLETLIGHT1.CreatePen(PS_SOLID, 1,RGB(205,5,255));
	m_VIOLETLIGHT2.CreatePen(PS_SOLID, 2,RGB(205,5,255));
	m_bkPen.CreatePen(PS_SOLID, 1,RGB(220,220,220));
	m_GRAY1.CreatePen(PS_SOLID, 1,RGB(170,170,170));
	m_GRAY2.CreatePen(PS_SOLID, 3,RGB(170,170,170));
	m_BlueGRAY.CreatePen(PS_SOLID, 1,RGB(210,210,240));
	m_BLACK.CreatePen(PS_SOLID, 1,RGB(0,0,0));
	m_BLACK3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
	m_BLACK2.CreatePen(PS_SOLID, 2,RGB(0,0,0));
	m_BLACK5.CreatePen(PS_SOLID, 5,RGB(0,0,0));
	//SigmoidColorMix(m_RGB100,MaxRGB100);
	//GaussianColorMix(m_RGB100,MaxRGB100);
	MyOriginalColorMix(m_RGB100,MaxRGB100);

	int i,j;
	for(i=0;i<MaxPen;i++){
		j=(MaxRGB100-1)-i*(MaxRGB100/MaxPen);
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;
	LogBrh.lbColor=RGB(220, 220, 220);
	m_GrayBrush.CreateBrushIndirect( &LogBrh );
	LogBrh.lbColor=RGB(255,0,0);
	m_RedBrush.CreateBrushIndirect( &LogBrh );

	/////////////// Parameters For Applications
	m_GlobalTitle="NoName";
	m_NewlyLoaded=0; m_DoNotErase=0;
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	m_SelectedItemID=-1; m_numCaptured=0;

	
	/////  Statistics  ///////
	m_FirstTime0ElementErr=1;

}					


CCLAS_View::~CCLAS_View() 
{
	m_bIsON=0;
	
	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	
	CloseSharedMemoryArea();

	int i;
	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	m_GrayBrush.DeleteObject(); m_RedBrush.DeleteObject();m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen;i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); m_rgbPEN3[i].DeleteObject();
	}
	m_NullPen.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject();m_GREEN.DeleteObject(); m_GREEN2.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN1.DeleteObject();m_CYAN2.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();m_VIOLET.DeleteObject();
	m_ORANGE2.DeleteObject(); m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_VIOLETLIGHT2.DeleteObject();m_VIOLETLIGHT1.DeleteObject();m_BLACK.DeleteObject();m_BLACK2.DeleteObject();m_BLACK3.DeleteObject();m_bkPen.DeleteObject();
	m_GRAY1.DeleteObject();m_GRAY2.DeleteObject();m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	m_font.DeleteObject();
}

BOOL CCLAS_View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCLAS_View drawing
UINT Graph2D_I(LPVOID WinObjPtr);
#define SIZE_OF_BITMAP_X 0.5
#define SIZE_OF_BITMAP_Y 0.5
void CCLAS_View::OnDraw(CDC* pDC)
{
#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CCLAS_Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1){
		VirMemDC();
		m_dHowManyClmns=-1;
		OnFileOpen();//Try to open a default root file.
		m_bFistTime=0;
		if(m_dHowManyClmns!=-1){
			AfxBeginThread( Graph2D_I, this);
			return;//Opening a default root file has been successful.
		}
		else{//Failed to open a default root file => implement the programs own version.
			sprintf(m_FileName,"");
			sprintf(m_JustFileName,"NO root file");
			m_dHowManyClmns=MaxFigCln; 
		}
		Re_measure_ClientView();
		Start_a_NEW_set_of_figures();
		AfxBeginThread( Graph2D_I, this);
	}
	else{
		CRect rt;
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			RequestDisplayEvent(RESIZING_WIN);
		}
		else{//Just display without changing anything
			CClientDC objDC(this);
			objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
		}
	}
}

void CCLAS_View::Start_a_NEW_set_of_figures()
{			
	int i,j;
	CFigure *pFig;
	m_numCaptured=0;
	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->SetMinMaxXY(-1000,1000,0,100);pFig->SetLabelsXY("","");
			pFig->m_Captured=0;
			pFig->m_Deleted=0;
	}	}
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	Batch_SetFontSize(&m_DC[0]);
	RequestDisplayEvent(RESIZING_WIN);
}

void CCLAS_View::Re_measure_ClientView()
{			
	CRect rt;
	GetClientRect(&rt);
	if(m_bFistTime==1){
		CFont* pFont = m_DC[0].GetCurrentFont();
		LOGFONT LogFont;
		pFont->GetLogFont(&LogFont);
		m_fontHeight=LogFont.lfHeight;
	}
	m_TitleArea.top=rt.top;	m_TitleArea.bottom=rt.top+0; m_topOfFigs=m_TitleArea.bottom;
	m_TitleArea.left=rt.left; m_TitleArea.right=rt.right;
	m_dSizeOfOneRow =(rt.bottom-m_topOfFigs-ViewMargin4FigsAlongEdges)/m_dHowManyRows;  if(m_dSizeOfOneRow <=0)m_dSizeOfOneRow=0;
	m_dSizeOfOneClmn=(rt.right-rt.left-ViewMargin4FigsAlongEdges)/m_dHowManyClmns; if(m_dSizeOfOneClmn<=0)m_dSizeOfOneClmn=0;
	m_savedClientRect=rt;m_savedClientRect.top=m_topOfFigs;
}


/////////////////////////////////////////////////////////////////////////////
// CCLAS_View printing

BOOL CCLAS_View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCLAS_View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCLAS_View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCLAS_View diagnostics

#ifdef _DEBUG
void CCLAS_View::AssertValid() const
{
	CView::AssertValid();
}

void CCLAS_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCLAS_Doc* CCLAS_View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCLAS_Doc)));
	return (CCLAS_Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCLAS_View message handlers
UINT Graph2D_I(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_I() ;
	if(*(pView->M_Blip_OFF)==1)AfxGetMainWnd()->SendMessage(WM_CLOSE);
	return 0;
}
UINT Graph2D_II(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_II() ;
	return 0;
}
UINT Graph2D_III(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_III() ;
	return 0;
}
UINT Graph2D_IV(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_IV() ;
	return 0;
}
UINT Graph2D_V(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_V() ;
	return 0;
}
UINT Graph2D_VI(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_VI() ;
	return 0;
}
UINT Graph2D_VII(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_VII() ;
	return 0;
}
UINT Graph2D_VIII(LPVOID WinObjPtr) //A global Function
{
	CCLAS_View* pView=(CCLAS_View*) WinObjPtr;
	pView->Show2D_VIII() ;
	return 0;
}


void CCLAS_View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//if(m_nShowMode==GRAPH_3D){ThD.KeyDown(nChar); goto ToEND;}
	m_KeyCode=nChar;
	switch(nChar){
		case VK_DELETE:
			OnDeleteFigs();
			break;
		default: break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCLAS_View::On2DGraph_I() 
{
		AfxBeginThread( Graph2D_I, this);
}
void CCLAS_View::On2DGraph_II() 
{
		AfxBeginThread( Graph2D_II, this);
}
void CCLAS_View::On2DGraph_III() 
{
		AfxBeginThread( Graph2D_III, this);
}
void CCLAS_View::On2DGraph_IV() 
{
		AfxBeginThread( Graph2D_IV, this);
}
void CCLAS_View::On2DGraph_V() 
{
		AfxBeginThread( Graph2D_V, this);
}
void CCLAS_View::On2DGraph_VI() 
{
		AfxBeginThread( Graph2D_VI, this);
}
void CCLAS_View::On2DGraph_VII() 
{
		AfxBeginThread( Graph2D_VII, this);
}
void CCLAS_View::On2DGraph_VIII() 
{
		AfxBeginThread( Graph2D_VIII, this);
}

void CCLAS_View::VirMemDC()
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
		m_bmp[i].CreateCompatibleBitmap(&aDC, m_nScrMaxX*SIZE_OF_BITMAP_X, m_nScrMaxY*SIZE_OF_BITMAP_Y);
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_bkBrush);
		m_DC[i].SelectObject(&m_font);
	}
	m_SavedDC=m_DC[0].SaveDC();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################  2D Graphics #######################################


void CCLAS_View::Show2D_I() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;

	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	m_pFig=&m_Fig[0][0][0]; 
	m_pFig->Frame_Graph2D(&m_DC[0],0);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);

	//############ Create the memory map
	CreateSharedMemoryArea();
	int i;
	double Saved_DataPointsPerWave=180;
	CAiChannel *pChan;
	for(i=0; i<MAX_AI_CHAN;i++){
		pChan=&(AiChan[i]);
		pChan->M_AI=M_AI[i];
		pChan->M_usableAiIdx=M_usableAiIdx[i];
	}
	for(i=0; i<NUM_NN_AI_CHAN;i++){	
		pChan=&(AiChan[i]);
		pChan->M_firstIdxOfWavelet=M_firstIdxOfWavelet[i];
		pChan->M_lengthOfSignal=M_lengthOfSignal[i];
		pChan->M_AdjustXToXthreshold=M_AdjustXToXthreshold[i];
		pChan->M_WvEcode=M_WvEcode[i];
		pChan->M_DataPointsPerWave=&M_DataPointsPerWave[i];
	}

	while(*M_usableAiIdx[0]<0){	Sleep(10);	if(m_bIsON==0)goto END_SIG_ACQUISION;}
	m_LastTimeDisplayed=*M_LatestTimeStampInMicroS;
	if(*M_SpotlightChanID_forCLAS>=0)Saved_DataPointsPerWave=*AiChan[*M_SpotlightChanID_forCLAS].M_DataPointsPerWave;

	int DispIdx=0; 
	int NumSelectedWaveformsDisplayed=0,NumNonSelectedWaveformsDisplayed=0;
	timeBeginPeriod(1); //This one makes the Sleep() function accurate
	while(1){
		if(m_bIsON==0 || *M_Blip_OFF==1)break;
		//################  Take care of any display demand
		while(DispIdx!=m_DispEventIdx){
			DisplayStuff(DispIdx,&objDC);
			if(++DispIdx>=MAX_DISP_REQUESTS)DispIdx=0;
		}
		//############### Refresh the screen at every 60ms (~16Hz)
		if(*M_LatestTimeStampInMicroS-m_LastTimeDisplayed>60000||(NumSelectedWaveformsDisplayed>*M_NumSelectedWaveformsDisplayed)){
			RefreshFrame();
			NumSelectedWaveformsDisplayed=0;
			NumNonSelectedWaveformsDisplayed=0;
			m_LastTimeDisplayed=*M_LatestTimeStampInMicroS;
		}
		else if(*M_LatestTimeStampInMicroS<m_LastTimeDisplayed)m_LastTimeDisplayed=*M_LatestTimeStampInMicroS;//the server has newly restarted.
	
		if(m_bIsON==0)break;
		Sleep(5); 
		if(m_bIsON==0)break;
		
		if(*M_SpotlightChanID_forCLAS>=0 && *AiChan[*M_SpotlightChanID_forCLAS].M_DataPointsPerWave!=Saved_DataPointsPerWave){
			m_Fig[0][0][0].m_dMinX=-(fabs(M_WaveSpanPre_ms[*M_SpotlightChanID_forCLAS]));
			m_Fig[0][0][0].m_dMaxX= (fabs(M_WaveSpanPost_ms[*M_SpotlightChanID_forCLAS]));
			RequestDisplayEvent(RESIZING_WIN);
			Saved_DataPointsPerWave=*AiChan[*M_SpotlightChanID_forCLAS].M_DataPointsPerWave;
		}
		//##############  Draw Waves
		while(NumSelectedWaveformsDisplayed<*M_NumSelectedWaveformsDisplayed){
			DrawWave(M_SelectedWaveFormsToDisp[NumSelectedWaveformsDisplayed++],*M_SpotlightChanID_forCLAS);
		}
		while(NumNonSelectedWaveformsDisplayed<*M_NumNonSelectedWaveformsDisplayed){
			DrawWave(M_NonSelectedWaveFormsToDisp[NumNonSelectedWaveformsDisplayed++],*M_SpotlightChanID_forCLAS);
		}
		if(m_bIsON==0)break;
	}
	timeEndPeriod(1);//match this with 	timeBeginPeriod(1) above 

	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
	END_SIG_ACQUISION:;
}

//In case there is a need to centralize the dispalying processes, embed this this function in a host thread
void CCLAS_View::DisplayStuff(int RequestIdx, CDC *pObjDC)
{	
	switch(m_DispEvents[RequestIdx][0]){
	case RESIZING_WIN:
		Re_measure_ClientView();
		Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
		Batch_Prepare4FigFrame();
		//Display
		Batch_DrawFigureFrames();
		break;
	default:
		Sleep(0);
	}
	//if(RequestIdx==RENAMING_TITLE){
	//	//DisplayTitle();
	//}
	//if(RequestIdx==RENAMING_FIG){
	//	CClientDC objDC(this);
	//	m_pFig->ComputeMappingXY();
	//	m_pFig->DispNameOfFigure(&m_DC[0]); 
	//	m_pFig->RefreshNameBox(&objDC,&m_DC[0]); 
	//}	
	//if(RequestIdx==RESCALING_AXES){
	//	CClientDC objDC(this);
	//	m_pFig->ComputeMappingXY();
	//	m_pFig->Frame_Graph2D(&m_DC[0],1); 
	//	m_pFig->RefreshFrame(&objDC,&m_DC[0]); 
	//	Hilight_OneCapturedFig(m_pFig); 
	//}	
}

//Every N ms this will refresh all the attached figures to display the waves.
void CCLAS_View::RefreshFrame()
{
	CClientDC objDC(this);
	m_pFig->RefreshPane(&objDC,&m_DC[0]);
	m_DC[0].SelectObject(&m_NullPen);
	m_pFig->ErasePaneBlack(&m_DC[0]);
}

void CCLAS_View::DrawWave(int WvID, int ChanID)
{
	if(ChanID<0)return;
	if(AiChan[ChanID].M_lengthOfSignal[WvID]==(*(AiChan[ChanID].M_DataPointsPerWave))){
		if(AiChan[ChanID].M_firstIdxOfWavelet[WvID]+(*(AiChan[ChanID].M_DataPointsPerWave))>*M_usableAiIdx[ChanID]){
			Sleep((int)M_WaveSpanPost_ms[ChanID]);// Wait for the M_AI[] to catch up.
		}
		if(AiChan[ChanID].M_WvEcode[WvID]==-1){
			m_pFig->DispOneWaveform(&AiChan[ChanID].M_AI[AiChan[ChanID].M_firstIdxOfWavelet[WvID]],(*(AiChan[ChanID].M_DataPointsPerWave)), AiChan[ChanID].M_WvEcode[WvID], AiChan[ChanID].M_AdjustXToXthreshold[WvID],&m_DC[0]);//Continuous waveform along the signal queue 
		}
		else m_pFig->DispOneWaveform(&AiChan[ChanID].M_AI[AiChan[ChanID].M_firstIdxOfWavelet[WvID]],(*(AiChan[ChanID].M_DataPointsPerWave)), AiChan[ChanID].M_WvEcode[WvID], AiChan[ChanID].M_AdjustXToXthreshold[WvID],&m_DC[0]);//Continuous waveform along the signal queue 
	}
	//else DispOneWaveform_FragmentedWave(WvID, ChanID);//Fragmented waveform
}

void CCLAS_View::DispOneWaveform_FragmentedWave(int WaveID, int ChanID)
{
	if(ChanID<0)return;
	CDC *pDC=&m_DC[0];
	int dataCnt, headPart,tailPart;
	double wf, cnvrt=1000./(*M_SamplingRate);
	double *pW=&(AiChan[ChanID].M_AI[AiChan[ChanID].M_firstIdxOfWavelet[WaveID]]), maxY=m_pFig->m_dMaxY, minY=m_pFig->m_dMinY;
	double bias=cnvrt*AiChan[ChanID].M_AdjustXToXthreshold[WaveID]+m_pFig->m_dMinX;

	if(AiChan[ChanID].M_WvEcode[WaveID]==-1)m_DC[0].SelectObject(m_GRAY1);
	else if(AiChan[ChanID].M_WvEcode[WaveID]==0)pDC->SelectObject(m_VIOLETLIGHT1);
	else if(AiChan[ChanID].M_WvEcode[WaveID]==1)pDC->SelectObject(m_GREEN);
	else if(AiChan[ChanID].M_WvEcode[WaveID]==2)pDC->SelectObject(m_CYAN1);
	else 	AfxMessageBox("AiChan[ChanID].M_WvEcode[]=? 5");

	wf=*pW; 
	if(wf>maxY)wf=maxY; else if(wf<minY)wf=minY;
	m_pFig->MyMoveTo(m_pFig->m_dMinX ,wf, pDC);
	headPart=AiChan[ChanID].M_lengthOfSignal[WaveID];
	for(dataCnt=1;dataCnt<headPart;dataCnt++){
		wf=*(pW+dataCnt); if(wf>maxY){wf=maxY;}	else if(wf<minY)wf=minY;
		m_pFig->MyLineTo(cnvrt*dataCnt+bias,wf, pDC);
	}	
	tailPart=(*(AiChan[ChanID].M_DataPointsPerWave))-headPart;
	bias=cnvrt*(headPart+AiChan[ChanID].M_AdjustXToXthreshold[WaveID])+m_pFig->m_dMinX;
	pW=&(AiChan[ChanID].M_AI[0]);
	for(dataCnt=0;dataCnt<tailPart;dataCnt++){
		wf=wf=*(pW+dataCnt); if(wf>maxY){wf=maxY;}	else if(wf<minY)wf=minY;
		m_pFig->MyLineTo(cnvrt*dataCnt+bias,wf, pDC);
	}	
}

void CCLAS_View::RequestDisplayEvent(int Event) 
{
	RequestDisplayEvent(Event,  NULL,  NULL);
}
void CCLAS_View::RequestDisplayEvent(int Event,  int SomeIntValue) 
{
	RequestDisplayEvent(Event,  SomeIntValue,  NULL); 
}
void CCLAS_View::RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2) 
{
	m_DispEvents[m_DispEventIdx][0]=Event;
	m_DispEvents[m_DispEventIdx][1]=SomeIntValue1; 
	m_DispEvents[m_DispEventIdx][2]=SomeIntValue2; 
	if(m_DispEventIdx+1<MAX_DISP_REQUESTS)m_DispEventIdx++; else m_DispEventIdx=0;
}


void CCLAS_View::Show2D_II() 
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
	LinearColorMix(m_RGB100,MaxRGB100);
	for(int i=0; i<MaxI; i++){
		pFig3->FillSolidRect(i,250,6,60,m_RGB100[i],&m_DC[0]);
	}		
	LinearColorMix_adjusted(m_RGB100,MaxRGB100);
	for(int i=0; i<MaxI; i++){
		pFig3->FillSolidRect(i,200,6,60,m_RGB100[i],&m_DC[0]);
	}		
	MyOriginalColorMix(m_RGB100,MaxRGB100);
	for(int i=0; i<MaxI; i++){
		pFig3->FillSolidRect(i,150,6,60,m_RGB100[i],&m_DC[0]);
	}		
	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	pFig->PutText(MsgPosX(pFig,0), MsgPosY(pFig,1.03 ), "More colorful mix using Gaussian kernels",&m_DC[0]);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}


void CCLAS_View::Show2D_III() 
{

}

void CCLAS_View::Show2D_IV() 
{  

}

void CCLAS_View::Show2D_V() 
{

}
void CCLAS_View::Show2D_VI() 
{
}


void CCLAS_View::Show2D_VII() 
{
}


void CCLAS_View::Show2D_VIII()
{ 

}

void CCLAS_View::CommentI(CDC* pDC)
{
}

void CCLAS_View::CommentV(CDC* pDC)
{
}

void CCLAS_View::CommentII(CDC* pDC)
{
}

void CCLAS_View::CommentIII(CDC* pDC)
{
}

void CCLAS_View::CommentIV(CDC* pDC)
{
}

void CCLAS_View::Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) 
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


void CCLAS_View::Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) 
{
#define F2FY 0
#define F2FX 0
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
void CCLAS_View::Batch_SetFontSize(CDC* pDC) 
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
void CCLAS_View::Batch_Prepare4FigFrame() 
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

void CCLAS_View::Batch_DrawFigureFrames() 
{	
	CClientDC objDC(this);
	m_DC[0].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY); 
	m_DoNotErase=1;//This is to inform the Labeling part not to erase previous site, which may not correspond to new position due to resize.
	int i,j;
	CFigure *pFig;
	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			if(pFig->m_Deleted==0)pFig->Frame_Graph2D(&m_DC[0],0); 
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
void CCLAS_View::FontInRect(CDC *pDC, CString sText, CRect &rFont)
{
	// Gets current font details
	CFont* pFont = pDC->GetCurrentFont();
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

void CCLAS_View::OnChangeParameters() 
{
	CDialogueBox	ParameterBOX;

	ParameterBOX.m_dVariable1=m_dMaximumX;
	ParameterBOX.m_dVariable2=m_dMinimumX;
	ParameterBOX.m_dVariable3=m_dMaximumY;
	ParameterBOX.m_dMinimumY=m_dMinimumY;
	if(ParameterBOX.DoModal()==IDOK){
		if(ParameterBOX.m_dVariable1<=ParameterBOX.m_dVariable2||ParameterBOX.m_dVariable3<=ParameterBOX.m_dMinimumY){AfxMessageBox("m_dMaximumX<=m_dMinimumX OR m_dMaximumY<=m_dMinimumY ==> Try again", MB_OK); return;}
		else{
			m_dMaximumX=ParameterBOX.m_dVariable1;
			m_dMinimumX=ParameterBOX.m_dVariable2;
			m_dMaximumY=ParameterBOX.m_dVariable3;
			m_dMinimumY=ParameterBOX.m_dMinimumY;
		}
	}

}

void CCLAS_View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CCLAS_View::OnEnChangeEditString1()
{
	StringEdit	ParameterBOX;

	if(m_CallingFrom==1)ParameterBOX.m_UserInputString1=m_GlobalTitle;
	else if(m_CallingFrom==2)ParameterBOX.m_UserInputString1=m_pSpotlightFig->m_Title.NameString;
	if(ParameterBOX.DoModal()==IDOK){
		if(m_CallingFrom==1)m_GlobalTitle=ParameterBOX.m_UserInputString1;
		else if(m_CallingFrom==2)m_pSpotlightFig->m_Title.NameString=ParameterBOX.m_UserInputString1;
	}
}


void CCLAS_View::CreateSharedMemoryArea()
{
	//In case the memory has been created by some other process, try it by opening. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;	
	#include "MemMap_Open_DAS.h" 
}

void CCLAS_View::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}

