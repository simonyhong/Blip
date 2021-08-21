// XCORView.cpp : implementation of the CXCOR_View class
//

#include "stdafx.h"
#include "XCOR.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "XCOR_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "XCOR_View.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "Dialog_Box.h"
#include "StringEdit.h"
#include "AcceptOneNum.h"
#include "Trial.h"
#include "VarDefinitionsSDF.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CXCOR_View::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CXCOR_View

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CXCOR_View, CView)

BEGIN_MESSAGE_MAP(CXCOR_View, CView)
	//{{AFX_MSG_MAP(CXCOR_View)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_2DGraph_III, &CXCOR_View::On_Start_AFRESH)
	ON_COMMAND(ID_2DGraph_IV, &CXCOR_View::OnRasterPlot)
	ON_UPDATE_COMMAND_UI(ID_2DGraph_IV, &CXCOR_View::CheckRasterPlot)
	ON_COMMAND(ID_2DGraph_V, OnStop_or_Pause)
	ON_COMMAND(ID_Parameters, OnChangeParameters)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_IS_TO_DISP_MARKS, &CXCOR_View::OnIsToDisplayMarks)
	ON_UPDATE_COMMAND_UI(ID_IS_TO_DISP_MARKS, &CXCOR_View::OnUpdateIsToDisplayMarks)
	ON_UPDATE_COMMAND_UI(ID_2DGraph_V, &CXCOR_View::Check_Stop_or_Pause)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_SCALE_DELETE_FIGS, &CXCOR_View::OnDeleteFigs)
	ON_COMMAND(ID_SCALE_DELETE_THE_REST, &CXCOR_View::OnDelete_the_Rest_Figs)
	ON_COMMAND(ID_FILE_SAVE, &CXCOR_View::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CXCOR_View::OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT_STRING1, &CXCOR_View::OnEnChangeEditString1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CXCOR_View construction/destruction
CXCOR_View::CXCOR_View()
{
	// TODO: add construction code here
	m_nColorShow=BLACK_BRUSH;
	m_bShow2D=0;  
	m_bFistTime=1;
	m_nKey=0;
	m_nSpeed=0;
	FlagVerScreen=0;
	m_bIsON=0;
	m_dScaleUpDn=1;
	m_dHowManyRows=MaxFigRow; m_dHowManyClmns=MaxFigCln;
	m_BlownUpPaneID=-1;
	m_bIsScrXYforIJ=0;
	m_usableTrialIdxInMem=-1;
	m_Stop_Or_Pause=1;
	m_NumTrialsInMap_InThisProgram=0;
	FirstTimeMsg=1;
	m_DispEventIdx=0;
	m_isDisplayerRunning=0;
	int i,j;
	for(i=0;i<MAX_TRLS_IN_MAP;i++){
		m_PrvEvent_ReMapped[i]=0;
		for(j=0;j<NUM_NN_AI_CHAN;j++){
			m_PrvSpkTimeOfNn1MAP_ReMapped[i][j]=0;
			m_PrvSpkTimeOfNn2MAP_ReMapped[i][j]=0;
			m_PrvSpkTimeOfNn3MAP_ReMapped[i][j]=0;
		}
	}

	////////////////////////////////////
	m_NULL.CreatePen(PS_NULL, 1,RGB(0,0,0));
	m_RED.CreatePen(PS_SOLID, 1,RGB(255,0,0));
	m_RED3.CreatePen(PS_SOLID, 3,RGB(255,0,0));
	m_RED5.CreatePen(PS_SOLID, 5,RGB(255,0,0));
	m_RED8.CreatePen(PS_SOLID, 8,RGB(255,0,0));
	m_White2.CreatePen(PS_SOLID, 2,RGB(255,255,255));
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

	for(i=0;i<MaxPen;i++){
		j=(MaxRGB100-1)-i*(MaxRGB100/MaxPen);
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}

	/////////////// Parameters For Applications
	m_GlobalTitle="NoName";
	m_NewlyLoaded=0; m_DoNotErase=0;
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	m_SelectedItemID=-1; m_numCaptured=0;
	m_AlignmentEcode=0;
	for(i=0;i<MaxFigCln;i++){
		for(j=0;j<MaxFigRow;j++){
			m_Fig[j][i][0].m_row=j;
			m_Fig[j][i][0].m_clm=i;
			m_Fig[j][i][0].m_dpth=0;
		}
	}

	
	m_HalfKernel_ms=0;//Just to avoid error; in the case the user manipulates the figures before the XCOR_Main() starts.
	m_FullSizeXCOR_ms=0; m_FullKernel_ms=0;//Just to avoid error; in the case the user manipulates the figures before the XCOR_Main() starts.
	m_NumOfDisplayedRasterRows=0;//Just to avoid error; in the case the user manipulates the figures before the XCOR_Main() starts.
	m_FirstTime0ElementErr=1;
	m_IsFirstTimeRasterPlot=1;
	m_IsToDisplayMarks=0;
	m_pUsableSpikeIdx_RefNn=NULL;
	m_XCOR_RefChan=0; m_XCOR_SbjChan=0;
	m_XCOR_RefNn=0; m_XCOR_SbjNn=0;
	m_fSigma=0.01;
	m_UserReqest_ChangeXOR_Ker=0;
	m_Ref_AndOr_SbjChnaged=0;
	for(int i=0; i<HOW_MANY_TRLS_TO_DISP; i++){m_SizeOfRaster[i]=1000; m_Raster[i].SetSize(m_SizeOfRaster[i]); m_NumSpikesInRow[i]=0;}
}					


CXCOR_View::~CXCOR_View() 
{
	m_bIsON=0;


	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	

	CloseSharedMemoryArea();
	for(int i=0; i<HOW_MANY_TRLS_TO_DISP; i++){m_Raster[i].RemoveAll();}

	int i;
	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen;i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); m_rgbPEN3[i].DeleteObject();
	}
	m_font.DeleteObject();
	m_NULL.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject(); m_GREEN.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();m_VIOLET.DeleteObject();
	m_White2.DeleteObject(); m_ORANGE2.DeleteObject(); m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_BLACK.DeleteObject();m_BLACK2.DeleteObject();m_BLACK3.DeleteObject();m_bkPen.DeleteObject();
	m_GRAY2.DeleteObject();m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8;
}

