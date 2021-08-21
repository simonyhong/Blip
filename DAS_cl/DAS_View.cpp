// DAS_View.cpp : implementation of the CDasView class
//

#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "PrefixSuffix.h"
#include "Figure_DAS.h"
#include "Channel.h"
#include "DAS_View.h"
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

BOOL CDasView::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CDasView

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CDasView, CView)

BEGIN_MESSAGE_MAP(CDasView, CView)
	//{{AFX_MSG_MAP(CDasView)
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
	ON_COMMAND(ID_IS_HIGH_TO_LOW, &CDasView::OnIsFromHighToLow)
	ON_UPDATE_COMMAND_UI(ID_IS_HIGH_TO_LOW, &CDasView::OnUpdateIsPD)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SELECT1, &CDasView::OnSelect0)
	ON_COMMAND(ID_UNSELECT1, &CDasView::OnUnselect1)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_SCALE_DELETE_FIGS, &CDasView::OnDeleteFigs)
	ON_COMMAND(ID_SCALE_DELETE_THE_REST, &CDasView::OnDelete_the_Rest_Figs)
	ON_COMMAND(ID_FILE_SAVE, &CDasView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CDasView::OnFileOpen)
	ON_EN_CHANGE(IDC_EDIT_STRING1, &CDasView::OnEnChangeEditString1)
	ON_UPDATE_COMMAND_UI(ID_SELECT1, &CDasView::OnUpdateSelect0)
	ON_UPDATE_COMMAND_UI(ID_UNSELECT1, &CDasView::OnUpdateUnselect1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_VT_DELETE, &CDasView::OnVtDelete)
	ON_COMMAND(ID_SELECT2, &CDasView::OnSelect1)
	ON_COMMAND(ID_SELECT3, &CDasView::OnSelect2)
	ON_UPDATE_COMMAND_UI(ID_SELECT2, &CDasView::OnUpdateSelect1)
	ON_UPDATE_COMMAND_UI(ID_SELECT3, &CDasView::OnUpdateSelect2)
	ON_COMMAND(ID_CONFIG_FILE_SAVE, &CDasView::OnConfigFileSave)
	ON_COMMAND(ID_FILE_CLOSE, &CDasView::OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CDasView::OnUpdateFileSave)
	ON_COMMAND(ID_ADJUST_SCALE_TO_FIT_DATA, &CDasView::OnAdjustScaleToFitData)
	ON_COMMAND(ID_DISPLAY_ACQUISION_DISRUPTION, &CDasView::OnDisplayAcquisionDisruption)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_ACQUISION_DISRUPTION, &CDasView::OnUpdateDisplayAcquisionDisruption)
	ON_COMMAND(ID_VIEW_FLIP_SIGNAL, &CDasView::On_FlipSignal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FLIP_SIGNAL, &CDasView::On_UpdateViewFlipSignal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CDasView construction/destruction
CDasView::CDasView()
{
	// TODO: add construction code here
	m_nColorShow=BLACK_BRUSH;
	m_bShow2D=0;  
	m_bFistTime=1;
	m_nKey=0;
	m_FlagVirMemDC=0;
	m_bProceed=0;
	m_bIsON=0;
	m_KeyCode=-1;
	m_dScaleUpDn=1;
	m_isInsidePane=0;
	m_dHowManyRows=2; m_dHowManyClmns=1;
	m_SelColor=0;
	m_bIsScrXYforIJ=0;
	m_MaxNumWaveformsToDisp=7;
	m_IsThresholdHeld=0;
	m_DispEventIdx=0;
	m_numAiChannels=MAX_AI_CHAN;
	m_AntiDromicOutput=0;
	m_prvFileNameRect.top=0;m_prvFileNameRect.bottom=0;m_prvFileNameRect.left=0;m_prvFileNameRect.right=0;

	m_isSaving=0;
	sprintf(m_PreFix,"");sprintf(m_SuFFix,"SPK");
	Make_TheBaseOfFileName();
	m_RecordingChanId=0; m_StopAfterSavingNwaves=3000;

	////////////////////////////////////
	m_NULL.CreatePen(PS_NULL, 1,RGB(0,0,0));
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
	/////////////
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
	m_DoNotErase=0;
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	m_numCaptured=0;
	m_pSpotlightFig=NULL;
	for(i=0;i<MaxFigCln;i++){
		for(j=0;j<MaxFigRow;j++){
			m_Fig[j][i][0].m_row=j;
			m_Fig[j][i][0].m_clm=i;
			m_Fig[j][i][0].m_dpth=0;
		}
	}
	
	m_prvDate="";
	m_NumAiChannels=0;
	m_FirstTime0ElementErr=1;
	m_DisplayAcquisionDisruption=0;
	//for(int i=0;i<200;i++)m_Hist[i]=0;
}					


CDasView::~CDasView() 
{
	int i;
	for(i=0; i<NUM_NN_AI_CHAN;i++){
		M_numMatchingKinds[i]=0; //This is to inform server that the client is out.
		M_ChanBoxInfoChanged[i]=1;//The channel info needs to be updated
		if(m_AiChan[i].m_IsThisChanUsed==1){if(M_NumOfFigsUsingThisChan[i]>0)M_NumOfFigsUsingThisChan[i]=0;} //Inform the server that the figure is not using this channel anymore.
	}
	*M_HeyDASsv_SomeChanBoxInfoChanged=1;

	m_bIsON=0;
	*M_isDAScl_Running=0;
	CloseSharedMemoryArea();
	
	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	

	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	m_GrayBrush.DeleteObject(); m_RedBrush.DeleteObject();m_bkBrush.DeleteObject();
	for(i=0; i<MaxPen;i++){
		m_rgbPEN1[i].DeleteObject(); m_rgbPEN2[i].DeleteObject(); m_rgbPEN3[i].DeleteObject();
	}
	m_NULL.DeleteObject(); m_RED.DeleteObject(); m_BLUE.DeleteObject();m_GREEN.DeleteObject(); m_GREEN2.DeleteObject();m_DkGREEN.DeleteObject();m_CYAN1.DeleteObject();m_CYAN2.DeleteObject(); m_PINK.DeleteObject();m_BROWN.DeleteObject();m_VIOLET.DeleteObject();
	m_ORANGE2.DeleteObject(); m_VIOLET2.DeleteObject();m_VIOLET3.DeleteObject();m_VIOLETLIGHT2.DeleteObject();m_VIOLETLIGHT1.DeleteObject();m_BLACK.DeleteObject();m_BLACK2.DeleteObject();m_BLACK3.DeleteObject();m_bkPen.DeleteObject();
	m_GRAY1.DeleteObject();m_GRAY2.DeleteObject();m_CYAN3.DeleteObject();m_BlueGRAY.DeleteObject();m_PINK3.DeleteObject();m_RED3.DeleteObject(); m_BLUE3.DeleteObject(); m_GREEN3.DeleteObject();
	m_RED5.DeleteObject(); m_BLUE5.DeleteObject(); m_BLACK5.DeleteObject(); m_RED8.DeleteObject(); m_BLUE8.DeleteObject(); m_BLACK8.DeleteObject();
	m_font.DeleteObject();
}

BOOL CDasView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDasView drawing
UINT Graph2D_I(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Show2D_I() ;
	if(*(pView->M_Blip_OFF)==1)AfxGetMainWnd()->SendMessage(WM_CLOSE);
	return 0;
}
UINT Graph2D_II(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	//pView->Show2D_II() ;
	return 0;
}
UINT Graph2D_III(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	//pView->DAS_cl_MAIN() ;
	return 0;
}
UINT Graph2D_IV(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Show2D_IV() ;
	return 0;
}

#define SIZE_OF_BITMAP_X 1.
#define SIZE_OF_BITMAP_Y 1.
void CDasView::OnDraw(CDC* pDC)
{
#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CDasDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1){
		VirMemDC();
		m_dHowManyClmns=-1;
		OnFileOpen();//Try to open a default root file.
		if(m_dHowManyClmns!=-1){//Succeeded in opening a default root file 
			RecalibratePanes();//The screen positions of the left, right, up, down edges of all panes need to be assigned.
			AfxBeginThread( Graph2D_I, this);
			while(m_NumAiChannels==0){Sleep(5); if(m_bIsON==0)return;}
			if(*M_isDAS_Running==0)LaunchServer();
			return;//Opening a default root file has been successful.
		}
		//Failed to open a default root file => implement the program's own version.
		sprintf(m_FileNameFULL,"");
		sprintf(m_ConfigJustFileName,"NO root file");
		m_dHowManyClmns=1; 
		Re_measure_ClientView();
		Start_a_NEW_set_of_figures();
		RecalibratePanes();
		m_bFistTime=0;
		AfxBeginThread( Graph2D_I, this);
		while(m_NumAiChannels==0){Sleep(5); if(m_bIsON==0)return;}
		if(*M_isDAS_Running==0)LaunchServer();
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

			Re_measure_ClientView();
			Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
			Batch_Prepare4FigFrame();
			RecalibratePanes();
			for(int chanIdx=0; chanIdx<m_NumAiChannels; chanIdx++){	
				m_AiChan[chanIdx].Remeasure_Box_Positions_DataAnchored();	
			}
			Remeasure_ThresholdPositions();	
			RequestDisplayEvent(RESIZING_WIN);
		}
		else{//Just display without changing anything
			RequestDisplayEvent(REFRESH_SCREEN);
		}
	}
}

