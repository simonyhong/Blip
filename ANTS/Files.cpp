
#include "stdafx.h"
#include "ANTS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ANTS_Doc.h"
#include "SharedConstants.h"
#include "Channel.h"
#include "Figure.h"
#include "ANTS_View.h"
#include "SinTable.h"

char GlobalMsg[1024];
UINT MessageThread(LPVOID MessageStr)
{
	AfxMessageBox((char*) MessageStr);
	 return 0;
}

UINT Save_Antidromic_Trial(LPVOID Ptr)
{	
	CAntsView* pView=(CAntsView*) Ptr;
	EnterCriticalSection(&(pView->m_CriticalSection4Save));
	//######

	pView->Save_Antidromic_StimulationSignal(pView->M_StimDigitalOutChannels[*pView->M_CurrIdx_Of_StimGateRelayID]);
	
	//######
	LeaveCriticalSection(&(pView->m_CriticalSection4Save));
	return 1;
}
UINT Save_Orthodromic_Trial(LPVOID Ptr)
{	
	CAntsView* pView=(CAntsView*) Ptr;
	EnterCriticalSection(&(pView->m_CriticalSection4Save));
	//######

	pView->Save_ORTHO_StimulationSignal(pView->M_StimDigitalOutChannels[*pView->M_CurrIdx_Of_StimGateRelayID]);
	
	//######
	LeaveCriticalSection(&(pView->m_CriticalSection4Save));
	return 1;
}


