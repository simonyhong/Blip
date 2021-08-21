
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

//This one discards the parent folder name out of the full pathway "FolderPath"
int CXCOR_View::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
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

CString CXCOR_View::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

int CXCOR_View::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
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

void CXCOR_View::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
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

void CXCOR_View::OnFileOpen()
{
	m_bIsON=0;int a;
	if(m_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\Config_XCOR.txt", m_FileName);
		sprintf(m_JustFileName,"Config_XCOR.txt");
	}
	else{
		char tmpStr[1024], XCOR_Config_Folder[1024];
		CString St=	GetExecutableName();
		strcpy(tmpStr,St);
		if(FromA_findB_addC_giveD(tmpStr, "Blip", "_Config_Files", XCOR_Config_Folder)==0){
			XCOR_Config_Folder[0]='\0';
		}
		if(GetFile_WholePath_WithInitialFolder_Name(TOREAD,"Config File",XCOR_Config_Folder,"*.txt")==0){return;}
	}
	int clm,row,strLength,i;
	float dummyF, versionNum;
	char dummySt[1024], tmpStr[256];
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileName,"r"))!= NULL){
		m_numCaptured=0; m_NewlyLoaded=1;
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				pFig->m_Deleted=1;
				pFig->m_Captured=0;
			}
		}
		fscanf(fp,"%s",dummySt );//	Version_number:	
		if(!wildcmp("*Version*",dummySt))return;
		fscanf(fp,"%f",&versionNum);// Version numbere
		fscanf(fp,"%s",dummySt );//	GlobalTitle:	
		fgets(dummySt,1023,fp);	strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}}
		m_GlobalTitle=dummySt;	
		fscanf(fp,"%s",dummySt );//	How_many_ROWs:	
		fscanf(fp,"%d",&m_dHowManyRows );			
		fscanf(fp,"%s",dummySt );//	How_many_COLUMNs:	
		fscanf(fp,"%d",&m_dHowManyClmns );		
		while(fscanf( fp,"%d%d",&row,&clm)!=EOF){
			if(row>MaxFigRow||clm>MaxFigCln){AfxMessageBox("row>MaxFigRow||clm>MaxFigCln."); exit(0);}
			pFig=&m_Fig[row][clm][0];
			fscanf(fp,"%d%d%d%d",&pFig->m_nTx,&pFig->m_nTy,&pFig->m_nBx,&pFig->m_nBy);	
			fscanf(fp,"%d%d",&pFig->m_FontWidth,&pFig->m_FontHeight);	
			fscanf(fp,"%d%d",&pFig->m_howManyXguideLines,&pFig->m_howManyYguideLines);	
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cXaxis[0],""); else sprintf(pFig->m_cXaxis[0],dummySt);//The label of X axis
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cYaxis[0],""); else sprintf(pFig->m_cYaxis[0],dummySt);//The label of Y axis
			fscanf( fp,"%lf%lf%lf%lf",&pFig->m_dMinX,&pFig->m_dMaxX,&pFig->m_dMinY,&pFig->m_dMaxY);	 
			fscanf( fp,"%d",&pFig->m_Deleted);	
			fscanf( fp, "%d %d %d %d %lf",&pFig->m_XCOR_Label.m_AssociatedVar,&pFig->m_XCOR_Label.status,&pFig->m_XCOR_Label.m_Var2,&pFig->m_XCOR_Label.status2,&pFig->m_dSigma);	
			m_XCOR_RefChan	=pFig->m_XCOR_Label.m_AssociatedVar; m_XCOR_RefNn =pFig->m_XCOR_Label.status; m_XCOR_SbjChan=pFig->m_XCOR_Label.m_Var2; m_XCOR_SbjNn =pFig->m_XCOR_Label.status2; m_fSigma=pFig->m_dSigma;
			sprintf(tmpStr,"Ref(Ch:%d/Nn:%d)  Sbj(Ch:%d/Nn:%d)",pFig->m_XCOR_Label.m_AssociatedVar,pFig->m_XCOR_Label.status,pFig->m_XCOR_Label.m_Var2,pFig->m_XCOR_Label.status2);
			pFig->m_XCOR_Label.NameString=tmpStr;
		}
	}
	if(m_dHowManyClmns!=-1){
		fclose(fp);	
		EliminateExcluded();
	}
	if(m_bFistTime!=1){
		m_NewlyLoaded=0;
	}
}


void CXCOR_View::OnFileSave()
{
	if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,"*.txt")==0){return;}
	int clm,row;
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileName,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)VERSION_NUM_OF_XCORTER_PLOTTER );		//Version numbere
		if(strlen(m_GlobalTitle)==0)fprintf( fp, "GlobalTitle: NoName\n");
		else fprintf( fp, "GlobalTitle: %s\n",m_GlobalTitle);//fputs( LPCTSTR( m_user_input), fp);			
		fprintf( fp, "How_many_ROWs: %d\n",m_dHowManyRows );			
		fprintf( fp, "How_many_COLUMNs: %d\n\n",m_dHowManyClmns );		
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				fprintf( fp, "%d\t%d\n",row,clm);	
				fprintf( fp, "%d\t%d\t%d\t%d\n",pFig->m_nTx,pFig->m_nTy,pFig->m_nBx,pFig->m_nBy);	
				fprintf( fp, "%d\t%d\n",pFig->m_FontWidth,pFig->m_FontHeight);	
				fprintf( fp, "%d\t%d\n",pFig->m_howManyXguideLines,pFig->m_howManyYguideLines);	
				if(strlen(pFig->m_cXaxis[0])==0)fprintf( fp, "NULL\n");
				else fprintf( fp, "%s\n",pFig->m_cXaxis[0]);
				if(strlen(pFig->m_cYaxis[0])==0)fprintf( fp, "NULL\n");
				else fprintf( fp, "%s\n", pFig->m_cYaxis[0]);
					
				fprintf( fp, "%0.3lf\t%0.3lf\t%0.3lf\t%0.3lf\n",pFig->m_dMinX,pFig->m_dMaxX,pFig->m_dMinY,pFig->m_dMaxY);	 
				fprintf( fp, "%d\n",pFig->m_Deleted);	
				fprintf( fp, "%d\t%d\t%d\t%d\t%0.3lf\t\n\n\n", pFig->m_XCOR_Label.m_AssociatedVar, pFig->m_XCOR_Label.status, pFig->m_XCOR_Label.m_Var2, pFig->m_XCOR_Label.status2,m_fSigma);	
			}
		}
	}
	fclose(fp);
}



//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath or m_FolderPathRST
int CXCOR_View::GetFile_WholePath_WithInitialFolder_Name(BOOL IsToREAD, char *TitleOfDialog, char *initialFolder, char *initialFileName)
{	
	CFileDialog dialog(IsToREAD, "txt", initialFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.txt)|All files(*.*)|*.*|| ",NULL);
    dialog.m_ofn.lpstrTitle = TitleOfDialog; 
    dialog.m_ofn.lpstrInitialDir = initialFolder; 


	if(dialog.DoModal()==IDCANCEL){m_bIsON=0;return 0;}
	//File name, just file name, folder path
	sprintf(m_FileName,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	//RST folder
	if(strcmp(TitleOfDialog,"Data File")==0){
		strcpy_s(m_FolderPathRST,m_FileName);
		m_FolderPathRST[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	}
	//Config folder
	else{
		strcpy_s(m_FolderPath,m_FileName);
		m_FolderPath[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	}
	
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
int CXCOR_View::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
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

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CXCOR_View::GetFolderName(char* FolderPath, char *FolderName)
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


int CXCOR_View::wildcmp(char *wild, char *string) 
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
