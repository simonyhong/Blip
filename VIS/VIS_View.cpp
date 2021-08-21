// VISView.cpp : implementation of the CVISView class
//

#include "stdafx.h"
#include "VIS.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "VIS_Doc.h"
#include "Figures.h"
#include "SharedConstants.h"
#include "VIS_View.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "Mmsystem.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CVISView::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CVISView

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CVISView, CView)

BEGIN_MESSAGE_MAP(CVISView, CView)
	//{{AFX_MSG_MAP(CVISView)
	ON_COMMAND(ID_3DGraph, On3DGraph)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_2DGraph, On2DGraph_I)
	ON_UPDATE_COMMAND_UI(ID_3DGraph, OnUpdate3DGraph)
	ON_COMMAND(ID_CLEAR_SCREEN, OnClearScreen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_CALIBRATE_SCREEN, &CVISView::OnCalibrateScreen)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SMALLER_PANNELS, &CVISView::OnSmallerPanels)
	ON_COMMAND(ID_BIGGER_PANNELS, &CVISView::OnBiggerPanels)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_FILE_SAVE, &CVISView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CVISView::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CVISView construction/destruction
CVISView::CVISView()
{
	// TODO: add construction code here
	CRect rec(605,0,1300,700);
	m_rectShow3D=rec;
	m_nColorShow=BLACK_BRUSH;
	m_bFistTime=1;
	m_nKey=0;
	FlagVerScreen=0;
	m_bIsON=0;
	m_IsMemMap=0;
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
		j=(MaxRGB100-1)-i*(MaxRGB100/MaxPen);
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;
	LogBrh.lbColor=RGB(0,0,0);
	m_backgroundBrush.CreateBrushIndirect( &LogBrh );			
	m_backgroundScreenPen.CreatePen(PS_SOLID, 1,LogBrh.lbColor);

	/////////////// Parameters For Applications
	m_PrevTicks=GetTickCount();
	m_NewlyLoaded=0; 
	m_dMinimumX=-1000;m_dMaximumX=1000;
	m_dMinimumY=0;m_dMaximumY=100;
	m_savedClientRect.top=0;m_savedClientRect.bottom=0;m_savedClientRect.left=0;m_savedClientRect.right=0;
	for(i=0;i<MAX_STIM_OBJ;i++)m_pPicture[i]=NULL;
	for(i=0;i<MAX_STIM_OBJ;i++)strcpy(m_SavedPicFileName[i],"");
	
	m_StartTime=-1;
	m_FirstTime0ElementErr=1;
}					


