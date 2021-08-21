#include "stdafx.h"
#include "Analysis.h"

#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include <gl/gl.h>
#include <gl/glu.h>

CAnalysisView *CMRI::m_pView;
double	CMRI::m_LowerLeftFirstPixel_Abscissa_3D;
double	CMRI::m_LowerLeftFirstPixel_Ordinate_3D;	


//Used by 2D MRI
void CAnalysisView::Redraw_Everything() 
{	
	EnterCriticalSection(&m_CriticalSection2);
	CFigure *pFig=&m_Fig[0][0][0]; 
	CClientDC objDC(this);
	
	Disply_MRI_and_ColorLegend();
	
	if(m_toShowDrawing==1){
		Draw_Drawing();
	}
	
	if(m_toShowNeuralInfo==1){
		if(m_isCoronal_Or_Sagittal==CORONAL)Draw_Neural_Info_CORONAL(m_MRI[m_Selected_MRI_Idx].m_Depth);
		else                                Draw_Neural_Info_SAGITTAL(m_MRI[m_Selected_MRI_Idx].m_Depth);
	}

	//Redraw the electrode tip
	m_DC[0].SelectObject(m_RED);
	if(m_isCoronal_Or_Sagittal==CORONAL)pFig->Circle(*M_ElectrodeTipX_3DAtlas, *M_ElectrodeTipZ_3DAtlas,4,&m_DC[0]);
	else if(m_isCoronal_Or_Sagittal==SAGITTAL)pFig->Circle(*M_ElectrodeTipY_3DAtlas, *M_ElectrodeTipZ_3DAtlas,4,&m_DC[0]); //Z axis is flipped in OpenGL (uses the right hand system).
	Copy_ElectrodeCoordinate_toDisplay();
	pFig->Disp_Electrode_Position(&m_DC[0]);

	//Display the name of the MRI file
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5,"                      "); //Erase previous MRI file name
	m_DC[0].TextOut( m_TitleArea.left+5,m_TitleArea.top+5,m_MRI[m_Selected_MRI_Idx].m_JustMRI_FileName); //MRI file name

	//Show Ruller
	if(m_toShowRuller==1){
		m_DC[0].SelectObject(m_BLUE);
		double x=0, y=0;
		while(1){
			//Vertical lines
			if(!(-x<pFig->m_dMinX)) pFig->LineAtoB(-x, pFig->m_dMinY,-x,  pFig->m_dMaxY,&m_DC[0]);
			if(!( x>pFig->m_dMaxX)) pFig->LineAtoB( x, pFig->m_dMinY, x,  pFig->m_dMaxY,&m_DC[0]);
			//Horizontal lines
			if(!(-y<pFig->m_dMinY)) pFig->LineAtoB(pFig->m_dMinX, -y,pFig->m_dMaxX,  -y,&m_DC[0]);
			if(!( y>pFig->m_dMaxY)) pFig->LineAtoB(pFig->m_dMinX,  y, pFig->m_dMaxX,  y,&m_DC[0]);
			x+=5; y+=5;
			if(-x<pFig->m_dMinX && x>pFig->m_dMaxX && -y<pFig->m_dMinY && y>pFig->m_dMaxY)break; 
		}
	}
	//Refresh the screen
	objDC.BitBlt(0,0,m_nScrMaxX,m_nScrMaxY,&m_DC[0],0,0,SRCCOPY); m_DrewTmpLine=0;
	m_DC[0].SelectObject(m_bkBrush);
	LeaveCriticalSection(&m_CriticalSection2);
}

