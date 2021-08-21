
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
#include "MainFrm.h"

void CAntsView::DisplayLFP_AroundStimulation(int StimChan, CDC *pObjDC)
{
	if(m_NumOfStimulationChannels>MAX_NUM_STIM_CHANNELS-1 || StimChan<DO_FIRST_GATING_CHAN || StimChan>DO_LAST_GATING_CHAN){
		AfxMessageBox("Wrong Channel number"); return;
	}
	int Distance, WaitTimeInMs, DataConfiguation;
	int row, clm, lyr, RecChan, StimChanIdx, i, idx,TriggerSig_AIidx;
	double *pArtifact, initX, *pM_AI;
	//Display the LFP around the stimulation point
	double miliSEcPerPoint=(*M_microSecPerPoint)*0.001, BiasX, Voltage, MinY,  MaxY;
	int initI, endI, endI_inclusive;
	CDC *pDC;
	CFigure *pFig;
	
	//Find the first layer that corresponds to the stimulation channel
	//The StimChan is already vetted and used for stimulation; so just find the first layer that contains a figure with a matching stimulation and vetted for display (pFig->m_IsToDisplay==1).    
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(StimChan==pFig->m_StimGatingRelay.m_ChanID && pFig->m_IsToDisplay==1){
					m_DisplayedStimLayer=lyr;
					goto EndOfLayerSearch;
	}	}	}	}	
	return;//There is no layer of figures containing the stimulation channel.
	EndOfLayerSearch:;

	//Display the layer
	Draw_Disp_Tabs_and_DispStimLayer(m_DisplayedStimLayer, 1, 0);
	//Draw on it
	CPen *pOldPen=m_DC[m_DisplayedStimLayer].SelectObject(&m_DkGREEN);
	pDC=&m_DC[m_DisplayedStimLayer];
	//Draw on all the figures with the already vetted recording channels that is associated with the current stimulation channel
	for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
		for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
			pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
			if(pFig->m_StimGatingRelay.m_ChanID!=StimChan || pFig->m_IsToDisplay!=1){pFig->RefreshFrame(pObjDC,pDC); continue;}
			if(pFig->m_NumLfpDrawn>=m_numberOfSuperimposeLines){pFig->Frame_Graph2D(pDC, 1); pFig->m_NumLfpDrawn=0;}//Clear superimpose history.
			RecChan=pFig->m_RecordingChan.m_ChanID;
			//Draw Measuring Lines
			DrawAllMeasuringLines_NoDisplayNow(pFig);
			//Determine where to align the stimulation point in the figure.
			MinY=pFig->m_dMinY,  MaxY=pFig->m_dMaxY;
			if(m_bIsTo_ANTI_Stimulate==1){
				TriggerSig_AIidx=M_EstimatedAntidromicStimPointIdx[RecChan]; 
				BiasX=pFig->m_MeasuringLineTime[0]; //The alignment of stimulation (at 0) is after the interval between the first measuring line and 0.
			}
			else{
				TriggerSig_AIidx=M_EstimatedOrthodromicStimPointIdx[RecChan];
				BiasX=0; //The alignment of stimulation (at 0) is right when the orthodromic request is made with no delay by the first measuring line.
			}
			initI=(-BiasX+pFig->m_dMinX)/miliSEcPerPoint+1, endI=(-BiasX+pFig->m_dMaxX)/miliSEcPerPoint;	if(initI>0){AfxMessageBox("initI needs to be smaller than 0"); continue;}
			pDC->SelectObject(&m_rgbPEN1[pFig->m_NumLfpDrawn*(int)Divide(6,m_numberOfSuperimposeLines)]);
			//pDC->SelectObject(&m_DkGREEN);
			pM_AI=M_AI[pFig->m_RecordingChan.m_ChanID];
			Voltage=pM_AI[qIdx(TriggerSig_AIidx+initI)]; if(Voltage>MaxY)Voltage=MaxY; if(Voltage<MinY)Voltage=MinY;
			pFig->MyMoveTo(pFig->m_dMinX,Voltage,pDC);
			initI=TriggerSig_AIidx+initI; endI_inclusive=TriggerSig_AIidx+endI-1;
			if(initI>=0 && (endI_inclusive>=0 && endI_inclusive<*pM_AIQueueSize))DataConfiguation=ContinuousArry;
			else DataConfiguation=SplitArry;
			initI=qIdx(initI); endI_inclusive=qIdx(endI_inclusive);
			//#########  Wait if necessary
			int Distance=ShortestDistanceFrom_A_to_B(*M_usableAiIdx[RecChan], endI_inclusive, *pM_AIQueueSize);
			if(Distance>0)	WaitTimeInMs=(Distance*(*M_microSecPerPoint))*0.001+1;
			else WaitTimeInMs=0;
			if(WaitTimeInMs>3*60000){AfxMessageBox("Wait more than 3 minutes?");}
			while(ShortestDistanceFrom_A_to_B(endI_inclusive, *M_usableAiIdx[RecChan], *pM_AIQueueSize)<0){Sleep(5); if(m_bIsON==0)return; }
			//#########  Draw the LFP around the stimulation point
			if(DataConfiguation==ContinuousArry){
				for(idx=initI;idx<=endI_inclusive;idx++){
					Voltage=pM_AI[idx]; if(Voltage>MaxY)Voltage=MaxY; if(Voltage<MinY)Voltage=MinY;
					pFig->MyLineTo((idx-TriggerSig_AIidx)*miliSEcPerPoint+BiasX,Voltage,pDC);
				}
			}
			//One portion of the data is at the begining, AND the other portion of the data is at the end of the AI Queue.
			else if(DataConfiguation==SplitArry){ 
				for(idx=initI;idx<*pM_AIQueueSize;idx++){
					Voltage=pM_AI[idx]; if(Voltage>MaxY)Voltage=MaxY; if(Voltage<MinY)Voltage=MinY;
					pFig->MyLineTo((idx-TriggerSig_AIidx)*miliSEcPerPoint+BiasX,Voltage,pDC);
				}
				for(idx=0;idx<=endI_inclusive;idx++){
					Voltage=pM_AI[idx]; if(Voltage>MaxY)Voltage=MaxY; if(Voltage<MinY)Voltage=MinY;
					pFig->MyLineTo((idx-TriggerSig_AIidx)*miliSEcPerPoint+BiasX,Voltage,pDC);
				}
			}
			pFig->m_NumLfpDrawn++;
	
			#ifdef ZAPPER
			Show the learned artifact
			pDC->SelectObject(&m_VIOLET);
			StimChanIdx=-1;
			for(i=0;i<MAX_NUM_STIM_CHANNELS;i++){
				if(M_Zapper[RecChan].M_StimRelayID[i]==pFig->m_StimGatingRelay.m_ChanID){
					StimChanIdx=i; goto EndOfStimIdSearch;
				} 
			}
			EndOfStimIdSearch:;
			initX=BiasX+(*M_StartAIidx_ofArtifact_4ANTS-TriggerSig_AIidx)*miliSEcPerPoint;
			if(StimChanIdx!=-1)pArtifact=M_Zapper[RecChan].M_Artifact[StimChanIdx];
			else{ AfxMessageBox("StimChanIdx==-1"); return;}
			initI=0;
			endI=((pFig->m_dMaxX)*1000)/(*M_microSecPerPoint); if(endI>ARTIFACT_SIZE)endI=ARTIFACT_SIZE;
			Voltage=pArtifact[qIdx(initI)]; if(Voltage>MaxY)Voltage=MaxY; if(Voltage<MinY)Voltage=MinY;
			pFig->MyMoveTo(initX,Voltage,pDC);
			for(i=initI+1;i<endI;i++){
				Voltage=pArtifact[i]; if(Voltage>MaxY)Voltage=MaxY; if(Voltage<MinY)Voltage=MinY;
				pFig->MyLineTo(i*miliSEcPerPoint+initX,Voltage,pDC);
			}
			pFig->Circle(initX,0,5,pDC);
			#endif

			pFig->RefreshFrame(pObjDC,pDC);
		}
	}
	m_DC[m_DisplayedStimLayer].SelectObject(pOldPen);
}