BOOL CXCOR_View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
UINT KILL_THREAD(LPVOID WinObjPtr) //A global Function
{
	CXCOR_View* pView=(CXCOR_View*) WinObjPtr;
	while(1){
		if(*(pView->M_Blip_OFF)==1){
			Sleep(50); //Give some time for the XCOR_Main() loop to close, in the case it is running.
			AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
			return 0;
		}
		Sleep(20);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CXCOR_View drawing
#define SIZE_OF_BITMAP_X 0.8
#define SIZE_OF_BITMAP_Y 0.6
void CXCOR_View::OnDraw(CDC* pDC)
{
	CXCORDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1){
		VirMemDC();
		m_dHowManyClmns=-1;
		m_Fig[0][0][0].m_pView=this;
		OnFileOpen();//Try to open a default root file.
		CreateSharedMemoryArea();
		AfxBeginThread( KILL_THREAD, this); //This is running just to detect the user's commend of "close all Blip windows". This thread is needed to close RAS even when it is Stopped. There must be a better way to do this rather than running a thread all the time. 
		if(m_dHowManyClmns!=-1){
			m_bFistTime=0;
			return;//Opening a default root file has been successive.
		}
		//Failed to open a default root file => implement the programs own version.
		sprintf(m_FileName,"");
		sprintf(m_JustFileName,"NO root file");
		m_dHowManyClmns=MaxFigCln; 
		Re_measure_ClientView();
		Start_a_NEW_set_of_figures();
		m_bFistTime=0;
	}
	else{
		CRect rt;
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			if(m_savedClientRect.right<rt.right ||m_savedClientRect.bottom<rt.bottom){
				if(m_savedClientRect.right>=SIZE_OF_BITMAP_X*m_nScrMaxX ||m_savedClientRect.bottom>=SIZE_OF_BITMAP_Y*m_nScrMaxY){
					int Y=SIZE_OF_BITMAP_Y*m_nScrMaxY;
					char ScreenMsg[1024]; sprintf(ScreenMsg,"Simon: To save Bitmap memory, the size of the window is currently limited to Horizontal:%d, Vertical:%d\n To change this setting, adjust SIZE_OF_BITMAP_X and SIZE_OF_BITMAP_Y in the program.\n Let me know if you have an easier solution.",(int)(SIZE_OF_BITMAP_X*m_nScrMaxX),Y);
					AfxMessageBox(ScreenMsg);
				}
			}
			if(	m_isDisplayerRunning==1)RequestDisplayEvent(RESIZING_WIN);
			else { CClientDC ObjDC(this); DispForResizing( &ObjDC);	}
		}
		else{//Just display without changing anything
			if(m_isDisplayerRunning==1)RequestDisplayEvent(REFRESH_SCREEN);
			else{
				CClientDC ObjDC(this);
				ObjDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
			}
		}
	}
}

void CXCOR_View::Start_a_NEW_set_of_figures()
{			
	int i,j;
	CFigure *pFig;
	m_numCaptured=0;
	for(i=0;i<m_dHowManyClmns;i++){
		for(j=0;j<m_dHowManyRows;j++){
			pFig=&m_Fig[j][i][0];
			pFig->SetMinMaxXY(-1000,1000,0,100);pFig->SetLabelsXY("","");
			pFig->m_Captured=0;
			pFig->m_Deleted=0;
	}	}
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	Batch_SetFontSize(&m_DC[0]);
	Batch_Prepare4FigFrame();
	Batch_DrawFigureFrames(1);
}

void CXCOR_View::Re_measure_ClientView()
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
// CXCOR_View printing

BOOL CXCOR_View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CXCOR_View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CXCOR_View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CXCOR_View diagnostics

#ifdef _DEBUG
void CXCOR_View::AssertValid() const
{
	CView::AssertValid();
}

void CXCOR_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXCORDoc* CXCOR_View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXCORDoc)));
	return (CXCORDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXCOR_View message handlers

UINT XCOR_MAIN(LPVOID WinObjPtr) //A global Function
{
	CXCOR_View* pView=(CXCOR_View*) WinObjPtr;
	pView->XCOR_Main() ;
	if(*(pView->M_Blip_OFF)==1)AfxGetMainWnd()->SendMessage(WM_CLOSE);

	return 0;
}


void CXCOR_View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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


void CXCOR_View::On_Start_AFRESH() 
{
	//Wipe previous history.
	m_Ref_AndOr_SbjChnaged=1;
	m_UserReqest_ChangeXOR_Ker=1;

	//Request to start XCOR_MAIN
	OnRasterPlot();//This function starts to run, only when it is not already runing.
}


void CXCOR_View::OnRasterPlot() 
{
	if(m_Stop_Or_Pause==1){
		m_Stop_Or_Pause=0;//Starting
		if(m_isDisplayerRunning==1)RequestDisplayEvent(REDRAW_ALL_CONTENT);
		else 		RedrawAllContent(); //Since the display loop is not running.
	}
	if(m_bIsON==0){
		AfxBeginThread( XCOR_MAIN, this);
	}
}
void CXCOR_View::CheckRasterPlot(CCmdUI *pCmdUI) 
{
	pCmdUI->SetCheck(m_Stop_Or_Pause==0);
}
void CXCOR_View::OnStop_or_Pause() 
{
	if(m_Stop_Or_Pause==0){
		m_Stop_Or_Pause=1; //Stop or pause da
		if(m_pUsableSpikeIdx_RefNn!=NULL && m_pUsableSpikeIdx_SubNn!=NULL){
			m_XCOR_RefSpkIdx=*m_pUsableSpikeIdx_RefNn;
			m_XCOR_SbjSpk_StartIdx=*m_pUsableSpikeIdx_SubNn;
			m_XCOR_SbjSpk_EndIdx=*m_pUsableSpikeIdx_SubNn;
		}
	}
}
void CXCOR_View::Check_Stop_or_Pause(CCmdUI *pCmdUI) 
{
	pCmdUI->SetCheck(m_Stop_Or_Pause==1);
}


void CXCOR_View::VirMemDC()
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
	
	int result;
	for(int i=0;i<MaxDC;i++){
		result=m_DC[i].CreateCompatibleDC(&aDC);
		result=m_bmp[i].CreateCompatibleBitmap(&aDC, m_nScrMaxX*SIZE_OF_BITMAP_X, m_nScrMaxY*SIZE_OF_BITMAP_Y);
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_bkBrush);
		m_DC[i].SelectObject(&m_font);
		//m_DC[i].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
	}
	m_SavedDC=m_DC[0].SaveDC();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################  2D Graphics #######################################
