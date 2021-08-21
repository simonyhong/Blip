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
#include "MainFrm.h"
CPen CRunningLineDisplay::m_VIOLETLIGHT2;
CPen CRunningLineDisplay::m_rgbPEN1[MAX_NUM_PENS_FOR_RUNNING_LINE],CRunningLineDisplay::m_rgbPEN2[MAX_NUM_PENS_FOR_RUNNING_LINE],CRunningLineDisplay::m_rgbPEN3[MAX_NUM_PENS_FOR_RUNNING_LINE], CRunningLineDisplay::m_White3Pen;
int  CRunningLineDisplay::m_PensHaveBeenCreated, CRunningLineDisplay::m_PenIDs_alternating[MAX_NUM_PENS_FOR_RUNNING_LINE];

#define MAX_CONTINUOUS_SDF_ms	(MAX_TIMESPAN_IN_SEC_STORED_IN_Q*1000)//in ms	


CRunningLineDisplay::CRunningLineDisplay()
{
	m_FirstDisp=1;
	m_SIGi=0;//m_SIGi points the next array index to process
	m_IsToSuperImpose=0;
	m_numUserPickedVariables=0;
	m_sTriggerEcode="None";
	m_RecordingSourceChanID=0; m_RL_RecordingSourceNn=0;
	m_PensHaveBeenCreated=0;
	for(int i=0;i<MAX_NUM_USER_PICKED_VARS;i++){m_UserPickedVar[i].m_PrvVarValue=0;}
}

CRunningLineDisplay::~CRunningLineDisplay()
{
	m_SDF.RemoveAll();
	for(int i=0;i<MAX_NUM_PENS_FOR_RUNNING_LINE;i++){
		m_rgbPEN1[i].DeleteObject();
		m_rgbPEN2[i].DeleteObject();
		m_rgbPEN3[i].DeleteObject();
	}
	m_VIOLETLIGHT2.DeleteObject();
	m_White3Pen.DeleteObject();
}

void CMOXY_View::Running_Line_Display(int RL_FigID) 
{
	if(m_bIsON==0){AfxMessageBox("The MOXY needs to start first.");return; }
	m_isRunningLineSDF=1;

	m_RunPanel[RL_FigID].Init_RunningLineSDF_FIG();
	m_RunPanel[RL_FigID].Redraw_RunningLineFig();
	m_RunPanel[RL_FigID].RunningLineSDF();
}