void CAntsView::DisplayStuff(int RequestIdx, CDC *pObjDC)
{	
	int i,row,clm,lyr;
	CFigure *pFig;
	char strTemp[100];
	int ScreenX;
	switch(m_DispEvents[RequestIdx][0]){
	case REDRAW_ALL_LAYERS:
		Re_measure_ClientView();
		Batch_FigPosition_Settings(m_dScaleUpDn);
		Batch_Prepare4FigFrame();
		//Display
		Batch_DrawFigureFrames(DONT_DISPLAY_NOW);
		BatchDraw_LayerContents(DONT_DISPLAY_NOW); //This one uses Shared Memories.
		#ifdef ZAPPER
		Disp_ZapStatus();
		Disp_LearnedLevel(-1);
		#endif
		m_numCaptured=0;
		pObjDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
		*M_StimulationLatencyBias_InMicroS=m_StimulationLatencyBias_InMicroS;
		break;
	case UNVEIL_CURRENT_PAGE:
		pObjDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
		m_numCaptured=0;
		break;
	case REDRAW_MEASURING_LINE:
		if(m_pSpotlightFig==NULL || m_HeldLineIdx<0)return;
		int StimChanDOIdx;
		CPen *pOldPen;
			/*if(m_DispEvents[RequestIdx][3]<500){
				Sleep(0);
			}*/
		m_DC[m_DisplayedStimLayer].SelectStockObject(WHITE_BRUSH);
		pOldPen=m_DC[m_DisplayedStimLayer].SelectObject(&m_NULL_pen);
		ScreenX=m_DispEvents[RequestIdx][2];
		if(m_HeldLineIdx==0 && (m_pSpotlightFig->FromScreenToDataY(m_DispEvents[RequestIdx][3])-m_pSpotlightFig->m_dMinY)/m_pSpotlightFig->m_dSpanY>0.8){ //The user grabs the upper portion of the first measuring bar
			for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
				if(m_pSpotlightFig->m_clm!=clm)continue;
				for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
					pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
					if(pFig->m_Deleted==0){//Find the m_HeldLineIdx that has the same index info of the stimulation channel as the one by the figure.
						Update_and_Draw_MeasuringLine(pFig, ScreenX);// Updates the value of m_MeasuringLineTime[m_HeldLineIdx]
						Draw_Text_OfMeasuringLine(pFig, m_HeldLineIdx, ScreenX);
		}	}	}	}
		else{
			Update_and_Draw_MeasuringLine(m_pSpotlightFig, ScreenX);// Updates the value of m_MeasuringLineTime[m_HeldLineIdx]
			Draw_Text_OfMeasuringLine(m_pSpotlightFig, m_HeldLineIdx,  ScreenX);
		}
		if(m_HeldLineIdx==0){//The measuring line that determines the latency between the detection of a spike and the delivery of a stimulation
			//Find all the Displaying figures (meaning already validated channel numbers) bearing the triggering recording channel.
			//  AND set their M_TriggerToStimulationLatency[] using their associated stimulating channels.
			Set_StimLatency_and_PostStimPeriod();
		}
		pObjDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
		break;
	case RENAMING_TITLE:
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			if(lyr==m_DisplayedStimLayer)DrawGlobalTitle(lyr,DISPLAY_NOW);
			else                         DrawGlobalTitle(lyr,DONT_DISPLAY_NOW);   
		}
		break;
		#ifdef Zapper
	case DISP_ZAP_STATUS:
		Disp_ZapStatus();
		break;
	case DISP_ZAP_LEVEL:
		Disp_LearnedLevel(m_DispEvents[RequestIdx][2]);
		break;
		#endif
	case REDRAW_SAVE_TAG:
		m_DC[m_DisplayedStimLayer].TextOut(m_pSpotlightFig->m_ClickToSave.StringRect.left,m_pSpotlightFig->m_ClickToSave.StringRect.top,m_pSpotlightFig->m_ClickToSave.NameString);
		m_pSpotlightFig->RefreshClickToSaveBox(pObjDC,&m_DC[m_DisplayedStimLayer]); 
		break;
	case REDRAW_RECORDING_STIMULATION_CHAN_IDS:
		for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
			for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
				pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
				pFig->Compute_Mapping_XY(); 
				pFig->Disp_Rec_Stim_Names(&m_DC[m_DisplayedStimLayer]); 
				pFig->RefreshNameBox(pObjDC,&m_DC[m_DisplayedStimLayer]); 
			}
		}		
		break;
	case REDRAW_FOCUS:
		for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
			for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
				pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
				m_DC[m_DisplayedStimLayer].TextOut(pFig->m_Focus.StringRect.left,pFig->m_Focus.StringRect.top,pFig->m_Focus.NameString);
				pObjDC->BitBlt(BIT_BLT(pFig->m_Focus.StringRect,&m_DC[m_DisplayedStimLayer])); 
			}
		}	
		break;
	case RESCALING_AXES:
		for(row=0;row<m_dHowManyRows[m_DisplayedStimLayer];row++){
			for(clm=0;clm<m_dHowManyClmns[m_DisplayedStimLayer];clm++){
				pFig=&m_Fig[row][clm][m_DisplayedStimLayer];
				if(pFig->m_Captured!=1||pFig->m_Deleted==1)continue;
				pFig->Compute_Mapping_XY(); 
				pFig->Frame_Graph2D(&m_DC[m_DisplayedStimLayer],1); 
				Hilight_OneCapturedFig(pFig);
				Recalibrate_Panes_and_MeasuringLineRectPositions();
		}	}	
		InitMeasuringLineParameters_and_DrawLines();
		pObjDC->BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
		break;
	case DISP_LFP_AROUND_STIM:
		DisplayLFP_AroundStimulation(m_DispEvents[RequestIdx][2], pObjDC);
		break;
	case REDRAW_FILE_NAME: 
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			if(lyr==m_DisplayedStimLayer)RedrawFileName(lyr,DISPLAY_NOW);
			else                         RedrawFileName(lyr,DONT_DISPLAY_NOW);   
		}
		break;
	case REDRAW_SUPERIMPOSE_NUMBER: 
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			if(lyr==m_DisplayedStimLayer)Draw_Disp_SuperimposeNumber(lyr,DISPLAY_NOW);
			else                         Draw_Disp_SuperimposeNumber(lyr,DONT_DISPLAY_NOW);   
		}
		break;
	case REDRAW_TRIGGER_CHANNEL_NUMBER: 
		for(lyr=0;lyr<m_dHowManyLayers;lyr++){
			if(lyr==m_DisplayedStimLayer)Draw_TriggerChanNumber(lyr,DISPLAY_NOW);
			else                         Draw_TriggerChanNumber(lyr,DONT_DISPLAY_NOW);   
		}
		break;

	default: break;
	}	
}