#define MY_PEEK_MESSAGE {if(::PeekMessage (&message, NULL,0,0, PM_REMOVE)){::TranslateMessage(&message);::DispatchMessage(&message);}	}	
//With a 32bit int (INT_MAX~ 2147,000,000), a micro second counter will last only ~ 30 minutes!!
//With a 64bit int (INT_MAX~ 9,223,372,036,854,775,807), a micro second counter will last ~287000 years!!

void CXCOR_View::FindFirstIdxOfSbjSpike() 
{
	int tmpIdx, PrevStatus;
	m_FoundStart=0; m_FoundEnd=0;
	m_StartTimeWindow_MicS_SbjSpk=m_pSpikeTimeStampOf_RefNn[m_XCOR_RefSpkIdx] - m_XcorSize_ofPreRefSpike_inMicS;
	m_EndTimeWindow_MicS_SbjSpk  =m_pSpikeTimeStampOf_RefNn[m_XCOR_RefSpkIdx] + m_XcorSize_ofPostRefSpike_inMicS;
	if(*m_pUsableSpikeIdx_SubNn>0){
		if(m_XCOR_SbjSpk_StartIdx<0){m_XCOR_SbjSpk_StartIdx=*m_pUsableSpikeIdx_SubNn; m_XCOR_SbjSpk_EndIdx=*m_pUsableSpikeIdx_SubNn; }//Initializing m_XCOR_SbjSpk_StartIdx, m_XCOR_SbjSpk_EndIdx for the first time.
		if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_StartIdx]>=m_StartTimeWindow_MicS_SbjSpk) && (m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_StartIdx]<=m_EndTimeWindow_MicS_SbjSpk)){//Within the limit.
			PrevStatus=0;
		}
		else if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_StartIdx]<m_StartTimeWindow_MicS_SbjSpk)){//Smaller than the limit
			PrevStatus=-1;
		}
		else{ //Bigger than the limit
			PrevStatus=1;
		}
		while(1){
			if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_StartIdx]>=m_StartTimeWindow_MicS_SbjSpk) && (m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_StartIdx]<=m_EndTimeWindow_MicS_SbjSpk)){//Within the limit.
				if(PrevStatus==1){m_FoundEnd=1; m_XCOR_SbjSpk_EndIdx=m_XCOR_SbjSpk_StartIdx;}//From bigger to within.
				else if(PrevStatus==-1)m_FoundStart=1; //From samll to within.
				tmpIdx=idxSpkQ(m_XCOR_SbjSpk_StartIdx-1); //Search for another spike backward in time.
				if(m_pSpikeTimeStampOf_SubNn[tmpIdx]<0){m_FoundStart=1; break;} //Use the earliest spike as the first.
				if((m_pSpikeTimeStampOf_SubNn[tmpIdx]<m_StartTimeWindow_MicS_SbjSpk)){m_FoundStart=1; break;}//Going below the limit => Keep the current one.
				m_XCOR_SbjSpk_StartIdx=tmpIdx;
				PrevStatus=0;
			}
			else if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_StartIdx]<m_StartTimeWindow_MicS_SbjSpk)){//Smaller than the limit
				if(m_XCOR_SbjSpk_StartIdx==*m_pUsableSpikeIdx_SubNn)break;
				tmpIdx=idxSpkQ(m_XCOR_SbjSpk_StartIdx+1); //Search for another spike forward in time.
				if(m_pSpikeTimeStampOf_SubNn[tmpIdx]>m_EndTimeWindow_MicS_SbjSpk)break;//Bigger than the limit
				m_XCOR_SbjSpk_StartIdx=tmpIdx; 
				PrevStatus=-1;
			}
			else{ //Bigger than the limit
				tmpIdx=idxSpkQ(m_XCOR_SbjSpk_StartIdx-1); //Search for another spike backward in time.
				if(m_pSpikeTimeStampOf_SubNn[tmpIdx]<0)break;
				if((m_pSpikeTimeStampOf_SubNn[tmpIdx]<m_StartTimeWindow_MicS_SbjSpk)){break;}
				m_XCOR_SbjSpk_StartIdx=tmpIdx;
				PrevStatus=1;
			}
		}	
		if(m_FoundStart==0){if(m_FoundEnd==1)AfxMessageBox("m_FoundStart==0,  m_FoundEnd==1?"); return;}
		if(m_FoundEnd==1)return; //At this point, m_FoundStart==1, so all found.
		if(m_XCOR_SbjSpk_EndIdx<0)m_XCOR_SbjSpk_EndIdx=m_XCOR_SbjSpk_StartIdx; //in case this is the first search of the program use the StartIdx as the initial point of the EndIdx.
		//So, where is the EndIdx?
		if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_EndIdx]>=m_StartTimeWindow_MicS_SbjSpk) && (m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_EndIdx]<=m_EndTimeWindow_MicS_SbjSpk)){//Within the limit.
			PrevStatus=0;
		}
		else if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_EndIdx]<m_StartTimeWindow_MicS_SbjSpk)){//Smaller than the limit
			PrevStatus=-1;
		}
		else{ //Bigger than the limit
			PrevStatus=1;
		}		
		while(1){
			if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_EndIdx]>=m_StartTimeWindow_MicS_SbjSpk) && (m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_EndIdx]<=m_EndTimeWindow_MicS_SbjSpk)){//Within the limit.
				if(PrevStatus==1){m_FoundEnd=1; break;} //From bigger to within.
				if(m_XCOR_SbjSpk_EndIdx==*m_pUsableSpikeIdx_SubNn){m_FoundEnd=1;  break;}//There is anymore subject spike to examine.
				tmpIdx=idxSpkQ(m_XCOR_SbjSpk_EndIdx+1); //Search for another spike forward in time.
				if((m_pSpikeTimeStampOf_SubNn[tmpIdx]>m_EndTimeWindow_MicS_SbjSpk)){m_FoundEnd=1; break;}//Going over the limit => Keep the current one.
				m_XCOR_SbjSpk_EndIdx=tmpIdx;
				PrevStatus=0;
			}
			else if((m_pSpikeTimeStampOf_SubNn[m_XCOR_SbjSpk_EndIdx]<m_StartTimeWindow_MicS_SbjSpk)){//Smaller than the limit
				if(m_XCOR_SbjSpk_EndIdx==*m_pUsableSpikeIdx_SubNn)break;
				tmpIdx=idxSpkQ(m_XCOR_SbjSpk_EndIdx+1); //Search for another spike forward in time.
				if(m_pSpikeTimeStampOf_SubNn[tmpIdx]>m_EndTimeWindow_MicS_SbjSpk)break;//Bigger than the limit
				m_XCOR_SbjSpk_EndIdx=tmpIdx; 
				PrevStatus=-1;
			}
			else{ //Bigger than the limit
				tmpIdx=idxSpkQ(m_XCOR_SbjSpk_EndIdx-1); //Search for another spike backward in time.
				if(m_pSpikeTimeStampOf_SubNn[tmpIdx]<0)break;
				if((m_pSpikeTimeStampOf_SubNn[tmpIdx]<m_StartTimeWindow_MicS_SbjSpk)){break;}
				m_XCOR_SbjSpk_EndIdx=tmpIdx;
				PrevStatus=1;
			}
		}	
	}
	if(m_XCOR_SbjSpk_EndIdx-m_XCOR_SbjSpk_StartIdx>100){
		Sleep(0);
	}
}

