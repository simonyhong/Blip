#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"


void CAnalysisView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//WhichFigure(point);

	CView::OnRButtonDown(nFlags, point);
}

void CAnalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC objDC(this);
	double relX, relY;

	CRectTracker tracker; //The "rubberband" instance!!
	if(tracker.TrackRubberBand(this, point, TRUE)) {//This function will capture the mouse until the user releases the left mouse button, presses the ESC key, or presses the right mouse button. As the user moves the mouse cursor, the feedback is updated by calling DrawTrackerRect and OnChangedRect.
		// The follwoing happens AFTER the rubber band is released.
		tracker.m_rect.NormalizeRect();
		m_CapturRec.top=tracker.m_rect.top;
		m_CapturRec.left=tracker.m_rect.left;
		m_CapturRec.right=tracker.m_rect.right;
		m_CapturRec.bottom=tracker.m_rect.bottom;

		Figs_Captured();
		//RefreshEverything();
	}
	else{
		WhichFigure(point);
	}

	CView::OnLButtonDown(nFlags, point);
}
    
void CAnalysisView::Figs_Captured() 
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
BOOL CAnalysisView::RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom)
{
	if(LineCrossingRect(recLeft1,recTop1,recRight1,recTop1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recTop1,recRight1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recBottom1,recLeft1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recLeft1,recBottom1,recLeft1,recTop1, recLeft, recTop, recRight, recBottom))return 1;
	else return 0;
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CAnalysisView::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
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
    
void CAnalysisView::Hilight_AllCaptured_Figs() 
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

void CAnalysisView::Hilight_OneCapturedFig(CFigure *pFig) 
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
void CAnalysisView::UnHilight_OneCaptured(CFigure *pFig) 
{
	m_DC[0].SelectObject(m_BLACK2);
	m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_BLACK2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
}


void CAnalysisView::OnContextMenu(CWnd* pWnd, CPoint point)
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

void CAnalysisView::DisplayTitle()
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
	objDC.SelectObject(&OldPen);m_DC[0].SelectObject(&OldPen);
}

