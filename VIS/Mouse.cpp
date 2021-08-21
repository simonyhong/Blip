#include "stdafx.h"
#include "VIS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "VIS_Doc.h"
#include "Figures.h"
#include "SharedConstants.h"
#include "VIS_View.h"
#include "SinTable.h"



void CVISView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//WhichFigure(point);

	CView::OnRButtonDown(nFlags, point);
}

void CVISView::OnLButtonDown(UINT nFlags, CPoint point)
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

	}

	CView::OnLButtonDown(nFlags, point);
}

//Using 4 LineCrossingRect(), see if the two rectangles cross each other.
BOOL CVISView::RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom)
{
	if(LineCrossingRect(recLeft1,recTop1,recRight1,recTop1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recTop1,recRight1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recBottom1,recLeft1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recLeft1,recBottom1,recLeft1,recTop1, recLeft, recTop, recRight, recBottom))return 1;
	else return 0;
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CVISView::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
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


