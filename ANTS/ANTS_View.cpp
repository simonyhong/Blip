// AntsView.cpp : implementation of the CAntsView class


#include "stdafx.h"
#include "ANTS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ANTS_Doc.h"
#include "SharedConstants.h"
#include "Channel.h"
#include "Figure.h"
#include "ANTS_View.h"
#include "SinTable.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CAntsView::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CAntsView

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CAntsView, CView)

BEGIN_MESSAGE_MAP(CAntsView, CView)
	//{{AFX_MSG_MAP(CAntsView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_AutoStim, OnAutoStim)
	ON_COMMAND(ID_VTBOX_MODE, On_VTBOX_mode)
	ON_COMMAND(ID_ThresholdMode, OnThresholdMode)
	ON_UPDATE_COMMAND_UI(ID_AutoStim, OnUpdateAutoStim)
	//ON_UPDATE_COMMAND_UI(ID_ThresholdMode, OnUpdateThresholdMode)
	//ON_UPDATE_COMMAND_UI(ID_VTBOX_MODE, OnUpdate_VTBOX_mode)
	ON_COMMAND(ID_2DGraph_IV, On2DGraph_IV)
	ON_COMMAND(ID_2DGraph_V, On2DGraph_V)
	ON_COMMAND(ID_2DGraph_VI, On2DGraph_VI)
	ON_COMMAND(ID_2DGraph_VII, On2DGraph_VII)
	ON_COMMAND(ID_2DGraph_VIII, On2DGraph_VIII)
	ON_COMMAND(ID_Parameters, OnChangeParameters)
	ON_COMMAND(ID_Functions, OnFunctions)
	ON_COMMAND(ID_PROCEED, OnProceed)
	ON_COMMAND(ID_SPEED_UP, OnSpeedUp)
	ON_COMMAND(ID_SLOW_DOWN, OnSlowDown)
	ON_COMMAND(Quick_Function, OnQuickFunction)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_ORTHODROMIC_STIMULATION, &CAntsView::OnIs_OrthodromicStimulation)
	ON_UPDATE_COMMAND_UI(ID_ORTHODROMIC_STIMULATION, &CAntsView::OnUpdateIsOrthodromicStimulation)
	ON_COMMAND(ID_ANTIDROMIC_STIMULATION, &CAntsView::OnIs_AntidromicStimulation)
	ON_UPDATE_COMMAND_UI(ID_ANTIDROMIC_STIMULATION, &CAntsView::OnUpdateIsToAntidromicallyStimulate)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_SCALE_DELETE_FIGS, &CAntsView::OnDeleteFigs)
	ON_COMMAND(ID_SCALE_DELETE_THE_REST, &CAntsView::OnDelete_the_Rest_Figs)
	ON_COMMAND(ID_FILE_SAVE, &CAntsView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CAntsView::OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT_STRING1, &CAntsView::OnEnChangeEditString1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_FILE_SAVE_AS, &CAntsView::OnConfigFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CAntsView::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_CLOSE, &CAntsView::OnFileClose)
	ON_COMMAND(ID_PROPERTIES, &CAntsView::OnProperties)
	ON_COMMAND(ID_FASTER_STM, &CAntsView::OnFasterStm)
	ON_UPDATE_COMMAND_UI(ID_FASTER_STM, &CAntsView::OnUpdateFasterStm)
	ON_COMMAND(ID_FILE_CONTINUOUS_RECORDING_MODE, &CAntsView::OnFileContinuousRecordingMode)
	ON_UPDATE_COMMAND_UI(ID_FILE_CONTINUOUS_RECORDING_MODE, &CAntsView::OnUpdateFileContinuousRecordingMode)
	ON_COMMAND(ID_TOOLS_SET_PRE_POST_PERIODS_TO_SAVE, &CAntsView::OnSetPrePostPeriods_of_OrthodromicStimulationLFP_ToSave)