void CDasView::LaunchServer()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef X64_Version
	Find_Blip_andAddSome("\\DAS_sv\\x64\\Release\\DAS_sv.exe", DirStr);
	#endif
	#ifndef X64_Version
	Find_Blip_andAddSome("\\DAS_sv\\Release\\DAS_sv.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		AfxMessageBox("Failed to execute DAS_sv.exe\n Try to open manually.");
	}
}

void CDasView::Remeasure_ThresholdPositions()
{
	CAiChannel *pChan;
	for(int i=0; i<NUM_NN_AI_CHAN;i++){
		pChan=&(m_AiChan[i]);
		if(pChan->pFig==NULL)continue;//No figure is attached to this channel.
		pChan->pFig->FromDataToScreenY(*(pChan->M_Threshold), pChan->pFig->m_prvThreshold_onScrn); //It maps a relative point (i,j) of the graph to a point on the screen.	
	}
}

void CDasView::Start_a_NEW_set_of_figures()
{			
	int i,j,chanIdx;
	CFigure *pFig;
	m_numCaptured=0;
	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			chanIdx=i*MaxFigCln+j;
			pFig=&m_Fig[i][j][0];
			pFig->m_toDisplayThreshold=1; pFig->m_Threshold=0.5; pFig->m_prvThreshold_onScrn=0.5;
			if(pFig->pChan!=NULL && (pFig->pChan->m_ChanID>=0&&pFig->pChan->m_ChanID<NUM_NN_AI_CHAN)&& pFig->pChan->m_IsThisChanUsed!=1 ){//make all the visible channels active.
				AttachFig_toChan(pFig,pFig->pChan->m_ChanID);
			}
			else if(chanIdx>=0&&chanIdx<NUM_NN_AI_CHAN)AttachFig_toChan(pFig,chanIdx);
			else{pFig->SetFontSize(&m_font); pFig->SetMinMaxXY(-0.5,4,-1,1);pFig->SetLabelsXY("",""); pFig->m_Captured=0; pFig->m_Deleted=1; continue;}
			pFig->SetFontSize(&m_font);
			pFig->SetMinMaxXY(-0.5,4,-1,1);pFig->SetLabelsXY("","");
			pFig->m_Captured=0;
			pFig->m_Deleted=0;
	}	}
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	Batch_Prepare4FigFrame();
	Batch_DrawFigureFrames();
}

void CDasView::Re_measure_ClientView()
{			
	CRect rt;
	GetClientRect(&rt);
	m_savedClientRect=rt;
	if(m_bFistTime==1){
		CFont* pFont = m_DC[0].GetCurrentFont();
		LOGFONT LogFont;
		pFont->GetLogFont(&LogFont);
		m_fontHeight=LogFont.lfHeight;
		m_fontWidth=LogFont.lfWidth;
	}
	m_TitleArea.top=rt.top;	m_TitleArea.bottom=rt.top+m_fontHeight; m_topOfFigs=m_TitleArea.bottom;
	m_TitleArea.left=rt.left; m_TitleArea.right=rt.right;

	//File name or its base
	RemeasureFileNameRect();

	//The basic size of the figures
	m_dSizeOfOneRow =(rt.bottom-m_topOfFigs-ViewMargin4FigsAlongEdges)/m_dHowManyRows;  if(m_dSizeOfOneRow <=0)m_dSizeOfOneRow=0;
	m_dSizeOfOneClmn=(rt.right-rt.left-ViewMargin4FigsAlongEdges)/m_dHowManyClmns; if(m_dSizeOfOneClmn<=0)m_dSizeOfOneClmn=0;
	m_savedClientRect.top=m_topOfFigs;
}



/////////////////////////////////////////////////////////////////////////////
// CDasView printing

BOOL CDasView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDasView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDasView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDasView diagnostics

#ifdef _DEBUG
void CDasView::AssertValid() const
{
	CView::AssertValid();
}