void CXCOR_View::Reset_Ref_Sbj_IDXes() 
{
	//Reference neuron
	if(m_XCOR_RefNn==0){m_pUsableSpikeIdx_RefNn=M_usableSpikeIdxOfNn1[m_XCOR_RefChan]; m_pSpikeTimeStampOf_RefNn=M_spikeTimeStampOfNn1[m_XCOR_RefChan];}
	else if(m_XCOR_RefNn==1){m_pUsableSpikeIdx_RefNn=M_usableSpikeIdxOfNn2[m_XCOR_RefChan]; m_pSpikeTimeStampOf_RefNn=M_spikeTimeStampOfNn2[m_XCOR_RefChan];}
	else if(m_XCOR_RefNn==2){m_pUsableSpikeIdx_RefNn=M_usableSpikeIdxOfNn3[m_XCOR_RefChan]; m_pSpikeTimeStampOf_RefNn=M_spikeTimeStampOfNn3[m_XCOR_RefChan];}
	else{ AfxMessageBox("Wrong NnID"); exit(0);}
	//Subject neuron
	if(m_XCOR_SbjNn==0){m_pUsableSpikeIdx_SubNn=M_usableSpikeIdxOfNn1[m_XCOR_SbjChan]; m_pSpikeTimeStampOf_SubNn=M_spikeTimeStampOfNn1[m_XCOR_SbjChan];}
	else if(m_XCOR_SbjNn==1){m_pUsableSpikeIdx_SubNn=M_usableSpikeIdxOfNn2[m_XCOR_SbjChan]; m_pSpikeTimeStampOf_SubNn=M_spikeTimeStampOfNn2[m_XCOR_SbjChan];}
	else if(m_XCOR_SbjNn==2){m_pUsableSpikeIdx_SubNn=M_usableSpikeIdxOfNn3[m_XCOR_SbjChan]; m_pSpikeTimeStampOf_SubNn=M_spikeTimeStampOfNn3[m_XCOR_SbjChan];}
	else{ AfxMessageBox("Wrong NnID"); exit(0);}

	m_XCOR_RefSpkIdx=-1; m_XCOR_SbjSpk_StartIdx=-1; m_XCOR_SbjSpk_EndIdx=-1;
	for(int i=0;i<m_FullSizeXCOR_ms;i++)m_XCOR[i]=0;  
	m_NumWeightings=1;
	m_RasterIdx=-1;
	m_NumOfDisplayedRasterRows=0;
	if(m_XCOR_RefChan==m_XCOR_SbjChan && m_XCOR_RefNn==m_XCOR_SbjNn)m_AutoCorrelation=1; else m_AutoCorrelation=0;
}

void CXCOR_View::Set_XCOR_range_and_Kernel() 
{
	CFigure *pFig=&m_Fig[0][0][0];
	m_HalfKernel_ms=2*m_fSigma; m_FullKernel_ms=2*m_HalfKernel_ms;
	CreateKer(m_fSigma, m_HalfKernel_ms);
	if(pFig->m_dMinX>0 || pFig->m_dMaxX<5){ if(pFig->m_dMinX<0){ pFig->m_dMinX=0; AfxMessageBox("pFig->m_dMinX set to 0");} if(pFig->m_dMaxX<5){pFig->m_dMaxX=5; AfxMessageBox("pFig->m_dMaxX set to 5 ms");}}
	m_PreSpikeTimeWnd=(int)fabs(pFig->m_dMinX); m_PostSpikeTimeWnd=(int)fabs(pFig->m_dMaxX);
	int add1=0; 
	if(m_HalfKernel_ms!=(int)m_HalfKernel_ms)add1=1;//This is to compensate for the rounding down the integer at the equation <1>
	m_XcorSize_ofPreRefSpike =m_PreSpikeTimeWnd +m_HalfKernel_ms+add1; 
	m_XcorSize_ofPostRefSpike=m_PostSpikeTimeWnd+m_HalfKernel_ms+add1;
	m_FullSizeXCOR_ms=m_XcorSize_ofPreRefSpike+m_XcorSize_ofPostRefSpike; 
	m_XCOR.SetSize(m_FullSizeXCOR_ms+2); m_TmpXCOR.SetSize(m_FullSizeXCOR_ms+2);
	m_XcorSize_ofPreRefSpike_inMicS =m_XcorSize_ofPreRefSpike*1000;	
	m_XcorSize_ofPostRefSpike_inMicS=m_XcorSize_ofPostRefSpike*1000;	
	for(int i=0;i<m_FullSizeXCOR_ms;i++)m_XCOR[i]=0;  
	m_NumWeightings=1;
	m_RasterIdx=-1;
	m_NumOfDisplayedRasterRows=0;
}