CVISView::~CVISView() 
{
	m_bIsON=0;
	CloseMemoryMAP_4TASMOSVIS();
	if(m_IsMemMap==1){//In the case MOXY has not declared memory map, there is nothing to close.
		CloseSharedMemoryArea();
	}
	
	for(int i=0;i<MAX_STIM_OBJ;i++)	if(m_pPicture[i])m_pPicture[i]->Release();

	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	

	int i;
	for(i=0; i<MaxDC;i++){
		m_DC[i].DeleteDC();
		m_bmp[i].DeleteObject();
	}
	m_backgroundBrush.DeleteObject();
	m_backgroundScreenPen.DeleteObject();
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

BOOL CVISView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

UINT Graph2D_I(LPVOID WinObjPtr) //A global Function
{
	CVISView* pView=(CVISView*) WinObjPtr;
	pView->VIsual_Stimuli_generator();
	if(*pView->M_CLOSE_VIS==1){
		AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CVISView drawing

void CVISView::OnDraw(CDC* pDC)
{
#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CVISDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(ThD.m_nShowMode==GRAPH_3D){
		pDC->SelectStockObject(WHITE_BRUSH);//m_nColorShow);
		pDC->Rectangle(m_rectShow3D);
	}
	if(m_bFistTime==1){
		VirMemDC();
		Re_measure_ClientView();
		OnFileOpen();//Try to open a default root file.
		if(m_FileOpenSucceeded!=1){//Failed to open a default root file => implement the programs own version.
			sprintf(m_FileName,"");
			sprintf(m_JustFileName,"NO root file");
		}
		AfxBeginThread( Graph2D_I, this);
		m_bFistTime=0;
	}
	else{
		CRect rt;
		GetClientRect(&rt);
		if(m_savedClientRect.right!=rt.right||m_savedClientRect.bottom!=rt.bottom){//Specifies the window's coordinates when the window is in the normal (restored) position. 
			m_savedClientRect=rt;
		}
		PvisAllOff_implement();
	}
}

void CVISView::Re_measure_ClientView()
{			
	CRect rt;
	GetClientRect(&rt);
	if(m_bFistTime==1){
		CFont* pFont = m_DC[0].GetCurrentFont();
		LOGFONT LogFont;
		pFont->GetLogFont(&LogFont);
		m_fontHeight=LogFont.lfHeight;
	}
	m_savedClientRect=rt;
}


/////////////////////////////////////////////////////////////////////////////
// CVISView printing

BOOL CVISView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVISView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVISView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVISView diagnostics

#ifdef _DEBUG
void CVISView::AssertValid() const
{
	CView::AssertValid();
}

void CVISView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVISDoc* CVISView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVISDoc)));
	return (CVISDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVISView message handlers


void CVISView::On2DGraph_I() 
{
		AfxBeginThread( Graph2D_I, this);
}
void CVISView::VirMemDC()
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

//############   VIS  (VIsual Stimuli generator)  ############
void CVISView::VIsual_Stimuli_generator() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CRect rt; GetClientRect(&rt); m_savedClientRect=rt;//This takes care of the problem where the difference of client area has not been updated, at the start of this program.

	FigPosition_Settings();
	SetFontSize(&m_Figure);
	m_Figure.ComputeMappingXY();
	m_Figure.Frame_Graph2D(&m_DC[0]);


	// Shared memory area for SIG (DAS)
	CreateSharedMemoryArea();
	// creates the area to be shared
	CreateSharedMemoryArea_4TASVIS();
	//*M_MsgID_fromTAS_toVIS=0; <=Do not put this before the while loop!!
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS); // REALTIME_PRIORITY_CLASS (may make the Windows freeze), HIGH_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS 
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);// THREAD_PRIORITY_TIME_CRITICAL  THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_NORMAL  THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_LOWEST  THREAD_PRIORITY_IDLE 
	
	m_IsMemMap=1; //now the memory map has been declared.
	// clear screen, reset
	PvisAllOff_implement();
	*M_Is_VIS_Running=1;

	timeBeginPeriod(1);//Increase the time accuracy 
	//Receive messages
	while(1){
		if(*M_MsgID_fromTAS_toVIS!=0){
			WhichFunction(*M_MsgID_fromTAS_toVIS);
			*M_MsgID_fromTAS_toVIS=0;
		}
		if(*M_Msg_from_MOXY_to_VIS!=0){
			Which_MOXY_Function();
		}		
		if(m_bIsON==0|| *M_CLOSE_VIS==1)break;

		//Turn off the square after "DURATION_OF_WHITE_BOX ms" of its appearance.
		#define DURATION_OF_WHITE_BOX 60
		if(m_StartTime>0 && clock()-m_StartTime> DURATION_OF_WHITE_BOX){
			m_DC[0].FillSolidRect(*M_PhotoDiodeFlash_PosX,*M_PhotoDiodeFlash_PosY,*M_PhotoDiodeFlash_PosWidth,*M_PhotoDiodeFlash_PosHeight,RGB(0,0,0)); 
			objDC.BitBlt(*M_PhotoDiodeFlash_PosX,*M_PhotoDiodeFlash_PosY,*M_PhotoDiodeFlash_PosWidth,*M_PhotoDiodeFlash_PosHeight,&m_DC[0],0,0,SRCCOPY);
			m_StartTime=-1;
		}

		Sleep(1);
	}
	timeEndPeriod(1);//match this with 	timeBeginPeriod(1) above 
	//*M_MsgID_fromTAS_toVIS=0;
	*M_Is_VIS_Running=0;

	/////////////////////
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].RestoreDC(nSaveDC); 
	m_bIsON=0;		
}


