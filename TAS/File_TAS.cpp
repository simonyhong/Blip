#include "stdafx.h"
#include "TAS.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "TAS_Doc.h"
#include "Figure_TAS.h"


#include "PositionWindows.h"
#include "Channel.h"
#include "TAS_View.h"
#include "SinTable.h"
#include "DialogueBox.h"
#include <time.h>
#include "Task_Constants.h"
#include "VarDefinitions.h"

//Only the N (MAX_TRLS_IN_MAP) recent trials will be stored in the memory
void CTASView::EndTrial_SaveTrialDataToMem_and_InCaseFileIsOpenToFileAsWell()
{
	int TrlIdxInMap,err;
	int IsToCreate,FirstTimeMsg;
	//int a=	GetFileSize(M_StartTime[0],  NULL);
	if(*M_NumTrlsInMAP<MAX_TRLS_IN_MAP){
		TrlIdxInMap=*M_NumTrlsInMAP;
		IsToCreate=1,FirstTimeMsg=1;
		//Create a new trial data in memory MAP
		do{//This dummy while loop is given to mimic the loop that RAS has.
		#include "MemMap_Open_Trials.h"
		}while(0);
		(*M_NumTrlsInMAP)++;
	}
	else{
		//Over-write on an existing oldest trial entity in memory
		TrlIdxInMap=*M_usableTrialIdxInMem+1;
		if(TrlIdxInMap>=MAX_TRLS_IN_MAP)TrlIdxInMap=0;
		if(*M_numEcodes>M_ECodesMAP_size[TrlIdxInMap]){
			//if(!UnmapViewOfFile(hView_M_ECodesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			//CloseHandle(hFile_M_ECodesMAP[TrlIdxInMap]); 
			//if(!UnmapViewOfFile(hView_M_EventTimesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			//CloseHandle(hFile_M_EventTimesMAP[TrlIdxInMap]); 

			//Re-Create a new Ecode & ETime area in memory MAP
			IsToCreate=2;//2 means Re-Create
			#include "MemMap_Open_Ecodes.h"
		}
		int i,Nn, sizeOfMap;
		char NameOfMap[256];
		IsToCreate=2;//2 means Re-Create
		for(i=0;i<NUM_NN_AI_CHAN;i++){
			if(*M_SpkTimeOfNn1MAP_size[TrlIdxInMap][i]<*M_numSpikesOfNn1[i]){
				#include "MemMap_Open_SpikesNn1.h"
			}
			if(*M_SpkTimeOfNn2MAP_size[TrlIdxInMap][i]<*M_numSpikesOfNn2[i]){
				#include "MemMap_Open_SpikesNn2.h"
			}
			if(*M_SpkTimeOfNn3MAP_size[TrlIdxInMap][i]<*M_numSpikesOfNn3[i]){
				#include "MemMap_Open_SpikesNn3.h"
			}
		}
	}
	int	i,chan, Nn,endI;
	__int64 tmpTime=0;
	__int64 *pSpk, *pMSpk;
	*M_TrialID[TrlIdxInMap]=m_trialID;//Starts with 0
	//Event codes etc.
	for(chan=0;chan<*M_numNeuralChannels;chan++){
		if(*M_usableSpikeIdxOfNn1[chan]>=0 && tmpTime<*(M_spikeTimeStampOfNn1[chan]+*M_usableSpikeIdxOfNn1[chan])){
			tmpTime=*(M_spikeTimeStampOfNn1[chan]+*M_usableSpikeIdxOfNn1[chan]);
		}
		if(*M_usableSpikeIdxOfNn2[chan]>=0 && tmpTime<*(M_spikeTimeStampOfNn2[chan]+*M_usableSpikeIdxOfNn2[chan])){
			tmpTime=*(M_spikeTimeStampOfNn2[chan]+*M_usableSpikeIdxOfNn2[chan]);
		}
		if(*M_usableSpikeIdxOfNn3[chan]>=0 && tmpTime<*(M_spikeTimeStampOfNn3[chan]+*M_usableSpikeIdxOfNn3[chan])){
			tmpTime=*(M_spikeTimeStampOfNn3[chan]+*M_usableSpikeIdxOfNn3[chan]);
		}
	}
	*M_StartTime[TrlIdxInMap]=M_EventTimes[*M_FirstEcodeIdxOfTrial];
	*M_EndTime[TrlIdxInMap]  =BiggerOne_int64(tmpTime,M_EventTimes[*M_FirstEcodeIdxOfTrial+*M_numEcodes-1]);
	*M_numEcodesMAP[TrlIdxInMap]=*M_numEcodes;
	int		*pECode=M_ECodesMAP[TrlIdxInMap],a ;
	__int64 *pETime=M_EventTimesMAP[TrlIdxInMap],k;
	for(i=0;i<*M_numEcodes;i++){
		pECode[i]=M_EventCodes[*M_FirstEcodeIdxOfTrial+i];
		pETime[i]=M_EventTimes[*M_FirstEcodeIdxOfTrial+i];
	}
	//Spikes
	CSpksInTrialByChan *pSpkInChan;
	int NumSpks, firstSpkIdx, numChansWithSpikes=0,firstNeuronInChan;
	for(chan=0;chan<*M_numNeuralChannels;chan++){
		firstNeuronInChan=1;
		*M_NumNnsInChanMAP[TrlIdxInMap][chan]=0;
		for(Nn=0;Nn<3;Nn++){
			//First neuron of the channel
			if(Nn==0){     NumSpks=*M_numSpikesOfNn1[chan]; *M_numSpksOfNn1MAP[TrlIdxInMap][chan]=NumSpks; if(NumSpks<1)continue; firstSpkIdx=*M_firstSpkIdxInTrialOfNn1[chan]; pSpk=M_SpkTimeOfNn1MAP[TrlIdxInMap][chan]; pMSpk=(M_spikeTimeStampOfNn1[chan]+firstSpkIdx);}
			else if(Nn==1){NumSpks=*M_numSpikesOfNn2[chan]; *M_numSpksOfNn2MAP[TrlIdxInMap][chan]=NumSpks; if(NumSpks<1)continue; firstSpkIdx=*M_firstSpkIdxInTrialOfNn2[chan]; pSpk=M_SpkTimeOfNn2MAP[TrlIdxInMap][chan]; pMSpk=(M_spikeTimeStampOfNn2[chan]+firstSpkIdx);}
			else {         NumSpks=*M_numSpikesOfNn3[chan]; *M_numSpksOfNn3MAP[TrlIdxInMap][chan]=NumSpks; if(NumSpks<1)continue; firstSpkIdx=*M_firstSpkIdxInTrialOfNn3[chan]; pSpk=M_SpkTimeOfNn3MAP[TrlIdxInMap][chan]; pMSpk=(M_spikeTimeStampOfNn3[chan]+firstSpkIdx);}
			if(firstNeuronInChan==1){*M_ChanIDMAP[TrlIdxInMap][numChansWithSpikes++]=chan; firstNeuronInChan=0;}
			*M_NnIdWithSpk[TrlIdxInMap][numChansWithSpikes-1][*M_NumNnsInChanMAP[TrlIdxInMap][numChansWithSpikes-1]]=Nn;
			(*M_NumNnsInChanMAP[TrlIdxInMap][numChansWithSpikes-1])++;
			a=*M_SpkTimeOfNn1MAP_size[TrlIdxInMap][chan];
			err=GetLastError();
			if(NumSpks>(*M_Spike_QueueSize)-2){
				if(NumSpks>(*M_Spike_QueueSize))AfxMessageBox("NumSpks>(*M_Spike_QueueSize)?\n This should not happen, because DAS_sv does not keep more than MAX_SPIKEs");
				else AfxMessageBox("There have been too many spikes (>MAX_SPIKEs) in this trial.\n The spikes occurred later part of the trial have neen truncated. \n To avoid this situation, you can either shorten the duration of the recording for each trial, or increase MAX_SPIKEs, or avoid junk signals");
			}
			if(firstSpkIdx+NumSpks<(*M_Spike_QueueSize)){
				for(i=0;i<NumSpks;i++){
					//k=pMSpk[i];
					//pSpk[i]=k;
					pSpk[i]=pMSpk[i];
				}
			}
			else{
				endI=(*M_Spike_QueueSize)-firstSpkIdx;
				for(i=0;i<endI;i++){
					//k=pMSpk[i];
					//pSpk[i]=k;
					pSpk[i]=pMSpk[i];
				}
				endI=NumSpks-endI;
				if(Nn==0){     pSpk=&M_SpkTimeOfNn1MAP[TrlIdxInMap][chan][i]; pMSpk=(M_spikeTimeStampOfNn1[chan]+0);}
				else if(Nn==1){pSpk=&M_SpkTimeOfNn2MAP[TrlIdxInMap][chan][i]; pMSpk=(M_spikeTimeStampOfNn2[chan]+0);}
				else {         pSpk=&M_SpkTimeOfNn3MAP[TrlIdxInMap][chan][i]; pMSpk=(M_spikeTimeStampOfNn3[chan]+0);}
				for(i=0;i<endI;i++){
					pSpk[i]=pMSpk[i];
				}
			}
		}
		*M_NumNnChansWithSpikesMAP[TrlIdxInMap]=numChansWithSpikes;
	}

	//Analog Signals (Eye positions)
	if(*M_numBehaviorChans>0 && *M_firstBehaviorAiIdxInTrial>-1){
		m_numBehaviorChannels=*M_numBehaviorChans;
		m_firstBehaviorAiIdx=*M_firstBehaviorAiIdxInTrial;
		if(*M_firstBehaviorAiIdxInTrial<0||*M_usableAiIdx[FIRST_BEHAVIOR_CHAN_IDX]<0)return;//The DAS has not started, or just started.
		m_lastBehaviorAiIdx=*M_usableAiIdx[FIRST_BEHAVIOR_CHAN_IDX];
		m_BehaviorSamplingRate=*M_BehaviorSamplingRate;
		m_StartTimeOfAI=M_timeStamp4BehaviorAI_MicS[m_firstBehaviorAiIdx];//Currently, all Behavior Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
		m_EndTimeOfAI  =M_timeStamp4BehaviorAI_MicS[m_lastBehaviorAiIdx];//Currently, all Behavior Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
	}
	*M_usableTrialIdxInMem=TrlIdxInMap;
	*M_isThereNewTrial=0;
	if(m_isSaving==1){
		PostMessage(WM_MYMESSAGE_NEW_TRIAL_TO_SAVE, *M_usableTrialIdxInMem, (LPARAM)this);//To trigger the save function.
	}
	else m_NumUserData=0; //This line resets the parameters of the user data (created by Name_and_Value_to_record() or Name_and_ArrayOfValues_to_record() ). 
}