//#define ONLY_FOR_SIMULATION
void CXCOR_View::XCOR_Main() 
{
	m_bIsON=1;
	if(m_Stop_Or_Pause==1){
		m_Stop_Or_Pause=0;
		RedrawAllContent(); //Since the display loop is not running.
	}
	CClientDC objDC(this);
	int i;
	CSingleLock ___SyncObj_Display(&m_sema); 


	//Request to wipe previous history, in the while() loop below.
	m_Ref_AndOr_SbjChnaged=1;
	m_UserReqest_ChangeXOR_Ker=1;

	//For simulation
	#ifdef ONLY_FOR_SIMULATION
	*M_Spike_QueueSize = MAX_SPIKEs;
	*M_isDAS_Running=1;
	#endif

	//Wait for DAS-server to start & initialize "DYNAMIC" variables.
	while(*M_isDAS_Running==0){Sleep(10); if(m_bIsON==0|| *M_Blip_OFF==1)goto EndProgram; }

	__int64	*timeStampMicS_1stNn=M_timeStampMicS[m_XCOR_RefChan], *timeStampMicS_2ndNn=M_timeStampMicS[m_XCOR_SbjChan];
	//Find a spike that is most recent to be usable. To be usable, the time between the spike and the current data (*M_usableSIGi) should be at least equal to m_PostSpikeTimeWnd. 
	m_XCOR_RefSpkIdx=-1; //Assume that there hasn't been any spike.
	float OneOverSamplesPerMs=1.0/((int)(0.001*DATA_SAMPLING_RATE));
	__int64	TimeSinceSpike;
	int	EarliestSpkIdx, SpkIdx, LatestSpkIdx, prvUsableSpikeIdx_1stNn, prvUsableSpikeIdx_2ndNn;
	__int64 *AIidx_of_1stNnSpk=AddressOf_SpkIndexOfNn(m_XCOR_RefChan,m_XCOR_RefNn), *AIidx_of_2ndNnSpk=AddressOf_SpkIndexOfNn(m_XCOR_SbjChan,m_XCOR_SbjNn);

	//For simulation
	#ifdef ONLY_FOR_SIMULATION
	*m_pUsableSpikeIdx_RefNn=5;
	for(i=0;i<=*m_pUsableSpikeIdx_RefNn;i++)m_pSpikeTimeStampOf_RefNn[i]=i*1000.;
	*m_pUsableSpikeIdx_SubNn=5;
	for(i=0;i<=*m_pUsableSpikeIdx_SubNn;i++)m_pSpikeTimeStampOf_RefNn[i]=i*1000.;
	m_XCOR_RefSpkIdx=0;
	#endif
	
	//#########################  The main loop ####################################################
	//#########################  The main loop ####################################################
	CFigure *pFig=&m_Fig[0][0][0];
	int DispIdx=0, nextDispIDx, DispIdxDist, p, RelTimeMs, bias, UsableSpikeIdx_RefNn, XCOR_Updated, NextRefIdx,xcorIdx;
	double InverseNumWeightings=1, ComplimentaryWeighting=1.-InverseNumWeightings; 
	__int64 SpikeTimeStampOf_RefNn, LastTimeDisplayedXcor=*M_LatestTimeStampInMicroS;
	m_DispEventIdx=0;
	m_isDisplayerRunning=1;
	while(1){
		//Do this only when it is NOT in Pause or Stop.
		if(m_Stop_Or_Pause==0){	

			#ifdef ONLY_FOR_SIMULATION
			*M_LatestTimeStampInMicroS=m_pSpikeTimeStampOf_RefNn[m_XCOR_RefSpkIdx]+m_XcorSize_ofPostRefSpike_inMicS;
			#endif

			//The user has requested to change the IDs of Reference and Subject Neurons
			if(m_Ref_AndOr_SbjChnaged==1){m_Ref_AndOr_SbjChnaged=0; Reset_Ref_Sbj_IDXes();}
			//The user has requested to change the scales of the graph
			if(m_UserReqest_ChangeXOR_Ker==1){m_UserReqest_ChangeXOR_Ker=0; Set_XCOR_range_and_Kernel();}

			UsableSpikeIdx_RefNn=*m_pUsableSpikeIdx_RefNn;
			if(m_XCOR_RefSpkIdx<0 && UsableSpikeIdx_RefNn>=0){//Reference spike happens for the first time.
				m_XCOR_RefSpkIdx=UsableSpikeIdx_RefNn;
			}
			if(m_XCOR_RefSpkIdx!=UsableSpikeIdx_RefNn){ //m_XCOR_RefSpkIdx is a DONE idx.
				XCOR_Updated=0;
				NextRefIdx=idxSpkQ(m_XCOR_RefSpkIdx+1);
				if(*M_LatestTimeStampInMicroS-m_pSpikeTimeStampOf_RefNn[NextRefIdx] >= m_XcorSize_ofPostRefSpike_inMicS){//See whether the current time is away from the NEXT Reference spike by m_XcorSize_ofPostRefSpike_inMicS.
					XCOR_Updated=1;
					m_XCOR_RefSpkIdx=NextRefIdx;
					m_RasterIdx++; if(m_RasterIdx>=HOW_MANY_TRLS_TO_DISP)m_RasterIdx=0;
					m_NumSpikesInRow[m_RasterIdx]=0; 
					m_NumOfDisplayedRasterRows++; //Inform the refreshing functions to draw the content, since there is something.
					SpikeTimeStampOf_RefNn=m_pSpikeTimeStampOf_RefNn[m_XCOR_RefSpkIdx];
					while(1){
						//##############################  Do XCOR  #######################################
						if(m_XCOR_SbjSpk_StartIdx<0)if(*m_pUsableSpikeIdx_SubNn>0)m_XCOR_SbjSpk_StartIdx=*m_pUsableSpikeIdx_SubNn;//Subject spike happens for the first time.
						//Find the indexes (m_XCOR_SbjSpk_StartIdx, m_XCOR_SbjSpk_EndIdx) of the subject spikes
						FindFirstIdxOfSbjSpike();			
						//Do the XCOR
						for(i=0;i<m_FullSizeXCOR_ms;i++)m_TmpXCOR[i]=0;  //This needs to be done even when there was no subject's spike.
						if(m_FoundStart==1){
							m_SbjSpkIdx=m_XCOR_SbjSpk_StartIdx;
							while(1){
								if(!(m_AutoCorrelation==1 && m_XCOR_RefSpkIdx==m_SbjSpkIdx)){//Skip the "self" part of the Autocorrelation.
									//Add the contribution of the subject spike on the XCOR
									RelTimeMs=m_XcorSize_ofPreRefSpike+(int)((m_pSpikeTimeStampOf_SubNn[m_SbjSpkIdx]-SpikeTimeStampOf_RefNn)*0.001);
									bias=RelTimeMs-m_HalfKernel_ms;//The Gaussian kernel adds its weight (KrVal[p]) from "RelTimeMs-m_HalfKernel_ms" to "RelTimeMs+m_HalfKernel_ms". <1>
									if(bias>=0 && bias<m_FullSizeXCOR_ms){//Removing this if-else condition will cause an error because some spikes would make the kernels go negative array index or over the maximimum of the given XCOR array.  
										for(p=0;p<m_FullKernel_ms;p++){
											m_TmpXCOR[bias+p]+=KrVal[p]; //KrVal[] has the Gaussian kernel vlaues. 
										}
									}
									else{//The left or right side of the kernel is out of the XCOR array. (see the X-correlation PPT file). Removing this if-else condition will cause an error because some spikes would make the kernels go negative array index or over the maximimum of the given XCOR array. 
										for(p=0;p<m_FullKernel_ms;p++){
											xcorIdx=bias+p; if(xcorIdx<m_HalfKernel_ms||xcorIdx>=m_FullSizeXCOR_ms-m_HalfKernel_ms)continue;//This line saves the effort to write the portion that would NOT be displayed. But since this portion is small compared the large dispalyed area, this if() line makes the efficiency go down.
											m_TmpXCOR[bias+p]+=KrVal[p]; //KrVal[] has the Gaussian kernel vlaues. 
										}
									}
									m_Raster[m_RasterIdx][m_NumSpikesInRow[m_RasterIdx]++]=RelTimeMs-m_XcorSize_ofPreRefSpike; if(m_NumSpikesInRow[m_RasterIdx]>=m_SizeOfRaster[m_RasterIdx]-1){m_Raster[m_RasterIdx].SetSize(2*m_SizeOfRaster[m_RasterIdx]); m_SizeOfRaster[m_RasterIdx]*=2;}
								}
								if(m_SbjSpkIdx==m_XCOR_SbjSpk_EndIdx)break;
								m_SbjSpkIdx=idxSpkQ(m_SbjSpkIdx+1);
							}
						}
						//This weighting needs to be done even when there was no subject's spike.
						InverseNumWeightings=1./m_NumWeightings; ComplimentaryWeighting=1.-InverseNumWeightings; 
						for(i=0;i<m_FullSizeXCOR_ms;i++)m_XCOR[i]=m_XCOR[i]*ComplimentaryWeighting +InverseNumWeightings*m_TmpXCOR[i];  
						m_NumWeightings++; 
						//###############################################################################

						//Find the NEXT Reference Spike.
						if(m_XCOR_RefSpkIdx==UsableSpikeIdx_RefNn)break;//m_XCOR_RefSpkIdx is a DONE idx.
						NextRefIdx=idxSpkQ(m_XCOR_RefSpkIdx+1);
						if(*M_LatestTimeStampInMicroS-m_pSpikeTimeStampOf_RefNn[NextRefIdx]< m_XcorSize_ofPostRefSpike_inMicS)break;
						m_XCOR_RefSpkIdx=NextRefIdx;
						m_RasterIdx++; if(m_RasterIdx>=HOW_MANY_TRLS_TO_DISP)m_RasterIdx=0;
						m_NumSpikesInRow[m_RasterIdx]=0;
						m_NumOfDisplayedRasterRows++; //Inform the refreshing functions to draw the content, since there is something.
						SpikeTimeStampOf_RefNn=m_pSpikeTimeStampOf_RefNn[m_XCOR_RefSpkIdx];
					}
				}
				//Display the XCOR result
				if(*M_LatestTimeStampInMicroS-LastTimeDisplayedXcor>33000 && XCOR_Updated==1){
					LastTimeDisplayedXcor=*M_LatestTimeStampInMicroS;
					//Draw raster & XCOR
					___SyncObj_Display.Lock();
					pFig->Frame_Graph2D(&m_DC[0],1, 0); 
					pFig->Disp_Raster(&m_DC[0]);
					pFig->Disp_XCOR(&m_XCOR[m_HalfKernel_ms], m_FullSizeXCOR_ms-m_FullKernel_ms, &m_DC[0]);
					pFig->RefreshFrame(&objDC, &m_DC[0]);
					___SyncObj_Display.Unlock();
				}
				else{
					Sleep(2);
				}
			}
		}

		//Pause or Stop
		if(m_Stop_Or_Pause==1)Sleep(5);
		else Sleep(1);

		//################  Display ############################
		if(DispIdx<=m_DispEventIdx)DispIdxDist=m_DispEventIdx-DispIdx;
		else DispIdxDist=MAX_DISP_REQUESTS-DispIdx+m_DispEventIdx;
		if(DispIdxDist>0){
			for(i=0;i<DispIdxDist; i++){
				if(DispIdx+1<MAX_DISP_REQUESTS)nextDispIDx=DispIdx+1; else nextDispIDx=0;
				if(i+1<DispIdxDist && m_DispEvents[DispIdx][0]==m_DispEvents[nextDispIDx][0]){
					if(m_DispEvents[DispIdx][1]>=1){//Number of arguments
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
				___SyncObj_Display.Lock();
				DisplayStuff(DispIdx,&objDC);
				___SyncObj_Display.Unlock();
				DispIdx=nextDispIDx;				
			}
		}
		//######################################################

		if(m_bIsON==0 || *M_Blip_OFF==1)break;
	}

	//#############################################################################
	//#############################################################################
EndProgram:;
	m_isDisplayerRunning=0;
	m_bIsON=0;
	m_Stop_Or_Pause=1;
}

int CXCOR_View::idxSpkQ(int i)
{
	if(i>=0 && i<((*M_Spike_QueueSize)))return i;
	else if(i>(*M_Spike_QueueSize)-1)return (i-(*M_Spike_QueueSize));
	else return ((*M_Spike_QueueSize)+i);//if(i<0) 
}

__int64* CXCOR_View::AddressOf_SpkIndexOfNn(int ChanID, int NnID) 
{
	if(NnID==0) return M_spike_IdxOfAIOfNn1[ChanID];
	else if(NnID==1) return M_spike_IdxOfAIOfNn2[ChanID];
	else if(NnID==2) return M_spike_IdxOfAIOfNn3[ChanID];
	else{ AfxMessageBox("Wrong NnID"); exit(0);}
}

void CXCOR_View::DisplayStuff(int RequestIdx, CDC *pObjDC)
{
	switch(m_DispEvents[RequestIdx][0]){
	case RESIZING_WIN:
		DispForResizing(pObjDC);
		break;
	//case RENAMING_FIG:
	//	for(rowP=0;rowP<m_dHowManyRows;rowP++){
	//		for(clmP=0;clmP<m_dHowManyClmns;clmP++){
	//			pFig=&m_Fig[rowP][clmP][0];
	//			pFig->DispNameOfFigure(&m_DC[0]);
	//			pFig->RefreshNameBox(pObjDC, &m_DC[0]);
	//	}	}
	//	RefreshFrame();
	//	break;	
	case RESCALING_AXES:
		DispForRescalingAxes(pObjDC);
		break;	
	//case DISP_FILE_NAME:
	//	DispFileName();
		break;	
	case REDRAW_ALL_CONTENT:
		RedrawAllContent();
		break;	
	case REDRAW_ONE_FIG_CONTENT:
		Draw_OneFig_andContent(m_DispEvents[RequestIdx][2], m_DispEvents[RequestIdx][3]);
		break;	
	case REFRESH_SCREEN:
		pObjDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
		break;	
	}
}	

void CXCOR_View::RedrawAllContent() 
{
	CClientDC objDC(this);
	Batch_DrawFigureFrames(1);
	DisplayTitle();
	//Hilight_AllCaptured_Figs();

	if(m_NumOfDisplayedRasterRows>0){
		CFigure *pFig=&m_Fig[0][0][0];
		pFig->Disp_Raster(&m_DC[0]);
		pFig->Disp_XCOR(&m_XCOR[m_HalfKernel_ms], m_FullSizeXCOR_ms-m_FullKernel_ms, &m_DC[0]);
	}
	m_DC[0].SetBkColor(RGB(220, 220, 220));
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5, m_JustFileName);
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
}

void CXCOR_View::Draw_OneFig_andContent(int row, int clm) 
{
	Draw_OneFig_andContent(&m_Fig[row][clm][0]) ;
}

void CXCOR_View::Draw_OneFig_andContent(CFigure *pFig) 
{
	if(pFig->m_Deleted==1)return; 
	pFig->Frame_Graph2D(&m_DC[0],1, !m_Stop_Or_Pause); 
	pFig->EmboseAndHighlightFrame(&m_DC[0], 1, !m_Stop_Or_Pause);
	CClientDC objDC(this);
	pFig->DispLabel(pFig->m_XCOR_Label,&objDC, &m_DC[0]);

	if(m_NumOfDisplayedRasterRows>0){
		pFig->Disp_Raster(&m_DC[0]);
		pFig->Disp_XCOR(&m_XCOR[m_HalfKernel_ms], m_FullSizeXCOR_ms-m_FullKernel_ms, &m_DC[0]);
	}
	pFig->RefreshFrame(&objDC, &m_DC[0]);
	m_DC[0].SetBkColor(RGB(220, 220, 220));
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5, m_JustFileName);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
}

