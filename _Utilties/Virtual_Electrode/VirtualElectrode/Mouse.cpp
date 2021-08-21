#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"


void CAnalysisView::OnRButtonDown(UINT nFlags, CPoint point)
{

	if(m_UnderConstructionObjID==-1 && m_Selected_MRI_Idx!=-1){
		double dataX, dataVertical;
		CFigure *pFig=&m_Fig[0][0][0];
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);
		m_CurrentlyEditedObjIdx=LineHitTest(dataX,dataVertical,pFig);
	}
	CView::OnRButtonDown(nFlags, point);
}

void CAnalysisView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){
		m_Saved_Left_MousePointFor3DX=NO_3DMOVE; m_Saved_Left_MousePointFor3DY=NO_3DMOVE;
		m_Saved_Right_MousePointFor3DX=NO_3DMOVE; m_Saved_Right_MousePointFor3DY=NO_3DMOVE;
	}
	//The user dropped the previously grabbed objets.
	else if((m_GrabbedObjectIdx>-1 || m_GrabbedDotIdx>-1|| m_GrabbedMarkingIdx>-1) && m_Selected_MRI_Idx!=-1){
		m_GrabbedObjectIdx=-1;m_GrabbedDotIdx=-1;m_GrabbedMarkingIdx=-1;
		CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
		double dataX, dataVertical;
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);
		int i,idx;
		if(m_isCoronal_Or_Sagittal==CORONAL)for(i=0; i<m_NumSelectedMarkings;i++){idx=m_SelectedMarkings[i];M_MarkingCORONAL[idx].m_x+=dataX-m_SelectedAtX;M_MarkingCORONAL[idx].m_z+=dataVertical-m_SelectedAtVertical;}
		else                                for(i=0; i<m_NumSelectedMarkings;i++){idx=m_SelectedMarkings[i];M_MarkingSAGITTAL[idx].m_y+=dataX-m_SelectedAtX;M_MarkingSAGITTAL[idx].m_z+=dataVertical-m_SelectedAtVertical;}                                
		if(m_isCoronal_Or_Sagittal==CORONAL)for(i=0; i<m_NumSelectedDots;i++){idx=m_SelectedDots[i];pDraw->m_Dot[idx].m_x+=dataX-m_SelectedAtX;pDraw->m_Dot[idx].m_z+=dataVertical-m_SelectedAtVertical;}
		else                                for(i=0; i<m_NumSelectedDots;i++){idx=m_SelectedDots[i];pDraw->m_Dot[idx].m_y+=dataX-m_SelectedAtX;pDraw->m_Dot[idx].m_z+=dataVertical-m_SelectedAtVertical;}                                
		m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
		if(m_isCoronal_Or_Sagittal==CORONAL)for(i=0; i<m_NumSelectedObjs;i++){idx=m_SelectedObjs[i];pDraw->m_LinedObj[idx].Translate_In2D_CORONAL(dataX-m_SelectedAtX, dataVertical-m_SelectedAtVertical);}
		else                                for(i=0; i<m_NumSelectedObjs;i++){idx=m_SelectedObjs[i];pDraw->m_LinedObj[idx].Translate_In2D_SAGITTAL(dataX-m_SelectedAtX, dataVertical-m_SelectedAtVertical);}
		Redraw_Everything();
	}
	else if(m_CurrentlyEditedObjIdx>-1 && m_GrabedVertex>-1){
		CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
		CLinedObj* pObj=&pDraw->m_LinedObj[m_CurrentlyEditedObjIdx];
		double dataX, dataVertical;
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);
		if(m_isCoronal_Or_Sagittal==CORONAL)pObj->m_Vtx[m_GrabedVertex].m_x=dataX;
		else                                pObj->m_Vtx[m_GrabedVertex].m_y=dataX;
		pObj->m_Vtx[m_GrabedVertex  ].m_z=dataVertical;
		m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
		pObj->UpdateBoundary(m_isCoronal_Or_Sagittal);
		m_GrabedVertex=-1;
		Redraw_Everything();
	}
	CView::OnLButtonUp(nFlags, point);
}

