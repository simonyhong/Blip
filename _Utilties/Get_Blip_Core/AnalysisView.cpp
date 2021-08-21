// AnalysisView.cpp : implementation of the CAnalysisView class
//

#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"

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
	ON_COMMAND(ID_2DGraph, On2DGraph_I)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_FILE_SAVE, &CAnalysisView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CAnalysisView::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CAnalysisView construction/destruction
CAnalysisView::CAnalysisView()
{
	m_bFistTime=1;
	m_nKey=0;
	m_bIsON=0;
}					


CAnalysisView::~CAnalysisView() 
{
	m_bIsON=0;    //In case the thread uses a switch (m_bIsON)
}

BOOL CAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAnalysisView drawing
DWORD WINAPI  Graph2D_I(LPVOID WinObjPtr);

void CAnalysisView::OnDraw(CDC* pDC)
{
	CAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1){
		m_hThread=CreateThread(NULL,0,Graph2D_I,this,0,NULL);
		m_bFistTime=0;
	}
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
	pView->Show2D_I() ;
	return 0;
}

void CAnalysisView::On2DGraph_I() 
{
	    m_hEvStopMainThread=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,NULL/*use name?*/);
		m_hThread=CreateThread(NULL,0,Graph2D_I,this,0,NULL);
		//AfxBeginThread( Graph2D_I, this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

//########################   File Copier  #########################
//########################   File Copier  #########################
void CAnalysisView::Show2D_I() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	CClientDC objDC(this);

	objDC.TextOut(10, 10,"Select a folder to copy.");
	objDC.TextOut(10, 30,"Some files, such as .ncb, will be excluded.");
	objDC.TextOut(10, 50,"This program can be used to rip the core of any program."); //This is because this program uses an "exclusion" method.

	
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Pick a Directory to copy.");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        // get the name of the folder
        SHGetPathFromIDList ( pidl, m_FolderPath );
        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
	else return;

	GetFolderName(m_FolderPath,m_JustFolderName);

	//###### Put the extension of the files that you would like to exclude, below.
	//Additionally, some files and folders (e.g. TAS_Mine folder, and files in _VIS_Figures) are excluded by default: see FindAllFiles() for details
	m_NumExts=0;
	sprintf(m_MatchExtStr[m_NumExts],"*.ncb"); m_NumExts++; //intellisense database.
	sprintf(m_MatchExtStr[m_NumExts],"*.sdf"); m_NumExts++; //intellisense database.
	sprintf(m_MatchExtStr[m_NumExts],"*.sbr"); m_NumExts++; 
	sprintf(m_MatchExtStr[m_NumExts],"*.suo"); m_NumExts++; 
	sprintf(m_MatchExtStr[m_NumExts],"*.user"); m_NumExts++; 
	sprintf(m_MatchExtStr[m_NumExts],"*.ipch"); m_NumExts++; 
	sprintf(m_MatchExtStr[m_NumExts],"*Zapper.ppt"); m_NumExts++; 
	//sprintf(m_MatchExtStr[m_NumExts],"End_Users_Definitions.h"); m_NumExts++; 
	//#####################################################################

	//Make the name of the New_BLIP folder
	char Final_Message[1028],TmpDir[1028];
	int result, errorCode, isSucceed;
	strcpy(TmpDir, m_FolderPath);
	RemoveForwardSlashAtEnd(TmpDir);
	GetGrandParentFolderName(TmpDir, m_NewBlip_FolderPath);AddFrwdSlashAtEndIfNotHave(m_NewBlip_FolderPath); sprintf(m_NewBlip_FolderPath,"%sRename_me_to_%s\\",m_NewBlip_FolderPath,m_JustFolderName);
	result=CreateDirectory(m_NewBlip_FolderPath,NULL); //Creates the new folder if doesn't exist already.
	if(result==0){
		errorCode=GetLastError();
		if(errorCode==ERROR_ALREADY_EXISTS)isSucceed=1;
		else isSucceed=0;
	}
	else {isSucceed=1;}
	if(isSucceed==1)sprintf(Final_Message,"Done!!\nThe designated files in folder \"%s\" have been copied to folder \"%s\" .",m_FolderPath, m_NewBlip_FolderPath);
	else{ sprintf(Final_Message,"Failed to create New_%s folder.",m_JustFolderName); return;}

	//Message
	objDC.TextOut(10, 10,"                                                                                ");
	objDC.TextOut(10, 30,"                                                                                ");
	objDC.TextOut(10, 50,"                                                                                                             ");
	objDC.TextOut(10, 10,"Copying in progress...");

	m_isToCopyExe=1;//By default *.exe will not be deleted.
	if(Confirm_Deletion()==1)FindAllFiles(m_NewBlip_FolderPath, m_FolderPath, 1);

	//Finish
	m_bIsON=0;
	objDC.TextOut(10, 10,"                                                                                ");
	objDC.TextOut(10, 10,"Done!");
	AfxMessageBox(Final_Message);
}
//#####################################################################
//#####################################################################

