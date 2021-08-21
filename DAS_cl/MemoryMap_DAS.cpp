#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "Figure_DAS.h"
#include "Channel.h"
#include "DAS_View.h"


void CDasView::CreateSharedMemoryArea()
{
	//In case the memory has been created by some other process, try it by opening. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;	
	#include "MemMap_Open_DAS.h" 
}

void CDasView::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}

