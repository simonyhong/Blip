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
#include "DialogueBox.h"


void CAntsView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//WhichFigure(point);

	CView::OnRButtonDown(nFlags, point);
}

void CAntsView::OnLButtonUp(UINT nFlags, CPoint point)
{

	m_HeldLineIdx=-1;

	CView::OnLButtonUp(nFlags, point);
}

void CAntsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	WhichFigure(point);
	if((nFlags & MK_CONTROL)!=MK_CONTROL){
		if(m_pSpotlightFig==NULL)return;
		for(int i=0;i<NUM_MEASURING_LINES;i++){
			if(m_HeldLineIdx!=-1)break;
			if(abs(m_pSpotlightFig->m_MeasureingLine_ScrnCoord[i]-point.x)<m_pSpotlightFig->m_FontWidth && (point.y>m_pSpotlightFig->m_nPaneTy && point.y<m_pSpotlightFig->m_nPaneBy)){m_HeldLineIdx=i;break;}
		}
		if(m_HeldLineIdx!=3 && m_HeldLineIdx!=-1)return;
	}
	else{
		CRectTracker tracker; //The "rubberband" instance!!
		if(tracker.TrackRubberBand(this, point, TRUE)) {//This function will capture the mouse until the user releases the left mouse button, presses the ESC key, or presses the right mouse button. As the user moves the mouse cursor, the feedback is updated by calling DrawTrackerRect and OnChangedRect.
			// The follwoing happens AFTER the rubber band is released.
			tracker.m_rect.NormalizeRect();
			m_CapturRec.top=tracker.m_rect.top;
			m_CapturRec.left=tracker.m_rect.left;
			m_CapturRec.right=tracker.m_rect.right;
			m_CapturRec.bottom=tracker.m_rect.bottom;

			Figs_Captured();
			//RequestDisplayEvent(REDRAW_SAVE_TAG);
		}
	}

	CView::OnLButtonDown(nFlags, point);
}
    
void CAntsView::Recalibrate_Panes_and_MeasuringLineRectPositions() 
{
	int row,clm,lyr, i;
	CFigure *pFig;

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative data to a point on the screen.	
				pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative data to a point on the screen.
			
				for(i=0;i<NUM_MEASURING_LINES;i++){
					pFig->m_MeasuringTimeRect[i].left=pFig->m_MeasureingLine_ScrnCoord[i]; 
					pFig->m_MeasuringTimeRect[i].right=pFig->m_MeasuringTimeRect[i].left+pFig->m_FontWidth*5; 
					pFig->m_MeasuringTimeRect[i].top=pFig->m_nPaneTy+pFig->m_FontHeight*1.2; 
					pFig->m_MeasuringTimeRect[i].bottom=pFig->m_MeasuringTimeRect[i].top+pFig->m_FontHeight*1.2; 
				}
			}
	}	}
}