//This one:
// (1) updates the line position with a new one  
// (2) registers the value  
// (3) displays it
// m_HeldLineIdx==-1 case is blocked before this function
void CAntsView::Update_and_Draw_MeasuringLine(CFigure* pFig, int X) 
{
	CClientDC objDC(this);
	m_DC[m_DisplayedStimLayer].SetROP2(R2_NOTXORPEN);
	objDC.SetROP2(R2_NOTXORPEN);
	objDC.SelectObject(m_rgbPEN1[m_HeldLineIdx]);
	m_DC[m_DisplayedStimLayer].SelectObject(m_rgbPEN1[m_HeldLineIdx]);
	//XOR on the previous line
	objDC.MoveTo(pFig->m_MeasureingLine_ScrnCoord[m_HeldLineIdx],pFig->m_MeasuringTimeRect[0].bottom); objDC.LineTo(pFig->m_MeasureingLine_ScrnCoord[m_HeldLineIdx],pFig->m_nPaneBy);
	m_DC[m_DisplayedStimLayer].MoveTo(pFig->m_MeasureingLine_ScrnCoord[m_HeldLineIdx],pFig->m_MeasuringTimeRect[0].bottom); m_DC[m_DisplayedStimLayer].LineTo(pFig->m_MeasureingLine_ScrnCoord[m_HeldLineIdx],pFig->m_nPaneBy);
	//Draw a new line
	objDC.MoveTo(X,pFig->m_MeasuringTimeRect[0].bottom);objDC.LineTo(X,pFig->m_nPaneBy);
	m_DC[m_DisplayedStimLayer].MoveTo(X,pFig->m_MeasuringTimeRect[0].bottom);m_DC[m_DisplayedStimLayer].LineTo(X,pFig->m_nPaneBy);

	pFig->m_MeasureingLine_ScrnCoord[m_HeldLineIdx]=X;
	pFig->m_MeasuringLineTime[m_HeldLineIdx]=pFig->FromScreenToDataX(X);

	objDC.SetROP2(R2_COPYPEN);
	m_DC[m_DisplayedStimLayer].SetROP2(R2_COPYPEN);
}