void CXCOR_View::DispForResizing(CDC *pObjDC) 
{
	Re_measure_ClientView();
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	Batch_Prepare4FigFrame();
	Batch_DrawFigureFrames(0);
	Hilight_AllCaptured_Figs();
	DisplayTitle();
	m_bIsScrXYforIJ=0;
	pObjDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);

	if(m_NumOfDisplayedRasterRows>0){
		CClientDC objDC(this);
		CFigure *pFig=&m_Fig[0][0][0];
		pFig=&m_Fig[0][0][0];
		pFig->Disp_Raster(&objDC);
		pFig->Disp_XCOR(&m_XCOR[m_HalfKernel_ms], m_FullSizeXCOR_ms-m_FullKernel_ms, &objDC);
	}
}

void CXCOR_View::DispForRescalingAxes(CDC *pObjDC) 
{
	int rowP,clmP;
	CFigure *pFig;
	for(rowP=0;rowP<m_dHowManyRows;rowP++){
		for(clmP=0;clmP<m_dHowManyClmns;clmP++){
			pFig=&m_Fig[rowP][clmP][0];
			if(pFig->m_Captured!=1||pFig->m_Deleted==1)continue;
			pFig->Prepare4FigFrame(); 
			pFig->Frame_Graph2D(&m_DC[0],1, !m_Stop_Or_Pause); 
			Hilight_OneCapturedFig(pFig);
			pFig->RefreshFrame(pObjDC,&m_DC[0]); 
	}	}	
}

