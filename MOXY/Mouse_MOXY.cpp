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

#include "VarDefinitions.h"

void CMOXY_View::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_isMOXY_Running==0)return; //When the program has not started, don't do anything.
	if(m_IsVertSeparatorHeld==1)m_IsVertSeparatorHeld=0;
	if(m_IsHorizSeparatorHeld==1)m_IsHorizSeparatorHeld=0;
	if(m_UserClickedDIOIdx_ON==1){Relay_OFF_n_DisplayLED(m_UserClickedDIOIdx); m_UserClickedDIOIdx_ON=0;}
	if(m_NnCounterArrowDrawn!=0){
		if(m_NnCounterArrowDrawn==-1)RequestDisplayEvent(DECREASE_NEURON_NUM,0);
		else if(m_NnCounterArrowDrawn==1)RequestDisplayEvent(INCREASE_NEURON_NUM,0);
	}
	if(m_RWDCounterArrowDrawn!=0){
		if(m_RWDCounterArrowDrawn==-1)RequestDisplayEvent(DECREASE_RWD,0);
		else if(m_RWDCounterArrowDrawn==1)RequestDisplayEvent(INCREASE_RWD,0);
	}
	if(m_BChanArrowDrawn!=0){
		if(m_BChanArrowDrawn==-1)RequestDisplayEvent(DECREASE_B_CHAN,0);
		else if(m_BChanArrowDrawn==1)RequestDisplayEvent(INCREASE_B_CHAN,0);
	}

	CView::OnLButtonUp(nFlags, point);
}

void CMOXY_View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_isMOXY_Running==0)return; //When the program has not started, don't do anything.
	m_isInsidePane=0;
	m_selPaneID=WhichPane(point);
	if(m_selPaneID<0)return;//Hit outside the pane
	
	//The following maps a screen point (x,y) to a relative point (i,j) of the graph.
	if((nFlags & MK_CONTROL)==0 && (m_selPaneID==XY_MONITOR) && (m_isInsidePane==1)){
		FromScrnToDataCoordinate(point.x, point.y, M_EyeXsimulalted[*M_BehavioralObjectForXYmonitor], M_EyeYsimulalted[*M_BehavioralObjectForXYmonitor]);
		(*M_JustClicked)=1; //This is to simulate the eye XY position with a mouse click
	}
	//The following determines the bias of the EYE (or other equivalient) position
	if((nFlags & MK_CONTROL) && m_selPaneID==XY_MONITOR && m_isInsidePane==1){
	
#ifndef ENABLE_THIS_FOR_DISTRIBUTION_TO_EXCLUDE_CUSTOM_CODE
		//The following prevents the user from accidently assigning an infinite value of the eye position (which happens when the monkey closes the eye in Eyelink system) to become the value that the user specified with the mouse click.
		if(-4>(*(M_AI[FIRST_2D_BHVR_AI_CH_IDX  ]+*M_usableAiIdx[FIRST_2D_BHVR_AI_CH_IDX  ])) ||
		   -4>(*(M_AI[FIRST_2D_BHVR_AI_CH_IDX+1]+*M_usableAiIdx[FIRST_2D_BHVR_AI_CH_IDX+1]))  )goto AA;
#endif
			
		double DesiredX, DesiredY;
		FromScrnToDataCoordinate(point.x, point.y, DesiredX, DesiredY);
		M_EyeX_Bias[*M_BehavioralObjectForXYmonitor]+=(DesiredX-M_EyePosX[*M_BehavioralObjectForXYmonitor])/M_EyeX_Gain[*M_BehavioralObjectForXYmonitor];
		M_EyeY_Bias[*M_BehavioralObjectForXYmonitor]+=(DesiredY-M_EyePosY[*M_BehavioralObjectForXYmonitor])/M_EyeY_Gain[*M_BehavioralObjectForXYmonitor];
		Save_MOXY_Config();
	}
#ifndef ENABLE_THIS_FOR_DISTRIBUTION_TO_EXCLUDE_CUSTOM_CODE
	AA:;
#endif

	CView::OnLButtonDown(nFlags, point);
}