void CAnalysisView::Draw_Drawing() 
{
	CFigure *pFig=&m_Fig[0][0][0]; 
	CClientDC objDC(this);
	int i;
	
	//####  Markings
	CMRI *pMRI=&m_MRI[m_Selected_MRI_Idx];
	for(i=0; i<*M_NumberOfMarkingsCORONAL;i++){
		if(m_isCoronal_Or_Sagittal==CORONAL){
			if(DISTANCE_AB_1D(pMRI->m_Depth,M_MarkingCORONAL[i].m_y)>=m_MRI_Pixel_Dimension_In_mm)continue;
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingCORONAL[i].m_rgbID]);
			m_DC[0].SelectObject(m_ColorBrush[M_MarkingCORONAL[i].m_rgbID]);
			pFig->Circle(M_MarkingCORONAL[i].m_x,M_MarkingCORONAL[i].m_z,4,&m_DC[0]);
		}
		else{
			if(DISTANCE_AB_1D(pMRI->m_Depth,M_MarkingCORONAL[i].m_x)>=m_MRI_Pixel_Dimension_In_mm)continue;
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingCORONAL[i].m_rgbID]);
			m_DC[0].SelectObject(m_ColorBrush[M_MarkingCORONAL[i].m_rgbID]);
			pFig->Circle(M_MarkingCORONAL[i].m_y,M_MarkingCORONAL[i].m_z,4,&m_DC[0]);
		}
	}
	for(i=0; i<*M_NumberOfMarkingsSAGITTAL;i++){
		if(m_isCoronal_Or_Sagittal==CORONAL){
			if(DISTANCE_AB_1D(pMRI->m_Depth,M_MarkingSAGITTAL[i].m_y)>=m_MRI_Pixel_Dimension_In_mm)continue;
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingSAGITTAL[i].m_rgbID]);
			m_DC[0].SelectObject(m_ColorBrush[M_MarkingCORONAL[i].m_rgbID]);
			pFig->Circle(M_MarkingSAGITTAL[i].m_x,M_MarkingSAGITTAL[i].m_z,4,&m_DC[0]);
		}
		else{
			if(DISTANCE_AB_1D(pMRI->m_Depth,M_MarkingSAGITTAL[i].m_x)>=m_MRI_Pixel_Dimension_In_mm)continue;
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingSAGITTAL[i].m_rgbID]);
			m_DC[0].SelectObject(m_ColorBrush[M_MarkingCORONAL[i].m_rgbID]);
			pFig->Circle(M_MarkingSAGITTAL[i].m_y,M_MarkingSAGITTAL[i].m_z,4,&m_DC[0]);
		}
	}	
	m_DC[0].SelectObject(m_bkBrush);
	
	//####  Dots
	if(m_Selected_Drawing_Idx<0)return;
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
	for(i=0; i<pDraw->m_NumDots;i++){
		m_DC[0].SelectObject(m_rgbPEN1[pDraw->m_Dot[i].m_rgbID]);
		if(m_isCoronal_Or_Sagittal==CORONAL)pFig->Circle(pDraw->m_Dot[i].m_x,pDraw->m_Dot[i].m_z,2,&m_DC[0]);
		else                                pFig->Circle(pDraw->m_Dot[i].m_y,pDraw->m_Dot[i].m_z,2,&m_DC[0]);
	}		

	//####  Lines
	for(i=0; i<pDraw->m_NumLinedObj;i++){
		if(m_CurrentlyEditedObjIdx==i)pDraw->m_LinedObj[i].Draw_Obj(pFig, &m_DC[0], &objDC, m_rgbPEN1, m_isToEditObj, m_isCoronal_Or_Sagittal);
		else pDraw->m_LinedObj[i].Draw_Obj(pFig, &m_DC[0], &objDC, m_rgbPEN1, 0, m_isCoronal_Or_Sagittal);
	}
	Highlight_MarkedOnes();
}


void CAnalysisView::OnResetFigure()
{
	if(m_isCoronal_Or_Sagittal==-1 || m_isCoronal_Or_Sagittal==FROM_3D){return;}
	Init_Figure_AFTER_MRI_loading();
	Redraw_Everything();
}


void CAnalysisView::Init_Figure_AFTER_MRI_loading()
{
	if(m_MRI_Pixel_Dimension_In_mm<=0 || m_Pic_Loaded!=1){AfxMessageBox("m_MRI_Pixel_Dimension_In_mm<=0 || m_Pic_Loaded!=1"); exit(1);}
	m_dHowManyRows=1; m_dHowManyClmns=1;
	Re_measure_ClientView();
	if(m_bFistTime==1){Batch_SetFontSize(&m_DC[0]);}
	CFigure *pFig=&m_Fig[0][0][0]; 
	int AdjustedWindowHeight, AdjustedWindowWidth;
	AdjustedWindowHeight=m_savedClientRect.bottom-m_topOfFigs; //This is the available m_imgHeight to draw the picture
	AdjustedWindowWidth=m_savedClientRect.right-RightMargin;   //This is the available m_imgWidth  to draw the picture
	double Adjusted_wh_Ratio=AdjustedWindowWidth/(double)AdjustedWindowHeight;
	if(Adjusted_wh_Ratio<m_imgWidth/(double)m_imgHeight){
		//The adjusted (available) window is narrower than the given image raio. 
		//==> Use the  m_imgWidth of the window as the starting point.
		//AdjustedWindowHeight : AdjustedWindowWidth == m_imgHeight : m_imgWidth
		//AdjustedWindowHeight = AdjustedWindowWidth * (m_imgHeight/m_imgWidth)
		AdjustedWindowHeight=AdjustedWindowWidth*(m_imgHeight/(double)m_imgWidth);
	}
	else{// The m_imgWidth is shorter than the m_imgHeight
		AdjustedWindowWidth=AdjustedWindowHeight*(m_imgWidth/(double)m_imgHeight); 
	}
	m_dSizeOfOneRow=AdjustedWindowHeight; 
	m_dSizeOfOneClmn=AdjustedWindowWidth; 
	pFig->m_normTy=m_topOfFigs; pFig->m_normBy=pFig->m_normTy+m_dSizeOfOneRow;   
	pFig->m_normTx=0;        pFig->m_normBx=pFig->m_normTx+m_dSizeOfOneClmn;
	pFig->SetFig_Position_in_ClientView(pFig->m_normTx,pFig->m_normTy,pFig->m_normBx,pFig->m_normBy);
	m_MinX_corner= (0-m_OriginOfAbscissa_PixelCoordinate)*m_MRI_Pixel_Dimension_In_mm;
	m_MinZ_corner= (0-m_OriginOfOrdinate_PixelCoordinate)*m_MRI_Pixel_Dimension_In_mm;

	pFig->SetMinMaxXY((0-m_OriginOfAbscissa_PixelCoordinate)*m_MRI_Pixel_Dimension_In_mm, (m_imgWidth -m_OriginOfAbscissa_PixelCoordinate)*m_MRI_Pixel_Dimension_In_mm, 
					  (0-m_OriginOfOrdinate_PixelCoordinate)*m_MRI_Pixel_Dimension_In_mm, (m_imgHeight-m_OriginOfOrdinate_PixelCoordinate)*m_MRI_Pixel_Dimension_In_mm);	
	pFig->SetLabelsXY("","");	
	pFig->m_howManyX=-1; pFig->m_howManyY=-1; //Use the default drawing.
	pFig->Prepare4FigFrame(); 
	pFig->Frame_Graph2D(&m_DC[0],1);
}