//	ON_UPDATE_COMMAND_UI(ID_TOOLS_SET_PRE_POST_PERIODS_TO_SAVE, &CAntsView::OnSetPrePostPeriods_of_OrthodromicStimulationLFP_ToSave)
ON_COMMAND(ID_SAVE_LFP, &CAntsView::OnSaveLFP)
ON_UPDATE_COMMAND_UI(ID_SAVE_LFP, &CAntsView::OnUpdateSaveLFP)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CAntsView construction/destruction
CAntsView::CAntsView()
{
	// TODO: add construction code here
	m_FlagVirMemDC=0;
	m_nColorShow=BLACK_BRUSH;
	m_bShow2D=0;  
	m_bFistTime=1; m_bFistTime2=1;
	m_nKey=0;
	m_nSpeed=0;
	m_FlagVirMemDC=0;
	m_bProceed=0; m_IsFasterStm=0;
	m_bIsTo_ANTI_Stimulate=0;
	m_isOrthodromicStimulation=0;
	m_bIsON=0;
	m_dScaleUpDn=1;
	int i, j,row, clm, lyr;
	m_dHowManyLayers=MaxFigLyr;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		m_dHowManyRows[lyr]=MaxFigRow; m_dHowManyClmns[lyr]=MaxFigCln; 
	}
	m_BlownUpPaneID=-1;
	m_DispEventIdx=0;

	sprintf(m_PreFix,"");sprintf(m_SuFFix,"AD");sprintf(m_ActivationOrCollision,"act");
	m_prvDate="";
	CFigure *pFig;
	char StimName[32], ChanNames[64];
	for(lyr=0;lyr<MaxFigLyr;lyr++){
		for(row=0;row<MaxFigRow;row++){
			for(clm=0;clm<MaxFigCln;clm++){
				pFig=&m_Fig[row][clm][lyr];
				pFig->m_FigID=lyr*MaxFigRow*MaxFigCln+row*MaxFigCln+clm;
				pFig->m_row=row;
				pFig->m_clm=clm;
				pFig->m_lyr=lyr;
				pFig->m_pDC=&m_DC[lyr];
				//Give default channel IDs
				pFig->m_RecordingChan.m_ChanID=clm*MaxFigRow+row;
				pFig->m_StimGatingRelay.m_ChanID=lyr+DO_GATING_CHAN1;
				sprintf(ChanNames,"RecCh:%d",pFig->m_RecordingChan.m_ChanID);
				sprintf(StimName,"StmRly:%d",pFig->m_StimGatingRelay.m_ChanID);
				strcat(ChanNames, StimName);
				pFig->m_RecordingChan.NameString=ChanNames;   //pFig->m_RecordingChan.NameString stores BOTH the Stimulation & Recording names
				pFig->m_StimGatingRelay.NameString=StimName;  //pFig->m_StimGatingRelay.NameString is not used
			}
		}
	}
	m_HeldLineIdx=-1;
	m_StimulationLatencyBias_InMicroS=250;
	////////////////////////////////////
	m_NULL_pen.CreatePen(PS_NULL, 1,RGB(0,0,0));
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

	for(i=0;i<MaxPen;i++){
		j=(MaxRGB100-1)-i*(MaxRGB100/MaxPen);
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;
	LogBrh.lbColor=RGB(200,200,200);
	m_GrayBrush.CreateBrushIndirect( &LogBrh );
	LogBrh.lbColor=RGB(255,0,0);
	m_RedBrush.CreateBrushIndirect( &LogBrh );

	/////////////// Parameters For Applications
	m_bIsScrXYforIJ=0;
	m_GlobalTitle="Antidromic Stimulation";
	m_NewlyLoaded=0; m_DoNotErase=0;
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	m_SelectedItemID=-1; m_numCaptured=0; m_is_LFP_saving=0;
	m_numRecording_Stim_PairsToSave=0;
	
	m_FirstTime0ElementErr=1;
	m_LFP_Start_Xms_toSave=500; m_LFP_End_Xms_toSave=500;
	m_DisplayedStimLayer=0;
	m_Lable_Of_SuperimposeNumber.NameString="Superimpose#: 6"; m_numberOfSuperimposeLines=6;
	m_Lable_Of_TriggerChanNumber.NameString="TrigChan: 0 "; 

	//In some rare cases, in which the user wants to save several LFPs in a continuous mode from several recording channles, the computer could not keep up with the recording: the hard drive was not fast enough to handle this situation. 
	//It may be possible to make the saving process as a thread to run independently from the manin ANTS (TAS does this), but that would be a lot of work.
	m_IsContinuousRecordingMode=0; 
	InitializeCriticalSection(&m_CriticalSection4Save);

	if(m_IsContinuousRecordingMode==1)m_FirstRecordingInSequence=1; else m_FirstRecordingInSequence=0; 
	m_is_firstTimeOrthoStimulation_IntervalSetting=1;
}

CAntsView::~CAntsView() 
{
	m_bIsON=0;
	*M_IsAntidromicStim=0;
	*M_IsOrthoStim=0;
	*M_IsToLearnArtifact=0;
	Relay_OFF(M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]);//Turn off the gating signals.
	*M_isANTS_Running=0;

	SetEvent(m_EvStopProcess);//In case the thread uses a stop event (m_EvStopProcess)

	// Wait for thread to stop, in case the thread uses a thread handle, m_hThread.
	if(m_hThread!=NULL){
		Relay_OFF(M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]);//Turn off the gating signals.
	    WaitForSingleObject(m_hThread,1000);
	    CloseHandle(m_hThread);
	}
	CloseHandle(m_EvStopProcess);

	*M_IsAntidromicStim=0;
	*M_isANTS_Running=0;
	CloseSharedMemoryArea();
	
	DeleteCriticalSection(&m_CriticalSection4Save);

	int i;
	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	m_GrayBrush.DeleteObject();
	m_RedBrush.DeleteObject();
	m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen;i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); m_rgbPEN3[i].DeleteObject();
	}
	m_NULL_pen.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject(); m_GREEN.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();
	m_VIOLET.DeleteObject();m_ORANGE2.DeleteObject(); m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_BLACK.DeleteObject();m_BLACK2.DeleteObject();m_BLACK3.DeleteObject();m_bkPen.DeleteObject();m_GRAY2.DeleteObject();
	m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	m_font.DeleteObject();
}

BOOL CAntsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAntsView drawing
DWORD WINAPI Graph2D_IV(LPVOID WinObjPtr) //A global Function
{
	CAntsView* pView=(CAntsView*) WinObjPtr;
	pView->ANTS_main() ;
	if(*(pView->M_Blip_OFF)==1)AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);

	return 0;
}

