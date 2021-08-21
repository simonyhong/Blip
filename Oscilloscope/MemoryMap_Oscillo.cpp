#include "stdafx.h"
#include "Oscilloscope.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "OscilloscopeDoc.h"
#include "FiguresOsc.h"
#include "OscilloscopeView.h"
#include "SinTable.h"
#include "MainFrm.h"
#include "DialogueBox.h"
#include "StringEdit.h"


void COscilloscopeView::CreateSharedMemoryArea()
{
	//First try to open. 
	//In case there is no existing Map, there is a built-in mechanism in the include file that reverts the mode to a creating mode, and retries. 
	int IsToCreate=0;
	#include "MemMap_Open_DAS.h" 
}


void COscilloscopeView::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}
