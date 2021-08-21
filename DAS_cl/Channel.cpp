#include "stdafx.h"
#include "VarDefinitions_SIG.h"
#include "Figure_DAS.h"
#include "Channel.h"
#include "DAS_View.h"
#include "Channel.h"
#include <math.h>

CDC	CAiChannel::m_ChDC;
CBitmap CAiChannel::m_ChBmp;
CPen CAiChannel::m_VIOLETLIGHT2,CAiChannel::m_GREEN2,CAiChannel::m_CYAN2;
int	CAiChannel::m_DiviceContaxtCreatedAlready1=0, CAiChannel::m_DiviceContaxtCreatedAlready2=0;


CAiChannel::CAiChannel()
{
	m_IsThisChanUsed=0;//The default assumption: not used
	gain=10;
	m_idx_SpikeNn1=0; m_idx_SpikeNn2=0; m_idx_SpikeNn3=0;
	m_SIGi=0;//m_SIGi points the next array index to process
	m_IsThresholdHeld=0;
	m_numMatchingKinds=0;
	m_HighToLowCrossing=0;
	m_selectedBoxID=-1;
	for(int i=0;i<MAX_NNs_PerCHAN;i++){
		m_numSelRects[i]=0; m_numSelRectsTmp[i]=0; 
		m_numRejRects[i]=0; m_numRejRectsTmp[i]=0;
	}
	if(m_DiviceContaxtCreatedAlready1==0){
		m_DiviceContaxtCreatedAlready1=1;
		m_VIOLETLIGHT2.CreatePen(PS_SOLID, 2,RGB(205,5,255));
		m_GREEN2.CreatePen(PS_SOLID, 2,RGB(0,255,0));
		m_CYAN2.CreatePen(PS_SOLID, 2,RGB(0,255,255));
	}
	m_RtClickSelectedBoxID=-1;
	m_prvPoint.x=-1;
}

CAiChannel::~CAiChannel()
{
	m_isVT_Selected.RemoveAll();
	m_SelectedWaveFormsToDisp.RemoveAll();
	m_ChDC.DeleteDC();
	m_ChBmp.DeleteObject();
	m_VIOLETLIGHT2.DeleteObject();
	m_GREEN2.DeleteObject();
	m_CYAN2.DeleteObject();
}

//When the infomation of the "client's" VT box changes, it updates on the server side
void CAiChannel::UpdateServerWithVTBoxes()
{
	int Nn, Rct;

	pView->M_numMatchingKinds[m_ChanID]=m_numMatchingKinds;
	for(Nn=0;Nn<MAX_NNs_PerCHAN;Nn++){
		pView->M_numSelRects[m_ChanID][Nn]=m_numSelRects[Nn];
		for(Rct=0;Rct<m_numSelRects[Nn];Rct++){
			pView->M_SelRectBdryLtIdx[m_ChanID][Nn][Rct]=			m_SelRectBdryLtIdx[Nn][Rct]			;
			pView->M_SelRectBdryRtIdx[m_ChanID][Nn][Rct]=			m_SelRectBdryRtIdx[Nn][Rct]			;//Left and right boundaries of selection Boxes - in "data index" coordinate (intger form)
			pView->M_SelRectBdryLftX_IdxInDbl[m_ChanID][Nn][Rct]=	m_SelRectBdryLftX_IdxInDbl[Nn][Rct]	;
			pView->M_SelRectBdryRhtX_IdxInDbl[m_ChanID][Nn][Rct]=	m_SelRectBdryRhtX_IdxInDbl[Nn][Rct]	;//Left and right boundaries of selection Boxes - in "data index" coordinate (in fraction form)
			pView->M_SelRectBdryTopYDataCoord[m_ChanID][Nn][Rct]=	m_SelRectBdryTopYDataCoord[Nn][Rct]	;
			pView->M_SelRectBdryBtmYDataCoord[m_ChanID][Nn][Rct]=	m_SelRectBdryBtmYDataCoord[Nn][Rct]	; //Selection Boxes - in "data" coordinate
		}
		pView->M_numRejRects[m_ChanID][Nn]=m_numRejRects[Nn];
		for(Rct=0;Rct<m_numRejRects[Nn];Rct++){
			pView->M_RejRectBdryLtIdx[m_ChanID][Nn][Rct]=			m_RejRectBdryLtIdx[Nn][Rct]			;
			pView->M_RejRectBdryRtIdx[m_ChanID][Nn][Rct]=			m_RejRectBdryRtIdx[Nn][Rct]			;//Left and right boundaries of rejection Boxes - in "data index" coordinate (intger form)
			pView->M_RejRectBdryLftX_IdxInDbl[m_ChanID][Nn][Rct]=	m_RejRectBdryLftX_IdxInDbl[Nn][Rct]	;
			pView->M_RejRectBdryRhtX_IdxInDbl[m_ChanID][Nn][Rct]=	m_RejRectBdryRhtX_IdxInDbl[Nn][Rct]	;//Left and right boundaries of rejection Boxes - in "data index" coordinate (in fraction form)
			pView->M_RejRectBdryTopYDataCoord[m_ChanID][Nn][Rct]=	m_RejRectBdryTopYDataCoord[Nn][Rct]			;
			pView->M_RejRectBdryBtmYDataCoord[m_ChanID][Nn][Rct]=	m_RejRectBdryBtmYDataCoord[Nn][Rct]			; //Rejection Boxes - in "data" coordinate
		}
	}
	pView->M_IsFromHighToLow[m_ChanID]=m_HighToLowCrossing;
	pView->M_WaveSpanPre_ms[m_ChanID]=fabs(pFig->m_dMinX);
	pView->M_WaveSpanPost_ms[m_ChanID]=fabs(pFig->m_dMaxX);

	pView->M_ChanBoxInfoChanged[m_ChanID]=1;
	*pView->M_HeyDASsv_SomeChanBoxInfoChanged=1;
	*pView->M_HeyANTS_SomeChanBoxInfoChanged=1;
}


