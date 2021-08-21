#include "stdafx.h"
#include "MOXY.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"
#include "SinTable.h"
#include <time.h>
#include "VarDefinitions.h"


#include "Dialog_AppendOverwriteCancel.h"

void CMOXY_View::Save_MOXY_Config() 
{
	char tmpStr[1024], MOXY_Config_File[1024];
	CString St=	GetExecutableName();
	strcpy(tmpStr,St);
	if(FromA_findB_addC_giveD(tmpStr, "Blip", "_Config_Files\\Config_MOXY.txt", MOXY_Config_File)==0){
		MOXY_Config_File[0]='\0';
		AfxMessageBox("Could not find Blip");
		return;
	}

	FILE *fp;
	if((fp=fopen(MOXY_Config_File,"w"))!= NULL){
		double FutureUse=0, dummyDouble=0;
		char dummySt[1024];
		fprintf( fp, "Version_number: %0.2f\n\n",(float)VERSION_NUM_OF_MOXY );		//Version numbere
		fprintf( fp, "XY_Bias_&_Gain:\n");		
		for(int i=0; i<NUM_2D_BEHAVIOR_OBJS; i++){
			fprintf( fp, "%0.5lf\n",M_EyeX_Bias[i]);	
			fprintf( fp, "%0.5lf\n",M_EyeY_Bias[i]);	
			fprintf( fp, "%0.5lf\n",M_EyeX_Gain[i]);	
			fprintf( fp, "%0.5lf\n",M_EyeY_Gain[i]);	
			if(i==0)dummyDouble=m_XY_DisplayMode; else  dummyDouble=0;  //Every variable is recorded in %0.5lf  and read in %lf formats. 
			fprintf( fp, "%0.5lf\n",dummyDouble);//(1)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(2)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(3)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(4)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(5)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(6)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(7)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(8)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(9)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(10)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(11)
			fprintf( fp, "%0.5lf\n",FutureUse);	//(12)
			fprintf( fp, "%\n");	
		}
		fclose(fp);
	}
}

void CMOXY_View::Read_MOXY_Config() 
{
	char tmpStr[1024], MOXY_Config_File[1024];
	CString St=	GetExecutableName();
	strcpy(tmpStr,St);
	if(FromA_findB_addC_giveD(tmpStr, "Blip", "_Config_Files\\Config_MOXY.txt", MOXY_Config_File)==0){
		MOXY_Config_File[0]='\0';
		AfxMessageBox("Could not find Blip");
		return;
	}

	FILE *fp;
	if((fp=fopen(MOXY_Config_File,"r"))!= NULL){
		double FutureUse, dummyDouble=0, versionNum;
		char dummySt[1024];
		fscanf(fp,"%s",dummySt );//	Version_number:	
		if(!wildcmp("*Version*",dummySt))return;
		fscanf(fp,"%lf",&versionNum);// Version numbere
		fscanf(fp,"%s",dummySt );//	XY_Bias_&_Gain:
		
		//The Bias (=0) & Gains (=1) are initialized at "MemMap_Open_TASMOXYVIS.h"
		for(int i=0;i<NUM_2D_BEHAVIOR_OBJS;i++){
			if(fscanf( fp, "%lf",&M_EyeX_Bias[i])==0)break;	//This is okay because the variables are already initialized at "MemMap_Open_TASMOXYVIS.h".
			fscanf( fp, "%lf",&M_EyeY_Bias[i]);	
			fscanf( fp, "%lf",&M_EyeX_Gain[i]);	
			fscanf( fp, "%lf",&M_EyeY_Gain[i]);	
			fscanf( fp, "%lf",&dummyDouble);	if(i==0)m_XY_DisplayMode=dummyDouble;//(1) //Every variable is recorded in %0.5lf  and read in %lf formats. 
			fscanf( fp, "%lf",&FutureUse);	//(2)
			fscanf( fp, "%lf",&FutureUse);	//(3)	
			fscanf( fp, "%lf",&FutureUse);	//(4)	
			fscanf( fp, "%lf",&FutureUse);	//(5)	
			fscanf( fp, "%lf",&FutureUse);	//(6)	
			fscanf( fp, "%lf",&FutureUse);	//(7)	
			fscanf( fp, "%lf",&FutureUse);	//(8)	
			fscanf( fp, "%lf",&FutureUse);	//(9)	
			fscanf( fp, "%lf",&FutureUse);	//(10)	
			fscanf( fp, "%lf",&FutureUse);	//(11)	
			fscanf( fp, "%lf",&FutureUse);	//(12)	
		}
		fclose(fp);
	}
	else{//In the case there is no config file, make one.
		Save_MOXY_Config();
	}
}


