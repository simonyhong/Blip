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
#include "DialogueBox.h"


void CDasView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_IsValidOutput=WhichFigure(point);
	if(m_IsValidOutput==0 || m_pSpotlightFig==NULL)return;//Hit outside the pane or invalid Panel
	VERIFY(m_pSpotlightFig->pChan); //The same as ASSERT but works both in Debug and Release modes.
	CAiChannel *pChan=m_pSpotlightFig->pChan;
	if(pChan==NULL)return;//If the Figure is not attached to any channel, don't do anything.
	if(pChan->m_RtClickSelectedBoxID>=0){//If the user clicks the right button again, after a right click, void the right-click selection.
		pChan->DrawOneVTbox(pChan->m_RtClickSelectedBoxType, pChan->m_RtClickSelectedBoxIsToAccept, pChan->m_RtClickSelectedBoxID,  0);//De-highlight the previously right-click chosen box
		pChan->m_RtClickSelectedBoxID=-1;//If the user clicks after a right click, void the right click.
	}

	if(m_pSpotlightFig==NULL)return;//Hit outside the pane

	int colorType,isToAccept,DummyisForEdge;
	int boxID=pChan->WhichBox(point,colorType,isToAccept,DummyisForEdge);
	if(boxID<0)return;
	pChan->m_RtClickSelectedBoxID=boxID; pChan->m_RtClickSelectedBoxIsToAccept=isToAccept; pChan->m_RtClickSelectedBoxType=colorType;
	pChan->DrawOneVTbox(colorType,isToAccept, boxID,  1);

	CView::OnRButtonDown(nFlags, point);
}