//Caution: 
// 1) Use only the local variables as much as possible. For example, in the case SaveOneTrial() is called by the task before the previous SaveOneTrial() is in progress, using non-local variables may affect the currently saving variables. 
// 2) Do not use AfxMessageBox(). This will hault the saving until the user clicks it away. Instead use a thread such as MessageThread();
void CAntsView::Save_Antidromic_StimulationSignal(int CurrentStimulationChannel)
{
	//Before anyting happens to the shared memory copy the contents
	long long EstimatedAntidromicStimPointIdx[NUM_NN_AI_CHAN];
	for(int ChanID=0; ChanID<NUM_NN_AI_CHAN; ChanID++)EstimatedAntidromicStimPointIdx[ChanID]=M_EstimatedAntidromicStimPointIdx[ChanID]; //This is to be used for the saving function 

	//Do the saving
	CFile theFile;
	theFile.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive ar(&theFile, CArchive::store);

	int StimulationChannel, RecChan, SaveIdx, firstTime=1;
	long long i, ChosenIdx, LengthOfData,Length, NunberOfIntDataBelow=1, cnt, DataConfiguation,WaitTimeInMs,DataPointsBetweenStimulationAndUsableAiIdx;
	double LinePos;
	long long initI, endI, endI_inclusive;

	//Record any recording channels represented by figures that have the same stimulating number as the currently stimulated channel. 
	for(SaveIdx=0; SaveIdx<m_numRecording_Stim_PairsToSave;SaveIdx++){
		//Retrieve the stimulation channel and recording channel numbers
		RecChan=m_pFigForSave[SaveIdx]->m_RecordingChan.m_ChanID;
		StimulationChannel=m_pFigForSave[SaveIdx]->m_StimGatingRelay.m_ChanID;
		if(CurrentStimulationChannel!=StimulationChannel)continue; //Look for a recording channel that is associated to this stimulating channel.
		if(firstTime==1){firstTime=0; ++m_numOfSavedTrials;}

		//Do the real saving procedure
		LinePos=m_pFigForSave[SaveIdx]->m_MeasuringLineTime[0];
		initI=((-LinePos+m_pFigForSave[SaveIdx]->m_dMinX)*1000)/(*M_microSecPerPoint); 
		endI=((-LinePos+m_pFigForSave[SaveIdx]->m_dMaxX)*1000)/(*M_microSecPerPoint);
		ChosenIdx=EstimatedAntidromicStimPointIdx[RecChan]; //see DAS_sv "Set_AntiDO_forXms_Function()". 

		m_pM_AI=M_AI[RecChan];
		LengthOfData=endI-initI; if(LengthOfData!=(int)LengthOfData){sprintf(GlobalMsg, "LengthOfData!=(int)LengthOfData");	AfxBeginThread( MessageThread, GlobalMsg);}
		ar << (int)m_numOfSavedTrials;										//(1) LFP ID
		ar << (int)DATA_SAMPLING_RATE;										//(2) Sampling rate
		ar << (int)RecChan;													//(3) Recording channel
		ar << (long long)M_timeStampMicS[RecChan][qIdx(ChosenIdx+initI)];	//(4) Time of the first signal
		ar << (int)LengthOfData;											//(5) Number of data points (data length)
		ar << (long long)M_timeStampMicS[RecChan][qIdx(ChosenIdx+0)];		//(6) Time of stimulation.
		ar << (float)LinePos;												//(7) Stimulation Latency (The delay beteen the detaction of the spike till the stimulation)
		ar << (int)VERSION_NUM_OF_ANTS;										//(8) Version number (int form) 
		ar << (int)	2;								                        //(9) Additional int Data (This is to make ANTS future proof, letting it record as many int values as it needs.)

		ar << (int)CurrentStimulationChannel;								//(10) Stimulating channel
		ar << (int)*M_AntidromicTriggeringChanID;							//(11) Triggering channel

		initI+=ChosenIdx; endI+=ChosenIdx;
		endI_inclusive=endI-1;
		if(initI>=0 && (endI_inclusive>=0 && endI_inclusive<*pM_AIQueueSize))DataConfiguation=ContinuousArry;
		else DataConfiguation=SplitArry;
		initI=qIdx(initI); endI_inclusive=qIdx(endI_inclusive);
		long long Distance=ShortestDistanceFrom_A_to_B(*M_usableAiIdx[RecChan], endI_inclusive, *pM_AIQueueSize);
		if(Distance>0)	WaitTimeInMs=(Distance*(*M_microSecPerPoint))*0.001+10;
		else WaitTimeInMs=0;
		if(WaitTimeInMs>3*60000){sprintf(GlobalMsg, "Wait for more than 3 minutes?");AfxBeginThread( MessageThread, GlobalMsg);}
		while(ShortestDistanceFrom_A_to_B(endI_inclusive, *M_usableAiIdx[RecChan], *pM_AIQueueSize)<0){Sleep(5); if(m_bIsON==0){ar.Close(); theFile.Close(); return;} }
		if(DataConfiguation==ContinuousArry){
			for(i=initI;i<=endI_inclusive;i++){//Relative range of indexes
				ar << (int)(1000*m_pM_AI[i]);
			}
		}
		//The begining portion of the data is at the end of the AI QueueOne, AND the latter part of the data is at the begining of the AI QueueOne
		else if(DataConfiguation==SplitArry){ 
			for(i=initI;i<*pM_AIQueueSize;i++){//Relative range of indexes
				ar << (int)(1000*m_pM_AI[i]);
			}
			for(i=0;i<=endI_inclusive;i++){//Relative range of indexes
				ar << (int)(1000*m_pM_AI[i]);
			}
		}
	}

	ar.Close();
	theFile.Close();
}