UINT Running_Line_Display_Thread_1(LPVOID WinObjPtr); //A global Function
UINT Running_Line_Display_Thread_2(LPVOID WinObjPtr); //A global Function
void CRunningLineDisplay::Init_RunningLineSDF_FIG()
{
	pView->VirMemDC();
	int i;
	pView->m_DC[0].SelectObject(&pView->m_WhiteBrushView);

	m_pRLineFig->m_normTx=pView->m_Separator_inScreenCoord;  
	m_pRLineFig->m_normBx=pView->m_savedClientRect.right;  
	if(m_RL_FigID==0){
		m_pRLineFig->m_normTy=pView->m_savedClientRect.top;   
		m_pRLineFig->m_normBy=pView->m_HorizontalSeparator_inScreenCoord;
	}
	else{
		m_pRLineFig->m_normTy=pView->m_HorizontalSeparator_inScreenCoord;   
		m_pRLineFig->m_normBy=pView->m_savedClientRect.bottom; 
	}
	if(m_FirstDisp==1){
		m_FirstDisp=0;
		//Screen size and Figure positions
		//m_pRLineFig->SetFontSize(&pView->m_DC[0]);
		pView->SetFontSize(m_pRLineFig);
		m_pRLineFig->m_Is2ndAxis=1;
		if(m_RL_FigID==0)m_pRLineFig->Set_MinMax_n_Labels_XY("","",0,10000,-60,60,oFRAME);	
		else             m_pRLineFig->Set_MinMax_n_Labels_XY("","",0,600000,0,100,oFRAME);	
		m_pRLineFig->Set_2ndMinMaxY(0,400);//Register the second set of Y axis (the labels for the right side of the figure)	
		pView->RecalibratePanes();//Register the top, left, bottom, right positions of the "pane" of the figure to be used in the following line.
		m_pRLineFig->Prepare4FigFrame(&pView->m_DC[0]);
		// If TAS is already running, get the info of the user-defined variables.
		if(*(pView->M_isTAS_Running)==1 && m_RL_FigID==1){
			pView->Iitialize_UserPicked_Variables_to_Display();
			pView->Register_a_User_Picked_Variable();
		}
	}
	else{
		m_pRLineFig->Set_MinMax_n_Labels_XY("","",m_pRLineFig->m_dMinX,m_pRLineFig->m_dMaxX,m_pRLineFig->m_dMinY,m_pRLineFig->m_dMaxY,oFRAME);	
		m_pRLineFig->Set_2ndMinMaxY(m_pRLineFig->m_dMinY2, m_pRLineFig->m_dMaxY2);
		pView->RecalibratePanes();
		m_pRLineFig->Prepare4FigFrame(&pView->m_DC[0]);
	}
	//In case there is no space for the RL to display its content, disable (pause/idle) it. But leave the non-RL part running.
	if(m_pRLineFig->m_nPaneBx <= m_pRLineFig->m_nPaneTx)pView->m_isRunningLineSDF=0; 	//Display only the non-RL part (e.g. XY-Eye position monitor)
	//If it is restarting after pView->m_isRunningLineSDF==0 (meaning the RL part has been disabled), start all over.
	else if(pView->m_isRunningLineSDF==0){
		pView->m_isRunningLineSDF=2; Sleep(100); 	
		if(m_RL_FigID==0)AfxBeginThread( Running_Line_Display_Thread_1, pView);
		else AfxBeginThread( Running_Line_Display_Thread_2, pView);
	}

	//For RL timing
	CClientDC objDC(pView);
	m_PrvRLtime=0;
	//if(m_IsToSuperImpose==0)m_pRLineFig->RefreshFrame(&objDC,m_pDC);
	m_TimeOfMinX_ms=*M_LatestTimeStampInMicroS*0.001;

	//######## Labels of Axes
	double stepX,stepY, stepY2;
	m_pRLineFig->m_howManyX=5;m_pRLineFig->m_howManyY=5;
	stepX=(m_pRLineFig->m_dMaxX-m_pRLineFig->m_dMinX)/(m_pRLineFig->m_howManyX-1);
	for(i=0;i<m_pRLineFig->m_howManyX+1;i++){
		if(i%1==0){m_pRLineFig->m_IsLabeledTicX[i]=1;m_pRLineFig->m_XTics[i]=m_pRLineFig->m_dMinX+stepX*i;}
		else m_pRLineFig->m_IsLabeledTicY[i]=0;//Do not label
	} 
	stepY =(m_pRLineFig->m_dMaxY -m_pRLineFig->m_dMinY )/(m_pRLineFig->m_howManyY-1);
	stepY2=(m_pRLineFig->m_dMaxY2-m_pRLineFig->m_dMinY2)/(m_pRLineFig->m_howManyY-1);
	for(i=0;i<m_pRLineFig->m_howManyY+1;i++){
		if(i%1==0){
			m_pRLineFig->m_IsLabeledTicY[i]=1;
			m_pRLineFig->m_YTics[i]=m_pRLineFig->m_dMinY+stepY*i;
			m_pRLineFig->m_Y2ndTics[i]=m_pRLineFig->m_dMinY2+stepY2*i;
		}
		else m_pRLineFig->m_IsLabeledTicY[i]=0;//Do not label
	} 
	//######## User-picked variables to display
	m_pRLineFig->m_NumVariables_onLeftAxes=0; m_pRLineFig->m_NumVariables_onRightAxes=0;
	for(i=0;i<m_numUserPickedVariables;i++){
		m_pRLineFig->PositionOfUserPickedVariables(i,m_UserPickedVar[i].m_Front0_Or_Back1_axis);
	}
}

void CRunningLineDisplay::Redraw_RunningLineFig() 
{	
	CClientDC objDC(pView);
	CBrush *pOldBrush=(CBrush*)m_pDC->SelectStockObject(WHITE_BRUSH);
	//m_pDC->SelectObject(?);
	m_pDC->Rectangle(m_pRLineFig->m_nTx,m_pRLineFig->m_nTy,m_pRLineFig->m_nBx,m_pRLineFig->m_nBy);
	m_pRLineFig->Frame_Graph2D(m_pDC);
	//m_pDC->MoveTo(pView->m_Separator_inScreenCoord,m_pRLineFig->m_nPaneTy);m_pDC->LineTo(pView->m_Separator_inScreenCoord,m_pRLineFig->m_nPaneBy);
	m_pRLineFig->RefreshFrame(&objDC,m_pDC);
	m_MaxRunningSDFX=m_pRLineFig->m_dMaxX;
	m_DispEyeTraceAtEveryXms=(int)(5.*(m_pRLineFig->m_dSpanX/10000.)); //10000 (10s, where the denominator becomes 1) gets 5ms display interval.
	DispTriggerStatus();
	DispSupperimposeStatus();
	DispRecordingSource_Chan_Nn();
	DisplayUserPickedVariables();
	m_pDC->SelectObject(pOldBrush);
}