void CDasView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDasDoc* CDasView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDasDoc)));
	return (CDasDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDasView message handlers


UINT Graph2D_V(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Show2D_V() ;
	return 0;
}
UINT Graph2D_VI(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Show2D_VI() ;
	return 0;
}
UINT Graph2D_VII(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Show2D_VII() ;
	return 0;
}
UINT Graph2D_VIII(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	//pView->Show2D_VIII() ;
	return 0;
}


void CDasView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//if(m_nShowMode==GRAPH_3D){ThD.KeyDown(nChar); goto ToEND;}
	m_KeyCode=nChar;
	switch(nChar){
		case VK_DELETE:
			OnDeleteFigs();
			m_KeyCode=-1;
			break;
		default: break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDasView::On2DGraph_I() 
{
}
void CDasView::On2DGraph_II() 
{
		AfxBeginThread( Graph2D_II, this);
}
void CDasView::On2DGraph_III() 
{
}
void CDasView::On2DGraph_IV() 
{
		AfxBeginThread( Graph2D_IV, this);
}
void CDasView::On2DGraph_V() 
{
		AfxBeginThread( Graph2D_V, this);
}
void CDasView::On2DGraph_VI() 
{
		AfxBeginThread( Graph2D_VI, this);
}
void CDasView::On2DGraph_VII() 
{
		AfxBeginThread( Graph2D_VII, this);
}
void CDasView::On2DGraph_VIII() 
{
		//AfxBeginThread( Graph2D_VIII, this);
}

void CDasView::VirMemDC()
{
	//########  Preparing Virtual Window  ##########
	if(m_FlagVirMemDC==1){
		//Clear screen
		//m_DC[0].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
		return;
	}
	m_FlagVirMemDC=1;
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
	
	int i;
	for(i=0;i<MaxDC;i++){
		m_DC[i].CreateCompatibleDC(&aDC);
		m_bmp[i].CreateCompatibleBitmap(&aDC, m_nScrMaxX*SIZE_OF_BITMAP_X, m_nScrMaxY*SIZE_OF_BITMAP_Y);
		m_DC[i].SelectObject(&m_bmp[i]);
		m_DC[i].SelectObject(&m_bkBrush);
		m_DC[i].SelectObject(&m_font);
		//m_DC[i].PatBlt(0,0,m_nScrMaxX,m_nScrMaxY,PATCOPY);
	}
	
	if(CAiChannel::m_DiviceContaxtCreatedAlready2==0){
		CAiChannel::m_DiviceContaxtCreatedAlready2=1;
		CAiChannel::m_ChDC.CreateCompatibleDC(&aDC);
		CAiChannel::m_ChBmp.CreateCompatibleBitmap(&aDC, m_nScrMaxX*SIZE_OF_BITMAP_X, m_nScrMaxY*SIZE_OF_BITMAP_Y);
		CAiChannel::m_ChDC.SelectObject(&CAiChannel::m_ChBmp);
		CAiChannel::m_ChDC.SelectObject(&m_bkBrush);
		CAiChannel::m_ChDC.SelectObject(&m_font);
	}

	m_SavedDC=m_DC[0].SaveDC();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//################################################  2D Graphics #######################################
#define MY_PEEK_MESSAGE {if(::PeekMessage (&message, NULL,0,0, PM_REMOVE)){::TranslateMessage(&message);::DispatchMessage(&message);}	}		

//long CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
//long CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

//##################   No more than ONE DAS client at a time is allowed. (No duplicate channel windows are allowed at a time) ##################
void CDasView::Show2D_I() 
{
	m_bIsON=1;
	//############ Create the memory map
	CreateSharedMemoryArea();

	//########## "STATIC" variable initialization for the SOFTWARE Channel configuration
	//Dynamic initializations that may vary during execution will be handled in DAS_sv; see "Init_MemoryMapVariables()" in DAS_sv.
	int i;
	char chanID[256];
	CAiChannel *pChan;
	for(i=0; i<m_numAiChannels;i++){
		pChan=&(m_AiChan[i]);
//		pChan->m_AiIdx=0;//Initialize the data index.
		//Shared memory map
		pChan->M_AI=M_AI[i];
		pChan->M_timeStampMicS=M_timeStampMicS[i];
		pChan->M_AIQueueSize=&M_AIQueueSize[i];
		pChan->M_usableAiIdx=M_usableAiIdx[i];

		////Create a Voltage channel
		//DAQmxCreateAIVoltageChan(m_pTaskHandle_AI,
		//	chanID,
		//	"",
		//	DAQmx_Val_RSE/*Referenced single-ended mode*/,
		//	-pChan->gain,pChan->gain,
		//	DAQmx_Val_Volts,NULL);
	}
	if(NUM_NN_AI_CHAN>MaxFigRow*MaxFigCln){char Msg[128]; sprintf(Msg,"Currently, DAS supports up to %dx%d fugures.",MaxFigRow,MaxFigRow); AfxMessageBox(Msg); exit(1);}
	for(i=0; i<NUM_NN_AI_CHAN;i++){	
		pChan=&(m_AiChan[i]);
		pChan->pView=this;
		pChan->M_usableSIGi=M_usableSIGi[i];
		pChan->M_firstIdxOfWavelet=M_firstIdxOfWavelet[i];
		pChan->M_lengthOfSignal=M_lengthOfSignal[i];
		pChan->M_AdjustXToXthreshold=M_AdjustXToXthreshold[i];
		pChan->M_usableSpikeIdxOfNn1=M_usableSpikeIdxOfNn1[i];
		pChan->M_usableSpikeIdxOfNn2=M_usableSpikeIdxOfNn2[i];
		pChan->M_usableSpikeIdxOfNn3=M_usableSpikeIdxOfNn3[i];
		pChan->M_spikeWaveIdxOfNn1=M_spikeWaveIdxOfNn1[i];
		pChan->M_spikeWaveIdxOfNn2=M_spikeWaveIdxOfNn2[i];
		pChan->M_spikeWaveIdxOfNn3=M_spikeWaveIdxOfNn3[i];
		pChan->M_spikeTimeStampOfNn1=M_spikeTimeStampOfNn1[i];
		pChan->M_spikeTimeStampOfNn2=M_spikeTimeStampOfNn2[i];
		pChan->M_spikeTimeStampOfNn3=M_spikeTimeStampOfNn3[i];
		pChan->M_numSpikesOfNn1=M_numSpikesOfNn1[i];
		pChan->M_numSpikesOfNn2=M_numSpikesOfNn2[i];
		pChan->M_numSpikesOfNn3=M_numSpikesOfNn3[i];
		pChan->M_firstSpkIdxInTrialOfNn1=M_firstSpkIdxInTrialOfNn1[i];
		pChan->M_firstSpkIdxInTrialOfNn2=M_firstSpkIdxInTrialOfNn2[i];
		pChan->M_firstSpkIdxInTrialOfNn3=M_firstSpkIdxInTrialOfNn3[i];
		pChan->M_Spike_QueueSize=M_Spike_QueueSize;
		pChan->m_preWaveSpanDataSize=TIME_TO_DATA_POINTS(DEFAULT_WAVESPAN_PRE_THRESHOLD);
		pChan->m_postWaveSpanDataSize=TIME_TO_DATA_POINTS(DEFAULT_WAVESPAN_POST_THRESHOLD);
		pChan->m_DataPointsPerWave=pChan->m_postWaveSpanDataSize+pChan->m_preWaveSpanDataSize;
		pChan->pFig=&m_Fig[i/MaxFigCln][i-MaxFigCln*(i/MaxFigCln)][0];//First row displays the spike-discriminator
		if(pChan->pFig->m_ChanID!=-1){pChan->m_ChanID=pChan->pFig->m_ChanID;} //In case the Configuration File was loaded
		else{ pChan->pFig->m_ChanID=i; pChan->m_ChanID=i;}
		sprintf(chanID,"Channel: %d",pChan->pFig->m_ChanID);
		pChan->pFig->m_AssociatedChan.NameString=chanID;
		pChan->pFig->pChan=pChan;
		pChan->m_isVT_Selected.SetSize(MAX_WAVE_PTRs); //This needs to be done, because it has not been done. 
		for(int WaveCnt=0 ; WaveCnt<MAX_WAVE_PTRs; WaveCnt++){
			pChan->m_isVT_Selected[WaveCnt]=0; //Initialize (0 means undetermined)
		}
		pChan->M_Threshold=&M_Threshold[i];
		*pChan->M_Threshold=pChan->pFig->m_Threshold;
		pChan->M_WvEcode=M_WvEcode[i];
	}
	*M_SpotlightChanID_forCLAS=0;//The default channel to display selected/non-selected signals for CLAS. Selection of the figure by the user will change is one later.
	m_NumAiChannels=MAX_AI_CHAN;//2 neural analog channels neuronal signals; 4 analog channels for behavior monitoring
	m_ArraySize_RawData=m_NumAiChannels*(*M_SamplesPerRead);
	Make_TheBaseOfFileName();DispFileName();

	//######   Initial drawing and Figure-Channel configuration ########
	Figure_Channel_Config();
	//##################################################################


	//This is needed because DAS_sv is the only one initializing "DYNAMIC" variables.
	//See "Init_MemoryMapVariables()" in DAS_sv.
	//The initialized variables will be used in DAS_cl_MAIN();
	*M_isDAScl_Running=1;
	while(*M_isDAS_Running==0){Sleep(5); if(m_bIsON==0)return;}
	//############# Client's task starts here #############
	DAS_cl_MAIN();
	//#####################################################
	*M_isDAScl_Running=0;
	m_bIsON=0;
}

//Display manager for DAS
void CDasView::DAS_cl_MAIN() 
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);// THREAD_PRIORITY_TIME_CRITICAL  THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_NORMAL  THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_LOWEST  THREAD_PRIORITY_IDLE 
	int i,a,chan,Smpl=0, saveAtEvery=10, Prv_M_Variable_For_Anything=*M_Variable_For_Anything;
	char value[256];
	CFigure* pFig1=&m_Fig[0][1][0], *pFig10=&m_Fig[1][0][0]; 
	LPARAM pLpFig1=(LPARAM)pFig1; //This is just to hand over to a callback function
	CClientDC objDC(this);
	objDC.SetBkColor(RGB(220, 220, 220));
	CAiChannel *pChan;
	for(chan=0;chan<NUM_NN_AI_CHAN;chan++){	
		m_WaveIdxToDisp[chan]=M_usableWaveIdx[chan];	
		m_AiChan[chan].m_SelectedWaveFormsToDisp.SetSize(m_MaxNumWaveformsToDisp);
		m_NumSelectedWaveformsDisplayed[chan]=0; m_PrvNumSelectedWaveformsDisplayed[chan]=0;
		m_NumNotSelectedWaveformsDisplayed[chan]=0;
	}
	*M_NumSelectedWaveformsDisplayed=0;
	*M_NumNonSelectedWaveformsDisplayed=0;

	while(*M_usableAiIdx[0]<0){
		Sleep(5);
		if(m_bIsON==0)goto END_SIG_ACQUISION;
	}
	m_LastTimeDisplayed=*M_LatestTimeStampInMicroS;

	int SamplingEpoch_usxScale=*M_SamplingEpoch_us*0.01;
	int TimeDist=0, TimeIdx=0, jitter, biggerThan10Ms=0; __int64 JitterOfTimeStamp=0, TotalExamined=0;
	for(i=0;i<100;i++){m_Hist[i]=0;}

	int SaveIdxDist,usableIdx; 
	int Dist,tmpDist,nextDispIDx;
	int DispIdx=0, DispIdxDist, tmpWaveIdxToDisp, *pDispIdx,endI; 
	m_DispEventIdx=0;

	//##########  Designate Recording Channel #######################################
	DesignateRecordingChannel(m_RecordingChanId);
	//###############################################################################

	timeBeginPeriod(1); //This one makes the Sleep() function accurate
	while(1){
		//Update the neuron # in case TAS or ANTs has changed it.
		if(*M_NnNumberToRecord>0 && m_PrvNnNumberToRecord!=*M_NnNumberToRecord){Make_TheBaseOfFileName(); RequestDisplayEvent(DISP_FILE_NAME);}

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
				DisplayStuff(DispIdx,&objDC);	DispIdx=nextDispIDx;				
			}
		}

		if(m_DisplayAcquisionDisruption==1 && *M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec>5000){//~4.5ms of data acquision disruption.
			sprintf(value,"%d ms of acquision disruption",(int)((*M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec)/1000)); 
			*M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec=0;
			AfxMessageBox(value);
		}

		//############### Refresh the screen at every 60ms (~16Hz)
		if(*M_LatestTimeStampInMicroS-m_LastTimeDisplayed>60000){
			//The selected ones get drawn right before the figure is displayed => to place the selected ons on top of non-selected ones
			for(chan=0;chan<NUM_NN_AI_CHAN;chan++){
				if(m_AiChan[chan].m_IsThisChanUsed==0 || m_AiChan[chan].pFig==NULL)continue; //If the channel is not attached to any figure, skip.
				pDispIdx=&(m_AiChan[chan].m_SelectedWaveFormsToDisp[0]);
				endI=m_NumSelectedWaveformsDisplayed[chan];
				for(i=m_PrvNumSelectedWaveformsDisplayed[chan];i<endI;i++){
					DrawWave(pDispIdx[i],chan);
				}
				m_PrvNumSelectedWaveformsDisplayed[chan]=m_NumSelectedWaveformsDisplayed[chan];
			}
			RefreshFrame();
			//if(++Smpl%20==0){
			//	Smpl=0; 
			//	//##########  A newer and more relevant monitoring:
			//	//Simon: It seems that, occasionally, Windows just throws away some threads that it did not have chance to execute. 
			//	//		 For this reason, once some data acquisition is missing, it will be missing forever, therefore the backlog number (*M_Variable_For_Anything) may never be recovered to 0.
			//	//		 Because of this, the following warning is relevant only when pervious backlog is different from the current one; it gives the user a warning to refrain from doing too burdensome tasks (such as moving windows etc.) while Blip is running.			
			//	if(Prv_M_Variable_For_Anything!=*M_Variable_For_Anything){
			//		if(*M_Variable_For_Anything-Prv_M_Variable_For_Anything>50){
			//			sprintf(value,"%d ms of acquision disruption",(int)((*M_Variable_For_Anything-Prv_M_Variable_For_Anything)*((1000.*SAMPLES_PER_READ)/DATA_SAMPLING_RATE))); 
			//			AfxMessageBox(value);
			//		}
			//		Prv_M_Variable_For_Anything=*M_Variable_For_Anything;
			//	}

			//	//##########  The old and more strict monitoring:
			//	////Does the channel's thread lag behand the request of the DAQsv?
			//	//if(*M_Variable_For_Anything>0 || Prv_M_Variable_For_Anything!=*M_Variable_For_Anything){
			//	//	Prv_M_Variable_For_Anything=*M_Variable_For_Anything;
			//	//	sprintf(value,"Backlog:%d  ",*M_Variable_For_Anything); objDC.TextOut(500,1 ,value);
			//	//}
			//	//if(*M_Variable_For_Anything>50 && *M_Variable_For_Anything<1000){
			//	//	sprintf(value,"Backlog:%d  ",*M_Variable_For_Anything); objDC.TextOut(500,1 ,value);
			//	//	AfxMessageBox("There is some backlog of data.");
			//	//}
			//	//else if(*M_Variable_For_Anything>=1000){
			//	//	sprintf(value,"Backlog:%d  ",*M_Variable_For_Anything); objDC.TextOut(500,1 ,value);
			//	//	AfxMessageBox("There is too much backlog of data: Please restart everything");
			//	//}
			//}
			m_LastTimeDisplayed=*M_LatestTimeStampInMicroS;
		}
		else if(*M_LatestTimeStampInMicroS<m_LastTimeDisplayed)m_LastTimeDisplayed=*M_LatestTimeStampInMicroS;//the server has newly restarted.

		// The "Sleep() should be long like this, otherwise even the Save function will be running too much.
		// However, it shouldn't be too long (more than 10ms) because the wave array buffer is limited. 
		// In the worst case where the wave has a frequency of ~100KHz the buffer can hold up to 1 second worth of waves (in the case where the size is 100,000 as the current one MAX_WAVE_PTRs is)  
		Sleep(5); 
		
		//M_WvEcode[][] is used to display selected ones on top of non-selected ones.
		//##############  Draw Waves
		for(chan=0;chan<NUM_NN_AI_CHAN;chan++){
			pChan=&m_AiChan[chan];
			if(pChan->m_IsThisChanUsed==0 ||m_NumSelectedWaveformsDisplayed[chan]>=m_MaxNumWaveformsToDisp || M_usableWaveIdx[m_RecordingChanId]<0)continue;//Not used channel
			tmpWaveIdxToDisp==m_WaveIdxToDisp[chan];
			if(M_usableWaveIdx[chan]>=m_WaveIdxToDisp[chan]){Dist=M_usableWaveIdx[chan]-m_WaveIdxToDisp[chan];}
			else Dist=MAX_WAVE_PTRs-m_WaveIdxToDisp[chan]+M_usableWaveIdx[chan];
			tmpDist=Dist; tmpWaveIdxToDisp=m_WaveIdxToDisp[chan];
			//The case "m_WaveIdxToDisp[chan]<0" will not happen becasue Dist>0 happens only when "m_WaveIdxToDisp[chan]>=0"
			while(tmpDist>0 && m_NumSelectedWaveformsDisplayed[chan]<m_MaxNumWaveformsToDisp){//Register the selected waveforms to dispaly later
				if(tmpWaveIdxToDisp+1<MAX_WAVE_PTRs)tmpWaveIdxToDisp++;	else tmpWaveIdxToDisp=0;
				if(M_WvEcode[chan][tmpWaveIdxToDisp]>=0){
					if(chan==*M_SpotlightChanID_forCLAS)M_SelectedWaveFormsToDisp[(*M_NumSelectedWaveformsDisplayed)++]=tmpWaveIdxToDisp;
					pChan->m_SelectedWaveFormsToDisp[m_NumSelectedWaveformsDisplayed[chan]++]=tmpWaveIdxToDisp;
				}
				tmpDist--;
			}
			tmpDist=Dist; tmpWaveIdxToDisp=m_WaveIdxToDisp[chan];
			while(tmpDist>0 &&  m_NumSelectedWaveformsDisplayed[chan]+m_NumNotSelectedWaveformsDisplayed[chan]<m_MaxNumWaveformsToDisp){//Display the non-selected waveforms
				if(tmpWaveIdxToDisp+1<MAX_WAVE_PTRs)tmpWaveIdxToDisp++; else tmpWaveIdxToDisp=0;
				if(M_WvEcode[chan][tmpWaveIdxToDisp]<0){
					if(chan==*M_SpotlightChanID_forCLAS)M_NonSelectedWaveFormsToDisp[(*M_NumNonSelectedWaveformsDisplayed)++]=tmpWaveIdxToDisp;
					m_NumNotSelectedWaveformsDisplayed[chan]++; 
					DrawWave(tmpWaveIdxToDisp,chan);
				}
				tmpDist--;
			}
			if(m_WaveIdxToDisp[chan]+Dist<MAX_WAVE_PTRs)m_WaveIdxToDisp[chan]+=Dist; else m_WaveIdxToDisp[chan]=(Dist+m_WaveIdxToDisp[chan])-MAX_WAVE_PTRs;
		}
		//##############  Save Waves to a file.
		if(saveAtEvery++>=50&& m_isSaving==1 && M_usableWaveIdx[m_RecordingChanId]>0){
			saveAtEvery=0;
			if(m_numOfSavedWaves>=m_StopAfterSavingNwaves){m_isSaving=0; RequestDisplayEvent(DISP_FILE_NAME);}
			else{
				usableIdx=M_usableWaveIdx[m_RecordingChanId];
				if(usableIdx>=m_SaveIdx)SaveIdxDist=usableIdx-m_SaveIdx;
				else SaveIdxDist=MAX_WAVE_PTRs-m_SaveIdx+usableIdx;
				if(SaveIdxDist>0){
					SaveWaves(SaveIdxDist);
				}
			}
			sprintf(value,"(%d/%d)",m_numOfSavedSelectedWaves,m_numOfSavedWaves);
			objDC.TextOut(m_FileNameRect.right+m_fontWidth, m_FileNameRect.top,value);
		}

		//#############
		if(m_bIsON==0 || *M_Blip_OFF==1)break;
	}
	timeEndPeriod(1);//match this with 	timeBeginPeriod(1) above 