void CAntsView::Save_ORTHO_StimulationSignal(int CurrentStimulationChannel)
{	
	//Before anyting happens to the shared memory copy the contents
	long long EstimatedOrthodromicStimPointIdx[NUM_NN_AI_CHAN];
	for(int ChanID=0; ChanID<NUM_NN_AI_CHAN; ChanID++)EstimatedOrthodromicStimPointIdx[ChanID]=M_EstimatedOrthodromicStimPointIdx[ChanID]; //This is to be used for the saving function 

	//Do the saving
	CFile theFile;
	theFile.Open(m_strFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone); //CFile::shareDenyNone indicates that other processes can access while it is open by this program.
	theFile.SeekToEnd();
	CArchive ar(&theFile, CArchive::store);
	int StimulationChannel, RecChan, SaveIdx, firstTime=1;
	long long LengthOfData, futureUse_longlong=0, DataConfiguation;//0 is the stimulation point in the figure
	long long initI, endI_inclusive, i,NumDataPointsBeforeStim;
	long long DesiredPreStimLength, DesiredPostStimLength;
	__int64 InitTime;

	DesiredPreStimLength =(m_LFP_Start_Xms_toSave*1000)/(*M_microSecPerPoint); //This one will NOT apply for continuous recording; For continuous recording, the recording starts from the previous end point.
	DesiredPostStimLength=(m_LFP_End_Xms_toSave  *1000)/(*M_microSecPerPoint);

	//Record any recording channels represented by figures that have the same stimulating number as the currently stimulated channel. 
	for(int SaveIdx=0; SaveIdx<m_numRecording_Stim_PairsToSave;SaveIdx++){
		//Retrieve the stimulation channel and recording channel numbers
		RecChan=m_pFigForSave[SaveIdx]->m_RecordingChan.m_ChanID;
		StimulationChannel=m_pFigForSave[SaveIdx]->m_StimGatingRelay.m_ChanID;
		if(CurrentStimulationChannel!=StimulationChannel)continue; //Look for a recording channel that is associated to this stimulating channel.
		if(firstTime==1){firstTime=0; ++m_numOfSavedTrials;}
		
		//##########  Saving procedure #####################################################
		m_pM_AI=M_AI[RecChan];
		m_OrthoStimIdx=EstimatedOrthodromicStimPointIdx[RecChan];
		if(DesiredPreStimLength+DesiredPostStimLength>= 0.5*(*pM_AIQueueSize)){sprintf(GlobalMsg, "DesiredPreStimLength+DesiredPostStimLength>= 0.5*(*pM_AIQueueSize)"); AfxBeginThread( MessageThread, GlobalMsg); ar.Close(); theFile.Close(); return;}
		if(m_LFP_End_Xms_toSave<0){sprintf(GlobalMsg, "m_LFP_End_Xms_toSave<0"); AfxBeginThread( MessageThread, GlobalMsg);   ar.Close(); theFile.Close(); return;} //This function assumes a positive EndX.

		//From the 2nd time of the CONTINUOUS recording of the LFP, use the last idx+1 as the first starting point of saving. 
		//Also, if there has been an unexpected long heiatus ==> Restart the continuous recording.
		if(m_IsContinuousRecordingMode==1 && m_FirstRecordingInSequence==0 && abs(M_timeStampMicS[RecChan][m_OrthoStimIdx]-M_timeStampMicS[RecChan][m_LastIndexOfLFP])<(4*DesiredPreStimLength*(*M_microSecPerPoint))){
			initI=m_LastIndexOfLFP+1; 
		}
		else{// Discontinuous Recording 
			initI=m_OrthoStimIdx-DesiredPreStimLength; 
			if(m_IsContinuousRecordingMode==1 && m_FirstRecordingInSequence==1)m_FirstRecordingInSequence=0;
		}
		initI=qIdx(initI); 
		endI_inclusive=qIdx(m_OrthoStimIdx+(DesiredPostStimLength-1));
		//The data is stored linerly
		if(initI<endI_inclusive){
			DataConfiguation=ContinuousArry; 
			LengthOfData=endI_inclusive-initI+1;
			NumDataPointsBeforeStim=m_OrthoStimIdx-initI;
			if(initI>m_OrthoStimIdx || m_OrthoStimIdx>endI_inclusive){sprintf(GlobalMsg, "initI>m_OrthoStimIdx || m_OrthoStimIdx>endI_inclusive");	AfxBeginThread( MessageThread, GlobalMsg);}
		}
		//The data stored is segmented
		else{
			DataConfiguation=SplitArry;
			LengthOfData=(*pM_AIQueueSize-initI)+endI_inclusive+1;
			if(m_OrthoStimIdx>initI){NumDataPointsBeforeStim=m_OrthoStimIdx-initI;} else {NumDataPointsBeforeStim=(*pM_AIQueueSize-initI)+m_OrthoStimIdx;}
		}
		if(LengthOfData!=(int)LengthOfData){sprintf(GlobalMsg, "LengthOfData!=(int)LengthOfData");	AfxBeginThread( MessageThread, GlobalMsg);}
		if(NumDataPointsBeforeStim!=(int)NumDataPointsBeforeStim){sprintf(GlobalMsg, "NumDataPointsBeforeStim!=(int)NumDataPointsBeforeStim");	AfxBeginThread( MessageThread, GlobalMsg);}
		m_LastIndexOfLFP=endI_inclusive;
		InitTime=M_timeStampMicS[RecChan][initI];
		//#########  Wait if necessary
		long long WaitTimeInMs, Distance=ShortestDistanceFrom_A_to_B(*M_usableAiIdx[RecChan], endI_inclusive, *pM_AIQueueSize); //Wait until the end of the post-stimulation period
		if(Distance>0)	WaitTimeInMs=(Distance*(*M_microSecPerPoint))*0.001+1;
		else WaitTimeInMs=0;
		if(WaitTimeInMs>3*60000){sprintf(GlobalMsg, "Wait for more than 3 minutes?");AfxBeginThread( MessageThread, GlobalMsg);}
		while(ShortestDistanceFrom_A_to_B(endI_inclusive, *M_usableAiIdx[RecChan], *pM_AIQueueSize)<0){Sleep(5); if(m_bIsON==0){ar.Close();theFile.Close();return;} }
		//############################
		ar << (int)m_numOfSavedTrials;										// (1) LFP ID
		ar << (int)DATA_SAMPLING_RATE;										// (2) Sampling Rate
		ar << (long long)M_timeStampMicS[RecChan][m_OrthoStimIdx];			// (3) The time of stimulation 
		ar << (long long)M_timeStampMicS[RecChan][initI];					// (4) The time of the first signal
		ar << (long long)futureUse_longlong;								// (5) It is not reliable; do not use this.  ==> M_timeStampMicS[RecChan][initI]+ (*M_microSecPerPoint)*(LengthOfData-1)
		ar << (int)LengthOfData;											// (6) Data length
		ar << (int)NumDataPointsBeforeStim;									// (7) Number of data points before stimulation
		ar << (int) 3;														// (8) Number of int data below before LFP data. (This is to make ANTS future proof, letting it record as many int values as it needs.)

		ar << (int)VERSION_NUM_OF_ANTS;                                     // (9) Version number of ANTS (int form)
		ar << (int)CurrentStimulationChannel;	// (10) Stimulating channel
		ar << (int)RecChan;													// (11) Recording channel

		//The data is stored linerly
		if(DataConfiguation==ContinuousArry){
			for(i=initI;i<=endI_inclusive;i++){
				ar << (int)(1000*m_pM_AI[i]);
			}
		}
		//The begining portion of the data is at the end of the AI array, AND the latter part of the data is at the begining of the AI array
		else if(DataConfiguation==SplitArry){ 
			for(i=initI;i<*pM_AIQueueSize;i++){
				ar << (int)(1000*m_pM_AI[i]);
			}
			for(i=0;i<=endI_inclusive;i++){
				ar << (int)(1000*m_pM_AI[i]);
			}
		}
	}

	ar.Close();
	theFile.Close();
}