#define SIZE_OF_BITMAP_X  1.//0.5
#define SIZE_OF_BITMAP_Y  0.5
void CAntsView::OnDraw(CDC* pDC)
{
#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CAntsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1 && m_bFistTime2==1){
		m_bFistTime2=0;
		VirMemDC();
		Re_measure_ClientView(); //Get a preliminary measurement
		m_dHowManyClmns[0]=-1;
		OnFileOpen();//Try to open a default root file.
		Recalibrate_Panes_and_MeasuringLineRectPositions();
		m_EvStopProcess  	  =CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,NULL/*use name?*/);
		m_hThread=CreateThread(NULL,0,Graph2D_IV,this,0,NULL);
		if(m_dHowManyClmns[0]!=-1)return;//Opening a default root file has been successful.
		else{//Failed to open a default root file => implement the programs own version.
			sprintf(m_FileNameFULL,"");
			sprintf(m_ConfigJustFileName,"NO root file");
			m_dHowManyClmns[0]=MaxFigCln; 
		}
		Start_a_NEW_set_of_figures();
	}
	else{
		CRect rt;
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			if(m_savedClientRect.right<rt.right ||m_savedClientRect.bottom<rt.bottom){
				if(m_savedClientRect.right>=SIZE_OF_BITMAP_X*m_nScrMaxX ||m_savedClientRect.bottom>=SIZE_OF_BITMAP_Y*m_nScrMaxY){
					int Y=SIZE_OF_BITMAP_Y*m_nScrMaxY;
					char ScreenMsg[1024]; sprintf(ScreenMsg,"Simon: To save Bitmap memory, the size of the window is currently limited to Horizontal:%d, Vertical:%d\n To change this setting, adjust SIZE_OF_BITMAP_X and SIZE_OF_BITMAP_Y in the program.",(int)(SIZE_OF_BITMAP_X*m_nScrMaxX),Y);
					AfxMessageBox(ScreenMsg);
				}
			}
			RequestDisplayEvent(REDRAW_ALL_LAYERS);
		}
		else{
			RequestDisplayEvent(UNVEIL_CURRENT_PAGE);
		}
	}
}

void CAntsView::Start_a_NEW_set_of_figures()
{			
	CFigure *pFig;
	int row, clm,Lyr;
	for(Lyr=0;Lyr<m_dHowManyLayers;Lyr++){
		for(row=0;row<m_dHowManyRows[Lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[Lyr];clm++){
				pFig=&m_Fig[row][clm][Lyr];
				pFig->m_Deleted=0;
			}
		}
	}
	Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();

	Re_measure_ClientView();//This one calculates "m_dSizeOfOneRow, m_dSizeOfOneClmn"
	Meassure_SuperimposeNumber();
	Meassure_TriggerChanNumber();
	Batch_SetFontSize(&m_DC[m_DisplayedStimLayer]);
	Batch_FigPosition_Settings(m_dScaleUpDn);
	Batch_Prepare4FigFrame();
	Batch_DrawFigureFrames(DONT_DISPLAY_NOW);
	#ifdef ZAPPER
	Disp_ZapStatus();
	Disp_LearnedLevel(-1);
	#endif
	InitMeasuringLineParameters_and_DrawLines();
	m_numCaptured=0;
	RequestDisplayEvent(REDRAW_ALL_LAYERS);
}

void CAntsView::Re_measure_ClientView()
{			
	CRect rt;
	GetClientRect(&rt);
	if(m_bFistTime==1){
		CFont* pFont = m_DC[m_DisplayedStimLayer].GetCurrentFont();
		LOGFONT LogFont;
		pFont->GetLogFont(&LogFont);
		m_fontHeight=LogFont.lfHeight;
	}
	m_TitleArea.top=rt.top;	m_TitleArea.bottom=rt.top+m_fontHeight; m_topOfFigs=m_TitleArea.bottom+2*m_fontHeight;
	m_TitleArea.left=rt.left; m_TitleArea.right=rt.right;
	for(int Lyr=0;Lyr<m_dHowManyLayers;Lyr++){
		m_dSizeOfOneRow[Lyr] =Divide((rt.bottom-m_topOfFigs-ViewMargin4FigsAlongEdges),m_dHowManyRows[Lyr]);  if(m_dSizeOfOneRow[Lyr] <=0)m_dSizeOfOneRow[Lyr]=0;
		m_dSizeOfOneClmn[Lyr]=Divide((rt.right-rt.left-ViewMargin4FigsAlongEdges),m_dHowManyClmns[Lyr]); if(m_dSizeOfOneClmn[Lyr]<=0)m_dSizeOfOneClmn[Lyr]=0;
	}
	m_savedClientRect=rt;m_savedClientRect.top=m_topOfFigs;

	Meassure_SuperimposeNumber();
	Meassure_TriggerChanNumber();
}


/////////////////////////////////////////////////////////////////////////////
// CAntsView printing

BOOL CAntsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAntsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAntsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAntsView diagnostics

#ifdef _DEBUG
void CAntsView::AssertValid() const
{
	CView::AssertValid();
}

void CAntsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAntsDoc* CAntsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAntsDoc)));
	return (CAntsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAntsView message handlers

UINT Graph2D_V(LPVOID WinObjPtr) //A global Function
{
	CAntsView* pView=(CAntsView*) WinObjPtr;
	pView->Show2D_V() ;
	return 0;
}
UINT Graph2D_VI(LPVOID WinObjPtr) //A global Function
{
	CAntsView* pView=(CAntsView*) WinObjPtr;
	pView->Show2D_VI() ;
	return 0;
}
UINT Graph2D_VII(LPVOID WinObjPtr) //A global Function
{
	CAntsView* pView=(CAntsView*) WinObjPtr;
	pView->Show2D_VII() ;
	return 0;
}
UINT Graph2D_VIII(LPVOID WinObjPtr) //A global Function
{
	CAntsView* pView=(CAntsView*) WinObjPtr;
	pView->Show2D_VIII() ;
	return 0;
}


void CAntsView::On2DGraph_IV() 
{
		//AfxBeginThread( Graph2D_IV, this);
}
void CAntsView::On2DGraph_V() 
{
		AfxBeginThread( Graph2D_V, this);
}
void CAntsView::On2DGraph_VI() 
{
		AfxBeginThread( Graph2D_VI, this);
}
void CAntsView::On2DGraph_VII() 
{
		AfxBeginThread( Graph2D_VII, this);
}
void CAntsView::On2DGraph_VIII() 
{
		AfxBeginThread( Graph2D_VIII, this);
}

void CAntsView::VirMemDC()
{
	//########  Preparing Virtual Window  ##########
	if(m_FlagVirMemDC==1){
		return;
	}
	m_FlagVirMemDC=1;
////////////////////////////////
   LOGFONT lf;                        // Used to create the CFont.
   memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
   lf.lfWidth = 6;                   // Width of font
   lf.lfHeight = 12;                  // Request a 20-pixel-high font
   strcpy(lf.lfFaceName, "Times New Roman");    //    with face name "Arial".
   m_FontWidth=lf.lfWidth; 
   m_FontHeight=lf.lfHeight; 
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
		m_bmp[i].CreateCompatibleBitmap(&aDC, SIZE_OF_BITMAP_X*m_nScrMaxX, SIZE_OF_BITMAP_Y*m_nScrMaxY);
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_bkBrush);
		m_DC[i].SelectObject(&m_font);
		m_DC[i].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
	}
	m_SavedDC=m_DC[m_DisplayedStimLayer].SaveDC();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################  2D Graphics #######################################