//Due to a reason (start of a program, resize of a window...) screen positions of the left, right, up, down edges of all panes need to be assigned.
void CMOXY_View::RecalibratePanes() 
{
	int j;
	CFigure *pFig;

	for(j=0;j<MAX_GRAPHS;j++){
		pFig=&m_Fig[j];
		pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative data to a point on the screen.	
		pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative data to a point on the screen.
	}
}
#define TOL_STEP 3
void CMOXY_View::OnMouseMove(UINT nFlags, CPoint point)
{
	if(nFlags & MK_LBUTTON){
		if(m_isMOXY_Running==0)return; //When the program has not started, don't do anything.
		if(m_isRunningLineSDF==0)return;
//		if(!m_IsVertSeparatorHeld)WhichPane(point);
//		if(m_pSpotlightFig==NULL)return;//Hit outside the pane

		//CClientDC objDC(this);

		//If the curser hits the "separator" line,
		if(m_IsVertSeparatorHeld==1 ||abs(m_Separator_inScreenCoord-point.x)<m_RunPanel[0].m_pRLineFig->m_FontWidth){
			m_IsVertSeparatorHeld=1;
			//objDC.SetROP2(R2_NOTXORPEN);
			//objDC.SelectObject(m_BLUE3);
			
			//XOR on the previous line
			//objDC.MoveTo(m_Separator_inScreenCoord,m_pRunLineFig->m_nPaneTy);objDC.LineTo(m_Separator_inScreenCoord,m_pRunLineFig->m_nPaneBy);
			//Draw a new line
			//objDC.MoveTo(point.x,m_pRunLineFig->m_nPaneTy);objDC.LineTo(point.x,m_pRunLineFig->m_nPaneBy);
			m_Separator_inScreenCoord=point.x;
			ReCalibrateAllFigures();
			RequestDisplayEvent(REDRAW_ALL_FIGURES);
			//objDC.SetROP2(R2_COPYPEN);
		}
		else if(m_IsHorizSeparatorHeld==1 ||(abs(m_HorizontalSeparator_inScreenCoord-point.y)<m_RunPanel[0].m_pRLineFig->m_FontWidth && point.x> m_Separator_inScreenCoord)){
			m_IsHorizSeparatorHeld=1;
			m_VerticalRatioByDivider=m_HorizontalSeparator_inScreenCoord/(double)m_savedClientRect.bottom;
			m_HorizontalSeparator_inScreenCoord=point.y;
			ReCalibrateAllFigures();
			RequestDisplayEvent(REDRAW_ALL_FIGURES);
		}
	}
	CView::OnMouseMove(nFlags, point);
}