END_SIG_ACQUISION:;
	/////////////////////
}

void CDasView::DesignateRecordingChannel(int RecordingChannel) 
{
	pM_AI_save=M_AI[RecordingChannel];
	m_AIQueueSize_save=M_AIQueueSize[RecordingChannel];
	if(m_AIQueueSize_save<=0)AfxMessageBox("m_AIQueueSize_save<=0");
	m_DataPointsPerWave_save=m_AiChan[RecordingChannel].m_DataPointsPerWave;
	m_FirstIdxOfWavelet=m_AiChan[RecordingChannel].M_firstIdxOfWavelet;
	m_AdjustXToXthreshold_save=m_AiChan[RecordingChannel].M_AdjustXToXthreshold;
}

void CDasView::Figure_Channel_Config() 
{
	Re_measure_ClientView();
	Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
	Batch_Prepare4FigFrame();
	CFigure *pFig;
	int i,j,chanIdx;
	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			chanIdx=i*MaxFigCln+j;
			pFig=&m_Fig[i][j][0];
			//pFig->m_toDisplayThreshold=1; pFig->m_Threshold=0.5; pFig->m_prvThreshold_onScrn=0.5;
			if(pFig->pChan!=NULL && (pFig->pChan->m_ChanID>=0&&pFig->pChan->m_ChanID<NUM_NN_AI_CHAN)&& pFig->pChan->m_IsThisChanUsed!=1 ){//make all the visible channels active.
				AttachFig_toChan(pFig,pFig->pChan->m_ChanID);
			}
			else if(chanIdx>=0&&chanIdx<NUM_NN_AI_CHAN)AttachFig_toChan(pFig,chanIdx);
			else{pFig->SetFontSize(&m_font); pFig->SetMinMaxXY(-0.5,4,-1,1);pFig->SetLabelsXY("",""); pFig->m_Captured=0; pFig->m_Deleted=1; }
	}	}
	Batch_DrawFigureFrames();
	DisplayTitle();
	DispFileName();
	RecalibratePanes();//The screen positions of the left, right, up, down edges of all panes need to be assigned.
	//Initially, determine the screen position of the threshold
	Remeasure_ThresholdPositions();
	m_bFistTime=0; //Now declear the initiation of the program is over
	EliminateExcluded();
	RequestDisplayEvent(RENAMING_FIG);
}


