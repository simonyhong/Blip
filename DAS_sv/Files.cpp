#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "VarDefinitions_SIG.h"
#include "Channel.h"
#include "DAS_View.h"
#include "SinTable.h"
#include "MainFrm_DAS.h"
#include <NIDAQmx.h>
#include "Mmsystem.h" 


void CDasView::ReadThresholds() 
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
	int i=0;
	if((fp=fopen(DAS_sv_Config_File,"r"))!= NULL){
		while(1){
			if(i>=MAX_AI_CHAN)break;
			if(fscanf( fp, "%lf",&M_Threshold[i])==EOF)break;	
			i++;
		}
		fclose(fp);
		//In the case there was not enough data to fill the demand,
		for(;i<MAX_AI_CHAN;i++){
			M_Threshold[i]=0;
		}
	}
}

CString CDasView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

int CDasView::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
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

//This one discards the parent folder name out of the full pathway "FolderPath"
int CDasView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
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

int CDasView::wildcmp(char *wild, char *string) 
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