//It draws the measuring line
// m_HeldLineIdx==-1 case is blocked before this function
void CAntsView::Draw_Text_OfMeasuringLine(CFigure* pFig, int LineID, int X) 
{
	char strTemp[100];
	CPen *pOldPen;
	m_DC[m_DisplayedStimLayer].SelectStockObject(WHITE_BRUSH);
	pOldPen=m_DC[m_DisplayedStimLayer].SelectObject(&m_NULL_pen);
	m_DC[m_DisplayedStimLayer].Rectangle(pFig->m_MeasuringTimeRect[LineID]);
	sprintf(strTemp,"%0.2lf  ", fabs(pFig->m_MeasuringLineTime[LineID]));
	pFig->m_MeasuringTimeRect[LineID].left=X; 
	pFig->m_MeasuringTimeRect[LineID].right=pFig->m_MeasuringTimeRect[LineID].left+pFig->m_FontWidth*strlen(strTemp); 

	m_DC[m_DisplayedStimLayer].TextOut(pFig->m_MeasuringTimeRect[LineID].left,pFig->m_MeasuringTimeRect[LineID].top,strTemp);
	m_DC[m_DisplayedStimLayer].SelectObject(pOldPen);
}

void CAntsView::Meassure_SuperimposeNumber() 
{ 
	m_Lable_Of_SuperimposeNumber.StringRect.left=0.25*(m_TitleArea.right);
	m_Lable_Of_SuperimposeNumber.StringRect.right=m_Lable_Of_SuperimposeNumber.StringRect.left+m_FontWidth*strlen(m_Lable_Of_SuperimposeNumber.NameString);
	m_Lable_Of_SuperimposeNumber.StringRect.top=(int)(m_TitleArea.top+m_FontHeight*.2);
	m_Lable_Of_SuperimposeNumber.StringRect.bottom=(int)(m_Lable_Of_SuperimposeNumber.StringRect.top+m_FontHeight);
}

