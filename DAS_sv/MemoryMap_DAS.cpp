#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "VarDefinitions_SIG.h"
#include "Channel.h"
#include "DAS_View.h"
#include <NIDAQmx.h>



void CDasView::CreateSharedMemoryArea()
{
	//In case the memory has been created by some other process, try it by opening. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	IsToCreate=0;//This is a class member variable to be used to check if the memory area is already there or not.	
	#include "MemMap_Open_DAS.h" 
}

void CDasView::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}
