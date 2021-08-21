#include "stdafx.h"
#include "RAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "RAS_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "RAS_View.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "Dialog_Box.h"
#include "StringEdit.h"
#include "AcceptOneNum.h"
#include "FigureProperty.h"
#include "Trial.h"
#include "VarDefinitionsSDF.h"
#include "Dialog_ECodeContitions.h"


void CRASView::OnOpen_RST_File()
{
	if(m_Stop_Or_Pause==0){
		OnStop_or_Pause();
		Sleep(100);//Give the main loop some time to pause
	}

	m_IsFromFile=1;
	//Set the default directory only for the first file selection.
	if(m_isFirst_RST==1){
		m_isFirst_RST=0;
		char tmpStr[1024], Data_Folder[1024];
		CString St=	GetExecutableName();
		strcpy(tmpStr,St);
		if(FromA_findB_addC_giveD(tmpStr, "Blip", "_Data_Recorded", m_FolderPathRST)==0){
			m_FolderPathRST[0]='\0';
		}
	}
	if(Read_RST()==0){
		return;
	}

	CClientDC objDC(this);
	CFigure *pFig;
	int	TrlIdxInMap,sizeOfMap;
	char NameOfMap[256];
	int		*pECode, *pSpkTimes, numEcodes ,numSpks, index, trl, Nn, *pSpk, InitEcode;//, FirstTimeOfTrial;
	int IsToCreate;
	__int64 *pETime, alignTime, eventTimeToMark,dumy;
	CSpksInTrialByChan *pSpkInChan;
	CTrial *pTrial;
	CSpksInTrialByChan *pChan;
	int	*pSpkArray;
	int row, clm,i,j, size;
	
	//Initialize Figures
	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Deleted==1)continue;
			pFig->m_dMinXMinus2Sigma=pFig->m_dMinX-MAX_SIGMA_x2; pFig->m_dMaxXPlus2Sigma=pFig->m_dMaxX+MAX_SIGMA_x2;
			size=pFig->m_dMaxX-pFig->m_dMinX+2*MAX_SIGMA_x2;
			pFig->m_HistOfSpikes.SetSize(size);
			pFig->m_SDF.SetSize(size);
			pFig->m_HistArraySize=size;
		}
	}
	//Initialize Histogram and spike array
	ClearHistory(0);	
	
	//This is to prevent the main RAS_Main() from re-initializaing parameters unless the user explicitly asks to clear the history
	m_IsFirstTimeRasterPlot=0;

	//Register (parce) the ECode condition for each figure 
	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Deleted==1)continue;
			pFig->ExtractECodeConditions(pFig->m_MustExiestECodes,ECODE_MUST_EXIEST);
			pFig->ExtractECodeConditions(pFig->m_MustNotExiestECodes,ECODE_MUST_NOT_EXIEST);
			pFig->ExtractECodeConditions(pFig->m_OneOfThemMustExiestECodes,ONE_OF_ECODES_MUST_EXIEST);
	}	}

	//Draw
	for(trl=0;trl<m_NumTrialsInList;trl++){
		pTrial=m_pTrial[trl];
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0]; 
				if(pFig->m_Deleted==1)continue;
				pECode=&(pTrial->m_EventCode[0]);
				pETime=&(pTrial->m_EventTimeInMs[0]);
				numEcodes=pTrial->m_numEventCodes;
				InitEcode=0; //FirstTimeOfTrial=1; 
				while(1){
					//##############  Filter with (1) trial type, (2) alignment, And find an event to mark  ###############
					if(ConditionsToMeet(pFig, pECode, pETime, numEcodes, InitEcode, alignTime, eventTimeToMark)==0){
						//if(FirstTimeOfTrial==1 && pFig->m_PrvActive==1){pFig->BlackZeroAxes(&m_DC[0]);pFig->RefreshFrame(&objDC, &m_DC[0]);	}
						goto NEXT_FIG;
					}

					//#############################################################
					//Determine which neuron to display
					numSpks=0;
					for(i=0;i<pTrial->m_numChans;i++){
						pChan=pTrial->m_pChan[i];
						if(pChan->m_ChanID==pFig->m_ChanID){
							for(Nn=0;Nn<pChan->m_numNns;Nn++){
								if(pChan->m_NnID[Nn]==pFig->m_NnID){
									numSpks=pChan->m_numSpikesOfNn[pChan->m_NnID[Nn]];
									pSpk=&(pChan->m_SpikeTimesNn[pChan->m_NnID[Nn]][0]);
								}
							}
						}
					}
					//#####################  Figure  #############################
					if(pFig->m_numSpkArrayLists<MAX_TRLS_STORED_IN_FIG){
						pFig->m_CurrIdxOfSpkTimes++;// == pFig->m_numSpkArrayLists
						if(DEFAULT_SPK_NUM_PER_TRIAL<numSpks)pFig->m_SpkTimes[pFig->m_numSpkArrayLists].SetSize(numSpks);
						else pFig->m_SpkTimes[pFig->m_numSpkArrayLists].SetSize(DEFAULT_SPK_NUM_PER_TRIAL);
						pSpkArray=&(pFig->m_SpkTimes[pFig->m_numSpkArrayLists][0]);
						pFig->m_numSpks[pFig->m_numSpkArrayLists]=numSpks;
						if(pFig->m_numSpkArrayLists!=pFig->m_CurrIdxOfSpkTimes){
							AfxMessageBox("pFig->m_numSpkArrayLists!=pFig->m_CurrIdxOfSpkTimes?");
						}
						pFig->m_EventTimeToMark[pFig->m_numSpkArrayLists]=(int)(eventTimeToMark-alignTime);
						pFig->m_numSpkArrayLists++;
					}
					else{
						index=pFig->m_CurrIdxOfSpkTimes+1;
						if(index>=MAX_TRLS_STORED_IN_FIG)index=0;
						pFig->m_CurrIdxOfSpkTimes=index;
						if(pFig->m_OldestSpkArrayIdx+1<MAX_TRLS_STORED_IN_FIG)pFig->m_OldestSpkArrayIdx++; else pFig->m_OldestSpkArrayIdx=0;
						if(pFig->m_SpkTimes[index].GetSize()<numSpks)pFig->m_SpkTimes[index].SetSize(numSpks);
						pSpkArray=&(pFig->m_SpkTimes[index][0]);
						pFig->m_numSpks[index]=numSpks;
						pFig->m_EventTimeToMark[index]=(int)(eventTimeToMark-alignTime);
					}
					for(i=0;i<numSpks;i++){
						pSpkArray[i]=(int)(pSpk[i]-alignTime);
					}
					//pFig->Frame_Graph2D(&m_DC[0],1);
					pFig->Build_SpkHistogram_withoutSDF(); 
					//pFig->RefreshFrame(&objDC, &m_DC[0]);
				}
				NEXT_FIG:;
			}
		}
	}
	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0]; 
			if(pFig->m_Deleted==1)continue;
			pFig->Make_SDF_from_Histogram(); 
		}
	}
	//#########################   Dispaly   ##############################
	//Because this function pauses the main loop (m_Stop_Or_Pause==1) "RequestDisplayEvent(REDRAW_ALL_CONTENT)" will not work.
	RedrawAllContent();
}