void CAnalysisView::Disply_MRI_and_ColorLegend()
{
	CClientDC objDC(this);
	CFigure *pFig;
	pFig=&m_Fig[0][0][0];

	//####  Picture
	m_MRI[m_Selected_MRI_Idx].Draw_MRI(pFig, &objDC, 0);

	//####  Color bar legend
	int i,j;
	double step=((pFig->m_dMaxY-pFig->m_dMinY)/(double)BCGYRV_COLORS), BrightnessLevels=(RightMargin+30/*==GAP_XnY*/)/(double)BRIGHTNESS_LEVELS;
	int RectY= pFig->FromDataToScreenY(0)-pFig->FromDataToScreenY(step);
	int scrnX, scrnY;
	pFig->FromDataToScreenXY(pFig->m_dMaxX,pFig->m_dMaxY,scrnX, scrnY);
	for(i=0;i<BRIGHTNESS_LEVELS;i++){ 
		for(j=0;j<BCGYRV_COLORS;j++){ 
			m_DC[0].FillSolidRect(scrnX+BrightnessLevels*i, scrnY+RectY*j, BrightnessLevels,RectY,m_RGB60[i*BCGYRV_COLORS+j]);
		}
	}

	//#### The crosshair at 3D (0,0)
	if(m_toRegisterOrign==1 || m_ShowCrosshairOnce==1|| (m_isToDispOrigin==1 && m_MRI[m_Selected_MRI_Idx].m_Depth==0)){
		if(m_ShowCrosshairOnce==1)m_ShowCrosshairOnce=0;
		m_DC[0].SelectObject(m_BLUE);
		pFig->LineAtoB(0,-2*m_MRI_Pixel_Dimension_In_mm,0,+2*m_MRI_Pixel_Dimension_In_mm,&m_DC[0]);
		pFig->LineAtoB(-2*m_MRI_Pixel_Dimension_In_mm,0,+2*m_MRI_Pixel_Dimension_In_mm,0,&m_DC[0]);
	}
	//#### The crosshair at Grid (0,0)
	if(m_isCoronal_Or_Sagittal==CORONAL && m_MRI[m_Selected_MRI_Idx].m_Depth==*M_GridCenter_Y){
		m_DC[0].SelectObject(m_VIOLET);
		pFig->LineAtoB(*M_GridCenter_X-2*m_MRI_Pixel_Dimension_In_mm, *M_GridCenter_Z,*M_GridCenter_X+2*m_MRI_Pixel_Dimension_In_mm, *M_GridCenter_Z,&m_DC[0]);
		pFig->LineAtoB(*M_GridCenter_X, *M_GridCenter_Z-2*m_MRI_Pixel_Dimension_In_mm,*M_GridCenter_X, *M_GridCenter_Z+2*m_MRI_Pixel_Dimension_In_mm,&m_DC[0]);
	}
	if(m_isCoronal_Or_Sagittal==SAGITTAL && m_MRI[m_Selected_MRI_Idx].m_Depth==*M_GridCenter_X){
		m_DC[0].SelectObject(m_VIOLET);
		pFig->LineAtoB(*M_GridCenter_Y-2*m_MRI_Pixel_Dimension_In_mm, *M_GridCenter_Z,*M_GridCenter_Y+2*m_MRI_Pixel_Dimension_In_mm, *M_GridCenter_Z,&m_DC[0]);
		pFig->LineAtoB(*M_GridCenter_Y, *M_GridCenter_Z-2*m_MRI_Pixel_Dimension_In_mm,*M_GridCenter_Y, *M_GridCenter_Z+2*m_MRI_Pixel_Dimension_In_mm,&m_DC[0]);
	}
	m_DC[0].SetBkColor(RGB(255,255,255));
}


