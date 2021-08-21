
#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"

#include "AnalysisView.h"
#include "SinTable.h"

void CAnalysisView::OnFileOpen()
{
}

int CAnalysisView::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
{
	char tmpStr[1024], FindNameWild[100],FindName[100];
	strcpy(tmpStr,fromA);
	sprintf(FindNameWild,"*\\%s*",findB);
	sprintf(FindName,"*\\%s",findB);
	if(wildcmp(FindNameWild,tmpStr)==1){
		RE_Search:;
		if(wildcmp(FindName,tmpStr)==1){
			sprintf(giveD,"%s\\",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search;}
		strcat(giveD,addC);
	}		
	else{
		sprintf(tmpStr,"Couldn't find %s",findB);
		AfxMessageBox(tmpStr);
		return 0;
	}
	return 1;
}

CString CAnalysisView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}


//This one discards the parent folder name out of the full pathway "FolderPath"
int CAnalysisView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first two (\*) letters
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}


void CAnalysisView::OnFileSave()
{
}


//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPath & m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath
int CAnalysisView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
{	
	CFileDialog dialog(IsToREAD, "txt", initialFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.txt)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){m_bIsON=0;return 0;}
	sprintf(m_FileName,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPath,m_FileName);
	m_FolderPath[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	
	//Extract file name without extension (.txt)
	#define dotSizePlus1 2 //The size of the identifier ".txt"
	char strbuf[1000];
	int i,sizeofChar,foundExt=0;
	sizeofChar=strlen(m_JustFileName);
	for(i=0;i<sizeofChar;i++){
		strncpy_s(strbuf,dotSizePlus1,m_JustFileName+i,_TRUNCATE);
		if(wildcmp(".", strbuf)){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileName);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileName)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}
//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPath & m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath
int CAnalysisView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD)
{	
	CFileDialog dialog(IsToREAD, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "All files(*.*)|*.*|| ",NULL);
	//CFileDialog dialog(IsToREAD, "rst", "*.rst", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.rst)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){m_bIsON=0;return 0;}
	sprintf(m_FileName,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPath,m_FileName);
	m_FolderPath[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	
	//Extract file name without extension (.txt)
	#define dotSizePlus1 2 //The size of the identifier ".txt"
	char strbuf[1000];
	int i,sizeofChar,foundExt=0;
	sizeofChar=strlen(m_JustFileName);
	for(i=0;i<sizeofChar;i++){
		strncpy_s(strbuf,dotSizePlus1,m_JustFileName+i,_TRUNCATE);
		if(wildcmp(".", strbuf)){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileName);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileName)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CAnalysisView::GetLastFolderName(char* FolderPath, char *FolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=2;i<sizeofChar-1;i++){//ignore the first two (\*) letters
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(wildcmp("\\", FldrName))break;
	}
	strncpy_s(FldrName,i+1,&tmpStr[0],_TRUNCATE);
	_strrev(FldrName);
	strcpy_s(FolderName,100,FldrName);
	return 1;
}

int CAnalysisView::wildcmp(char *wild, char *string) 
{
  char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } 
	else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } 
	else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  int a=strlen(wild);
  if(a==0)return 1;
  return !*wild;
}

bool CAnalysisView::DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true)
{
  int len = _tcslen(lpszDir);
  TCHAR *pszFrom = new TCHAR[len+2];
  _tcscpy(pszFrom, lpszDir);
  pszFrom[len] = 0;
  pszFrom[len+1] = 0; //Two null characters at the end.
  
  SHFILEOPSTRUCT fileop;
  fileop.hwnd   = NULL;    // no status display
  fileop.wFunc  = FO_DELETE;  // delete operation
  fileop.pFrom  = pszFrom;  // source file name as double null terminated string
  fileop.pTo    = NULL;    // no destination needed
  fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT;  // do not prompt the user
  
  if(!noRecycleBin)
    fileop.fFlags |= FOF_ALLOWUNDO;

  fileop.fAnyOperationsAborted = FALSE;
  fileop.lpszProgressTitle     = NULL;
  fileop.hNameMappings         = NULL;

  int ret = SHFileOperation(&fileop);
  delete [] pszFrom;  
  return (ret == 0);
}