void CAnalysisView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){
		if(((nFlags & MK_LBUTTON)==MK_LBUTTON) && ((nFlags & MK_CONTROL)!=MK_CONTROL)){ 
			if(m_Saved_Left_MousePointFor3DX==NO_3DMOVE){
				return;
			}
			else{//3D - Rotation around the vertical axis, and translation along the vertical axis
				//Find the axis along which the sphere grabed by the mouse is turning.
				m_yAngle+=0.1*(point.x-m_Saved_Left_MousePointFor3DX);
				m_Saved_Left_MousePointFor3DX=point.x;
				m_yTranslate+=0.01*(m_Saved_Left_MousePointFor3DY-point.y);
				m_Saved_Left_MousePointFor3DY=point.y;
				SetEvent(M_KeyDownEvents[1]);
			}
		}
		else if(((nFlags & MK_LBUTTON)==1) && ((nFlags & MK_CONTROL)==MK_CONTROL)){
			if(m_Saved_Right_MousePointFor3DX==NO_3DMOVE){
				return;
			}
			else{//3D -Translation (along the horizontal, depth, axes)
				//Find the axis along which the sphere grabed by the mouse is turning.
				m_xTranslate+=0.01*(point.x-m_Saved_Right_MousePointFor3DX);
				m_Saved_Right_MousePointFor3DX=point.x;
				m_zTranslate+=0.01*(point.y-m_Saved_Right_MousePointFor3DY);
				m_Saved_Right_MousePointFor3DY=point.y;
				SetEvent(M_KeyDownEvents[1]);
			}
		}
	}
	else if(m_DrawingLoaded==1 && m_Selected_MRI_Idx!=-1){//2D
		double dataX, dataVertical;
		CFigure *pFig=&m_Fig[0][0][0];
		CClientDC objDC(this);
		int i,idx;
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);
		if(dataX>=pFig->m_dMinX && dataVertical>=pFig->m_dMinY && dataX<=pFig->m_dMaxX && dataVertical<=pFig->m_dMaxY){
			//Constructing an object
			if(m_UnderConstructionObjID>-1){
				CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
				CClientDC objDC(this);				objDC.SetROP2(R2_NOTXORPEN);
				objDC.SelectObject(m_rgbPEN1[m_RGBID]);
				int scrnX, scrnY;
				if(m_isCoronal_Or_Sagittal==CORONAL )m_Fig[0][0][0].FromDataToScreenXY(pDraw->m_LinedObj[m_UnderConstructionObjID].m_Vtx[pDraw->m_LinedObj[m_UnderConstructionObjID].m_NumVertices-1].m_x,pDraw->m_LinedObj[m_UnderConstructionObjID].m_Vtx[pDraw->m_LinedObj[m_UnderConstructionObjID].m_NumVertices-1].m_z, scrnX, scrnY);
				if(m_isCoronal_Or_Sagittal==SAGITTAL)m_Fig[0][0][0].FromDataToScreenXY(pDraw->m_LinedObj[m_UnderConstructionObjID].m_Vtx[pDraw->m_LinedObj[m_UnderConstructionObjID].m_NumVertices-1].m_y,pDraw->m_LinedObj[m_UnderConstructionObjID].m_Vtx[pDraw->m_LinedObj[m_UnderConstructionObjID].m_NumVertices-1].m_z, scrnX, scrnY);
				//Delete previous line: XOR on the previous line
				if(m_savedX!=-1 && m_savedY!=-1 && m_DrewTmpLine==1){
					objDC.MoveTo(scrnX, scrnY); objDC.LineTo(m_savedX,m_savedY);
					m_DrewTmpLine=0;
				}
				//Draw a new line
				objDC.MoveTo(scrnX, scrnY); objDC.LineTo(point.x,point.y);
				m_savedX=point.x, m_savedY=point.y;
				objDC.SetROP2(R2_COPYPEN);
				m_DrewTmpLine=1;
			}
			//Move an object
			else if((m_GrabbedObjectIdx>-1 || m_GrabbedDotIdx>-1 || m_GrabbedMarkingIdx>-1) && (nFlags & MK_LBUTTON)==MK_LBUTTON){
				Highlight_MarkedOnes_withTranslation(dataX-m_SelectedAtX, dataVertical-m_SelectedAtVertical);
				//for(i=0; i<m_NumSelectedObjs;i++){
				//	idx=m_SelectedObjs[i];
				//	m_LinedObj[idx].Draw_Obj_RelativeToXY(dataX-m_SelectedAtX, dataVertical-m_SelectedAtVertical, pFig, &m_DC[0], &objDC, m_rgbPEN1);
				//}
			}
			else if(m_CurrentlyEditedObjIdx>-1){
				if(m_GrabedVertex>-1){
					CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
					CLinedObj* pObj=&pDraw->m_LinedObj[m_CurrentlyEditedObjIdx];
					double horizontalData_1,horizontalData0, horizontalData1;
					if(m_isCoronal_Or_Sagittal==CORONAL ){horizontalData0=pObj->m_Vtx[m_GrabedVertex].m_x;}else{horizontalData0=pObj->m_Vtx[m_GrabedVertex].m_y;}
					if(m_GrabedVertex==0){
						if(m_isCoronal_Or_Sagittal==CORONAL ){horizontalData1=pObj->m_Vtx[m_GrabedVertex+1].m_x; }else{horizontalData1=pObj->m_Vtx[m_GrabedVertex+1].m_y;}
						pFig->MyMoveTo(horizontalData0+dataX-m_SelectedAtX, pObj->m_Vtx[m_GrabedVertex  ].m_z+dataVertical-m_SelectedAtVertical, &objDC);
						pFig->MyLineTo(horizontalData1, pObj->m_Vtx[m_GrabedVertex+1].m_z, &objDC);
					}
					else if(m_GrabedVertex==pObj->m_NumVertices-1){
						if(m_isCoronal_Or_Sagittal==CORONAL ){horizontalData_1=pObj->m_Vtx[m_GrabedVertex-1].m_x; }else{horizontalData_1=pObj->m_Vtx[m_GrabedVertex-1].m_y;}
						pFig->MyMoveTo(horizontalData_1, pObj->m_Vtx[m_GrabedVertex-1].m_z, &objDC); 
						pFig->MyLineTo(horizontalData0+dataX-m_SelectedAtX, pObj->m_Vtx[m_GrabedVertex  ].m_z+dataVertical-m_SelectedAtVertical, &objDC);
					}
					else{
						if(m_isCoronal_Or_Sagittal==CORONAL ){horizontalData_1=pObj->m_Vtx[m_GrabedVertex-1].m_x; horizontalData1=pObj->m_Vtx[m_GrabedVertex+1].m_x; }else{horizontalData_1=pObj->m_Vtx[m_GrabedVertex-1].m_y;horizontalData1=pObj->m_Vtx[m_GrabedVertex+1].m_y;}
						pFig->MyMoveTo(horizontalData_1, pObj->m_Vtx[m_GrabedVertex-1].m_z, &objDC); 
						pFig->MyLineTo(horizontalData0+dataX-m_SelectedAtX, pObj->m_Vtx[m_GrabedVertex  ].m_z+dataVertical-m_SelectedAtVertical, &objDC);
						pFig->MyLineTo(horizontalData1, pObj->m_Vtx[m_GrabedVertex+1].m_z, &objDC);
					}					
				}
			}
			else{
				m_savedX=-1, m_savedY=-1;
			}
		}
		else if(m_UnderConstructionObjID>-1){
			ProperlyFinish();
		}
		//Measuring the Ordinate
		else if(((nFlags & MK_LBUTTON)==MK_LBUTTON) && dataX<pFig->m_dMinX && (dataVertical>=pFig->m_dMinY && dataVertical<=pFig->m_dMaxY)){
			pFig->RefreshFrame(&objDC, &m_DC[0]);
			objDC.SelectObject(m_BlueGRAY);
			char CoordinateMsg[64];
			sprintf(CoordinateMsg,"pixel: %0.2lf (3D: %0.2lf)",((dataVertical-pFig->m_dMinY)/m_MRI_Pixel_Dimension_In_mm), dataVertical);
			pFig->PutText(pFig->m_dMinX,dataVertical,CoordinateMsg,&objDC);
			pFig->LineAtoB(pFig->m_dMinX,dataVertical,pFig->m_dMaxX,dataVertical,&objDC);
		}
		//Measuring the abscissa
		else if(((nFlags & MK_LBUTTON)==MK_LBUTTON) && (dataVertical<pFig->m_dMinY || dataVertical>pFig->m_dMaxY) && (dataX>=pFig->m_dMinX && dataX<=pFig->m_dMaxX)){
			pFig->RefreshFrame(&objDC, &m_DC[0]);
			objDC.SelectObject(m_BlueGRAY);
			char CoordinateMsg[64];
			sprintf(CoordinateMsg,"pixel: %0.2lf (3D: %0.2lf)",((dataX-pFig->m_dMinX)/m_MRI_Pixel_Dimension_In_mm), dataX);
			pFig->PutText(dataX,pFig->m_dMaxY,CoordinateMsg,&objDC);
			pFig->LineAtoB(dataX,pFig->m_dMinY,dataX,pFig->m_dMaxY,&objDC);
		}
	}

	CView::OnMouseMove(nFlags, point);
}