void CAntsView::Meassure_TriggerChanNumber() 
{ 
	m_Lable_Of_TriggerChanNumber.StringRect.left=0.65*(m_TitleArea.right);
	m_Lable_Of_TriggerChanNumber.StringRect.right=m_Lable_Of_TriggerChanNumber.StringRect.left+m_FontWidth*strlen(m_Lable_Of_TriggerChanNumber.NameString);
	m_Lable_Of_TriggerChanNumber.StringRect.top=(int)(m_TitleArea.top+m_FontHeight*.2);
	m_Lable_Of_TriggerChanNumber.StringRect.bottom=(int)(m_Lable_Of_TriggerChanNumber.StringRect.top+m_FontHeight);
}

void CAntsView::Draw_Disp_SuperimposeNumber(int layer, int IstoDisplayNow)
{
	m_DC[layer].TextOut(m_Lable_Of_SuperimposeNumber.StringRect.left, m_Lable_Of_SuperimposeNumber.StringRect.top, m_Lable_Of_SuperimposeNumber.NameString);

	if(IstoDisplayNow==1){
		CClientDC objDC(this);
		int Width=m_Lable_Of_SuperimposeNumber.StringRect.right-m_Lable_Of_SuperimposeNumber.StringRect.left;
		int Height=m_Lable_Of_SuperimposeNumber.StringRect.bottom-m_Lable_Of_SuperimposeNumber.StringRect.top;
		objDC.BitBlt(m_Lable_Of_SuperimposeNumber.StringRect.left,m_Lable_Of_SuperimposeNumber.StringRect.top,Width,Height,&m_DC[layer],m_Lable_Of_SuperimposeNumber.StringRect.left,m_Lable_Of_SuperimposeNumber.StringRect.top,SRCCOPY);
	}
}