//For VIS to display the "GRID" of degrees.
void CVISView::OnCalibrateScreen() 
{	
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();

	CFigure *pFig=&m_Figure;
	m_DC[0].SelectObject(&m_backgroundBrush);
	m_Figure.m_normTx=m_savedClientRect.left;  m_Figure.m_normTy=m_savedClientRect.top;   
	m_Figure.m_normBx=m_savedClientRect.right;  m_Figure.m_normBy=m_savedClientRect.bottom; 
	m_Figure.m_LateralMargin=0;
	m_Figure.Set_MinMax_n_Labels_XY("","",-m_ScreenSize_inDegrees_4VIS,m_ScreenSize_inDegrees_4VIS,-m_ScreenSize_inDegrees_4VIS,m_ScreenSize_inDegrees_4VIS,xFRAME);		
	m_Figure.Frame_Graph2D(&m_DC[0]);

	//Rectangles at every 10 degrees
	for(double i=10; i<=m_ScreenSize_inDegrees_4VIS;i+=10){
		m_DC[0].SelectObject(&m_rgbPEN1[(int)((MaxPen-1)*(i/m_ScreenSize_inDegrees_4VIS))]);
		m_Figure.Graph2D(-i,m_Figure.m_dMaxY,-i,m_Figure.m_dMinY,2,&m_DC[0]); m_Figure.PutIntVal(-i,MsgPosY(pFig,0.3),-i,&m_DC[0]);
		m_Figure.Graph2D(i,m_Figure.m_dMaxY,i,m_Figure.m_dMinY,2,&m_DC[0]);   m_Figure.PutIntVal( i,MsgPosY(pFig,0.3), i,&m_DC[0]);
		m_Figure.Graph2D(m_Figure.m_dMinX,-i,m_Figure.m_dMaxX,-i,2,&m_DC[0]); m_Figure.PutIntVal(MsgPosX(pFig,0.3),-i,-i,&m_DC[0]);
		m_Figure.Graph2D(m_Figure.m_dMinX,i,m_Figure.m_dMaxX,i,2,&m_DC[0]);   m_Figure.PutIntVal(MsgPosX(pFig,0.3), i,-i,&m_DC[0]);
	}
	//Diagonal lines
	m_DC[0].SelectStockObject(WHITE_PEN);
	m_Figure.Graph2D(m_Figure.m_dMinX,m_Figure.m_dMaxY,m_Figure.m_dMaxX,m_Figure.m_dMinY,2,&m_DC[0]); 
	m_Figure.Graph2D(m_Figure.m_dMinX,m_Figure.m_dMinY,m_Figure.m_dMaxX,m_Figure.m_dMaxY,2,&m_DC[0]); 
	//Cross
	m_Figure.Graph2D((m_Figure.m_dMinX+m_Figure.m_dMaxX)*0.5,m_Figure.m_dMaxY,(m_Figure.m_dMinX+m_Figure.m_dMaxX)*0.5,m_Figure.m_dMinY,2,&m_DC[0]); 
	m_Figure.Graph2D(m_Figure.m_dMinX,(m_Figure.m_dMinY+m_Figure.m_dMaxY)*0.5,m_Figure.m_dMaxX,(m_Figure.m_dMinY+m_Figure.m_dMaxY)*0.5,2,&m_DC[0]); 
	objDC.BitBlt(m_savedClientRect.left,m_savedClientRect.top,m_savedClientRect.right-m_savedClientRect.left,m_savedClientRect.bottom-m_savedClientRect.top,&m_DC[0],m_savedClientRect.left,m_savedClientRect.top,SRCCOPY);

	/////////////////////
	m_DC[0].RestoreDC(nSaveDC); 
}

void CVISView::FigPosition_Settings() 
{
	CFigure *pFig;

	m_Figure.m_normTx=m_savedClientRect.left;  
	m_Figure.m_normTy=m_savedClientRect.top;   
	m_Figure.m_normBx=m_savedClientRect.right;  
	m_Figure.m_normBy=m_savedClientRect.bottom; 
	m_Figure. m_LateralMargin=0;
	m_Figure.SetFig_Position_in_ClientView(m_Figure.m_normTx,m_Figure.m_normTy,m_Figure.m_normBx,m_Figure.m_normBy);
}

// Writes a string sText with a size which fits perfectly in a rectangle rFont
// Font already attached with device context pDC is scaled
// This does not work for default font of the device contex
void CVISView::FontInRect(CDC *pDC, CString sText, CRect &rFont)
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


void CVISView::OnClearScreen() 
{
	PvisAllOff_implement();
}


void CVISView::OnBiggerPanels()
{
	m_ScreenSize_inDegrees_4VIS+=1;
	OnCalibrateScreen();
}

void CVISView::OnSmallerPanels()
{
	if(m_ScreenSize_inDegrees_4VIS>5)m_ScreenSize_inDegrees_4VIS-=1;
	OnCalibrateScreen();
}

void CVISView::SetFontSize(CFigure* pFig)
{
	// Get the sizes of the font
	//CFont* pFont = m_DC[0].GetCurrentFont(); This SOMETIMES returns an incorrect pointer.
	LOGFONT LogFont;
	m_font.GetLogFont(&LogFont);
	pFig->m_FontWidth=LogFont.lfWidth;
	pFig->m_FontHeight=LogFont.lfHeight;
}

void CVISView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