//This "WhichPane()" function changes the size of the pane and its position, when the "blow-up button" is pressed. 
int CMOXY_View::WhichPane(CPoint point) 
{
#define BNDRY_MRGN 20
	int found=-1,j;
	//CClientDC objDC(this);
	CFigure *pFig;

	//If the rectangle on screen (m_paneTopX, m_paneTopY, m_paneBottomX, m_paneBottomY) is not defined for the graph pane, define it!
	if(!m_bIsScrXYforIJ){ 
		for(j=0;j<MAX_GRAPHS;j++){
			pFig=&m_Fig[j];
			pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
			pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
		}
		m_bIsScrXYforIJ=1;
	}
	for(j=0;j<MAX_GRAPHS;j++){
		pFig=&m_Fig[j];
		if((point.x>=pFig->m_nTx && point.x<=pFig->m_nBx)&&(point.y<=pFig->m_nBy&&point.y>=pFig->m_nTy)){ 
			found=j; 
			if((point.x>=pFig->m_nPaneTx && point.x<=pFig->m_nPaneBx)&&(point.y<=pFig->m_nPaneBy&&point.y>=pFig->m_nPaneTy)){//Clicked somewhere inside of the PANE
				m_isInsidePane=1;
				m_minPaneX=pFig->m_dMinX; m_maxPaneX=pFig->m_dMaxX; m_minPaneY=pFig->m_dMinY; m_maxPaneY=pFig->m_dMaxY;
				m_paneTopX=(int)pFig->m_nPaneTx; m_paneTopY=(int)pFig->m_nPaneTy; m_paneBottomX=(int)pFig->m_nPaneBx; m_paneBottomY=(int)pFig->m_nPaneBy;
			}
			if(m_isRtBUTTON_pressed==1)return found;
			if(j>=FIRST_RUNNINGLINE_FIG){
				int monitorFigIdx;
				CRunningLineDisplay *pMonitorFig;
				monitorFigIdx=j-FIRST_RUNNINGLINE_FIG; if(monitorFigIdx<0||monitorFigIdx>=MAX_GRAPHS){AfxMessageBox("monitorFigIdx<0||monitorFigIdx>=MAX_GRAPHS"); return -1;}
				pMonitorFig=&m_RunPanel[monitorFigIdx];
				//Trigger Status 
				if(pMonitorFig->m_RL_FigID==0&& (point.x>=pFig->m_TriggerOptionStatus.StringRect.left && point.x<=pFig->m_TriggerOptionStatus.StringRect.right)&&(point.y>=pFig->m_TriggerOptionStatus.StringRect.top && point.y<=pFig->m_TriggerOptionStatus.StringRect.bottom)){
					EditTriggerECode(pMonitorFig);
					RequestDisplayEvent(TOGGLE_TRIGGER_STATUS,monitorFigIdx);
				}
				//Superimposition
				else if((point.x>=pFig->m_SuperImposeStatus.StringRect.left && point.x<=pFig->m_SuperImposeStatus.StringRect.right)&&(point.y>=pFig->m_SuperImposeStatus.StringRect.top && point.y<=pFig->m_SuperImposeStatus.StringRect.bottom)){
					pMonitorFig->m_IsToSuperImpose=!pMonitorFig->m_IsToSuperImpose;
					RequestDisplayEvent(TOGGLE_SUPERIMPOSITION_STATUS,monitorFigIdx);
				}
				//Recording Source (channel & neuron)
				else if(pMonitorFig->m_RL_FigID==0&& (point.x>=pFig->m_NnRecordingChan.StringRect.left && point.x<=pFig->m_NnRecordingChan.StringRect.right)&&(point.y>=pFig->m_NnRecordingChan.StringRect.top && point.y<=pFig->m_NnRecordingChan.StringRect.bottom)){
					m_callingFrom=0;
					AcceptOneString(pMonitorFig);
					m_callingFrom=1;
					AcceptOneString(pMonitorFig);
					Connect_ChannelVariables_ToMemoryMap(monitorFigIdx);
					m_isRunningLineSDF=0;                  //Simulate the sequence of 1)disableing the RL part,
					pMonitorFig->Init_RunningLineSDF_FIG();//and, 2) restarting all over again
					RequestDisplayEvent(DISP_RECORDING_SOURCE,monitorFigIdx);
				}
				//User-picked variable display
				int left,right,top,bottom;
				for(int i=0;i<*pMonitorFig->M_NumUserPickedVariables;i++){
					left=pMonitorFig->m_pRLineFig->m_VarToDispRECT[i].left;
					right=pMonitorFig->m_pRLineFig->m_VarToDispLineEnd[i];//The text + line
					top=pMonitorFig->m_pRLineFig->m_VarToDispRECT[i].top-pMonitorFig->m_pRLineFig->m_FontHeight; //One fontHeight higher than the variable itself.
					bottom=pMonitorFig->m_pRLineFig->m_VarToDispRECT[i].bottom;//To include the number itself.
					if((point.x>=left && point.x<=right)&&(point.y>=top && point.y<=bottom)){
						if(M_VarToDisp[i].M_ColorOrder+1==MAX_NUM_PENS_FOR_RUNNING_LINE){M_VarToDisp[i].M_ColorOrder=-1;}
						else{ M_VarToDisp[i].M_ColorOrder++; }
						pMonitorFig->m_UserPickedVar[i].m_ColorOrder=M_VarToDisp[i].M_ColorOrder;
						RequestDisplayEvent(DISP_LABELS_OF_USER_PICKED_VAR,monitorFigIdx);
					}
				}
				// Edit: X, Y axes
				//Axis-scale manipulators has been clicked?
				if(  ((point.x>=pFig->m_FirstYlabel_left && point.x<=pFig->m_FirstYlabel_right )&&(point.y>=pFig->m_FirstYlabel_top && point.y<=pFig->m_FirstYlabel_bottom ))
				   ||((point.x>=pFig->m_FirstXlabel_left && point.x<=pFig->m_FirstXlabel_right )&&(point.y>=pFig->m_FirstXlabel_top && point.y<=pFig->m_FirstXlabel_bottom ))
				   ||((point.x>=pFig->m_LastYlabel_left  && point.x<=pFig->m_LastYlabel_right  )&&(point.y>=pFig->m_LastYlabel_top  && point.y<=pFig->m_LastYlabel_bottom  ))
				   ||((point.x>=pFig->m_LastXlabel_left  && point.x<=pFig->m_LastXlabel_right  )&&(point.y>=pFig->m_LastXlabel_top  && point.y<=pFig->m_LastXlabel_bottom  ))
				   ||((point.x>=pFig->m_FirstY2label_left&& point.x<=pFig->m_FirstY2label_right)&&(point.y>=pFig->m_FirstY2label_top&& point.y<=pFig->m_FirstY2label_bottom))
				   ||((point.x>=pFig->m_LastY2label_left && point.x<=pFig->m_LastY2label_right )&&(point.y>=pFig->m_LastY2label_top && point.y<=pFig->m_LastY2label_bottom ))
				   ){
					OnAxisScale(pFig);
					pFig->Set_2ndMinMaxY(pFig->m_dMinY2, pFig->m_dMaxY2);
					m_RunPanel[monitorFigIdx].Init_RunningLineSDF_FIG();
					RequestDisplayEvent(REDRAW_RUNNINGLINE_FIG,monitorFigIdx);
				}
			}
			else if(j==XY_FIG){
				//File Name: Prefix & suffix ...
				if((point.x>=pFig->m_FileName.StringRect.left && point.x<=pFig->m_FileName.StringRect.right)&&(point.y>=pFig->m_FileName.StringRect.top && point.y<=pFig->m_FileName.StringRect.bottom)){
					EditPrefixSuffix();//This requests re-display of the name
				}
				//Grid
				else if((point.x>=pFig->m_GridNet.StringRect.left && point.x<=pFig->m_GridNet.StringRect.right)&&(point.y>=pFig->m_GridNet.StringRect.top && point.y<=pFig->m_GridNet.StringRect.bottom)){
					if(pFig->m_GridNet.status+1<MAX_EYE_GRID_MODES)pFig->m_GridNet.status++; else pFig->m_GridNet.status=0;
					RequestDisplayEvent(REDRAW_XY_WINDOW_FIG);
				}
				//Nn #
				else if((point.x>=pFig->m_FileName.ControlRect.left && point.x<=pFig->m_FileName.ControlRect.right)&&(point.y>=pFig->m_FileName.ControlRect.top && point.y<=pFig->m_FileName.ControlRect.bottom)){
					if(point.y<=0.5*(pFig->m_FileName.ControlRect.top+pFig->m_FileName.ControlRect.bottom)){
						(*M_NnNumberToRecord)++;
						RequestDisplayEvent(INCREASE_NEURON_NUM,1);
					}
					else{
						(*M_NnNumberToRecord)--;
						RequestDisplayEvent(DECREASE_NEURON_NUM,1);
					}
					Make_TheBaseOfFileName();
				}
				//RWD
				else if((point.x>=pFig->m_RwdAmount.ControlRect.left && point.x<=pFig->m_RwdAmount.ControlRect.right)&&(point.y>=pFig->m_RwdAmount.ControlRect.top && point.y<=pFig->m_RwdAmount.ControlRect.bottom)){
					if(point.y<=0.5*(pFig->m_RwdAmount.ControlRect.top+pFig->m_RwdAmount.ControlRect.bottom)){
						*M_Dur_BigRWD+=25;
						RemeasureRWDRect(*M_Dur_BigRWD);
						RequestDisplayEvent(INCREASE_RWD,1);
					}
					else if(*M_Dur_BigRWD>=25){
						 *M_Dur_BigRWD-=25;
						RemeasureRWDRect(*M_Dur_BigRWD);
						RequestDisplayEvent(DECREASE_RWD, 1);
					}
				}
				//Behavioral Object & its channels
				else if((point.x>=pFig->m_BehavioralObj.ControlRect.left && point.x<=pFig->m_BehavioralObj.ControlRect.right)&&(point.y>=pFig->m_BehavioralObj.ControlRect.top && point.y<=pFig->m_BehavioralObj.ControlRect.bottom)){
					if(point.y<=0.5*(pFig->m_BehavioralObj.ControlRect.top+pFig->m_BehavioralObj.ControlRect.bottom)){
						if(*M_BehavioralObjectForXYmonitor+1<NUM_2D_BEHAVIOR_OBJS){
							(*M_BehavioralObjectForXYmonitor)++;
							m_RunPanel[0].m_pEyeX=&M_EyePosX[*M_BehavioralObjectForXYmonitor]; m_RunPanel[0].m_pEyeY=&M_EyePosY[*M_BehavioralObjectForXYmonitor];
							Remeasure_BehavioralObj_Rect(*M_BehavioralObjectForXYmonitor);
							RequestDisplayEvent(INCREASE_B_CHAN,1);
						}
					}
					else{
						if(*M_BehavioralObjectForXYmonitor-1>=0){
							(*M_BehavioralObjectForXYmonitor)--;
							m_RunPanel[0].m_pEyeX=&M_EyePosX[*M_BehavioralObjectForXYmonitor]; m_RunPanel[0].m_pEyeY=&M_EyePosY[*M_BehavioralObjectForXYmonitor];
							Remeasure_BehavioralObj_Rect(*M_BehavioralObjectForXYmonitor);
							RequestDisplayEvent(DECREASE_B_CHAN,1);
						}
					}
				}
			}
			// DIO clicked
			else if(j==DIO_FIG){
				int DIOx=(int)(point.x/(m_Separator_inScreenCoord/(0.5*MaxDIO)));
				int DIOy=(point.y/(0.5*pFig->m_nBy));
				m_UserClickedDIOIdx=DIOy*0.5*MaxDIO+DIOx;
				Relay_ON_n_DisplayLED(m_UserClickedDIOIdx); m_UserClickedDIOIdx_ON=1;
				Name_and_Value_to_record("User-clicked DO:",m_UserClickedDIOIdx);
			}
			//pFig->RefreshPane(&objDC,&m_DC[pFig->m_page]);
			m_bIsScrXYforIJ=0;//Remap the graph.
			break;
		}
	}
	return found;
}