void CDasView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_IsValidOutput==0 || m_pSpotlightFig==NULL)return;//Hit outside the pane or invalid Panel
	CAiChannel *pChan=m_pSpotlightFig->pChan;
	if(pChan==NULL)return;//If the Figure is not attached to any channel, don't do anything.
	pChan->m_prvPoint.x=-1; pChan->m_prvPoint.y=-1;//This reset prevents the jerkey "repositioning" of the box when the mouse grabes it.
	if(pChan->m_hasBoxMoved){
		//######## Clear the flags ########
		pChan->m_hasBoxMoved=0; pChan->m_selectedBoxID=-1;
	}
	if(pChan->m_IsThresholdHeld){
		pChan->m_IsThresholdHeld=0; m_IsThresholdHeld=0;
	}
	if(m_pSpotlightFig->m_IsScaleHeld){
		m_pSpotlightFig->m_IsScaleHeld=0;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CDasView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_IsThresholdHeld)return;
	m_isInsidePane=0;
	WhichFigure(point);
	if(m_dontDoRubberbending==1){m_dontDoRubberbending=0; return;}//This is triggered by the axes menu bar clicking. This prevents the ruberbending.
	if(m_pSpotlightFig==NULL)return;//Hit outside the pane

	CAiChannel *pChan=m_pSpotlightFig->pChan;
	if(pChan==NULL)return;//If the Figure is not attached to any channel, don't do anything.
	#define TOL_STEP 20
	//The following maps a screen point (x,y) to a relative point (i,j) of the graph.
	float pointX_inDataCoord, pointY_inDataCoord;
	FromScrnToDataCoordinate(point.x, point.y, pointX_inDataCoord, pointY_inDataCoord,m_pSpotlightFig) ;

	//This is to stop the rubber-bending when the mouse is handling the threshold line.
	if(abs(m_pSpotlightFig->m_prvThreshold_onScrn-point.y)<TOL_STEP && pointX_inDataCoord<m_pSpotlightFig->m_dMinX)return;
	if(abs(m_pSpotlightFig->m_prvScaleY_onScreen-point.y)<TOL_STEP && pointX_inDataCoord>m_pSpotlightFig->m_dMaxX)return;
	if(pChan->m_RtClickSelectedBoxID>=0){//If the user clicks the left button, after a right click, void the right click.
		pChan->DrawOneVTbox(pChan->m_RtClickSelectedBoxType,pChan->m_RtClickSelectedBoxIsToAccept, pChan->m_RtClickSelectedBoxID, 0);//De-highlight the previously right-click chosen box
		pChan->m_RtClickSelectedBoxID=-1;//If the user clicks after a right click, void the right click.
	}
	int colorType,DummyisForEdge,isToAccept;
	int boxID_BtnDn;
	boxID_BtnDn=pChan->WhichBox(point,colorType,isToAccept,DummyisForEdge); 
	if(boxID_BtnDn>=0)return; //A box has been chosen

	if(m_isInsidePane==1){
		CRectTracker tracker; // The "rubberband" instance!!
		if(tracker.TrackRubberBand(this, point, TRUE)) {//This function will capture the mouse until the user releases the left mouse button, presses the ESC key, or presses the right mouse button. As the user moves the mouse cursor, the feedback is updated by calling DrawTrackerRect and OnChangedRect.
			// The follwoing happens AFTER the rubber-banding stops.

			//Adjust the rectangle to prevent some mistakes by the user. Especially along the borders of the window.
			tracker.m_rect.NormalizeRect();
 			if(tracker.m_rect.left<m_pSpotlightFig->m_nPaneTx)tracker.m_rect.left=m_pSpotlightFig->m_nPaneTx;
			if(tracker.m_rect.right>m_pSpotlightFig->m_nPaneBx)tracker.m_rect.right=m_pSpotlightFig->m_nPaneBx;
			if(tracker.m_rect.top<m_pSpotlightFig->m_nPaneTy)tracker.m_rect.top=m_pSpotlightFig->m_nPaneTy;
			if(tracker.m_rect.bottom>m_pSpotlightFig->m_nPaneBy)tracker.m_rect.bottom=m_pSpotlightFig->m_nPaneBy;		
			if(tracker.m_rect.right-tracker.m_rect.left<10)goto NullAction;
			if(tracker.m_rect.bottom-tracker.m_rect.top<10)goto NullAction;

			pChan->m_prvPoint.x=-1; pChan->m_prvPoint.y=-1;//This part ("left button up" after the rubber-banding) catches the moment where the Left Button has been released: This moment is not detected by OnLButtonUp() function!!
			if(m_KeyCode==-1){//No key is pressed in combination with the mouse movement==>Select

				pChan->m_SelRectsOnScrn[m_SelColor][pChan->m_numSelRects[m_SelColor]]=tracker.m_rect;
				pChan->m_numSelRectsTmp[m_SelColor]++; //This stores the number temporarily until the necessary steps has been taken and it is ready to increase "real" variable m_numSelRects.
				pChan->RegisterSelBox(m_SelColor, 1, pChan->m_numSelRects[m_SelColor]);
			}
			else {//some key is being pressed, in combination with the mouse move
				pChan->m_RejRectsOnScrn[m_SelColor][pChan->m_numRejRects[m_SelColor]]=tracker.m_rect;
				pChan->m_numRejRectsTmp[m_SelColor]++; //This stores the number temporarily until the necessary steps has been taken and it is ready to increase "real" variable m_numRejRects.
				pChan->RegisterSelBox(m_SelColor, 0, pChan->m_numRejRects[m_SelColor]);
			}
			m_CapturRec=tracker.m_rect;
			Figs_Captured();
		}
	}
	NullAction:
	CView::OnLButtonDown(nFlags, point);
}
    
