#include "stdafx.h"
#include "XCOR_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "XCOR_View.h"
#include "Mmsystem.h" 


void CXCOR_View::CreateSharedMemoryArea()
{
	int IsToCreate=0;
	#include "MemMap_Open_DAS.h" 
}

void CXCOR_View::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}



