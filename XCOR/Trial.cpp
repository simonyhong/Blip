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
#include "MainFrm.h"
#include "Dialog_Box.h"
#include "StringEdit.h"
#include "AcceptOneNum.h"
#include "Trial.h"
#include "VarDefinitionsSDF.h"


//The IMPLEMENT_SERIAL macro is used to define the various functions needed when you derive a serializable class from CObject. 
//The first two arguments to the macro are the name of the class and the name of its immediate base class. The third argument is a version number for objects of the class. Use an integer greater than or equal to 0. 
IMPLEMENT_SERIAL(CTrial, CObject, 1)

CSpksInTrialByChan::~CSpksInTrialByChan()
{
	for(int i=0;i<3;i++){
		m_SpikeTimesNn[i].RemoveAll();
	}
}
//////////////////////////////////////////////


CTrial::CTrial()
{
	m_numChans=0;
	m_numChansInList=0;
	
	m_SizeOfData=100;
	m_NumUserData=0;

	m_NameArray.SetSize(m_SizeOfData);
	m_SizeOfName.SetSize(m_SizeOfData); 
	m_ValuesToRecord.SetSize(m_SizeOfData);
	m_NumOfValues.SetSize(m_SizeOfData); 
	m_FirstLocationOfValue.SetSize(m_SizeOfData); 
	m_TimeOfOccurrenceInMs.SetSize(m_SizeOfData); 
}
CTrial::~CTrial()
{
	m_EventCode.RemoveAll();
	m_EventTimeInMs.RemoveAll();

	//////
	for(int i=0;i<m_NumUserData;i++)free(m_NameArray[i]);
	m_NameArray.RemoveAll();
	m_SizeOfName.RemoveAll();
	m_ValuesToRecord.RemoveAll();
	m_NumOfValues.RemoveAll();
	m_FirstLocationOfValue.RemoveAll();
	m_TimeOfOccurrenceInMs.RemoveAll();
}