//3D (X, Z) --> coronal program's picture's (x, y)
//The Z axis in 3D is mapped to the program's internal y axis (not the monitor's y axis, which starts the upper left corner as 0 and downward as y+). 
//The X axis in 3D is mapped to the progarm's internal x axis.
void CMRI::Draw_MRI(CFigure *pFig, CClientDC *pDC, int toDisplayNow)
{
	int i,j,Mat_Sizexi,Mat_SizexIPlusJ;
	int Red, Green, Blue;
	//####  Picture
	pFig->FromDataToScreenXY((double)pFig->m_dMinX, (double)pFig->m_dMaxY, pFig->m_nPaneTx, pFig->m_nPaneTy); //It maps a relative point (i,j) of the graph to a point on the screen.	
	pFig->FromDataToScreenXY((double)pFig->m_dMaxX, (double)pFig->m_dMinY, pFig->m_nPaneBx, pFig->m_nPaneBy); //It maps a relative point (i,j) of the graph to a point on the screen.	

	int FirstXpixel =m_pView->ThD_to_MRI_Pixel_Coordinate_AbscissaAxis(pFig->m_dMinX ); if(FirstXpixel<0)FirstXpixel=0; if(FirstXpixel>m_pView->m_imgWidth-1 )FirstXpixel=m_pView->m_imgWidth-1;
	int LastXpixel  =m_pView->ThD_to_MRI_Pixel_Coordinate_AbscissaAxis(pFig->m_dMaxX); if(LastXpixel <0)LastXpixel=0;  if(LastXpixel >m_pView->m_imgWidth-1 )LastXpixel =m_pView->m_imgWidth-1;
	int FirstZpixel =m_pView->ThD_to_MRI_Pixel_Coordinate_OrdinateAxis(pFig->m_dMinY ); if(FirstZpixel<0)FirstZpixel=0; if(FirstZpixel>m_pView->m_imgHeight-1)FirstZpixel=m_pView->m_imgHeight-1;
	int LastZpixel  =m_pView->ThD_to_MRI_Pixel_Coordinate_OrdinateAxis(pFig->m_dMaxY); if(LastZpixel <0)LastZpixel=0;  if(LastZpixel >m_pView->m_imgHeight-1)LastZpixel =m_pView->m_imgHeight-1;

	double PixelLeft, PixelRight, PixelBottom, PixelTop, PixelWidth, PixelHeight;
	int RectX= pFig->FromDataToScreenX(m_pView->MRI_Pixel_to_3D_Coordinate_AbscissaAxis(1))-pFig->FromDataToScreenX(m_pView->MRI_Pixel_to_3D_Coordinate_AbscissaAxis(0))+2;
	int RectY=-pFig->FromDataToScreenY(m_pView->MRI_Pixel_to_3D_Coordinate_OrdinateAxis(1))+pFig->FromDataToScreenY(m_pView->MRI_Pixel_to_3D_Coordinate_OrdinateAxis(0))+2;
	PixelWidth = ( pFig->FromScreenToDataX(RectX)-pFig->FromScreenToDataX(0));
	PixelHeight= ( pFig->FromScreenToDataX(RectY)-pFig->FromScreenToDataX(0));
	if(m_pView->m_Pic_Loaded==1){
		for(i=FirstXpixel;i<=LastXpixel;i++){ 
			Mat_Sizexi=m_pView->m_imgHeight*i;
			for(j=FirstZpixel;j<=LastZpixel;j++){
				Mat_SizexIPlusJ=Mat_Sizexi+j;
				Red=   (int)(m_RforI[Mat_SizexIPlusJ]); if(Red  >254){Red  =255;} else if(Red<0  ){Red=0;} 
				Green= (int)(m_GforI[Mat_SizexIPlusJ]); if(Green>254){Green=255;} else if(Green<0){Green=0;}
				Blue=  (int)(m_BforI[Mat_SizexIPlusJ]); if(Blue >254){Blue =255;} else if(Blue<0 ){Blue=0;}   
				PixelLeft  =m_pView->MRI_Pixel_to_3D_Coordinate_AbscissaAxis(i);  PixelRight=PixelLeft+PixelWidth;
				PixelBottom=m_pView->MRI_Pixel_to_3D_Coordinate_OrdinateAxis(j);  PixelTop  =PixelBottom+PixelHeight;
				if(PixelLeft>=pFig->m_dMaxX||PixelRight<=pFig->m_dMinX || PixelBottom>=PixelTop||PixelTop<=pFig->m_dMinY)continue;
				if(PixelLeft<pFig->m_dMinX)PixelLeft=pFig->m_dMinX;if(PixelRight>pFig->m_dMaxX)PixelRight=pFig->m_dMaxX;
				if(PixelBottom<pFig->m_dMinY)PixelBottom=pFig->m_dMinY;if(PixelTop>pFig->m_dMaxY)PixelTop=pFig->m_dMaxY;
				pFig->FillSolidRect_XY_Rectangle(PixelLeft,PixelRight,PixelTop,PixelBottom,RGB(Red, Green, Blue), &m_pView->m_DC[0]);
				//pFig->FillSolidRect_XY_Centered(m_pView->MRI_Pixel_to_3D_Coordinate_AbscissaAxis(i),m_pView->MRI_Pixel_to_3D_Coordinate_OrdinateAxis(j),RectX,RectY,RGB(Red, Green, Blue), &m_pView->m_DC[0]);
		}	}
	}
	if(toDisplayNow==1)pFig->RefreshFrame(pDC, &m_pView->m_DC[0]);
}