void CAiChannel::ShowAll_VTBoxes()
{
	int i,colorType,rectLeft,rectTop,rectRight,rectBottom;
	m_ChDC.SelectObject(m_VIOLETLIGHT2);
	CRect *pRect;
	for(colorType=0;colorType<MAX_NNs_PerCHAN;colorType++){//Newly added one is on top
		if(colorType==0){m_ChDC.SelectObject(m_VIOLETLIGHT2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
		else if(colorType==1){m_ChDC.SelectObject(m_GREEN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
		else {m_ChDC.SelectObject(m_CYAN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}

		for(i=0;i<m_numSelRects[colorType];i++){
			//Show the rectangle
			pRect=&m_SelRectsOnScrn[colorType][i];
			rectLeft=pRect->left; rectTop=pRect->top; rectRight=pRect->right; rectBottom=pRect->bottom;
			m_ChDC.MoveTo(rectLeft,rectTop); m_ChDC.LineTo(rectRight,rectTop); m_ChDC.LineTo(rectRight,rectBottom); m_ChDC.LineTo(rectLeft,rectBottom); m_ChDC.LineTo(rectLeft,rectTop);
			//objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
		}
		for(i=0;i<m_numRejRects[colorType];i++){
			//Show the rectangle
			pRect=&m_RejRectsOnScrn[colorType][i];
			rectLeft=pRect->left; rectTop=pRect->top; rectRight=pRect->right; rectBottom=pRect->bottom;
			m_ChDC.MoveTo(rectLeft,rectTop); m_ChDC.LineTo(rectRight,rectTop); m_ChDC.LineTo(rectRight,rectBottom); m_ChDC.LineTo(rectLeft,rectBottom); m_ChDC.LineTo(rectLeft,rectTop);
			//objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
			//Diagonal line
			//objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectBottom); 
			m_ChDC.MoveTo(rectLeft,rectTop); m_ChDC.LineTo(rectRight,rectBottom);
		}
	}
}

void CAiChannel::RedrawDiscriminatorRect(CPoint &point, int colorType, int isToAccept, int BoxID)
{
	if(m_prvPoint.x<0){//the mouse has moved for the first time 
		m_prvPoint=point;
		return;
	}
	if(m_prvPoint==point)return;//the mouse has NOT been moved.

	CClientDC objDC(pView);
	int rectLeft,rectTop,rectRight,rectBottom;
	int DifferenceX=point.x-m_prvPoint.x;
	int DifferenceY=point.y-m_prvPoint.y;

	//m_DC[0].SelectObject(m_VIOLETLIGHT2);
	//m_DC[0].SetROP2(R2_NOTXORPEN);
	if(colorType==0){objDC.SelectObject(m_VIOLETLIGHT2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else if(colorType==1){objDC.SelectObject(m_GREEN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else if(colorType==2){objDC.SelectObject(m_CYAN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else {
		AfxMessageBox("pcolorType=? 3");
	}
	//objDC.SetROP2(R2_NOTXORPEN);
	CRect *pRect;
	if(isToAccept==1){pRect=&m_SelRectsOnScrn[colorType][BoxID];} else pRect=&m_RejRectsOnScrn[colorType][BoxID];

	////Erase the previous one
	rectLeft=pRect->left; rectTop=pRect->top; rectRight=pRect->right; rectBottom=pRect->bottom;
	////m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectTop); m_DC[0].LineTo(rectRight,rectBottom); m_DC[0].LineTo(rectLeft,rectBottom); m_DC[0].LineTo(rectLeft,rectTop);
	//objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
	//if(isToAccept==0){//Diagonal line for the rejecting box
	//	objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectBottom); //m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectBottom);//Diagonal line
	//}
	//Redraw a new one
	rectLeft=rectLeft+DifferenceX; rectTop=rectTop+DifferenceY; rectRight=rectRight+DifferenceX; rectBottom=rectBottom+DifferenceY;
	if(rectLeft<pFig->m_nPaneTx)rectLeft=pFig->m_nPaneTx; if(rectTop<pFig->m_nPaneTy)rectTop=pFig->m_nPaneTy; if(rectRight>pFig->m_nPaneBx)rectRight=pFig->m_nPaneBx; if(rectBottom>pFig->m_nPaneBy)rectBottom=pFig->m_nPaneBy;
	if(rectLeft>=rectRight||rectTop>=rectBottom)return;
	//m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectTop); m_DC[0].LineTo(rectRight,rectBottom); m_DC[0].LineTo(rectLeft,rectBottom); m_DC[0].LineTo(rectLeft,rectTop);
	objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
	if(isToAccept==0){//Diagonal line for the rejecting box
		objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectBottom); //m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectBottom);//Diagonal line
	}
	pRect->left=rectLeft; pRect->top=rectTop; pRect->right=rectRight; pRect->bottom=rectBottom;

	//Update
	m_prvPoint=point;
	RegisterSelBox(colorType, isToAccept,BoxID);
}

//Rubber-bending of the box edge
void CAiChannel::Redraw_BoxEdge(CPoint &point, int colorType, int isToAccept, int BoxID, int &edgeMoveType)
{
	if(m_prvPoint.x<0){//the mouse has moved for the first time 
		m_prvPoint=point;
		return;
	}
	if(m_prvPoint==point)return;//the mouse has NOT been moved.

	CRect *pRect;
	if(isToAccept==1){pRect=&m_SelRectsOnScrn[colorType][BoxID];} else pRect=&m_RejRectsOnScrn[colorType][BoxID];

	//Erase the previous one
	CClientDC objDC(pView);
	//m_DC[0].SelectObject(m_VIOLETLIGHT2);
	if(colorType==0){objDC.SelectObject(m_VIOLETLIGHT2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else if(colorType==1){objDC.SelectObject(m_GREEN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else if(colorType==2){objDC.SelectObject(m_CYAN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else 	AfxMessageBox("colorType=? 2");
	//objDC.SetROP2(R2_NOTXORPEN);
	////m_DC[0].SetROP2(R2_NOTXORPEN);
	int rectLeft,rectTop,rectRight,rectBottom;
	rectLeft=pRect->left; rectTop=pRect->top; rectRight=pRect->right; rectBottom=pRect->bottom;
	////m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectTop); m_DC[0].LineTo(rectRight,rectBottom); m_DC[0].LineTo(rectLeft,rectBottom); m_DC[0].LineTo(rectLeft,rectTop);
	//objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
	//if(isToAccept==0){//Diagonal line
	//	objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectBottom); //m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectBottom);
	//}

	int DifferenceX=point.x-m_prvPoint.x;
	int DifferenceY=point.y-m_prvPoint.y;
	switch(edgeMoveType){
		case LEFT_EDGE:
			rectLeft=rectLeft+DifferenceX; 
			break;
		case TOP_EDGE:
			rectTop=rectTop+DifferenceY; 
			break;
		case RIGHT_EDGE:
			rectRight=rectRight+DifferenceX; 
			break;
		case BOTTOM_EDGE:
			rectBottom=rectBottom+DifferenceY;
			break;
		case LEFT_TOP_CORNER:
			rectLeft=rectLeft+DifferenceX; rectTop=rectTop+DifferenceY;
			break;
		case RIGHT_TOP_CORNER:
			rectTop=rectTop+DifferenceY; rectRight=rectRight+DifferenceX; 
			break;
		case RIGHT_BOTTOM_CORNER:
			rectRight=rectRight+DifferenceX; rectBottom=rectBottom+DifferenceY;
			break;
		case LEFT_BOTTOM_CORNER:
			rectLeft=rectLeft+DifferenceX; rectBottom=rectBottom+DifferenceY;
			break;
		default:AfxMessageBox("Strange rubber bending mode.");
			break;
	}
	if(rectLeft<pFig->m_nPaneTx)rectLeft=pFig->m_nPaneTx; if(rectTop<pFig->m_nPaneTy)rectTop=pFig->m_nPaneTy; if(rectRight>pFig->m_nPaneBx)rectRight=pFig->m_nPaneBx; if(rectBottom>pFig->m_nPaneBy)rectBottom=pFig->m_nPaneBy;
	if(rectLeft>=rectRight||rectTop>=rectBottom)return;
	//m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectTop); m_DC[0].LineTo(rectRight,rectBottom); m_DC[0].LineTo(rectLeft,rectBottom); m_DC[0].LineTo(rectLeft,rectTop);
	objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
	if(isToAccept==0){//Diagonal line
		objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectBottom); //m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectBottom);
	}

	//Update
	if(rectLeft<=rectRight){
		pRect->left=rectLeft; 
		pRect->right=rectRight; 
	}
	else{
		pRect->left=rectRight; 
		pRect->right=rectLeft; 
	}
	if(rectTop<=rectBottom){
		pRect->top=rectTop; 
		pRect->bottom=rectBottom;
	}
	else{
		pRect->top=rectBottom; 
		pRect->bottom=rectTop;
	}
	m_prvPoint=point;
	RegisterSelBox(colorType, isToAccept, BoxID);
}

void CAiChannel::RegisterSelBox(int colorType, int isToAccept, int BoxID) 
{
	if(BoxID>=MAX_VT_RECTs_PER_NEURON){AfxMessageBox("BoxID>=MAX_VT_RECTs_PER_NEURON"); return;}
	double LeftOfBoxDataCoord, RghtOfBoxDataCoord,TopOfBoxDataCoord,BtmOfBoxDataCoord;//The box in time coordinate
	double LeftBndry_inFloatIdx, RghtBndry_inFloatIdx; //Data index BUT in FLOAT format. To be used used for the on-line line-crossing analysis =>This index-coordinate (instead of the time coordinate) is used to eiliminate the index-to-time converting process on-line.
	double relX_left,relX_rght; //The relative (0.~1.) positions of the left and right boundaries of the box.
	int beforeLeftBndry_idx,afterRightBndry_idx;//Data indexes to be  used for the on-line line-crossing analysis
	//CClientDC DC(pView);
	CRect *pRect;

	if(isToAccept==1){pRect=&m_SelRectsOnScrn[colorType][BoxID];} else pRect=&m_RejRectsOnScrn[colorType][BoxID];

	//The following maps SCREEN coordinate (x,y) to data coordinate (x',y') in the GRAPH.
	FromScrnToDataCoordinate(pRect->left, pRect->top,    LeftOfBoxDataCoord, TopOfBoxDataCoord);
	FromScrnToDataCoordinate(pRect->right,pRect->bottom, RghtOfBoxDataCoord, BtmOfBoxDataCoord);
	//Show the box
	//DC.SelectObject(m_CYAN2);
	//pFig->Graph2D((double)LeftOfBoxDataCoord,TopOfBoxDataCoord,(double)LeftOfBoxDataCoord,BtmOfBoxDataCoord,2,&DC);
	//pFig->Graph2D((double)RghtOfBoxDataCoord,TopOfBoxDataCoord,(double)RghtOfBoxDataCoord,BtmOfBoxDataCoord,2,&DC);

	relX_left=((double)(pRect->left -pFig->m_nPaneTx)/(double)(pFig->m_nPaneBx-pFig->m_nPaneTx)); //The relative (0 - 1) position of the left boundary of the box.
	relX_rght=((double)(pRect->right-pFig->m_nPaneTx)/(double)(pFig->m_nPaneBx-pFig->m_nPaneTx)); //The relative (0 - 1) position of the right boundary of the box.
	LeftBndry_inFloatIdx=(relX_left*(m_DataPointsPerWave-0));
	RghtBndry_inFloatIdx=(relX_rght*(m_DataPointsPerWave-0));
	beforeLeftBndry_idx=(int)(relX_left*m_DataPointsPerWave); if(beforeLeftBndry_idx<0)beforeLeftBndry_idx=0; if(beforeLeftBndry_idx>m_DataPointsPerWave-2)beforeLeftBndry_idx=m_DataPointsPerWave-2;
	afterRightBndry_idx=(int)(relX_rght*m_DataPointsPerWave); if(afterRightBndry_idx<1)afterRightBndry_idx=1; if(afterRightBndry_idx>m_DataPointsPerWave-1)afterRightBndry_idx=m_DataPointsPerWave-1;
	beforeLeftBndry_idx =SearchForDataIdx_closeButBeforeX(LeftBndry_inFloatIdx,beforeLeftBndry_idx);if(beforeLeftBndry_idx<0)beforeLeftBndry_idx=0; if(beforeLeftBndry_idx>m_DataPointsPerWave-2)beforeLeftBndry_idx=m_DataPointsPerWave-2;
	afterRightBndry_idx =SearchForDataIdx_closeButAfterX (RghtBndry_inFloatIdx,afterRightBndry_idx);if(afterRightBndry_idx<1)afterRightBndry_idx=1; if(afterRightBndry_idx>m_DataPointsPerWave-1)afterRightBndry_idx=m_DataPointsPerWave-1;
	//Show the range of X to search
	//DC.SelectObject(m_GREEN);
	//pFig->Graph2D(ToDataCoordSpkWV(beforeLeftBndry_idx),TopOfBoxDataCoord,ToDataCoordSpkWV(beforeLeftBndry_idx),BtmOfBoxDataCoord,2,&DC);
	//pFig->Graph2D(ToDataCoordSpkWV(afterRightBndry_idx),TopOfBoxDataCoord,ToDataCoordSpkWV(afterRightBndry_idx),BtmOfBoxDataCoord,2,&DC);
	
	if(isToAccept==1){//Selection
		//The relavent indexes (corresponding to before and after the box limits) of the wavelet index that need to be examined during the on-line line-crossing analysis.
		m_SelRectBdryLtIdx[colorType][BoxID]=beforeLeftBndry_idx;	m_SelRectBdryRtIdx[colorType][BoxID]=afterRightBndry_idx;

		//Data index BUT in DOUBLE format. To be used for the on-line line-crossing analysis.
		//=>This index-coordinate (instead of the time coordinate) is used to eiliminate the index-to-time converting process on-line.
		m_SelRectBdryLftX_IdxInDbl[colorType][BoxID]=LeftBndry_inFloatIdx; m_SelRectBdryRhtX_IdxInDbl[colorType][BoxID]=RghtBndry_inFloatIdx; 

		//Rectangle in data coordinate (to re-create "m_SelRectsOnScrn") 
		//Also "m_SelRectBdryTopYDataCoord[][]" and "m_SelRectBdryBtmYDataCoord" are used for the on-line line-crossing analysis.
		m_SelRectBdryLftXDataCoord[colorType][BoxID]=LeftOfBoxDataCoord;      m_SelRectBdryRhtXDataCoord[colorType][BoxID]=RghtOfBoxDataCoord;
		m_SelRectBdryTopYDataCoord[colorType][BoxID]=TopOfBoxDataCoord;       m_SelRectBdryBtmYDataCoord[colorType][BoxID]=BtmOfBoxDataCoord;

		if(BoxID==m_numSelRects[colorType]){//a new rectangle has been added
			if(m_numSelRects[colorType]==0&&m_numRejRects[colorType]==0)m_numMatchingKinds++;
			m_numSelRects[colorType]++;//Update this when every step of preparing the new box is ready so that the other threads use the new box without trouble.
		}
	}
	else{//Rejection
		m_RejRectBdryLtIdx[colorType][BoxID]=beforeLeftBndry_idx; m_RejRectBdryRtIdx[colorType][BoxID]=afterRightBndry_idx;

		m_RejRectBdryLftX_IdxInDbl[colorType][BoxID]=LeftBndry_inFloatIdx; m_RejRectBdryRhtX_IdxInDbl[colorType][BoxID]=RghtBndry_inFloatIdx; 

		m_RejRectBdryLftXDataCoord[colorType][BoxID]=LeftOfBoxDataCoord;      m_RejRectBdryRhtXDataCoord[colorType][BoxID]=RghtOfBoxDataCoord;
		m_RejRectBdryTopYDataCoord[colorType][BoxID]=TopOfBoxDataCoord;       m_RejRectBdryBtmYDataCoord[colorType][BoxID]=BtmOfBoxDataCoord;

		if(BoxID==m_numRejRects[colorType]){//a new rectangle has been added
			if(m_numSelRects[colorType]==0&&m_numRejRects[colorType]==0)m_numMatchingKinds++;
			m_numRejRects[colorType]++;//Update this when every step of preparing the new box is ready so that the other threads use the new box without trouble.
		}
	}
	//When the infomation of the "client's" VT box changes, it updates on the server side
	UpdateServerWithVTBoxes();
}

void CAiChannel::ReRegisterSelBoxes_andNoticeServer() 
{	
	int NnID,i;
	for(NnID=0;NnID<MAX_NNs_PerCHAN;NnID++){
		if(m_numSelRects[NnID]>0){
			for(i=0;i<m_numSelRects[NnID];i++){
				RegisterSelBox(NnID,1,i);
			}
		}
		if(m_numRejRects[NnID]>0){
			for(i=0;i<m_numRejRects[NnID];i++){
				RegisterSelBox(NnID,0,i);
			}
		}
	}
}

//This maps a screen point (x,y) to a data point (i,j) on the selected graph.
void CAiChannel::FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY) 
{
	int paneTopX, paneTopY,paneBottomX, paneBottomY;
	pFig->FromDataToScreenXY(pFig->m_dMinX,pFig->m_dMaxY,paneTopX,paneTopY);
	pFig->FromDataToScreenXY(pFig->m_dMaxX,pFig->m_dMinY,paneBottomX,paneBottomY);
	DataX=(pFig->m_dMaxX-pFig->m_dMinX)*(((double)(x   -paneTopX))/(paneBottomX-paneTopX))+pFig->m_dMinX;	
	DataY=(pFig->m_dMaxY-pFig->m_dMinY)*(((double)(paneBottomY-y))/(paneBottomY-paneTopY))+pFig->m_dMinY;
}

int CAiChannel::SearchForDataIdx_closeButBeforeX(float X, int candidateIdx)
{
	if(candidateIdx-1<X){
		if(candidateIdx<X){
			if(candidateIdx+1<X){
				return (candidateIdx+1);
			}
			else return (candidateIdx);
		}
		else return (candidateIdx-1);
	}
	else return (candidateIdx-2);
}

int CAiChannel::SearchForDataIdx_closeButAfterX(float X, int candidateIdx)
{
	if(candidateIdx+1>X){
		if(candidateIdx>X){
			if(candidateIdx-1>X){
				return (candidateIdx-1);
			}
			else return (candidateIdx);
		}
		else return (candidateIdx+1);
	}
	else return (candidateIdx+2);
}

//Newly added one is on top
int CAiChannel::WhichBox(CPoint &point,int &colorType,int &isToAccept,int &edgeMoveType)
{
#define EdgeHalfThickness 2 //The allowed precision of mouse position around the box.
	int i,clrType,leftIn,rightIn,topIn,bottomIn;
	edgeMoveType=-1;//Initialize
	CRect *pRect;
	for(clrType=0;clrType<MAX_NNs_PerCHAN;clrType++){//Newly added one is on top
		for(i=m_numSelRects[clrType]-1;i>=0;i--){//Newly added one is on top
			pRect=&m_SelRectsOnScrn[clrType][i];
			if(point.x>=pRect->left-EdgeHalfThickness && point.x<=pRect->right+EdgeHalfThickness){
				if(point.y>=pRect->top-EdgeHalfThickness && point.y<=pRect->bottom+EdgeHalfThickness){
					//Edge test
					leftIn=0;rightIn=0;topIn=0;bottomIn=0;
					if(point.x<=pRect->left+EdgeHalfThickness){
						leftIn=1;	edgeMoveType=LEFT_EDGE;
					}
					if(point.x>=pRect->right-EdgeHalfThickness){
						rightIn=1;	edgeMoveType=RIGHT_EDGE;
					}
					if(point.y<=pRect->top+EdgeHalfThickness){
						topIn=1;	edgeMoveType=TOP_EDGE;
					}
					if(point.y>=pRect->bottom-EdgeHalfThickness){
						bottomIn=1;	edgeMoveType=BOTTOM_EDGE;
					}
					if(leftIn&&topIn)edgeMoveType=LEFT_TOP_CORNER;
					else if(rightIn&&topIn)edgeMoveType=RIGHT_TOP_CORNER;
					else if(rightIn&&bottomIn)edgeMoveType=RIGHT_BOTTOM_CORNER;
					else if(leftIn&&bottomIn)edgeMoveType=LEFT_BOTTOM_CORNER;
					colorType=clrType; isToAccept=1;
					return i;
				}
			}
		}
		for(i=m_numRejRects[clrType]-1;i>=0;i--){//Newly added one is on top
			pRect=&m_RejRectsOnScrn[clrType][i];
			if(point.x>=pRect->left-EdgeHalfThickness && point.x<=pRect->right+EdgeHalfThickness){
				if(point.y>=pRect->top-EdgeHalfThickness && point.y<=pRect->bottom+EdgeHalfThickness){
					//Edge test
					leftIn=0;rightIn=0;topIn=0;bottomIn=0;
					if(point.x<=pRect->left+EdgeHalfThickness){
						leftIn=1;	edgeMoveType=LEFT_EDGE;
					}
					if(point.x>=pRect->right-EdgeHalfThickness){
						rightIn=1;	edgeMoveType=RIGHT_EDGE;
					}
					if(point.y<=pRect->top+EdgeHalfThickness){
						topIn=1;	edgeMoveType=TOP_EDGE;
					}
					if(point.y>=pRect->bottom-EdgeHalfThickness){
						bottomIn=1;	edgeMoveType=BOTTOM_EDGE;
					}
					if(leftIn&&topIn)edgeMoveType=LEFT_TOP_CORNER;
					else if(rightIn&&topIn)edgeMoveType=RIGHT_TOP_CORNER;
					else if(rightIn&&bottomIn)edgeMoveType=RIGHT_BOTTOM_CORNER;
					else if(leftIn&&bottomIn)edgeMoveType=LEFT_BOTTOM_CORNER;
					colorType=clrType; isToAccept=0;
					return i;
				}
			}
		}
	}
	return -1;
}

void CAiChannel::DrawOneVTbox(int colorType, int isToAccept, int BoxID, int isToHighlight)
{
	CClientDC objDC(pView);
	if(isToHighlight){
		objDC.SelectObject(pView->m_ORANGE2);//m_DC[0].SelectObject(m_ORANGE2);
	}
	else if(colorType==0){objDC.SelectObject(m_VIOLETLIGHT2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else if(colorType==1){objDC.SelectObject(m_GREEN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else if(colorType==2){objDC.SelectObject(m_CYAN2);}//m_DC[0].SelectObject(m_VIOLETLIGHT2);}
	else 	AfxMessageBox("colorType=? 3");

	int rectLeft,rectTop,rectRight,rectBottom;
	CRect *pRect;
	if(isToAccept==1){pRect=&m_SelRectsOnScrn[colorType][BoxID];} else pRect=&m_RejRectsOnScrn[colorType][BoxID];

	rectLeft=pRect->left; rectTop=pRect->top; rectRight=pRect->right; rectBottom=pRect->bottom;
	//m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectTop); m_DC[0].LineTo(rectRight,rectBottom); m_DC[0].LineTo(rectLeft,rectBottom); m_DC[0].LineTo(rectLeft,rectTop);
	objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectTop); objDC.LineTo(rectRight,rectBottom); objDC.LineTo(rectLeft,rectBottom); objDC.LineTo(rectLeft,rectTop);
	if(isToAccept==0){//Diagonal line
		objDC.MoveTo(rectLeft,rectTop); objDC.LineTo(rectRight,rectBottom); //m_DC[0].MoveTo(rectLeft,rectTop); m_DC[0].LineTo(rectRight,rectBottom);
	}
}


void CAiChannel::DeleteOneVTBox(int colorType, int isToAccept, int BoxID)
{
	int i,j;
	if(isToAccept==1){//Accepting Box
		for(i=BoxID;i<m_numSelRects[colorType]-1;i++){
			m_SelRectBdryLtIdx[colorType][i]=m_SelRectBdryLtIdx[colorType][i+1];m_SelRectBdryRtIdx[colorType][i]=m_SelRectBdryRtIdx[colorType][i+1];
			m_SelRectBdryLftX_IdxInDbl[colorType][i]=m_SelRectBdryLftX_IdxInDbl[colorType][i+1]; m_SelRectBdryRhtX_IdxInDbl[colorType][i]=m_SelRectBdryRhtX_IdxInDbl[colorType][i+1]; 
			m_SelRectBdryLftXDataCoord[colorType][i]=m_SelRectBdryLftXDataCoord[colorType][i+1]; m_SelRectBdryRhtXDataCoord[colorType][i]=m_SelRectBdryRhtXDataCoord[colorType][i+1];
			m_SelRectBdryTopYDataCoord[colorType][i]=m_SelRectBdryTopYDataCoord[colorType][i+1]; m_SelRectBdryBtmYDataCoord[colorType][i]=m_SelRectBdryBtmYDataCoord[colorType][i+1];
			m_SelRectsOnScrn[colorType][i]=m_SelRectsOnScrn[colorType][i+1];
		}
		m_numSelRects[colorType]--; m_numSelRectsTmp[colorType]--;
	}
	else{//Rejecting Box
		for(i=BoxID;i<m_numRejRects[colorType]-1;i++){
			m_RejRectBdryLtIdx[colorType][i]=m_RejRectBdryLtIdx[colorType][i+1];m_RejRectBdryRtIdx[colorType][i]=m_RejRectBdryRtIdx[colorType][i+1];
			m_RejRectBdryLftX_IdxInDbl[colorType][i]=m_RejRectBdryLftX_IdxInDbl[colorType][i+1]; m_RejRectBdryRhtX_IdxInDbl[colorType][i]=m_RejRectBdryRhtX_IdxInDbl[colorType][i+1]; 
			m_RejRectBdryLftXDataCoord[colorType][i]=m_RejRectBdryLftXDataCoord[colorType][i+1]; m_RejRectBdryRhtXDataCoord[colorType][i]=m_RejRectBdryRhtXDataCoord[colorType][i+1];
			m_RejRectBdryTopYDataCoord[colorType][i]=m_RejRectBdryTopYDataCoord[colorType][i+1]; m_RejRectBdryBtmYDataCoord[colorType][i]=m_RejRectBdryBtmYDataCoord[colorType][i+1];
			m_RejRectsOnScrn[colorType][i]=m_RejRectsOnScrn[colorType][i+1];
		}
		m_numRejRects[colorType]--; m_numRejRectsTmp[colorType]--;
	}
	if(m_numSelRects[colorType]==0&&m_numRejRects[colorType]==0)m_numMatchingKinds--;
	
	//When the infomation of the "client's" VT box changes, it updates on the server side
	UpdateServerWithVTBoxes();
}

void CAiChannel::Remeasure_Box_Positions_DataAnchored()
{	
	if(pFig==NULL)return;
	CRect *pRectScrn;
	int i, colorType,left,top,right,bottom;
	for(colorType=0;colorType<MAX_NNs_PerCHAN;colorType++){
		for(i=0;i<m_numSelRects[colorType];i++){
			pRectScrn=&m_SelRectsOnScrn[colorType][i];
			pFig->FromDataToScreenXY((double)m_SelRectBdryLftXDataCoord[colorType][i], (double)m_SelRectBdryTopYDataCoord[colorType][i],left,top);
			pFig->FromDataToScreenXY((double)m_SelRectBdryRhtXDataCoord[colorType][i], (double)m_SelRectBdryBtmYDataCoord[colorType][i],right,bottom);
			pRectScrn->left=left; pRectScrn->top=top; pRectScrn->right=right; pRectScrn->bottom=bottom;
		}
		for(i=0;i<m_numRejRects[colorType];i++){
			pRectScrn=&m_RejRectsOnScrn[colorType][i];
			pFig->FromDataToScreenXY((double)m_RejRectBdryLftXDataCoord[colorType][i], (double)m_RejRectBdryTopYDataCoord[colorType][i],left,top);
			pFig->FromDataToScreenXY((double)m_RejRectBdryRhtXDataCoord[colorType][i], (double)m_RejRectBdryBtmYDataCoord[colorType][i],right,bottom);
			pRectScrn->left=left; pRectScrn->top=top; pRectScrn->right=right; pRectScrn->bottom=bottom;
		}
	}
}

//Fixing the position of the box on the screen, recalculate the Y data points of the box rectangle.
void CAiChannel::Remeasure_Box_YPositions_ScreenAnchored()
{	
	if(pFig==NULL)return;
	int i, colorType;
	for(colorType=0;colorType<MAX_NNs_PerCHAN;colorType++){
		for(i=0;i<m_numSelRects[colorType];i++){
			m_SelRectBdryTopYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_SelRectsOnScrn[colorType][i].top);
			m_SelRectBdryBtmYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_SelRectsOnScrn[colorType][i].bottom);
		}
		for(i=0;i<m_numRejRects[colorType];i++){
			m_RejRectBdryTopYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_RejRectsOnScrn[colorType][i].top);
			m_RejRectBdryBtmYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_RejRectsOnScrn[colorType][i].bottom);
		}
	}
}

//Fixing the position of the box on the screen, recalculate the data points of the box rectangle.
void CAiChannel::Remeasure_Box_XY_Positions_ScreenAnchored()
{	
	if(pFig==NULL)return;
	int i, colorType;
	for(colorType=0;colorType<MAX_NNs_PerCHAN;colorType++){
		for(i=0;i<m_numSelRects[colorType];i++){
			m_SelRectBdryTopYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_SelRectsOnScrn[colorType][i].top);
			m_SelRectBdryBtmYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_SelRectsOnScrn[colorType][i].bottom);
			m_SelRectBdryLftXDataCoord[colorType][i]=pFig->FromScreenToDataY(m_SelRectsOnScrn[colorType][i].left);
			m_SelRectBdryRhtXDataCoord[colorType][i]=pFig->FromScreenToDataY(m_SelRectsOnScrn[colorType][i].right);
		}
		for(i=0;i<m_numRejRects[colorType];i++){
			m_RejRectBdryTopYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_RejRectsOnScrn[colorType][i].top);
			m_RejRectBdryBtmYDataCoord[colorType][i]=pFig->FromScreenToDataY(m_RejRectsOnScrn[colorType][i].bottom);
			m_RejRectBdryLftXDataCoord[colorType][i]=pFig->FromScreenToDataY(m_RejRectsOnScrn[colorType][i].left);
			m_RejRectBdryRhtXDataCoord[colorType][i]=pFig->FromScreenToDataY(m_RejRectsOnScrn[colorType][i].right);
		}
	}
}