void CRASView::RemakeRasterAndSDF_OneFig(CFigure *pFig) 
{
	CClientDC objDC(this);
	int	TrlIdxInMap,sizeOfMap;
	char NameOfMap[256];
	int		*pECode, *pSpkTimes, numEcodes ,numSpks, index, trl, Nn, *pSpk, InitEcode;//, FirstTimeOfTrial;
	int IsToCreate;
	__int64 *pETime, alignTime, eventTimeToMark,dumy;
	CSpksInTrialByChan *pSpkInChan;
	CTrial *pTrial;
	CSpksInTrialByChan *pChan;
	int	*pSpkArray;
	int i,j, size;

	pFig->EmptyHistogramAndSpikes();
	for(trl=0;trl<m_NumTrialsInList;trl++){
		pTrial=m_pTrial[trl];
		if(pFig->m_Deleted==1)continue;
		pECode=&(pTrial->m_EventCode[0]);
		pETime=&(pTrial->m_EventTimeInMs[0]);
		numEcodes=pTrial->m_numEventCodes;
		InitEcode=0;//FirstTimeOfTrial=1; 
		while(1){
			//##############  Filter with (1) trial type, (2) alignment, And find an event to mark  ###############
			if(ConditionsToMeet(pFig, pECode, pETime, numEcodes, InitEcode, alignTime, eventTimeToMark)==0){
				//if(FirstTimeOfTrial==1 && pFig->m_PrvActive==1){pFig->BlackZeroAxes(&m_DC[0]);pFig->RefreshFrame(&objDC, &m_DC[0]);	}
				goto NEXT_TRL;
			}

			//#############################################################
			//Determine which neuron to display
			numSpks=0;
			for(i=0;i<pTrial->m_numChans;i++){
				pChan=pTrial->m_pChan[i];
				if(pChan->m_ChanID==pFig->m_ChanID){
					for(Nn=0;Nn<pChan->m_numNns;Nn++){
						if(pChan->m_NnID[Nn]==pFig->m_NnID){
							numSpks=pChan->m_numSpikesOfNn[pChan->m_NnID[Nn]];
							pSpk=&(pChan->m_SpikeTimesNn[pChan->m_NnID[Nn]][0]);
						}
					}
				}
			}
			//#####################  Figure  #############################
			if(pFig->m_numSpkArrayLists<MAX_TRLS_STORED_IN_FIG){
				pFig->m_CurrIdxOfSpkTimes++;// == pFig->m_numSpkArrayLists
				if(DEFAULT_SPK_NUM_PER_TRIAL<numSpks)pFig->m_SpkTimes[pFig->m_numSpkArrayLists].SetSize(numSpks);
				else pFig->m_SpkTimes[pFig->m_numSpkArrayLists].SetSize(DEFAULT_SPK_NUM_PER_TRIAL);
				pSpkArray=&(pFig->m_SpkTimes[pFig->m_numSpkArrayLists][0]);
				pFig->m_numSpks[pFig->m_numSpkArrayLists]=numSpks;
				if(pFig->m_numSpkArrayLists!=pFig->m_CurrIdxOfSpkTimes){
					AfxMessageBox("pFig->m_numSpkArrayLists!=pFig->m_CurrIdxOfSpkTimes?");
				}
				pFig->m_EventTimeToMark[pFig->m_numSpkArrayLists]=(int)(eventTimeToMark-alignTime);
				pFig->m_numSpkArrayLists++;
			}
			else{
				index=pFig->m_CurrIdxOfSpkTimes+1;
				if(index>=MAX_TRLS_STORED_IN_FIG)index=0;
				pFig->m_CurrIdxOfSpkTimes=index;
				if(pFig->m_OldestSpkArrayIdx+1<MAX_TRLS_STORED_IN_FIG)pFig->m_OldestSpkArrayIdx++; else pFig->m_OldestSpkArrayIdx=0;
				if(pFig->m_SpkTimes[index].GetSize()<numSpks)pFig->m_SpkTimes[index].SetSize(numSpks);
				pSpkArray=&(pFig->m_SpkTimes[index][0]);
				pFig->m_numSpks[index]=numSpks;
				pFig->m_EventTimeToMark[index]=(int)(eventTimeToMark-alignTime);
			}
			for(i=0;i<numSpks;i++){
				pSpkArray[i]=(int)(pSpk[i]-alignTime);
			}
			pFig->Build_SpkHistogram_withoutSDF();
		}
		NEXT_TRL:;
		pFig->Make_SDF_from_Histogram(); 
	}
}