#include "AppendOverwriteCancel.h"
void CTASView::OnSaveTrialsToFile()
{
	if(m_isTAS_Running==0)return;
	if(m_isSaving==1){//Turn it off!! It has the same effect as pushing the "close" button from the menu.
		m_isSaving=0; 
		RequestDisplayEvent(DISP_FILE_NAME);
		RequestDisplayEvent(REFRESH_XY_TITLE_AREA);//To display the base of the file name.
		return;
	}

	Make_TheBaseOfFileName();
	int result=LocateAndCreateFolder_IfItIsNotAlreayThere();
	if(result==-1){//Failed to create or open a folder.
		if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,m_BaseOfFileName)==0)return;
	}
	GetLastFolderName(m_FolderPathWithoutFileName,m_LastPartOfFolderName);

	//#########  Examine if there is an existing file with the same name.  #########
	FILE *fp;
	fp=fopen(m_FileNameFULL,"r");
	m_strFileName=m_FileNameFULL;
	if(fp!= NULL){
		fclose(fp);//First, close the file.
		CAppendOverwriteCancel SaveChoice;
		if(SaveChoice.DoModal()==IDCANCEL){
			return;
		}
		else if(SaveChoice.m_Choice==OVERWRITE_FILE){
			fp=fopen(m_FileNameFULL,"w");fclose(fp);//Clear the content of the file.
			Serialize_Header();//Write a header.
		}
		//else{} //The default action is to append.
	}
	else{//New file
		Serialize_Header();
	}
	//##############################################################################

	*M_isThereNewTrial=0; //Nothing to save for now
	m_isSaving=1;
	RequestDisplayEvent(DISP_FILE_NAME);//This needs to come after "m_isSaving=1" because CFigure::DisplayFileName() uses this info to draw the LED
	RequestDisplayEvent(REFRESH_XY_TITLE_AREA);
}