void CAntsView::InitMeasuringLineParameters_and_DrawLines() 
{  
	int i,row,clm,Lyr;
	CFigure *pFig;
	for(Lyr=0;Lyr<m_dHowManyLayers;Lyr++){
		for(row=0;row<m_dHowManyRows[Lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[Lyr];clm++){
				pFig=&m_Fig[row][clm][Lyr];
				if(pFig->m_Deleted==1)continue;
				for(i=0;i<NUM_MEASURING_LINES;i++){
					pFig->m_MeasureingLine_ScrnCoord[i]=pFig->FromDataToScreenX((double)pFig->m_MeasuringLineTime[i]);
				}
			}	
		}
	}
	//This one uses m_MeasureingLine_ScrnCoord[] calculated above to decide the time-rectangles for the measuring lines
	Recalibrate_Panes_and_MeasuringLineRectPositions();//The screen positions of the left, right, up, down edges of all panes need to be assigned.
	
	//This one uses m_MeasureingLine_ScrnCoord[] and the time rectangles to draw them.
	for(Lyr=0;Lyr<m_dHowManyLayers;Lyr++){
		for(row=0;row<m_dHowManyRows[Lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[Lyr];clm++){
				pFig=&m_Fig[row][clm][Lyr];			
				if(pFig->m_Deleted==1)continue;
				DrawAllMeasuringLines_NoDisplayNow(pFig);
			}
		}
	}
}

void CAntsView::Init_Stim_and_Recording_Channles() 
{  
	//Initially, assume there is no channel to stimulate
	*M_NumOfStimulationChannels=0; *M_CurrIdx_Of_StimGateRelayID=0;
	for(int i=0; i<MAX_NUM_STIM_CHANNELS; i++){
		M_StimDigitalOutChannels[i]=-1; M_InterStimulationInterval[i]=1000;
	}
	//Read in valid stimulation parameters
	int ChanCount=0;
	for(int i=0; i<MAX_NUM_STIM_CHANNELS; i++){
		if(m_StimulationChannels[i]>=0 && m_PostStimPeriod[i]>=MIMIMUM_POST_ANTI_DELAY){M_StimDigitalOutChannels[i]=m_StimulationChannels[i]; M_InterStimulationInterval[i]=m_PostStimPeriod[i]; ChanCount++;}	else {break;}
	}
	*M_NumOfStimulationChannels=ChanCount;

	int row,clm,lyr, StimChanDOIdx;
	CFigure *pFig;
	//Calculate the detaction-to-stimulation latency using the measuring line of the figure.
	//Calculate the sampling number after the stimulation 
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				StimChanDOIdx=GetIndexOfSimChannel(pFig->m_StimGatingRelay.m_ChanID);
				if(StimChanDOIdx!=-1){
					M_TriggerToStimulationLatency[StimChanDOIdx]=(int)(1000*fabs(pFig->m_MeasuringLineTime[0]));
					AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(StimChanDOIdx);//How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
				}
			}
		}
	}
	Meassure_SuperimposeNumber();
	Meassure_TriggerChanNumber();
	TriggerChannelName_Register(m_Lable_Of_TriggerChanNumber.m_ChanID);// m_Lable_Of_TriggerChanNumber.m_ChanID is taken from the Default_Config.txt.
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		if(lyr==m_DisplayedStimLayer)Draw_TriggerChanNumber(lyr,DISPLAY_NOW);
		else                         Draw_TriggerChanNumber(lyr,DONT_DISPLAY_NOW);   
	}


	//Clear Focused Channels
	for(int i=0;i<*M_NumOfStimulationChannels;i++)M_isFocusedChannel[i]=0;  *M_NumOfFocusChannels=0;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				pFig->m_Focus.NameString="O ";
			}
		}
	}
}

UINT Save_Antidromic_Trial(LPVOID Ptr);
UINT Save_Orthodromic_Trial(LPVOID Ptr);