void CRunningLineDisplay::Register_a_variable_to_display(int UserPickedVarID)//char NickName[], int Left0_Or_Right1_axis, int IsToShowRunningLine)
{	
	m_UserPickedVar[m_numUserPickedVariables].m_TypeOfVar=pView->M_VarToDisp[UserPickedVarID].M_TypeOfVar;
	m_pRLineFig->m_VarToDispString[m_numUserPickedVariables]=pView->M_VarToDisp[UserPickedVarID].M_VarInChar;
	//m_pRLineFig->PositionOfUserPickedVariables(m_numUserPickedVariables,pView->M_VarToDisp[UserPickedVarID].M_Left0_Or_Right1_axis);
	m_UserPickedVar[m_numUserPickedVariables].m_ColorOrder=pView->M_VarToDisp[UserPickedVarID].M_ColorOrder;
	m_UserPickedVar[m_numUserPickedVariables].m_Front0_Or_Back1_axis=pView->M_VarToDisp[UserPickedVarID].M_Left0_Or_Right1_axis;
	m_UserPickedVar[m_numUserPickedVariables].m_pValue=&(pView->M_VarToDisp[UserPickedVarID].M_Value);
	m_UserPickedVar[m_numUserPickedVariables].m_pAddThis=&(pView->M_VarToDisp[UserPickedVarID].M_AddThisToDisplay);
	m_UserPickedVar[m_numUserPickedVariables].m_pMultiplyThis=&(pView->M_VarToDisp[UserPickedVarID].M_MultiplyThisToDisplay);
	m_numUserPickedVariables++;
	m_pRLineFig->m_numUserPickedVariables=m_numUserPickedVariables;

	//######## Position the variables to display
	m_pRLineFig->m_NumVariables_onLeftAxes=0; m_pRLineFig->m_NumVariables_onRightAxes=0;
	for(int i=0;i<m_numUserPickedVariables;i++){
		m_pRLineFig->PositionOfUserPickedVariables(i,m_UserPickedVar[i].m_Front0_Or_Back1_axis);
	}
	pView->RequestDisplayEvent(DISP_LABELS_OF_USER_PICKED_VAR,m_RL_FigID);
}

void CRunningLineDisplay::DisplayUserPickedVariables() 
{
	if(m_RL_FigID==0){
		Sleep(0);
	}
	CClientDC objDC(pView);
	int left, right, top, bottom,i;
	char a[1222];
	for(i=0;i<m_numUserPickedVariables;i++){
		left=m_pRLineFig->m_VarToDispRECT[i].left;
		right=m_pRLineFig->m_VarToDispLineEnd[i];//The text + line
		top=m_pRLineFig->m_VarToDispRECT[i].top-m_pRLineFig->m_FontHeight; //One fontHeight higher than the variable itself.
		bottom=m_pRLineFig->m_VarToDispRECT[i].top;
		//The nickname of the variable
		m_pDC->TextOut(left,top,m_pRLineFig->m_VarToDispString[i]);
		strcpy(a,m_pRLineFig->m_VarToDispString[i]);
		//The legend line
		if(m_UserPickedVar[i].m_ColorOrder<0)m_pDC->SelectObject(&m_White3Pen);
		else m_pDC->SelectObject(&m_rgbPEN3[m_UserPickedVar[i].m_ColorOrder]);
		m_pDC->MoveTo(m_pRLineFig->m_VarToDispRECT[i].right+m_pRLineFig->m_FontWidth,top+0.5*m_pRLineFig->m_FontHeight); m_pDC->LineTo(right,top+0.5*m_pRLineFig->m_FontHeight);
		//This BitBlt needs to be inside the "for-loop" here, since it "unvails" the name of the values one by one.
		objDC.BitBlt(left,top,	right-left,bottom-top, m_pDC,left,top,SRCCOPY);
	}
	char VarValue[64];
	for(i=0;i<m_numUserPickedVariables;i++){
		if(m_UserPickedVar[i].m_TypeOfVar<=1)sprintf(VarValue,"%d",(int)(m_UserPickedVar[i].m_PrvVarValue));   //unsigned char or Int
		else							 sprintf(VarValue,"%0.1lf",(double)(m_UserPickedVar[i].m_PrvVarValue));//Float or double
		objDC.TextOut(m_pRLineFig->m_VarToDispRECT[i].left,m_pRLineFig->m_VarToDispRECT[i].top,VarValue);
	}
}