CString CMOXY_View::GetDateString()
{
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;  // Get the current time from the operating system.
	CTime CurrTime( osBinaryTime ); //  CTime from C run-time time
	CString Date = CurrTime.Format( "%m%d%y" );	
	return Date;
}

CString CMOXY_View::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

void CMOXY_View::Make_TheBaseOfFileName()
{
	m_Date = GetDateString();	
	strcpy(m_BaseOfFileName,m_PreFix);
	strcat(m_BaseOfFileName,m_Date);
	//strcat(m_BaseOfFileName,"_");
	strcat(m_BaseOfFileName,m_SuFFix);
	if(m_FlagVirMemDC==1 && m_isMOXY_Running==1){//"if(m_FlagVirMemDC==0)" prevents the program trying to access this part before going through "VirMemDC()"
		RemakeFileName(); 
	}
	//Update the File name in Memory Map
	strcpy(M_PreFix,m_PreFix);
	strcpy(M_SuFFix,m_SuFFix);
	strcpy(M_BaseOfFileName,m_BaseOfFileName);
	strcpy(M_JustFileName,m_JustFileName);	
	//Display
	RequestDisplayEvent(DISP_FILE_NAME_AND_RECORDING_STATUS);
	RequestDisplayEvent(REFRESH_XY_TITLE_AREA);
}

void CMOXY_View::RemakeFileName()
{	
	m_PrvNnNumberToRecord=*M_NnNumberToRecord;
	char NnStr[10]; itoa(*M_NnNumberToRecord,NnStr,10);
	sprintf(m_JustFileName,"%s%s.rst",m_BaseOfFileName,NnStr);
}

void CMOXY_View::UpdateFileName_StroredLocal_AND_Disp_Name_n_RecodingStatus()
{	
	strcpy(m_PreFix,M_PreFix);
	strcpy(m_SuFFix,M_SuFFix);
	strcpy(m_BaseOfFileName,M_BaseOfFileName);
	strcpy(m_JustFileName,M_JustFileName);
	m_pXYFig->m_FileName.NameString=m_JustFileName;
	m_pXYFig->RectOfLabel(m_pXYFig->m_FileName, &m_DC[0]);
	m_isSaving=*M_isSaving; //The fillowing is done at the begining of View: m_Fig[XY_FIG].m_pIsSaving=&m_isSaving;
	RequestDisplayEvent(DISP_FILE_NAME_AND_RECORDING_STATUS);
}

void CMOXY_View::Serialize(CArchive& ar)
{

}

//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPathWithoutFileName & m_FileNameFULL
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPathWithoutFileName
int CMOXY_View::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
{	
	CFileDialog dialog(IsToREAD, "rst", initialFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.rst)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){return 0;}
	sprintf(m_FileNameFULL,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPathWithoutFileName,m_FileNameFULL);
	m_FolderPathWithoutFileName[strlen(m_FileNameFULL)-strlen(m_JustFileName)]='\0';
	
	//Extract file name without extension (.txt)
	#define dotSizePlus1 2 //The size of the identifier ".txt"
	char strbuf[1000];
	int i,sizeofChar,foundExt=0;
	sizeofChar=strlen(m_JustFileName);
	for(i=0;i<sizeofChar;i++){
		strncpy_s(strbuf,dotSizePlus1,m_JustFileName+i,_TRUNCATE);
		if(strcmp(".", strbuf)==0){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileNameFULL);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileNameFULL)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CMOXY_View::GetLastFolderName(char* FolderPath, char *LastFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first letter
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	strncpy_s(FldrName,i+1,&tmpStr[0],_TRUNCATE);
	_strrev(FldrName);
	strcpy_s(LastFolderName,100,FldrName);
	return 1;
}