void CAntsView::ANTS_main() 
{  	
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	CClientDC objDC(this);
	Re_measure_ClientView();
	Batch_FigPosition_Settings(m_dScaleUpDn);
	//Batch_SetFontSize(&m_DC[m_DisplayedStimLayer]);//This is not needed because the initial Config file has this.
	Batch_Prepare4FigFrame();
	m_numCaptured=0;

	CreateSharedMemoryArea();
	int i,j,k;
	//Zapper
	#ifdef ZAPPER
	Disp_ZapStatus();
	Zapper_MakeListOfGatingRelayIDs(); //Inform the software Zappers in DAS_sv, the current status directed by the Config file.
	//At the begining, assume that the zapper has not learned anything yet.
	double *pArtifact;
	for(i=0; i<NUM_NN_AI_CHAN; i++){
		for(j=0; j<MAX_NUM_STIM_CHANNELS; j++){
			M_Zapper[i].M_LearnedLevel[j]=0; 
			pArtifact=M_Zapper[i].M_Artifact[j];
			for(k=0; k<ARTIFACT_SIZE; k++){pArtifact[k]=0;}
		}
	} 
	Disp_LearnedLevel(0);
	#endif

	Make_TheBaseOfFileName(*M_is_LFP_saving);//This to display the file name that will be used to store the neural data.
	Batch_DrawFigureFrames(DONT_DISPLAY_NOW);
	BatchDraw_LayerContents(DONT_DISPLAY_NOW); //This one uses Shared Memories.
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
	Init_Stim_and_Recording_Channles();
	//This one is done not at the point of figure file loading but after the Shared Memory stuff.
	Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();//Inspect the figures and register proper Stimulation Channels to M_StimDigitalOutChannels[].

	//###  Some "STATIC" (just fixed-addresses) initialization of shared variables.
	m_bFistTime=0; //Now declear the initiation of the program is over
	*M_IsAntidromicStim=0;
	pM_AIQueueSize=&M_AIQueueSize[*M_AntidromicTriggeringChanID];
	*M_is_LFP_saving=0; 	
	*M_StimulationLatencyBias_InMicroS=m_StimulationLatencyBias_InMicroS;

	//ANTS uses 
	// *M_HeyANTS_StimGivenToGatingDIO_X, *M_NnNumberToRecord
	// to see if it needs to take an action.
	//Because of this, ANTS is safe to run even before the initialization of DYNAMIC variables by the DAS-server.
	//So do not need to wait for DAS-server to initialize the DYNAMIC variables.

	if(M_numMatchingKinds[*M_AntidromicTriggeringChanID]>0)*M_WhichAntidromicMode=VTBOX_STIM_MODE; 
	else *M_WhichAntidromicMode=THRESHOLD_STIM_MODE;

	char value[256];
	int DispIdx=0, DispIdxDist, nextDispIDx, stimChan; 
	m_DispEventIdx=0;
	*M_HeyANTS_StimGivenToGatingDIO_X=-1;//Clear any prior message from DAS
	m_OrthoStimIdx=-1;
	*M_isANTS_Running=1;
	char EvName[64], firstMSG=1; 	
	for(int i=0; i<MAX_DO_NUM; i++){
		sprintf(EvName,EventObjectsName,i);
		m_M_DO_Active[i] =OpenEvent(EVENT_ALL_ACCESS,TRUE,EvName);
		if(firstMSG==1 && m_M_DO_Active[i]==NULL){firstMSG=0; AfxMessageBox("m_M_DO_Active[i]==NULL");}
	}

	//##########  Display (& save) Antidromic or Orthodromic stimulation #############
	if(*M_NumOfStimulationChannels>0){
		*M_RememberedTurnedOnStimRelayID=M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]; //*M_RememberedTurnedOnStimRelayID is used to turn off later even when the user has changed the configuration.
		Relay_ON(*M_RememberedTurnedOnStimRelayID);//Initially, turn on the gating signal of the first channel. This is done here, rather than in DAS_sv because DAS_sv does not know if Antidromic stimulation will be used or not.
	}
	while(1){
		//###########  Stimulation in progress
		if(*M_HeyANTS_StimGivenToGatingDIO_X>=0){
			stimChan=*M_HeyANTS_StimGivenToGatingDIO_X;
			*M_HeyANTS_StimGivenToGatingDIO_X=-1;
			//##########  Save LFP
			if(*M_is_LFP_saving==1){
				//Antidromic display
				if(*M_IsAntidromicStim==1){
					AfxBeginThread( Save_Antidromic_Trial, this);
					sprintf(value,"(%d)",m_numOfSavedTrials);
					objDC.TextOut((m_FileName.NameString.GetLength()+5)*m_FontWidth, m_TitleArea.top,value);
				}
				//Orthodromic display
				if(m_isOrthodromicStimulation==1){
					AfxBeginThread( Save_Orthodromic_Trial, this);
					sprintf(value,"(%d)",m_numOfSavedTrials);
					objDC.TextOut((m_FileName.NameString.GetLength()+5)*m_FontWidth, m_TitleArea.top,value);
				}
			}
			//#########  Display 
			RequestDisplayEvent(DISP_LFP_AROUND_STIM, stimChan);
		}

		//m_EvEverythingElse set
		if(*M_NnNumberToRecord>0 && m_PrvNnNumberToRecord!=*M_NnNumberToRecord){Make_TheBaseOfFileName(*M_is_LFP_saving);}

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
				//SyncObj_Display.Lock();
				DisplayStuff(DispIdx,&objDC);	DispIdx=nextDispIDx;				
				//SyncObj_Display.Unlock();
			}
		}

		//###########   Trigger mode change
		if(*M_HeyANTS_SomeChanBoxInfoChanged==1){ //m_EvTriggerModeChange set
			*M_HeyANTS_SomeChanBoxInfoChanged=0;
			if(M_numMatchingKinds[*M_AntidromicTriggeringChanID]>0){
				if(*M_WhichAntidromicMode==THRESHOLD_STIM_MODE)*M_WhichAntidromicMode=VTBOX_STIM_MODE;
			}
			else{
				if(*M_WhichAntidromicMode==VTBOX_STIM_MODE)*M_WhichAntidromicMode=THRESHOLD_STIM_MODE;
			}
			RequestDisplayEvent(MINIMUM_REQ_TIME_TO_TRIGGER);
		}

		//###########   Sleep
		if(m_bIsON==0 || *M_Blip_OFF==1)break;
		for(i=0; i<10; i++){
			if(*M_HeyANTS_StimGivenToGatingDIO_X>=0)break;
			Sleep(1);
		}
	}

	//###########   Wrap up!!
	m_bIsON=0;
	*M_IsAntidromicStim=0;
	*M_IsOrthoStim=0;
	*M_IsToLearnArtifact=0;
	Relay_OFF(M_StimDigitalOutChannels[ *M_CurrIdx_Of_StimGateRelayID]);//Turn off the gating signals.
	*M_isANTS_Running=0;
}