BOOL CAnalysisView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_isCoronal_Or_Sagittal==FROM_3D){
		if((nFlags & MK_CONTROL)==MK_CONTROL){
			m_xScale3D+=0.001*zDelta, m_yScale3D+=0.001*zDelta, m_zScale3D+=0.001*zDelta;
			SetEvent(M_KeyDownEvents[1]);
		}
	}
	else if(m_Selected_MRI_Idx!=-1){
		if((nFlags & MK_CONTROL)==MK_CONTROL){
			CFigure *pFig=&m_Fig[0][0][0];
			m_Magnification2D+=0.002*zDelta;
			double AnchorX_in3D, AnchorZ_in3D;
			CRect rClient;
			GetClientRect(&rClient);
			ClientToScreen(&rClient);
			FromScrnToDataCoordinate(pt.x-rClient.left, pt.y-rClient.top, AnchorX_in3D, AnchorZ_in3D, &m_Fig[0][0][0]);
			pFig->m_dMinX= AnchorX_in3D - (AnchorX_in3D-pFig->m_dMinX)*(m_PrevMag2D/m_Magnification2D);
			pFig->m_dMaxX= AnchorX_in3D + (pFig->m_dMaxX-AnchorX_in3D)*(m_PrevMag2D/m_Magnification2D);
			pFig->m_dMinY= AnchorZ_in3D - (AnchorZ_in3D-pFig->m_dMinY)*(m_PrevMag2D/m_Magnification2D);
			pFig->m_dMaxY= AnchorZ_in3D + (pFig->m_dMaxY-AnchorZ_in3D)*(m_PrevMag2D/m_Magnification2D);
			pFig->Prepare4FigFrame(); 
			pFig->Frame_Graph2D(&m_DC[0],1);
			Redraw_Everything(); 
			m_PrevMag2D=m_Magnification2D;
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CAnalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC objDC(this);
	double dataX, dataVertical;

	if(m_isCoronal_Or_Sagittal==FROM_3D){
		if(((nFlags & MK_LBUTTON)==MK_LBUTTON) && ((nFlags & MK_CONTROL)!=MK_CONTROL)){ 
			if(m_Saved_Left_MousePointFor3DX==NO_3DMOVE){
				m_Saved_Left_MousePointFor3DX=point.x; 
				m_Saved_Left_MousePointFor3DY=point.y; 
			}
			else{
				return;
			}
		}
		else if(((nFlags & MK_LBUTTON)==1) && ((nFlags & MK_CONTROL)==MK_CONTROL)){
			if(m_Saved_Right_MousePointFor3DX==NO_3DMOVE){
				m_Saved_Right_MousePointFor3DX=point.x; 
				m_Saved_Right_MousePointFor3DY=point.y; 
			}
			else{
				return;
			}
		}
	}
	else if(m_Selected_MRI_Idx!=-1){
		//m_selPaneID=WhichFigure(point);
		m_selPaneID=0;
		CFigure *pFig=&m_Fig[m_selPaneID][0][0]; 
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);

		//######## Electrode Position User Input (X, Y, Z)
		if((point.x>=pFig->m_Title.StringRect.left && point.x<=pFig->m_Title.StringRect.right)&&(point.y>=pFig->m_Title.StringRect.top && point.y<=pFig->m_Title.StringRect.bottom)){
			OnUserInput_of_Electrode_Position();
			return;
		}

		//######## Register the Origin of the 3D in MRI
		if(m_toRegisterOrign==1){
			m_OriginOfAbscissa_PixelCoordinate=(dataX   -m_MinX_corner)/m_MRI_Pixel_Dimension_In_mm;
			m_OriginOfOrdinate_PixelCoordinate=(dataVertical-m_MinZ_corner)/m_MRI_Pixel_Dimension_In_mm;
			Init_Figure_AFTER_MRI_loading();
			m_toRegisterOrign=0;
			m_ShowCrosshairOnce=1;
			Save_3D_2D_Config();
			Redraw_Everything();
			return;
		}
		
		//######## Register the Grid Origin of the 3D in MRI
		if(m_toRegisterGridOrign==1){
			if(m_isCoronal_Or_Sagittal==CORONAL ){
				*M_GridCenter_X=dataX   ;
				*M_GridCenter_Z=dataVertical;
				*M_GridCenter_Y=m_MRI[m_Selected_MRI_Idx].m_Depth;
			}
			if(m_isCoronal_Or_Sagittal==SAGITTAL){
				*M_GridCenter_Y=dataX  ;
				*M_GridCenter_Z=dataVertical;
				*M_GridCenter_X=m_MRI[m_Selected_MRI_Idx].m_Depth;
			}
			
			//Move the electrode just to give the user feedback.
			(*M_ElectrodeTopX_3DAtlas)=*M_GridCenter_X;	(*M_ElectrodeTopY_3DAtlas)=*M_GridCenter_Y;	(*M_ElectrodeTopZ_3DAtlas)=*M_GridCenter_Z;	(*M_ElectrodeTipX_3DAtlas)=*M_GridCenter_X;	(*M_ElectrodeTipY_3DAtlas)=*M_GridCenter_Y;	(*M_ElectrodeTipZ_3DAtlas)=*M_GridCenter_Z;
			Redraw_Everything();
			Sleep(100);//Just for feedback for user

			OnPitchRollYaw(); //This one triggers Redraw_Everything()
			m_toRegisterGridOrign=0;
			return;
		}

		//############################## Inside the pane
		int hitObjIdx=-1, hitPointIdx=-1, hitMarkingIdx;
		if(dataX>=pFig->m_dMinX && dataVertical>=pFig->m_dMinY && dataX<=pFig->m_dMaxX && dataVertical<=pFig->m_dMaxY){
			CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
			CRectTracker tracker; //The "rubberband" instance!!
			//Already editing an object?
			if(m_isToEditObj==1 && m_CurrentlyEditedObjIdx>-1){
				CLinedObj* pObj=&m_Drawing[m_Selected_Drawing_Idx].m_LinedObj[m_CurrentlyEditedObjIdx];
				m_GrabedVertex=pObj->PointHitTest(dataX, dataVertical,pFig, m_isCoronal_Or_Sagittal);
				//No vertex is hit
				if(m_GrabedVertex<0){
					//See if the user has hit a line segment of the chosen object.
					m_GrabedVertex=pObj->LineHitTest_AND_Insert_Vertex(dataX,dataVertical,m_MRI[m_Selected_MRI_Idx].m_Depth,pFig, m_isCoronal_Or_Sagittal);
					if(m_GrabedVertex<0){//If no line-segment has been hit, end the object-editing mode.
						m_isToEditObj=0;
						Redraw_Everything();
					}
				}
			}
			//Editing an object!
			if(m_isToEditObj==1 && m_CurrentlyEditedObjIdx>-1 && m_GrabedVertex>-1){
				m_SelectedAtX=dataX; m_SelectedAtVertical=dataVertical;
			}
			//Selecting objects
			else if(m_EditMode==SELECT_OBJ){
				//Selecting a lined object or inserting a vertex to an existing line object
				if(m_UnderConstructionObjID==-1 && ( (hitObjIdx=LineHitTest(dataX,dataVertical,pFig))>-1) || (hitPointIdx=PointHitTest(dataX,dataVertical,pFig))>-1 || (hitMarkingIdx=MarkingHitTest(dataX,dataVertical,pFig))>-1){// The insertion of a vertex to an existing line is done only when the latest vertex is NOT an "active" LINE_TYPE.
					//Select an object
					if(hitObjIdx>-1){
						if((CheckIfThisObjIsSelected(hitObjIdx)<0) && (m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0) && (nFlags & MK_ALT)!=MK_ALT){UnMarkObjects_and_RedrawEveryting(); }
						m_GrabbedObjectIdx=hitObjIdx;
						RegisterObjForSelection(m_GrabbedObjectIdx);
					}
					//Selected a point
					else if(hitPointIdx>-1){
						if((CheckIfThisPointIsSelected(hitPointIdx)<0) && (m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0) && (nFlags & MK_ALT)!=MK_ALT){UnMarkObjects_and_RedrawEveryting(); }
						m_GrabbedDotIdx=hitPointIdx;
						RegisterDotForSelection(m_GrabbedDotIdx);
					}
					//Selected a Marking
					else if(hitMarkingIdx>-1){
						if((CheckIfThisMarkingIsSelected(hitMarkingIdx)<0) && (m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0) && (nFlags & MK_ALT)!=MK_ALT){UnMarkObjects_and_RedrawEveryting(); }
						m_GrabbedMarkingIdx=hitMarkingIdx;
						RegisterMarkingForSelection(m_GrabbedMarkingIdx);
					}
					m_SelectedAtX=dataX, m_SelectedAtVertical=dataVertical;
					Highlight_MarkedOnes();
				}
				//Capturing objects with a net
				else if(tracker.TrackRubberBand(this, point, TRUE)){//This function will capture the mouse until the user releases the left mouse button, presses the ESC key, or presses the right mouse button. As the user moves the mouse cursor, the feedback is updated by calling DrawTrackerRect and OnChangedRect.
					if((m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0) && (nFlags & MK_ALT)!=MK_ALT){UnMarkObjects_and_RedrawEveryting(); }
					// The follwoing happens AFTER the rubber band is released.
					tracker.m_rect.NormalizeRect();
					m_CapturRec.top=tracker.m_rect.top;
					m_CapturRec.left=tracker.m_rect.left;
					m_CapturRec.right=tracker.m_rect.right;
					m_CapturRec.bottom=tracker.m_rect.bottom;
			
					pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
					pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
					m_CapturRect_Left=pFig->FromScreenToDataX(m_CapturRec.left); 
					m_CapturRect_Top=pFig->FromScreenToDataY(m_CapturRec.top);
					m_CapturRect_Right=pFig->FromScreenToDataX(m_CapturRec.right); 
					m_CapturRect_Bottom=pFig->FromScreenToDataY(m_CapturRec.bottom);
					Include_test();
					Redraw_Everything();
				}
				//Clicked a blank
				else{
					if((m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0) && (nFlags & MK_ALT)!=MK_ALT){UnMarkObjects_and_RedrawEveryting(); }
				}
			}
			//DRAWING
			// Line 
			else if(m_TYPE==LINE_TYPE){
				if(m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0){UnMarkObjects_and_RedrawEveryting();}
				m_DC[0].SelectObject(m_rgbPEN1[m_RGBID]); objDC.SelectObject(m_rgbPEN1[m_RGBID]); pFig->Circle(dataX,dataVertical,2,&m_DC[0]);pFig->Circle(dataX,dataVertical,2,&objDC);
				//Create a new Line object
				if(m_UnderConstructionObjID==-1){
					if(pDraw->m_NumLinedObj>=pDraw->m_LinedObj.GetSize()-1)pDraw->m_LinedObj.SetSize(2*(pDraw->m_LinedObj.GetSize()+1));
					pDraw->m_LinedObj[pDraw->m_NumLinedObj].InitObj();
					if(m_isCoronal_Or_Sagittal==CORONAL )pDraw->m_LinedObj[pDraw->m_NumLinedObj].Add_VertexIn3D(dataX, m_MRI[m_Selected_MRI_Idx].m_Depth, dataVertical, m_RGBID, m_isCoronal_Or_Sagittal);
					if(m_isCoronal_Or_Sagittal==SAGITTAL)pDraw->m_LinedObj[pDraw->m_NumLinedObj].Add_VertexIn3D(m_MRI[m_Selected_MRI_Idx].m_Depth, dataX,	dataVertical, m_RGBID, m_isCoronal_Or_Sagittal);
					m_UnderConstructionObjID=pDraw->m_NumLinedObj;
					pDraw->m_NumLinedObj++;
				}
				//Add to an existing line object.
				else{
					if(m_isCoronal_Or_Sagittal==CORONAL )pDraw->m_LinedObj[m_UnderConstructionObjID].Add_VertexIn3D(dataX, m_MRI[m_Selected_MRI_Idx].m_Depth, dataVertical,  m_RGBID, m_isCoronal_Or_Sagittal);
					if(m_isCoronal_Or_Sagittal==SAGITTAL)pDraw->m_LinedObj[m_UnderConstructionObjID].Add_VertexIn3D(m_MRI[m_Selected_MRI_Idx].m_Depth, dataX,	dataVertical,  m_RGBID, m_isCoronal_Or_Sagittal);
					pDraw->m_LinedObj[m_UnderConstructionObjID].Draw_lastLineSegment(pFig, &m_DC[0], &objDC, m_rgbPEN1, m_isCoronal_Or_Sagittal);
				}
			}					
			//Dot
			else if(m_TYPE==DOT_TYPE){
				m_UnderConstructionObjID=-1;
				m_DC[0].SelectObject(m_rgbPEN1[m_RGBID]); objDC.SelectObject(m_rgbPEN1[m_RGBID]); pFig->Circle(dataX,dataVertical,2,&m_DC[0]);pFig->Circle(dataX,dataVertical,2,&objDC);
				if(m_isCoronal_Or_Sagittal==CORONAL )Add_VertexIn3D(dataX, m_MRI[m_Selected_MRI_Idx].m_Depth, dataVertical, m_RGBID, m_TYPE, 0, pDraw->m_NumDots);
				if(m_isCoronal_Or_Sagittal==SAGITTAL)Add_VertexIn3D(m_MRI[m_Selected_MRI_Idx].m_Depth, dataX, dataVertical, m_RGBID, m_TYPE, 0, pDraw->m_NumDots);
			}
			//Marking
			else{
				m_UnderConstructionObjID=-1;
				m_DC[0].SelectObject(m_rgbPEN1[m_RGBID]); objDC.SelectObject(m_rgbPEN1[m_RGBID]); pFig->Circle(dataX,dataVertical,4,&m_DC[0]);pFig->Circle(dataX,dataVertical,4,&objDC);
				if(m_isCoronal_Or_Sagittal==CORONAL )Add_MarkingIn3D(dataX, m_MRI[m_Selected_MRI_Idx].m_Depth, dataVertical, m_RGBID, m_TYPE, *M_NumberOfMarkingsCORONAL);
				if(m_isCoronal_Or_Sagittal==SAGITTAL)Add_MarkingIn3D(m_MRI[m_Selected_MRI_Idx].m_Depth, dataX, dataVertical, m_RGBID, m_TYPE, *M_NumberOfMarkingsSAGITTAL);
			}
		}
		//############################## Outside the pane
		//Color Palette 
		else if(dataX>pFig->m_dMaxX){
			CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
			double step=((pFig->m_dMaxY-pFig->m_dMinY)/(double)BCGYRV_COLORS), BrightnessLevels=(RightMargin+30/*==GAP_XnY*/)/(double)BRIGHTNESS_LEVELS;
			int idx, color,brightness;
			int RectY= pFig->FromDataToScreenY(0)-pFig->FromDataToScreenY(step);
			int MaxXinScreen, MaxYinScreen;
			int SavedColorID=m_RGBID;
			pFig->FromDataToScreenXY(pFig->m_dMaxX,pFig->m_dMaxY,MaxXinScreen, MaxYinScreen);
			if(dataVertical>=pFig->m_dMinY && dataVertical<=pFig->m_dMaxY){
				color=(pFig->m_dMaxY-dataVertical)/step; 
				brightness=Rectify(point.x-MaxXinScreen)/BrightnessLevels;
				idx=brightness*BCGYRV_COLORS+color;
				if(idx>=MaxRGB60 || idx<0){AfxMessageBox("idx>=MaxRGB60 || idx<0"); return;}
				else{m_RGBID=idx;}
			}
			if(m_NumSelectedDots>0 || m_NumSelectedObjs>0|| m_NumSelectedMarkings>0)Change_Color_of_SelectedOnes(m_RGBID);
			if(SavedColorID!=m_RGBID){
				//The user has changed the color in the midst of building an object.
				if(m_UnderConstructionObjID>-1){
					pDraw->m_LinedObj[m_UnderConstructionObjID].Change_Color(m_RGBID); //This helps to redraw the object right away
				}
			}
			Redraw_Everything();
		}
	}
	else if(m_isCoronal_Or_Sagittal==CORONAL){
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);
		Edit_NnInfo_from2D(dataX, m_MRI[m_Selected_MRI_Idx].m_Depth, dataVertical);
	}
	else if(m_isCoronal_Or_Sagittal==SAGITTAL){
		FromScrnToDataCoordinate(point.x, point.y, dataX, dataVertical, &m_Fig[0][0][0]);
		Edit_NnInfo_from2D(m_MRI[m_Selected_MRI_Idx].m_Depth,dataX, dataVertical);
	}

	CView::OnLButtonDown(nFlags, point);
}