void CAnalysisView::Draw_Neural_Info_CORONAL(double MRI_Depth) 
{
	CFigure *pFig=&m_Fig[0][0][0]; 
	int i;
	double x, y, z;

	for(i=0;i<m_HowManyNeurons;i++){
		x=m_Nn[i].m_x; y=m_Nn[i].m_y; z=m_Nn[i].m_z;
		if(DISTANCE_AB_1D(y, MRI_Depth)>NEURAL_DEPTH_RANGE_IN_MRI)continue;
		pFig->Circle(x,z,2,&m_DC[0]);
		
		if(m_Nn[i].m_type>=0 && m_Nn[i].m_type<m_numTextures){
			Draw_Texture(&CoronalTexture[m_Nn[i].m_type], x,z);
		}
	}
}
void CAnalysisView::Draw_Neural_Info_SAGITTAL(double MRI_Depth) 
{
	CFigure *pFig=&m_Fig[0][0][0]; 
	int i;
	double x, y, z;

	for(i=0;i<m_HowManyNeurons;i++){
		x=m_Nn[i].m_x; y=m_Nn[i].m_y; z=m_Nn[i].m_z;
		if(DISTANCE_AB_1D(x, MRI_Depth)>NEURAL_DEPTH_RANGE_IN_MRI)continue;
		pFig->Circle(y,z,2,&m_DC[0]);
		
		if(m_Nn[i].m_type>=0 && m_Nn[i].m_type<m_numTextures){
			Draw_Texture(&CoronalTexture[m_Nn[i].m_type], y,z);
		}
	}
}


void CAnalysisView::Draw_Texture(TextureForCORONAL *pTexture, double X, double Y)
{
#define SCALE_BOX 0.5
	CClientDC objDC(this);
	CFigure *pFig;
	pFig=&m_Fig[0][0][0];
	int i,j,Mat_Sizexi,Mat_SizexIPlusJ;
	//####  Picture
	int RectX=-pFig->FromDataToScreenX(0)+pFig->FromDataToScreenX(m_MRI_Pixel_Dimension_In_mm)+1;
	int RectY= pFig->FromDataToScreenY(0)-pFig->FromDataToScreenY(m_MRI_Pixel_Dimension_In_mm)+1;
	int Red, Green , Blue, height=pTexture->m_imgHeight, width=pTexture->m_imgWidth;
	double MiniX=pFig->m_dMinX, MiniY=pFig->m_dMinY;

	double TextureScale=0.5;

	if(m_CoronalTexture_Loaded==1){
		for(i=0;i<width;i++){ 
			Mat_Sizexi=height*i;
			for(j=0;j<height;j++){
				Mat_SizexIPlusJ=Mat_Sizexi+j;
				Red=   (int)(pTexture->m_RforI[Mat_SizexIPlusJ]); if(Red  >254){Red  =255;} else if(Red<0  ){Red=0;} 
				Green= (int)(pTexture->m_GforI[Mat_SizexIPlusJ]); if(Green>254){Green=255;} else if(Green<0){Green=0;}
				Blue=  (int)(pTexture->m_BforI[Mat_SizexIPlusJ]); if(Blue >254){Blue =255;} else if(Blue<0 ){Blue=0;}    
				m_DC[0].FillSolidRect(i*TextureScale+pFig->FromDataToScreenX(X)-(width*TextureScale*0.5),-j*TextureScale+pFig->FromDataToScreenY(Y)+(height*TextureScale*0.5),1,1,RGB(Red, Green, Blue));
		}	}
	}
}