void CAntsView::Draw_TriggerChanNumber(int layer, int IstoDisplayNow)
{
	if(m_isOrthodromicStimulation==1){
		CPen *pOldPen=m_DC[m_DisplayedStimLayer].SelectObject(&m_NULL_pen); 
		m_DC[layer].Rectangle(m_Lable_Of_TriggerChanNumber.StringRect);
		m_DC[m_DisplayedStimLayer].SelectObject(pOldPen);
	}
	else m_DC[layer].TextOut(m_Lable_Of_TriggerChanNumber.StringRect.left, m_Lable_Of_TriggerChanNumber.StringRect.top, m_Lable_Of_TriggerChanNumber.NameString);

	if(IstoDisplayNow==1){
		CClientDC objDC(this);
		int Width=m_Lable_Of_TriggerChanNumber.StringRect.right-m_Lable_Of_TriggerChanNumber.StringRect.left;
		int Height=m_Lable_Of_TriggerChanNumber.StringRect.bottom-m_Lable_Of_TriggerChanNumber.StringRect.top;
		objDC.BitBlt(m_Lable_Of_TriggerChanNumber.StringRect.left,m_Lable_Of_TriggerChanNumber.StringRect.top,Width,Height,&m_DC[layer],m_Lable_Of_TriggerChanNumber.StringRect.left,m_Lable_Of_TriggerChanNumber.StringRect.top,SRCCOPY);
	}
}


void CAntsView::DrawGlobalTitle(int layer, int IstoDisplayNow)
{
#define HeightWidthRatio 0.4
	m_DC[layer].SetBkColor(RGB(220, 220, 220));

	int CenterOfTitle= (m_TitleArea.left+m_TitleArea.right)*0.5;
	int length=m_GlobalTitle.GetLength(); 
	if(length==0){AfxMessageBox("Check if you have done m_GlobalTitle="" somewhere."); exit(0);}//Sometimes multi-thread causes problems.
	CSize size=m_DC[layer].GetOutputTextExtent(m_GlobalTitle);
	int WidthOfTitle=size.cx+1;	
	CClientDC objDC(this);
	CPen *OldPen=objDC.SelectObject(&m_NULL_pen); m_DC[layer].SelectObject(&m_NULL_pen);
	m_DC[layer].SelectObject(m_bkBrush);objDC.SelectObject(m_bkBrush);
	{m_DC[layer].Rectangle(m_dTitleRect.left,m_dTitleRect.top,m_dTitleRect.right,m_dTitleRect.bottom); objDC.Rectangle(m_dTitleRect.left,m_dTitleRect.top,m_dTitleRect.right,m_dTitleRect.bottom); }
	m_DoNotErase=0;
	m_dTitleRect.top=0+0.1*m_topOfFigs; m_dTitleRect.bottom=m_TitleArea.bottom; m_dTitleRect.left=CenterOfTitle-WidthOfTitle*0.5; m_dTitleRect.right=CenterOfTitle+WidthOfTitle*0.5;
	FontInRect(&m_DC[layer],m_GlobalTitle,m_dTitleRect);
	if(IstoDisplayNow==1){
		objDC.BitBlt(BIT_BLT(m_dTitleRect,&m_DC[layer]));
	}
	m_DC[layer].SetBkColor(RGB(255, 255, 255));
	m_DC[layer].SelectObject(OldPen);
}


