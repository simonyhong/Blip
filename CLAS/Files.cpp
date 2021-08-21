
#include "stdafx.h"
#include "CLAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "CLAS_Doc.h"
#include "Figure.h"
#include "CLAS_View.h"
#include "SinTable.h"

//This one discards the parent folder name out of the full pathway "FolderPath"
int CCLAS_View::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
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

CString CCLAS_View::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

void CCLAS_View::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
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

void CCLAS_View::OnFileOpen()
{
	if(m_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\Config_CLAS.txt", m_FileName);
		sprintf(m_JustFileName,"Config_CLAS.txt");
	}
	else if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){return;}
	int clm,row,strLength,i;
	float dummyF;
	char dummySt[1024];
	CFigure *pFig;

	FILE *fp;
	if((fp=fopen(m_FileName,"r"))!= NULL){
		m_numCaptured=0; m_NewlyLoaded=1;
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				pFig->m_Deleted=1;
				pFig->m_Captured=0;
			}
		}
		fscanf(fp,"%s",dummySt );//	Version_number:	
		if(!wildcmp("*Version*",dummySt))return;
		fscanf(fp,"%f",&dummyF);// Version numbere
		fscanf(fp,"%s",dummySt );//	GlobalTitle:	
		fgets(dummySt,1023,fp);		strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}} m_GlobalTitle=dummySt;	
		fscanf(fp,"%s",dummySt );//	How_many_ROWs:	
		fscanf(fp,"%d",&m_dHowManyRows );			
		fscanf(fp,"%s",dummySt );//	How_many_COLUMNs:	
		fscanf(fp,"%d",&m_dHowManyClmns );		
		while(fscanf( fp,"%d%d",&row,&clm)!=EOF){
			if(row>MaxFigRow||clm>MaxFigCln){AfxMessageBox("row>MaxFigRow||clm>MaxFigCln."); exit(0);}
			pFig=&m_Fig[row][clm][0];
			fscanf(fp,"%s",dummySt );//	Title:	
			fgets(dummySt,1023,fp);		strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}} pFig->m_Title.NameString=dummySt;	
			fscanf(fp,"%d%d%d%d",&pFig->m_nTx,&pFig->m_nTy,&pFig->m_nBx,&pFig->m_nBy);	
			fscanf(fp,"%d%d",&pFig->m_FontWidth,&pFig->m_FontHeight);	
			fscanf(fp,"%d%d",&pFig->m_howManyX,&pFig->m_howManyY);	
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cXaxis[0],""); else sprintf(pFig->m_cXaxis[0],dummySt);//The label of X axis
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cYaxis[0],""); else sprintf(pFig->m_cYaxis[0],dummySt);//The label of Y axis
			fscanf( fp,"%lf%lf%lf%lf",&pFig->m_dMinX,&pFig->m_dMaxX,&pFig->m_dMinY,&pFig->m_dMaxY);	 
			fscanf( fp,"%d",&pFig->m_Deleted);	
		}
	}
	if(m_dHowManyClmns!=-1){
		fclose(fp);	
		EliminateExcluded();
	}
}


void CCLAS_View::OnFileSave()
{
	if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,"*.txt")==0){return;}
	int clm,row;
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileName,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)VERSION_NUM_OF_RASTER_PLOTTER );		//Version numbere
		if(strlen(m_GlobalTitle)==0)fprintf( fp, "GlobalTitle: NoName\n");
		else fprintf( fp, "GlobalTitle: %s\n",m_GlobalTitle);//fputs( LPCTSTR( m_user_input), fp);			
		fprintf( fp, "How_many_ROWs: %d\n",m_dHowManyRows );			
		fprintf( fp, "How_many_COLUMNs: %d\n\n",m_dHowManyClmns );		
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				fprintf( fp, "%d\t%d\n",row,clm);	
				if(pFig->m_Title.NameString.GetLength()==0)fprintf( fp, "Title: NoName\n");
				else fprintf( fp, "Title: %s\n",pFig->m_Title.NameString );			
				fprintf( fp, "%d\t%d\t%d\t%d\n",pFig->m_nTx,pFig->m_nTy,pFig->m_nBx,pFig->m_nBy);	
				fprintf( fp, "%d\t%d\n",pFig->m_FontWidth,pFig->m_FontHeight);	
				fprintf( fp, "%d\t%d\n",pFig->m_howManyX,pFig->m_howManyY);	
				if(strlen(pFig->m_cXaxis[0])==0)fprintf( fp, "NULL\n");
				else fprintf( fp, "%s\n",pFig->m_cXaxis[0]);
				if(strlen(pFig->m_cYaxis[0])==0)fprintf( fp, "NULL\n");
				else fprintf( fp, "%s\n", pFig->m_cYaxis[0]);
					
				fprintf( fp, "%0.3f\t%0.3f\t%0.3f\t%0.3f\n",pFig->m_dMinX,pFig->m_dMaxX,pFig->m_dMinY,pFig->m_dMaxY);	 
				fprintf( fp, "%d\n\n\n",pFig->m_Deleted);	
			}
		}
	}
	fclose(fp);
}


//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPath & m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath
int CCLAS_View::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
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


int CCLAS_View::wildcmp(char *wild, char *string) 
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