void CAnalysisView::Highlight_MarkedOnes() 
{
	if(m_Selected_Drawing_Idx<0)return;
	CFigure *pFig=&m_Fig[0][0][0]; 
	CClientDC objDC(this);
	int i,idx;
	CDrawing *pDraw=&m_Drawing[m_Selected_Drawing_Idx];

	for(i=0; i<m_NumSelectedMarkings;i++){
		idx=m_SelectedMarkings[i];
		if(m_isCoronal_Or_Sagittal==CORONAL){
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingCORONAL[idx].m_rgbID]);
			objDC.SelectObject(m_rgbPEN1[M_MarkingCORONAL[idx].m_rgbID]);
			pFig->Circle(M_MarkingCORONAL[idx].m_x,M_MarkingCORONAL[idx].m_z,6,&objDC);
			pFig->Circle(M_MarkingCORONAL[idx].m_x,M_MarkingCORONAL[idx].m_z,6,&m_DC[0]);
		}
		else{
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingSAGITTAL[idx].m_rgbID]);
			objDC.SelectObject(m_rgbPEN1[M_MarkingSAGITTAL[idx].m_rgbID]);
			pFig->Circle(M_MarkingSAGITTAL[idx].m_y,M_MarkingSAGITTAL[idx].m_z,6,&objDC);
			pFig->Circle(M_MarkingSAGITTAL[idx].m_y,M_MarkingSAGITTAL[idx].m_z,6,&m_DC[0]);
		}
	}
	for(i=0; i<m_NumSelectedDots;i++){
		idx=m_SelectedDots[i];
		m_DC[0].SelectObject(m_rgbPEN1[pDraw->m_Dot[idx].m_rgbID]);
		objDC.SelectObject(m_rgbPEN1[pDraw->m_Dot[idx].m_rgbID]);
		if(m_isCoronal_Or_Sagittal==CORONAL){
			pFig->Circle(pDraw->m_Dot[idx].m_x,pDraw->m_Dot[idx].m_z,4,&objDC);
			pFig->Circle(pDraw->m_Dot[idx].m_x,pDraw->m_Dot[idx].m_z,4,&m_DC[0]);
		}
		else{
			pFig->Circle(pDraw->m_Dot[idx].m_y,pDraw->m_Dot[idx].m_z,4,&objDC);
			pFig->Circle(pDraw->m_Dot[idx].m_y,pDraw->m_Dot[idx].m_z,4,&m_DC[0]);
		}
	}
	for(i=0; i<m_NumSelectedObjs;i++){
		pDraw->m_LinedObj[m_SelectedObjs[i]].Highlight_Obj(pFig, &m_DC[0], &objDC, m_rgbPEN2, m_isCoronal_Or_Sagittal);
	}
	m_DC[0].SetBkColor(RGB(255,255,255));
}

void CAnalysisView::RegisterMarkingForSelection(int MarkingIdxToRegister) 
{
	int i;
	for(i=0; i<m_NumSelectedMarkings;i++){
		if(m_SelectedMarkings[i]==MarkingIdxToRegister)return;
	}
	if(m_NumSelectedMarkings>=m_SizeOfSelectedMarkings-1){m_SelectedMarkings.SetSize(2*m_SelectedMarkings.GetSize());m_SizeOfSelectedMarkings=m_SelectedMarkings.GetSize();}
	m_SelectedMarkings[m_NumSelectedMarkings++]=MarkingIdxToRegister;				
}

void CAnalysisView::RegisterDotForSelection(int PointIdxToRegister) 
{
	int i;
	for(i=0; i<m_NumSelectedDots;i++){
		if(m_SelectedDots[i]==PointIdxToRegister)return;
	}
	if(m_NumSelectedDots>=m_SizeOfSelectedDots-1){m_SelectedDots.SetSize(2*m_SelectedDots.GetSize());m_SizeOfSelectedDots=m_SelectedDots.GetSize();}
	m_SelectedDots[m_NumSelectedDots++]=PointIdxToRegister;				
}

void CAnalysisView::RegisterObjForSelection(int ObjIdxToRegister) 
{
	int i;
	for(i=0; i<m_NumSelectedObjs;i++){
		if(m_SelectedObjs[i]==ObjIdxToRegister)return;
	}
	if(m_NumSelectedObjs>=m_SizeOfSelectedObjs-1){m_SelectedObjs.SetSize(2*m_SelectedObjs.GetSize());m_SizeOfSelectedObjs=m_SelectedObjs.GetSize();}
	m_SelectedObjs[m_NumSelectedObjs++]=ObjIdxToRegister;				
}

