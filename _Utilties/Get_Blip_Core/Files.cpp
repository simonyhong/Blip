
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

void CAnalysisView::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
{
	char tmpStr[1024];
	CString ExeDirString=GetExecutableName();
	strcpy(tmpStr,ExeDirString);
	if(wildcmp("*\AnalogPlotter*",tmpStr)==1){
		RE_Search:;
		if(wildcmp("*\AnalogPlotter",tmpStr)==1){
			sprintf(FullPath,"%s",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search;}
		strcat(FullPath,AddedFolderName);
	}		
	else AfxMessageBox("Couldn't find AnalogPlotter folder.");
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
int CAnalysisView::GetFolderName(char* FolderPath, char *FolderName)
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

bool DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true)
{
  int len = _tcslen(lpszDir);
  TCHAR *pszFrom = new TCHAR[len+2];
  _tcscpy(pszFrom, lpszDir);
  pszFrom[len] = 0;
  pszFrom[len+1] = 0;
  
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

// This function will find all files in a directory and its subdirectory
void CAnalysisView::FindAllFiles(char *New_Blip_Dir, char* searchThisDir, bool searchSubDirs)
{

	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	int i, err, isToCopy;
	int result;
	char OldFullFileName[1024], NewFullFileName[1024];
	// Build the file search string
	char CurrentDir[1024] = {0};

	AddFrwdSlashAtEndIfNotHave(searchThisDir);
	strcpy(CurrentDir,searchThisDir);
	strcat(CurrentDir, "*");

	// Find the first file in the directory.
	hFind = FindFirstFile(CurrentDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// Found a directory
		if(FindFileData.dwFileAttributes == 48){
		//if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			if(searchSubDirs){
				// Folder that should not be created
				if(wildcmp("Debug", FindFileData.cFileName)==1
					|| wildcmp("ipch", FindFileData.cFileName)==1 
					|| (wildcmp("Release", FindFileData.cFileName)==1 && wildcmp("*\\x64\\", searchThisDir)==0)
					|| wildcmp("TAS_Mine", FindFileData.cFileName)==1){
					continue;
				}
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				_snprintf(searchDir2, 2047, "%s%s", searchThisDir, FindFileData.cFileName);
				// Create a new folder
				char New_Blip_Dir2[1024]; 
				strcpy(New_Blip_Dir2, New_Blip_Dir);
				AddFrwdSlashAtEndIfNotHave(New_Blip_Dir2); strcat(New_Blip_Dir2,FindFileData.cFileName);
				result=CreateDirectory(New_Blip_Dir2,NULL); //Creates the new folder if doesn't exist already.
				if(result==0){
					err=GetLastError();
					if(err!=ERROR_ALREADY_EXISTS){
						char CreateFolder_Message[256], FolderName[1024]; 
						GetFolderName(New_Blip_Dir2,FolderName);
						sprintf(CreateFolder_Message,"Failed to create %s folder", FolderName);
						AfxMessageBox(CreateFolder_Message);
						continue;
					}
				}
				// The content of folders to exclude
				if(wildcmp("_Data_Recorded", FindFileData.cFileName)==1 || wildcmp("_VIS_Figures", FindFileData.cFileName)==1)continue;
				FindAllFiles(New_Blip_Dir2,searchDir2, true);
			}
			// Do not need to process anymore
			continue;
		}

		//###### Found a file ######


		// The content of folders to exclude
		if(wildcmp("*\\x64\\Release\\", searchThisDir)==1){
			if(m_isToCopyExe==0 || wildcmp("*.exe", FindFileData.cFileName)==0){
				continue;
			}
		}				
		//###### Files to exclude
		isToCopy=1;
		for(i=0; i<m_NumExts; i++){
			if(wildcmp(m_MatchExtStr[i],FindFileData.cFileName)==1){
				isToCopy=0;
				break;
		}	}
		if(isToCopy==0)continue;

		//######### COPY
		//Old Name
		strcpy(OldFullFileName,searchThisDir); AddFrwdSlashAtEndIfNotHave(OldFullFileName); strcat(OldFullFileName,FindFileData.cFileName);
		//New Name
		strcpy(NewFullFileName,New_Blip_Dir); AddFrwdSlashAtEndIfNotHave(NewFullFileName); strcat(NewFullFileName,FindFileData.cFileName);
		//Copy
		result= CopyFile( OldFullFileName , NewFullFileName, FALSE ); //"FALSE" means to overwrite.
		if(result==0){
			char CopyError_Message[256]; 
			sprintf(CopyError_Message, "CopyFile(%s,%s, %d) has failed", OldFullFileName , NewFullFileName, FALSE);
			AfxMessageBox(CopyError_Message);
		}	

	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
}

