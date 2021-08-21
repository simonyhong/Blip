#include "stdafx.h"
#include "Oscilloscope.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "OscilloscopeDoc.h"
#include "FiguresOsc.h"
#include "OscilloscopeView.h"
#include "SinTable.h"
#include "DialogueBox.h"


void COscilloscopeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//WhichFigure(point);

	CView::OnRButtonDown(nFlags, point);
}

void COscilloscopeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_IsThresholdHeld){
		m_IsThresholdHeld=0;
		//The user just has released the threshold bar=> save it.
		SaveThresholds();
	}

	CView::OnLButtonUp(nFlags, point);
}

void COscilloscopeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC objDC(this);
	double relX, relY;

	//CRectTracker tracker; //The "rubberband" instance!!
	//if(tracker.TrackRubberBand(this, point, TRUE)) {//This function will capture the mouse until the user releases the left mouse button, presses the ESC key, or presses the right mouse button. As the user moves the mouse cursor, the feedback is updated by calling DrawTrackerRect and OnChangedRect.
	//	// The follwoing happens AFTER the rubber band is released.
	//	tracker.m_rect.NormalizeRect();
	//	m_CapturRec.top=tracker.m_rect.top;
	//	m_CapturRec.left=tracker.m_rect.left;
	//	m_CapturRec.right=tracker.m_rect.right;
	//	m_CapturRec.bottom=tracker.m_rect.bottom;

	//	Figs_Captured();
	//}
	//else{
		WhichFigure(point);
	//}

	CView::OnLButtonDown(nFlags, point);
}
    
#define TOL_STEP 20
void COscilloscopeView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON){
		CClientDC objDC(this);
		float pointX_inDataCoord, pointY_inDataCoord;
		FromScrnToDataCoordinate(point.x, point.y, pointX_inDataCoord, pointY_inDataCoord,m_pSpotlightFig);

		//If the curser grabs and moves the threshold line,
		if(m_IsThresholdHeld==1 || (pointX_inDataCoord<m_pSpotlightFig->m_dMinX && pointY_inDataCoord>m_pSpotlightFig->m_dMinY&& pointY_inDataCoord<m_pSpotlightFig->m_dMaxY)){ 
			m_IsThresholdHeld=1;
			CSingleLock SyncObj_Display(&m_sema); 	SyncObj_Display.Lock();
			objDC.SetROP2(R2_NOTXORPEN);
			objDC.SelectObject(m_BLUE3);
			m_DC[0].SetROP2(R2_NOTXORPEN);
			m_DC[0].SelectObject(m_BLUE3);
				
			//XOR on the previous line
			objDC.MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,m_prvThreshold_onScrn);objDC.LineTo(m_pSpotlightFig->m_nPaneBx-2,m_prvThreshold_onScrn);
			m_DC[0].MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,m_prvThreshold_onScrn);m_DC[0].LineTo(m_pSpotlightFig->m_nPaneBx-2,m_prvThreshold_onScrn);
			//Draw a new line
			objDC.MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,point.y);objDC.LineTo(m_pSpotlightFig->m_nPaneBx-2,point.y);
			m_DC[0].MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,point.y);m_DC[0].LineTo(m_pSpotlightFig->m_nPaneBx-2,point.y);
			//////
			//The threshold is directly communicated with the DAS server via "M_Threshold".
			M_Threshold[m_selectedChan]=pointY_inDataCoord; 
			m_prvThreshold_onScrn=point.y;
			objDC.SetROP2(R2_COPYPEN);
			m_DC[0].SetROP2(R2_COPYPEN);
			SyncObj_Display.Unlock();
		}
	}

	CView::OnMouseMove(nFlags, point);
}


//Due to a reason (start of a program, resize of a window...) the size of each pane needs to be made.
void COscilloscopeView::RecalibratePanes() 
{
	int clm,row;
	CFigure *pFig;

	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
			pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative data to a point on the screen.	
			pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative data to a point on the screen.
		}
	}
}
void COscilloscopeView::Figs_Captured() 
{
	int clm, row;
	CFigure *pFig;
	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
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
BOOL COscilloscopeView::RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom)
{
	if(LineCrossingRect(recLeft1,recTop1,recRight1,recTop1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recTop1,recRight1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recBottom1,recLeft1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recLeft1,recBottom1,recLeft1,recTop1, recLeft, recTop, recRight, recBottom))return 1;
	else return 0;
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL COscilloscopeView::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
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
    
void COscilloscopeView::Hilight_AllCaptured_Figs() 
{
	if(m_numCaptured>0){
		int clm, row;
		CFigure *pFig;
		m_DC[0].SelectObject(m_ORANGE2);
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				if(pFig->m_Captured==1){ 
					m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
				}
			}
		}
		CClientDC objDC(this);
		objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}
	m_DC[0].SelectObject(&m_bkPen);
}