//This returns a shortest length (in absolute value) between direct (within the end points of the queue) or detour (across the end points of the queue) lengths.
//The value is positive if B is bigger than A
//In a normal case, the sign of the outcome shows if the idx A needs to wait for B to catch up or B is already ahead.
int CAntsView::ShortestDistanceFrom_A_to_B(int A, int B, int SizeOfLoopedQueue) 
{
	int DirectDistance, DetourDistance;

	DirectDistance=B-A;
	if(A>B)DetourDistance=SizeOfLoopedQueue-A+B; else DetourDistance=-(SizeOfLoopedQueue-B+A); 

	if(abs(DirectDistance)<=abs(DetourDistance))return DirectDistance;
	else return DetourDistance;
}


void CAntsView::Minimum_RequiredTime_toTrigger(int layer, int IstoDisplayNow)
{
	char strTemp[256];
	sprintf(strTemp,"Shortest trigger time: %0.2lf+ %0.2lf  ", 0.001*(M_MiniRequiredLengthOfwaveForExamination[*M_AntidromicTriggeringChanID])*(*M_microSecPerPoint),0.001*(*M_SamplingEpoch_us)); 
	m_DC[layer].SetBkColor(RGB(220,220,220));
	m_DC[layer].TextOut(0.8*(m_TitleArea.right),m_TitleArea.top+m_FontHeight*.2,strTemp);
	if(IstoDisplayNow==1){
		CClientDC objDC(this);
		objDC.BitBlt(BIT_BLT(m_TitleArea,&m_DC[layer]));
	}
	m_DC[layer].SetBkColor(RGB(255, 255, 255));
}

void CAntsView::Draw_Disp_Tabs_and_DispStimLayer(int StimLayer, int toDisplayTabs, int toDisplayLayer)
{
	if(StimLayer>=MAX_NUM_STIM_CHANNELS || StimLayer<0)return;
	int layer;

	//Draw Tabs
	for(layer=0;layer<MAX_NUM_STIM_CHANNELS;layer++){
		if(layer==StimLayer){
			m_DC[StimLayer].SetBkColor(RGB(100,200,250));
		}
		else{
			m_DC[StimLayer].SetBkColor(RGB(70,120,190));
		}
		m_DC[StimLayer].TextOut(m_LayerTab[layer].StringRect.left, m_LayerTab[layer].StringRect.top, m_LayerTab[layer].NameString);
	}

	//Display the stimulation layer
	if(toDisplayLayer==1){
		CClientDC objDC(this);
		objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
	}
	//Display the tabs.
	else if(toDisplayTabs==1){
		CClientDC objDC(this);
		int Width=m_LayerTab[MAX_NUM_STIM_CHANNELS-1].StringRect.right-m_LayerTab[0].StringRect.left;
		int Height=m_LayerTab[0].StringRect.bottom-m_LayerTab[0].StringRect.top;
		objDC.BitBlt(m_LayerTab[0].StringRect.left,m_LayerTab[0].StringRect.top,Width,Height,&m_DC[StimLayer],m_LayerTab[0].StringRect.left,m_LayerTab[0].StringRect.top,SRCCOPY);
	}
	
	//Restore the default color
	m_DC[StimLayer].SetBkColor(RGB(255,255,255));
}

int CAntsView::GetIndexOfSimChannel(int StimDO) 
{
	int i;
	for(i=0;i<*M_NumOfStimulationChannels;i++){
		if(StimDO==M_StimDigitalOutChannels[i]){
			return i;
		}
	}
	return -1;
}

void CAntsView::Relay_ON(int whichBIT)
{
	if(whichBIT>=MAX_DO_NUM){AfxMessageBox("whichBIT>=MAX_DO_NUM"); return;}
	M_DO_Array[whichBIT]=RELAY_ON; //0 means on because NI acts as a voltage sink for the relay (70-ODC5 Grayhill)
	if(!SetEvent(m_M_DO_Active[whichBIT])){
		AfxMessageBox("SetEvent() failed");
	}
}

void CAntsView::Relay_OFF(int whichBIT)
{
	if(whichBIT>=MAX_DO_NUM){AfxMessageBox("whichBIT>=MAX_DO_NUM"); return;}
	M_DO_Array[whichBIT]=RELAY_OFF; //1 means off because NI acts as a voltage sink for the relay (70-ODC5 Grayhill)
	if(!SetEvent(m_M_DO_Active[whichBIT])){
		AfxMessageBox("SetEvent() failed");
	}
}

//This calculates the time delay (in samplings) after an antidromic stimulation trigger. To be more exact, M_TriggerToStimulationLatency[] has been added which plays a tiny role (mostly just a couple ms).
//This is to space the antidromic stimulations (e.g., every 1000 ms) to re-initiate the count-down process for the next time to check if antidromic stimulation needs to be considered once again:
//M_TriggerToStimulationLatency[] is set when the user moves the measuring line from the figure.
//M_InterStimulationInterval[] is set when the user changes the “Post-trigger delay” in “Properties” dialog
//So every time the measuring line or the property has been changed or a new figure becomes active with a stimulaing channel attached by the user, calculate this.
//Remember this is for each stimulating CHANNEL. Each channel can make the following antidromic stimulation delay by M_NumOfSamplingsAfterStim[]!!
void CAntsView::AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(int StimChanDOIdx) 
{
	if(M_InterStimulationInterval[StimChanDOIdx]<=MIMIMUM_POST_ANTI_DELAY || M_InterStimulationInterval[StimChanDOIdx]>5000){
		if(M_InterStimulationInterval[StimChanDOIdx]<=MIMIMUM_POST_ANTI_DELAY)AfxMessageBox("Are you sure that you want to trigger the antidromic stimulation that frequently(<=101ms)? ==> 1sec has been asigned, instead");
		if(M_InterStimulationInterval[StimChanDOIdx]>5000)AfxMessageBox("Are you sure that you want to trigger the antidromic stimulation that sparcely(>5000ms)? ==> 1sec has been asigned, instead");
		//Force a new time that is more appropriate
		M_InterStimulationInterval[StimChanDOIdx]=1000;
		m_PostStimPeriod[StimChanDOIdx]=1000;
	}
	M_NumOfSamplingsAfterStim[StimChanDOIdx]=Divide((M_TriggerToStimulationLatency[StimChanDOIdx] + 1000*M_InterStimulationInterval[StimChanDOIdx]) , ((*M_microSecPerPoint)*(*M_SamplesPerRead))); 
}

