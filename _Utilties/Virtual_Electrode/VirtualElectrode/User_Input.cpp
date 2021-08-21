#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include "DialogueBox.h"
#include "VarDefinitions.h"
#include "Dialog_SaveFirst.h"


void CAnalysisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_isCoronal_Or_Sagittal==-1)return;

	EnterCriticalSection(&m_CriticalSection);
#define MOVE_BY 0.5
	double MoveScale;
	if(nChar==VK_CONTROL)m_CTRL_Pressed=1;
	if(m_CTRL_Pressed==1)MoveScale=0.1;else MoveScale=1;

	if(m_isCoronal_Or_Sagittal==FROM_3D && !(nChar==VK_F9 ||nChar==VK_F10 ||nChar==VK_RETURN)){
		m_NnType=-1;
	}
	
	m_KeyCode=nChar;
	switch(nChar){
		case VK_DELETE:

			if(m_isCoronal_Or_Sagittal==FROM_3D){
				//Neurons
				for(int i=0;i<m_HowManyNeurons;i++){
					if(Sq3D(m_Nn[i].m_x-(*M_ElectrodeTipX_3DAtlas), m_Nn[i].m_y-(*M_ElectrodeTipY_3DAtlas), m_Nn[i].m_z-(*M_ElectrodeTipZ_3DAtlas))<=m_DistanceFromElectrode){
						DeleteNn(i);
						Save_Neuron_Data(1,1,1);
						AfxMessageBox("Change Saved");
					}
				}
				SetEvent(M_KeyDownEvents[1]);
			}
			else{
				Delete_MarkedOnes();
				Redraw_Everything();
			}
			break;
		case VK_LEFT:
			if(m_isCoronal_Or_Sagittal==FROM_3D){
				//m_Selected_Drawing_Idx=Find_Drawing_Previous_Depth(); *M_Selected_Drawing_Idx=m_Selected_Drawing_Idx;
				//SetEvent(M_KeyDownEvents[1]);
			}
			else{
				//Move selected objects
				if(m_NumSelectedDots>0 || m_NumSelectedObjs>0 || m_NumSelectedMarkings>0){
					MoveHorizontally(-MOVE_BY*MoveScale);
					Redraw_Everything();
				}
				else{
					//if(m_NeedsToSave!=0){
					//	Dialog_SaveFirst SaveFirstDlg;
					//	if(SaveFirstDlg.DoModal()==IDOK){
					//		OnFileSaveDrawing();
					//	}
					//}
					//######  In the case the user is editing a line...
					ProperlyFinish();

					//### MRI/Drawing selection and display is done here because the main function _2nd_InteractiveMRI() works on this only when the electrode moves.
					if(m_Selected_MRI_Idx==-1)return;//There is no MRI
					if(m_Selected_MRI_Idx==0)m_Selected_MRI_Idx=m_NumMRIs-1; else m_Selected_MRI_Idx--;
					Find_a_Matching_Drawing_toMRI();
					Redraw_Everything(); 
				}
			}
			break;
		case VK_RIGHT:
			if(m_isCoronal_Or_Sagittal==FROM_3D){
				//m_Selected_Drawing_Idx=Find_Drawing_Next_Depth(); *M_Selected_Drawing_Idx=m_Selected_Drawing_Idx;
				//SetEvent(M_KeyDownEvents[1]);
			}
			else{
				//Move the selected part of the drawing
				if(m_NumSelectedDots>0 || m_NumSelectedObjs>0 || m_NumSelectedMarkings>0){
					MoveHorizontally(+MOVE_BY*MoveScale);
					Redraw_Everything();
				}
				//Go to the next MRI
				else{
					//if(m_NeedsToSave!=0){
					//	Dialog_SaveFirst SaveFirstDlg;
					//	if(SaveFirstDlg.DoModal()==IDOK){
					//		OnFileSaveDrawing();
					//	}
					//}
					//######  In the case the user is editing a line...
					ProperlyFinish();

					//### MRI/Drawing selection and display is done here because the main function _2nd_InteractiveMRI() works on this only when the electrode moves.
					if(m_Selected_MRI_Idx==-1)return;//There is no MRI
					if(m_Selected_MRI_Idx==m_NumMRIs-1)m_Selected_MRI_Idx=0; else m_Selected_MRI_Idx++;
					Find_a_Matching_Drawing_toMRI();
					Redraw_Everything();
				}
			}
			break;
		case VK_UP:
			if(m_NumSelectedDots>0 || m_NumSelectedObjs>0 || m_NumSelectedMarkings>0){
				MoveVertically(+MOVE_BY*MoveScale);
				Redraw_Everything();
			}
			else{
				Move_Elec_along_Grid_axis(Z_GRID_AXIS,  ElecAdvanceStep*MoveScale);
			}
			break;
		case VK_DOWN:
			if(m_NumSelectedDots>0 || m_NumSelectedObjs>0 || m_NumSelectedMarkings>0){
				MoveVertically(-MOVE_BY*MoveScale);
				Redraw_Everything();
			}
			else{
				Move_Elec_along_Grid_axis(Z_GRID_AXIS,  -ElecAdvanceStep*MoveScale);
			}
			break;
		case VK_ESCAPE:
			if(m_isCoronal_Or_Sagittal!=FROM_3D){
				UnMarkObjects_and_RedrawEveryting();
				ProperlyFinish();
			}
			break;
		case VK_HOME:
			//Remember the point where the "Bias to 0" was made.
			*M_InitElecBtmX=*M_ElectrodeTipX_3DAtlas; *M_InitElecBtmY=*M_ElectrodeTipY_3DAtlas; *M_InitElecBtmZ=*M_ElectrodeTipZ_3DAtlas;
			*M_Z_bias_for_Home=Rounded_to_Nearest_Int(((int)(10000*DotProduct3D(*M_Grid_Z_unit_Vector_x, *M_Grid_Z_unit_Vector_y, *M_Grid_Z_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_GridCenter_X, (*M_ElectrodeTipY_3DAtlas)-*M_GridCenter_Y, (*M_ElectrodeTipZ_3DAtlas)-*M_GridCenter_Z)))*0.001)*0.1;

			//Update the display
			Copy_ElectrodeCoordinate_toDisplay();
			*M_HeyCoronalFigure_RedrawElectrode=1;
			*M_HeySagittalFigure_RedrawElectrode=1;
			SetEvent(M_KeyDownEvents[1]);
			break;
		case VK_F1:
			Translate_Electrode(0, -0.1*MoveScale); //Along the -X axis. Along the Allocentric 3D coordinate axis (not grid coordinate)
			break;
		case VK_F2:
			Translate_Electrode(0, 0.1*MoveScale); //Along the X axis. Along the Allocentric 3D coordinate axis (not grid coordinate)
			break;
		case VK_F3:
			Translate_Electrode(1, -0.1*MoveScale); //Along the -Y axis. Along the Allocentric 3D coordinate axis (not grid coordinate)
			break;
		case VK_F4:
			Translate_Electrode(1, 0.1*MoveScale); //Along the Y axis. Along the Allocentric 3D coordinate axis (not grid coordinate)
			break;
		case VK_F5:
			Translate_Electrode(2, -0.1*MoveScale); //Along the -Z axis. Along the Allocentric 3D coordinate axis (not grid coordinate)
			break;
		case VK_F6:
			Translate_Electrode(2, 0.1*MoveScale); //Along the Z axis. Along the Allocentric 3D coordinate axis (not grid coordinate)
			break;
		case 0x31: // '1'
			Move_Elec_along_Grid_axis(X_GRID_AXIS,  MOVE_BY*MoveScale);
			break;	
		case 0x32: // '2'
			Move_Elec_along_Grid_axis(X_GRID_AXIS,  -MOVE_BY*MoveScale);
			break;	
		case 0x33: // '3'
			Move_Elec_along_Grid_axis(Y_GRID_AXIS,  -MOVE_BY*MoveScale);
			break;
		case 0x34: // '4'
			Move_Elec_along_Grid_axis(Y_GRID_AXIS,  MOVE_BY*MoveScale);
			break;
		case VK_F9:
			//#ifdef PROGRAMED_ORIGIN_n_SCALE 
			//if(m_isCoronal_Or_Sagittal==FROM_3D){
			//	if(m_NnType<m_numTextures-1)m_NnType++; else m_NnType=0;
			//	SetEvent(M_KeyDownEvents[1]);
			//}
			//#endif
			break;
		case VK_RETURN:
			//if(m_isCoronal_Or_Sagittal==FROM_3D){ //It accepts the NULL type (-1); the 3D display just uses a white texture square for this.
			//	int isThereSelectedNn=-1;
			//	for(int i=0;i<m_HowManyNeurons;i++){
			//		if(Sq3D(m_Nn[i].m_x-(*M_ElectrodeTipX_3DAtlas), m_Nn[i].m_y-(*M_ElectrodeTipY_3DAtlas), m_Nn[i].m_z-(*M_ElectrodeTipZ_3DAtlas))<=m_DistanceFromElectrode){
			//			isThereSelectedNn=i; break;
			//		}
			//	}
			//	//Edit an existing neuron
			//	if(isThereSelectedNn>=0){
			//		if(Accept_NeuronInfo(&m_Nn[isThereSelectedNn])>0){
			//			Save_Neuron_Data(1,1,1);
			//			AfxMessageBox("Neural Info Saved");
			//		}
			//	}
			//	//Create a new neuron
			//	else{
			//		m_Nn[m_HowManyNeurons].m_type=m_NnType; 
			//		m_Nn[m_HowManyNeurons].m_x=(*M_ElectrodeTipX_3DAtlas);
			//		m_Nn[m_HowManyNeurons].m_y=(*M_ElectrodeTipY_3DAtlas);
			//		m_Nn[m_HowManyNeurons].m_z=(*M_ElectrodeTipZ_3DAtlas); 
			//		if(m_HowManyNeurons>=m_Nn.GetSize()-1)m_Nn.SetSize(2*m_Nn.GetSize());
			//		if(Accept_NeuronInfo(&m_Nn[m_HowManyNeurons])>0){
			//			m_HowManyNeurons++; 
			//			Save_Neuron_Data(1,1,1);
			//			AfxMessageBox("Neural Info Saved");
			//		}
			//	}
			//	m_NnType=-1; //Disable the "temporary rectangel display"
			//	SetEvent(M_KeyDownEvents[1]);
			//}
			break;
		default: break;
	}
	LeaveCriticalSection(&m_CriticalSection);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