void COscilloscopeView::Hilight_OneCapturedFig(CFigure *pFig) 
{
	m_DC[0].SelectObject(m_ORANGE2);
	m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 
	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_ORANGE2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	
	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
	m_DC[0].SelectObject(&m_bkPen);
}
void COscilloscopeView::UnHilight_OneCaptured(CFigure *pFig) 
{
	m_DC[0].SelectObject(m_BLACK2);
	m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_BLACK2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
}


void COscilloscopeView::OnContextMenu(CWnd* pWnd, CPoint point)
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

void COscilloscopeView::DisplayGlobalTitle()
{
#define HeightWidthRatio 0.4
	CClientDC objDC(this);
	m_DC[0].SetBkColor(RGB(220, 220, 220));

	int m_centerOfTitle= (m_TitleArea.left+m_TitleArea.right)*0.5;
	int length=m_GlobalTitle.GetLength(); 
	if(length==0){AfxMessageBox("Check if you have done m_GlobalTitle="" somewhere."); exit(0);}//Sometimes multi-thread causes problems.
	CSize size=m_DC[0].GetOutputTextExtent(m_GlobalTitle);
	int WidthOfTitle=size.cx+1;	
	CPen *OldPen=objDC.SelectObject(&m_NULL); m_DC[0].SelectObject(&m_NULL);
	m_DC[0].SelectObject(m_bkBrush);objDC.SelectObject(m_bkBrush);
	{m_DC[0].Rectangle(m_dTitleRect.left,m_dTitleRect.top,m_dTitleRect.right,m_dTitleRect.bottom); objDC.Rectangle(m_dTitleRect.left,m_dTitleRect.top,m_dTitleRect.right,m_dTitleRect.bottom); }
	m_DoNotErase=0;
	m_dTitleRect.top=0+0.1*m_topOfFigs; m_dTitleRect.bottom=m_TitleArea.bottom; m_dTitleRect.left=m_centerOfTitle-WidthOfTitle*0.5; m_dTitleRect.right=m_centerOfTitle+WidthOfTitle*0.5;
	FontInRect(&m_DC[0],m_GlobalTitle,m_dTitleRect);
	objDC.BitBlt(m_dTitleRect.left, m_dTitleRect.top, WidthOfTitle, m_topOfFigs,&m_DC[0],m_dTitleRect.left, m_dTitleRect.top,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
	m_DC[0].SelectObject(&OldPen);
}

void COscilloscopeView::WhichFigure(CPoint point) 
{
#define BNDRY_MRGN 20
	int clm,row;
	CFigure *pFig;
	m_pSpotlightFig=&m_Fig[0][0][0];

	//If the rectangle on screen (m_paneTopX, m_paneTopY, m_paneBottomX, m_paneBottomY) is not defined for the graph pane, define it!
	if(!m_bIsScrXYforIJ){ 
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
				pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
			}
		}
		m_bIsScrXYforIJ=1;
	}

	if(point.y<=m_topOfFigs){ 
		m_CallingFrom=1;
		OnEnChangeEditString1();
		if(m_GlobalTitle.GetLength()!=0){
			DisplayGlobalTitle();
		}
		goto END_OF_PREMEASURE;
	}