void CTrial::Serialize( CArchive& archive, CXCOR_View *pView)
{
	if(archive.IsStoring() ){AfxMessageBox("This is just for loading"); return;}

	__int64 SomeTime;
	int i, dummyIntVar, DataSize, FirstValID, sizeOfChar=sizeof(char), sizeOfDouble=sizeof(double);
    //archive >> m_TrialID; //This one is done at Download_RST().
    archive >> SomeTime; m_StartTimeOfTrialInMs=0.001*SomeTime; 
    archive >> SomeTime; m_EndTimeOfTrialInMs=0.001*SomeTime;
    archive >> m_numEventCodes;
	if(m_EventCode.GetSize()<m_numEventCodes){m_EventCode.SetSize(m_numEventCodes); m_EventTimeInMs.SetSize(m_numEventCodes);}
	for(i=0;i<m_numEventCodes;i++){
		archive >> m_EventCode[i];
	}
	for(i=0;i<m_numEventCodes;i++){
		archive >> SomeTime; m_EventTimeInMs[i]=0.001*SomeTime;
	}

	//####### User Data
	if(pView->m_FileYearMonthDate>111216){ //If the file was made after Dec. 30, 2011: it has the "User variables".
		archive >> m_NumUserData;
		if(m_SizeOfData<=m_NumUserData){
			if(2*m_SizeOfData>m_NumUserData)m_SizeOfData*=2; else m_SizeOfData=2*m_NumUserData;
			m_NameArray.SetSize(m_SizeOfData);
			m_SizeOfName.SetSize(m_SizeOfData);
			m_FirstLocationOfValue.SetSize(m_SizeOfData);
			m_NumOfValues.SetSize(m_SizeOfData);
			m_TimeOfOccurrenceInMs.SetSize(m_SizeOfData);
		}
		int CntOf_ValuesToRecord=0;
		for(i=0; i<m_NumUserData; i++){
			archive >> DataSize;
			m_SizeOfName[i]=DataSize;
			char* buffer=(char*) malloc(DataSize+2);
			if(pView->m_FileYearMonthDate<120105){
				archive.Read(buffer, (DataSize+1)*sizeOfChar); //Old type
			}
			else{
				archive.Read(buffer, (DataSize)*sizeOfChar);//New type
			}
			buffer[DataSize]='\0';
			m_NameArray[i]=buffer;
			//archive.Read(buffer, (1)*sizeOfChar);
			archive >> DataSize;
			m_NumOfValues[i]=DataSize;
			FirstValID=CntOf_ValuesToRecord;
			if(FirstValID+DataSize>=m_ValuesToRecord.GetSize()){
				m_ValuesToRecord.SetSize(2*(FirstValID+DataSize));
			}
			archive.Read(&m_ValuesToRecord[FirstValID], DataSize*sizeOfDouble);
			m_FirstLocationOfValue[i]=CntOf_ValuesToRecord;
			CntOf_ValuesToRecord+=DataSize;
			archive >> SomeTime; m_TimeOfOccurrenceInMs[i]=0.001*SomeTime;
		}
	}
	//####### Spikes	
	archive >> m_numChans;
	if(m_numChans>MAX_CHAN){AfxMessageBox("m_numChans>MAX_CHAN"); return;}
	int	chan, numSpks, Nn, NnID,* pSpk;
	__int64 Spk;
	CSpksInTrialByChan *pChan;
	m_TotNumNsAcrossChans=0; m_NumNnChansWithSpikes=0;
	for(chan=0;chan<m_numChans;chan++){
		if(chan>m_numChansInList-1){
			pChan=new CSpksInTrialByChan();
			m_ChanList.AddTail(pChan);
			m_pChan[m_numChansInList]=pChan;
			m_numChansInList++;
		}
		pChan=m_pChan[chan];
		archive >> pChan->m_ChanID;
		archive >> pChan->m_numNns;
		m_TotNumNsAcrossChans+=pChan->m_numNns;
		for(Nn=0;Nn<pChan->m_numNns;Nn++){
			archive >> NnID;
			pChan->m_NnID[Nn]=NnID;
			archive >> numSpks;
			m_NumNnChansWithSpikes++;
			pChan->m_numSpikesOfNn[NnID]=numSpks; 
			if(numSpks<1)AfxMessageBox("numSpks<1");
			if(numSpks>0)m_NumNnChansWithSpikes++;
			if(pChan->m_SpikeTimesNn[NnID].GetSize()<numSpks)pChan->m_SpikeTimesNn[NnID].SetSize(numSpks); 
			pSpk=&(pChan->m_SpikeTimesNn[NnID][0]);
			for(i=0;i<numSpks;i++){	archive >> Spk;	pSpk[i]=(int)(0.001*Spk);}
		}
	}
	//####### Analog Input regarding the behavior
	int	chanID;
	int pAI;
	int endi;
	archive >> m_BehaviorSamplingRate;
	archive >> m_StartTimeOfAI_inMs; m_StartTimeOfAI_inMs*=0.001;
	archive >> m_EndTimeOfAI_inMs;	m_EndTimeOfAI_inMs*=0.001;
	archive >> m_numBehaviorChannels;
	if(pView->m_FileYearMonthDate<120222){
		archive >> m_numDataPointsOfBehaviorAI;
		m_BehavioralAI0.SetSize(m_numDataPointsOfBehaviorAI+1);
		m_BehavioralAI1.SetSize(m_numDataPointsOfBehaviorAI+1);
	}
	for(chan=0;chan<m_numBehaviorChannels;chan++){
		archive >> chanID;
		if(pView->m_FileYearMonthDate>=120222){
			archive >> m_numDataPointsOfBehaviorAI;
			m_BehavioralAI0.SetSize(m_numDataPointsOfBehaviorAI+1);
			m_BehavioralAI1.SetSize(m_numDataPointsOfBehaviorAI+1);
		}
		if(chan==0)     archive.Read(&m_BehavioralAI0[0], (m_numDataPointsOfBehaviorAI)*sizeof(int));
		else if(chan==1)archive.Read(&m_BehavioralAI1[0], (m_numDataPointsOfBehaviorAI)*sizeof(int));
		else{
			for(i=0;i<m_numDataPointsOfBehaviorAI;i++){
				archive >> pAI;
			}
		}
	}
}
