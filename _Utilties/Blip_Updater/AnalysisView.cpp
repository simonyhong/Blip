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
#include "Dialog_ChooseSourceDir.h"

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

void CAnalysisView::Show2D_I() 
{
	//###################################  The programmer needs to define the following  ###############################################
	//###################################                                                ###############################################
	//(1) List folders and files to REPLACE, below.
	//(2) List files (among the listed folders to delete) that should not be touched.
	//(3) Make sure the indexes of m_CheckedToReplace[] in Confirm_Deletion() match the corresponding variables of the dialog. This is for the case the user selects which one needs to update others not.
	//(4) Done!!
	//For the BLIP update, “End_Users_Definitions.h” file is excluded from being copied to the user’s existing BLIP. 

	m_TotItems=0;
	m_NumFoldersToReplace=0;
	m_NumFilesNotToTouch=0;
	Give_BlipsChildFolderName("_Include");		// 1
	Give_BlipsChildFolderName("_Utilities");	// 2
	Give_BlipsChildFolderName("ANTS");			// 3
	Give_BlipsChildFolderName("CLAS");			// 4
	Give_BlipsChildFolderName("DAS_cl");		// 5
	Give_BlipsChildFolderName("DAS_sv");		// 6
	Give_BlipsChildFolderName("MOXY");			// 7
	Give_BlipsChildFolderName("Oscilloscope");	// 8
	Give_BlipsChildFolderName("RAS");			// 9
	Give_BlipsChildFolderName("VIS");			// 10
	Give_BlipsChildFolderName("XCOR");			// 11
	Give_BlipsChildFolderName_GrandChildFolderName_ToReplace("TAS","Update_Me");// 12

	//Files NOT TO TOUCH
	Give_BlipsChildFolderName_FileName_NotToTouch("_Include", "End_Users_Definitions.h");
	Give_BlipsChildFolderName_FileName_NotToTouch("_Include", "SharedConstants.h"); //This existing file will be renamed to "Previous_SharedConstants.h"

	//Single files
	m_NumFilesToReplace=0;
	Give_BlipsChildFolderName_FileName_ToReplace("TAS","Update_Me_TAS_Doc.cpp");// 13
	Give_BlipsChildFolderName_FileName_ToReplace("TAS","Update_Me_TAS_Doc.h");	// 14
	Give_BlipsChild_FileName_ToReplace("Blip_Guide.pptx");						// 15
	Give_BlipsChild_FileName_ToReplace("Release_Note.txt");						// 16
	Give_BlipsChild_FileName_ToReplace("Blip_Launcher.exe");					// 17




	//Single file: Copy to the user's folder without concent: there is no related action at Confirm_Deletion(). 
	//Give_BlipsChildFolderName_FileName_ToReplace("_Config_Files","Config_XCOR.txt");	Set_Check_status(16, 1); // 16
	//############################################################################################################################
	//############################################################################################################################


	CClientDC objDC(this);
	//########  Destination Folder (i.e., OLD Blip) ######
	objDC.TextOut(10, 10,"Select your OLD \"Blip\" folder to update.");
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Select your OLD \"Blip\" folder to update.");
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if( pidl != 0 ){
        // get the name of the folder
        SHGetPathFromIDList ( pidl, m_FolderPath );
        // free memory used
        IMalloc * imalloc = 0;
        if( SUCCEEDED( SHGetMalloc ( &imalloc )) ){
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
	else{
		objDC.TextOut(10, 10,"Nothing happened.                                                  ");
		return;
	}

	GetLastFolderName(m_FolderPath,m_JustFolderName);
	if(strcmp(m_JustFolderName,"Blip")!=0){
		if(Confirm_Directory()==0){
			objDC.TextOut(10, 10,"Nothing happened.                                                   ");
			return;
		}
	}

	//########  Find the candidate of the Source (i.e., new Blip) folders and files.   #########################
	int found=0;
	char tmpStr[1024], NewBlipFolder[1024], LastFolderName[256];
	CString St=	GetExecutableName();
	strcpy(tmpStr,St);
	// (1) See if the PARENT folder of the "Update_existing_Blip.exe" file contains Blip. The most likely case after downloading the Blip package.
	GetGrandParentFolderName(tmpStr,NewBlipFolder);
	GetLastFolderName(NewBlipFolder,LastFolderName);
	if(strcmp("New_Blip",LastFolderName)==0){
		found=IsThere_FolderA_inFolderB("Blip",NewBlipFolder,0);
		if(found==1){// "*New_Blip\\Blip" has been found
			AddFrwdSlashAtEndIfNotHave(NewBlipFolder);
			strcat(NewBlipFolder,"Blip");
		}
	}
	// (2) See if the parent folder of the "Updater" folder contains Blip. This happens when "Update_existing_Blip.exe" is running inside of the VC++ project in the Debugging mode.
	else{
		strcpy(tmpStr,St);
		if(wildcmp("*\\Blip_Updater*",tmpStr)==1 && FromA_findB_addC_giveD(tmpStr, "Blip_Updater", "", NewBlipFolder)==1){
			GetGrandParentFolderName(NewBlipFolder,tmpStr);GetGrandParentFolderName(tmpStr,tmpStr);GetGrandParentFolderName(tmpStr,tmpStr);
			found=IsThere_FolderA_inFolderB("Blip",tmpStr,0);
			if(found==1){
				strcpy(NewBlipFolder,tmpStr);
				strcat(NewBlipFolder,"\\Blip");
			}
		}
	}
	
	int isManual=1;//Manual 
	//If a candidate is found, see if the user wants to use it.
	if(found==1){
		CChooseSourceDir SourceDir;
		SourceDir.m_CandidateDirectory=NewBlipFolder;
		if(SourceDir.DoModal()==IDOK){
			isManual=SourceDir.m_UseCandidateDirectory;
		}
		else{
			objDC.TextOut(10, 10,"Nothing happened.                                                   ");
			return;
		}
	}
	// Either failed to find a candidate new Blip. OR, the user opted to choose the new Blip manually.
	if(isManual==1){
		objDC.TextOut(10, 10,"Select the NEW \"Blip\" folder.                     ");
		BROWSEINFO bi = { 0 };
		bi.lpszTitle = _T("Select the NEW \"Blip\" folder.");
		LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
		if( pidl != 0 ){
			// get the name of the folder
			SHGetPathFromIDList ( pidl, NewBlipFolder );
			// free memory used
			IMalloc * imalloc = 0;
			if( SUCCEEDED( SHGetMalloc ( &imalloc )) ){
				imalloc->Free ( pidl );
				imalloc->Release ( );
			}
		}
		else{
			objDC.TextOut(10, 10,"Nothing happened.                                     ");
			return;
		}
	}

	//#########################  Confirmation Dialog  ##############################
	if(Confirm_Deletion(NewBlipFolder,m_FolderPath)==0){
		objDC.TextOut(10, 10,"Nothing happened.                                                   ");
		return;
	}

	//##########  First, save the "not-to-touch" files to a "temp" forlder ##########
	char tmpStr2[2048],renamedtmp[2048],tmpSaveFolder[2048];
	if(m_NumFilesNotToTouch>0){
		sprintf(tmpSaveFolder,"%s\\__tmp",m_FolderPath);
		CreateFolder_IfNotAlreayThere(tmpSaveFolder);
		for(int i=0; i<m_NumFilesNotToTouch; i++){
			if(m_FileNotToTouch[i].CheckedToReplace==0)continue; //The user chose not to replace
			if(m_FileNotToTouch[i].IsThereParentFolder==1){
				sprintf(tmpStr,"%s\\%s\\%s",m_FolderPath,m_FileNotToTouch[i].ParentFolderName,m_FileNotToTouch[i].FileName);
				sprintf(tmpStr2,"%s\\%s",tmpSaveFolder,m_FileNotToTouch[i].ParentFolderName);
				CreateFolder_IfNotAlreayThere(tmpStr2);
				CopyDirectory(tmpStr, tmpStr2); //Copy the file
			}
			else{
				sprintf(tmpStr,"%s\\%s",m_FolderPath,m_FileNotToTouch[i].FileName);//Copy the file
				CopyDirectory(tmpStr, tmpSaveFolder); //Copy the file
			}
		}
	}
	//####################  Second, DELETE existing folders and files. ################
	Find_DeleteFoldersAndFiles(m_FolderPath, 1);
		
	//###################################  Copy  ######################################
	Find_CopyFoldersAndFiles(NewBlipFolder, m_FolderPath);
	//###########################  Restore the saved files. ###########################
	if(m_NumFilesNotToTouch>0){
		sprintf(tmpSaveFolder,"%s\\__tmp",m_FolderPath);
		for(int i=0; i<m_NumFilesNotToTouch; i++){
			if(m_FileNotToTouch[i].CheckedToReplace==0)continue; //The user chose not to replace
			if(m_FileNotToTouch[i].IsThereParentFolder==1){
				sprintf(tmpStr,"%s\\%s\\%s",tmpSaveFolder,m_FileNotToTouch[i].ParentFolderName,m_FileNotToTouch[i].FileName);
				sprintf(tmpStr2,"%s\\%s",m_FolderPath,m_FileNotToTouch[i].ParentFolderName);
				if(wildcmp("SharedConstants*",m_FileNotToTouch[i].FileName)==1){
					sprintf(renamedtmp,"%s\\%s\\Previous_SharedConstants.h",tmpSaveFolder,m_FileNotToTouch[i].ParentFolderName);
					rename(tmpStr, renamedtmp);
					sprintf(tmpStr,"%s",renamedtmp);
				}
			}
			else{
				sprintf(tmpStr,"%s\\%s",tmpSaveFolder,m_FileNotToTouch[i].FileName);
				sprintf(tmpStr2,"%s",m_FolderPath);
				if(wildcmp("SharedConstants*",m_FileNotToTouch[i].FileName)==1){
					sprintf(renamedtmp,"%s\\Previous_SharedConstants.h",tmpSaveFolder);
					rename(tmpStr, renamedtmp);
					sprintf(tmpStr,"%s",renamedtmp);
				}
			}
			CopyDirectory(tmpStr, tmpStr2); //Copy the file
		}
	}
	//##################################  Remove the tmp folder  ######################################
	DeleteDirectory(tmpSaveFolder,false); 

	//##################################  Done  ######################################
	AfxMessageBox("Done!! Recompile your TAS.");
}

int CAnalysisView::CreateFolder_IfNotAlreayThere(char FldrName[])
{	
	int FileResult=CreateDirectory(FldrName,NULL);//Returns a non zero number if succesfull.
	if(FileResult==0){
		FileResult=GetLastError();
		if(FileResult!=183){
			AfxMessageBox("Unable to automatically open/create a folder to store data");//183 means the folder is there already.
			return -1;
		}
	}
	return 0;
}

void CAnalysisView::Give_BlipsChildFolderName(char *ChildFolderName) 
{
	Give_BlipsChildFolderName_GrandChildFolderName_ToReplace(ChildFolderName,"");
}
void CAnalysisView::Give_BlipsChildFolderName_GrandChildFolderName_ToReplace(char *ChildFolderName, char *GrandChildFolderName) 
{
	//Make sure the user entered valid form of directories.
	if(wildcmp("*\\*",ChildFolderName)==1||wildcmp("*\\*",GrandChildFolderName)==1||wildcmp("*/*",GrandChildFolderName)==1){AfxMessageBox("\\ or / is not allowed"); exit(0);}
	if(strcmp(ChildFolderName,"")==0 ||strcmp(ChildFolderName," ")==0){AfxMessageBox("Blip's child folderName is missing"); exit(0);}

	int TwoTiers=1;
	if(strcmp(GrandChildFolderName,"")==0 ||strcmp(GrandChildFolderName," ")==0)TwoTiers=0;  
	if(TwoTiers){
		m_FolderToReplace[m_NumFoldersToReplace].IsThereParentFolder=1; 
		strcpy(m_FolderToReplace[m_NumFoldersToReplace].ParentFolderName,ChildFolderName); 
		strcpy(m_FolderToReplace[m_NumFoldersToReplace].FolderName,GrandChildFolderName);           
	}
	else{
		m_FolderToReplace[m_NumFoldersToReplace].IsThereParentFolder=0; 
		strcpy(m_FolderToReplace[m_NumFoldersToReplace].ParentFolderName,""); 
		strcpy(m_FolderToReplace[m_NumFoldersToReplace].FolderName,ChildFolderName);           
	}
	m_FolderToReplace[m_NumFoldersToReplace].CheckedToReplace=1; 
	m_FolderToReplace[m_NumFoldersToReplace].IdForCheck=m_TotItems;
	m_NumFoldersToReplace++;  
	m_TotItems++;
}

void CAnalysisView::Give_BlipsChild_FileName_ToReplace(char *FileName) 
{
	CAnalysisView::Give_BlipsChildFolderName_FileName_ToReplace("", FileName); 
}

void CAnalysisView::Give_BlipsChildFolderName_FileName_ToReplace(char *ChildFolderName, char *FileName) 
{
	//Make sure the user entered valid form of directories.
	if(wildcmp("*\\*",ChildFolderName)==1||wildcmp("*\\*",FileName)==1||wildcmp("*/*",FileName)==1){AfxMessageBox("\\ or / is not allowed"); exit(0);}
	if(strcmp(FileName,"")==0 ||strcmp(FileName," ")==0){AfxMessageBox("File name is missing"); exit(0);}

	int TwoTiers=1;
	if(strcmp(ChildFolderName,"")==0 ||strcmp(ChildFolderName," ")==0)TwoTiers=0;  
	if(TwoTiers){
		m_FileToReplace[m_NumFilesToReplace].IsThereParentFolder=1; 
		strcpy(m_FileToReplace[m_NumFilesToReplace].ParentFolderName,ChildFolderName); 
		strcpy(m_FileToReplace[m_NumFilesToReplace].FileName,FileName);           
	}
	else{
		m_FileToReplace[m_NumFilesToReplace].IsThereParentFolder=0; 
		strcpy(m_FileToReplace[m_NumFilesToReplace].ParentFolderName,""); 
		strcpy(m_FileToReplace[m_NumFilesToReplace].FileName,FileName);           
	}
	m_FileToReplace[m_NumFilesToReplace].CheckedToReplace=1; 
	m_FileToReplace[m_NumFilesToReplace].IdForCheck=m_TotItems;
	m_NumFilesToReplace++;  
	m_TotItems++;
}

void CAnalysisView::Give_BlipsChildFolderName_FileName_NotToTouch(char *ChildFolderName, char *FileName) 
{
	//Make sure the user entered valid form of directories.
	if(wildcmp("*\\*",ChildFolderName)==1||wildcmp("*\\*",FileName)==1||wildcmp("*/*",FileName)==1){AfxMessageBox("\\ or / is not allowed"); exit(0);}
	if(strcmp(FileName,"")==0 ||strcmp(FileName," ")==0){AfxMessageBox("File name is missing"); exit(0);}

	int TwoTiers=1;
	if(strcmp(ChildFolderName,"")==0 ||strcmp(ChildFolderName," ")==0)TwoTiers=0;  
	if(TwoTiers){
		m_FileNotToTouch[m_NumFilesNotToTouch].IsThereParentFolder=1; 
		strcpy(m_FileNotToTouch[m_NumFilesNotToTouch].ParentFolderName,ChildFolderName); 
		strcpy(m_FileNotToTouch[m_NumFilesNotToTouch].FileName,FileName);           
	}
	else{
		m_FileNotToTouch[m_NumFilesNotToTouch].IsThereParentFolder=0; 
		strcpy(m_FileNotToTouch[m_NumFilesNotToTouch].ParentFolderName,""); 
		strcpy(m_FileNotToTouch[m_NumFilesNotToTouch].FileName,FileName);           
	}
	m_FileNotToTouch[m_NumFilesNotToTouch].CheckedToReplace=1; 
	m_FileNotToTouch[m_NumFilesNotToTouch].IdForCheck=m_TotItems;
	m_NumFilesNotToTouch++;  
}

int CAnalysisView::Get_Check_status(int idx) 
{
	idx--;
	if(idx<0){AfxMessageBox("idx<0"); return 0;}

	int i;
	for(i=0; i<m_NumFoldersToReplace;i++){
		if(idx==m_FolderToReplace[i].IdForCheck)return m_FolderToReplace[i].CheckedToReplace;
	}
	for(i=0; i<m_NumFilesToReplace;i++){
		if(idx==m_FileToReplace[i].IdForCheck)return m_FileToReplace[i].CheckedToReplace;
	}
	return 0;
}
void CAnalysisView::Set_Check_status(int idx, int status) 
{
	idx--;
	if(idx<0){AfxMessageBox("idx<0"); return;}

	int i;
	for(i=0; i<m_NumFoldersToReplace;i++){
		if(idx==m_FolderToReplace[i].IdForCheck){
			m_FolderToReplace[i].CheckedToReplace=status;
			 return;
		}
	}
	for(i=0; i<m_NumFilesToReplace;i++){
		if(idx==m_FileToReplace[i].IdForCheck){
			 m_FileToReplace[i].CheckedToReplace=status;
			 return;
		}
	}
}