void CAntsView::RedrawFileName(int layer, int IstoDisplayNow) 
{	
	CClientDC objDC(this);
	m_DC[layer].SetBkColor(RGB(220, 220, 220));
	m_DC[layer].TextOut( m_FileName.StringRect.left,m_FileName.StringRect.top, m_FileName.NameString);
	m_DC[layer].SetBkColor(RGB(255, 255, 255));
	CBrush *pOldBrush;
	if(m_is_LFP_saving==0){
		pOldBrush=m_DC[layer].SelectObject(&m_GrayBrush); 
	}
	else if(m_is_LFP_saving==1){
		pOldBrush=m_DC[layer].SelectObject(&m_RedBrush); 
	}
	m_DC[layer].Ellipse(m_FileName.StringRect.left-m_FontWidth-m_FontHeight*0.5, m_FileName.StringRect.top+m_FontHeight*0.5-m_FontHeight*0.5,m_FileName.StringRect.left-m_FontWidth+m_FontHeight*0.5, m_FileName.StringRect.top+m_FontHeight*0.5+m_FontHeight*0.5);
	m_DC[layer].SelectObject(pOldBrush); 
	m_DC[m_DisplayedStimLayer].SelectObject(&m_NULL_pen);
	//objDC.BitBlt(m_FileName.StringRect.left,m_FileName.StringRect.top,m_FileName.StringRect.left,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
	if(IstoDisplayNow==1){
		objDC.BitBlt(BIT_BLT(m_TitleArea,&m_DC[layer]));
	}
}

void CAntsView::Draw_Tabs(int Layer, int IstoDisplayNow)
{
	//Draw Tabs
	for(int lyr=0;lyr<MAX_NUM_STIM_CHANNELS;lyr++){ 
		if(lyr==Layer){//Current lyr ==> Highlight it!
			m_DC[Layer].SetBkColor(RGB(100,200,250));
		}
		else{//Hidden lyr
			m_DC[Layer].SetBkColor(RGB(70,120,190));
		}
		m_DC[Layer].TextOut(m_LayerTab[lyr].StringRect.left, m_LayerTab[lyr].StringRect.top, m_LayerTab[lyr].NameString);
	}

	//Unveil the tabs row.
	if(IstoDisplayNow==1){
		CClientDC objDC(this);
		int Width=m_LayerTab[MAX_NUM_STIM_CHANNELS-1].StringRect.right-m_LayerTab[0].StringRect.left;
		int Height=m_LayerTab[0].StringRect.bottom-m_LayerTab[0].StringRect.top;
		objDC.BitBlt(m_LayerTab[0].StringRect.left,m_LayerTab[0].StringRect.top,Width,Height,&m_DC[Layer],m_LayerTab[0].StringRect.left,m_LayerTab[0].StringRect.top,SRCCOPY);
	}
	m_DC[Layer].SetBkColor(RGB(255,255,255));	//Restore the default color
}

void CAntsView::RectOfLabel_inTitleArea(float relX, int shiftToRightBy, int LengthOfLabel, CRect &rect)
{	
	rect.left   =(m_TitleArea.right-m_TitleArea.left)*relX  +m_TitleArea.left+shiftToRightBy;
	rect.right  =rect.left+m_FontWidth*LengthOfLabel;
	rect.top    =(int)(m_TitleArea.top/*+m_FontHeight*0.5*/+1); 
	rect.bottom = rect.top+m_FontHeight;
}

void CAntsView::RectOfLayerTabs(float left, int LengthOfLabel, CRect &rect)
{	
	rect.left   =left;
	rect.right  =rect.left+m_FontWidth*LengthOfLabel;
	rect.top    =(int)(m_topOfFigs-m_FontHeight*1); 
	rect.bottom = m_topOfFigs;
}

void CAntsView::DrawAllMeasuringLines_NoDisplayNow(CFigure *pFig)
{	
	pFig->DrawMeasuringLines(m_rgbPEN1);
}


void CAntsView::Batch_FigPosition_Settings(double scale) 
{
#define F2FY 5
#define F2FX 5
	int row,clm,lyr;
	CFigure *pFig;

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				pFig->m_normTx=scale*((clm+1)*F2FX+    clm*(m_dSizeOfOneClmn[lyr]-F2FX));  pFig->m_normTy=scale*((row+1)*F2FY+    row*(m_dSizeOfOneRow[lyr]-F2FY)+m_topOfFigs);   
				pFig->m_normBx=scale*((clm+1)*F2FX+(clm+1)*(m_dSizeOfOneClmn[lyr]-F2FX));  pFig->m_normBy=scale*((row+1)*F2FY+(row+1)*(m_dSizeOfOneRow[lyr]-F2FY)+m_topOfFigs); 
				pFig-> m_LateralMargin=20;
				pFig->SetFig_Position_in_ClientView(pFig->m_normTx,pFig->m_normTy,pFig->m_normBx,pFig->m_normBy);

				pFig->m_BlownUpTx=200; pFig->m_BlownUpTy=5; pFig->m_BlownUpBx=820; pFig->m_BlownUpBy=605; pFig->m_LateralMargin=20;
			}
		}	
	}
}

