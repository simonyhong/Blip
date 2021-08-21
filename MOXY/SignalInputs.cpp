
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
#include <time.h>

void CMOXY_View::Relay_ON_n_DisplayLED(int whichBIT)
{
	Relay_ON(whichBIT);
	m_whichDIO=whichBIT;
	RequestDisplayEvent(DIO_ON, whichBIT);
}
void CMOXY_View::Relay_OFF_n_DisplayLED(int whichBIT)
{
	Relay_OFF(whichBIT);
	m_whichDIO=whichBIT;
	RequestDisplayEvent(DIO_OFF, whichBIT); 
}

void CMOXY_View::Relay_ON(int whichBIT)
{
	if(whichBIT>=MAX_DO_NUM){AfxMessageBox("whichBIT>=MAX_DO_NUM"); return;}
	M_DO_Array[whichBIT]=RELAY_ON; //0 means on because NI acts as a voltage sink for the relay (70-ODC5 Grayhill)
	if(!SetEvent(m_M_DO_Active[whichBIT])){
		AfxMessageBox("SetEvent() failed");
	}
}
void CMOXY_View::Relay_OFF(int whichBIT)
{
	if(whichBIT>=MAX_DO_NUM){AfxMessageBox("whichBIT>=MAX_DO_NUM"); return;}
	M_DO_Array[whichBIT]=RELAY_OFF; //1 means off because NI acts as a voltage sink for the relay (70-ODC5 Grayhill)
	if(!SetEvent(m_M_DO_Active[whichBIT])){
		AfxMessageBox("SetEvent() failed");
	}
}

// This is in a Thread form because:
// it needs to turn on and off with a delay in between
LRESULT CMOXY_View::DIO_AutoOnOff_CallBack(WPARAM duration, LPARAM DIO_ID) 
{
	Relay_ON_n_DisplayLED(DIO_ID);
	SleepAccurate(duration,-1); 
	Relay_OFF_n_DisplayLED(DIO_ID);
	return NULL;
}

void CMOXY_View::InitDIO_FIg()
{	
	VirMemDC();
	CClientDC objDC(this);
	CDC *pDC=&m_DC[0];

	CFigure *pFig=&m_Fig[DIO_FIG];
	SetFontSize(pFig);
	//pFig->SetFontSize(&m_DC[0]);
	pFig->m_normTx=0;  pFig->m_normTy=0;   
	pFig->m_normBx=m_Separator_inScreenCoord;  pFig->m_normBy=0.2*(m_savedClientRect.bottom-m_savedClientRect.top); //XY_Up is the upper part of the XY_displayer
	pFig->Set_MinMax_n_Labels_XY("","",0,MaxDIO,0,1	,xFRAME);		
}

void CMOXY_View::Redraw_DIO_Fig()
{
	CFigure *pFig=&m_Fig[DIO_FIG];
	CDC *pDC=&m_DC[0];
	CBrush *pOldBrush=pDC->SelectObject(&m_DIO_backgroundBrush);
	pFig->Prepare4FigFrame(pDC);
	pFig->Frame_Graph2D(pDC);
	Label_DIO(pFig,pDC);
	pDC->SelectObject(pOldBrush);
}

//#define DIO_DISP_Y_POSITION(y) (0.95-0.5*((2*y)/MaxDIO)) 
//#define DIO_DISP_X_POSITION(x) (x*2-MaxDIO*((2*x)/MaxDIO)+0.5) 

void CMOXY_View::Label_DIO(CFigure *pFig, CDC *pDC)
{
	CClientDC objDC(this);
	for(int i=0;i<MaxDIO;i++){
		pFig->PutIntVal(DIO_DISP_X_POSITION(i),DIO_DISP_Y_POSITION(i)-0.2,i,pDC);
	}
	pDC->SelectObject(&m_BLACK); 
	pFig->Graph2D(pFig->m_dMinX,pFig->m_dSpanY*0.5+pFig->m_dMinY,9,pDC);pFig->Graph2D(pFig->m_dMaxX,pFig->m_dSpanY*0.5+pFig->m_dMinY,2,pDC);
	pFig->RefreshFrame(&objDC,pDC);
}