void CRunningLineDisplay::RunningLineSDF() 
{	
	//if(m_RL_FigID==1)return;
	CClientDC objDC(pView);
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	m_fSigma=40;
	m_halfGaussKerMs=2*m_fSigma;//Half the size of the Gaussian
	float fullGaussianMs=m_halfGaussKerMs*2;
	CreateKer(m_fSigma, m_halfGaussKerMs);

	m_SDF.SetSize(MAX_CONTINUOUS_SDF_ms);

	int usableIdx_SIGi,SDFi,SpkIdxForSDF=-1;
	double X, newRLtime, dVariable;
	int usableSpikeIdx,SPKi=-1,first;
	int maxX=m_pRLineFig->m_dMaxX;
	__int64 sizeOfDataQ=*M_AIQueueSize,LatestTimeStampMs;
	int i,initI,idx;
	int	usableEcodeIdx, Ecode, ECodeIdx=0, IsTimeToDispVar=100;
	__int64 sdfDATi,linearSDFi,SavedLinearSDFi;
	int p,color,ThickThin=0;
	m_SamplesPerMs=(int)(0.001*DATA_SAMPLING_RATE);
	float OneOverSamplesPerMs=1.0/m_SamplesPerMs;
	__int64 CurrentSDFi;
	m_pPenForSDF=&m_rgbPEN1[MAX_NUM_PENS_FOR_RUNNING_LINE-1];
	CSingleLock SyncObj_Display(&pView->m_sema); 

	//User picked variables to display
	char VarValue[64];

	while(*pView->M_isDAS_Running==0){	Sleep(1); if(pView->m_bIsON==0)goto EndProgram; }
	m_samplesPerMs=(int)(0.001*(*pView->M_SamplingRate)); if((float)m_samplesPerMs!=(float)(0.001*(*pView->M_SamplingRate)))AfxMessageBox("pView->M_SamplingRate is not a multiple of m_samplesPerMs");
	float invSamplesPerMs=(float)(1./(float)m_samplesPerMs);
	float halfGaussInMs=(float)(m_halfGaussKerMs*m_samplesPerMs);

	pView->RequestDisplayEvent(REDRAW_ALL_FIGURES); pView->RequestDisplayEvent(REDRAW_DIO_FIG); 

	m_prvEyeX=0; m_prvEyeY=0;m_PrvRLtime=0;
	SDFi=0; linearSDFi=0;
	if(*pView->M_isDAS_Running==1 && *M_usableSIGi>=0){
		sdfDATi=*M_usableSIGi;
		m_TimeOfMinX_ms=M_timeStampMicS[sdfDATi]*0.001;
		m_SDFbeginTime_us=M_timeStampMicS[sdfDATi];
		if(*M_usableSpikeIdxOfNn_RL>0)SPKi=*M_usableSpikeIdxOfNn_RL; 
		for(i=0;i<=*M_usableSpikeIdxOfNn_RL;i++){
			if(M_spikeTimeStampOfNn_RL[i]>=m_SDFbeginTime_us-m_halfGaussKerMs*1000){//Find a spike near the current Data point, if possible.
				SpkIdxForSDF=i; break;
			}
		}
		if(SpkIdxForSDF==-1){SpkIdxForSDF=*M_usableSpikeIdxOfNn_RL; }	
		//########  Clear the m_SDF[] around the current position. ######
		if(*M_usableSIGi-fullGaussianMs>-1){
			CurrentSDFi=(*M_usableSIGi-fullGaussianMs)*OneOverSamplesPerMs;//fullGaussianMs is used instead of m_halfGaussKerMs in case the spike-to-SDF process is lagging behaind a bit.
		}
		else CurrentSDFi=(AI_Q_SIZE+(*M_usableSIGi-fullGaussianMs))*OneOverSamplesPerMs;
		for(int i=CurrentSDFi-5*fullGaussianMs;i<CurrentSDFi+5*fullGaussianMs; i++){
			m_SDF[idxSDF(i)]=0; //Clear the m_SDF[] around the current position.
		}	
	} 
	else{ //DAS server & client are not running 
		m_TimeOfMinX_ms=0;
		m_SDFbeginTime_us=0;
	}
	m_prvXYpos_DispTimeMs=*M_LatestTimeStampInMicroS*0.001;
	m_x=0; m_lastSDFX=0;m_lastSDFY=0;

	m_pRLineFig->Graph2D(0,0, 9, &objDC);

	//Ecode display prapration
	int DisplayedEcode=0, Etime, alternate=0;
	double fontHight=m_pRLineFig->FromScreenToDataY(m_pRLineFig->m_FontHeight)-m_pRLineFig->FromScreenToDataY(0);


	//Main Loop 2 (The running-line display)
	while(1){
		Sleep(1);//The fastest process would be ~1KHz (maybe too fast?). See individual sections for more details (some are slower). 
		if(pView->m_bIsON==0)return;
		LatestTimeStampMs=*M_LatestTimeStampInMicroS*0.001;
		while(*pView->M_isDAS_Running==0){	Sleep(1); if(pView->m_bIsON==0)goto EndProgram; }
		if(pView->m_isRunningLineSDF==2){
			goto EndProgram;//Starting all over
		}

		//Only the first pannel does this
		if(m_RL_FigID==0){
			//########## Triggered start of running line.
			if((*M_IsTriggeredMode)==1 && (*M_hasTriggered)==1){
				(*M_hasTriggered)=0;
				m_TimeOfMinX_ms=(*M_TriggeredTime_ms);
				if(m_IsToSuperImpose==0){
					SyncObj_Display.Lock();
					pView->m_RunPanel[m_RL_FigID].Redraw_RunningLineFig();	//pView->RequestDisplayEvent(REDRAW_RUNNINGLINE_FIG,m_RL_FigID); //Don't use this one (parallel drawing) 
					SyncObj_Display.Unlock();
				}
				m_x=0; m_lastSDFX=0;
				m_PrvRLtime=0;
			}			
			//##########  ECODES
			usableEcodeIdx=*pView->M_usableEcodeIdx;
			if(usableEcodeIdx>-1){
				while(ECodeIdx!=usableEcodeIdx){			
					if(++ECodeIdx>=ECODE_Q_SIZE)ECodeIdx=0;
					Ecode=pView->M_EventCodes[ECodeIdx];
					for(idx=0;idx<*(pView->M_NumEcodesInTable);idx++){
						if(Ecode==pView->M_EcodeTable[idx].m_EcodeItself){
							Etime=0.001*pView->M_EventTimes[ECodeIdx]-m_TimeOfMinX_ms;
							SyncObj_Display.Lock();
							//Line
							m_pRLineFig->ThinVioletLine(Etime,m_pRLineFig->m_dMaxY,Etime,MsgPosY(m_pRLineFig,0.8),&objDC);
							//"Alias" of the ECode 
							m_pRLineFig->PutText(Etime,m_pRLineFig->m_dMaxY+alternate*fontHight,&(pView->M_EcodeTable[idx].M_EcodeAlias[0]),&objDC);
							SyncObj_Display.Unlock();
							alternate=!alternate;
							//For SDF to alternate colors
							color=(int)((MAX_NUM_PENS_FOR_RUNNING_LINE-1)*((float)idx/(*(pView->M_NumEcodesInTable)-1)));
							color=m_PenIDs_alternating[color];
							if(ThickThin==0){m_pPenForSDF=&m_rgbPEN1[color];ThickThin=1;}
							else			{m_pPenForSDF=&m_rgbPEN2[color];ThickThin=0;}
							break;
						}
					}
				}
			}
			//##########  SDF  ########
			//##########  SDF  ########
			if(*M_usableSIGi>=0){//The spike discriminator AI index
				usableSpikeIdx=*M_usableSpikeIdxOfNn_RL;
				if(SpkIdxForSDF<0 && usableSpikeIdx>=0){//Spike happens for the first time.
					SpkIdxForSDF=usableSpikeIdx;
				}
				else{//The first spike of the program is considered for calculation one iteration later, but that is okay.
					// Caclulate SDF using available spikes.
					while(SpkIdxForSDF!=usableSpikeIdx){
						SpkIdxForSDF=idxSpkQ(++SpkIdxForSDF); //get the next spike index 
						initI=M_spike_IdxOfAIOfNn_RL[SpkIdxForSDF]*OneOverSamplesPerMs-m_halfGaussKerMs; //Map the M_AI index (ms*Samples/Ms) to SDF index (in ms)
						for(p=0;p<=fullGaussianMs;p++){
							m_SDF[idxSDF(p+initI)]+=KrVal[p]; //KrVal [] has the Gaussian kernel vlaues. 
						}
					}
				}
				// Calculate the current SDF position to show, using the current index related to M_AI[], (*M_usableSIGi).
				usableIdx_SIGi=*M_usableSIGi;  
				CurrentSDFi=usableIdx_SIGi*OneOverSamplesPerMs-m_halfGaussKerMs;//Calculate the current SDF index using the M_AI[] index 
				if(CurrentSDFi<0) CurrentSDFi=MAX_CONTINUOUS_SDF_ms+CurrentSDFi;
				// SDF Display
				if(SDFi!=CurrentSDFi){
					SyncObj_Display.Lock();
					objDC.SelectObject(m_pPenForSDF);
					DispSDF(SDFi, CurrentSDFi,  &objDC); //It equalizes the indexes (SDFi==CurrentSDFi)
					SyncObj_Display.Unlock();
				}
				else Sleep(1);
				//##########  Raster  ########
				//##########  Raster  ########
				if(SPKi<0 && usableSpikeIdx>=0)SPKi=usableSpikeIdx;//SPKi is setting for the first time
				else{//The first spike of the program is considered one iteration later, but that is okay.
					SyncObj_Display.Lock();
					objDC.SelectObject(&pView->m_BLACK);
					while(SPKi!=usableSpikeIdx){
						if(++SPKi>=*M_Spike_QueueSize)SPKi=0;
						X=(M_spikeTimeStampOfNn_RL[SPKi]*0.001-m_TimeOfMinX_ms);
						if(X>=m_pRLineFig->m_dMinX && X<=m_pRLineFig->m_dMaxX){
							m_pRLineFig->MySetTick(X,MsgPosY(m_pRLineFig,0.01), &objDC);
						}
					}	
					SyncObj_Display.Unlock();
				}
			}
			if(pView->m_bIsON==0) break;
		}

		//####  Draw every ~5 ms (200Hz; including eye position *LINE TRACE*)  ######//
		//####  Draw every ~5 ms (200Hz; including eye position *LINE TRACE*)  ######//
		if(LatestTimeStampMs-m_prvXYpos_DispTimeMs>m_DispEyeTraceAtEveryXms){ 
			newRLtime=(double)(LatestTimeStampMs-m_TimeOfMinX_ms);
			if(m_RL_FigID==0){
				//##########  Eye position *LINE TRACE*
				pView->Get_Latest_XYpos();//This needs to be here becase the Eye position trace has a higher temporal resolution of display than that of the Eye position DOT.
				m_prvXYpos_DispTimeMs=LatestTimeStampMs;
				if(newRLtime<m_MaxRunningSDFX){//Skip displaying
					if((m_prvEyeX>m_pRLineFig->m_dMinY && m_prvEyeX<m_pRLineFig->m_dMaxY) && ((*m_pEyeX)>m_pRLineFig->m_dMinY && (*m_pEyeX)<m_pRLineFig->m_dMaxY)){
						SyncObj_Display.Lock();
						objDC.SelectObject(&pView->m_RED);
						m_pRLineFig->LineAtoB(m_PrvRLtime,m_prvEyeX,newRLtime,(*m_pEyeX),&objDC);
						SyncObj_Display.Unlock();
					}
					if((m_prvEyeY>m_pRLineFig->m_dMinY && m_prvEyeY<m_pRLineFig->m_dMaxY) && ((*m_pEyeY)>m_pRLineFig->m_dMinY && (*m_pEyeY)<m_pRLineFig->m_dMaxY)){
						SyncObj_Display.Lock();
						objDC.SelectObject(&pView->m_BLUE);
						m_pRLineFig->LineAtoB(m_PrvRLtime,m_prvEyeY,newRLtime,(*m_pEyeY),&objDC);
						SyncObj_Display.Unlock();
					}
				}
				m_prvEyeX=(*m_pEyeX); m_prvEyeY=(*m_pEyeY);
			}
			//##########  User defined variables
			for(i=0;i<m_numUserPickedVariables;i++){
				dVariable=(*(m_UserPickedVar[i].m_pValue))*(*(m_UserPickedVar[i].m_pMultiplyThis))+(*(m_UserPickedVar[i].m_pAddThis));
				if(m_UserPickedVar[i].m_ColorOrder>=0){//Show only the ones that the user wanted to see the running-line display
					SyncObj_Display.Lock();
					objDC.SelectObject(&m_rgbPEN2[m_UserPickedVar[i].m_ColorOrder]);
					m_pDC->SelectObject(&m_rgbPEN2[m_UserPickedVar[i].m_ColorOrder]);
					//if(m_RL_FigID==1)m_pDC->SelectObject(&m_rgbPEN2[m_UserPickedVar[i].m_ColorOrder]);
					if(m_UserPickedVar[i].m_Front0_Or_Back1_axis==USE_RIGHT_AXIS){
						m_pRLineFig->LineAtoB2nd(m_PrvRLtime,m_UserPickedVar[i].m_PrvVarValue,newRLtime,dVariable,&objDC);
						m_pRLineFig->LineAtoB2nd(m_PrvRLtime,m_UserPickedVar[i].m_PrvVarValue,newRLtime,dVariable,m_pDC);
						//if(m_RL_FigID==1)m_pRLineFig->LineAtoB2nd(m_PrvRLtime,m_UserPickedVar[i].m_PrvVarValue,newRLtime,dVariable,m_pDC);
					}
					else{
						m_pRLineFig->LineAtoB(m_PrvRLtime,m_UserPickedVar[i].m_PrvVarValue,newRLtime,dVariable,&objDC);
						m_pRLineFig->LineAtoB(m_PrvRLtime,m_UserPickedVar[i].m_PrvVarValue,newRLtime,dVariable,m_pDC);
						//if(m_RL_FigID==1)m_pRLineFig->LineAtoB(m_PrvRLtime,m_UserPickedVar[i].m_PrvVarValue,newRLtime,dVariable,m_pDC);
					}
					SyncObj_Display.Unlock();
				}
				m_UserPickedVar[i].m_PrvVarValue=dVariable;
			}
			if(IsTimeToDispVar-- <1){//Update the number of the user-defined variable 
				IsTimeToDispVar=50;
				for(i=0;i<m_numUserPickedVariables;i++){
					if(m_UserPickedVar[i].m_TypeOfVar<=1)sprintf(VarValue,"%d       ",(int)(m_UserPickedVar[i].m_PrvVarValue));     //Int
					else							     sprintf(VarValue,"%0.1lf     ",(double)(m_UserPickedVar[i].m_PrvVarValue));//Float or double
					SyncObj_Display.Lock();
					//if(strcmp(VarValue,m_UserPickedVar[i].m_VarInChar)!=0){//Display only when they are different.
						objDC.TextOut(m_pRLineFig->m_VarToDispRECT[i].left,m_pRLineFig->m_VarToDispRECT[i].top,VarValue);
						strcpy(m_UserPickedVar[i].m_VarInChar,VarValue);
					//}
					SyncObj_Display.Unlock();
				}
			}
			//##########  update time
			if(newRLtime>m_MaxRunningSDFX-5&& (*M_IsTriggeredMode)==0){
				m_PrvRLtime=0;
				if(m_IsToSuperImpose==0){
					pView->m_RunPanel[m_RL_FigID].Redraw_RunningLineFig();//pView->RequestDisplayEvent(REDRAW_RUNNINGLINE_FIG,m_RL_FigID); //Don't use this one (parallel drawing)
				}
				m_TimeOfMinX_ms=LatestTimeStampMs;
			}
			else m_PrvRLtime=newRLtime;
		}
	}
EndProgram:;
}