void CDasView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON){
		if(!m_IsThresholdHeld){
			WhichFigure(point);
		}
		if(m_pSpotlightFig==NULL)return;//Hit outside the pane
		CAiChannel *pChan=m_pSpotlightFig->pChan;
		if(pChan==NULL)return;//If the Figure is not attached to any channel, don't do anything.
		int chanID=pChan->m_ChanID;

		CClientDC objDC(this);
		float pointX_inDataCoord, pointY_inDataCoord;
		FromScrnToDataCoordinate(point.x, point.y, pointX_inDataCoord, pointY_inDataCoord,m_pSpotlightFig);

		if(pChan->m_selectedBoxID>=0){//A box has already been captured:==> This if() solves the problem where the once-captured box gets lost because the mouse moved so fast that it has got out of the box temporarily. 
			if(pChan->m_hasBoxMoved!=1)AfxMessageBox("m_selectedBoxID>0,  but m_hasBoxMoved!=1?");
			if(pChan->m_EdgeMoveType<0){//Moving a box
				pChan->RedrawDiscriminatorRect(point, pChan->m_selectedBoxType, pChan->m_selectedIsToAccept, pChan->m_selectedBoxID);
			}
			else{//Resizing a box
				pChan->Redraw_BoxEdge(point, pChan->m_selectedBoxType, pChan->m_selectedIsToAccept, pChan->m_selectedBoxID, pChan->m_EdgeMoveType);
			}
		}
		else{//See if the mouse pointer is inside of a VT box.
			if(!pChan->m_IsThresholdHeld){
				int colorType,isToAccept,edgeMoveType;
				int boxID=pChan->WhichBox(point,colorType,isToAccept, edgeMoveType);
				if(boxID>=0){//A box has just been captured: Redraw the box
					pChan->m_hasBoxMoved=1;  pChan->m_selectedBoxType=colorType; pChan->m_selectedIsToAccept=isToAccept; pChan->m_selectedBoxID=boxID; pChan->m_EdgeMoveType=edgeMoveType;
					if(edgeMoveType<0){//Moving a box
						pChan->RedrawDiscriminatorRect(point, colorType, isToAccept, boxID);
					}
					else{//Resizing a box
						pChan->Redraw_BoxEdge(point, colorType, isToAccept, boxID, edgeMoveType);
					}
				}
			}
			//If the curser hits the threshold line,
			if(pChan->m_IsThresholdHeld ||(abs(m_pSpotlightFig->m_prvThreshold_onScrn-point.y)<TOL_STEP && pointX_inDataCoord<m_pSpotlightFig->m_dMinX)){ 
				pChan->m_IsThresholdHeld=1; m_IsThresholdHeld=1;
				objDC.SetROP2(R2_NOTXORPEN);
				objDC.SelectObject(m_BLUE3);
				m_DC[0].SetROP2(R2_NOTXORPEN);
				m_DC[0].SelectObject(m_BLUE3);
				
				//XOR on the previous line
				objDC.MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,m_pSpotlightFig->m_prvThreshold_onScrn);objDC.LineTo(m_pSpotlightFig->m_nPaneBx-2,m_pSpotlightFig->m_prvThreshold_onScrn);
				m_DC[0].MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,m_pSpotlightFig->m_prvThreshold_onScrn);m_DC[0].LineTo(m_pSpotlightFig->m_nPaneBx-2,m_pSpotlightFig->m_prvThreshold_onScrn);
				//Draw a new line
				objDC.MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,point.y);objDC.LineTo(m_pSpotlightFig->m_nPaneBx-2,point.y);
				m_DC[0].MoveTo(m_pSpotlightFig->m_nPaneTx-2*m_pSpotlightFig->m_FontWidth,point.y);m_DC[0].LineTo(m_pSpotlightFig->m_nPaneBx-2,point.y);
				//////
				//The threshold is directly communicated with the DAS server via "M_Threshold".
				if(m_pSpotlightFig->pChan!=NULL)*(pChan->M_Threshold)=pointY_inDataCoord; 
				m_pSpotlightFig->m_Threshold=pointY_inDataCoord; 
				m_pSpotlightFig->m_prvThreshold_onScrn=point.y;
				objDC.SetROP2(R2_COPYPEN);
				m_DC[0].SetROP2(R2_COPYPEN);
			}
			else if(m_pSpotlightFig->m_IsScaleHeld ||(abs(m_pSpotlightFig->m_prvScaleY_onScreen-point.y)<TOL_STEP && pointX_inDataCoord>m_pSpotlightFig->m_dMaxX)){
				m_pSpotlightFig->m_IsScaleHeld=1;
				objDC.SetROP2(R2_NOTXORPEN);
				objDC.SelectObject(m_VIOLET3);
				m_DC[0].SetROP2(R2_NOTXORPEN);
				m_DC[0].SelectObject(m_VIOLET3);
				//XOR on the previous line
				objDC.MoveTo(m_pSpotlightFig->m_nPaneBx+2,m_pSpotlightFig->m_prvScaleY_onScreen);objDC.LineTo(m_pSpotlightFig->m_nBx-2,m_pSpotlightFig->m_prvScaleY_onScreen);
				m_DC[0].MoveTo(m_pSpotlightFig->m_nPaneBx+2,m_pSpotlightFig->m_prvScaleY_onScreen);m_DC[0].LineTo(m_pSpotlightFig->m_nBx-2,m_pSpotlightFig->m_prvScaleY_onScreen);
				//Draw a new line
				objDC.MoveTo(m_pSpotlightFig->m_nPaneBx+2,point.y);objDC.LineTo(m_pSpotlightFig->m_nBx-2,point.y);
				m_DC[0].MoveTo(m_pSpotlightFig->m_nPaneBx+2,point.y);m_DC[0].LineTo(m_pSpotlightFig->m_nBx-2,point.y);
				//////
				FromScrnToDataCoordinate2(point.y, m_pSpotlightFig->m_ScaleY,m_pSpotlightFig); 
				m_pSpotlightFig->m_prvScaleY_onScreen=point.y;
				// Reflect the change of Y scale
				double relY=((double)(m_pSpotlightFig->m_Threshold-m_pSpotlightFig->m_dMinY))/(m_pSpotlightFig->m_dMaxY-m_pSpotlightFig->m_dMinY);// This line and the 2 lines below together, corrects the position of the threshold after the change of the scale fo the axes.
				if(m_pSpotlightFig->m_ScaleY>=0){
					m_pSpotlightFig->m_dMaxY=m_pSpotlightFig->m_dTypedInHalfBaseY+0.5*(m_pSpotlightFig->m_dTypedInMaxBaseY-m_pSpotlightFig->m_dTypedInMinBaseY)/(1+m_pSpotlightFig->m_ScaleY);
					m_pSpotlightFig->m_dMinY=m_pSpotlightFig->m_dTypedInHalfBaseY-0.5*(m_pSpotlightFig->m_dTypedInMaxBaseY-m_pSpotlightFig->m_dTypedInMinBaseY)/(1+m_pSpotlightFig->m_ScaleY);
				}
				else{
					m_pSpotlightFig->m_dMaxY=m_pSpotlightFig->m_dTypedInHalfBaseY+0.5*(m_pSpotlightFig->m_dTypedInMaxBaseY-m_pSpotlightFig->m_dTypedInMinBaseY)*(1-m_pSpotlightFig->m_ScaleY);
					m_pSpotlightFig->m_dMinY=m_pSpotlightFig->m_dTypedInHalfBaseY-0.5*(m_pSpotlightFig->m_dTypedInMaxBaseY-m_pSpotlightFig->m_dTypedInMinBaseY)*(1-m_pSpotlightFig->m_ScaleY);
				}
				//Register the change of scale
				m_pSpotlightFig->SetMinMaxY(m_pSpotlightFig->m_dMinY,m_pSpotlightFig->m_dMaxY);
				//Update the threshold
				m_pSpotlightFig->m_Threshold=m_pSpotlightFig->m_dMinY+relY*(m_pSpotlightFig->m_dMaxY-m_pSpotlightFig->m_dMinY);
				*(m_pSpotlightFig->pChan->M_Threshold)=m_pSpotlightFig->m_Threshold;
				//Update the selection boxes: Fixing the position of the box on the screen, recalculate the Y data points (top, bottom) of the box rectangle.
				m_pSpotlightFig->pChan->Remeasure_Box_YPositions_ScreenAnchored();//This updates the VTBox info and informs the server the changes
				m_pSpotlightFig->pChan->UpdateServerWithVTBoxes();//Inform the change to the server.

				objDC.SetROP2(R2_COPYPEN);
				m_DC[0].SetROP2(R2_COPYPEN);
				RequestDisplayEvent(DISP_Y_SCALE);  //Send a message to redraw
			}
		}
	}
	CView::OnMouseMove(nFlags, point);
}