void CAnalysisView::UnMarkObjects_and_RedrawEveryting()
{
	m_NumSelectedDots=0;m_NumSelectedMarkings=0; m_NumSelectedObjs=0; Redraw_Everything();
}

void CAnalysisView::Change_Color_of_SelectedOnes(int RGBID)
{
	int i,idx;
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
	if(m_isCoronal_Or_Sagittal==CORONAL){
		for(i=0; i<m_NumSelectedMarkings;i++){
			M_MarkingCORONAL[m_SelectedMarkings[i]].m_rgbID=RGBID;
		}
	}
	else{
		for(i=0; i<m_NumSelectedMarkings;i++){
			M_MarkingSAGITTAL[m_SelectedMarkings[i]].m_rgbID=RGBID;
		}
	}
	for(i=0; i<m_NumSelectedDots;i++){
		pDraw->m_Dot[m_SelectedDots[i]].m_rgbID=RGBID;
	}
	for(i=0; i<m_NumSelectedObjs;i++){
		pDraw->m_LinedObj[m_SelectedObjs[i]].Change_Color(m_RGBID);
	}
	m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CAnalysisView::Include_test() 
{			
	CFigure *pFig=&m_Fig[0][0][0]; 
	CClientDC objDC(this);
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
	CMRI *pMRI=&m_MRI[m_Selected_MRI_Idx];

	int i;
	if(m_isCoronal_Or_Sagittal==CORONAL){
		for(i=0; i<*M_NumberOfMarkingsCORONAL;i++){
			if(m_CapturRect_Left<=M_MarkingCORONAL[i].m_x && m_CapturRect_Right>=M_MarkingCORONAL[i].m_x && m_CapturRect_Top>=M_MarkingCORONAL[i].m_z && m_CapturRect_Bottom<=M_MarkingCORONAL[i].m_z && DISTANCE_AB_1D(pMRI->m_Depth,M_MarkingCORONAL[i].m_y)<m_MRI_Pixel_Dimension_In_mm){
				RegisterMarkingForSelection(i);
		}	}	
		for(i=0; i<pDraw->m_NumDots;i++){
			if(m_CapturRect_Left<=pDraw->m_Dot[i].m_x && m_CapturRect_Right>=pDraw->m_Dot[i].m_x && m_CapturRect_Top>=pDraw->m_Dot[i].m_z && m_CapturRect_Bottom<=pDraw->m_Dot[i].m_z){
				RegisterDotForSelection(i);
		}	}	
	}
	else{
		for(i=0; i<*M_NumberOfMarkingsSAGITTAL;i++){
			if(m_CapturRect_Left<=M_MarkingSAGITTAL[i].m_y && m_CapturRect_Right>=M_MarkingSAGITTAL[i].m_y && m_CapturRect_Top>=M_MarkingSAGITTAL[i].m_z && m_CapturRect_Bottom<=M_MarkingSAGITTAL[i].m_z && DISTANCE_AB_1D(pMRI->m_Depth,M_MarkingSAGITTAL[i].m_x)<m_MRI_Pixel_Dimension_In_mm){
				RegisterMarkingForSelection(i);
		}	}	
		for(i=0; i<pDraw->m_NumDots;i++){
			if(m_CapturRect_Left<=pDraw->m_Dot[i].m_y && m_CapturRect_Right>=pDraw->m_Dot[i].m_y && m_CapturRect_Top>=pDraw->m_Dot[i].m_z && m_CapturRect_Bottom<=pDraw->m_Dot[i].m_z){
				RegisterDotForSelection(i);
		}	}	
	}

	for(i=0; i<pDraw->m_NumLinedObj;i++){
		if(m_CapturRect_Left<=pDraw->m_LinedObj[i].m_Left && m_CapturRect_Right>=pDraw->m_LinedObj[i].m_Right && m_CapturRect_Top>=pDraw->m_LinedObj[i].m_Top && m_CapturRect_Bottom<=pDraw->m_LinedObj[i].m_Bottom){
			RegisterObjForSelection(i);
		}
	}
}


int CAnalysisView::MarkingHitTest(double clickX, double clickY, CFigure *pFig) 
{
	int i;
	if(m_isCoronal_Or_Sagittal==CORONAL){
		for(i=0; i<*M_NumberOfMarkingsCORONAL;i++){
			if(abs(pFig->FromDataToScreenX(M_MarkingCORONAL[i].m_x )-pFig->FromDataToScreenX(clickX))<CLICK_PRECISION_ON_SCREEN && abs(pFig->FromDataToScreenY(M_MarkingCORONAL[i].m_z)-pFig->FromDataToScreenY(clickY))<CLICK_PRECISION_ON_SCREEN){
				return i;
			}
		}
	}
	else{
		for(i=0; i<*M_NumberOfMarkingsSAGITTAL;i++){
			if(abs(pFig->FromDataToScreenX(M_MarkingSAGITTAL[i].m_y)-pFig->FromDataToScreenX(clickX))<CLICK_PRECISION_ON_SCREEN && abs(pFig->FromDataToScreenY(M_MarkingSAGITTAL[i].m_z)-pFig->FromDataToScreenY(clickY))<CLICK_PRECISION_ON_SCREEN){
				return i;
			}
		}
	}
	return -1;
}

int CAnalysisView::PointHitTest(double clickX, double clickY, CFigure *pFig) 
{
	int i;
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
    for(i=0; i<pDraw->m_NumDots;i++){
		if(m_isCoronal_Or_Sagittal==CORONAL && abs(pFig->FromDataToScreenX(pDraw->m_Dot[i].m_x)-pFig->FromDataToScreenX(clickX))<CLICK_PRECISION_ON_SCREEN && abs(pFig->FromDataToScreenY(pDraw->m_Dot[i].m_z)-pFig->FromDataToScreenY(clickY))<CLICK_PRECISION_ON_SCREEN){
			return i;
		}
		else if(m_isCoronal_Or_Sagittal==SAGITTAL && abs(pFig->FromDataToScreenX(pDraw->m_Dot[i].m_y)-pFig->FromDataToScreenX(clickX))<CLICK_PRECISION_ON_SCREEN && abs(pFig->FromDataToScreenY(pDraw->m_Dot[i].m_z)-pFig->FromDataToScreenY(clickY))<CLICK_PRECISION_ON_SCREEN){
			return i;
		}
	}
	return -1;
}

int CAnalysisView::LineHitTest(double clickX, double clickY, CFigure *pFig) 
{
	int i;
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
	for(i=0; i<pDraw->m_NumLinedObj;i++){
		if((m_BiggerVertexIdxOfHitLine=pDraw->m_LinedObj[i].LineHitTest(clickX, clickY,pFig, m_isCoronal_Or_Sagittal))>-1){
			return i;
		}
	}
	return -1;
}

int CAnalysisView::CheckIfThisObjIsSelected(int ObjIdx)
{
	for(int i=0; i<m_NumSelectedObjs;i++){
		if(m_SelectedObjs[i]==ObjIdx){
			return i;
		}
	}
	return -1;
}
int CAnalysisView::CheckIfThisPointIsSelected(int PointIdx)
{
	for(int i=0; i<m_NumSelectedDots;i++){
		if(m_SelectedDots[i]==PointIdx){
			return i;
		}
	}
	return -1;
}
int CAnalysisView::CheckIfThisMarkingIsSelected(int PointIdx)
{
	for(int i=0; i<m_NumSelectedMarkings;i++){
		if(m_SelectedMarkings[i]==PointIdx){
			return i;
		}
	}
	return -1;
}

void CAnalysisView::Figs_Captured() 
{
	int clm, row;
	CFigure *pFig;
	for(row=0;row<m_dHowManyRows;row++){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			pFig=&m_Fig[row][clm][0];
			if((m_CapturRec.top<=pFig->m_nTy && m_CapturRec.bottom>=pFig->m_nBy && m_CapturRec.left<=pFig->m_nTx && m_CapturRec.right>=pFig->m_nBx)||
				RectCrossingRect(m_CapturRec.left,m_CapturRec.top,m_CapturRec.right,m_CapturRec.bottom,pFig->m_nTx,pFig->m_nTy,pFig->m_nBx,pFig->m_nBy)||
				(m_CapturRec.top>=pFig->m_nTy && m_CapturRec.bottom<=pFig->m_nBy && m_CapturRec.left>=pFig->m_nTx && m_CapturRec.right<=pFig->m_nBx)){
				if(pFig->m_Captured==1){
				}
				else{
					pFig->m_Captured=1; 
					m_numCaptured++;
					Hilight_OneCapturedFig(pFig);
				}
			}
			else{ 		
				if(pFig->m_Captured==1&&(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL)){
					pFig->m_Captured=0;//Clerar previous selection unless Control key is pressed
					m_numCaptured--;
					UnHilight_OneCaptured(pFig);
				}
			}

		}
	}
}
//Using 4 LineCrossingRect(), see if the two rectangles cross each other.
BOOL CAnalysisView::RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom)
{
	if(LineCrossingRect(recLeft1,recTop1,recRight1,recTop1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recTop1,recRight1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recRight1,recBottom1,recLeft1,recBottom1, recLeft, recTop, recRight, recBottom)||
	LineCrossingRect(recLeft1,recBottom1,recLeft1,recTop1, recLeft, recTop, recRight, recBottom))return 1;
	else return 0;
}