int CRASView::Read_RST() 
{
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 
	CTrial *pTrial;
	char toPrint[64];
	int i,j, FirstLocation, NumUserVal;
	int *pData;
	__int64 initTime;
	
	//################################################################
	//Read the file.
	if(GetFile_WholePath_WithInitialFolder_Name(TOREAD,"Data File",m_FolderPathRST,"*.rst")==0)return 0;
	GetFolderName(m_FolderPathRST,m_JustFolderName);
	if(wildcmp("*.rst",m_JustFileName)==0){AfxMessageBox("This is not a *.rst file"); return 0;}
	CFile theFile;
	if(theFile.Open(m_FileName, CFile::modeRead | CFile::shareDenyNone)== FALSE){
		return 0;
	};
	CArchive archive(&theFile, CArchive::load);

	//Convert Date to number
	char Date6Digit[64];
	if(Find_first_6Digit_unintrupted_number(Date6Digit,m_JustFileName)==1){
		m_FileYearMonthDate=Convert_6digitDate_ToNumber(Date6Digit);
	}
	else{//Assume that the file has a new format.
		m_FileYearMonthDate=991231; // The name does not contain 6 digit date. ==> Assume it is a new type 
	}
	//Read the header from the file.
	m_NumTrialsInList=0;
	CArray<char, char& > m_Header;
	char dumpChar[64];
	int SizeOfData, SizeOfChar=sizeof(char), SizeOfInt=sizeof(int), TrialID;
	if(m_FileYearMonthDate>111216){ //If the file was made after Dec. 16, 2011: it has the "header".
 		archive.Read(&SizeOfData,SizeOfInt);
		m_Header.SetSize(SizeOfData+5);
 		if(m_FileYearMonthDate<120105){//Old type
			archive.Read(&(m_Header[0]),(SizeOfData+3)); 
		}
		else{
			archive.Read(&(m_Header[0]),(SizeOfData-1));
 			archive.Read(dumpChar,1);
		}
	}
	
	//Clean the Trial first
	POSITION aPos;
	aPos = m_TrialList.GetHeadPosition();
	while(aPos){delete m_TrialList.GetNext(aPos); }//It deletes the entities connected to the list.
	m_TrialList.RemoveAll(); //It deletes the list itself.

	//Now do the stuff for CTrial class
	while( archive.Read(&TrialID,SizeOfInt)!=0 ){
		pTrial=new CTrial();
		m_TrialList.AddTail(pTrial);
		m_pTrial[m_NumTrialsInList]=pTrial;
		pTrial->Serialize(archive, this);
		m_NumTrialsInList++;
	}
	archive.Close();
	theFile.Close();
	//################################################################

	//objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	m_DC[0].RestoreDC(nSaveDC); 
	return 1;
}

