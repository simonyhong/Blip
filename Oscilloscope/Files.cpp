
#include "stdafx.h"
#include "Oscilloscope.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "OscilloscopeDoc.h"
#include "FiguresOsc.h"
#include "OscilloscopeView.h"
#include "SinTable.h"
//This one discards the parent folder name out of the full pathway "FolderPath"
int COscilloscopeView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
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

CString COscilloscopeView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

void COscilloscopeView::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
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

#define FAUX_MIN 0
#define FAUX_MAX (-1)
void COscilloscopeView::OnFileOpen()
{
	if(m_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\Config_Osc.txt", m_FileName);
		sprintf(m_JustFileName,"Config_Osc.txt");
	}
	else if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){return;}
	int clm,row,strLength,i,ch=0;
	float dummyF, VerNo;
	double dummyD[4];
	char dummySt[1024];
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
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
		fscanf(fp,"%f",&VerNo);// Version numbere
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
			
			if(VerNo>1){
				do{
					if(fscanf( fp, "%lf%lf%lf%lf",&dummyD[0],&dummyD[1],&dummyD[2],&dummyD[3])==EOF)break;	 
					if(dummyD[0]==FAUX_MIN && dummyD[1]==FAUX_MAX && dummyD[2]==FAUX_MIN && dummyD[3]==FAUX_MAX)break;
					if(ch<MAX_AI_CHAN){pFig->m_dChMinX[ch]=dummyD[0]; pFig->m_dChMaxX[ch]=dummyD[1]; pFig->m_dChMinY[ch]=dummyD[2]; pFig->m_dChMaxY[ch]=dummyD[3];} 
					ch++;
				}while(1);
			}
			else{//Old version
				fscanf( fp,"%lf%lf%lf%lf",&pFig->m_dMinX,&pFig->m_dMaxX,&pFig->m_dMinY,&pFig->m_dMaxY);	
				pFig->m_dChMinX[0]=pFig->m_dMinX; pFig->m_dChMaxX[0]=pFig->m_dMaxX; pFig->m_dChMinY[0]=pFig->m_dMinY; pFig->m_dChMaxY[0]=pFig->m_dMaxY;
			}

			fscanf( fp,"%d",&pFig->m_Deleted);	
		}
		//If the file does not have enough data to cover the whole channels,
		if(ch<MAX_AI_CHAN){
			for(;ch<MAX_AI_CHAN;ch++){
				pFig->m_dChMinX[ch]=0; pFig->m_dChMaxX[ch]=100; pFig->m_dChMinY[ch]=-10; pFig->m_dChMaxY[ch]=10;	 
			}
		}
		if(VerNo>1){pFig->m_dMinX=pFig->m_dChMinX[0]; pFig->m_dMaxX=pFig->m_dChMaxX[0]; pFig->m_dMinY=pFig->m_dChMinY[0]; pFig->m_dMaxY=pFig->m_dChMaxY[0];	}
		fclose(fp);	
	}
	if(m_dHowManyClmns!=-1){
		EliminateExcluded();
	}
}

void COscilloscopeView::OnFileSave()
{
	if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,"*.txt")==0){return;}
	int clm,row,ch;
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileName,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)OSCILLOSCOPE_CONSTANTS );		//Version numbere
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
					
				//fprintf( fp, "%0.3f\t%0.3f\t%0.3f\t%0.3f\n",pFig->m_dMinX*ScaleFactor,pFig->m_dMaxX*ScaleFactor,pFig->m_dMinY,pFig->m_dMaxY);	 
				for(ch=0;ch<MAX_AI_CHAN;ch++){
					fprintf( fp, "%0.3f\t%0.3f\t%0.3f\t%0.3f\n",pFig->m_dChMinX[ch],pFig->m_dChMaxX[ch],pFig->m_dChMinY[ch],pFig->m_dChMaxY[ch]);	 
				}
				fprintf( fp, "%0.3lf\t%0.3lf\t%0.3lf\t%0.3lf\n",(double)FAUX_MIN,(double)FAUX_MAX,(double)FAUX_MIN,(double)FAUX_MAX);//To indicate the end of the channels' scopes

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
int COscilloscopeView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
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

void COscilloscopeView::SaveThresholds() 
{
	char tmpStr[1024], DAS_sv_Config_File[1024];
	CString St=	GetExecutableName();
	strcpy(tmpStr,St);
	if(FromA_findB_addC_giveD(tmpStr, "Blip", "_Config_Files\\DAS_sv_Config_Files\\Thresholds.txt", DAS_sv_Config_File)==0){
		DAS_sv_Config_File[0]='\0';
		AfxMessageBox("Could not find Blip");
		return;
	}

	FILE *fp;
	if((fp=fopen(DAS_sv_Config_File,"w"))!= NULL){
		for(int i=0;i<MAX_AI_CHAN;i++){
			fprintf( fp, "%0.5lf\n",M_Threshold[i]);	
		}
		fclose(fp);
	}
}

void COscilloscopeView::ReadThresholds() 
{
	char tmpStr[1024], DAS_sv_Config_File[1024];
	CString St=	GetExecutableName();
	strcpy(tmpStr,St);
	if(FromA_findB_addC_giveD(tmpStr, "Blip", "_Config_Files\\DAS_sv_Config_Files\\Thresholds.txt", DAS_sv_Config_File)==0){
		DAS_sv_Config_File[0]='\0';
		AfxMessageBox("Could not find Blip");
		return;
	}

	FILE *fp;
	if((fp=fopen(DAS_sv_Config_File,"r"))!= NULL){
		for(int i=0;i<MAX_AI_CHAN;i++){
			fscanf( fp, "%lf",&M_Threshold[i]);	
		}
		fclose(fp);
	}
}

int COscilloscopeView::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
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

int COscilloscopeView::wildcmp(char *wild, char *string) 
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
