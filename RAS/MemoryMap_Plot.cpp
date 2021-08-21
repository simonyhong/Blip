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
#include "Mmsystem.h" 


void CRASView::CreateSharedMemoryArea()
{
	int IsToCreate=0;
	#include "MemMap_Open_DAS.h" 
}

void CRASView::CloseSharedMemoryArea()
{
	#include "MemMap_Close_DAS.h" 
}

void CRASView::CloseTrialMemoryMAP()
{
	int	NumTrialsInMap=m_NumTrialsInMap_InThisProgram;
	#include "MemMap_Close_Trials.h" 
	m_NumTrialsInMap_InThisProgram=0;
}

