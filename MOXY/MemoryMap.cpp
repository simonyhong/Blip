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

#include <afxmt.h>
#include "Mmsystem.h" 


void CMOXY_View::CreateSharedMemoryArea()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
	#include "MemMap_Open_DAS.h" 
}


void CMOXY_View::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}

void CMOXY_View::CloseTrialMemoryMAP()
{
	int	NumTrialsInMap=*M_NumTrlsInMAP;
	#include "MemMap_Close_Trials.h" 
}



///////////////////////

void CMOXY_View::CreateSharedMemoryArea_4TASVIS()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
#include "MemMap_Open_TASMOXYVIS.h"
}    

void CMOXY_View::DestroySharedMemoryArea_4TASVIS ()
{
#include "MemMap_Close_TASMOXYVIS.h"
}