int CTASView::LocateAndCreateFolder_IfItIsNotAlreayThere()
{
	CString Date = GetDateString();	

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
CString CTASView::GetDateString()
{
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;  // Get the current time from the operating system.
	CTime CurrTime( osBinaryTime ); //  CTime from C run-time time
	CString Date = CurrTime.Format( "%m%d%y" );	
	return Date;
}

CString CTASView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

int CTASView::CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FldrName[], CString Date)
{	
	int FileResult=CreateDirectory(FldrName,NULL);//Returns a non zero number if succesfull.
	if(FileResult==0){
		FileResult=GetLastError();
		if(FileResult!=183){
			AfxMessageBox("Unable to automatically open/create a folder to store data");//183 means the folder is there already.
			return -1;
		}
	}
	if(m_prvDate!=Date){
		m_prvDate=m_Date; *M_NnNumberToRecord=1;
		//Since the file sequence number has been changed remake the file name. 
		RemakeFileName();
	}//When the date changes, the file sequence resets.
	strcpy(m_FolderPathWithoutFileName,FldrName);
	sprintf(m_FileNameFULL,"%s\\%s",m_FolderPathWithoutFileName,m_JustFileName);
	return 0;
}

void CTASView::Make_TheBaseOfFileName()
{
	m_Date = GetDateString();	
	strcpy(m_BaseOfFileName,m_PreFix);
	strcat(m_BaseOfFileName,m_Date); 
	//strcat(m_BaseOfFileName,"_");
	strcat(m_BaseOfFileName,m_SuFFix);
	strcpy(M_BaseOfFileName,m_BaseOfFileName);
	if(m_isTAS_Running==1){//"if(m_FlagVirMemDC==0)" prevents the program trying to access this part before going through "VirMemDC()"
		RemakeFileName();
	}
	RequestDisplayEvent(DISP_FILE_NAME);
	RequestDisplayEvent(REFRESH_XY_TITLE_AREA);
}