int CAntsView::qIdx(int i)
{
	if(i>=0 && i<(int)(*pM_AIQueueSize))return i;
	else if(i>(int)(*pM_AIQueueSize)-1)return (i-(int)(*pM_AIQueueSize));
	else return ((int)(*pM_AIQueueSize)+i);//if(i<0) => This happens when the program tries to retrieve the pre-threshold-crossing portion of the wave.
}

//The antidromic stimulation icon has been clicked.
void CAntsView::OnIs_AntidromicStimulation()
{
	//The user clicks anti- or ortho-dromic mode during the saving
	if(*M_is_LFP_saving==1){
		if(*M_IsOrthoStim==1){
			AfxMessageBox("Turn off the on-going Antidromic Stimulation first.");
			return;
		}
		else if(*M_IsAntidromicStim==1){OnSaveLFP();/*Let it turn off*/}
	}

	//Toggle the mode on/off
	m_bIsTo_ANTI_Stimulate=!m_bIsTo_ANTI_Stimulate;
	if(m_bIsTo_ANTI_Stimulate==1){
		//See 
		sprintf(m_SuFFix,"AD");
		*M_IsAntidromicStim=1;  
		*M_IsOrthoStim=0; m_isOrthodromicStimulation=0;
		m_GlobalTitle="Antidromic Stimulation";

		int row, clm, lyr;
		CFigure *pFig;
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					if(pFig->m_RecordingChan.m_ChanID>=0 && pFig->m_RecordingChan.m_ChanID<NUM_NN_AI_CHAN){
						pFig->m_Deleted=0;
					}
				}
			}
		}

		pM_AIQueueSize=&M_AIQueueSize[*M_AntidromicTriggeringChanID];
		if(M_numMatchingKinds[*M_AntidromicTriggeringChanID]>0)*M_WhichAntidromicMode=VTBOX_STIM_MODE; else *M_WhichAntidromicMode=THRESHOLD_STIM_MODE;

		//Compile the stimulation channel and recording channel numbers, without duplication.
		Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
		//Set the Stimulation Latencies and Post-stimulation period
		Set_StimLatency_and_PostStimPeriod();
	}
	else{
		*M_IsAntidromicStim=0;
		sprintf(m_SuFFix,"LFP");
		m_GlobalTitle="Anti-,Ortho-dromic Stimulation";
	}
	Make_TheBaseOfFileName(*M_is_LFP_saving);
	RequestDisplayEvent(RENAMING_TITLE);
	RequestDisplayEvent(REDRAW_FILE_NAME);
	RequestDisplayEvent(REDRAW_TRIGGER_CHANNEL_NUMBER);
}

//The orthodromic stimulation icon has been clicked.
void CAntsView::OnIs_OrthodromicStimulation()
{
	//The user clicks anti- or ortho-dromic mode during the saving
	if(*M_is_LFP_saving==1){
		if(*M_IsAntidromicStim==1){
			AfxMessageBox("Turn off the on-going Antidromic Stimulation first.");
			return;
		}
		else if(*M_IsOrthoStim==1){OnSaveLFP();/*Let it turn off*/}
	}

	//Toggle the mode on/off
	m_isOrthodromicStimulation=!m_isOrthodromicStimulation;
	//Receve the # of the recording channel from the user and apply it to every displayed figure.
	if(m_isOrthodromicStimulation==1){
		//Set appropriate variables
		sprintf(m_SuFFix,"LFP");
		*M_IsOrthoStim=1; 
		m_GlobalTitle="Orthodromic Stimulation";

		//############  Disable incompatable ones
		*M_IsAntidromicStim=0; m_bIsTo_ANTI_Stimulate=0; 
		if(m_IsFasterStm==1){
			m_IsFasterStm=0;
			for(int i=0;i<m_NumOfStimulationChannels;i++){
				M_InterStimulationInterval[i]=m_SavedPostStimPeriod[i];
				m_PostStimPeriod[i]=M_InterStimulationInterval[i];	
				AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(i); //How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
			}
		}
		if(*M_WhichAntidromicMode==AUTO_STIM_MODE){
			if(M_numMatchingKinds[*M_AntidromicTriggeringChanID]>0)*M_WhichAntidromicMode=VTBOX_STIM_MODE; else *M_WhichAntidromicMode=THRESHOLD_STIM_MODE;
		}
		//Disable the measuring lines that sets the stimulation latency
		int row,clm,lyr;
		CFigure *pFig;
		int StimChanDOIdx;
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					StimChanDOIdx=GetIndexOfSimChannel(pFig->m_StimGatingRelay.m_ChanID);
					if(StimChanDOIdx!=-1){
						//Set pFig->M_TriggerToStimulationLatency[0] to 0
						pFig->m_MeasuringLineTime[0]=0;
						M_TriggerToStimulationLatency[StimChanDOIdx]=0;
						AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(StimChanDOIdx);//How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
						*M_CurrIdx_Of_StimGateRelayID=StimChanDOIdx;//Open the gate attached to the figure. ==>Because of the last one is the one loaded most lately, the last one becomes the one stimulated unless the TAS decides to change this.
		}	}	}	}

		//Compile the stimulation channel and recording channel numbers, without duplication.
		Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
	}
	else{
		*M_IsOrthoStim=0; 
		sprintf(m_SuFFix,"AD");
		m_GlobalTitle="Anti-,Ortho-dromic Stimulation";
	}
	Make_TheBaseOfFileName(*M_is_LFP_saving);
	RequestDisplayEvent(REDRAW_SUPERIMPOSE_NUMBER);		
	RequestDisplayEvent(REDRAW_ALL_LAYERS);		
}