void CAntsView::OnFileOpen()
{
	if(m_is_LFP_saving==1){AfxMessageBox("Configuration change is not supported during saving"); return;}
	if(m_bFistTime==1){
		Find_Blip_andAddSome("\\_Config_Files\\ANTS_Config_Files\\Default_Config.txt", m_FileNameFULL);
		sprintf(m_ConfigJustFileName,"Default_Config.txt");
	}
	else{
		if(GetFile_WholePath_Name_CurPath(TOREAD,"*.txt")==0){return;}
		strcpy(m_ConfigJustFileName,m_JustFileName);
	}
	int clm,row,lyr,strLength,i;
	float dummyF, VerNum;
	char dummySt[1024];
	int StimulationChannels[10000], PostStimPeriod[10000], NumOfStimulationChannels;
	int TmpID, dummyInt, Number_of_Other_Int_variables;
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileNameFULL,"r"))!= NULL){
		m_numCaptured=0; m_NewlyLoaded=1;
		fscanf(fp,"%s",dummySt );//	Version_number:	
		if(!wildcmp("*Version*",dummySt))return;
		fscanf(fp,"%f",&VerNum);// Version numbere
		fscanf(fp,"%s",dummySt );//	GlobalTitle:	
		fgets(dummySt,1023,fp);		strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}} m_GlobalTitle=dummySt;	
		fscanf(fp,"%s",dummySt );//	How_many_LAYERs:	
		fscanf(fp,"%d",&m_dHowManyLayers );	
		for(lyr=0; lyr<m_dHowManyLayers; lyr++){
			fscanf(fp,"%s",dummySt );//	How_many_ROWs:	
			fscanf(fp,"%d",&(m_dHowManyRows[lyr]) );			
			fscanf(fp,"%s",dummySt );//	How_many_COLUMNs:	
			fscanf(fp,"%d",&(m_dHowManyClmns[lyr]) );		
		}
		for(lyr=m_dHowManyLayers; lyr<MaxFigLyr; lyr++){
			m_dHowManyRows[lyr]=0;			
			m_dHowManyClmns[lyr]=0;;		
		}
		fscanf(fp,"%s",dummySt );//	StimLatencyBias:	
		fscanf(fp,"%d",&m_StimulationLatencyBias_InMicroS);		
		fscanf(fp,"%s",dummySt );//	Number_of_stimulation_channels:	
		fscanf(fp,"%d",&NumOfStimulationChannels);		
		if(NumOfStimulationChannels<0 || NumOfStimulationChannels>MAX_NUM_STIM_CHANNELS){AfxMessageBox("NumOfStimulationChannels<0 || NumOfStimulationChannels>MAX_NUM_STIM_CHANNELS");	}
		for(i=0;i<NumOfStimulationChannels;i++){
			fscanf( fp, "%d%d",&StimulationChannels[i],&PostStimPeriod[i]);
		}
		int ChanCount=0;
		for(i=0;i<NumOfStimulationChannels;i++){
			if(StimulationChannels[i]>=DO_FIRST_GATING_CHAN && StimulationChannels[i]<=DO_LAST_GATING_CHAN){
				m_StimulationChannels[ChanCount]=StimulationChannels[i];
				if(PostStimPeriod[i]<MIMIMUM_POST_ANTI_DELAY)m_PostStimPeriod[i]=MIMIMUM_POST_ANTI_DELAY; else m_PostStimPeriod[ChanCount]=PostStimPeriod[i];
				if(++ChanCount==MAX_NUM_STIM_CHANNELS)break;
			}
		}
		//Temporarily accept the stimulation channel count. It will be validated at Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures() below.
		m_NumOfStimulationChannels=ChanCount;
		// *M_NumOfStimulationChannels=ChanCount; <=Can't do this because the shared memory may not be available yet.

		if(VerNum>=2){
			//The following is to make the freedom to add as many variables as possible, to make ANTS future proof.
			fscanf(fp,"%s",dummySt);		
			fscanf(fp,"%d", &Number_of_Other_Int_variables);//	Number_of_Other_Int_variables:	
			for(i=0;i<Number_of_Other_Int_variables;i++){
				fscanf( fp, "%d",&dummyInt);
				if(i==0)m_Lable_Of_TriggerChanNumber.m_ChanID=dummyInt;
			}
		}
		else m_Lable_Of_TriggerChanNumber.m_ChanID=0;

		while(fscanf( fp,"%d%d%d",&row,&clm,&lyr)!=EOF){
			if(row>MaxFigRow||clm>MaxFigCln){AfxMessageBox("row>MaxFigRow||clm>MaxFigCln."); exit(0);}
			pFig=&m_Fig[row][clm][lyr];
			
			fscanf(fp,"%s",dummySt );//	Recording_Channel:	
			fgets(dummySt,1023,fp);		strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}} pFig->m_RecordingChan.NameString=dummySt;	pFig->m_RecordingChan.NameString.TrimLeft();
			if(Find_Last_Num_inString(dummySt, TmpID)==-1)pFig->m_RecordingChan.m_ChanID=-1; 	else pFig->m_RecordingChan.m_ChanID=TmpID; 
			sprintf(dummySt,"RecCh:%d",pFig->m_RecordingChan.m_ChanID); pFig->m_RecordingChan.NameString=dummySt;
			
			fscanf(fp,"%s",dummySt );// Stimulating_Channel:	
			fgets(dummySt,1023,fp);		strLength=strlen(dummySt);for(i=strLength-1;i>=0;i--){	if(dummySt[i]<32){dummySt[i]='\0'; break;}} pFig->m_StimGatingRelay.NameString=dummySt;	pFig->m_StimGatingRelay.NameString.TrimLeft();
			if(Find_Last_Num_inString(dummySt, TmpID)==-1)pFig->m_StimGatingRelay.m_ChanID=-1; else pFig->m_StimGatingRelay.m_ChanID=TmpID; 
			sprintf(dummySt,"StmRly:%d",pFig->m_StimGatingRelay.m_ChanID); pFig->m_StimGatingRelay.NameString=dummySt;
			
			pFig->m_RecordingChan.NameString+=" "+pFig->m_StimGatingRelay.NameString; //pFig->m_RecordingChan.NameString stores BOTH the Stimulation & Recording names

			fscanf(fp,"%d%d%d%d",&pFig->m_nTx,&pFig->m_nTy,&pFig->m_nBx,&pFig->m_nBy);	
			fscanf(fp,"%d%d",&pFig->m_FontWidth,&pFig->m_FontHeight);	
			fscanf(fp,"%d%d",&pFig->m_howManyX,&pFig->m_howManyY);	
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cXaxis,""); else sprintf(pFig->m_cXaxis,dummySt);//The label of X axis
			fscanf( fp,"%s",dummySt);if(wildcmp("NULL", dummySt))sprintf(pFig->m_cYaxis,""); else sprintf(pFig->m_cYaxis,dummySt);//The label of Y axis
			fscanf( fp,"%lf%lf%lf%lf",&pFig->m_dMinX,&pFig->m_dMaxX,&pFig->m_dMinY,&pFig->m_dMaxY);	 
			fscanf( fp,"%d",&pFig->m_Deleted);	
		}
	}
	if(m_dHowManyClmns[0]!=-1){
		fclose(fp);	
		ConfigureLayerButtons(); //Configure the layer buttons
		EliminateExcluded();//This one has this: Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();//Inspect the figures and register proper Stimulation Channels to M_StimDigitalOutChannels[].
	}
	if(m_bFistTime==0){
		m_DisplayedStimLayer=0;//Use the first layer as the default display area.
		RequestDisplayEvent(REDRAW_ALL_LAYERS);
	}
	m_pSpotlightFig=&m_Fig[0][0][0];//This is added becasue sometimes, the selection click of a FILE affects OnMouseMove(), in that if(nFlags & MK_LBUTTON) clause becomes true even when I did not click the window.
}