//Treat them as private.
void CAnalysisView::InformOtherProgramsToUpdate()
{
	Copy_ElectrodeCoordinate_toDisplay();
	*M_HeyCoronalFigure_RedrawElectrode=1;
	*M_HeySagittalFigure_RedrawElectrode=1;
}

void CAnalysisView::ProperlyFinish()
{
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
	m_isToEditObj=0;
	if(m_UnderConstructionObjID>-1){
		//There isn't enough verticies to finish the object => Abondon it.
		if(pDraw->m_LinedObj[m_UnderConstructionObjID].m_NumVertices<2){
			pDraw->m_LinedObj.RemoveAt(m_UnderConstructionObjID);
			pDraw->m_NumLinedObj--;
		}

		m_UnderConstructionObjID=-1;
		Redraw_Everything();
	}
}

void CAnalysisView::Find_MRI_Closest_To_CoronalSection_ToElectrodeTip()
{
	//Find the closest slice from the tip of the electrode.
	double cloesestDist=INT_MAX;
	int SliceClosestToElectrodeTip=0;
	for(int i=0;i<m_NumMRIs;i++){
		if(DISTANCE_AB_1D(m_MRI[i].m_Depth,(*M_ElectrodeTipY_3DAtlas))<cloesestDist){
			cloesestDist=DISTANCE_AB_1D(m_MRI[i].m_Depth,(*M_ElectrodeTipY_3DAtlas));
			SliceClosestToElectrodeTip=i;
		}
	}
	m_Selected_MRI_Idx=SliceClosestToElectrodeTip;
}