//Find Xintersection between y=yEqualsTo; and a line formed by two points (X1, Y1), (X2, Y2).
double CRunningLineDisplay::FindIntersactionX_between_One_line_and_AnotherWithTwoPoints(double yEqualsTo, double X1, double Y1, double X2, double Y2)
{
	return (yEqualsTo-Y1)/((Y2-Y1)/(X2-X1))+X1;
}
void CRunningLineDisplay::DispSDF(int &SDFi, int endI, CClientDC *pObjDC)
{
	if(m_lastSDFX<m_pRLineFig->m_dMinX)m_pRLineFig->MoveTo2nd(m_pRLineFig->m_dMinX,m_lastSDFY, pObjDC);
	else m_pRLineFig->MoveTo2nd(m_lastSDFX,m_lastSDFY, pObjDC);

	while(SDFi!=endI){
		if(++SDFi>=MAX_CONTINUOUS_SDF_ms)SDFi=0;
		m_y=(double)m_SDF[SDFi]; 
		m_x=M_timeStampMicS[SDFi*m_SamplesPerMs]*0.001-m_TimeOfMinX_ms;
		if(m_y>m_pRLineFig->m_dMaxY2)m_pRLineFig->MoveTo2nd(m_x,m_y, pObjDC);
		else if(m_x>0){if(m_x<m_MaxRunningSDFX)m_pRLineFig->LineTo2nd(m_x,m_y, pObjDC);}//LineTo2nd() is used because the SDF uses the right (2nd) axis of the figure.
		else m_pRLineFig->MoveTo2nd(0.,m_y, pObjDC); //This will set the initial (m_x=0) point of the figure correct.
		m_SDF[idxSDF((int)(SDFi+m_halfGaussKerMs*10))]=0;//Clear the way ahead
	}
	m_lastSDFX=m_x; m_lastSDFY=m_y;
}