void CDasView::Figs_Captured() 
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
BOOL CDasView::RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom)
{
	if(LineCrossingRect(recLeft1,recTop1,recRight1,recTop1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recTop1,recRight1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recBottom1,recLeft1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recLeft1,recBottom1,recLeft1,recTop1, recLeft, recTop, recRight, recBottom))return 1;
	else return 0;
}

void CDasView::Hilight_AllCaptured_Figs() 
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

void CDasView::Hilight_OneCapturedFig(CFigure *pFig) 
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

void CDasView::UnHilight_OneCaptured(CFigure *pFig) 
{
	m_DC[0].SelectObject(m_BLACK2);
	m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_BLACK2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
}

void CDasView::DisplayTitle()
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
	m_dTitleRect.top=0; m_dTitleRect.bottom=m_TitleArea.bottom; m_dTitleRect.left=m_centerOfTitle-WidthOfTitle*0.5; m_dTitleRect.right=m_centerOfTitle+WidthOfTitle*0.5;
	//FontInRect(&m_DC[0],m_GlobalTitle,m_dTitleRect); //??
	m_DC[0].SetBkColor(RGB(220, 220, 220));
	m_DC[0].TextOut( m_dTitleRect.left,m_dTitleRect.top+1, m_GlobalTitle);

	objDC.BitBlt(m_dTitleRect.left, m_dTitleRect.top, WidthOfTitle, m_topOfFigs,&m_DC[0],m_dTitleRect.left, m_dTitleRect.top,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
	m_DC[0].SelectObject(&OldPen);
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CDasView::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
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
   
//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CDasView::LineCrossingRect(int x1,double y1,int x2,double y2,float recLeft,float recTop,float recRight,float recBottom)
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

//Due to a reason (start of a program, resize of a window...) screen positions of the left, right, up, down edges of all panes need to be assigned.
void CDasView::RecalibratePanes() 
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

//This "WhichPane()" function changes the size of the pane and its position, when the "blow-up button" is pressed. 
int CDasView::WhichFigure(CPoint point) 
{
#define BNDRY_MRGN 20
	int clm,row;
	m_IsValidOutput=1;
	CFigure *pFig;
	m_pSpotlightFig=NULL;

	//If the rectangle on screen (m_paneTopX, m_paneTopY, m_paneBottomX, m_paneBottomY) is not defined for the graph pane, define it!
	if(!m_bIsScrXYforIJ){ 
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
				pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
		}	}
		m_bIsScrXYforIJ=1;
	}

	if(point.y<=m_topOfFigs){ 
		if(point.x>=m_dTitleRect.left && point.x<=m_dTitleRect.right){
			m_CallingFrom=1;
			OnEnChangeEditString1();
			if(m_GlobalTitle.GetLength()!=0){
				DisplayTitle();
			}
			goto END_OF_PREMEASURE;
		}
		if(point.x>=m_FileNameRect.left && point.x<=m_FileNameRect.right){
			//Prefix & Suffix
			EditPrefixSuffix();
			//Which channel to save?
			m_CallingFrom=3;
			m_EnteredChanID=m_RecordingChanId;
			OnEnChangeEditString1();
			if(m_EnteredChanID<0||m_EnteredChanID>=NUM_NN_AI_CHAN){AfxMessageBox("The specified channel number is out of the maximum (MAX_AI_CHAN-1).");return m_IsValidOutput;}//Some invalid channel number has been entered.
			//Stop after saving N waves
			m_CallingFrom=4;
			m_EnteredChanID=m_StopAfterSavingNwaves;
			OnEnChangeEditString1();
			//Saving-realted variables to speed up the saving
			DesignateRecordingChannel(m_RecordingChanId);
			RequestDisplayEvent(DISP_FILE_NAME);
			return m_IsValidOutput;
		}
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
				m_pSpotlightFig=pFig; *M_SpotlightChanID_forCLAS=pFig->m_ChanID;
				if(*M_SpotlightChanID_forCLAS<0){m_IsValidOutput=0; return m_IsValidOutput;}
				if((point.x>=pFig->m_nPaneTx && point.x<=pFig->m_nPaneBx)&&(point.y<=pFig->m_nPaneBy&&point.y>=pFig->m_nPaneTy)){//Clicked somewhere inside of the PANE
					m_isInsidePane=1;
				}
				m_minPaneX=pFig->m_dMinX; m_maxPaneX=pFig->m_dMaxX; m_minPaneY=pFig->m_dMinY; m_maxPaneY=pFig->m_dMaxY;
				m_paneTopX=(int)pFig->m_nPaneTx; m_paneTopY=(int)pFig->m_nPaneTy; m_paneBottomX=(int)pFig->m_nPaneBx; m_paneBottomY=(int)pFig->m_nPaneBy;
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
				//Figure Labels has been clicked?
				else if((point.x>=pFig->m_AssociatedChan.StringRect.left && point.x<=pFig->m_AssociatedChan.StringRect.right)&&(point.y>=pFig->m_AssociatedChan.StringRect.top && point.y<=pFig->m_AssociatedChan.StringRect.bottom)){
					clickType=FIG_LABEL_CLICKED;
					goto END_OF_PREMEASURE;
				}
				//Channal Label has been clicked?
				else if((point.x>=pFig->m_AssociatedChan.StringRect.left && point.x<=pFig->m_AssociatedChan.StringRect.right)&&(point.y>=pFig->m_AssociatedChan.StringRect.top && point.y<=pFig->m_AssociatedChan.StringRect.bottom)){
					clickType=FIG_LABEL_CLICKED;
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

	//No figure has been clicked
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
		int recalibrateBoxes=0;
		if(m_pSpotlightFig->m_Captured==1){
			switch(clickType){
				case AXIS_SCALE_CLICKED://Scale of axes 
					OnChangeParameters();
					double relY;
					for(clm=0;clm<m_dHowManyClmns;clm++){
						for(row=0;row<m_dHowManyRows;row++){
							pFig=&m_Fig[row][clm][0];
							if(pFig->m_Captured!=1)continue;
							if(m_dMinimumX!=pFig->m_dMinX || m_dMaximumX!=pFig->m_dMaxX || m_dMinimumY!=pFig->m_dMinY || m_dMaximumY!=pFig->m_dMaxY){
								pFig->RescaleAxes(m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY);
							}	
					}	}		
					RequestDisplayEvent(RESCALING_AXES);  //Send a message to redraw
					break;
				case FIG_LABEL_CLICKED:
					if(m_numCaptured>1)AfxMessageBox("Only the last-clicked one will be changed");
					m_CallingFrom=2; 
					m_EnteredChanID=m_pSpotlightFig->m_ChanID;
					OnEnChangeEditString1();
					if(m_EnteredChanID<0||m_EnteredChanID>NUM_NN_AI_CHAN-1){//Some invalid channel number has been entered.
						char MsgBuff[1024]; sprintf(MsgBuff,"If you would like to use more than the preset number (%d) of recording channels, change the number:NUM_NN_AI_CHAN in the program and recompile.",NUM_NN_AI_CHAN);
						AfxMessageBox(MsgBuff);
						DisconnectFigure_and_StopClientChan(m_pSpotlightFig->m_ChanID);
						m_IsValidOutput=0;
					}
					else if(m_EnteredChanID!=m_pSpotlightFig->m_ChanID ||m_pSpotlightFig->pChan==NULL){
						//If the figure had an existing attached Channel,
						if(m_pSpotlightFig->pChan!=NULL){
							StopClientChan_and_RemoveFigInfoInClientChan(m_pSpotlightFig->m_ChanID);
						}
						//Attach the figure to a new Channel
						AttachFig_toChan(m_pSpotlightFig,m_EnteredChanID);
					}
					RequestDisplayEvent(RENAMING_FIG); //Send a message to redraw
					break;
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
						m_pSpotlightFig->RescaleAxes(m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY);
					}
					RequestDisplayEvent(RESCALING_AXES);  //Send a message to redraw
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2; 
					m_EnteredChanID=m_pSpotlightFig->m_ChanID;
					OnEnChangeEditString1();
					if(m_EnteredChanID<0||m_EnteredChanID>NUM_NN_AI_CHAN-1){//Some invalid channel number has been entered.
						DisconnectFigure_and_StopClientChan(m_pSpotlightFig->m_ChanID);
					}
					else if(m_EnteredChanID!=m_pSpotlightFig->m_ChanID||m_pSpotlightFig->pChan==NULL){
						//If the figure had an existing attached Channel,
						if(m_pSpotlightFig->pChan!=NULL){
							StopClientChan_and_RemoveFigInfoInClientChan(m_pSpotlightFig->m_ChanID);
						}
						//Attach the figure to a new Channel
						AttachFig_toChan(m_pSpotlightFig,m_EnteredChanID);
					}
					RequestDisplayEvent(RENAMING_FIG); //Send a message to redraw
					break;
				default: 				
				break;
			}
		}
	}
	return m_IsValidOutput;
}