//Using the counterclockwise test (CCW), see if any of the 4 line-segments of the rectangle crosses the line segment defined by the two points (x1,y1), (x2,y2).
BOOL CAnalysisView::LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom)
{
#define CCW(ax, ay, bx, by, cx, cy ) ((cy-ay)*(bx-ax)>(by-ay)*(cx-ax))
	if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recLeft,recTop,recRight,recTop,x1,y1)!=CCW(recLeft,recTop,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recRight,recBottom)!=CCW(x1,y1,x2,y2,recRight,recTop))&&(CCW(recRight,recBottom,recRight,recTop,x1,y1)!=CCW(recRight,recBottom,recRight,recTop,x2,y2))
		)
		return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recBottom)!=CCW(x1,y1,x2,y2,recRight,recBottom))&&(CCW(recLeft,recBottom,recRight,recBottom,x1,y1)!=CCW(recLeft,recBottom,recRight,recBottom,x2,y2))
		)return 1;
	else if(
		(CCW(x1,y1,x2,y2,recLeft,recTop)!=CCW(x1,y1,x2,y2,recLeft,recBottom))&&(CCW(recLeft,recTop,recLeft,recBottom,x1,y1)!=CCW(recLeft,recTop,recLeft,recBottom,x2,y2))
		)
		return 1;
	else return 0;
}
    