void CAntsView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON){
		//WhichFigure(point);//This one is not needed because OnLButtonDown() does this
		if(m_pSpotlightFig==NULL)return;

		//If the curser hits the "separator" line,
		for(int i=0;i<NUM_MEASURING_LINES;i++){
			if(m_HeldLineIdx!=-1)break;
			if(abs(m_pSpotlightFig->m_MeasureingLine_ScrnCoord[i]-point.x)<m_pSpotlightFig->m_FontWidth && (point.y>m_pSpotlightFig->m_nPaneTy && point.y<m_pSpotlightFig->m_nPaneBy)){m_HeldLineIdx=i;break;}
		}
		if(m_HeldLineIdx!=3&& m_HeldLineIdx!=-1){
			RequestDisplayEvent(REDRAW_MEASURING_LINE,3, point.x, point.y);
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CAntsView::Figs_Captured() 
{
	int clm, row;
	CFigure *pFig;
	for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
		for(clm=0;clm<m_dHowManyClmns[ m_DisplayedStimLayer];clm++){
			pFig=&m_Fig[row][clm][ m_DisplayedStimLayer];
			if((m_CapturRec.top<=pFig->m_nTy && m_CapturRec.bottom>=pFig->m_nBy && m_CapturRec.left<=pFig->m_nTx && m_CapturRec.right>=pFig->m_nBx)||
				RectCrossingRect(m_CapturRec.left,m_CapturRec.top,m_CapturRec.right,m_CapturRec.bottom,pFig->m_nTx,pFig->m_nTy,pFig->m_nBx,pFig->m_nBy)||
				(m_CapturRec.top>=pFig->m_nTy && m_CapturRec.bottom<=pFig->m_nBy && m_CapturRec.left>=pFig->m_nTx && m_CapturRec.right<=pFig->m_nBx)){
				if(pFig->m_Captured==1){
				}
				else{
					pFig->m_Captured=1; 
					m_numCaptured++;
					Hilight_OneCapturedFig(pFig);
				}
			}
			else{ 		
				if(pFig->m_Captured==1&&(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL)){
					pFig->m_Captured=0;//Clerar previous selection unless Control key is pressed
					m_numCaptured--;
					UnHilight_OneCaptured(pFig);
				}
			}
		}
	}
}

//Using 4 LineCrossingRect(), see if the two rectangles cross each other.
BOOL CAntsView::RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom)
{
	if(LineCrossingRect(recLeft1,recTop1,recRight1,recTop1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recTop1,recRight1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recBottom1,recLeft1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recLeft1,recBottom1,recLeft1,recTop1, recLeft, recTop, recRight, recBottom))return 1;
	else return 0;
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CAntsView::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
{
#define CCW(ax, ay, bx, by, cx, cy ) ((cy-ay)*(bx-ax)>(by-ay)*(cx-ax))
	if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recLeft,recTop,recRight,recTop,x1,y1)!=CCW(recLeft,recTop,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recRight,recBottom)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recRight,recBottom,recRight,recTop,x1,y1)!=CCW(recRight,recBottom,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recBottom)!=CCW(x1,y1,x2,y2,recRight,recBottom))&&(CCW(recLeft,recBottom,recRight,recBottom,x1,y1)!=CCW(recLeft,recBottom,recRight,recBottom,x2,y2))
		)return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recLeft,recBottom))&&(CCW(recLeft,recTop,recLeft,recBottom,x1,y1)!=CCW(recLeft,recTop,recLeft,recBottom,x2,y2))
		)
		return 1;
	else return 0;
}
    
void CAntsView::Hilight_AllCaptured_Figs() 
{
	if(m_numCaptured>0){
		int clm, row;
		CFigure *pFig;
		m_DC[m_DisplayedStimLayer].SelectObject(m_ORANGE2);
		for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
			for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
				pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
				if(pFig->m_Captured==1){ 
					m_DC[m_DisplayedStimLayer].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nTx,pFig->m_nTy); 
				}
			}
		}
		CClientDC objDC(this);
		objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
	}
	m_DC[m_DisplayedStimLayer].SelectObject(&m_bkPen);
}

void CAntsView::Hilight_OneCapturedFig(CFigure *pFig) 
{
	CDC *pDC=pFig->m_pDC;
	pDC->SelectObject(m_ORANGE2);
	pDC->MoveTo(pFig->m_nTx,pFig->m_nTy); 
	pDC->LineTo(pFig->m_nBx,pFig->m_nTy); 	pDC->LineTo(pFig->m_nBx,pFig->m_nBy); 	pDC->LineTo(pFig->m_nTx,pFig->m_nBy); 	pDC->LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_ORANGE2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	
	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
	pDC->SelectObject(&m_bkPen);
}
void CAntsView::UnHilight_OneCaptured(CFigure *pFig) 
{
	CDC *pDC=pFig->m_pDC;
	pDC->SelectObject(m_BLACK2);
	pDC->MoveTo(pFig->m_nTx,pFig->m_nTy); 	pDC->LineTo(pFig->m_nBx,pFig->m_nTy); 	pDC->LineTo(pFig->m_nBx,pFig->m_nBy); 	pDC->LineTo(pFig->m_nTx,pFig->m_nBy); 	pDC->LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_BLACK2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
}