int CRunningLineDisplay::idxSpkQ(int i)
{
	if(i>=0 && i<((*M_Spike_QueueSize)))return i;
	else if(i>(*M_Spike_QueueSize)-1)return (i-(*M_Spike_QueueSize));
	else return ((*M_Spike_QueueSize)+i);//if(i<0) 
}

int CRunningLineDisplay::idxSDF(int i)
{
	if(i>=0 && i<((MAX_CONTINUOUS_SDF_ms)))return i;
	else if(i>(MAX_CONTINUOUS_SDF_ms)-1)return (i-(MAX_CONTINUOUS_SDF_ms));
	else return ((MAX_CONTINUOUS_SDF_ms)+i);//if(i<0) 
}


void CRunningLineDisplay::CreateKer(float sig, float RangeX)
{	
	#define KrCntr 0
	int p, IniP,FinP;
	float RangeXSq, sigSq, SqDist;
	sigSq=sig*sig; RangeXSq=RangeX*RangeX;
	//////////////////////////////////
	m_GaussianAmp=pView->Kernel_1D(1000, sig, RangeX);	//KrSum==1 means no amplification
	NumP=0;
	IniP=KrCntr-RangeX; FinP=KrCntr+RangeX;
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(KrCntr,p))>(float)RangeXSq)continue;
		KrVal[NumP]=m_GaussianAmp*exp(-SqDist/sigSq);
		P[NumP]=p; NumP++;
	}
}