void CAnalysisView::Hilight_AllCaptured_Figs() 
{
	if(m_numCaptured>0){
		int clm, row;
		CFigure *pFig;
		m_DC[0].SelectObject(m_ORANGE2);
		for(row=0;row<m_dHowManyRows;row++){
			for(clm=0;clm<m_dHowManyClmns;clm++){
				pFig=&m_Fig[row][clm][0];
				if(pFig->m_Captured==1){ 
					m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
				}
			}
		}
		CClientDC objDC(this);
		objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY);
	}
	m_DC[0].SelectObject(&m_bkPen);
}

void CAnalysisView::Hilight_OneCapturedFig(CFigure *pFig) 
{
	m_DC[0].SelectObject(m_ORANGE2);
	m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 
	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_ORANGE2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	
	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
	m_DC[0].SelectObject(&m_bkPen);
}
void CAnalysisView::UnHilight_OneCaptured(CFigure *pFig) 
{
	m_DC[0].SelectObject(m_BLACK2);
	m_DC[0].MoveTo(pFig->m_nTx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nTy); 	m_DC[0].LineTo(pFig->m_nBx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nBy); 	m_DC[0].LineTo(pFig->m_nTx,pFig->m_nTy); 
	CClientDC objDC(this);
	objDC.SelectObject(m_BLACK2);
	objDC.MoveTo(pFig->m_nTx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nTy); 	objDC.LineTo(pFig->m_nBx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nBy); 	objDC.LineTo(pFig->m_nTx,pFig->m_nTy); 
}