void CTASView::RemakeFileName()
{	
	m_PrvNnNumberToRecord=*M_NnNumberToRecord;
	char NnStr[10]; itoa(*M_NnNumberToRecord,NnStr,10);
	sprintf(m_JustFileName,"%s%s.rst",m_BaseOfFileName,NnStr);
	strcpy(M_JustFileName,m_JustFileName);

	Inform_MOX("UpdateFileName");
}

void CTASView::UpdateFileName_StroredLocal()
{	
	strcpy(m_PreFix,M_PreFix);
	strcpy(m_SuFFix,M_SuFFix);
	strcpy(m_BaseOfFileName,M_BaseOfFileName);
	strcpy(m_JustFileName,M_JustFileName);
}

void CTASView::Give_Prefix_Suffix_forFileName(char prefix[], char suffix[])
{	
	strcpy(m_PreFix,prefix);	strcpy(M_PreFix,prefix);
	strcpy(m_SuFFix,suffix);	strcpy(M_SuFFix,suffix);
	Make_TheBaseOfFileName();
}

LRESULT CTASView::SaveOneTrial(WPARAM TrialIdxInMem, LPARAM pView)
{	
	CFile theFile;
	theFile.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive archive(&theFile, CArchive::store);

	((CTASView *)pView)->Serialize(archive);
	
	archive.Close();
	theFile.Close();

	m_NumUserData=0; //This line resets the parameters of the user data (created by Name_and_Value_to_record() or Name_and_ArrayOfValues_to_record() ). 
	return 1;
}