void CRunningLineDisplay::PensForChannel()
{
	if(m_PensHaveBeenCreated==1)return;
	m_PensHaveBeenCreated=1;
	pView->MyOriginalColorMix(m_RGB100,MaxRGB100);
	int i,j;
	for(i=0;i<MAX_NUM_PENS_FOR_RUNNING_LINE;i++){
		j=(MaxRGB100-1)-i*(MaxRGB100/MAX_NUM_PENS_FOR_RUNNING_LINE);
		if(j<0)j=0;
		m_rgbPEN1[i].CreatePen(PS_SOLID, 1,m_RGB100[j]);
		m_rgbPEN2[i].CreatePen(PS_SOLID, 2,m_RGB100[j]);
		m_rgbPEN3[i].CreatePen(PS_SOLID, 3,m_RGB100[j]);
	}
	///////  Mix the numbers to alternate ////////
	int cnt;
	int HalfPoint=(MAX_NUM_PENS_FOR_RUNNING_LINE+(MAX_NUM_PENS_FOR_RUNNING_LINE%2))*0.5;
	for(i=0,cnt=0;i<MAX_NUM_PENS_FOR_RUNNING_LINE;i+=2){
		m_PenIDs_alternating[i]=cnt++;
	}
	for(i=1,cnt=0;i<MAX_NUM_PENS_FOR_RUNNING_LINE;i+=2){
		m_PenIDs_alternating[i]=HalfPoint+cnt++;
	}
	///////////////////
	m_VIOLETLIGHT2.CreatePen(PS_SOLID, 2,RGB(200,0,250));
	m_White3Pen.CreatePen(PS_SOLID, 3,RGB(255,255,255));
}

