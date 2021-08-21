#include "stdafx.h"
#include <math.h>

#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"


void CFigure::Frame2D_for_VIS_Replica(CDC* pDC, CMOXY_View *pView)
{
#define Tick 5
//#define Tick 10
	int nSaveDC=pDC->SaveDC();
	int shift, i;
	double  stepX,stepY, L,Lo;
	char charVarTemp[100];
	CBrush *pOldBrush;
	CPen *pOldPen;
	m_dMeanY=(m_dMinY+m_dMaxY)/2.;  m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY;
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/double(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/double(m_dSpanX));
	m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.

	pOldPen=pDC->SelectObject(&m_BlackPen2); 
	pOldBrush=pDC->SelectObject(&m_WhiteBrush);
	pDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);
	pDC->SelectObject(&m_BlackBrush);
	pDC->Rectangle(lX(m_dMinX),lY(m_dMaxY),lX(m_dMaxX),lY(m_dMinY));
	pDC->SetBkColor(RGB(255, 255, 255));

	if(m_dStepX==0)stepX=m_dSpanX/10.;
	else stepX=m_dStepX;
	if(m_dStepY==0)stepY=m_dSpanY/4.;
	else stepY=m_dStepY;
	//------------------------- Horizontal lines
	shift=0;
	for(i=0;i<m_howManyY;i++){
		Lo=lY(m_YTics[i]);
		pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo); pDC->LineTo((int)(m_nTx-Tick+m_dGapX),(int)Lo);
		if(m_IsLabeledTicY[i]){
			if(m_YTics[i]==(int)m_YTics[i])sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
			else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
			pDC->TextOut((int)(m_nTx+m_FontWidth), (int)Lo-5,charVarTemp);
		}
	}
	//------------------------- Vertical lines
	for(i=0;i<m_howManyX;i++){
		Lo=lX(m_XTics[i]);
		pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	pDC->LineTo((int)Lo,(int)(m_nBy+Tick-m_dGapY));
		if(m_IsLabeledTicX[i]){
			if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
			else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
			pDC->TextOut((int)(Lo)-3,(int)(m_nBy-0.7*m_dGapY),charVarTemp);
		}
	}
	//################  Polar Mash
	if(m_GridNet.status==1){
		PlotGrid(pDC);
		m_GridNet.NameString="GridMash";
	}
	else if(m_GridNet.status==2){
		PlotPolarMash(pDC);
		m_GridNet.NameString="PolarMash";
	}
	else{m_GridNet.NameString="No Mash";}
	pDC->TextOut(m_GridNet.StringRect.left, m_GridNet.StringRect.top,m_GridNet.NameString);

	DisplayFileName_and_RecordingStatus(pView);
	DisplayLabel(m_RwdAmount);
	pDC->RestoreDC(nSaveDC); 
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