void CTASView::Serialize_Header()
{
	CFile theFile;
	theFile.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive archive(&theFile, CArchive::store);

	archive << m_LengthOfHeader;
	archive << m_Header;

	archive.Close();
	theFile.Close();
}

void CTASView::Serialize(CArchive& ar)
{
	int TrlIdxInMap=*M_usableTrialIdxInMem;
	int i,j, numEcode;
    // Now do the stuff for CTrial class
	if( ar.IsStoring() ){
        ar << *M_TrialID[TrlIdxInMap];
        ar << *M_StartTime[TrlIdxInMap];
        ar << *M_EndTime[TrlIdxInMap];
		//###### ECODES
        ar << *M_numEcodesMAP[TrlIdxInMap];
		numEcode=*M_numEcodesMAP[TrlIdxInMap];
		int		*pECode=M_ECodesMAP[TrlIdxInMap];
		__int64 *pETime=M_EventTimesMAP[TrlIdxInMap];
		for(i=0;i<numEcode;i++){
			ar << pECode[i];
		}
		for(i=0;i<numEcode;i++){
			ar << pETime[i];
		}
		//###### USER VALUES
        ar << m_NumUserData;
		for(i=0; i<m_NumUserData; i++){
			ar << m_SizeOfName[i];
			ar << m_NameArray[i];
			ar << m_NumOfValues[i];
			for(j=0; j<m_NumOfValues[i]; j++){
				ar << m_ValuesToRecord[m_FirstLocationOfValue[i]+j];
			}
			ar << m_TimeOfOccurrence[i];
		}

		//####### Spikes
		int	numChan=*M_NumNnChansWithSpikesMAP[TrlIdxInMap];
		ar << numChan;
		int	chan,Nn, NumSpks,numNns,chanIdxWithSpk,NnID;
		__int64* pSpk;
		for(chan=0;chan<numChan;chan++){
			chanIdxWithSpk=*M_ChanIDMAP[TrlIdxInMap][chan];
			ar << chanIdxWithSpk;
			numNns=*M_NumNnsInChanMAP[TrlIdxInMap][chanIdxWithSpk];
			ar << numNns;
			for(Nn=0;Nn<numNns;Nn++){
				NnID=*M_NnIdWithSpk[TrlIdxInMap][chanIdxWithSpk][Nn];
				if(NnID==0){     NumSpks=*M_numSpksOfNn1MAP[TrlIdxInMap][chanIdxWithSpk]; pSpk=M_SpkTimeOfNn1MAP[TrlIdxInMap][chanIdxWithSpk];}
				else if(NnID==1){NumSpks=*M_numSpksOfNn2MAP[TrlIdxInMap][chanIdxWithSpk]; pSpk=M_SpkTimeOfNn2MAP[TrlIdxInMap][chanIdxWithSpk];}
				else {			 NumSpks=*M_numSpksOfNn3MAP[TrlIdxInMap][chanIdxWithSpk]; pSpk=M_SpkTimeOfNn3MAP[TrlIdxInMap][chanIdxWithSpk];}
				ar << NnID;// neuron ID
				ar << NumSpks;
				if(NumSpks<1)AfxMessageBox("NumSpks<1 ?");
				for(i=0;i<NumSpks;i++){	ar << pSpk[i];	}
			}
		}

		//####### Analog Input regarding the behavior
		if(m_numBehaviorChannels<1 || m_firstBehaviorAiIdx<0)return;
		double *pAI;
		int endI,numDataPointsOfBehaviorAI;
		ar << m_BehaviorSamplingRate;
		ar << m_StartTimeOfAI;
		ar << m_EndTimeOfAI;
		ar << m_numBehaviorChannels;
		if(m_lastBehaviorAiIdx>m_firstBehaviorAiIdx){
			numDataPointsOfBehaviorAI=m_lastBehaviorAiIdx-m_firstBehaviorAiIdx;//Currently, all Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
			ar << numDataPointsOfBehaviorAI;
			for(chan=0;chan<m_numBehaviorChannels;chan++){
				ar << chan;
				pAI=M_AI[FIRST_BEHAVIOR_CHAN_IDX+chan];
				for(i=m_firstBehaviorAiIdx;i<m_lastBehaviorAiIdx;i++){
					ar << (int)(1000*pAI[i]);
				}
			}
		}
		else{
			numDataPointsOfBehaviorAI=M_AIQueueSize[FIRST_BEHAVIOR_CHAN_IDX]-m_firstBehaviorAiIdx+m_lastBehaviorAiIdx;//Currently, all Analog input channels have the same value of M_firstBehaviorAiIdxInTrial
			ar << numDataPointsOfBehaviorAI;
			endI=M_AIQueueSize[FIRST_BEHAVIOR_CHAN_IDX];
			for(chan=0;chan<m_numBehaviorChannels;chan++){
				ar << chan;
				pAI=M_AI[FIRST_BEHAVIOR_CHAN_IDX+chan];
				for(i=m_firstBehaviorAiIdx;i<endI;i++){
					ar << (int)(1000*(int)(1000*pAI[i]));
				}
				for(i=0;i<m_lastBehaviorAiIdx;i++){
					ar << (int)(1000*pAI[i]);
				}
			}
		}
	}
	//Load the data
	else{
       // ar >> m_TrialID >> m_BehaviorSamplingRate;
	}
}

