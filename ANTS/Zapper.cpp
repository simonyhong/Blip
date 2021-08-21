
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


void CAntsView::Disp_ZapStatus()
{
#define HeightWidthRatio 0.4
	CClientDC objDC(this);
	m_DC[m_DisplayedStimLayer].SetBkColor(RGB(220, 220, 220));

	CString sZapStatus="Learn";
	CSize size=m_DC[m_DisplayedStimLayer].GetOutputTextExtent(sZapStatus);
	int WidthOfTitle=size.cx+1;	
	int CenterOfZapStatus= m_TitleArea.right-20; if(CenterOfZapStatus<0)CenterOfZapStatus=0;
	CPen *OldPen=objDC.SelectObject(&m_NULL_pen); m_DC[m_DisplayedStimLayer].SelectObject(&m_NULL_pen);
	CBrush *OldBrush=m_DC[m_DisplayedStimLayer].SelectObject(&m_bkBrush);objDC.SelectObject(m_bkBrush);
	{m_DC[m_DisplayedStimLayer].Rectangle(m_ZapRect.left,m_ZapRect.top,m_ZapRect.right,m_ZapRect.bottom); objDC.Rectangle(m_ZapRect.left,m_ZapRect.top,m_ZapRect.right,m_ZapRect.bottom); }
	m_ZapRect.top=0+0.1*m_topOfFigs; m_ZapRect.bottom=m_TitleArea.bottom; m_ZapRect.left=CenterOfZapStatus-WidthOfTitle*0.5; m_ZapRect.right=CenterOfZapStatus+WidthOfTitle*0.5;
	if(*M_isZapperLearning==1){
		m_DC[m_DisplayedStimLayer].SetBkColor(RGB(255, 90, 0));
	}
	FontInRect(&m_DC[m_DisplayedStimLayer],sZapStatus,m_ZapRect);
	objDC.BitBlt(BIT_BLT(m_ZapRect,&m_DC[m_DisplayedStimLayer]));
	m_DC[m_DisplayedStimLayer].SetBkColor(RGB(255, 255, 255));
	m_DC[m_DisplayedStimLayer].SelectObject(OldPen);
	m_DC[m_DisplayedStimLayer].SelectObject(OldBrush);
}

void CAntsView::Disp_LearnedLevel(int FigID)
{
#define HeightWidthRatio 0.4
	CClientDC objDC(this);
	m_DC[m_DisplayedStimLayer].SetBkColor(RGB(220, 220, 220));
	CPen *OldPen=objDC.SelectObject(&m_NULL_pen); m_DC[m_DisplayedStimLayer].SelectObject(&m_NULL_pen);
	CFigure *pFig;

	//Color bar legend
	#define RECT_WIDTH 14
	CRect Rect;
	double step;
	int RectY, LearnedLevel;
	if(FigID>=0){
		pFig=&m_Fig[FigID][0][0];
		step=((pFig->m_dMaxY-pFig->m_dMinY)/(double)MaxRGB100);
		RectY= pFig->FromDataToScreenY(pFig->m_dMinY)-pFig->FromDataToScreenY(pFig->m_dMinY+step)+1;
		LearnedLevel=MaxRGB100*pFig->m_LearnedLevel;
		Rect.left=pFig->FromDataToScreenX(pFig->m_dMaxX)+1; Rect.top=pFig->FromDataToScreenY(pFig->m_dMaxY);	Rect.right=Rect.left+RECT_WIDTH;	Rect.bottom=pFig->FromDataToScreenY(pFig->m_dMinY);
		//First delete the previous one
		m_DC[m_DisplayedStimLayer].SelectStockObject(WHITE_BRUSH);
		m_DC[m_DisplayedStimLayer].Rectangle(Rect);
		//Draw a new one
		for(int i=0;i<LearnedLevel;i++){ 
			pFig->FillSolidRect(pFig->m_dMaxX*1.0, pFig->m_dMinY+(i+0.5)*step,RECT_WIDTH,RectY,m_RGB100[i],&m_DC[m_DisplayedStimLayer]);
		}
		objDC.BitBlt(BIT_BLT(Rect,&m_DC[m_DisplayedStimLayer]));
	}
	else{
		int row,clm,lyr, i;
		CFigure *pFig;
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			for(row=0;row<m_dHowManyRows[lyr];row++){
				for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
					pFig=&m_Fig[row][clm][lyr];
					if((FigID==DRAW_ONLY_SELECTED) && (pFig->m_Captured!=1||pFig->m_Deleted==1))continue;
					step=((pFig->m_dMaxY-pFig->m_dMinY)/(double)MaxRGB100);
					RectY= pFig->FromDataToScreenY(pFig->m_dMinY)-pFig->FromDataToScreenY(pFig->m_dMinY+step)+1;
					LearnedLevel=MaxRGB100*pFig->m_LearnedLevel;
					Rect.left=pFig->FromDataToScreenX(pFig->m_dMaxX)+1; Rect.top=pFig->FromDataToScreenY(pFig->m_dMaxY); Rect.right=Rect.left+RECT_WIDTH;	Rect.bottom=pFig->FromDataToScreenY(pFig->m_dMinY);
					//First delete the previous one
					m_DC[m_DisplayedStimLayer].SelectStockObject(WHITE_BRUSH);
					m_DC[m_DisplayedStimLayer].Rectangle(Rect);
					//Draw a new one
					for(i=0;i<LearnedLevel;i++){ 
						pFig->FillSolidRect(pFig->m_dMaxX*1.0, pFig->m_dMinY+(i+0.5)*step,RECT_WIDTH,RectY,m_RGB100[i],&m_DC[m_DisplayedStimLayer]);
					}	
					objDC.BitBlt(BIT_BLT(Rect,&m_DC[m_DisplayedStimLayer]));
				}
			}
		}
	}
	m_DC[m_DisplayedStimLayer].SetBkColor(RGB(255, 255, 255));
	m_DC[m_DisplayedStimLayer].SelectObject(OldPen);
}