void CDasView::SaveWaves(int N)
{	
	CFile theFile;
	theFile.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive ar(&theFile, CArchive::store);
	int i,cnt,Length,furtureUseInt=0,firstIdx;
	while(N-- >0){
		if(m_SaveIdx+1<MAX_WAVE_PTRs)m_SaveIdx++; else m_SaveIdx=0;//This needs to come before anything.

		if(m_numOfSavedWaves>=m_StopAfterSavingNwaves)break;
		if(m_numOf_ConsecutiveNonSelectedWaves_Saved>=9 && M_WvEcode[m_RecordingChanId][m_SaveIdx]<0)continue;//This guarentees that at least 1 out of 10 will be a selected wave
		firstIdx=m_FirstIdxOfWavelet[m_SaveIdx];
		ar << ++m_numOfSavedWaves; //Nth wave to be saved
		ar << (int)DATA_SAMPLING_RATE;  //Hz
		ar << M_timeStampMicS[m_RecordingChanId][firstIdx];
		ar << m_AdjustXToXthreshold_save[m_SaveIdx]; //The time of the first signal
		ar << m_DataPointsPerWave_save; //The number of data points (data length)
		ar << M_WvEcode[m_RecordingChanId][m_SaveIdx]; //The ECode: not selected(<0), neuron 1 (0), neuron 2(1), neuron3(2).
		if(M_WvEcode[m_RecordingChanId][m_SaveIdx]<0)m_numOf_ConsecutiveNonSelectedWaves_Saved++; 
		else {m_numOfSavedSelectedWaves++; m_numOf_ConsecutiveNonSelectedWaves_Saved=0;}
		ar << m_RecordingChanId;
		ar << furtureUseInt; //For furture use

		if(firstIdx+m_DataPointsPerWave_save<m_AIQueueSize_save){
			for(i=firstIdx, cnt=0;cnt<m_DataPointsPerWave_save;cnt++,i++){
				ar << (int)(1000*pM_AI_save[i]);
			}
		}
		else{
			Length=m_AIQueueSize_save-firstIdx;
			for(i=firstIdx, cnt=0;cnt<Length;cnt++,i++){
				ar << (int)(1000*pM_AI_save[i]);
			}
			Length=m_DataPointsPerWave_save-Length;
			for(i=0;i<Length;i++){
				ar << (int)(1000*pM_AI_save[i]);
			}
		}
	}

	ar.Close();
	theFile.Close();
}

