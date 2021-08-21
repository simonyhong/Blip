
#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "Figure_DAS.h"
#include "Channel.h"
#include "DAS_View.h"
#include "SinTable.h"
#include "AppendOverwriteCancel.h"
#include "StringEdit.h"

void CDasView::OnFileSave()
{
	if(m_isSaving==1){//Turn it off!! It has the same effect as pushing the "close" button from the menu.
		m_isSaving=0; 
		RequestDisplayEvent(DISP_FILE_NAME);//To display the base of the file name.
		return;
	}

	int tempRecordingChannel=-1;
	//In the case there are many figures, specify the recording channel:
	if(!(m_dHowManyClmns==1 && m_dHowManyRows==1)){
		StringEdit	ParameterBOX;
		char buff[256]; sprintf(buff,"Recording Channel: %d",m_RecordingChanId);
		ParameterBOX.m_UserInputString1=buff;
		if(ParameterBOX.DoModal()==IDOK){
			char ChanLabel[256];
			strcpy(ChanLabel,ParameterBOX.m_UserInputString1);
			m_EnteredChanID=Find_Last_Num_inString(ChanLabel);
			if(m_EnteredChanID>=0 && m_EnteredChanID<NUM_NN_AI_CHAN){
				CFigure *pFig;
				int i, j;
				for(i=0;i<m_dHowManyRows;i++){
					for(j=0;j<m_dHowManyClmns;j++){
						pFig=&m_Fig[i][j][0];
						if(pFig->pChan!=NULL && pFig->pChan->m_ChanID==m_EnteredChanID && pFig->pChan->m_IsThisChanUsed==1 ){//make all the visible channels active.
							tempRecordingChannel=(int)m_EnteredChanID;
						}
				}	}
				if(tempRecordingChannel!=-1)m_RecordingChanId=tempRecordingChannel;
				else{AfxMessageBox("Wrong Channel"); return;} //Wrong channel
			}
			else{AfxMessageBox("Wrong Channel"); return;} //Wrong channel
		}
		else return;//The user canceled the input.
	}

	Make_TheBaseOfFileName();
	int result=LocateAndCreateFolder_IfItIsNotAlreayThere();
	if(result==-1){//Failed to create or open a folder.
		if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,m_BaseOfFileName)==0)return;
	}
	GetLastFolderName(m_FolderPathWithoutFileName,m_FolderName);

	//#########  Examine if there is an existing file with the same name.  #########
	FILE *fp;
	if((fp=fopen(m_FileNameFULL,"r"))!= NULL){
		fclose(fp);//First, close the file.
		CAppendOverwriteCancel SaveChoice;
		if(SaveChoice.DoModal()==IDCANCEL){
			return;
		}
		else if(SaveChoice.m_Choice==OVERWRITE_FILE){
			fp=fopen(m_FileNameFULL,"w"); fclose(fp); //It will delete the content
		}
		//else{} //The default action is to append.
	}
	//##############################################################################

	m_strFileName=m_FileNameFULL;

	//##########  Switch Recording Channel #######################################
	DesignateRecordingChannel(m_RecordingChanId);
	m_SaveIdx=M_usableWaveIdx[m_RecordingChanId];//Start from the currently available wavelet
	//##############################################################################

	m_numOfSavedWaves=0; m_numOfSavedSelectedWaves=0; m_numOf_ConsecutiveNonSelectedWaves_Saved=0;
	m_isSaving=1;

	RemeasureFileNameRect();
	RequestDisplayEvent(DISP_FILE_NAME);//To display the name of the file that has been selected.
}

void CDasView::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
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

int CDasView::LocateAndCreateFolder_IfItIsNotAlreayThere()
{
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;  // Get the current time from the operating system.
	CTime CurrTime( osBinaryTime ); //  CTime from C run-time time
	CString Date = CurrTime.Format( "%m%d%y" );	

	char tmpStr[1024],FldrName[1024];
	CString ExeDirString=GetExecutableName();
	strcpy(tmpStr,ExeDirString);
	if(wildcmp("*\Blip*",tmpStr)==1){
		RE_Search1:;
		if(wildcmp("*\Blip",tmpStr)==1){
			sprintf(FldrName,"%s",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search1;}
		sprintf(FldrName,"%s\\_Data_Recorded\\%s",FldrName,Date);
		//SetCurrentDirectory(FldrName);
		return CreateFolder_IfNotAlreayThere_Also_MakeFileName(FldrName, Date);
	}		

	return -1;//Failed to open or create a folder
}


CString CDasView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

int CDasView::CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FldrName[], CString Date)
{	
	int FileResult=CreateDirectory(FldrName,NULL);//Returns a non zero number if succesfull.
	if(FileResult==0){
		FileResult=GetLastError();
		if(FileResult!=183){
			AfxMessageBox("Unable to automatically open/create a folder to store data");//183 means the folder is there already.
			return -1;
		}
	}
	char FileSeqStr[128]; 		itoa(*M_NnNumberToRecord,FileSeqStr,10);
	sprintf(m_JustFileName,"%s%s.wv",m_BaseOfFileName,FileSeqStr);
	strcpy(m_FolderPathWithoutFileName,FldrName);
	sprintf(m_FileNameFULL,"%s\\%s",m_FolderPathWithoutFileName,m_JustFileName);
	return 0;
}

