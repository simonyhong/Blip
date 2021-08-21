#include "stdafx.h"
#include "MOXY.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"
#include "SinTable.h"

#include <time.h>


void CMOXY_View::positionBox_OnOff(int WindID, int Switch)//On and off of the window
{
	RequestDisplayEvent(DISP_POSITION_BOX,WindID, Switch); 
}

//DISPLAY ON/OFF of the Eye Window
void CMOXY_View::Turn_PositionBox_OnOff(int WindID, int Switch)//On and off of the window
{
	CClientDC objDC(this);
	CDC *mpDC=&m_DC[0];
	int nSaveDC=mpDC->SaveDC();

	if(Switch){//ON
		//Redraw the new one
		mpDC->SelectObject(&m_PositionWindPens[WindID]);
		objDC.SelectObject(&m_PositionWindPens[WindID]);
		if(m_XY_DisplayMode==CARTESIAN_MODE){
			m_pXYFig->ContourRect(M_PositionWinds[WindID].LeftBoundary, M_PositionWinds[WindID].UpperBoundary, M_PositionWinds[WindID].RightBoundary, M_PositionWinds[WindID].LowerBoundary, mpDC);
			m_pXYFig->ContourRect(M_PositionWinds[WindID].LeftBoundary, M_PositionWinds[WindID].UpperBoundary, M_PositionWinds[WindID].RightBoundary, M_PositionWinds[WindID].LowerBoundary, &objDC);
		}
		else{
			m_pXYFig->ContourRect_VisReplica(M_PositionWinds[WindID].LeftBoundary, M_PositionWinds[WindID].UpperBoundary, M_PositionWinds[WindID].RightBoundary, M_PositionWinds[WindID].LowerBoundary, mpDC);
			m_pXYFig->ContourRect_VisReplica(M_PositionWinds[WindID].LeftBoundary, M_PositionWinds[WindID].UpperBoundary, M_PositionWinds[WindID].RightBoundary, M_PositionWinds[WindID].LowerBoundary, &objDC);
		}
		//Save the position of the box
		M_PositionWinds[WindID].Old_LeftBoundary =M_PositionWinds[WindID].LeftBoundary; 
		M_PositionWinds[WindID].Old_UpperBoundary=M_PositionWinds[WindID].UpperBoundary; 
		M_PositionWinds[WindID].Old_RightBoundary=M_PositionWinds[WindID].RightBoundary; 
		M_PositionWinds[WindID].Old_LowerBoundary=M_PositionWinds[WindID].LowerBoundary; 
	}
	else{//OFF
		////First erase the previous rectangle
		mpDC->SelectObject(&m_backgroundScreenPen);
		objDC.SelectObject(&m_backgroundScreenPen);
		if(m_XY_DisplayMode==CARTESIAN_MODE){
			m_pXYFig->ContourRect(M_PositionWinds[WindID].Old_LeftBoundary, M_PositionWinds[WindID].Old_UpperBoundary, M_PositionWinds[WindID].Old_RightBoundary, M_PositionWinds[WindID].Old_LowerBoundary, mpDC);
			m_pXYFig->ContourRect(M_PositionWinds[WindID].Old_LeftBoundary, M_PositionWinds[WindID].Old_UpperBoundary, M_PositionWinds[WindID].Old_RightBoundary, M_PositionWinds[WindID].Old_LowerBoundary, &objDC);
		}
		else{
			m_pXYFig->ContourRect_VisReplica(M_PositionWinds[WindID].Old_LeftBoundary, M_PositionWinds[WindID].Old_UpperBoundary, M_PositionWinds[WindID].Old_RightBoundary, M_PositionWinds[WindID].Old_LowerBoundary, mpDC);
			m_pXYFig->ContourRect_VisReplica(M_PositionWinds[WindID].Old_LeftBoundary, M_PositionWinds[WindID].Old_UpperBoundary, M_PositionWinds[WindID].Old_RightBoundary, M_PositionWinds[WindID].Old_LowerBoundary, &objDC);
		}
	}
	mpDC->RestoreDC(nSaveDC); 
}