void CMOXY_View::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_isMOXY_Running==0)return; //When the program has not started, don't do anything.
	m_isInsidePane=0;
	m_isRtBUTTON_pressed=1;
	if(WhichPane(point)==XY_FIG){
		/* This portion is for the user to select an object to be used to display when the user clicks the XY window with the right click to get attention from the monkey.
		int tmpID=m_pXYFig->WhichObjSelected(point);
		if(tmpID>=0){//The user has selected a NEW object ID.
			m_SelectedVisualObjectID=tmpID;
			CClientDC objDC(this);
			RequestDisplayEvent(REDRAW_XY_WINDOW_FIG);//To refresh the List of Object IDs
		}
		else */
		if(m_SelectedVisualObjectID>=0 && m_isInsidePane==1){
			double xy[2];
			m_RightClickX=point.x; m_RightClickY=point.y; 
			FromScrnToDataCoordinate(point.x, point.y,xy[0],xy[1]);
			PvisShowStimAtHereNOW(1,&m_SelectedVisualObjectID,&xy[0],&xy[1]);
			Name_and_ArrayOfValues_to_record("Use-presened a dot",xy,2);
		}
	}

	CView::OnRButtonDown(nFlags, point);
}

void CMOXY_View::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_isRtBUTTON_pressed=0;
	if(m_SelectedVisualObjectID>=0){
		PvisHideStimAtHereNOW(1,&m_SelectedVisualObjectID);
	}
	CView::OnRButtonUp(nFlags, point);
}


void CMOXY_View::RemeasureRWDRect(int Dur_BigRWD)
{
	if(m_pXYFig->m_RwdAmount.status==Dur_BigRWD)return;
	m_pXYFig->m_RwdAmount.status=Dur_BigRWD;
	char RwdStr[256], tmpRwdStr[256]; 
	itoa(Dur_BigRWD,RwdStr,10); 
	sprintf(tmpRwdStr,"RWD: %s",RwdStr);
	m_pXYFig->m_RwdAmount.NameString=tmpRwdStr;
	m_pXYFig->RectOfLabel(m_pXYFig->m_RwdAmount, &m_DC[0]);
}

void CMOXY_View::Remeasure_BehavioralObj_Rect(int BehavioralObj)
{
	char ObjStr[256], tmpRwdStr[256]; 
	itoa(BehavioralObj,ObjStr,10); 
	sprintf(tmpRwdStr,"B-Obj: %s",ObjStr);
	m_pXYFig->m_BehavioralObj.NameString=tmpRwdStr;
	m_pXYFig->RectOfLabel(m_pXYFig->m_BehavioralObj, &m_DC[0]);
}