//This one discards the parent folder name out of the full pathway "FolderPath"
int CMOXY_View::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first letter
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}

void CMOXY_View::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
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

int CMOXY_View::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
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

void CMOXY_View::RootFileOpen()
{
	if(m_isMOXY_Running==0){
		Find_Blip_andAddSome("\\_Config_Files\\Config_MOXY.txt",m_FileNameFULL);
		sprintf(m_JustFileName,"Config_MOXY.txt");
	}
	else if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){return;}

	FILE *fp;
	if((fp=fopen(m_FileNameFULL,"r"))!= NULL){
		fscanf(fp,"%lf",&m_futureVariable );
		fclose(fp);
	}
}

void CMOXY_View::Launch_DAS_sv()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\DAS_sv\\x64\\Release\\DAS_sv.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\DAS_sv\\Release\\DAS_sv.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CMOXY_View::Launch_DAS_cl()
{
	if(*M_isDAScl_Running==1){AfxMessageBox("DAS client (Voltate-Time discriminator) is already running."); return;}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\DAS_cl\\x64\\Release\\DAS_cl.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\DAS_cl\\Release\\DAS_cl.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CMOXY_View::On_Launch_VIS()
{
	if(*M_Is_VIS_Running==1){AfxMessageBox("VIS is already running."); return;}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\VIS\\x64\\Release\\VIS.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\VIS\\Release\\VIS.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CMOXY_View::On_Launch_RAS()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\RAS\\x64\\Release\\RAS.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\RAS\\Release\\RAS.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CMOXY_View::On_Launch_ANTS()
{
	if(*M_isANTS_Running==1){AfxMessageBox("ANTS is already running."); return;}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\ANTS\\x64\\Release\\ANTS.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\ANTS\\Release\\ANTS.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CMOXY_View::On_Launch_Oscilloscope()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\Oscilloscope\\x64\\Release\\Oscilloscope.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\Oscilloscope\\Release\\Oscilloscope.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}

void CMOXY_View::OnCLAS()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\CLAS\\x64\\Release\\CLAS.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\CLAS\\Release\\CLAS.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}


void CMOXY_View::On_Launch_XCOR()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\XCOR\\x64\\Release\\XCOR.exe", DirStr);
	#else  // x32
	Find_Blip_andAddSome("\\XCOR\\Release\\XCOR.exe", DirStr);
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}


void CMOXY_View::OnVirtualElectrode()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	char DirStr[1024];
	#ifdef _M_X64 //x64
	Find_Blip_andAddSome("\\_Utilities\\Virtual_Electrode\\VirtualElectrode\\x64\\Release\\Virtual_Electrode.exe", DirStr);
	#else  // x32
	return;
	#endif	
	int result=CreateProcess(DirStr, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(result==0){
		char MsgStr[1050];
		sprintf(MsgStr,"Cannot open: %s", DirStr);
		AfxMessageBox(MsgStr);
	}
}


int CMOXY_View::wildcmp(char *wild, char *string) 
{
  char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if(*wild == '*') {
      if(!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } 
	else if((*wild == *string) || (*wild == '?')) {
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

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
int CMOXY_View::Find_Last_Num_inString(char *GivenString, int &NumToReturn)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0, sign=0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			if(*p=='-')sign=1; else sign=0;
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return -1;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	return 0;
}

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
//In case it does not find a number, it give a waning and does not register anything.
int CMOXY_View::Find_Last_PositiveNum_inString(char *GivenString, int &NumToReturn)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		char strbuf[1000];
		_snprintf(strbuf, 999, "The name does not contain a number, or an alias of an ECode", GivenString);
		AfxMessageBox(strbuf);
		return -1;
	}
	NumToReturn=num;
	return 0;
}