void CAnalysisView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	//if(m_SelectedItemID==SCALE_AREA){
		menu.LoadMenu(IDR_MENU1);
		CMenu* pPopup=menu.GetSubMenu(0);
		ASSERT(pPopup!=NULL);
	//}

	//Display the popup menu
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x, point.y,this);
}

void CAnalysisView::DisplayTitle()
{
#define HeightWidthRatio 0.4
	CClientDC objDC(this);
	m_DC[0].SetBkColor(RGB(220, 220, 220));

	int m_centerOfTitle= (m_TitleArea.left+m_TitleArea.right)*0.5;
	int length=m_GlobalTitle.GetLength(); 
	if(length==0){AfxMessageBox("Check if you have done m_GlobalTitle="" somewhere."); exit(0);}//Sometimes multi-thread causes problems.
	int WidthOfTitle=m_topOfFigs*HeightWidthRatio*length;
	CPen *OldPen=objDC.SelectObject(&m_NULL); m_DC[0].SelectObject(&m_NULL);
	if(m_DoNotErase!=1){m_DC[0].Rectangle(m_dTitleRect.left,m_dTitleRect.top,m_dTitleRect.right,m_dTitleRect.bottom); objDC.Rectangle(m_dTitleRect.left,m_dTitleRect.top,m_dTitleRect.right,m_dTitleRect.bottom); }
	m_DoNotErase=0;
	m_dTitleRect.top=0+0.1*m_topOfFigs; m_dTitleRect.bottom=m_TitleArea.bottom; m_dTitleRect.left=m_centerOfTitle-WidthOfTitle*0.5; m_dTitleRect.right=m_centerOfTitle+WidthOfTitle*0.5;
	//FontInRect(&m_DC[0],m_GlobalTitle,m_dTitleRect);
	objDC.BitBlt(m_dTitleRect.left, m_dTitleRect.top, WidthOfTitle, m_topOfFigs,&m_DC[0],m_dTitleRect.left, m_dTitleRect.top,SRCCOPY);
	m_DC[0].SetBkColor(RGB(255, 255, 255));
	objDC.SelectObject(&OldPen);m_DC[0].SelectObject(&OldPen);
}

//This "WhichPane()" function changes the size of the pane and its position, when the "blow-up button" is pressed. 
void CAnalysisView::WhichFigure(CPoint point) 
{
#define BNDRY_MRGN 20
	int clm,row;
	CFigure *pFig;
	m_pSpotlightFig=NULL;

	//If the rectangle on screen (m_paneTopX, m_paneTopY, m_paneBottomX, m_paneBottomY) is not defined for the graph pane, define it!
	if(!m_bIsScrXYforIJ){ 
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
				pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	
			}
		}
		m_bIsScrXYforIJ=1;
	}

	if(point.y<=m_dTitleRect.bottom && point.y>=m_dTitleRect.top && point.x<=m_dTitleRect.right && point.x>=m_dTitleRect.left){ 
		m_CallingFrom=1;
		//OnEnChangeEditString1();
		if(m_GlobalTitle.GetLength()!=0){
			DisplayTitle();
		}
		goto END_OF_PREMEASURE;
	}

