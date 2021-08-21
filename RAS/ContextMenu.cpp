
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

void CRASView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	CMenu menu;
	//if(m_SelectedItemID==SCALE_AREA){
		menu.LoadMenu(IDR_MENU1);
		CMenu* pPopup=menu.GetSubMenu(0);
		ASSERT(pPopup!=NULL);
	//}

	//Display the popup menu
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x, point.y,this);
}

void CRASView::OnDeleteFigs()
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	int clm,row;
	CFigure *pFig;
	CClientDC objDC(this);

	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Captured==1){
				pFig->m_Deleted=1;
				pFig->m_Captured=0;
			}
		}
	}
	EliminateExcluded();
	m_bIsScrXYforIJ=0;
}

void CRASView::OnDelete_the_Rest_Figs()
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	int clm,row;
	CFigure *pFig;
	CClientDC objDC(this);

	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Captured==0){
				pFig->m_Deleted=1;
			}
		}
	}
	EliminateExcluded();
	m_bIsScrXYforIJ=0;
}

void CRASView::EliminateExcluded()
{
	int clm,row;
	int validColummTill=0, ValidRowTill=0;
	CFigure *pFig;
	CClientDC objDC(this);

	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
			if(pFig->m_Deleted==0){
				if(validColummTill<clm+1)validColummTill=clm+1;
				if(ValidRowTill<row+1)ValidRowTill=row+1;
			}
		}
	}
	if(m_dHowManyRows!=ValidRowTill||m_dHowManyClmns!=validColummTill||m_NewlyLoaded==1){
		if(ValidRowTill==0&&validColummTill==0){//Everything has been deleted==> restart!
			if(m_bIsON==1){m_bIsON=0; while(m_isDisplayerRunning)Sleep(10);}//Signal to quit whatever currently going on, and restart.
			ValidRowTill=MaxFigRow; validColummTill=MaxFigCln;
			m_dHowManyRows=ValidRowTill;
			m_dHowManyClmns=validColummTill;
			Start_a_NEW_set_of_figures();		
		}
		m_dHowManyRows=ValidRowTill;
		m_dHowManyClmns=validColummTill;
		if(m_bFistTime==1){//The program is starting for the first time using a "defaultRoot.txt" file. Since it needs to measure and stor the dimention of the ClientView area...
			Re_measure_ClientView();
		}
		m_dSizeOfOneRow=(m_savedClientRect.bottom-m_savedClientRect.top-ViewMargin4FigsAlongEdges)/m_dHowManyRows;  if(m_dSizeOfOneRow<=0)m_dSizeOfOneRow=0;
		m_dSizeOfOneClmn=(m_savedClientRect.right-m_savedClientRect.left-ViewMargin4FigsAlongEdges)/m_dHowManyClmns;if(m_dSizeOfOneClmn<=0)m_dSizeOfOneClmn=0;
		m_NewlyLoaded=0;
		Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
		Batch_Prepare4FigFrame();
		Batch_DrawFigureFrames(1);
		Hilight_AllCaptured_Figs();
		DisplayTitle();
	}
	else{
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				if(pFig->m_Deleted==1){
					pFig->Prepare4FigFrame(); 
					pFig->Frame_Graph2D(&m_DC[0],0, !m_Stop_Or_Pause); 
					pFig->RefreshFrame(&objDC,&m_DC[0]); 
					UnHilight_OneCapturedFig(pFig);
				}
			}
		}
	}
}