void CAntsView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	//if(m_SelectedItemID==SCALE_AREA){
		menu.LoadMenu(IDR_MENU1);
		CMenu* pPopup=menu.GetSubMenu(0);
		ASSERT(pPopup!=NULL);
	//}

	//Display the popup menu
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x, point.y,this);
}

//This "WhichPane()" function changes the size of the pane and its position, when the "blow-up button" is pressed. 
void CAntsView::WhichFigure(CPoint point) 
{
#define BNDRY_MRGN 20
	int clm,row,lyr;
	CFigure *pFig;
	m_pSpotlightFig=NULL;

	//If the rectangle on screen (m_paneTopX, m_paneTopY, m_paneBottomX, m_paneBottomY) is not defined for the graph pane, define it!
	if(!m_bIsScrXYforIJ){ 
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
					pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
				}
			}
		}
		m_bIsScrXYforIJ=1;
	}

	//Clicked the title area
	if(point.y<=m_TitleArea.bottom && point.y>=m_TitleArea.top && point.x<=m_TitleArea.right && point.x>=m_TitleArea.left){ 
		if(point.y<=m_dTitleRect.bottom && point.y>=m_dTitleRect.top && point.x<=m_dTitleRect.right && point.x>=m_dTitleRect.left){ 
			m_CallingFrom=1;
			OnEnChangeEditString1();
			if(m_GlobalTitle.GetLength()!=0){
				RequestDisplayEvent(RENAMING_TITLE);
			}
			return;
		}
		else if((point.x>=m_FileName.StringRect.left && point.x<=m_FileName.StringRect.right)&&(point.y>=m_FileName.StringRect.top && point.y<=m_FileName.StringRect.bottom)){
			EditPrefixSuffix();
			RequestDisplayEvent(REDRAW_FILE_NAME);
			return;
		}
		else if((point.x>=m_Lable_Of_SuperimposeNumber.StringRect.left && point.x<=m_Lable_Of_SuperimposeNumber.StringRect.right)&&(point.y>=m_Lable_Of_SuperimposeNumber.StringRect.top && point.y<=m_Lable_Of_SuperimposeNumber.StringRect.bottom)){
			m_CallingFrom=3;
			OnEnChangeEditString1();
		}
		else if((point.x>=m_Lable_Of_TriggerChanNumber.StringRect.left && point.x<=m_Lable_Of_TriggerChanNumber.StringRect.right)&&(point.y>=m_Lable_Of_TriggerChanNumber.StringRect.top && point.y<=m_Lable_Of_TriggerChanNumber.StringRect.bottom)){
			m_CallingFrom=4;
			OnEnChangeEditString1();
		}
		#ifdef Zapper
		else if((point.x>=m_ZapRect.left && point.x<=m_ZapRect.right)&&(point.y>=m_ZapRect.top && point.y<=m_ZapRect.bottom)){
			*M_isZapperLearning=!(*M_isZapperLearning);
			RequestDisplayEvent(DISP_ZAP_STATUS);
			return;
		}
		#endif
	}

	//Page (Layer) tabs clicked?
	for(int i=0;i<MAX_NUM_STIM_CHANNELS;i++){
		if((point.x>=m_LayerTab[i].StringRect.left && point.x<=m_LayerTab[i].StringRect.right)&&(point.y>=m_LayerTab[i].StringRect.top && point.y<=m_LayerTab[i].StringRect.bottom)){
			if(m_DisplayedStimLayer!=i){ //If the newly selected page is not the previously (==m_DisplayedStimLayer) displayed layer...
				//Uncapture all the captured figures in the currently displayed page before displaying a new page.
				for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
					for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
						pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
						if(pFig->m_Captured==1){
							pFig->m_Captured=0;
							UnHilight_OneCapturedFig(pFig);
						}
					}
				}
				m_DisplayedStimLayer=i; //Change the page
				m_numCaptured=0;
				RequestDisplayEvent(UNVEIL_CURRENT_PAGE);
			}
			return;
		}
	}
	