#define BIT_BUILT(Rect, pDC) Rect.left, Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top, pDC, Rect.left, Rect.top,SRCCOPY
//objDC.BitBlt(BIT_BUILT(m_TitleArea,&m_DC[0]));
void CDasView::DisplayStuff(int RequestIdx, CDC *pObjDC)
{		int rowP,clmP;
	CFigure *pFig;
	switch(m_DispEvents[RequestIdx][0]){
	case RESIZING_WIN:
		Batch_DrawFigureFrames();
		Hilight_AllCaptured_Figs();
		DisplayTitle();
		DispFileName();
		break;
	case RENAMING_FIG:
		for(rowP=0;rowP<m_dHowManyRows;rowP++){
			for(clmP=0;clmP<m_dHowManyClmns;clmP++){
				pFig=&m_Fig[rowP][clmP][0];
				pFig->DispNameOfFigure(&m_DC[0]);
				pFig->RefreshNameBox(pObjDC, &m_DC[0]);
		}	}
 		RefreshFrame();
		break;	
	case RESCALING_AXES:
		for(rowP=0;rowP<m_dHowManyRows;rowP++){
			for(clmP=0;clmP<m_dHowManyClmns;clmP++){
				pFig=&m_Fig[rowP][clmP][0];
				if(pFig->m_Captured!=1)continue;
				pFig->ComputeMapping_X_Y_Y2(); 
				pFig->Frame_Graph2D(&m_DC[0],1); 
				pFig->RefreshFrame(pObjDC,&m_DC[0]); 
				Hilight_OneCapturedFig(pFig);
		}	}	
		break;	
	case DISP_FILE_NAME:
		DispFileName();
		break;	
	case DISP_Y_SCALE:
		if(m_pSpotlightFig!=NULL){
			m_pSpotlightFig->Disp_Y_Scale(&m_DC[0]);
			pObjDC->BitBlt(m_pSpotlightFig->m_nTx,m_pSpotlightFig->m_nTy,m_pSpotlightFig->m_FontWidth*3,m_pSpotlightFig->m_nBy-m_pSpotlightFig->m_nTy,&m_DC[0],m_pSpotlightFig->m_nTx,m_pSpotlightFig->m_nTy,SRCCOPY);
		}
		break;	
	case REFRESH_SCREEN:
		pObjDC->BitBlt(0,0,m_savedClientRect.right, m_savedClientRect.bottom,&m_DC[0],0,0,SRCCOPY);
		break;	
	}
}

void CDasView::DispFileName()
{
	m_DC[0].SelectObject(m_NULL);
	m_DC[0].SelectObject(m_GrayBrush);
	m_DC[0].SetBkColor(RGB(220, 220, 220));
	m_DC[0].Rectangle(m_prvFileNameRect.left,m_prvFileNameRect.top,m_prvFileNameRect.right+m_fontWidth*2,m_prvFileNameRect.bottom );
	int left=m_FileNameRect.left-m_fontWidth-m_fontHeight*0.5;
	if(m_isSaving==0){
		m_DC[0].SelectObject(m_GrayBrush); 
	}
	else if(m_isSaving==1){
		m_DC[0].SelectObject(m_RedBrush); 
	}
	m_DC[0].TextOut(m_FileNameRect.left, m_FileNameRect.top,m_JustFileName);
	m_DC[0].Ellipse(left, m_FileNameRect.top+m_fontHeight*0.5-m_fontHeight*0.5,m_FileNameRect.left-m_fontWidth+m_fontHeight*0.5, m_FileNameRect.top+m_fontHeight*0.5+m_fontHeight*0.5);
	CClientDC objDC(this);
	//objDC.BitBlt(left,m_FileNameRect.top,m_FileNameRect.right-left,m_FileNameRect.bottom-m_FileNameRect.top,&m_DC[0],left,m_FileNameRect.top,SRCCOPY);
	objDC.BitBlt(left,m_FileNameRect.top,m_TitleArea.right-left,m_FileNameRect.bottom-m_FileNameRect.top,&m_DC[0],left,m_FileNameRect.top,SRCCOPY);
	m_prvFileNameRect=m_FileNameRect;
}


//Every N ms this will refresh all the attached figures to display the waves.
void CDasView::RefreshFrame()
{
	CAiChannel *pChan;
	CClientDC objDC(this);
	for(int chanID=0;chanID<NUM_NN_AI_CHAN;chanID++){
		pChan=&(m_AiChan[chanID]);
		if(pChan->m_IsThisChanUsed==0 || pChan->pFig==NULL)continue; //If the channel is not attached to any figure, skip.
		pChan->ShowAll_VTBoxes();
		pChan->m_ChDC.SelectObject(&m_NULL);
		pChan->pFig->RefreshPane(&objDC,&pChan->m_ChDC);
		if(m_NumSelectedWaveformsDisplayed[chanID]+m_NumNotSelectedWaveformsDisplayed[chanID]>=m_MaxNumWaveformsToDisp){
			pChan->pFig->ErasePaneBlack(&pChan->m_ChDC);
			m_NumSelectedWaveformsDisplayed[chanID]=0; m_PrvNumSelectedWaveformsDisplayed[chanID]=0;
			m_NumNotSelectedWaveformsDisplayed[chanID]=0;
			if(chanID==*M_SpotlightChanID_forCLAS){*M_NumSelectedWaveformsDisplayed=0;*M_NumNonSelectedWaveformsDisplayed=0;}
			m_WaveIdxToDisp[chanID]=M_usableWaveIdx[chanID]; //Catch up if it hasn't.
		}
	}
}

void CDasView::DrawWave(int WvID, int ChanID)
{
	if(m_AiChan[ChanID].pFig==NULL)return;//In case there is no valid figure to draw the waves, return;
	if(m_AiChan[ChanID].M_lengthOfSignal[WvID]==m_AiChan[ChanID].m_DataPointsPerWave){
		if(m_AiChan[ChanID].M_firstIdxOfWavelet[WvID]+m_AiChan[ChanID].m_DataPointsPerWave>*M_usableAiIdx[ChanID]){
			Sleep((int)M_WaveSpanPost_ms[ChanID]);// Wait for the M_AI[] to catch up.
		}
		if(m_AiChan[ChanID].M_WvEcode[WvID]==-1){
			m_AiChan[ChanID].pFig->DispOneWaveform(&m_AiChan[ChanID].M_AI[m_AiChan[ChanID].M_firstIdxOfWavelet[WvID]],m_AiChan[ChanID].m_DataPointsPerWave, m_AiChan[ChanID].M_WvEcode[WvID], m_AiChan[ChanID].M_AdjustXToXthreshold[WvID],&(m_AiChan[ChanID].m_ChDC));//Continuous waveform along the signal queue 
		}
		else m_AiChan[ChanID].pFig->DispOneWaveform(&m_AiChan[ChanID].M_AI[m_AiChan[ChanID].M_firstIdxOfWavelet[WvID]],m_AiChan[ChanID].m_DataPointsPerWave, m_AiChan[ChanID].M_WvEcode[WvID], m_AiChan[ChanID].M_AdjustXToXthreshold[WvID],&(m_AiChan[ChanID].m_ChDC));//Continuous waveform along the signal queue 
	}
	else DispOneWaveform_FragmentedWave(WvID, ChanID);//Fragmented waveform
}

void CDasView::DispOneWaveform_FragmentedWave(int WaveID, int ChanID)
{
	CFigure *pFig=m_AiChan[ChanID].pFig; 
	CDC *pDC=&(m_AiChan[ChanID].m_ChDC);
	int dataCnt, headPart,tailPart;
	double wf, cnvrt=1000./(*M_SamplingRate);
	double *pW=&(m_AiChan[ChanID].M_AI[m_AiChan[ChanID].M_firstIdxOfWavelet[WaveID]]), maxY=pFig->m_dMaxY, minY=pFig->m_dMinY;
	double bias=cnvrt*m_AiChan[ChanID].M_AdjustXToXthreshold[WaveID]+pFig->m_dMinX;

	if(m_AiChan[ChanID].M_WvEcode[WaveID]==-1)m_DC[0].SelectObject(m_GRAY1);
	else if(m_AiChan[ChanID].M_WvEcode[WaveID]==0)pDC->SelectObject(m_VIOLETLIGHT1);
	else if(m_AiChan[ChanID].M_WvEcode[WaveID]==1)pDC->SelectObject(m_GREEN);
	else if(m_AiChan[ChanID].M_WvEcode[WaveID]==2)pDC->SelectObject(m_CYAN1);
	else 	AfxMessageBox("m_AiChan[ChanID].M_WvEcode[]=? 5");

	wf=*pW; 
	if(wf>maxY)wf=maxY; else if(wf<minY)wf=minY;
	pFig->MyMoveTo(pFig->m_dMinX ,wf, pDC);
	headPart=m_AiChan[ChanID].M_lengthOfSignal[WaveID];
	for(dataCnt=1;dataCnt<headPart;dataCnt++){
		wf=*(pW+dataCnt); if(wf>maxY){wf=maxY;}	else if(wf<minY)wf=minY;
		pFig->MyLineTo(cnvrt*dataCnt+bias,wf, pDC);
	}	
	tailPart=m_AiChan[ChanID].m_DataPointsPerWave-headPart;
	bias=cnvrt*(headPart+m_AiChan[ChanID].M_AdjustXToXthreshold[WaveID])+pFig->m_dMinX;
	pW=&(m_AiChan[ChanID].M_AI[0]);
	for(dataCnt=0;dataCnt<tailPart;dataCnt++){
		wf=wf=*(pW+dataCnt); if(wf>maxY){wf=maxY;}	else if(wf<minY)wf=minY;
		pFig->MyLineTo(cnvrt*dataCnt+bias,wf, pDC);
	}	
}