void CAnalysisView::Find_a_Matching_Drawing_toMRI()
{
	double ImageDepth=m_MRI[m_Selected_MRI_Idx].m_Depth;
	for(int i=0;i<m_NumDrawings;i++){
		if(m_Drawing[i].m_Depth==ImageDepth){
			m_Selected_Drawing_Idx=i;
			return;
		}
	}
	m_Selected_Drawing_Idx=-1;
}

void CAnalysisView::Find_SliceClosest_SagittalSection_ToElectrodeTip()
{
	//Find the closest slice from the tip of the electrode.
	double cloesestDist=INT_MAX;
	int SliceClosestToElectrodeTip=0;
	for(int i=0;i<m_NumMRIs;i++){
		if(DISTANCE_AB_1D(m_MRI[i].m_Depth, (*M_ElectrodeTipX_3DAtlas))<cloesestDist){
			cloesestDist=DISTANCE_AB_1D(m_MRI[i].m_Depth, (*M_ElectrodeTipX_3DAtlas));
			SliceClosestToElectrodeTip=i;
		}
	}
	m_Selected_MRI_Idx=SliceClosestToElectrodeTip;
}

void CAnalysisView::MoveHorizontally(double horizontal)
{
	int i;
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
    if(m_isCoronal_Or_Sagittal==CORONAL )for(i=0; i<m_NumSelectedMarkings; i++)M_MarkingCORONAL[m_SelectedMarkings[i]].m_x+=horizontal;
    else                                 for(i=0; i<m_NumSelectedMarkings; i++)M_MarkingSAGITTAL[m_SelectedMarkings[i]].m_y+=horizontal;
    if(m_isCoronal_Or_Sagittal==CORONAL )for(i=0; i<m_NumSelectedDots; i++)pDraw->m_Dot[m_SelectedDots[i]].m_x+=horizontal;
    else                                 for(i=0; i<m_NumSelectedDots; i++)pDraw->m_Dot[m_SelectedDots[i]].m_y+=horizontal;

	if(m_isCoronal_Or_Sagittal==CORONAL )for(i=0; i<m_NumSelectedObjs;i++)pDraw->m_LinedObj[m_SelectedObjs[i]].Translate_Horizontally_CORONAL(horizontal);
	else                                 for(i=0; i<m_NumSelectedObjs;i++)pDraw->m_LinedObj[m_SelectedObjs[i]].Translate_Horizontally_SAGITTAL(horizontal);
	m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CAnalysisView::MoveVertically(double vertical)
{
	int i;
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);

	if(m_isCoronal_Or_Sagittal==CORONAL )for(i=0; i<m_NumSelectedMarkings; i++)M_MarkingCORONAL[m_SelectedMarkings[i]].m_z+=vertical;
	else                                 for(i=0; i<m_NumSelectedMarkings; i++)M_MarkingSAGITTAL[m_SelectedMarkings[i]].m_z+=vertical;
	for(i=0; i<m_NumSelectedDots; i++)pDraw->m_Dot[m_SelectedDots[i]].m_z+=vertical;
	for(i=0; i<m_NumSelectedObjs;i++)pDraw->m_LinedObj[m_SelectedObjs[i]].TranslateVertically(vertical);
	m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CAnalysisView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	m_KeyCode=-1;
	if(nChar==VK_CONTROL)m_CTRL_Pressed=0;
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


#include "Dialog_NeuronInfo.h"
int CAnalysisView::Accept_NeuronInfo(CNeuron *pNn)
{
	int NeedsToSave=0;
	CNeuronInfo NnInfo;

	NnInfo.m_NeuronName=pNn->m_NnName;
	NnInfo.m_Type=pNn->m_type;
	NnInfo.m_Note=pNn->m_Note;
	NnInfo.m_Variable1=pNn->m_AntidromicStimulatedFrom; // ==> "#define m_AntidromicStimulatedFrom m_NnVariables[0]"
	NnInfo.m_Variable2=pNn->m_MarkingLesion;	// ==> "#define m_MarkingLesion m_NnVariables[1]"
	if(NnInfo.DoModal()==IDOK){
		if(pNn->m_NnName                  !=NnInfo.m_NeuronName){pNn->m_NnName=NnInfo.m_NeuronName;NeedsToSave=1;}
		if(pNn->m_type                    !=NnInfo.m_Type){pNn->m_type=NnInfo.m_Type;NeedsToSave=1;}
		if(pNn->m_Note                    !=NnInfo.m_Note){pNn->m_Note=NnInfo.m_Note;NeedsToSave=1;}
		if(pNn->m_AntidromicStimulatedFrom!=NnInfo.m_Variable1){pNn->m_AntidromicStimulatedFrom=NnInfo.m_Variable1;NeedsToSave=1;}
		if(pNn->m_MarkingLesion           !=NnInfo.m_Variable2){pNn->m_MarkingLesion=NnInfo.m_Variable2;NeedsToSave=1;}
	}
	return NeedsToSave;
}

void CAnalysisView::Edit_NnInfo_from2D(double x, double y, double z)
{
	int isThereSelectedNn=-1;
	double MinDistance=1, Dist;

	for(int i=0;i<m_HowManyNeurons;i++){
		Dist=M3D(m_Nn[i].m_x-(x), m_Nn[i].m_y-(y), m_Nn[i].m_z-(z));
		if(Dist<=m_DistanceFromElectrode*20 && Dist<MinDistance){
			isThereSelectedNn=i; MinDistance=Dist;
		}
	}
	//Edit an existing neuron
	if(isThereSelectedNn!=-1){
		if(Accept_NeuronInfo(&m_Nn[isThereSelectedNn])==1){
			Save_Neuron_Data(1,1,1);
			Redraw_Everything(); 
			AfxMessageBox("Neural Info Updated");
		}
	}
	else return; //Creation of a new neuron is done at the 3D enviornment only.

	m_NnType=-1; //Disable the "temporary display"
	SetEvent(M_KeyDownEvents[1]);
}

void CAnalysisView::OnChangeParameters() 
{
	CDialogueBox	ParameterBOX;

	ParameterBOX.m_dVariable1=m_dMaximumX;
	ParameterBOX.m_dVariable2=m_dMinimumX;
	ParameterBOX.m_dVariable3=m_dMaximumY;
	ParameterBOX.m_dMinimumY=m_dMinimumY;
	if(ParameterBOX.DoModal()==IDOK){
		if(ParameterBOX.m_dVariable1<=ParameterBOX.m_dVariable2||ParameterBOX.m_dVariable3<=ParameterBOX.m_dMinimumY){AfxMessageBox("m_dMaximumX<=m_dMinimumX OR m_dMaximumY<=m_dMinimumY ==> Try again", MB_OK); return;}
		else{
			m_dMaximumX=ParameterBOX.m_dVariable1;
			m_dMinimumX=ParameterBOX.m_dVariable2;
			m_dMaximumY=ParameterBOX.m_dVariable3;
			m_dMinimumY=ParameterBOX.m_dMinimumY;
		}
	}
}

//This routine extracts numbers from a string. 
//There could be many isolated numbers scattered among letters, but it registers only the last one (number e.g. 123 from 76DR123.txt).
int CAnalysisView::Find_Last_Integer_inString(char *GivenString)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1,foundNum;
	int num = 0, sign=0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			num=0;//This one is here to make sure that the only the last group of digits are captured (e.g. 123 from 76DR123.txt).
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				num = num + (*q-48) * pow((double)10,(double)iCount);
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			if(*p=='-')sign=1; else sign=0;
			++p ;
		}
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return INT_MAX;
	}
	if(sign==1) return -num;
	else		return  num;
}