//This "WhichPane()" function changes the size of the pane and its position, when the "blow-up button" is pressed. 
void CAnalysisView::WhichFigure(CPoint point) 
{
#define BNDRY_MRGN 20
	int clm,row, isOkayed;
	CFigure *pFig;
	m_pSpotlightFig=NULL;
	CClientDC objDC(this);

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

	if(point.y<=m_dTitleRect.bottom && point.y>=m_dTitleRect.top && point.x<=m_dTitleRect.right && point.x>=m_dTitleRect.left){ 
		m_CallingFrom=1;
		OnEnChangeEditString1();
		if(m_GlobalTitle.GetLength()!=0){
			DisplayTitle();
		}
		goto END_OF_PREMEASURE;
	}

#define CLICK_ON_BLANK			0
#define AXIS_SCALE_CLICKED		1
#define FIG_LABEL_CLICKED		3
#define CHAN_ID_NN_ID_CLICKED	4
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
					    ||((point.x>=pFig->m_LastXlabel_left && point.x<=pFig->m_LastXlabel_right)&&(point.y>=pFig->m_LastXlabel_top && point.y<=pFig->m_LastXlabel_bottom))
						||((point.x>=pFig->m_FirstY2label_left && point.x<=pFig->m_FirstY2label_right)&&(point.y>=pFig->m_FirstY2label_top && point.y<=pFig->m_FirstY2label_bottom))
						||((point.x>=pFig->m_LastY2label_left && point.x<=pFig->m_LastY2label_right)&&(point.y>=pFig->m_LastY2label_top && point.y<=pFig->m_LastY2label_bottom))){
					m_dMinimumX=m_pSpotlightFig->m_dMinX; m_dMaximumX=m_pSpotlightFig->m_dMaxX; m_dMinimumY=m_pSpotlightFig->m_dMinY; m_dMaximumY=m_pSpotlightFig->m_dMaxY; m_dMinimumY2=m_pSpotlightFig->m_dMinY2; m_dMaximumY2=m_pSpotlightFig->m_dMaxY2;
					clickType=AXIS_SCALE_CLICKED;
					goto END_OF_PREMEASURE;
				}
				//Figure Labels has been clicked?
				else if((point.x>=pFig->m_Title.StringRect.left && point.x<=pFig->m_Title.StringRect.right)&&(point.y>=pFig->m_Title.StringRect.top && point.y<=pFig->m_Title.StringRect.bottom)){
					clickType=FIG_LABEL_CLICKED;
					goto END_OF_PREMEASURE;
				}
				else if((point.x>=pFig->m_NnRecordingChan.StringRect.left && point.x<=pFig->m_NnRecordingChan.StringRect.right)&&(point.y>=pFig->m_NnRecordingChan.StringRect.top && point.y<=pFig->m_NnRecordingChan.StringRect.bottom)){
					clickType=CHAN_ID_NN_ID_CLICKED;
					goto END_OF_PREMEASURE;
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
							if(m_dMinimumX!=pFig->m_dMinX || m_dMaximumX!=pFig->m_dMaxX || m_dMinimumY!=pFig->m_dMinY || m_dMaximumY!=pFig->m_dMaxY|| m_dMinimumY2!=pFig->m_dMinY2 || m_dMaximumY2!=pFig->m_dMaxY2){
								if(m_dMinimumX!=pFig->m_dMinX || m_dMaximumX!=pFig->m_dMaxX)InitSDF(m_dMaximumX-m_dMinimumX);
								pFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
								pFig->Set_2ndMinMaxY(m_dMinimumY2,m_dMaximumY2);
								//DrawOneFigure(pFig, 1);
					}	}	}		
					RequestDisplayEvent(RESCALING_AXES); //Send a message to redraw
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2;
					OnEnChangeEditString1();
					//for(clm=0;clm<m_dHowManyClmns;clm++){
					//	for(row=0;row<m_dHowManyRows;row++){
					//		pFig=&m_Fig[row][clm][0];
					//		if(pFig->m_Captured!=1)continue;
					//		pFig->m_Title.NameString=m_pSpotlightFig->m_Title.NameString;
					//		DrawOneFigure(pFig, 1);
					//}	}	
					//RequestDisplayEvent(RENAMING_FIG); //Send a message to redraw
					if(m_IsFirstCallByIconPress==0){
						int TrlNum;
						char TrialNumStr[256];
						strcpy(TrialNumStr,m_pSpotlightFig->m_Title.NameString);
						TrlNum=Find_Last_Num_inString(TrialNumStr);
						if(TrlNum>0 && TrlNum<=m_NumTrialsInList){
							m_RequestedTrlToDisp=TrlNum-1;
							Show2D_I();
						}
					}
					break;
				case CHAN_ID_NN_ID_CLICKED:
					isOkayed=0;
					ChannelID();
					if(m_IsEditOkayed==1)isOkayed++;
					NnID();
					if(isOkayed>0||m_IsEditOkayed==1){
						for(clm=0;clm<m_dHowManyClmns;clm++){
							for(row=0;row<m_dHowManyRows;row++){
								pFig=&m_Fig[row][clm][0];
								if(pFig->m_Captured!=1||pFig->m_Deleted==1)continue;
								pFig->m_ChanID=m_pSpotlightFig->m_ChanID; pFig->m_NnID=m_pSpotlightFig->m_NnID;
								if(m_IsFirstCallByIconPress==0)Show2D_I();
								pFig->DispRecordingSource(&objDC, &m_DC[0]);
						}	}	
					}
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
					if(m_dMinimumX!=m_pSpotlightFig->m_dMinX || m_dMaximumX!=m_pSpotlightFig->m_dMaxX || m_dMinimumY!=m_pSpotlightFig->m_dMinY || m_dMaximumY!=m_pSpotlightFig->m_dMaxY|| m_dMinimumY2!=pFig->m_dMinY2 || m_dMaximumY2!=pFig->m_dMaxY2){
						if(m_dMinimumX!=m_pSpotlightFig->m_dMinX || m_dMaximumX!=m_pSpotlightFig->m_dMaxX)InitSDF(m_dMaximumX-m_dMinimumX);
						m_pSpotlightFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
						m_pSpotlightFig->Set_2ndMinMaxY(m_dMinimumY2,m_dMaximumY2);
						//DrawOneFigure(m_pSpotlightFig, 1);
					}
					RequestDisplayEvent(RESCALING_AXES); //Send a message to redraw
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2; 
					OnEnChangeEditString1();
					//DrawOneFigure(m_pSpotlightFig, 1);
					//RequestDisplayEvent(RENAMING_FIG); //Send a message to redraw
					if(m_IsFirstCallByIconPress==0){
						int TrlNum;
						char TrialNumStr[256];
						strcpy(TrialNumStr,m_pSpotlightFig->m_Title.NameString);
						TrlNum=Find_Last_Num_inString(TrialNumStr);
						if(TrlNum>0 && TrlNum<=m_NumTrialsInList){
							m_RequestedTrlToDisp=TrlNum-1;
							Show2D_I();
						}
					}
					break;
				case CHAN_ID_NN_ID_CLICKED:
					isOkayed=0;
					ChannelID();
					if(m_IsEditOkayed==1)isOkayed++;
					NnID();
					if(isOkayed>0||m_IsEditOkayed==1){
						if(m_IsFirstCallByIconPress==0)Show2D_I();
						m_pSpotlightFig->DispRecordingSource(&objDC, &m_DC[0]);
					}
					break;
				default: 				
					break;
			}
		}
	}
}

void CAnalysisView::DrawOneFigure(CFigure *pFig, int isToHighlight) 
{
	CClientDC objDC(this);
	pFig->ComputeMappingXY(); 
	pFig->Frame_Graph2D(&m_DC[0],1); 
	pFig->RefreshFrame(&objDC,&m_DC[0]); 
	if(isToHighlight==1)Hilight_OneCapturedFig(pFig);
	else UnHilight_OneCaptured(pFig);
}