void CAnalysisView::Highlight_MarkedOnes_withTranslation(double Horizontal, double TransZ) 
{
	if(m_Selected_Drawing_Idx<0)return;
	CFigure *pFig=&m_Fig[0][0][0]; 
	CClientDC objDC(this);
	int i,idx;
	CDrawing *pDraw=&m_Drawing[m_Selected_Drawing_Idx];

	for(i=0; i<m_NumSelectedMarkings;i++){
		idx=m_SelectedMarkings[i];
		if(m_isCoronal_Or_Sagittal==CORONAL){
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingCORONAL[idx].m_rgbID]);
			objDC.SelectObject(m_rgbPEN1[M_MarkingCORONAL[idx].m_rgbID]);
			pFig->Circle(M_MarkingCORONAL[idx].m_x+Horizontal,M_MarkingCORONAL[idx].m_z+TransZ,6,&objDC); pFig->Circle(M_MarkingCORONAL[idx].m_x+Horizontal,M_MarkingCORONAL[idx].m_z+TransZ,6,&m_DC[0]);
		}
		else{
			m_DC[0].SelectObject(m_rgbPEN1[M_MarkingSAGITTAL[idx].m_rgbID]);
			objDC.SelectObject(m_rgbPEN1[M_MarkingSAGITTAL[idx].m_rgbID]);
			pFig->Circle(M_MarkingSAGITTAL[idx].m_y+Horizontal,M_MarkingSAGITTAL[idx].m_z+TransZ,6,&objDC); pFig->Circle(M_MarkingSAGITTAL[idx].m_y+Horizontal,M_MarkingSAGITTAL[idx].m_z+TransZ,6,&m_DC[0]);
		}
	}

	for(i=0; i<m_NumSelectedDots;i++){
		idx=m_SelectedDots[i];
		m_DC[0].SelectObject(m_rgbPEN1[pDraw->m_Dot[idx].m_rgbID]);
		objDC.SelectObject(m_rgbPEN1[pDraw->m_Dot[idx].m_rgbID]);
		if(m_isCoronal_Or_Sagittal==CORONAL){pFig->Circle(pDraw->m_Dot[idx].m_x+Horizontal,pDraw->m_Dot[idx].m_z+TransZ,4,&objDC); pFig->Circle(pDraw->m_Dot[idx].m_x+Horizontal,pDraw->m_Dot[idx].m_z+TransZ,4,&m_DC[0]);}
		else                               {pFig->Circle(pDraw->m_Dot[idx].m_y+Horizontal,pDraw->m_Dot[idx].m_z+TransZ,4,&objDC); pFig->Circle(pDraw->m_Dot[idx].m_y+Horizontal,pDraw->m_Dot[idx].m_z+TransZ,4,&m_DC[0]);}
	}

	if(m_isCoronal_Or_Sagittal==CORONAL)for(i=0; i<m_NumSelectedObjs;i++)pDraw->m_LinedObj[m_SelectedObjs[i]].Draw_Obj_RelativeToXY_CORONAL(Horizontal,TransZ, pFig, &m_DC[0], &objDC, m_rgbPEN2);
	else                               for(i=0; i<m_NumSelectedObjs;i++)pDraw->m_LinedObj[m_SelectedObjs[i]].Draw_Obj_RelativeToXY_SAGITTAL(Horizontal,TransZ, pFig, &m_DC[0], &objDC, m_rgbPEN2);
}

void CAnalysisView::Add_VertexIn3D(double x, double y, double z, int RGBID, int type, int LineHeadOrTail, int idx)
{
	CDrawing *pDraw=Select_Current_Drawing(m_Selected_Drawing_Idx);
	if(pDraw->m_NumDots>=pDraw->m_Dot.GetSize()-1)pDraw->m_Dot.SetSize(2*(pDraw->m_Dot.GetSize()+1));
	pDraw->m_Dot[pDraw->m_NumDots].m_x=x; 	pDraw->m_Dot[pDraw->m_NumDots].m_y=y;	pDraw->m_Dot[pDraw->m_NumDots].m_z=z;
	pDraw->m_Dot[pDraw->m_NumDots].m_rgbID=RGBID; 	
	pDraw->m_Dot[pDraw->m_NumDots].m_type=type; 
	pDraw->m_Dot[pDraw->m_NumDots].m_LineHeadOrTail=LineHeadOrTail; 
	pDraw->m_NumDots++;
	m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.
}

void CAnalysisView::Add_MarkingIn3D(double x, double y, double z, int RGBID, int type, int idx)
{
	if(m_isCoronal_Or_Sagittal==CORONAL){
		if(*M_NumberOfMarkingsCORONAL>=MAX_MARKINGS-1){AfxMessageBox("*M_NumberOfMarkings>=MAX_MARKINGS-1"); return;}
		M_MarkingCORONAL[*M_NumberOfMarkingsCORONAL].m_x=x; 	M_MarkingCORONAL[*M_NumberOfMarkingsCORONAL].m_y=y;	M_MarkingCORONAL[*M_NumberOfMarkingsCORONAL].m_z=z;
		M_MarkingCORONAL[*M_NumberOfMarkingsCORONAL].m_rgbID=RGBID; 	
		M_MarkingCORONAL[*M_NumberOfMarkingsCORONAL].m_MarkingType=type; 
		(*M_NumberOfMarkingsCORONAL)++;
	}
	else{
		if(*M_NumberOfMarkingsSAGITTAL>=MAX_MARKINGS-1){AfxMessageBox("*M_NumberOfMarkings>=MAX_MARKINGS-1"); return;}
		M_MarkingSAGITTAL[*M_NumberOfMarkingsSAGITTAL].m_x=x; 	M_MarkingSAGITTAL[*M_NumberOfMarkingsSAGITTAL].m_y=y;	M_MarkingSAGITTAL[*M_NumberOfMarkingsSAGITTAL].m_z=z;
		M_MarkingSAGITTAL[*M_NumberOfMarkingsSAGITTAL].m_rgbID=RGBID; 	
		M_MarkingSAGITTAL[*M_NumberOfMarkingsSAGITTAL].m_MarkingType=type; 
		(*M_NumberOfMarkingsSAGITTAL)++;
	}
	m_NeedsToSave=1; //When the user tries to change/quit the current MRI, save it first.

	Redraw_Everything();
	//Inform the 3D display
	SetEvent(M_KeyDownEvents[1]);
}