//Configure the layer buttons
void CAntsView::ConfigureLayerButtons()
{
	char Chan[8];
	int LateralPosition=2*m_FontWidth;
	for(int i=0;i<MAX_NUM_STIM_CHANNELS;i++){
		m_LayerTab[i].NameString="    "; m_LayerTab[i].NameString+=itoa(i+1,Chan,10); m_LayerTab[i].NameString+="    ";
		RectOfLayerTabs(LateralPosition,m_LayerTab[i].NameString.GetLength(),m_LayerTab[i].StringRect); 
		LateralPosition=m_LayerTab[i].StringRect.right;//+2*m_FontWidth;
	}
}

void CAntsView::OnConfigFileSave()
{
	if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,"*.txt")==0){return;}
	int clm,row,lyr, i;
	CFigure *pFig;

	FILE *fp;
	//Try to append first, if there is an existing file.
	if((fp=fopen(m_FileNameFULL,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)VERSION_NUM_OF_ANTS );		//Version numbere
		if(strlen(m_GlobalTitle)==0)fprintf( fp, "GlobalTitle: NoName\n");
		else fprintf( fp, "GlobalTitle: %s\n",m_GlobalTitle);//fputs( LPCTSTR( m_user_input), fp);			
		fprintf( fp, "How_many_LAYERs: %d\n",m_dHowManyLayers );			
		for(lyr=0; lyr<m_dHowManyLayers; lyr++){
			fprintf( fp, "How_many_ROWs: %d\n",m_dHowManyRows[lyr] );			
			fprintf( fp, "How_many_COLUMNs: %d\n",m_dHowManyClmns[lyr] );		
		}
		fprintf( fp, "StimLatencyBias: %d\n\n",m_StimulationLatencyBias_InMicroS );		

		fprintf( fp, "Number_of_stimulation_channels: %d\n",m_NumOfStimulationChannels);
		for(i=0;i<m_NumOfStimulationChannels;i++){
			if(m_StimulationChannels[i]>=0 && m_PostStimPeriod[i]>=0){
				fprintf( fp, "%d\t%d\n",m_StimulationChannels[i],m_PostStimPeriod[i]);
			}	
			else {AfxMessageBox("m_StimulationChannels[i]<0 || m_PostStimPeriod[i]<0\n Saving failed."); break;}
		}
		fprintf( fp, "\n");	

		//######### The following is to make the freedom to add as many variables as possible, to make ANTS future proof.
		fprintf( fp, "Number_of_Other_Int_variables: %d\n",1);
		fprintf( fp, "%d\n",m_Lable_Of_TriggerChanNumber.m_ChanID);

		//##############################   Figures  #######################################
		fprintf( fp, "\n");	
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					fprintf( fp, "%d\t%d\t%d\n",row,clm,lyr);	
					if(pFig->m_RecordingChan.NameString.GetLength()==0)fprintf( fp, "Recording_Channel: NoName\n");
					else fprintf( fp, "Recording_Channel: RecCh:%d\n",pFig->m_RecordingChan.m_ChanID );			
					if(pFig->m_StimGatingRelay.NameString.GetLength()==0)fprintf( fp, "Stimulating_Relay: NoName\n");
					else fprintf( fp, "Stimulating_Relay: StmRly:%d\n",pFig->m_StimGatingRelay.m_ChanID );			
					fprintf( fp, "%d\t%d\t%d\t%d\n",pFig->m_nTx,pFig->m_nTy,pFig->m_nBx,pFig->m_nBy);	
					fprintf( fp, "%d\t%d\n",pFig->m_FontWidth,pFig->m_FontHeight);	
					fprintf( fp, "%d\t%d\n",pFig->m_howManyX,pFig->m_howManyY);	
					if(strlen(pFig->m_cXaxis)==0)fprintf( fp, "NULL\n");
					else fprintf( fp, "%s\n",pFig->m_cXaxis);
					if(strlen(pFig->m_cYaxis)==0)fprintf( fp, "NULL\n");
					else fprintf( fp, "%s\n", pFig->m_cYaxis);
					
					fprintf( fp, "%0.3f\t%0.3f\t%0.3f\t%0.3f\n",pFig->m_dMinX,pFig->m_dMaxX,pFig->m_dMinY,pFig->m_dMaxY);	 
					fprintf( fp, "%d\n",pFig->m_Deleted);	
				}
			}
		}
	}
	fclose(fp);
}