void CXCOR_View::RequestDisplayEvent(int Event) 
{
	RequestDisplayEvent(Event, 0, NULL,  NULL);
}
void CXCOR_View::RequestDisplayEvent(int Event,  int SomeIntValue) 
{
	RequestDisplayEvent(Event, 1, SomeIntValue,  NULL); 
}
void CXCOR_View::RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2) 
{
	RequestDisplayEvent(Event, 2, SomeIntValue1, SomeIntValue2);
}
void CXCOR_View::RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) 
{	
	m_DispEvents[m_DispEventIdx][0]=Event;
	m_DispEvents[m_DispEventIdx][1]=NumVariables;
	for(int i=0; i<NumVariables;i++){
		if(i==0)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue1; 
		else if(i==1)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue2; 
	}
	if(m_DispEventIdx+1<MAX_DISP_REQUESTS)m_DispEventIdx++; else m_DispEventIdx=0;
}

void CXCOR_View::Show2D_I() 
{
}
void CXCOR_View::Show2D_III() 
{	
}
void CXCOR_View::Show2D_IV() 
{
}

void CXCOR_View::Show2D_VI() 
{
}
void CXCOR_View::Show2D_VII() 
{
}

void CXCOR_View::Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) 
{
#define TICK_STEPX(pB) ((pB->m_dMaxX-pB->m_dMinX)/(pB->m_howManyXguideLines-1))
#define TICK_STEPY(pB) ((pB->m_dMaxY-pB->m_dMinY)/(pB->m_howManyYguideLines-1))
	int i;
	float TickStepX, TickStepY;

	pBrd->m_howManyXguideLines=HowManyTicsX; TickStepX=TICK_STEPX(pBrd);
	for(i=0;i<pBrd->m_howManyXguideLines;i++){pBrd->m_IsLabeledTicX[i]=1;pBrd->m_XTics[i]=pBrd->m_dMinX+i*TickStepX;} 
	pBrd->m_howManyYguideLines=HowManyTicsY; TickStepY=TICK_STEPY(pBrd);
	for(i=0;i<pBrd->m_howManyYguideLines;i++){pBrd->m_IsLabeledTicY[i]=1;pBrd->m_YTics[i]=pBrd->m_dMinY+i*TickStepY;} 
}

