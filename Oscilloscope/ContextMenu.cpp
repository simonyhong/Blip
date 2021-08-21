
#include "stdafx.h"
#include "Oscilloscope.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "OscilloscopeDoc.h"
#include "FiguresOsc.h"
#include "OscilloscopeView.h"
#include "SinTable.h"


void COscilloscopeView::OnDeleteFigs()
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	int clm,row;
	CFigure *pFig;
	CClientDC objDC(this);

	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Captured==1){
				pFig->m_Deleted=1;
				pFig->m_Captured=0;
			}
		}
	}
	EliminateExcluded();
}

void COscilloscopeView::OnDelete_the_Rest_Figs()
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	int clm,row;
	CFigure *pFig;
	CClientDC objDC(this);

	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Captured==0){
				pFig->m_Deleted=1;
			}
		}
	}
	EliminateExcluded();
}

void COscilloscopeView::EliminateExcluded()
{
	int clm,row;
	int validColummTill=0, ValidRowTill=0;
	CFigure *pFig;
	CClientDC objDC(this);

	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Deleted==0){
				if(validColummTill<clm+1)validColummTill=clm+1;
				if(ValidRowTill<row+1)ValidRowTill=row+1;
			}
		}
	}
	if(m_dHowManyRows!=ValidRowTill||m_dHowManyClmns!=validColummTill||m_NewlyLoaded==1){
		if(ValidRowTill==0&&validColummTill==0){//Everything has been deleted==> restart!
			ValidRowTill=MaxFigRow; validColummTill=MaxFigCln;
			m_dHowManyRows=ValidRowTill;
			m_dHowManyClmns=validColummTill;
			Start_a_NEW_set_of_figures();		
		}
		m_dHowManyRows=ValidRowTill;
		m_dHowManyClmns=validColummTill;
		if(m_bFistTime==1){//The program is starting for the first time using a "defaultRoot.txt" file. Since it needs to measure and store the dimension of the ClientView area...
			Re_measure_ClientView();
			m_bFistTime=0;
		}
		m_dSizeOfOneRow=(m_savedClientRect.bottom-m_savedClientRect.top-ViewMargin4FigsAlongEdges)/m_dHowManyRows;  if(m_dSizeOfOneRow<=0)m_dSizeOfOneRow=0;
		m_dSizeOfOneClmn=(m_savedClientRect.right-m_savedClientRect.left-ViewMargin4FigsAlongEdges)/m_dHowManyClmns;if(m_dSizeOfOneClmn<=0)m_dSizeOfOneClmn=0;
		Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
		m_NewlyLoaded=0;
		Batch_Prepare4FigFrame();
		Batch_DrawFigureFrames();
		Hilight_AllCaptured_Figs();
		DisplayGlobalTitle();
	}
	else{
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				if(pFig->m_Deleted==1){
					pFig->ComputeMapping_XY(); 
					pFig->Frame_Graph2D(&m_DC[0],0); 
					pFig->RefreshFrame(&objDC,&m_DC[0]); 
					UnHilight_OneCaptured(pFig);
				}
			}
		}
	}
}