#include "PitchRollYaw.h"
void CAnalysisView::OnPitchRollYaw()
{
	if(*M_isCoronal_Running==0 && *M_isSagittal_Running==0 && *M_is3D_Running==0){AfxMessageBox("Hello there:\n Run Coronal/Sagittal/3D program first."); return;}

	CPitchRollYaw ConfigDialog;
	ConfigDialog.m_GridPitch=*M_Grid_Pitch;
	ConfigDialog.m_GridRoll=*M_Grid_Roll;
	ConfigDialog.m_GridYaw=*M_Grid_Yaw;
	if(ConfigDialog.DoModal()==IDOK){
		*M_Grid_Pitch=ConfigDialog.m_GridPitch;
		*M_Grid_Roll =ConfigDialog.m_GridRoll;
		*M_Grid_Yaw	 =ConfigDialog.m_GridYaw;
		Calculate_Electrode_and_Grid_Coordinates();
		Save_3D_2D_Config();
		*M_HeyCoronalFigure_RedrawElectrode=1;*M_HeySagittalFigure_RedrawElectrode=1;//Just to trigger the initiation of drawing.
	}
}

//Set the distance range from the electrode tip within which the drawing objects "glow"
#include "Dialog_InputOneNum.h"
void CAnalysisView::OnView_DistanceFromElectrode()
{
	Dialog_InputOneNum	ParameterBOX;

	ParameterBOX.m_GiveOneNum=m_DistanceFromElectrode;
	if(ParameterBOX.DoModal()==IDOK){
		m_DistanceFromElectrode=ParameterBOX.m_GiveOneNum;
	}
}