void CXCOR_View::Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) 
{
#define F2FY 5
#define F2FX 5
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyClmns;i++){
		for(j=0;j<m_dHowManyRows;j++){
			pFig=&m_Fig[j][i][0];
			pFig->m_normTx=scale*((i+1)*F2FX+    i*(sizeOfOneClmn-F2FX));  pFig->m_normTy=scale*((j+1)*F2FY+    j*(sizeOfOneRow-F2FY)+m_topOfFigs);   
			pFig->m_normBx=scale*((i+1)*F2FX+(i+1)*(sizeOfOneClmn-F2FX));  pFig->m_normBy=scale*((j+1)*F2FY+(j+1)*(sizeOfOneRow-F2FY)+m_topOfFigs); 
			pFig-> m_LateralMargin=20;
			pFig->SetFig_Position_in_ClientView(pFig->m_normTx,pFig->m_normTy,pFig->m_normBx,pFig->m_normBy);

			pFig->m_BlownUpTx=200; pFig->m_BlownUpTy=5; pFig->m_BlownUpBx=820; pFig->m_BlownUpBy=605; pFig->m_LateralMargin=20;
		}	
	}
}
void CXCOR_View::Batch_SetFontSize(CDC* pDC) 
{
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyClmns;i++){
		for(j=0;j<m_dHowManyRows;j++){
			pFig=&m_Fig[j][i][0];
			pFig->SetFontSize(pDC);
		}	
	}
}
void CXCOR_View::Batch_Prepare4FigFrame() 
{
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyClmns;i++){
		for(j=0;j<m_dHowManyRows;j++){
			pFig=&m_Fig[j][i][0];
			pFig->Prepare4FigFrame();
		}	
	}
}

void CXCOR_View::Batch_DrawFigureFrames(int isToUnveil) 
{	
	CClientDC objDC(this);
	m_DC[0].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY); 
	m_DoNotErase=1;//This is to inform the Labeling part not to erase previous site, which may not correspond to new position due to resize.
	int i,j;
	CFigure *pFig;
	for(i=0;i<m_dHowManyClmns;i++){
		for(j=0;j<m_dHowManyRows;j++){
			pFig=&m_Fig[j][i][0];
			if(pFig->m_Deleted==0)pFig->Frame_Graph2D(&m_DC[0],1, !m_Stop_Or_Pause); 
		}
	}
	m_DC[0].SetBkColor(RGB(220, 220, 220));
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5, m_JustFileName);
	if(isToUnveil)objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
}


// Writes a string sText with a size which fits perfectly in a rectangle rFont
// Font already attached with device context pDC is scaled
// This does not work for default font of the device contex
void CXCOR_View::FontInRect(CDC *pDC, CString sText, CRect &rFont)
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

void CXCOR_View::OnChangeParameters() 
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


void CXCOR_View::OnIsToDisplayMarks()
{
}

void CXCOR_View::OnUpdateIsToDisplayMarks(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsToDisplayMarks==1);
}


//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
int CXCOR_View::Find_Last_Num_inString(char *GivenString, int &NumToReturn)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0, sign=0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			if(*p=='-')sign=1; else sign=0;
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return -1;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	return 0;
}


void CXCOR_View::Show2D_VIII() 
{
}