int CAnalysisView::CopyDirectory(LPCTSTR Source, LPCTSTR DestFolder)
{
	//Source Folder
  int len = _tcslen(Source);
  TCHAR *tmpSource = new TCHAR[len+2];
  _tcscpy(tmpSource, Source);
  tmpSource[len] = 0;   tmpSource[len+1] = 0; //Two null characters at the end.
  //Destination folder
  len = _tcslen(DestFolder);
  TCHAR *pszTo = new TCHAR[len+2];
  _tcscpy(pszTo, DestFolder);
  pszTo[len] = 0;   pszTo[len+1] = 0; //Two null characters at the end.

   SHFILEOPSTRUCT sf;
    memset(&sf,0,sizeof(sf));
    sf.hwnd = 0;
    sf.wFunc = FO_COPY;
    sf.pFrom = tmpSource;
    sf.pTo = pszTo;
    sf.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI;
 
    int n = SHFileOperation(&sf);
    if( n == 0)
    {
		return 0;
    }
    else
    {
		char FailedMsg[2048]; 
		sprintf(FailedMsg,"Failed to copy:\n %s \n to \n %s",Source,DestFolder);
        AfxMessageBox(FailedMsg);
		return 1;
    }
}

// This function will find all files in a directory and its subdirectory
void CAnalysisView::Find_DeleteFoldersAndFiles(char* searchThisDir, bool searchSubDirs)
{
	RemoveForwardSlashAtEnd(searchThisDir);

	char tmpStr[2048];
	for(int i=0; i<m_NumFoldersToReplace; i++){
		if(m_FolderToReplace[i].CheckedToReplace==0)continue; //The user chose not to replace
		if(wildcmp("*__tmp*",searchThisDir)==1)continue;
		if(m_FolderToReplace[i].IsThereParentFolder==1){
			sprintf(tmpStr,"*%s\\%s",m_FolderToReplace[i].ParentFolderName,m_FolderToReplace[i].FolderName);
		}
		else sprintf(tmpStr,"*%s",m_FolderToReplace[i].FolderName);
		if(wildcmp(tmpStr,searchThisDir)==1){
			DeleteDirectory(searchThisDir,false); //Remove the folder
			return;
		}	
	}

	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	int i, err;
	char FullFileName[1024];
	// Build the file search string
	char searchDir[2048] = {0};

	// If it already is a path that ends with \, only add the *
	if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", searchThisDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", searchThisDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// Found a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(searchSubDirs){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				strcpy(searchDir2, searchThisDir); 	AddFrwdSlashAtEndIfNotHave(searchDir2);
				strcat(searchDir2, FindFileData.cFileName);
				Find_DeleteFoldersAndFiles(searchDir2, true);
			}
			// Do not need to process anymore
			continue;
		}

		//############ See if the file is one of the files to delete  ##########
		for(i=0;i<m_NumFilesToReplace;i++){
			if(m_FileToReplace[i].CheckedToReplace==0)continue;
			if(m_FileToReplace[i].IsThereParentFolder==1){
				sprintf(tmpStr,"*\\%s",m_FileToReplace[i].ParentFolderName);
				if(wildcmp(tmpStr,searchThisDir)==0)continue; //The parent folder names are differnt.
			}
			if(wildcmp(m_FileToReplace[i].FileName,FindFileData.cFileName)){
				strcpy(FullFileName, searchThisDir); 	AddFrwdSlashAtEndIfNotHave(FullFileName);
				strcat(FullFileName, FindFileData.cFileName);
				err=remove(FullFileName);
			}
		}
		//######################################################################
	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
}