void CAnalysisView::Delete_MarkedOnes() 
{		
	if(m_Selected_Drawing_Idx<0)return;
	int i,j, idx;
	CDrawing *pDraw=&m_Drawing[m_Selected_Drawing_Idx];

	//Markings
	BubbleSort(&m_SelectedMarkings[0],m_NumSelectedMarkings);
    for(i=m_NumSelectedMarkings-1; i>=0;i--){
		idx=m_SelectedMarkings[i];
		if(m_isCoronal_Or_Sagittal==CORONAL){
			for(j=idx+1; j<*M_NumberOfMarkingsCORONAL; j++){
				M_MarkingCORONAL[j-1].m_x=M_MarkingCORONAL[j].m_x; 	
				M_MarkingCORONAL[j-1].m_y=M_MarkingCORONAL[j].m_y;	
				M_MarkingCORONAL[j-1].m_z=M_MarkingCORONAL[j].m_z;
				M_MarkingCORONAL[j-1].m_rgbID=M_MarkingCORONAL[j].m_rgbID; 	
				M_MarkingCORONAL[j-1].m_MarkingType=M_MarkingCORONAL[j].m_MarkingType; 
			}
			(*M_NumberOfMarkingsCORONAL)--; 
		}
		else{
			for(j=idx+1; j<*M_NumberOfMarkingsSAGITTAL; j++){
				M_MarkingSAGITTAL[j-1].m_x=M_MarkingSAGITTAL[j].m_x; 	
				M_MarkingSAGITTAL[j-1].m_y=M_MarkingSAGITTAL[j].m_y;	
				M_MarkingSAGITTAL[j-1].m_z=M_MarkingSAGITTAL[j].m_z;
				M_MarkingSAGITTAL[j-1].m_rgbID=M_MarkingSAGITTAL[j].m_rgbID; 	
				M_MarkingSAGITTAL[j-1].m_MarkingType=M_MarkingSAGITTAL[j].m_MarkingType; 
			}
			(*M_NumberOfMarkingsSAGITTAL)--; 
		}
	}
	m_NumSelectedMarkings=0;

	//Dots
	BubbleSort(&m_SelectedDots[0],m_NumSelectedDots);
    for(i=m_NumSelectedDots-1; i>=0;i--){
		idx=m_SelectedDots[i];
		pDraw->m_Dot.RemoveAt(idx);
		pDraw->m_NumDots--; 
	}
	if(pDraw->m_NumDots<0)AfxMessageBox("m_NumDots<0");
	m_NumSelectedDots=0;

	//Lines
	BubbleSort(&m_SelectedObjs[0],m_NumSelectedObjs);
	for(i=m_NumSelectedObjs-1; i>=0;i--){
		idx=m_SelectedObjs[i];
		pDraw->m_LinedObj.RemoveAt(idx);
		pDraw->m_NumLinedObj--;
	}
	if(pDraw->m_NumLinedObj<0)AfxMessageBox("m_NumLinedObj<0");
	m_NumSelectedObjs=0;

	m_GrabbedObjectIdx=-1;
	m_GrabbedDotIdx=-1;
	m_GrabbedMarkingIdx=-1;
}

void CAnalysisView::Delete_AllDrawingObjects() 
{		
	int i,j, idx;
	if(m_Drawing.GetSize()<1 || m_Selected_Drawing_Idx<0)return;
	CDrawing *pDraw=&m_Drawing[m_Selected_Drawing_Idx];

	if(m_isCoronal_Or_Sagittal==CORONAL){
		(*M_NumberOfMarkingsCORONAL)=0; 
	}
	else{
		(*M_NumberOfMarkingsSAGITTAL)=0; 
	}
 
	
	for(i=pDraw->m_NumDots-1; i>=0;i--){
		pDraw->m_Dot.RemoveAt(i);
	}
	pDraw->m_NumDots=0; 
	m_NumSelectedDots=0;

	for(i=pDraw->m_NumLinedObj-1; i>=0;i--){
		pDraw->m_LinedObj.RemoveAt(i);
	}
	pDraw->m_NumLinedObj=0;
	m_NumSelectedObjs=0;
	m_GrabbedObjectIdx=-1;
	m_GrabbedDotIdx=-1;
	m_GrabbedMarkingIdx=-1;
}