void CAntsView::Batch_SetFontSize(CDC* pDC) 
{
	int row,clm,lyr;
	CFigure *pFig;

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				pFig->SetFontSize(pDC);
			}
		}	
	}
}

void CAntsView::Batch_Prepare4FigFrame() 
{
	int row,clm,lyr;
	CFigure *pFig;

	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				pFig->Compute_Mapping_XY();
			}
		}	
	}
}

void CAntsView::Batch_DrawFigureFrames(int IsToDisplayNow) 
{	
	m_DoNotErase=1;//This is to inform the Labeling part not to erase previous site, which may not correspond to new position due to resize.
	int row,clm,lyr;
	CFigure *pFig;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		m_DC[lyr].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY); 
		for(row=0;row<m_dHowManyRows[lyr];row++){
			for(clm=0;clm<m_dHowManyClmns[lyr];clm++){
				pFig=&m_Fig[row][clm][lyr];
				if(pFig->m_Deleted==0)pFig->Frame_Graph2D(&m_DC[lyr],1); 
			}
		}
	}
	for(lyr=m_dHowManyLayers;lyr<MaxFigLyr;lyr++){
		m_DC[lyr].Rectangle(0,0,m_nScrMaxX,m_nScrMaxY); 
	}
	if(IsToDisplayNow==1){
		CClientDC objDC(this);
		objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
	}
}

void CAntsView::BatchDraw_LayerContents(int IsToDisplayNow) 
{	
	CClientDC objDC(this);
	int lyr;
	for(lyr=0;lyr<m_dHowManyLayers;lyr++){
		Draw_Disp_SuperimposeNumber(lyr, DONT_DISPLAY_NOW);;
		Draw_TriggerChanNumber(lyr, DONT_DISPLAY_NOW);;
		Minimum_RequiredTime_toTrigger(lyr, DONT_DISPLAY_NOW);
		DrawGlobalTitle(lyr, DONT_DISPLAY_NOW);
		Draw_Tabs(lyr, DONT_DISPLAY_NOW); //Every layer has a full set of page numbers displayed
		InitMeasuringLineParameters_and_DrawLines();
		RedrawFileName(lyr, DONT_DISPLAY_NOW);
		//objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[lyr],0,0,SRCCOPY);
	}
	if(IsToDisplayNow==1){
		objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[m_DisplayedStimLayer],0,0,SRCCOPY);
	}
}


// Writes a string sText with a size which fits perfectly in a rectangle rFont
// Font already attached with device context pDC is scaled
// This does not work for default font of the device contex
void CAntsView::FontInRect(CDC *pDC, CString sText, CRect &rFont)
{
	// Gets current font details
	CFont* pFont = pDC->GetCurrentFont();
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);

	// Scales the font to fit in the rectangle
	int a=pDC->GetDeviceCaps(LOGPIXELSY);
	LogFont.lfHeight = -72 * rFont.Height() / pDC->GetDeviceCaps(LOGPIXELSY);
	LogFont.lfWidth = -72 * rFont.Width() / pDC->GetDeviceCaps(LOGPIXELSX) / sText.GetLength();

	// Create a font of calculated size
	CFont Font;
	Font.CreateFontIndirect(&LogFont);

	// Rechecks the string width and height
	pFont = pDC->SelectObject(&Font);
	CSize Size = pDC->GetTextExtent(sText);
	pDC->SelectObject(pFont);
	Font.DeleteObject();

	// Fine tunes the font width and height
	LogFont.lfWidth = (int) (LogFont.lfWidth * rFont.Width() / Size.cx);
	LogFont.lfHeight = (int) (LogFont.lfHeight * rFont.Height() / Size.cy);

	// Create a font with accurate size and displays string
	Font.CreateFontIndirect(&LogFont);
	pFont = pDC->SelectObject(&Font);
	pDC->DrawText(sText, rFont, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pDC->SelectObject(pFont);
	Font.DeleteObject();
}