void CDasView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_isSaving==1);
}

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CDasView::GetLastFolderName(char* FolderPath, char *FolderName)
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
	strncpy_s(FldrName,i+1,&tmpStr[0],_TRUNCATE);
	_strrev(FldrName);
	strcpy_s(FolderName,100,FldrName);
	return 1;
}

void CDasView::OnFileOpen()
{
	if(m_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\DAS_cl_Config_Files\\Config_DAS_cl.txt", m_FileNameFULL);
		sprintf(m_ConfigJustFileName,"Config_DAS_cl.txt");
	}
	else{
		if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){return;}
		strcpy(m_ConfigJustFileName,m_JustFileName);
	}
	int clm,row,strLength,i, ChID;
	float dummyF;
	char dummySt[1024];
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileNameFULL,"r"))!= NULL){
		m_numCaptured=0; 
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
		fgets(dummySt,1023,fp);	strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}}
		m_GlobalTitle=dummySt;	
		fscanf(fp,"%s",dummySt );//	How_many_ROWs:	
		fscanf(fp,"%d",&m_dHowManyRows );			
		fscanf(fp,"%s",dummySt );//	How_many_COLUMNs:	
		fscanf(fp,"%d",&m_dHowManyClmns );		
		while(fscanf( fp,"%d%d",&row,&clm)!=EOF){
			if(row>MaxFigRow||clm>MaxFigCln){AfxMessageBox("row>MaxFigRow||clm>MaxFigCln."); exit(0);}
			pFig=&m_Fig[row][clm][0];
			fscanf(fp,"%s",dummySt );//	Title:	
			fgets(dummySt,1023,fp);	strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32/*control character*/){dummySt[i]='\0'; break;}}
			pFig->m_AssociatedChan.NameString=dummySt;	
			ChID=Find_Last_Num_inString(dummySt);
			if(ChID<0 || ChID>=NUM_NN_AI_CHAN){
				ChID=NUM_NN_AI_CHAN-1; 
				char Message1[100];
				sprintf(Message1, "%s's channel configuration is incompatable:\n ChID<0 || ChID>NUM_NN_AI_CHAN-1", m_ConfigJustFileName);
				AfxMessageBox(Message1);
			}
			pFig->m_ChanID=ChID;
			fscanf(fp,"%d%d%d%d",&pFig->m_nTx,&pFig->m_nTy,&pFig->m_nBx,&pFig->m_nBy);	
			fscanf(fp,"%d%d",&pFig->m_FontWidth,&pFig->m_FontHeight);	
			fscanf(fp,"%d%d",&pFig->m_howManyX,&pFig->m_howManyY);	
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cXaxis[0],""); else sprintf(pFig->m_cXaxis[0],dummySt);//The label of X axis
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cYaxis[0],""); else sprintf(pFig->m_cYaxis[0],dummySt);//The label of Y axis
			fscanf( fp,"%lf%lf%lf%lf",&pFig->m_dMinX,&pFig->m_dMaxX,&pFig->m_dMinY,&pFig->m_dMaxY);	 
			fscanf( fp,"%d",&pFig->m_Deleted);	
		}
		for(int i=0;i<m_dHowManyRows;i++){
			for(int j=0;j<m_dHowManyClmns;j++){
				pFig=&m_Fig[i][j][0];
				pFig->m_toDisplayThreshold=1; pFig->m_Threshold=-0.05;
				pFig->RegisterMinMaxYBase(); //Reflect the change of Y-scale.
		}	}
	}
	if(m_dHowManyClmns!=-1){
		fclose(fp);	
	}
	if(m_bFistTime!=1){
		Figure_Channel_Config();
	}
}

void CDasView::OnConfigFileSave()
{
	if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,"*.txt")==0){return;}
	int clm,row;
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileNameFULL,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)BLIP_VERSION );		//Version numbere
		if(strlen(m_GlobalTitle)==0)fprintf( fp, "GlobalTitle: NoName\n");
		else fprintf( fp, "GlobalTitle: %s\n",m_GlobalTitle);//fputs( LPCTSTR( m_user_input), fp);			
		fprintf( fp, "How_many_ROWs: %d\n",m_dHowManyRows );			
		fprintf( fp, "How_many_COLUMNs: %d\n\n",m_dHowManyClmns );		
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				fprintf( fp, "%d\t%d\n",row,clm);	
				if(pFig->m_AssociatedChan.NameString.GetLength()==0)fprintf( fp, "Title: NoName\n");
				else fprintf( fp, "Title: %s\n",pFig->m_AssociatedChan.NameString );			
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
//The full file name with the full pathway:            m_FolderPathWithoutFileName & m_FileNameFULL
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPathWithoutFileName
int CDasView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
{	
	CFileDialog dialog(IsToREAD, "wv", initialFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "All files(*.*)|*.*|| ",NULL);
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
		if(wildcmp(".", strbuf)){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileNameFULL);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileNameFULL)-strlen(m_JustFileName)+i]='\0';	
	
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

//This routine extracts numbers from a string. 
//There could be many isolated numbers scattered among letters, but it registers only the last one (number e.g. 123 from 76DR123.txt).
int CDasView::Find_Last_Num_inString(char *GivenString)
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
		char strbuf[1000];
		_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		AfxMessageBox(strbuf);
		return INT_MAX;
	}
	if(sign==1) return -num;
	else		return  num;
}