// This function will find all files in a directory and its subdirectory
void CAnalysisView::Find_CopyFoldersAndFiles(char* SourceDir, char* Destination)
{
	RemoveForwardSlashAtEnd(SourceDir);
	RemoveForwardSlashAtEnd(Destination);

	char tmpStr[2048];
	char tmpDest[2048];
	for(int i=0; i<m_NumFoldersToReplace; i++){
		if(m_FolderToReplace[i].CheckedToReplace==0)continue;
		strcpy(tmpStr,"*\\");
		if(m_FolderToReplace[i].IsThereParentFolder==1){
			strcat(tmpStr,m_FolderToReplace[i].ParentFolderName);
			AddFrwdSlashAtEndIfNotHave(tmpStr); //Add the forward slash.
		}
		strcat(tmpStr,m_FolderToReplace[i].FolderName);

		if(wildcmp(tmpStr,SourceDir)==1){
			RemoveForwardSlashAtEnd(SourceDir); //Remove the forward slash.
			if(strcmp(m_FolderToReplace[i].ParentFolderName,"")!=0){
				sprintf(tmpDest,"%s\\%s\\",Destination,m_FolderToReplace[i].ParentFolderName);
				CopyDirectory(SourceDir, tmpDest); //Copy the folder
			}
			else CopyDirectory(SourceDir, Destination); //Copy the folder
			return;
		}	
	}

	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	int err;
	char FullFileName[1024];
	// Build the file search string
	char searchDir[2048] = {0};

	// If it already is a path that ends with \, only add the *
	if(SourceDir[strlen(SourceDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", SourceDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", SourceDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// Found a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(1){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				strcpy(searchDir2, SourceDir);  AddFrwdSlashAtEndIfNotHave(searchDir2);
				strcat(searchDir2,FindFileData.cFileName);
				Find_CopyFoldersAndFiles(searchDir2, Destination);
			}
			// Do not need to process anymore
			continue;
		}

		//############ See if the file is one of the single files to replace  ##########
		for(int i=0;i<m_NumFilesToReplace;i++){
			if((m_FileToReplace[i].CheckedToReplace!=0 && m_FileToReplace[i].CheckedToReplace!=1) || (m_FileToReplace[i].IsThereParentFolder!=0 && m_FileToReplace[i].IsThereParentFolder!=1)){AfxMessageBox("Parameters are wrong: CheckedToReplace or IsThereParentFolder");}
			if(m_FileToReplace[i].CheckedToReplace==0)continue;
			if(m_FileToReplace[i].IsThereParentFolder==1){
				sprintf(tmpStr,"*\\%s",m_FileToReplace[i].ParentFolderName);
				if(wildcmp(tmpStr,SourceDir)==0)continue; //The parent folder names are differnt.
			}
			if(wildcmp("*End_Users_Definitions.h",FindFileData.cFileName)==1)continue; //For BLIP update, do not copy this.
			//Copy!!
			if(wildcmp(m_FileToReplace[i].FileName,FindFileData.cFileName)){
				//Source file
				strcpy(FullFileName, SourceDir);  AddFrwdSlashAtEndIfNotHave(FullFileName);
				strcat(FullFileName,FindFileData.cFileName);
				//Destination folder
				strcpy(tmpDest, Destination);  AddFrwdSlashAtEndIfNotHave(tmpDest);
				if(m_FileToReplace[i].IsThereParentFolder==1){
					strcat(tmpDest,m_FileToReplace[i].ParentFolderName);  AddFrwdSlashAtEndIfNotHave(tmpDest);
				}
				//Copy the file
				CopyDirectory(FullFileName, tmpDest); 
			}
		}
		//######################################################################
	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
}



// This function will find all files in a directory and its subdirectory
int CAnalysisView::IsThere_FolderA_inFolderB(char* findFolderA, char* inFolderB, int searchChild)
{
	int found=0;
	char tmpStr[2048];
	sprintf(tmpStr,"*%s",findFolderA);
	if(wildcmp(tmpStr,inFolderB)==1){
		return 1;
	}

	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	int i, err;
	char FullFileName[1024];
	// Build the file search string
	char searchDir[2048] = {0};

	// If it already is a path that ends with \, only add the *
	if(findFolderA[strlen(inFolderB) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", inFolderB);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", inFolderB);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return found;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// Found a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			if(strcmp(findFolderA,FindFileData.cFileName)==0){
				found=1;
				break;
			}	
			if(searchChild){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				// If it already is a path that ends with \, only add the *
				if(inFolderB[strlen(inFolderB) - 1] == '\\') _snprintf(searchDir2, 2047, "%s%s", inFolderB, FindFileData.cFileName);
				// Otherwise, add the \* to the end of the path
				else _snprintf(searchDir2, 2047, "%s\\%s", inFolderB, FindFileData.cFileName);
				found=IsThere_FolderA_inFolderB(findFolderA, searchDir2,searchChild);
				if(found==1)break;
			}
			continue;
		}

	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
	return found; 
}
