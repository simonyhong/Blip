// TAS_View.cpp : implementation of the CTASView class
//

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
#include <afxmt.h>


//###################################  The actual communications  ################################


//This is a special function which does not wait fpr or handshake with VIS.
void CMOXY_View::PvisShowStimAtHereNOW(int N, int Obj[], double X[], double Y[])
{
	int i;
	*M_UserClickedN=N;
	for(i=0;i<N;i++)M_UserClickedVisObjIDs[i]=Obj[i]; 
	for(i=0;i<N;i++)M_cntrX[Obj[i]]=X[i]; 
	for(i=0;i<N;i++)M_cntrY[Obj[i]]=Y[i]; 
	if(m_IsToShowFigureOnRightClick==0)*M_Msg_from_MOXY_to_VIS=1; //The message is to inform the VIS to draw a box.
	else *M_Msg_from_MOXY_to_VIS=2;								  //The message is to inform the VIS to draw a Figure.

	//Replicate VIS
	if(m_IsToShowFigureOnRightClick==0)PvisShowStimAtHereNOW_ReplicationOfVIS();	//This replicates VIS display in the XY monitor
	else PvisShowFigAtHereNOW_ReplicationOfVIS();

}

//This is a special function which does not wait fpr or handshake with VIS.
void CMOXY_View::PvisHideStimAtHereNOW(int N, int Obj[])
{
	int i;
	*M_UserClickedN=N;
	for(i=0;i<N;i++)M_UserClickedVisObjIDs[i]=Obj[i]; 
	*M_Msg_from_MOXY_to_VIS=3; //The message is to inform the VIS to Erase the user-clicked stimulus.

	//Replicate VIS
	PvisHideStimAtHereNOW_ReplicationOfVIS();	//This replicates VIS display in the XY monitor
}


void CMOXY_View::Iitialize_UserPicked_Variables_to_Display()
{	
	m_TotNumUserPickedVariables=0;
	for(int i=0;i<NUM_RL_PANELS;i++){
		m_RunPanel[i].m_pRLineFig->m_numUserPickedVariables=0;
		m_RunPanel[i].m_numUserPickedVariables=0;
	}
}

//This function re-registeres all the user picked variables afresh everytime it is called. This eliminates any disparity between TAS MOXY.
void CMOXY_View::Register_a_User_Picked_Variable()
{	
	int i;
	for(i=m_TotNumUserPickedVariables;i<*M_NumUserPickedVariables; i++){
		m_RunPanel[M_VarToDisp[i].M_FigIdx].Register_a_variable_to_display(i);
		RequestDisplayEvent(DISP_LABELS_OF_USER_PICKED_VAR,M_VarToDisp[i].M_FigIdx);
		m_TotNumUserPickedVariables++;
	}
}