//The auto-stimulation icon has been clicked.
void CAntsView::OnAutoStim() 
{
	if(*M_IsOrthoStim==1){return;}

	if(*M_WhichAntidromicMode!=AUTO_STIM_MODE){
		*M_WhichAntidromicMode=AUTO_STIM_MODE;
	}
	else{
		if(M_numMatchingKinds[*M_AntidromicTriggeringChanID]>0)*M_WhichAntidromicMode=VTBOX_STIM_MODE; else *M_WhichAntidromicMode=THRESHOLD_STIM_MODE;
	}
}

//The icon is just to show the current mode, not functioning.
void CAntsView::OnThresholdMode() 
{
	//*M_WhichAntidromicMode=THRESHOLD_STIM_MODE;
}
//The icon is just to show the current mode, not functioning.
void CAntsView::On_VTBOX_mode() 
{
	//*M_WhichAntidromicMode=VTBOX_STIM_MODE;
}

void CAntsView::OnUpdateIsToAntidromicallyStimulate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bIsTo_ANTI_Stimulate==1);
}
void CAntsView::OnUpdateIsOrthodromicStimulation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isOrthodromicStimulation==1);
}
void CAntsView::OnUpdateAutoStim(CCmdUI *pCmdUI)
{	
	if(m_bFistTime==1)return;
	pCmdUI->SetCheck(*M_WhichAntidromicMode==AUTO_STIM_MODE);
}
//void CAntsView::OnUpdateThresholdMode(CCmdUI *pCmdUI)
//{	
//	//if(m_bFistTime==1)return;
//	//pCmdUI->SetCheck(*M_WhichAntidromicMode==THRESHOLD_STIM_MODE);
//}
//void CAntsView::OnUpdate_VTBOX_mode(CCmdUI *pCmdUI)
//{	
//	//if(m_bFistTime==1)return;
//	//pCmdUI->SetCheck(*M_WhichAntidromicMode==VTBOX_STIM_MODE);
//}
void CAntsView::RequestDisplayEvent(int Event) 
{
	RequestDisplayEvent(Event,1,  NULL,  NULL);
}
void CAntsView::RequestDisplayEvent(int Event,  int SomeIntValue) 
{
	RequestDisplayEvent(Event,2,  SomeIntValue,  NULL); 
}
void CAntsView::RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2) 
{
	RequestDisplayEvent(Event,3,  SomeIntValue1,  SomeIntValue2); 
}
void CAntsView::RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) 
{	
	m_DispEvents[m_DispEventIdx][0]=Event;
	m_DispEvents[m_DispEventIdx][1]=NumVariables;
	for(int i=0; i<NumVariables;i++){
		if(i==0)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue1; 
		else if(i==1)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue2; 
	}
	if(m_DispEventIdx+1<MAX_DISP_REQUESTS)m_DispEventIdx++; else m_DispEventIdx=0;
}
void CAntsView::CreateSharedMemoryArea()
{
	//In case the memory has been created by some other process, try it by opening. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;	
	#include "MemMap_Open_DAS.h" 
}

void CAntsView::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}



void CAntsView::Show2D_V() 
{

}
void CAntsView::Show2D_VI() 
{

}


void CAntsView::Show2D_VII() 
{
}


void CAntsView::Show2D_VIII()
{ 

}



void CAntsView::OnFunctions() 
{

}

void CAntsView::OnQuickFunction() 
{

}


void CAntsView::OnProceed() 
{
	m_bProceed=!m_bProceed;
}


void CAntsView::OnSlowDown() 
{
	m_nSpeed+=10;	
}

void CAntsView::OnSpeedUp() 
{
	if(m_nSpeed>=10)m_nSpeed-=10;	
}

#define FAST_LEARNING_STIM_INTERVAL 150 //ms
void CAntsView::OnFasterStm()
{
	if(*M_IsOrthoStim==1){return;}

	m_IsFasterStm=!m_IsFasterStm;
	//*M_IsToLearnArtifact=m_IsFasterStm;
	int i;
	if(m_IsFasterStm==1){
		for(i=0;i<m_NumOfStimulationChannels;i++){
			m_SavedPostStimPeriod[i]=M_InterStimulationInterval[i];
			M_InterStimulationInterval[i]=FAST_LEARNING_STIM_INTERVAL;
			m_PostStimPeriod[i]=M_InterStimulationInterval[i];	
			AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(i); //How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
		}	
	}
	else{
		for(i=0;i<m_NumOfStimulationChannels;i++){
			M_InterStimulationInterval[i]=m_SavedPostStimPeriod[i];
			m_PostStimPeriod[i]=M_InterStimulationInterval[i];	
			AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(i); //How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
		}
	}	
}


void CAntsView::OnUpdateFasterStm(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsFasterStm==1);
}


void CAntsView::OnFileContinuousRecordingMode()
{
	m_IsContinuousRecordingMode=!m_IsContinuousRecordingMode;
	if(m_IsContinuousRecordingMode==1)m_FirstRecordingInSequence=1;
}


void CAntsView::OnUpdateFileContinuousRecordingMode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_IsContinuousRecordingMode==1);
}