//Conceptually, a behavioral AI channel == FIRST_2D_BHVR_AI_CH_IDX+ BehavioralObj*2.  
//Dimentions of that behavior channel() are: (X, Y,...)==(FIRST_2D_BHVR_AI_CH_IDX+BehavioralObj*2+0, FIRST_2D_BHVR_AI_CH_IDX+BehavioralObj*2+1,...)
//Make sure that this is the same as the one in TAS
void CMOXY_View::Get_Latest_XYpos()
{
	int AiChanID;
	int MaxI=NUM_2D_BEHAVIOR_OBJS;if(MaxI>NUM_2D_BEHAVIOR_OBJS)MaxI=NUM_2D_BEHAVIOR_OBJS;
	for(int i=0; i<MaxI; i++){
		AiChanID=FIRST_2D_BHVR_AI_CH_IDX+i*2;
		if(((*M_JustClicked)&&(i==(*M_BehavioralObjectForXYmonitor))) ||*M_usableAiIdx[AiChanID]<0 ){//The user just clicked the screen. The cliking is valid for the rest of the trial.
			M_EyePosX[i]=M_EyeXsimulalted[i]; 
			M_EyePosY[i]=M_EyeYsimulalted[i];
		}
		else{
			M_EyeX_rawData[i]=(SCALE_XY_POS*(*(M_AI[AiChanID  ]+*M_usableAiIdx[AiChanID  ]))  +M_EyeX_Bias[i])*M_EyeX_Gain[i];
			M_EyeY_rawData[i]=(SCALE_XY_POS*(*(M_AI[AiChanID+1]+*M_usableAiIdx[AiChanID+1]))  +M_EyeY_Bias[i])*M_EyeY_Gain[i];
			M_EyePosX[i]=M_EyeX_rawData[i]; 
			M_EyePosY[i]=M_EyeY_rawData[i];
		}
	}
}

void CMOXY_View::DrawNew_ObjPositions(CDC *pDC)
{
#define BOX_IN_WIN(X,Y) (X >m_pXYFig->m_nPaneTx && X <m_pXYFig->m_nPaneBx-m_EyeSquareWidth && Y >m_pXYFig->m_nPaneTy && Y <m_pXYFig->m_nPaneBy-m_EyeSquareHeight)
	CPen *pOldPen=pDC->SelectObject(&m_NullPen);

	//Draw new ones
	int MaxI=NUM_2D_BEHAVIOR_OBJS;if(MaxI>NUM_2D_BEHAVIOR_OBJS)MaxI=NUM_2D_BEHAVIOR_OBJS;
	int TopleftX, TopleftY;
	for(int i=0; i<MaxI; i++){
		if(M_IsVisibleObj[i]==0)continue;
		//Erase the previous one
		if(BOX_IN_WIN(m_PrvTopleftX[i],m_PrvTopleftY[i])){
			pDC->BitBlt(m_PrvTopleftX[i],m_PrvTopleftY[i],m_EyeSquareWidth, m_EyeSquareHeight,&m_DC0Mrr,m_PrvTopleftX[i],m_PrvTopleftY[i],SRCCOPY);//Copies the source rectangle directly to the destination rectangle.
		}
		if(m_XY_DisplayMode==EYE_MONITOR_MODE)m_pXYFig->ReportBackTopLeftXY_Eye(M_EyePosX[i], M_EyePosY[i], m_EyeSquareWidth,m_EyeSquareHeight,TopleftX, TopleftY);
		else               /*CARTESIAN_MODE*/ m_pXYFig->ReportBackTopLeftXY(M_EyePosX[i], M_EyePosY[i], m_EyeSquareWidth,m_EyeSquareHeight,TopleftX, TopleftY);
		//Draw a new one
		if(BOX_IN_WIN(TopleftX, TopleftY)){
			if(i==0){
				pDC->FillSolidRect(TopleftX,TopleftY,m_EyeSquareWidth,m_EyeSquareHeight,	RGB(200,200,200));
			}
			else{
				pDC->FillSolidRect(TopleftX,TopleftY,m_EyeSquareWidth,m_EyeSquareHeight,	m_RGB100[(MaxRGB100-1)-(int)(Divide(i,(MaxI-1))*(MaxRGB100-15))]);
			}
			
			//This one just flips the what's already drawn
			//m_pXYFig->ReportBackTopLeftXY_Eye(M_EyePosX[i], M_EyePosY[i], m_EyeSquareWidth,m_EyeSquareHeight,m_PrvTopleftX, m_PrvTopleftY);
			//if(BOX_IN_WIN)
			//	pDC->BitBlt(m_PrvTopleftX,m_PrvTopleftY,m_EyeSquareWidth, m_EyeSquareHeight,&m_DC[0],m_PrvTopleftX,m_PrvTopleftY,DSTINVERT);//Inverts the destination rectangle (the source doesn't matter)
			m_PrvTopleftX[i]=TopleftX; m_PrvTopleftY[i]=TopleftY;
		}
		m_prvEyePosX[i]=M_EyePosX[i];m_prvEyePosY[i]=M_EyePosY[i];
	}
	pDC->SelectObject(pOldPen);
	pDC->SetROP2(R2_COPYPEN);	
}

