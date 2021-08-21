#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"

#include "AnalysisView.h"
#include "SinTable.h"


void CAnalysisView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

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

	}
	else{
	}

	CView::OnLButtonDown(nFlags, point);
}
    