void CTASView::Write_Your_Headr_Here_to_Write_on_the_File(char *header)
{
	if(strlen(m_Header)==0){
		m_Header.Format("BLIP Version: %d; ",BLIP_CONSTANTS);
		m_Header+="Date recorded: "+GetDateString()+"; ";
		m_Header+=header; m_Header+="; ";
	}
	else{
		m_Header+=header; m_Header+="; ";
	}
	m_LengthOfHeader=m_Header.GetLength();
}

void CTASView::Name_and_ArrayOfValues_to_record(char Name[], double ArrayOfValues[], int N) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_ArrayOfValues_to_record(char Name[], float ArrayOfValues[], int N) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_ArrayOfValues_to_record(char Name[], int ArrayOfValues[], int N) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;
	#include "Name_and_ArrayOfValues_to_Record.h"
}

void CTASView::Name_and_Value_to_record(char Name[], double value) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=1; 
	double ArrayOfValues[1]={value};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=2; 
	double ArrayOfValues[2]={V1, V2};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=3; 
	double ArrayOfValues[3]={V1, V2, V3};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=4; 
	double ArrayOfValues[4]={V1, V2, V3, V4};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=5; 
	double ArrayOfValues[5]={V1, V2, V3, V4, V5};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=6; 
	double ArrayOfValues[6]={V1, V2, V3, V4, V5, V6};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=7; 
	double ArrayOfValues[7]={V1, V2, V3, V4, V5, V6, V7};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=8; 
	double ArrayOfValues[8]={V1, V2, V3, V4, V5, V6, V7, V8};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=9; 
	double ArrayOfValues[9]={V1, V2, V3, V4, V5, V6, V7, V8, V9};
	#include "Name_and_ArrayOfValues_to_Record.h"
}
void CTASView::Name_and_Value_to_record(char Name[], double V1, double V2, double V3, double V4, double V5, double V6, double V7, double V8, double V9, double V10) 
{	
	m_TimeOfOccurrence[m_NumUserData]=*M_LatestTimeStamp;

	int N=10; 
	double ArrayOfValues[10]={V1, V2, V3, V4, V5, V6, V7, V8, V9, V10};
	#include "Name_and_ArrayOfValues_to_Record.h"
}

//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPathWithoutFileName & m_FileNameFULL
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPathWithoutFileName
int CTASView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
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
		if(wildcmp(".", strbuf)){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileNameFULL);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileNameFULL)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CTASView::GetLastFolderName(char* FolderPath, char *LastFolderName)
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
	strcpy_s(LastFolderName,100,FldrName);
	return 1;
}

//This one discards the parent folder name out of the full pathway "FolderPath"
int CTASView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
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
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
int CTASView::Find_Last_Num_inString(char *GivenString, int &NumToReturn)
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
int CTASView::Find_Last_PositiveNum_inString(char *GivenString, int &NumToReturn)
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
