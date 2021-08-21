#include "stdafx.h"
#include "Analysis.h"


#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "MainFrm.h"

void CAnalysisView::CreateSharedMemoryArea()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
	#include "MemMap_Open.h" 
}


void CAnalysisView::CloseSharedMemoryArea()
{
	#include "MemMap_Close.h" 
}
