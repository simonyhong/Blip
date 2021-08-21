
#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "Figure_DAS.h"
#include "Channel.h"
#include "DAS_View.h"
#include "SinTable.h"


void CDasView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if(m_pSpotlightFig==NULL)return;//Hit outside the pane
	CAiChannel *pChan=m_pSpotlightFig->pChan;
	if(pChan==NULL)return;//If the Figure is not attached to any channel, don't do anything.
	CMenu menu;
	CClientDC objDC(this);

	//if(m_SelectedItemID==SCALE_AREA){
	if(pChan->m_RtClickSelectedBoxID>=MAX_VT_RECTs_PER_NEURON)AfxMessageBox("pChan->m_RtClickSelectedBoxID>=MAX_VT_RECTs_PER_NEURON");
	if(pChan->m_RtClickSelectedBoxID>=0){//The user clicked an area that does not belong to a VT box area.
		OnVtDelete();
		//The following code sometimes crashes the computer.
		//menu.LoadMenu(IDR_MENU1);
		//CMenu* pPopup=menu.GetSubMenu(1);
		//VERIFY(pPopup); //The same as ASSERT but works both in Debug and Release modes.
		//ASSERT(pPopup!=NULL);
		//if(pPopup==NULL){
		//	AfxMessageBox("?");
		//}
		////Display the popup menu
		//pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x, point.y,this);
	}
	else{
		menu.LoadMenu(IDR_MENU1);
		CMenu* pPopup=menu.GetSubMenu(0);
		ASSERT(pPopup!=NULL);
		//Display the popup menu
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x, point.y,this);
	}
}
void CDasView::OnVtDelete()
{
	CAiChannel *pChan=m_pSpotlightFig->pChan;
	//Delete the box, Rearrange the list of boxes
	pChan->DeleteOneVTBox(pChan->m_RtClickSelectedBoxType,pChan->m_RtClickSelectedBoxIsToAccept,pChan->m_RtClickSelectedBoxID);
	pChan->m_RtClickSelectedBoxID=-1;//Turn off the flag

	//Display
	CClientDC objDC(this);
	pChan->pFig->ErasePaneBlack(&objDC);
	pChan->ShowAll_VTBoxes();
}


void CDasView::OnDeleteFigs()
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
				if((pFig->m_ChanID>=0 && pFig->m_ChanID<NUM_NN_AI_CHAN) && pFig->pChan!=NULL){
					DisconnectFigure_and_StopClientChan(pFig->m_ChanID);
				}
			}
		}
	}
	EliminateExcluded();
	char MsgBuff[1024]; sprintf(MsgBuff,"Currently, %d neural channles are available.\n If you would like to change this setting, change the number:NUM_NN_AI_CHAN in the program and recompile.",NUM_NN_AI_CHAN);
	AfxMessageBox(MsgBuff);
}

void CDasView::OnDelete_the_Rest_Figs()
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
				pFig->m_Captured=0;
				if((pFig->m_ChanID>=0 && pFig->m_ChanID<NUM_NN_AI_CHAN) && pFig->pChan!=NULL ){
					DisconnectFigure_and_StopClientChan(pFig->m_ChanID);
				}
			}
		}
	}
	EliminateExcluded();
}

void CDasView::EliminateExcluded()
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
	if(m_dHowManyRows!=ValidRowTill||m_dHowManyClmns!=validColummTill){
		if(ValidRowTill==0&&validColummTill==0){//Everything has been deleted==> restart!
			ValidRowTill=NUM_NN_AI_CHAN/(MaxFigCln+1)+1; //Depending on the number of Neural channels specified, the number of rows will vary. 
			if(ValidRowTill>MaxFigRow){char Msg[128]; sprintf(Msg,"Currently, DAS supports up to %dx%d fugures.",MaxFigRow,MaxFigRow); AfxMessageBox(Msg); exit(1);}
			if(ValidRowTill>1)validColummTill=MaxFigCln; else validColummTill=NUM_NN_AI_CHAN; 
			//ValidRowTill=MaxFigRow; validColummTill=MaxFigCln;
			m_dHowManyRows=ValidRowTill;
			m_dHowManyClmns=validColummTill;
			Start_a_NEW_set_of_figures();		
		}
		m_dHowManyRows=ValidRowTill;
		m_dHowManyClmns=validColummTill;
		m_dSizeOfOneRow=(m_savedClientRect.bottom-m_savedClientRect.top-ViewMargin4FigsAlongEdges)/m_dHowManyRows;  if(m_dSizeOfOneRow<=0)m_dSizeOfOneRow=0;
		m_dSizeOfOneClmn=(m_savedClientRect.right-m_savedClientRect.left-ViewMargin4FigsAlongEdges)/m_dHowManyClmns;if(m_dSizeOfOneClmn<=0)m_dSizeOfOneClmn=0;
		Batch_FigPosition_Settings(m_dSizeOfOneRow, m_dSizeOfOneClmn, m_dScaleUpDn);
		Batch_Prepare4FigFrame();
		Batch_DrawFigureFrames();
		Hilight_AllCaptured_Figs();
		DisplayTitle();
		RecalibratePanes();
		for(int chanIdx=0; chanIdx<m_NumAiChannels; chanIdx++){	
			m_AiChan[chanIdx].Remeasure_Box_Positions_DataAnchored();	
		}
	}
	else{//Gray out
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				if(pFig->m_Deleted==1){
					pFig->ComputeMapping_X_Y_Y2(); 
					pFig->formal_Frame2D(&m_DC[0],0); 
					pFig->RefreshFrame(&objDC,&m_DC[0]); 
					//UnHilight_OneCaptured(pFig);
				}
			}
		}
	}
}