void CAntsView::OnFileSave(){}

#include "AppendOverwriteCancel.h"
#include "ActivationOrCollision.h"

void CAntsView::AssessConfiguration()
{
	//Is it because DAS-client is not on?
	if(*M_isDAScl_Running==0){AfxMessageBox("Turn on DAS-client"); return;}
	int lyr, row, clm, FoundValidOne=0, StimIdx, idx, ChID;
	CFigure *pFig;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr]; 
				ChID=pFig->m_StimGatingRelay.m_ChanID;
				if(pFig->m_Deleted!=1 && ChID>=DO_FIRST_GATING_CHAN && ChID<=DO_LAST_GATING_CHAN && pFig->m_RecordingChan.m_ChanID>=0 && pFig->m_RecordingChan.m_ChanID<NUM_NN_AI_CHAN){
					if(M_NumOfFigsUsingThisChan[pFig->m_RecordingChan.m_ChanID]<=0){
						FoundValidOne=1; //This one has valid chaannels but the recording channel is not used by DAS-client
					}
				}
			}
		}
	}
	if(FoundValidOne==1)AfxMessageBox("There is no recording channel that the DAS-client is currently showing"); 
	else AfxMessageBox("Set the stimulating/recording channel IDs first"); 
	return;
}


void CAntsView::OnSaveLFP()
{
	if(*M_is_LFP_saving==1){//Turn it off!! It has the same effect as pushing the "close" button from the menu.
		*M_is_LFP_saving=0; m_is_LFP_saving=0; 
		RequestDisplayEvent(REDRAW_FILE_NAME);//To display the base of the file name.
		return;
	}
	
	//Is it correctly configured? If not, why?
	if(m_numRecording_Stim_PairsToSave<=0){
		AssessConfiguration();
		return;
	}

	//This line makes sure that when the user stops the recording in the "m_IsContinuousRecordingMode==1" and later resumes the recording, the first part of the signal should be recalculated instead of trying to save from the place recorded last.
	if(m_IsContinuousRecordingMode==1)m_FirstRecordingInSequence=1; else m_FirstRecordingInSequence=0;

	//Pick either antidromic activation or collision
	if(m_bIsTo_ANTI_Stimulate==1){
		CActivationOrCollision ActivationOrCollision;
		if(ActivationOrCollision.DoModal()==IDOK){strcpy(m_ActivationOrCollision,"act");}
		else {strcpy(m_ActivationOrCollision,"col");}
	}
	//Set: m_LFP_Start_Xms_toSave, m_LFP_End_Xms_toSave.
	else if(m_isOrthodromicStimulation==1){
		if(m_is_firstTimeOrthoStimulation_IntervalSetting==1){
			OnSetPrePostPeriods_of_OrthodromicStimulationLFP_ToSave();
		}
	}
	else{AfxMessageBox("Select Ortho/Anti-dromic first"); return;}

	Make_TheBaseOfFileName(1/*ToSave*/);
	int result=LocateAndCreateFolder_IfItIsNotAlreayThere();
	if(result==-1){//Failed to create or open a folder.
		if(GetFile_WholePath_Name_CurPath(NOT_TOREAD,m_BaseOfFileName)==0){AfxMessageBox("Failed to create or open a folder"); return;}
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
	m_numOfSavedTrials=0;
	*M_is_LFP_saving=1; m_is_LFP_saving=1; //Now inform the ANTS_main() to save!
	RequestDisplayEvent(REDRAW_FILE_NAME);
}


void CAntsView::OnUpdateSaveLFP(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_is_LFP_saving==1);
}