void CRunningLineDisplay::DispSupperimposeStatus() 
{	
	CClientDC ObjDC(pView);
	if(m_IsToSuperImpose==1){
		m_pRLineFig->m_SuperImposeStatus.NameString="   Superimposition    ";
	}
	else m_pRLineFig->m_SuperImposeStatus.NameString="No Superimposition";
	m_pDC->TextOut(m_pRLineFig->m_SuperImposeStatus.StringRect.left, m_pRLineFig->m_SuperImposeStatus.StringRect.top,m_pRLineFig->m_SuperImposeStatus.NameString);
	ObjDC.BitBlt(m_pRLineFig->m_SuperImposeStatus.StringRect.left,m_pRLineFig->m_SuperImposeStatus.StringRect.top,m_pRLineFig->m_SuperImposeStatus.StringRect.right-m_pRLineFig->m_SuperImposeStatus.StringRect.left,m_pRLineFig->m_SuperImposeStatus.StringRect.bottom-m_pRLineFig->m_SuperImposeStatus.StringRect.top,m_pDC,m_pRLineFig->m_SuperImposeStatus.StringRect.left,m_pRLineFig->m_SuperImposeStatus.StringRect.top,SRCCOPY);
}

void CRunningLineDisplay::DispRecordingSource_Chan_Nn() 
{	
	if(m_RL_FigID>0)return; //Currently, only the top figure handles the recording source
	CClientDC ObjDC(pView);
	char Source[256];
	sprintf(Source,"Chan:%d/Nn:%d",m_RecordingSourceChanID,m_RL_RecordingSourceNn);
	m_pRLineFig->m_NnRecordingChan.NameString=Source;
	m_pDC->TextOut(m_pRLineFig->m_NnRecordingChan.StringRect.left, m_pRLineFig->m_NnRecordingChan.StringRect.top,m_pRLineFig->m_NnRecordingChan.NameString);
	ObjDC.BitBlt(m_pRLineFig->m_NnRecordingChan.StringRect.left,m_pRLineFig->m_NnRecordingChan.StringRect.top,m_pRLineFig->m_NnRecordingChan.StringRect.right-m_pRLineFig->m_NnRecordingChan.StringRect.left,m_pRLineFig->m_NnRecordingChan.StringRect.bottom-m_pRLineFig->m_NnRecordingChan.StringRect.top,m_pDC,m_pRLineFig->m_NnRecordingChan.StringRect.left,m_pRLineFig->m_NnRecordingChan.StringRect.top,SRCCOPY);
}


void CRunningLineDisplay::DispTriggerStatus() 
{	
	if(m_RL_FigID>0)return; //Currently, only the top figure handles the trigger status
	CClientDC ObjDC(pView);
	if(*(pView->M_IsTriggeredMode)==1){
		//When the  ECode is <MAX_NUM_OF_ALIASES_1TO1, the alias of it (if there is any) could be in the pView->m_EcodeAlias_1to1[]
		int i;
		for(i=0;i<*(pView->M_NumEcodesInTable);i++){
			if(*M_nTriggerECode==pView->M_EcodeTable[i].m_EcodeItself){
				CString buff=&pView->M_EcodeTable[i].M_EcodeAlias[0];
				m_pRLineFig->m_TriggerOptionStatus.NameString="Trigger:"+buff+"  ";
				break;
			}
		}
		if(i==*(pView->M_NumEcodesInTable)){
			char Buff[100];sprintf(Buff,"%d",*M_nTriggerECode);// strcat((char*)(&m_pRLineFig->m_TriggerOptionStatus.NameString),Buff);
			CString buff=Buff;
			m_pRLineFig->m_TriggerOptionStatus.NameString="Trigger:"+buff+"  ";
		}
	}
	else m_pRLineFig->m_TriggerOptionStatus.NameString="Free-Run  ";
	int left=m_pRLineFig->m_TriggerOptionStatus.StringRect.left,right=m_pRLineFig->m_TriggerOptionStatus.StringRect.right, top=m_pRLineFig->m_TriggerOptionStatus.StringRect.top,bottom=m_pRLineFig->m_TriggerOptionStatus.StringRect.bottom;
	m_pDC->TextOut(left, top,m_pRLineFig->m_TriggerOptionStatus.NameString);
	ObjDC.BitBlt(left,top,right-left,bottom-top,m_pDC,left,top,SRCCOPY);
}