void CMOXY_View::Init_XYwindow()
{
	VirMemDC();
	int i;
	double stepX,stepY;

	SetFontSize(m_pXYFig);
	//m_pXYFig->SetFontSize(&m_DC[0]);
	m_pXYFig->m_IsForVIS=1;
	m_pXYFig->m_normTx=m_savedClientRect.left;  m_pXYFig->m_normTy=0.2*(m_savedClientRect.bottom-m_savedClientRect.top);   
	m_pXYFig->m_normBx=m_Separator_inScreenCoord;  m_pXYFig->m_normBy=m_savedClientRect.bottom; 
	m_pXYFig->Set_MinMax_n_Labels_XY("","",-m_XY_Monitor_Scale,m_XY_Monitor_Scale,-m_XY_Monitor_Scale,m_XY_Monitor_Scale	,oFRAME);		
	*M_PosFigMinX=-m_XY_Monitor_Scale; *M_PosFigMaxX=m_XY_Monitor_Scale; *M_PosFigMinY=-m_XY_Monitor_Scale; *M_PosFigMaxY=m_XY_Monitor_Scale;		
	m_pXYFig->m_howManyX=3;m_pXYFig->m_howManyY=3;
	stepX=(m_pXYFig->m_dMaxX-m_pXYFig->m_dMinX)/(m_pXYFig->m_howManyX-1);
	for(i=0;i<m_pXYFig->m_howManyX+1;i++){
		if(i%1==0){m_pXYFig->m_IsLabeledTicX[i]=1;m_pXYFig->m_XTics[i]=m_pXYFig->m_dMinX+stepX*i;}
		else m_pXYFig->m_IsLabeledTicY[i]=0;//Do not label
	} 
	stepY=(m_pXYFig->m_dMaxY-m_pXYFig->m_dMinY)/(m_pXYFig->m_howManyY-1);
	for(i=0;i<m_pXYFig->m_howManyY+1;i++){
		if(i%1==0){m_pXYFig->m_IsLabeledTicY[i]=1;m_pXYFig->m_YTics[i]=m_pXYFig->m_dMinY+stepY*i;}
		else m_pXYFig->m_IsLabeledTicY[i]=0;//Do not label
	} 
	m_pXYFig->Prepare4FigFrame(&m_DC[0]);
	m_pXYFig->Prepare4VisReplica();
}

void CMOXY_View::Redraw_XYwindow()
{
	CClientDC objDC(this);
	CDC *mpDC=&m_DC[0];
	m_pXYFig->Frame2D_for_VIS_Replica(m_XY_DisplayMode, mpDC, this);
	//m_pXYFig->ListObjIDs(mpDC,m_SelectedVisualObjectID); //This line is for the user to select an object to be used to display when the user clicks the XY window with the right click to get attention from the monkey.
	Draw_IncreaseArrow(m_NnCounterArrowDrawn,m_pXYFig->m_FileName,0, mpDC);
	Draw_DecreaseArrow(m_NnCounterArrowDrawn,m_pXYFig->m_FileName,0, mpDC);
	m_pXYFig->DisplayLabel(m_pXYFig->m_FileName);
	Draw_IncreaseArrow(m_RWDCounterArrowDrawn,m_pXYFig->m_RwdAmount,0, mpDC);
	Draw_DecreaseArrow(m_RWDCounterArrowDrawn,m_pXYFig->m_RwdAmount,0, mpDC);
	m_pXYFig->DisplayLabel(m_pXYFig->m_RwdAmount);
	Draw_IncreaseArrow(m_BChanArrowDrawn,m_pXYFig->m_BehavioralObj,0, mpDC);
	Draw_DecreaseArrow(m_BChanArrowDrawn,m_pXYFig->m_BehavioralObj,0, mpDC);
	m_pXYFig->DisplayLabel(m_pXYFig->m_BehavioralObj);
	m_pXYFig->RefreshFrame(&objDC,mpDC);
}

