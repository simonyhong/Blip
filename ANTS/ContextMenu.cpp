
#include "stdafx.h"
#include "ANTS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ANTS_Doc.h"
#include "SharedConstants.h"
#include "Channel.h"
#include "Figure.h"
#include "ANTS_View.h"
#include "SinTable.h"


void CAntsView::OnDeleteFigs()
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	int clm,row,lyr;
	CFigure *pFig;
	CClientDC objDC(this);
	char strBuf[256], Name[256];

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(pFig->m_Captured==1){
					pFig->m_Deleted=1;
					pFig->m_Captured=0;
					//pFig->m_StimGatingRelay.m_ChanID=-1;
					//sprintf(strBuf,"StmRly:%d",pFig->m_StimGatingRelay.m_ChanID); pFig->m_StimGatingRelay.NameString=strBuf;
					//sprintf(Name,"RecCh:%d",pFig->m_RecordingChan.m_ChanID); m_pSpotlightFig->m_RecordingChan.NameString=Name;
					//m_pSpotlightFig->m_RecordingChan.NameString+=" "+ pFig->m_StimGatingRelay.NameString;
				}
			}
		}
	}
	Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
	EliminateExcluded();
}

void CAntsView::OnDelete_the_Rest_Figs()
{
	if(m_numCaptured==0){AfxMessageBox("Nothing has been selected ==> Try again", MB_OK); return;}
	int clm,row,lyr;
	CFigure *pFig;
	CClientDC objDC(this);
	char strBuf[256], Name[256];

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(pFig->m_Captured==0){
					pFig->m_Deleted=1;
					//pFig->m_StimGatingRelay.m_ChanID=-1;
					//sprintf(strBuf,"StmRly:%d",pFig->m_StimGatingRelay.m_ChanID); pFig->m_StimGatingRelay.NameString=strBuf;
					//sprintf(Name,"RecCh:%d",pFig->m_RecordingChan.m_ChanID); m_pSpotlightFig->m_RecordingChan.NameString=Name;
					//m_pSpotlightFig->m_RecordingChan.NameString+=" "+ pFig->m_StimGatingRelay.NameString;				
				}
			}
		}
	}
	Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
	EliminateExcluded();
	m_bIsScrXYforIJ=0;
}


void CAntsView::EliminateExcluded()
{
	int clm,row,lyr, numFig;
	int validColummTill[MaxFigLyr], ValidRowTill[MaxFigLyr];
	CFigure *pFig;
	CClientDC objDC(this);

	numFig=0;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		validColummTill[lyr]=0;  ValidRowTill[lyr]=0;
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(pFig->m_Deleted==0){
					if(validColummTill[lyr]<clm+1)validColummTill[lyr]=clm+1;
					if(ValidRowTill[lyr]<row+1)ValidRowTill[lyr]=row+1;
					numFig++;
				}
			}
		}
	}
	int changed=0;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		if(m_dHowManyRows[lyr]!=ValidRowTill[lyr]||m_dHowManyClmns[lyr]!=validColummTill[lyr]||m_NewlyLoaded==1){
			changed=1; break;
		}
	}
	if(changed==1){
		if(numFig==0){//Everything has been deleted==> restart!
			m_dHowManyLayers=MaxFigLyr;
			for(lyr=0;lyr<m_dHowManyLayers;lyr++){
				m_dHowManyRows[lyr]=MaxFigRow; m_dHowManyClmns[lyr]=MaxFigCln; 
			}
			Start_a_NEW_set_of_figures();		
		}
		else{
			if(m_bFistTime==1){//The program is starting for the first time using a "defaultRoot.txt" file. Since it needs to measure and store the dimension of the ClientView area...
				Re_measure_ClientView();
			}
			for(lyr=0;lyr<m_dHowManyLayers;lyr++){
				m_dHowManyRows[lyr]=ValidRowTill[lyr]; m_dHowManyClmns[lyr]=validColummTill[lyr]; 
			}
			for(int Lyr=0;Lyr<m_dHowManyLayers;Lyr++){
				m_dSizeOfOneRow[Lyr] =Divide((m_savedClientRect.bottom-m_savedClientRect.top-ViewMargin4FigsAlongEdges),m_dHowManyRows[Lyr]);  if(m_dSizeOfOneRow[Lyr]<=0)m_dSizeOfOneRow[Lyr]=0;
				m_dSizeOfOneClmn[Lyr]=Divide((m_savedClientRect.right-m_savedClientRect.left-ViewMargin4FigsAlongEdges),m_dHowManyClmns[Lyr]); if(m_dSizeOfOneClmn[Lyr]<=0)m_dSizeOfOneClmn[Lyr]=0;
			}
			Batch_FigPosition_Settings(m_dScaleUpDn);
		}

		m_NewlyLoaded=0;		
		RequestDisplayEvent(REDRAW_ALL_LAYERS);
	}
	else{
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					if(pFig->m_Deleted==1){
						pFig->Compute_Mapping_XY(); 
						pFig->Frame_Graph2D(&m_DC[lyr],0); 
						pFig->RefreshFrame(&objDC,&m_DC[lyr]); 
						UnHilight_OneCaptured(pFig);
					}
				}
			}
		}
	}

	//Inspect the figures and register proper Stimulation Channels to M_StimDigitalOutChannels[].
	//For the first loading the program, since the Shared Memory is not initiated, this one will be donw sometime after the Shared Memory initialization.
	if(m_bFistTime!=1)Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
}