#define CLICK_ON_BLANK			0
#define AXIS_SCALE_CLICKED		1
#define RECORDING_AND_STIMULATION_CHANS_CLICKED	3
//#define STIM_CHAN_CLICKED		4
#define FOCUS_CLICKED			5
#define SAVE_LABEL_CLICKED		6
#define ZAP_LEVEL_CLICKED		7
	int	clickType=-1, found=-1;
	int NumOfFocusChannels=0,i,j;
	//Here it PRE-examines if the user has already chosen some Figures before clicking an icon to "Batch" manipulate them.
	lyr=m_DisplayedStimLayer;
	for(row=0;row<m_dHowManyRows[lyr];row++){
		for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
			pFig=&m_Fig[row][clm][lyr];
			if((point.x>=pFig->m_nTx && point.x<=pFig->m_nBx)&&(point.y<=pFig->m_nBy&&point.y>=pFig->m_nTy)){ 
				m_pSpotlightFig=pFig;
				// Edit: X, Y axes
				//Axis-scale manipulators has been clicked?
				if(  ((point.x>=pFig->m_FirstYlabel_left && point.x<=pFig->m_FirstYlabel_right)&&(point.y>=pFig->m_FirstYlabel_top && point.y<=pFig->m_FirstYlabel_bottom))
						||((point.x>=pFig->m_FirstXlabel_left && point.x<=pFig->m_FirstXlabel_right)&&(point.y>=pFig->m_FirstXlabel_top && point.y<=pFig->m_FirstXlabel_bottom))
						||((point.x>=pFig->m_LastYlabel_left && point.x<=pFig->m_LastYlabel_right)&&(point.y>=pFig->m_LastYlabel_top && point.y<=pFig->m_LastYlabel_bottom))
						||((point.x>=pFig->m_LastXlabel_left && point.x<=pFig->m_LastXlabel_right)&&(point.y>=pFig->m_LastXlabel_top && point.y<=pFig->m_LastXlabel_bottom))){
					m_dMinimumX=m_pSpotlightFig->m_dMinX; m_dMaximumX=m_pSpotlightFig->m_dMaxX; m_dMinimumY=m_pSpotlightFig->m_dMinY; m_dMaximumY=m_pSpotlightFig->m_dMaxY;
					clickType=AXIS_SCALE_CLICKED;
					goto END_OF_PREMEASURE;
				}
				//Recording / Stimulation channel clicked?
				else if((point.x>=pFig->m_RecordingChan.StringRect.left && point.x<=pFig->m_RecordingChan.StringRect.right)&&(point.y>=pFig->m_RecordingChan.StringRect.top && point.y<=pFig->m_RecordingChan.StringRect.bottom)){
					clickType=RECORDING_AND_STIMULATION_CHANS_CLICKED;
					goto END_OF_PREMEASURE;
				}
				//"Focus" clicked?
				else if((point.x>=pFig->m_Focus.StringRect.left && point.x<=pFig->m_Focus.StringRect.right)&&(point.y>=pFig->m_Focus.StringRect.top && point.y<=pFig->m_Focus.StringRect.bottom)){
					clickType=FOCUS_CLICKED;
					if(*M_is_LFP_saving==1){AfxMessageBox("You cannot change the configuration during Saving."); return;}
					goto END_OF_PREMEASURE;
				}
				//"To save" has been clicked?
				else if((point.x>=pFig->m_ClickToSave.StringRect.left && point.x<=pFig->m_ClickToSave.StringRect.right)&&(point.y>=pFig->m_ClickToSave.StringRect.top && point.y<=pFig->m_ClickToSave.StringRect.bottom)){
					clickType=SAVE_LABEL_CLICKED;
					goto END_OF_PREMEASURE;
				}
				else{
					double dataX, dataY;
					FromScrnToDataCoordinate(point.x, point.y, dataX, dataY, pFig);
					//Color choice
					if(dataX>pFig->m_dMaxX){
						if(dataY>=pFig->m_dMinY && dataY<=pFig->m_dMaxY){
							clickType=ZAP_LEVEL_CLICKED;
							goto END_OF_PREMEASURE;
						}
					}
					else{//Clicked on a blank area
						clickType=CLICK_ON_BLANK;
						goto END_OF_PREMEASURE;
					}
				}
			}
		}
	}
	END_OF_PREMEASURE:

	//#################   (1) Fig is NOT clicked
	if(m_pSpotlightFig==NULL){
		lyr=m_DisplayedStimLayer;
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL){//Clear previous selection unless Control key is pressed
					if(pFig->m_Captured==1){
						pFig->m_Captured=0;
						UnHilight_OneCapturedFig(pFig); 
						m_numCaptured--;
					}
				}
			}
		}
	}
	//#################    Clicked a Figure!!
	//There are three pices of information: (2) if a figure is clicked, is it already selected? (3) what kind of clickType is this?
	else{
		if(m_pSpotlightFig->m_Captured==0){//If the clicked figure has not been selected already: First clear all the perior-selected ones and highlight only the newly clicked one, unless the Control key is pressed
			if(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL){
				lyr=m_DisplayedStimLayer;
				for(row=0;row<m_dHowManyRows[lyr];row++){
					for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
						pFig=&m_Fig[row][clm][lyr];
						if(pFig->m_Captured==1){
							pFig->m_Captured=0;
							UnHilight_OneCapturedFig(pFig); 
							m_numCaptured--;
						}
					}
				}
			}
			m_pSpotlightFig->m_Captured=1;
			m_numCaptured++;
			Hilight_OneCapturedFig(m_pSpotlightFig);
		}
		switch(clickType){
			case AXIS_SCALE_CLICKED:
				OnChangeParameters();
				lyr=m_DisplayedStimLayer;
				for(row=0;row<m_dHowManyRows[lyr];row++){
					for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
						pFig=&m_Fig[row][clm][lyr];
						if(pFig->m_Captured!=1||pFig->m_Deleted==1)continue;
						if(m_dMinimumX!=pFig->m_dMinX || m_dMaximumX!=pFig->m_dMaxX || m_dMinimumY!=pFig->m_dMinY || m_dMaximumY!=pFig->m_dMaxY){
							pFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
				}	}	}
				RequestDisplayEvent(RESCALING_AXES);
				break;
			case RECORDING_AND_STIMULATION_CHANS_CLICKED:
				if(Dialog_Record_Stimulation_Channels()!=-1){
					int row,clm;
					CFigure *pFig;
					int StimChanDOIdx;
					for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
						for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
							pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
							//Recording Channel
							if(pFig->m_Captured!=1 || pFig->m_RecordingChan.m_ChanID==m_pSpotlightFig->m_RecordingChan.m_ChanID)continue;
							pFig->m_RecordingChan.m_ChanID=m_pSpotlightFig->m_RecordingChan.m_ChanID; 
							pFig->m_RecordingChan.NameString=m_pSpotlightFig->m_RecordingChan.NameString; //m_pSpotlightFig->m_RecordingChan.NameString contains BOTH the Stimulation & Recording channel names
							//Stimulation Channel
							if(m_pSpotlightFig->m_StimGatingRelay.m_ChanID==pFig->m_StimGatingRelay.m_ChanID || pFig->m_Captured!=1||pFig->m_Deleted==1)continue;
							pFig->m_StimGatingRelay.m_ChanID=m_pSpotlightFig->m_StimGatingRelay.m_ChanID;
							pFig->m_StimGatingRelay.NameString=m_pSpotlightFig->m_StimGatingRelay.NameString;
							StimChanDOIdx=GetIndexOfSimChannel(pFig->m_StimGatingRelay.m_ChanID);
							if(StimChanDOIdx!=-1){
								M_TriggerToStimulationLatency[StimChanDOIdx]=(int)(1000*fabs(pFig->m_MeasuringLineTime[0]));
								AfterSetting_PreStim_PostTrigger_Periods_Calculate_NumOfSamplingsAfterStim(StimChanDOIdx);//How frequently do you want the antidromics stimulation to be triggered? For example, 1000ms? or 500ms?
							}				
						}
					}
					Find_RecordingChannels_ToSave_And_Display_BasedOn_ContentsInFigures();
					Set_StimLatency_and_PostStimPeriod();
					RequestDisplayEvent(MINIMUM_REQ_TIME_TO_TRIGGER);
					RequestDisplayEvent(REDRAW_RECORDING_STIMULATION_CHAN_IDS);
				}
				break;
			case FOCUS_CLICKED:
				NumOfFocusChannels=*M_NumOfFocusChannels;
				for(row=0;row<m_dHowManyRows[ m_DisplayedStimLayer];row++){
					for(clm=0;clm<m_dHowManyClmns[ m_DisplayedStimLayer];clm++){
						pFig=&m_Fig[row][clm][ m_DisplayedStimLayer];
						if(pFig->m_Captured==1 && pFig->m_Deleted==0){
							for(i=0;i<*M_NumOfStimulationChannels;i++){
								if(pFig->m_StimGatingRelay.m_ChanID==M_StimDigitalOutChannels[i]){
									if(M_isFocusedChannel[i]==1){
										M_isFocusedChannel[i]=0;
										(*M_NumOfFocusChannels)--;
										if(NumOfFocusChannels<0)AfxMessageBox("NumOfFocusChannels<0");
										pFig->m_Focus.NameString="O ";
									}
									else{
										M_isFocusedChannel[i]=1;
										(*M_NumOfFocusChannels)++;
										pFig->m_Focus.NameString="@ ";
									}
									break;
								}
							}
						}
					}
				}
				RequestDisplayEvent(REDRAW_FOCUS);
				break;
			case SAVE_LABEL_CLICKED:
				if(*M_is_LFP_saving==0){
					m_pSpotlightFig->m_toSave=!m_pSpotlightFig->m_toSave; //Toggle the save/not save 
					if(m_pSpotlightFig->m_toSave==1)m_pSpotlightFig->m_ClickToSave.NameString="To save  "; else m_pSpotlightFig->m_ClickToSave.NameString="Save Not ";
					RequestDisplayEvent(REDRAW_SAVE_TAG);
				}
				break;
			case ZAP_LEVEL_CLICKED:
				#ifdef Zapper
				double level, dataX, dataY;
				FromScrnToDataCoordinate(point.x, point.y, dataX, dataY, m_pSpotlightFig);
				level=(dataY-pFig->m_dMinY)/(pFig->m_dMaxY-pFig->m_dMinY); 
				if(level>1){level=1;} else if(level<0){level=0;}
				lyr=m_DisplayedStimLayer;
				for(row=0;row<m_dHowManyRows[lyr];row++){
					for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
						pFig=&m_Fig[row][clm][lyr];
						if(pFig->m_Captured==1 && pFig->m_Deleted==0){
							pFig->m_LearnedLevel=level;
							Update_LearnedLevel(*M_AntidromicTriggeringChanID, pFig->m_StimGatingRelay.m_ChanID, level);
						}
					}
				}
				RequestDisplayEvent(DISP_ZAP_LEVEL,DRAW_ONLY_SELECTED);
				#endif
				break;
			default: break;
		}
	}
}

void CAntsView::UpdateScales() 
{
	if(m_dMinimumX!=m_pSpotlightFig->m_dMinX || m_dMaximumX!=m_pSpotlightFig->m_dMaxX || m_dMinimumY!=m_pSpotlightFig->m_dMinY || m_dMaximumY!=m_pSpotlightFig->m_dMaxY){
		m_pSpotlightFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
	}
	RequestDisplayEvent(RESCALING_AXES);
}

void CAntsView::DrawOneFigure(CFigure *pFig, int isToHighlight) 
{	CClientDC objDC(this);
	pFig->Compute_Mapping_XY(); 
	pFig->Frame_Graph2D(&m_DC[m_DisplayedStimLayer],1); 
	pFig->RefreshFrame(&objDC,&m_DC[m_DisplayedStimLayer]); 
	if(isToHighlight==1)Hilight_OneCapturedFig(pFig);
	else UnHilight_OneCaptured(pFig);
}

void CAntsView::UnHilight_OneCapturedFig(CFigure *pFig) 
{
	m_DC[m_DisplayedStimLayer].SelectObject(m_BLACK2);
	m_DC[m_DisplayedStimLayer].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[m_DisplayedStimLayer].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_BLACK2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
}