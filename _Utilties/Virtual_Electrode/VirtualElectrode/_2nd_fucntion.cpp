#include "stdafx.h"
#include "Analysis.h"
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"

void CAnalysisView::_2nd_InteractiveMRI() 
{
	m_bIsON=1;
	VirMemDC();
	CMRI::m_pView=this;

	//################################################
	Read_3D_2D_Config();	
	if((m_isCoronal_Or_Sagittal==CORONAL && (*M_isSagittal_Running==0||*M_is3D_Running==0)) || (m_isCoronal_Or_Sagittal==SAGITTAL && (*M_isCoronal_Running==0 ||*M_is3D_Running==0)) )
		Calculate_Electrode_and_Grid_Coordinates();	
	if(m_isCoronal_Or_Sagittal==CORONAL){*M_isCoronal_Running=1;} else {*M_isSagittal_Running=1;}

	//################## Reset MRI
	m_NumMRIs=0; 
	//################## Reset the drawing.
	m_NumSelectedMarkings=0; m_NumSelectedDots=0; m_NumSelectedObjs=0; m_GrabbedObjectIdx=-1; m_GrabbedDotIdx=-1; m_GrabbedMarkingIdx=-1; m_CurrentlyEditedObjIdx=-1; m_UnderConstructionObjID=-1; m_EditMode=MAKE_LINE_SEG; m_TYPE=LINE_TYPE; m_isToDispOrigin=1;
	m_CoronalTexture_Loaded=0; //User's description of neural info as picture icons
	
	//################# Find and register MRI BMP files in the directory.
	Find_MRI_Files(M_DataDir,0,m_isCoronal_Or_Sagittal);// MRI.SetSize() happens in this function
	if(m_NumMRIs==0){AfxMessageBox("No MR images"); goto EndProgram;}
	Rearrage_MRI_Files();//After finding all files and their paths, sort the files in an ascending order using the number parts of the folder names,  (e.g. L-1.20.bmp)
	Load_MRIs(m_isCoronal_Or_Sagittal); if(m_Selected_MRI_Idx==-1)goto EndProgram;//There is no MRI

	//################# Prepare for the display
	Init_Figure_AFTER_MRI_loading();

	//################# Drawing
	m_Drawing.SetSize(m_NumMRIs,30);
	Load_DrawingData(m_isCoronal_Or_Sagittal);
	Load_Neuron_Data_and_Texture(1,1,1);

	//################  Prepare to communicate with the 3D program
	char EventName[100]; 
	for(int i=0; i<MAXIMUM_WAIT_KeyOBJECTS;i++){
		sprintf(EventName,"EVENT_M_KeyDownEvents_%d",i);
		if(*M_isKeyDownEventsCreated==0){*M_isKeyDownEventsCreated=1; M_KeyDownEvents[i]=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,EventName);}
		else M_KeyDownEvents[i]=OpenEvent(EVENT_ALL_ACCESS,TRUE,EventName);
	}
	SetEvent(M_KeyDownEvents[1]);

	//#############################   Electrode position-based slice update  ##############################
	*M_HeyCoronalFigure_RedrawElectrode=1;*M_HeySagittalFigure_RedrawElectrode=1;//Just to trigger the initiation of drawing.
	while(1){
		//The electrode has moved.
		if((m_isCoronal_Or_Sagittal==CORONAL && *M_HeyCoronalFigure_RedrawElectrode==1) || (m_isCoronal_Or_Sagittal==SAGITTAL && *M_HeySagittalFigure_RedrawElectrode==1) ){
			//Find the closest slice from the tip of the electrode.
			if(m_isCoronal_Or_Sagittal==CORONAL){
				*M_HeyCoronalFigure_RedrawElectrode=0;
				Find_MRI_Closest_To_CoronalSection_ToElectrodeTip();
			}
			else if(m_isCoronal_Or_Sagittal==SAGITTAL){
				*M_HeySagittalFigure_RedrawElectrode=0;
				Find_SliceClosest_SagittalSection_ToElectrodeTip();
			}
			Find_a_Matching_Drawing_toMRI();
			Redraw_Everything();
		}
		if(m_bIsON==0)break;
		Sleep(1);
		if(m_bIsON==0)break;
	}
	
EndProgram:
	m_bIsON=0;
	if(m_isCoronal_Or_Sagittal==CORONAL){*M_isCoronal_Running=0;}
	else {*M_isSagittal_Running=0;}
	m_isCoronal_Or_Sagittal=-1;
}

CDrawing *CAnalysisView::Select_Current_Drawing(int Selected_Drawing_Idx) 
{		
	if(Selected_Drawing_Idx==-1){//There is no matching Drawing for the MRI --> creat one. 
		CDrawing *pDraw;
		if(m_NumDrawings>=m_Drawing.GetSize()-1){m_Drawing.SetSize(m_NumDrawings+10,50);}
		m_Selected_Drawing_Idx=m_NumDrawings;
		m_NumDrawings++;
		pDraw=&m_Drawing[m_Selected_Drawing_Idx];
		pDraw->m_Depth=m_MRI[m_Selected_MRI_Idx].m_Depth; 
		Sort_Drawing_Using_Depth();
	}
	return &m_Drawing[m_Selected_Drawing_Idx];
}

void CAnalysisView::Sort_Drawing_Using_Depth()
{
	double TmpCoordinate[Max_Slices];

	//First make copies of variables
	for(int i=0; i<m_NumDrawings; i++){
		TmpCoordinate[i]=M_DrawingFileInfo[i].m_Depth;
	}

	//Sort the files in an ascending order using the number parts of the folder names,  (e.g. DR1, DR2...)
	Sort_inAscendingOrder(TmpCoordinate,m_NumDrawings,m_SortedDrawingIdx);
}

int CAnalysisView::Find_Drawing_Previous_Depth()
{
	int toReturn=-1;
	if(m_NumDrawings>0 && m_Selected_Drawing_Idx==-1)return 0;
	for(int i=0; i<m_NumDrawings; i++){
		if(m_Selected_Drawing_Idx==m_SortedDrawingIdx[i]){
			if(i==0)toReturn=m_NumDrawings-1;
			else toReturn=i-1;
			break;
		}
	}
	return toReturn;
}

int CAnalysisView::Find_Drawing_Next_Depth()
{
	int toReturn=-1;
	if(m_NumDrawings>0 && m_Selected_Drawing_Idx==-1)return 0;
	for(int i=0; i<m_NumDrawings; i++){
		if(m_Selected_Drawing_Idx==m_SortedDrawingIdx[i]){
			if(i==m_NumDrawings-1)toReturn=0;
			else toReturn=i+1;
			break;
		}
	}
	return toReturn;
}

