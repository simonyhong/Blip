
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

#define MAX_GAIN_VALUES	1000

char GlobalMessage2[1024];
UINT MessageThread2(LPVOID MessageStr)
{
	AfxMessageBox((char*) MessageStr);
	 return 0;
}

int CDasView::CountBoards()
{
	int devNum = 1;
	int NumBoards = 0;
	char channelList[512], tmpstr[512];
	// Counts how many boards are installed and available to be opened
	while(true)
	{
		// Generate board name
		sprintf(tmpstr, "Dev%d", devNum);

		// Get comma-separated list of channels
		if (DAQmxGetDevAIPhysicalChans(tmpstr, channelList, 500) != 0)
		{
			// device # does not exist.
			devNum++;
			if (devNum == 32)
				// Reached max.  Quit looking for boards
				break;

			// Try next device #
			continue;
		}

		// Increment # of boards and device #
		NumBoards++;
		if(NumBoards==1)sprintf(m_BoardName[0], "Dev%d", devNum); //By default, the program uses the first found device.
		devNum++;
	}

	return NumBoards;
}

void CDasView::ReportSummaryOfBoards()
{
	int devNum = 1;
	int BoardCounter=0;
	char tmpstr[1024];
	char productType[256], channelList[512];
	
	m_BoardIdxUsed=0;
	m_NumOfBoards=CountBoards();

	if(m_NumOfBoards>1){
		while(1){
			// Generate board name
			sprintf(m_BoardName[BoardCounter], "Dev%d", devNum);

			// Get comma-separated list of channels
			if(DAQmxGetDevAIPhysicalChans(m_BoardName[BoardCounter], channelList, 500) != 0){
				devNum++;
				// device # does not exist.
				if (devNum == 32){
					// Reached max.  Quit looking for boards
					break;
				}
				// Try next device #
				continue;
			}
			// Get type of device
			DAQmxGetDevProductType(m_BoardName[BoardCounter], productType, 256);

			if(BoardCounter==0){
				sprintf(GlobalMessage2, "This will be used: Device %s, type %s.  This is board %d of %d.\n If you would like to change this behavior and disable message, disable ReportSummaryOfBoards()function and put the name of the divice (e.g., m_BoardName[0]=\"Dev1\") directly",	m_BoardName[BoardCounter], productType, devNum, m_NumOfBoards);			
				AfxBeginThread( MessageThread2, GlobalMessage2);
			}
			//else{
			//	sprintf(GlobalMessage2, "Found device %s, type %s.  This is board %d of %d.",	m_BoardName[BoardCounter], productType, devNum, m_NumOfBoards);			
			//}
			
			devNum++;
			BoardCounter++;
		}
	}
}