//int CDasView::qIdx(int i)
//{
//	//if(i>=0 && i<(*M_DataQueueSize))return i;
//	//else if(i>(*M_DataQueueSize)-1)return (i-(*M_DataQueueSize));
//	//else return ((*M_DataQueueSize)+i);//if(i<0) => This happens when the program tries to retrieve the pre-threshold-crossing portion of the wave.
//}

int CDasView::qIdxSpike(int i)
{
	if(i>=0 && i<MAX_SPIKEs) return i;
	else if(i<0) return (MAX_SPIKEs+i);// => This happens when the program tries to retrieve the pre-threshold-crossing portion of the wave.
	else return (i-MAX_SPIKEs);//if(i>MAX_SPIKEs-1)
}


//This SIMULATES the actions of the DAQ device
void CDasView::Show2D_II() 
{	
}
//Signal Gatherer (SIG: Discrimiate the threshold-crossing waves)
void CDasView::Simulated_Callback() 
{
}
//Displayer of the classified signallets (Display those threshold-crossing waves)
void CDasView::Show2D_IV() 
{  
}

void CDasView::Show2D_V() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////

	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}
void CDasView::Show2D_VI() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////

	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_bIsON=0;
}

void CDasView::Show2D_VII() 
{

}

void CDasView::Tics_n_Lines(CFigure *pBrd, int HowManyTicsX, int HowManyTicsY) 
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

void CDasView::Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) 
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
void CDasView::Batch_SetFontSize(CFont *pFont) 
{
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->SetFontSize(pFont);
		}	
	}
}
void CDasView::Batch_Prepare4FigFrame() 
{
	int i,j;
	CFigure *pFig;

	for(i=0;i<m_dHowManyRows;i++){
		for(j=0;j<m_dHowManyClmns;j++){
			pFig=&m_Fig[i][j][0];
			pFig->ComputeMapping_X_Y_Y2();
		}	
	}
}

void CDasView::Batch_DrawFigureFrames() 
{	
	CClientDC objDC(this);
	m_DC[0].SelectObject(m_bkBrush); 
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
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
}


// Writes a string sText with a size which fits perfectly in a rectangle rFont
// Font already attached with device context pDC is scaled
// This does not work for default font of the device contex
void CDasView::FontInRect(CDC *pDC, CString sText, CRect &rFont)
{
	// Gets current font details
	CFont* pFont = pDC->GetCurrentFont(); if(pFont==NULL)return;
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
	CSize Size = pDC->GetTextExtent(sText);//Buggy!! here
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

void CDasView::OnChangeParameters() 
{
	CDialogueBox	ParameterBOX;

	ParameterBOX.m_dVariable1=m_dMaximumX;
	ParameterBOX.m_dVariable2=m_dMinimumX;
	ParameterBOX.m_dVariable3=m_dMaximumY;
	ParameterBOX.m_dMinimumY=m_dMinimumY;
	if(ParameterBOX.DoModal()==IDOK){
		if(ParameterBOX.m_dVariable1<=ParameterBOX.m_dVariable2||ParameterBOX.m_dVariable3<=ParameterBOX.m_dMinimumY){AfxMessageBox("m_dMaximumX<=m_dMinimumX OR m_dMaximumY<=m_dMinimumY ==> Try again", MB_OK); return;}
		else if(ParameterBOX.m_dVariable1<0||ParameterBOX.m_dVariable2>0){AfxMessageBox("m_dMaximumX<0||m_dMinimumX>0 ==> Try again", MB_OK); return;}
		else{
			m_dMaximumX=ParameterBOX.m_dVariable1;
			m_dMinimumX=ParameterBOX.m_dVariable2;
			m_dMaximumY=ParameterBOX.m_dVariable3;
			m_dMinimumY=ParameterBOX.m_dMinimumY;
		}
	}
	m_dontDoRubberbending=1;
}

void CDasView::OnIsFromHighToLow()
{
	for(int chanIdx=0; chanIdx<NUM_NN_AI_CHAN; chanIdx++){
		if(m_AiChan[chanIdx].m_IsThisChanUsed==0)continue;//A not used channel
		if(m_AiChan[chanIdx].pFig->m_Captured==1){
			m_AiChan[chanIdx].m_HighToLowCrossing=!m_AiChan[chanIdx].m_HighToLowCrossing;
			//When the infomation of "client's" configuration changes, it updates on the server side
			m_AiChan[chanIdx].UpdateServerWithVTBoxes();
		}
	}
}

void CDasView::OnUpdateIsPD(CCmdUI *pCmdUI)
{
}

void CDasView::OnSelect0()
{
	m_SelColor=0;
}
void CDasView::OnSelect1()
{
	m_SelColor=1;
}
void CDasView::OnSelect2()
{
	m_SelColor=2;
}
void CDasView::OnUnselect1()
{
	m_SelColor=-1;
}
void CDasView::OnUpdateSelect0(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_SelColor==0);
}
void CDasView::OnUpdateSelect1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_SelColor==1);
}
void CDasView::OnUpdateSelect2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_SelColor==2);
}
void CDasView::OnUpdateUnselect1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_SelColor==-1);
}

void CDasView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDasView::OnEnChangeEditString1()
{
	StringEdit	ParameterBOX;

	if(m_CallingFrom==1)ParameterBOX.m_UserInputString1=m_GlobalTitle;
	else if(m_CallingFrom==2)ParameterBOX.m_UserInputString1=m_pSpotlightFig->m_AssociatedChan.NameString;
	else if(m_CallingFrom==3){
		char buff[256]; sprintf(buff,"Recording Channel: %d",m_RecordingChanId);
		ParameterBOX.m_UserInputString1=buff;
	}
	else if(m_CallingFrom==4){
		char buff[256]; sprintf(buff,"Stop after N waves: %d",m_StopAfterSavingNwaves);
		ParameterBOX.m_UserInputString1=buff;
	}
	if(ParameterBOX.DoModal()==IDOK){
		if(m_CallingFrom==1)m_GlobalTitle=ParameterBOX.m_UserInputString1;
		else if(m_CallingFrom==2){
			char ChanLabel[256];
			strcpy(ChanLabel,ParameterBOX.m_UserInputString1);
			m_EnteredChanID=Find_Last_Num_inString(ChanLabel);
			if(m_EnteredChanID>=0 && m_EnteredChanID<NUM_NN_AI_CHAN){
				sprintf(ChanLabel,"Channel: %d",m_EnteredChanID);
				m_pSpotlightFig->m_AssociatedChan.NameString=ChanLabel;
				//If the user chages the channel of a soly displayed pannel, assume that this is the recording channel that the user wants.
				if(m_dHowManyClmns==1 && m_dHowManyRows==1)m_RecordingChanId=(int)m_EnteredChanID;
			}
			else{
				m_pSpotlightFig->m_AssociatedChan.NameString="No Chan   ";
			}
		}
		else if(m_CallingFrom==3){
			char ChanLabel[256];
			strcpy(ChanLabel,ParameterBOX.m_UserInputString1);
			m_EnteredChanID=Find_Last_Num_inString(ChanLabel);
			if(m_EnteredChanID>=0 && m_EnteredChanID<NUM_NN_AI_CHAN)m_RecordingChanId=(int)m_EnteredChanID;
		}
		else if(m_CallingFrom==4){
			char AfterN[256];
			strcpy(AfterN,ParameterBOX.m_UserInputString1);
			m_EnteredChanID=Find_Last_Num_inString(AfterN);
			if(m_EnteredChanID>=0 && m_EnteredChanID<INT_MAX)m_StopAfterSavingNwaves=(int)m_EnteredChanID;
			else AfxMessageBox("The number is too big or negative");
		}
	}
	m_dontDoRubberbending=1;
}


