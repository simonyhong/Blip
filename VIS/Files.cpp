
#include "stdafx.h"
#include "VIS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "VIS_Doc.h"
#include "Figures.h"
#include "SharedConst_TASMOXYVIS.h"
#include "SharedConstants.h"
#include "VIS_View.h"
#include "SinTable.h"

//This one discards the parent folder name out of the full pathway "FolderPath"
int CVISView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first letter
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(wildcmp("\\", FldrName))break;
	}
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}

CString CVISView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

void CVISView::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
{
	char tmpStr[1024];
	CString ExeDirString=GetExecutableName();
	strcpy(tmpStr,ExeDirString);
	if(wildcmp("*\Blip*",tmpStr)==1){
		RE_Search:;
		if(wildcmp("*\Blip",tmpStr)==1){
			sprintf(FullPath,"%s",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search;}
		strcat(FullPath,AddedFolderName);
	}		
	else AfxMessageBox("Couldn't find Blip folder.");
}
void CVISView::OnFileOpen()
{
	if(m_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\Config_VIS.txt", m_FileName);
		sprintf(m_JustFileName,"Config_VIS.txt");
	}
	else if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){m_FileOpenSucceeded=0; return;}

	FILE *fp;
	if((fp=fopen(m_FileName,"r"))!= NULL){
		fscanf(fp,"%lf",&m_ScreenSize_inDegrees_4VIS );
		m_FileOpenSucceeded=1;
		if(m_bFistTime==0)PvisAllOff_implement();//This updates the screen using "m_ScreenSize_inDegrees_4VIS".
		fclose(fp);
	}
	else m_FileOpenSucceeded=0;
}

void CVISView::OnFileSave()
{
	//if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,"*.txt")==0){return;}
	Find_Blip_andAddSome("\\_Config_Files\\Config_VIS.txt", m_FileName);

	FILE *fp;
	if((fp=fopen(m_FileName,"w"))!= NULL){
		fprintf(fp,"%d",(int)m_ScreenSize_inDegrees_4VIS);
		fclose(fp);
	}
}


//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPath & m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath
int CVISView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
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


int CVISView::wildcmp(char *wild, char *string) 
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