int CAntsView::LocateAndCreateFolder_IfItIsNotAlreayThere()
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

CString CAntsView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

int CAntsView::CreateFolder_IfNotAlreayThere_Also_MakeFileName(char FldrName[], CString Date)
{	
	int FileResult=CreateDirectory(FldrName,NULL);//Returns a non zero number if succesfull.
	if(FileResult==0){
		FileResult=GetLastError();
		if(FileResult!=183){
			AfxMessageBox("Unable to automatically open/create a folder to store data");//183 means the folder is there already.
			return -1;
		}
	}
	strcpy(m_FolderPathWithoutFileName,FldrName);
	sprintf(m_FileNameFULL,"%s\\%s",m_FolderPathWithoutFileName,m_JustFileName);
	return 0;
}

void CAntsView::Make_TheBaseOfFileName(int IsToSave)
{
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time( &osBinaryTime ) ;  // Get the current time from the operating system.
	CTime CurrTime( osBinaryTime ); //  CTime from C run-time time
	m_Date = CurrTime.Format( "%m%d%y" );
	strcpy(m_BaseOfFileName,m_PreFix);
	strcat(m_BaseOfFileName,m_Date);
	//strcat(m_BaseOfFileName,"_");
	strcat(m_BaseOfFileName,m_SuFFix);
	if(m_FlagVirMemDC==1){//"if(m_FlagVirMemDC==0)" prevents the program trying to access this part before going through "VirMemDC()"
		m_PrvNnNumberToRecord=*M_NnNumberToRecord;
		char NnStr[10]; itoa(*M_NnNumberToRecord,NnStr,10);
		if(m_bIsTo_ANTI_Stimulate==1 && IsToSave==1)sprintf(m_JustFileName,"%s%s%s.wv",m_BaseOfFileName,m_ActivationOrCollision,NnStr);
		else sprintf(m_JustFileName,"%s%s.wv",m_BaseOfFileName,NnStr);
		m_FileName.NameString=m_JustFileName;
		RectOfLabel_inTitleArea(0,2*m_FontHeight,m_FileName.NameString.GetLength(),m_FileName.StringRect); 
	}
	RequestDisplayEvent(REDRAW_FILE_NAME);
}

//This one discards the parent folder name out of the full pathway "FolderPath"
int CAntsView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
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

void CAntsView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_is_LFP_saving==1);
}

void CAntsView::OnFileClose()
{
	m_is_LFP_saving=0;
	RequestDisplayEvent(REDRAW_FILE_NAME);
}


//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CAntsView::GetLastFolderName(char* FolderPath, char *FolderName)
{
	char tmpStr[1000],FldrName[1000],tmpLowercase[1000];
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

//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPathWithoutFileName & m_FileNameFULL
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPathWithoutFileName
int CAntsView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
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

int CAntsView::wildcmp(char *wild, char *string) 
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
int CAntsView::Find_Last_PositiveNum_inString(char *GivenString)
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
		_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		AfxMessageBox(strbuf);
		return -1;
	}
	return num;
}

//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. 123 from 76DR123.txt).
int CAntsView::Find_Last_Num_inString(char *GivenString, int &NumToReturn)
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


void CAntsView::Find_Blip_andAddSome(char AddedFolderName[], char FullPath[])
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