#define CLICK_ON_BLANK			0
#define AXIS_SCALE_CLICKED		1
#define FIG_LABEL_CLICKED		3
	int	clickType, found=-1;
	//Here it PRE-examines if the user has already chosen some Figures before clicking an icon to "Batch" manipulate them.
	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
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
				//Channel # has been clicked?
				else if((point.x>=pFig->m_nPaneTx && point.x<=pFig->m_nPaneBx)&&(point.y>=pFig->m_Title.StringRect.top && point.y<=pFig->m_Title.StringRect.bottom)){
					int tempID=pFig->WhichChannelSelected(point);
					if(tempID!=-1)m_selectedChan=tempID;
					pFig->SetMinMaxXY(m_dMinimumX=pFig->m_dChMinX[m_selectedChan],m_dMaximumX=pFig->m_dChMaxX[m_selectedChan],m_dMinimumY=pFig->m_dChMinY[m_selectedChan],m_dMaximumY=pFig->m_dChMaxY[m_selectedChan]);
					if(m_selectedChan!=m_PrvSelectedChan){
						m_StartAfresh=1; //To reset: pAI=M_AI[m_selectedChan];
						//m_PrvSelectedChan=m_selectedChan; =>This is done at Oscilloscope_thread()
					}
					RequestDisplayEvent(REDRAW_FIG);
					return;
				}
				else{
					clickType=CLICK_ON_BLANK;
					goto END_OF_PREMEASURE;
				}
			}
		}
	}
	END_OF_PREMEASURE:

	//There are three pices of information: (1) which fig is clicked? (2) if a figure is clicked, is it already selected? (3) what kind of clickType is this?
	if(m_pSpotlightFig==NULL){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				if(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL){//Clear previous selection unless Control key is pressed
					if(pFig->m_Captured==1){
						pFig->m_Captured=0;
						UnHilight_OneCaptured(pFig); 
						m_numCaptured--;
					}
				}
			}
		}
	}
	//Clicked a Figure!!
	else{
		//Is the clicked figure already selected?
		if(m_pSpotlightFig->m_Captured==1){
			switch(clickType){
				case AXIS_SCALE_CLICKED:
					OnChangeParameters();
					for(clm=0;clm<m_dHowManyClmns;clm++){
						for(row=0;row<m_dHowManyRows;row++){
							pFig=&m_Fig[row][clm][0];
							if(pFig->m_Captured!=1)continue;
							if(m_dMinimumX!=pFig->m_dMinX || m_dMaximumX!=pFig->m_dMaxX || m_dMinimumY!=pFig->m_dMinY || m_dMaximumY!=pFig->m_dMaxY){
								pFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
								if(m_selectedChan!=-1){pFig->m_dChMinX[m_selectedChan]=pFig->m_dMinX; pFig->m_dChMaxX[m_selectedChan]=pFig->m_dMaxX; pFig->m_dChMinY[m_selectedChan]=pFig->m_dMinY; pFig->m_dChMaxY[m_selectedChan]=pFig->m_dMaxY;	}
								RequestDisplayEvent(REDRAW_FIG);
					}	}	}						
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2;
					OnEnChangeEditString1();
					for(clm=0;clm<m_dHowManyClmns;clm++){
						for(row=0;row<m_dHowManyRows;row++){
							pFig=&m_Fig[row][clm][0];
							if(pFig->m_Captured!=1)continue;
							pFig->m_Title.NameString=m_pSpotlightFig->m_Title.NameString;
							RequestDisplayEvent(REDRAW_FIG);
					}	}	
					break;
				default: break;
			}
		}
		else{
			//First clear all the perior-selected ones and highlight only the newly clicked one
			if(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL){//Clear previous selection unless Control key is pressed
				for(clm=0;clm<m_dHowManyClmns;clm++){
					for(row=0;row<m_dHowManyRows;row++){
						pFig=&m_Fig[row][clm][0];
						if(pFig->m_Captured==1){
							pFig->m_Captured=0;
							UnHilight_OneCaptured(pFig); 
						}
					}
				}
				m_numCaptured=0;
			}
			m_pSpotlightFig->m_Captured=1;
			m_numCaptured++;
			Hilight_OneCapturedFig(m_pSpotlightFig);
			//Do the user-requested job.
			switch(clickType){
				case AXIS_SCALE_CLICKED:
					OnChangeParameters();
					if(m_dMinimumX!=m_pSpotlightFig->m_dMinX || m_dMaximumX!=m_pSpotlightFig->m_dMaxX || m_dMinimumY!=m_pSpotlightFig->m_dMinY || m_dMaximumY!=m_pSpotlightFig->m_dMaxY){
						m_pSpotlightFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
						if(m_selectedChan!=-1){m_pSpotlightFig->m_dChMinX[m_selectedChan]=m_pSpotlightFig->m_dMinX; m_pSpotlightFig->m_dChMaxX[m_selectedChan]=m_pSpotlightFig->m_dMaxX; m_pSpotlightFig->m_dChMinY[m_selectedChan]=m_pSpotlightFig->m_dMinY; m_pSpotlightFig->m_dChMaxY[m_selectedChan]=m_pSpotlightFig->m_dMaxY;	}
						RequestDisplayEvent(REDRAW_FIG);
					}
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2; 
					OnEnChangeEditString1();
					RequestDisplayEvent(REDRAW_FIG);
					break;
				default: 				

				break;
			}
		}
	}
}