#define CLICK_ON_BLANK			0
#define AXIS_SCALE_CLICKED		1
#define FIG_LABEL_CLICKED		3
	int	clickType, found=-1;
	//Here it PRE-examines if the user has already chosen some Figures before clicking an icon to "Batch" manipulate them.
	for(clm=0;clm<m_dHowManyClmns;clm++){
		for(row=0;row<m_dHowManyRows;row++){
			pFig=&m_Fig[row][clm][0];
			if((point.x>=pFig->m_nTx && point.x<=pFig->m_nBx)&&(point.y<=pFig->m_nBy&&point.y>=pFig->m_nTy)){ 
				m_pSpotlightFig=pFig;
				// Edit: X, Y axes
				//Axis-scale manipulators has been clicked?
				if(  ((point.x>=pFig->m_FirstYlabel_left && point.x<=pFig->m_FirstYlabel_right)&&(point.y>=pFig->m_FirstYlabel_top && point.y<=pFig->m_FirstYlabel_bottom))
					    ||((point.x>=pFig->m_FirstXlabel_left && point.x<=pFig->m_FirstXlabel_right)&&(point.y>=pFig->m_FirstXlabel_top && point.y<=pFig->m_FirstXlabel_bottom))
						||((point.x>=pFig->m_LastYlabel_left && point.x<=pFig->m_LastYlabel_right)&&(point.y>=pFig->m_LastYlabel_top && point.y<=pFig->m_LastYlabel_bottom))
					    ||((point.x>=pFig->m_LastXlabel_left && point.x<=pFig->m_LastXlabel_right)&&(point.y>=pFig->m_LastXlabel_top && point.y<=pFig->m_LastXlabel_bottom))){
					m_dMinimumX=m_pSpotlightFig->m_dMinX; m_dMaximumX=m_pSpotlightFig->m_dMaxX; m_dMinimumY=m_pSpotlightFig->m_dMinY; m_dMaximumY=m_pSpotlightFig->m_dMaxY;
					clickType=AXIS_SCALE_CLICKED;
					goto END_OF_PREMEASURE;
				}
				//Figure Labels has been clicked?
				else if((point.x>=pFig->m_Title.StringRect.left && point.x<=pFig->m_Title.StringRect.right)&&(point.y>=pFig->m_Title.StringRect.top && point.y<=pFig->m_Title.StringRect.bottom)){
					clickType=FIG_LABEL_CLICKED;
					goto END_OF_PREMEASURE;
				}
				else{
					clickType=CLICK_ON_BLANK;
					goto END_OF_PREMEASURE;
				}
			}
		}
	}
	END_OF_PREMEASURE:

	//There are three pices of information: (1) which fig is clicked? (2) if a figure is clicked, is it already selected? (3) what kind of clickType is this?
	if(m_pSpotlightFig==NULL){
		for(clm=0;clm<m_dHowManyClmns;clm++){
			for(row=0;row<m_dHowManyRows;row++){
				pFig=&m_Fig[row][clm][0];
				if(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL){//Clear previous selection unless Control key is pressed
					if(pFig->m_Captured==1){
						pFig->m_Captured=0;
						UnHilight_OneCaptured(pFig); 
						m_numCaptured--;
					}
				}
			}
		}
	}
	//Clicked a Figure!!
	else{
		//Is the clicked figure already selected?
		if(m_pSpotlightFig->m_Captured==1){
			switch(clickType){
				case AXIS_SCALE_CLICKED:
					OnChangeParameters();
					for(clm=0;clm<m_dHowManyClmns;clm++){
						for(row=0;row<m_dHowManyRows;row++){
							pFig=&m_Fig[row][clm][0];
							if(pFig->m_Captured!=1)continue;
							if(m_dMinimumX!=pFig->m_dMinX || m_dMaximumX!=pFig->m_dMaxX || m_dMinimumY!=pFig->m_dMinY || m_dMaximumY!=pFig->m_dMaxY){
								pFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
								DrawOneFigure(pFig, 1);
					}	}	}		
					RequestDisplayEvent(RESCALING_AXES); //Send a message to redraw
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2;
					//OnEnChangeEditString1();
					for(clm=0;clm<m_dHowManyClmns;clm++){
						for(row=0;row<m_dHowManyRows;row++){
							pFig=&m_Fig[row][clm][0];
							if(pFig->m_Captured!=1)continue;
							pFig->m_Title.NameString=m_pSpotlightFig->m_Title.NameString;
							DrawOneFigure(pFig, 1);
					}	}	
					RequestDisplayEvent(RENAMING_FIG); //Send a message to redraw
					break;
				default: break;
			}
		}
		else{
			//First clear all the perior-selected ones and highlight only the newly clicked one
			if(m_KeyCode!=17&&m_KeyCode!=VK_RCONTROL){//Clear previous selection unless Control key is pressed
				for(clm=0;clm<m_dHowManyClmns;clm++){
					for(row=0;row<m_dHowManyRows;row++){
						pFig=&m_Fig[row][clm][0];
						if(pFig->m_Captured==1){
							pFig->m_Captured=0;
							UnHilight_OneCaptured(pFig); 
						}
					}
				}
				m_numCaptured=0;
			}
			m_pSpotlightFig->m_Captured=1;
			m_numCaptured++;
			Hilight_OneCapturedFig(m_pSpotlightFig);
			//Do the user-requested job.
			switch(clickType){
				case AXIS_SCALE_CLICKED:
					OnChangeParameters();
					if(m_dMinimumX!=m_pSpotlightFig->m_dMinX || m_dMaximumX!=m_pSpotlightFig->m_dMaxX || m_dMinimumY!=m_pSpotlightFig->m_dMinY || m_dMaximumY!=m_pSpotlightFig->m_dMaxY){
						m_pSpotlightFig->SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);
						DrawOneFigure(m_pSpotlightFig, 1);
					}
					RequestDisplayEvent(RESCALING_AXES); //Send a message to redraw
					break;
				case FIG_LABEL_CLICKED:
					m_CallingFrom=2; 
					//OnEnChangeEditString1();
					DrawOneFigure(m_pSpotlightFig, 1);
					RequestDisplayEvent(RENAMING_FIG); //Send a message to redraw
					break;
				default: 				
				break;
			}
		}
	}
}

void CAnalysisView::DrawOneFigure(CFigure *pFig, int isToHighlight) 
{
	CClientDC objDC(this);
	pFig->Prepare4FigFrame(); 
	pFig->Frame_Graph2D(&m_DC[0],1); 
	pFig->RefreshFrame(&objDC,&m_DC[0]); 
	if(isToHighlight==1)Hilight_OneCapturedFig(pFig);
	else UnHilight_OneCaptured(pFig);
}