//##################################################################################################
//#########################   Change of Figure->Channel configuration  #############################

//This one "unplugs" the figure (pChan->pFig->pChan=NULL) but leaves the figure's previously connected channel's ID (m_ChanID) in case it needs to reconnect.
//Also it removes the figure info in the channel (pChan->pFig=NULL), and disables the client channel (pChan->m_IsThisChanUsed=0;), and informs this fact to the server (M_NumOfFigsUsingThisChan[]-- )
void CDasView::DisconnectFigure_and_StopClientChan(int ChanID)
{
	if(ChanID<0 || ChanID>=NUM_NN_AI_CHAN){AfxMessageBox("ChanID<0 || ChanID>=NUM_NN_AI_CHAN"); return;}
	m_AiChan[ChanID].pFig->pChan=NULL; //Unplug the figure from the channel.
	m_AiChan[ChanID].pFig->m_ChanID=-1;
	StopClientChan_and_RemoveFigInfoInClientChan(ChanID);
}

void CDasView::StopClientChan_and_RemoveFigInfoInClientChan(int ChanID)
{
	if(ChanID<0 || ChanID>=NUM_NN_AI_CHAN){AfxMessageBox("ChanID<0 || ChanID>=NUM_NN_AI_CHAN"); return;}
	CAiChannel *pChan=&m_AiChan[ChanID];
	pChan->m_IsThisChanUsed=0;
	if(M_NumOfFigsUsingThisChan[ChanID]>0)M_NumOfFigsUsingThisChan[ChanID]--; //Inform the server that the figure is not using this channel anymore.
	pChan->pFig=NULL;
}

