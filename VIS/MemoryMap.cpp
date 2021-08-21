#include "stdafx.h"
#include "VIS.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "VIS_Doc.h"
#include "Figures.h"
#include "SharedConstants.h"
#include "VIS_View.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "Mmsystem.h" 


void CVISView::CreateSharedMemoryArea()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
	#include "MemMap_Open_DAS.h" 
}


void CVISView::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}



///////////////////////


void CVISView::CreateSharedMemoryArea_4TASVIS()
{
	//First, try to open without creating. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
#include "MemMap_Open_TASMOXYVIS.h"
}    

void CVISView::CloseMemoryMAP_4TASMOSVIS ()
{
#include "MemMap_Close_TASMOXYVIS.h"
}