int CRASView::Convert_6digitDate_ToNumber(char *DateString) 
{	
	char date[64];
	int DateInNum;
	//strcpy(date,DateString);
	Copy_fromNth_HowMany(date,DateString,5,2);
	DateInNum=atoi(date)*10000;
	Copy_fromNth_HowMany(date,DateString,3,2);
	DateInNum+=atoi(date);
	Copy_fromNth_HowMany(date,DateString,1,2);
	DateInNum+=atoi(date)*100;
	m_FileYearMonthDate=DateInNum;
	return DateInNum;
}

int CRASView::Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany) 
{	
	strcpy(Dest,Source);
	strrev(Dest);
	Dest[strlen(Dest)-(Nth-1)]='\0';
	strrev(Dest);
	Dest[HowMany]='\0';
	return 0;
}

int CRASView::Find_first_6Digit_unintrupted_number(char *SixDigitString, char *NameString) 
{	
	int numDigits,Start, endI, i=0;
	endI=strlen(NameString);
	Re_Search:;
	numDigits=0; Start=i;
	for(i=Start;i<endI;i++){
		if(NameString[i]<'0' || NameString[i]>'9'){i++; goto Re_Search;}
		numDigits++;
		if(numDigits==6){
			Copy_fromNth_HowMany(SixDigitString,NameString,Start+1,6);
			break;
		}
	}

	if(numDigits==6)return 1;
	else  return 0;//Failed to Find
}