//This one disables the figure (pFig->pChan=NULL) but leaves the figure's previously connected channel's ID (m_ChanID) in case it needs to reconnect.
void CDasView::UnplugFigureFromChan(CFigure *pFig)
{
	pFig->m_AssociatedChan.NameString="No Chan"; //Inform the previous channel that the previous figure has been detatched.
	pFig->pChan=NULL; //Unplug the figure from the channel.
	pFig->m_ChanID=-1;
}
//
void CDasView::AttachFig_toChan(CFigure *pFig, int ChanID)
{
	//If the new channel had an attached figure, detatch it.
	if(pFig->pChan!=NULL && pFig->pChan->pFig!=NULL && pFig->m_ChanID!=ChanID){
		UnplugFigureFromChan(pFig->pChan->pFig);
	}
	pFig->m_ChanID=ChanID; 
	pFig->pChan=&m_AiChan[ChanID];
	if(pFig->pChan->m_IsThisChanUsed!=1){
		pFig->pChan->m_IsThisChanUsed=1;
		M_NumOfFigsUsingThisChan[pFig->m_ChanID]++; 
	}
	//Inform the new channel that a new figure has arrived.
	pFig->pChan->pFig=pFig;
	char ChanLabel[256];
	sprintf(ChanLabel,"Channel: %d",ChanID);
	pFig->m_AssociatedChan.NameString=ChanLabel;
}
//######################################################################################################