void CDasView::EditPrefixSuffix()
{
	CPrefixSuffix	PrefixSuffix;
	char TmpStr[256];
	sprintf(TmpStr,"Prefix: %s",m_PreFix);
	PrefixSuffix.m_PrefixSuffix=TmpStr;
	if(PrefixSuffix.DoModal()==IDOK){
		PrefixSuffix.m_PrefixSuffix.Replace("Prefix:",""); PrefixSuffix.m_PrefixSuffix.TrimLeft();
		strcpy(m_PreFix,PrefixSuffix.m_PrefixSuffix);
	}
	sprintf(TmpStr,"Suffix: %s",m_SuFFix);
	PrefixSuffix.m_PrefixSuffix=TmpStr;
	if(PrefixSuffix.DoModal()==IDOK){
		PrefixSuffix.m_PrefixSuffix.Replace("Suffix:",""); PrefixSuffix.m_PrefixSuffix.TrimLeft();
		strcpy(m_SuFFix,PrefixSuffix.m_PrefixSuffix);
	}
	sprintf(TmpStr,"Neuron #: %d",*M_NnNumberToRecord);
	PrefixSuffix.m_PrefixSuffix=TmpStr;
	if(PrefixSuffix.DoModal()==IDOK){
		int TmpID;
		strcpy(TmpStr,PrefixSuffix.m_PrefixSuffix);
		TmpID=Find_Last_Num_inString(TmpStr);
		if(TmpID!=INT_MAX){
			*M_NnNumberToRecord=TmpID;
		}
	}
	Make_TheBaseOfFileName();
}

void CDasView::OnFileClose()
{
	m_isSaving=0;
	RemeasureFileNameRect();
	RequestDisplayEvent(DISP_FILE_NAME);
}

void CDasView::Make_TheBaseOfFileName()
{
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;  // Get the current time from the operating system.
	CTime CurrTime( osBinaryTime ); //  CTime from C run-time time
	m_Date = CurrTime.Format( "%m%d%y" );
	strcpy(m_BaseOfFileName,m_PreFix);
	strcat(m_BaseOfFileName,m_Date);
	//strcat(m_BaseOfFileName,"_");
	strcat(m_BaseOfFileName,m_SuFFix);
	if(m_FlagVirMemDC==1){//"if(m_FlagVirMemDC==0)" prevents the program trying to access this part before going through "VirMemDC()"
		m_PrvNnNumberToRecord=*M_NnNumberToRecord;
		char NnStr[10]; itoa(*M_NnNumberToRecord,NnStr,10);
		sprintf(m_JustFileName,"%s%s.wv",m_BaseOfFileName,NnStr);
	}

	RemeasureFileNameRect();
}

void CDasView::RequestDisplayEvent(int Event) 
{
	RequestDisplayEvent(Event, 0, NULL,  NULL);
}
void CDasView::RequestDisplayEvent(int Event,  int SomeIntValue) 
{
	RequestDisplayEvent(Event, 1, SomeIntValue,  NULL); 
}
void CDasView::RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2) 
{
	RequestDisplayEvent(Event, 2, SomeIntValue1, SomeIntValue2);
}
void CDasView::RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2) 
{	
	m_DispEvents[m_DispEventIdx][0]=Event;
	m_DispEvents[m_DispEventIdx][1]=NumVariables;
	for(int i=0; i<NumVariables;i++){
		if(i==0)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue1; 
		else if(i==1)m_DispEvents[m_DispEventIdx][i+2]=SomeIntValue2; 
	}
	if(m_DispEventIdx+1<MAX_DISP_REQUESTS)m_DispEventIdx++; else m_DispEventIdx=0;
}


void CDasView::RemeasureFileNameRect()
{
	m_FileNameRect.left=m_savedClientRect.left+m_fontWidth+m_fontHeight;
	m_FileNameRect.right=m_FileNameRect.left+m_fontWidth*strlen(m_JustFileName);
	m_FileNameRect.top=(int)(m_TitleArea.top+m_fontHeight*0.2); m_FileNameRect.bottom=m_FileNameRect.top+m_fontHeight;
}


void CDasView::OnAdjustScaleToFitData()
{
	if(*M_isDAS_Running==0){AfxMessageBox("DAS server is not running.", MB_OK);return;}
	int clm,row, MaxSize=4000, dataSize, dataStart,dataEnd,i, chanID, NumCaptured=0;
	double minY=INT_MAX, maxY=-INT_MAX, *pM_AI;
	CFigure *pFig;
	CClientDC objDC(this);
	CArray<double, double& >RawData; 
	RawData.SetSize(MaxSize);
	if(m_numCaptured==0){
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				if((pFig->m_ChanID>=0 && pFig->m_ChanID<NUM_NN_AI_CHAN) && pFig->pChan!=NULL || pFig->pChan->m_IsThisChanUsed==1){
					pFig->m_Captured=1; m_numCaptured++;//In case there is nothing selected do it for every figure.
				}
			}
		}
	}

	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Captured==1){
				if((pFig->m_ChanID>=0 && pFig->m_ChanID<NUM_NN_AI_CHAN) && pFig->pChan!=NULL){
					if(pFig->pChan->m_IsThisChanUsed==0)continue; 
					chanID=pFig->pChan->m_ChanID;
					dataSize=pFig->pChan->m_DataPointsPerWave;
					if(MaxSize<dataSize){MaxSize=dataSize;RawData.SetSize(MaxSize);}
					dataEnd=*pFig->pChan->M_usableAiIdx;
					dataStart=dataEnd-dataSize;
					pM_AI=&(M_AI[chanID][0]);
					if(dataStart>=0){
						for(i=dataStart;i<dataEnd;i++){
							if(pM_AI[i]<minY)minY=pM_AI[i];
							if(pM_AI[i]>maxY)maxY=pM_AI[i];
						}
					}
					else{
						dataStart=pFig->pChan->qIdx(dataEnd-dataSize); if(dataStart<=dataEnd)AfxMessageBox("What does this mean? The size of the display is bigger than the queue size of M_AI[]?");
						int sizeOfQ=M_AIQueueSize[chanID];
						for(i=dataStart;i<sizeOfQ;i++){
							if(pM_AI[i]<minY)minY=pM_AI[i];
							if(pM_AI[i]>maxY)maxY=pM_AI[i];
						}
						for(i=dataStart;i<dataEnd;i++){
							if(pM_AI[i]<minY)minY=pM_AI[i];
							if(pM_AI[i]>maxY)maxY=pM_AI[i];
						}
					}
					minY*=2; maxY*=2; //This is to make the scale fit loosly
					AdjustScale(minY,maxY); //Just by seeing the min, max, it figures out the best scale.
					pFig->RescaleAxes(pFig->m_dMinX,pFig->m_dMaxX,minY,maxY);
				}
			}
		}
	}
	RequestDisplayEvent(RESCALING_AXES);  //Send a message to redraw
	RawData.RemoveAll();
}


void CDasView::OnDisplayAcquisionDisruption()
{
	m_DisplayAcquisionDisruption=!m_DisplayAcquisionDisruption;
}


void CDasView::OnUpdateDisplayAcquisionDisruption(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DisplayAcquisionDisruption==1);
}


void CDasView::On_FlipSignal()
{
	*M_Flip_AI_SignalSign=!(*M_Flip_AI_SignalSign);
}


void CDasView::On_UpdateViewFlipSignal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(*M_Flip_AI_SignalSign==-1);
}
