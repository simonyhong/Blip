// AnalysisView.cpp : implementation of the CAnalysisView class
//

#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include "MainFrm.h"

int* CLinedObj::m_pNeedsToSave;


void CLinedObj::InitObj()
{
	m_NumVertices=0;
	m_Left=INT_MAX; m_Right=-INT_MAX; m_Top=-INT_MAX; m_Bottom=INT_MAX;
}

void CLinedObj::Draw_Obj(CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen, int isToEditPoints, int isCoronal_Or_Sagittal)
{
	if(m_NumVertices<2)return;
	double horizontal, virtical=m_Vtx[0].m_z;
	if(isCoronal_Or_Sagittal==CORONAL)horizontal=m_Vtx[0].m_x; else horizontal=m_Vtx[0].m_y; 
	pDC->SelectObject(pPen[m_rgbID]); pObjDC->SelectObject(pPen[m_rgbID]);
	pFig->MyMoveTo(horizontal,virtical,pDC);
	pFig->MyMoveTo(horizontal,virtical,pObjDC);
	if(isToEditPoints==0)pFig->Circle(horizontal,virtical,2,pDC);
	else pFig->FillSolidRect8x8(horizontal,virtical,RGB(0,0,255),pDC);
	for(int i=1; i<m_NumVertices; i++){
		if(isCoronal_Or_Sagittal==CORONAL)horizontal=m_Vtx[i].m_x; else horizontal=m_Vtx[i].m_y; 
		virtical=m_Vtx[i].m_z;
		pFig->MyLineTo(horizontal,virtical,pDC);
		pFig->MyLineTo(horizontal,virtical,pObjDC);
		if(isToEditPoints==0)pFig->Circle(horizontal,virtical,2,pDC);
		else pFig->FillSolidRect8x8(horizontal,virtical,RGB(0,0,255),pDC);
	}
}
int CLinedObj::PointHitTest(double Horizontal, double Z, CFigure *pFig, int isCoronal_Or_Sagittal)
{
	for(int i=0; i<m_NumVertices; i++){
		if(isCoronal_Or_Sagittal==CORONAL && abs(pFig->FromDataToScreenX(m_Vtx[i].m_x)-pFig->FromDataToScreenX(Horizontal))<CLICK_PRECISION_ON_SCREEN && abs(pFig->FromDataToScreenY(m_Vtx[i].m_z)-pFig->FromDataToScreenY(Z))<CLICK_PRECISION_ON_SCREEN){
			return i;
		}
		else if(isCoronal_Or_Sagittal==SAGITTAL && abs(pFig->FromDataToScreenX(m_Vtx[i].m_y)-pFig->FromDataToScreenX(Horizontal))<CLICK_PRECISION_ON_SCREEN && abs(pFig->FromDataToScreenY(m_Vtx[i].m_z)-pFig->FromDataToScreenY(Z))<CLICK_PRECISION_ON_SCREEN){
			return i;
		}
	}
	return -1;
}
void CLinedObj::Draw_Obj_RelativeToXY_CORONAL(double X, double Z, CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen)
{
	double horizontal, virtical=m_Vtx[0].m_z;
	if(m_NumVertices<2)return;
	pDC->SelectObject(pPen[m_rgbID]); pObjDC->SelectObject(pPen[m_rgbID]);
	pFig->MyMoveTo(m_Vtx[0].m_x+X,m_Vtx[0].m_z+Z,pDC);
	pFig->MyMoveTo(m_Vtx[0].m_x+X,m_Vtx[0].m_z+Z,pObjDC);
	pFig->Circle(m_Vtx[0].m_x+X,m_Vtx[0].m_z+Z,2,pDC);
	for(int i=1; i<m_NumVertices; i++){
		pFig->Circle(m_Vtx[i].m_x+X,m_Vtx[i].m_z+Z,2,pDC);
		pFig->MyLineTo(m_Vtx[i].m_x+X,m_Vtx[i].m_z+Z,pDC);
		pFig->MyLineTo(m_Vtx[i].m_x+X,m_Vtx[i].m_z+Z,pObjDC);
	}
}
void CLinedObj::Draw_Obj_RelativeToXY_SAGITTAL(double Y, double Z, CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen)
{
	double horizontal, virtical=m_Vtx[0].m_z;
	if(m_NumVertices<2)return;
	pDC->SelectObject(pPen[m_rgbID]); pObjDC->SelectObject(pPen[m_rgbID]);
	pFig->MyMoveTo(m_Vtx[0].m_y+Y,m_Vtx[0].m_z+Z,pDC);
	pFig->MyMoveTo(m_Vtx[0].m_y+Y,m_Vtx[0].m_z+Z,pObjDC);
	pFig->Circle(m_Vtx[0].m_y+Y,m_Vtx[0].m_z+Z,2,pDC);
	for(int i=1; i<m_NumVertices; i++){
		pFig->Circle(m_Vtx[i].m_y+Y,m_Vtx[i].m_z+Z,2,pDC);
		pFig->MyLineTo(m_Vtx[i].m_y+Y,m_Vtx[i].m_z+Z,pDC);
		pFig->MyLineTo(m_Vtx[i].m_y+Y,m_Vtx[i].m_z+Z,pObjDC);
	}
}
void CLinedObj::Change_Color(int RGBID)
{
	m_rgbID=RGBID;
	for(int i=0; i<m_NumVertices; i++){
		m_Vtx[i].m_rgbID=RGBID;
	}
}
void CLinedObj::Translate_In2D_CORONAL(double horizontal, double vertical)
{
	for(int i=0; i<m_NumVertices; i++){
		m_Vtx[i].m_x+=horizontal; m_Vtx[i].m_z+=vertical;
	}
	m_Left+=horizontal; m_Right+=horizontal; 
	m_Top+=vertical; m_Bottom+=vertical;
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}
void CLinedObj::Translate_In2D_SAGITTAL(double horizontal, double vertical)
{
	for(int i=0; i<m_NumVertices; i++){
		m_Vtx[i].m_y+=horizontal; m_Vtx[i].m_z+=vertical;
	}
	m_Left+=horizontal; m_Right+=horizontal; 
	m_Top+=vertical; m_Bottom+=vertical;
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CLinedObj::Translate_Horizontally_CORONAL(double X)
{
	for(int i=0; i<m_NumVertices; i++){
		m_Vtx[i].m_x+=X; 
	}
	m_Left+=X; m_Right+=X; 
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}
void CLinedObj::Translate_Horizontally_SAGITTAL(double Y)
{
	for(int i=0; i<m_NumVertices; i++){
		m_Vtx[i].m_y+=Y; 
	}
	m_Left+=Y; m_Right+=Y; 
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CLinedObj::TranslateVertically(double vertical)
{
	for(int i=0; i<m_NumVertices; i++){
		m_Vtx[i].m_z+=vertical;
	}
	m_Top+=vertical; m_Bottom+=vertical;
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CLinedObj::Highlight_Obj(CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen, int isCoronal_Or_Sagittal)
{
	if(m_NumVertices<2)return;
	pDC->SelectObject(pPen[m_rgbID]); pObjDC->SelectObject(pPen[m_rgbID]);
	double horizontal;
	if(isCoronal_Or_Sagittal==CORONAL){horizontal=m_Vtx[0].m_x;} 
	else{horizontal=m_Vtx[0].m_y;}
	pFig->Circle(horizontal,m_Vtx[0].m_z,2,pDC);
	pFig->MyMoveTo(horizontal,m_Vtx[0].m_z,pDC);
	pFig->MyMoveTo(horizontal,m_Vtx[0].m_z,pObjDC);
	for(int i=0; i<m_NumVertices; i++){
		if(isCoronal_Or_Sagittal==CORONAL){horizontal=m_Vtx[i].m_x;} 
		else{horizontal=m_Vtx[i].m_y;}
		pFig->Circle(horizontal,m_Vtx[i].m_z,2,pDC);
		pFig->MyLineTo(horizontal,m_Vtx[i].m_z,pDC);
		pFig->MyLineTo(horizontal,m_Vtx[i].m_z,pObjDC);
	}
}

void CLinedObj::Draw_lastLineSegment(CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen, int isCoronal_Or_Sagittal)
{
	if(m_NumVertices<2)return;
	double horizontal_2,horizontal_1;
	if(isCoronal_Or_Sagittal==CORONAL){horizontal_2=m_Vtx[m_NumVertices-2].m_x; horizontal_1=m_Vtx[m_NumVertices-1].m_x;} 
	else{horizontal_2=m_Vtx[m_NumVertices-2].m_y; horizontal_1=m_Vtx[m_NumVertices-1].m_y;}
	pDC->SelectObject(pPen[m_rgbID]);
	pFig->LineAtoB(horizontal_2,m_Vtx[m_NumVertices-2].m_z,horizontal_1,m_Vtx[m_NumVertices-1].m_z,pDC);
	pFig->LineAtoB(horizontal_2,m_Vtx[m_NumVertices-2].m_z,horizontal_1,m_Vtx[m_NumVertices-1].m_z,pObjDC);
}

void CLinedObj::Add_VertexIn3D(double x, double y, double z, int RGBID, int isCoronal_Or_Sagittal)
{
	m_Vtx[m_NumVertices].m_x=x, m_Vtx[m_NumVertices].m_y=y, m_Vtx[m_NumVertices].m_z=z; m_Vtx[m_NumVertices].m_rgbID=RGBID; m_rgbID=RGBID; 

	if(isCoronal_Or_Sagittal==CORONAL){if(m_Left>x)m_Left=x; if(m_Right<x)m_Right=x; }
	else                              {if(m_Left>y)m_Left=y; if(m_Right<y)m_Right=y; }
	if(m_Top<z)m_Top=z;   if(m_Bottom>z)m_Bottom=z;
	m_NumVertices++;
	if(m_NumVertices>=m_Vtx.GetSize()-1)m_Vtx.SetSize(2*m_Vtx.GetSize());
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CLinedObj::Insert_Vertex_CORONAL(int hitSegmentIdx, double x, double y, double z, int RGBID)
{
	m_Vtx.InsertAt(hitSegmentIdx, CVertex());
	m_Vtx[hitSegmentIdx].m_x=x, m_Vtx[hitSegmentIdx].m_y=y, m_Vtx[hitSegmentIdx].m_z=z; m_Vtx[hitSegmentIdx].m_rgbID=RGBID; m_rgbID=RGBID; 

	if(m_Left>x)m_Left=x; if(m_Right<x)m_Right=x; 
	if(m_Top<z)m_Top=z;   if(m_Bottom>z)m_Bottom=z;
	m_NumVertices++;
	if(m_NumVertices>=m_Vtx.GetSize()-1)m_Vtx.SetSize(2*m_Vtx.GetSize());
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}
void CLinedObj::Insert_Vertex_SAGITTAL(int hitSegmentIdx, double x, double y, double z, int RGBID)
{
	m_Vtx.InsertAt(hitSegmentIdx, CVertex());
	m_Vtx[hitSegmentIdx].m_x=x, m_Vtx[hitSegmentIdx].m_y=y, m_Vtx[hitSegmentIdx].m_z=z; m_Vtx[hitSegmentIdx].m_rgbID=RGBID; m_rgbID=RGBID; 

	if(m_Left>y)m_Left=y; if(m_Right<y)m_Right=y; 
	if(m_Top<z)m_Top=z;   if(m_Bottom>z)m_Bottom=z;
	m_NumVertices++;
	if(m_NumVertices>=m_Vtx.GetSize()-1)m_Vtx.SetSize(2*m_Vtx.GetSize());
	*m_pNeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}
void CLinedObj::UpdateBoundary(int isCoronal_Or_Sagittal)
{
	m_Left=INT_MAX; m_Right=-INT_MAX; m_Top=-INT_MAX; m_Bottom=INT_MAX;

	for(int i=0; i<m_NumVertices;i++){
		if(isCoronal_Or_Sagittal==CORONAL){if(m_Left>m_Vtx[i].m_x)m_Left=m_Vtx[i].m_x; if(m_Right<m_Vtx[i].m_x)m_Right=m_Vtx[i].m_x; }
		else                              {if(m_Left>m_Vtx[i].m_y)m_Left=m_Vtx[i].m_y; if(m_Right<m_Vtx[i].m_y)m_Right=m_Vtx[i].m_y; }
		if(m_Top  < m_Vtx[i].m_z)m_Top =m_Vtx[i].m_z; if(m_Bottom>m_Vtx[i].m_z)m_Bottom=m_Vtx[i].m_z;
	}
}

#define DISTANCE_CHECK_CORONAL(Horizontal,Vertical) (Distance_Point_toLineSeg(pFig->FromDataToScreenX(m_Vtx[i].m_x),pFig->FromDataToScreenY(m_Vtx[i].m_z),pFig->FromDataToScreenX(m_Vtx[i+1].m_x),pFig->FromDataToScreenY(m_Vtx[i+1].m_z), pFig->FromDataToScreenX(Horizontal),pFig->FromDataToScreenY(Vertical))<CLICK_PRECISION_ON_SCREEN)
#define DISTANCE_CHECK_SAGITTAL(Horizontal,Vertical) (Distance_Point_toLineSeg(pFig->FromDataToScreenX(m_Vtx[i].m_y),pFig->FromDataToScreenY(m_Vtx[i].m_z),pFig->FromDataToScreenX(m_Vtx[i+1].m_y),pFig->FromDataToScreenY(m_Vtx[i+1].m_z), pFig->FromDataToScreenX(Horizontal),pFig->FromDataToScreenY(Vertical))<CLICK_PRECISION_ON_SCREEN)
int CLinedObj::LineHitTest(double Horizontal, double Vertical, CFigure *pFig, int isCoronal_Or_Sagittal) 
{
	int i;
	for(i=0; i<m_NumVertices-1;i++){
		if(isCoronal_Or_Sagittal==CORONAL && (DISTANCE_CHECK_CORONAL(Horizontal,Vertical))){//in mm
			return i+1;
		}
		else if(isCoronal_Or_Sagittal==SAGITTAL && (DISTANCE_CHECK_SAGITTAL(Horizontal,Vertical))){//in mm
			return i+1;
		}
	}
	return -1;
}

int CLinedObj::LineHitTest_AND_Insert_Vertex(double x, double y, double z, CFigure *pFig, int isCoronal_Or_Sagittal) 
{
	int i;
	for(i=0; i<m_NumVertices-1;i++){
		if(isCoronal_Or_Sagittal==CORONAL && (DISTANCE_CHECK_CORONAL(x,z))){//in mm
			Insert_Vertex_CORONAL(i+1,x,y,z,m_rgbID);
			return i+1;
		}
		else if(isCoronal_Or_Sagittal==SAGITTAL && (DISTANCE_CHECK_SAGITTAL(x,z))){//in mm
			Insert_Vertex_SAGITTAL(i+1,x,y,z,m_rgbID);
			return i+1;
		}
	}
	return -1;
}

double CLinedObj::Distance_Point_toLineSeg(double x1,double y1,double x2,double y2,double x3,double y3) 
{		
	double x,y;
	double A = x3 - x1;
	double B = y3 - y1;
	double C = x2 - x1;
	double D = y2 - y1;
	double param = (A * C + B * D)/( C * C + D * D);
 
	if(param < 0){
		x = x1;
		y = y1;
	}
	else if(param > 1){
		x = x2;
		y = y2;
	}
	else{
		x = x1 + param * C;
		y = y1 + param * D;
	}
 	return  DISTANCE_AB(x3,y3,x,y);
}