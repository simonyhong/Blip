
#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"


//Reads the 2D & 3D configuration 
void CAnalysisView::Read_3D_2D_Config()
{
	if(m_isCoronal_Or_Sagittal==-1){AfxMessageBox("Did nothing."); return;}

	FILE *fp;
	int i;
	float dummyF=1.0;
	double NumberOfDoublesFollowing, futureUse;
	char ConfigFile[1028];
	sprintf(ConfigFile,"%s\\_3D_2D_Config.txt",M_DataDir);
	if((fp=fopen(ConfigFile,"r"))!= NULL){
		fscanf(fp,"%f",&dummyF);// Version numbere

		//Pixel scale (horizontal and vertial dimensions the same)
		fscanf(fp,"%lf",&m_MRI_Pixel_Dimension_In_mm );  

		//2D Coronal image: origin
		fscanf(fp,"%lf",&m_OriginOfAbscissa_MRIPixelCoordinate_Coronal_JustForSaveAndRead );  fscanf(fp,"%lf",&m_OriginOfOrdinate_MRIPixelCoordinate_Coronal_JustForSaveAndRead );	
		//2D Sagittal image: origin
		fscanf(fp,"%lf",&m_OriginOfAbscissa_MRIPixelCoordinate_Sagittal_JustForSaveAndRead );  fscanf(fp,"%lf",&m_OriginOfOrdinate_MRIPixelCoordinate_Sagittal_JustForSaveAndRead );	

		//Grid origin & its pitch, roll and yaw
		fscanf(fp,"%lf",M_GridCenter_X );  fscanf(fp,"%lf",M_GridCenter_Y );  fscanf(fp,"%lf",M_GridCenter_Z );	
		fscanf(fp,"%lf",M_Grid_Pitch );  fscanf(fp,"%lf",M_Grid_Roll );  fscanf(fp,"%lf",M_Grid_Yaw );	
		
		//Future use
		fscanf(fp,"%lf",&NumberOfDoublesFollowing );
		for(i=0; i<NumberOfDoublesFollowing; i++){
			fscanf(fp,"%lf",&futureUse );
		}
		fclose(fp);	

		if(m_isCoronal_Or_Sagittal==CORONAL){
			m_OriginOfAbscissa_PixelCoordinate=m_OriginOfAbscissa_MRIPixelCoordinate_Coronal_JustForSaveAndRead;  
			m_OriginOfOrdinate_PixelCoordinate=m_OriginOfOrdinate_MRIPixelCoordinate_Coronal_JustForSaveAndRead;	
		}
		else if(m_isCoronal_Or_Sagittal==SAGITTAL){
			m_OriginOfAbscissa_PixelCoordinate=m_OriginOfAbscissa_MRIPixelCoordinate_Sagittal_JustForSaveAndRead;  
			m_OriginOfOrdinate_PixelCoordinate=m_OriginOfOrdinate_MRIPixelCoordinate_Sagittal_JustForSaveAndRead;	
		}
	}
	else{AfxMessageBox("Hello there!\nCould not find \"_3D_2D_Config.txt\" \nPlease set the origin and scale first.\n Go to Tools-->Set Origin"); }
}

//Writes the 2D & 3D configuration 
void CAnalysisView::Save_3D_2D_Config()
{
	if(m_isCoronal_Or_Sagittal==-1){AfxMessageBox("Did nothing."); return;}
	if(m_isCoronal_Or_Sagittal==CORONAL){
		m_OriginOfAbscissa_MRIPixelCoordinate_Coronal_JustForSaveAndRead=m_OriginOfAbscissa_PixelCoordinate;  
		m_OriginOfOrdinate_MRIPixelCoordinate_Coronal_JustForSaveAndRead=m_OriginOfOrdinate_PixelCoordinate;	
	}
	else if(m_isCoronal_Or_Sagittal==SAGITTAL){
		m_OriginOfAbscissa_MRIPixelCoordinate_Sagittal_JustForSaveAndRead=m_OriginOfAbscissa_PixelCoordinate;  
		m_OriginOfOrdinate_MRIPixelCoordinate_Sagittal_JustForSaveAndRead=m_OriginOfOrdinate_PixelCoordinate;	
	}

	FILE *fp;
	char ConfigFile[1028];
	int i;
	double dummyDouble, futureUse=0, NumberOfDoublesFollowing=5;
	sprintf(ConfigFile,"%s\\_3D_2D_Config.txt",M_DataDir);
	if((fp=fopen(ConfigFile,"w"))!= NULL){
		// Version number
		fprintf(fp,"%0.2f\n\n",VERSION_NUM_OF_VIRTUAL_ELECTRODE);
		
		//Pixel dimension (horizontal and vertial dimensions the same)
		fprintf(fp,"%0.5lf\n"  ,m_MRI_Pixel_Dimension_In_mm );  

		//Coronal MRI Origin
		fprintf(fp,"%0.5lf\n"  ,m_OriginOfAbscissa_MRIPixelCoordinate_Coronal_JustForSaveAndRead );  
		fprintf(fp,"%0.5lf\n\n",m_OriginOfOrdinate_MRIPixelCoordinate_Coronal_JustForSaveAndRead );	
		//Sagittal MRI Origin
		fprintf(fp,"%0.5lf\n"  ,m_OriginOfAbscissa_MRIPixelCoordinate_Sagittal_JustForSaveAndRead );  
		fprintf(fp,"%0.5lf\n\n",m_OriginOfOrdinate_MRIPixelCoordinate_Sagittal_JustForSaveAndRead );	
		
		//Grid center & pitch, roll, yaw
		fprintf(fp,"%0.5lf\n",*M_GridCenter_X );  fprintf(fp,"%0.5lf\n",*M_GridCenter_Y );  fprintf(fp,"%0.5lf\n",*M_GridCenter_Z );	
		fprintf(fp,"%0.5lf\n",*M_Grid_Pitch );  fprintf(fp,"%0.5lf\n",*M_Grid_Roll );  fprintf(fp,"%0.5lf\n\n",*M_Grid_Yaw );
		
		//Future use
		fprintf(fp,"%0.0lf\n",NumberOfDoublesFollowing );  
		for(i=0; i<NumberOfDoublesFollowing; i++){
			fprintf(fp,"%0.5lf\n",futureUse );
		}

		fclose(fp);	
	}
}
	
//Reads the drawing data. 
void CAnalysisView::Load_DrawingData(int mode) 
{
	FILE *fp;
	char FileName[512], Dump[512], DrawingFile[1024], dummySt[1024];
	double x,y,z, r,g,b, Depth, prevDepth=-100000000*ONE_PI*ONE_PI;
	int i, j, Idx, type, LineHeadOrTail, StrLength, PrevCoronalSagittal=-100000*ONE_PI*ONE_PI;
	char date[64], SavedDate[64];
	int isCoronal, savedDotCnt, NumDrawings, ImgCnt=0, FirstPass=1, dummyInt, CoronaOrSagittal;
	CDrawing *pDraw;
	CLinedObj* pObj;
	Delete_AllDrawingObjects(); //Delete all the previous drawings.


	if(mode==CORONAL )sprintf(FileName,"_DrawingData_CORONAL.txt");
	if(mode==SAGITTAL)sprintf(FileName,"_DrawingData_SAGITTAL.txt");
	DoItAgain:;
	if(mode==FROM_3D && FirstPass==1)sprintf(FileName,"_DrawingData_CORONAL.txt");
	if(mode==FROM_3D && FirstPass==0)sprintf(FileName,"_DrawingData_SAGITTAL.txt");
	sprintf(DrawingFile,"%s\\%s",M_DataDir, FileName);
	//Read data
	if((fp=fopen(DrawingFile,"r"))!= NULL){
		//### Read ###
		fgets(Dump,512,fp);//-->This is just to get rid of the whole header line including the carriage return
		fscanf(fp,"%s",dummySt );//	Number_of_Markings:	
		if(strcmp(FileName, "_DrawingData_CORONAL.txt")==0){
			fscanf( fp, "%d",M_NumberOfMarkingsCORONAL);
			for(int markings=0; markings<*M_NumberOfMarkingsCORONAL; markings++){
				fscanf( fp, "%d",&dummyInt);
				fscanf( fp, "%lf%lf%lf%d%lf%lf%lf",&x, &y, &z, &type, &r, &g, &b);
				if(type!=MARKING_TYPE){
					AfxMessageBox("type!=MARKING_TYPE");
				}
				M_MarkingCORONAL[markings].m_x=x;
				M_MarkingCORONAL[markings].m_y=y;
				M_MarkingCORONAL[markings].m_z=z;
				M_MarkingCORONAL[markings].m_MarkingType=type;
				M_MarkingCORONAL[markings].r=r;M_MarkingCORONAL[markings].g=g;M_MarkingCORONAL[markings].b=b;
				M_MarkingCORONAL[markings].m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
			}
		}
		if(strcmp(FileName, "_DrawingData_SAGITTAL.txt")==0){
			fscanf( fp, "%d",M_NumberOfMarkingsSAGITTAL);
			for(int markings=0; markings<*M_NumberOfMarkingsSAGITTAL; markings++){
				fscanf( fp, "%d",&dummyInt);
				fscanf( fp, "%lf%lf%lf%d%lf%lf%lf",&x, &y, &z, &type, &r, &g, &b);
				if(type!=MARKING_TYPE){
					AfxMessageBox("type!=MARKING_TYPE");
				}
				M_MarkingSAGITTAL[markings].m_x=x;
				M_MarkingSAGITTAL[markings].m_y=y;
				M_MarkingSAGITTAL[markings].m_z=z;
				M_MarkingSAGITTAL[markings].m_MarkingType=type;
				M_MarkingSAGITTAL[markings].r=r;M_MarkingSAGITTAL[markings].g=g;M_MarkingSAGITTAL[markings].b=b;
				M_MarkingSAGITTAL[markings].m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
			}
		}
		fscanf(fp,"%s",dummySt );//	Number_of_Drawings:	
		fscanf( fp, "%d",&NumDrawings);
		if(NumDrawings+ImgCnt>=m_Drawing.GetSize()-1){m_Drawing.SetSize(NumDrawings+ImgCnt,50);}
		for(int img=0; img<NumDrawings; img++){
			pDraw=&m_Drawing[ImgCnt];
			if(fscanf( fp, "%d%d%lf%d%d",&Idx,&CoronaOrSagittal, &pDraw->m_Depth, &pDraw->m_NumDots, &pDraw->m_NumLinedObj)==EOF){AfxMessageBox("End of File"); break;}
			if(pDraw->m_NumDots>=pDraw->m_Dot.GetSize()-1){pDraw->m_Dot.SetSize(pDraw->m_NumDots,50);}
			if(pDraw->m_NumLinedObj>=pDraw->m_LinedObj.GetSize()-1){pDraw->m_LinedObj.SetSize(pDraw->m_NumLinedObj,50);}
			for(i=0; i<pDraw->m_NumDots;i++){
				fscanf( fp, "%lf%lf%lf%d%lf%lf%lf%d",&x, &y, &z, &type, &r, &g, &b, &LineHeadOrTail);
				if(type!=DOT_TYPE){
					AfxMessageBox("type!=DOT_TYPE");
				}
				pDraw->m_Dot[i].m_x=x;
				pDraw->m_Dot[i].m_y=y;
				pDraw->m_Dot[i].m_z=z;
				pDraw->m_Dot[i].m_type=type;
				pDraw->m_Dot[i].r=r;pDraw->m_Dot[i].g=g;pDraw->m_Dot[i].b=b;
				pDraw->m_Dot[i].m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
				pDraw->m_Dot[i].m_LineHeadOrTail=LineHeadOrTail;
			}

			for(i=0; i<pDraw->m_NumLinedObj;i++){
				pObj=&(pDraw->m_LinedObj[i]);
				fscanf( fp, "%d",&pObj->m_NumVertices);
				if(pObj->m_NumVertices>=pObj->m_Vtx.GetSize()-1)pObj->m_Vtx.SetSize(pObj->m_NumVertices,50);
				for(j=0; j<pObj->m_NumVertices;j++){
					fscanf( fp, "%lf%lf%lf%d%lf%lf%lf%d",&x, &y, &z, &type, &r, &g, &b, &LineHeadOrTail);
					if(type!=LINE_TYPE)AfxMessageBox("type!=LINE_TYPE");
					pObj->m_Vtx[j].m_x=x;
					pObj->m_Vtx[j].m_y=y;
					pObj->m_Vtx[j].m_z=z;
					pObj->m_Vtx[j].m_type=type;
					pObj->m_Vtx[j].r=r;pObj->m_Vtx[j].g=g;pObj->m_Vtx[j].b=b;
					pObj->m_Vtx[j].m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
					pObj->m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
					pObj->m_Vtx[j].m_LineHeadOrTail=LineHeadOrTail;
					if(LineHeadOrTail==TAIL){
						pObj->UpdateBoundary(m_isCoronal_Or_Sagittal);
						pObj->m_rgbID=pObj->m_Vtx[pObj->m_NumVertices-1].m_rgbID;
					}
				}
			}
			if(mode==FROM_3D || mode==CoronaOrSagittal){ImgCnt++;}
		}
		fclose(fp);
		if(mode==FROM_3D && FirstPass==1){FirstPass=0; goto DoItAgain;}
	}
	m_NumDrawings=ImgCnt;
	Sort_Drawing_Using_Depth();
	m_DrawingLoaded=1;
}

//Writes the drawing data
void CAnalysisView::OnFileSaveDrawing()
{
	if(*M_isCoronal_Running==0 && *M_isSagittal_Running==0 && *M_is3D_Running==0){AfxMessageBox("Hello there:\n Run Coronal/Sagittal/3D program first."); return;}

	if(m_isCoronal_Or_Sagittal==FROM_3D){
		Save_Neuron_Data(1,1,1);
		AfxMessageBox("Neural Info Saved");
	}
	else{
		ProperlyFinish();//In the case the user was editing a line...
		//First remove any empty Drawings.
		RemoveEmptyDrawings();

		FILE *fp;
		int i,j, Type;
		char DrawingFile[1024];
		CLinedObj* pObj;
		CDrawing *pDraw;
		CVertex *pDot;

		//### Write ###
		if(m_isCoronal_Or_Sagittal==CORONAL )sprintf(DrawingFile,"%s\\_DrawingData_CORONAL.txt",M_DataDir);
		if(m_isCoronal_Or_Sagittal==SAGITTAL)sprintf(DrawingFile,"%s\\_DrawingData_SAGITTAL.txt",M_DataDir);
		if((fp=fopen(DrawingFile,"w"))!= NULL){
			fprintf( fp, "Version: %0.1lf\n", ANATOMY_3D_VERSION);
			if(m_isCoronal_Or_Sagittal==CORONAL){
				fprintf( fp, "M_NumberOfMarkingsCORONAL: %d\n", *M_NumberOfMarkingsCORONAL);
				for(int Idx=0; Idx<*M_NumberOfMarkingsCORONAL; Idx++){
					fprintf( fp, "%d\n", Idx);
					M_MarkingCORONAL[Idx].r=GetRValue(m_RGB60[M_MarkingCORONAL[Idx].m_rgbID])/255.; 
					M_MarkingCORONAL[Idx].g=GetGValue(m_RGB60[M_MarkingCORONAL[Idx].m_rgbID])/255.;
					M_MarkingCORONAL[Idx].b=GetBValue(m_RGB60[M_MarkingCORONAL[Idx].m_rgbID])/255.;
					fprintf( fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\n",M_MarkingCORONAL[Idx].m_x ,M_MarkingCORONAL[Idx].m_y, M_MarkingCORONAL[Idx].m_z, MARKING_TYPE, M_MarkingCORONAL[Idx].r,M_MarkingCORONAL[Idx].g,M_MarkingCORONAL[Idx].b);
				}
			}
			else{
				fprintf( fp, "M_NumberOfMarkingsSAGITTAL: %d\n", *M_NumberOfMarkingsSAGITTAL);
				for(int Idx=0; Idx<*M_NumberOfMarkingsSAGITTAL; Idx++){
					fprintf( fp, "%d\n", Idx);
					M_MarkingSAGITTAL[Idx].r=GetRValue(m_RGB60[M_MarkingSAGITTAL[Idx].m_rgbID])/255.; 
					M_MarkingSAGITTAL[Idx].g=GetGValue(m_RGB60[M_MarkingSAGITTAL[Idx].m_rgbID])/255.;
					M_MarkingSAGITTAL[Idx].b=GetBValue(m_RGB60[M_MarkingSAGITTAL[Idx].m_rgbID])/255.;
					fprintf( fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\n",M_MarkingSAGITTAL[Idx].m_x ,M_MarkingSAGITTAL[Idx].m_y, M_MarkingSAGITTAL[Idx].m_z, MARKING_TYPE, M_MarkingSAGITTAL[Idx].r,M_MarkingSAGITTAL[Idx].g,M_MarkingSAGITTAL[Idx].b);
				}
			}
			fprintf( fp, "Number_Of_Drawings: %d\n", m_NumDrawings);
			for(int Idx=0; Idx<m_NumDrawings; Idx++){
				pDraw=&m_Drawing[Idx];
				fprintf( fp, "%d\n", Idx);
				fprintf( fp, "%d\n", m_isCoronal_Or_Sagittal);
				fprintf( fp, "%0.3lf\n", pDraw->m_Depth);
				fprintf( fp, "%d\n", pDraw->m_NumDots);
				fprintf( fp, "%d\n", pDraw->m_NumLinedObj);
				for(i=0; i<pDraw->m_NumDots;i++){
					pDot=&pDraw->m_Dot[i];
					pDot->r=GetRValue(m_RGB60[pDot->m_rgbID])/255.; 
					pDot->g=GetGValue(m_RGB60[pDot->m_rgbID])/255.;
					pDot->b=GetBValue(m_RGB60[pDot->m_rgbID])/255.;
					fprintf( fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\t%d\n",pDot->m_x ,pDot->m_y, pDot->m_z, DOT_TYPE, pDot->r,pDot->g,pDot->b,BODY);
				}
				for(i=0; i<pDraw->m_NumLinedObj;i++){
					pObj=&pDraw->m_LinedObj[i];
					if(pObj->m_NumVertices<2)AfxMessageBox("pObj->m_NumVertices<2");
					fprintf( fp, "%d\n", pObj->m_NumVertices);
					for(j=0; j<pObj->m_NumVertices;j++){
						if(j==0)Type=HEAD; else if(j==pObj->m_NumVertices-1)Type=TAIL; else Type=BODY;
						pObj->m_Vtx[j].r=GetRValue(m_RGB60[pObj->m_Vtx[j].m_rgbID])/255.; 
						pObj->m_Vtx[j].g=GetGValue(m_RGB60[pObj->m_Vtx[j].m_rgbID])/255.;
						pObj->m_Vtx[j].b=GetBValue(m_RGB60[pObj->m_Vtx[j].m_rgbID])/255.;
						fprintf( fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\t%d\n",pObj->m_Vtx[j].m_x ,pObj->m_Vtx[j].m_y, pObj->m_Vtx[j].m_z, LINE_TYPE, pObj->m_Vtx[j].r,pObj->m_Vtx[j].g,pObj->m_Vtx[j].b, Type);
					}
				}
				fprintf( fp, "\n");
			}
			fclose(fp);
			m_NeedsToSave=0;
		}
	}
}

//
void CAnalysisView::RemoveEmptyDrawings()
{
	CDrawing *pDraw;
	int InitNumDrawings=m_NumDrawings-1;
	int removedDrawings=0;
	for(int Idx=InitNumDrawings; Idx>-1; Idx--){
		pDraw=&m_Drawing[Idx];
		if(pDraw->m_NumDots==0 && pDraw->m_NumLinedObj==0){
			if(m_Selected_Drawing_Idx==Idx){
				m_Selected_Drawing_Idx=-1;
			}
			m_Drawing.RemoveAt(Idx);
			removedDrawings++;
		}
	}
	m_NumDrawings-=removedDrawings;
}


//Reads MRI files (BMP)
void CAnalysisView::Load_MRIs(int OpenOption)
{
	int i,j,Mat_Sizexi, PicLoaded=0;

	CClientDC objDC(this);
	CPoint pointXY;	HBITMAP hBmp;	BITMAP bmp; 	HDC comDC;   
	CPen myPen1(PS_INSIDEFRAME,1,RGB(255,0,0));
	COLORREF  rgb;
	CFigure *pFig=&m_Fig[0][0][0];
	char ProgressMsg[1042];

	for(int img=0; img<m_NumMRIs; img++){
		sprintf(ProgressMsg,"Progress(%%):    %d         ",int(100.*img)/m_NumMRIs);
		pFig->PutText(MsgPosX(pFig,0.5),MsgPosY(pFig,0.5), ProgressMsg,&objDC);
		hBmp=(HBITMAP)LoadImage(0,m_MRI[img].m_FigureNameWithPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		GetObject(hBmp,sizeof(bmp),&bmp);	
		if(bmp.bmWidth>0 && bmp.bmHeight>0){//Only when the loading was successful.
			comDC=CreateCompatibleDC(objDC);
			SelectObject(comDC,hBmp);
			m_imgWidth=bmp.bmWidth; m_imgHeight=bmp.bmHeight;
			m_MRI[img].m_RforI.SetSize(m_imgWidth*m_imgHeight); m_MRI[img].m_GforI.SetSize(m_imgWidth*m_imgHeight); m_MRI[img].m_BforI.SetSize(m_imgWidth*m_imgHeight); 
			DeleteObject(hBmp);
			for(i=0;i<m_imgWidth;i++){
				Mat_Sizexi=m_imgHeight*i;
				for(j=0;j<m_imgHeight;j++){ 
					rgb=GetPixel(comDC,i,m_imgHeight-1-j);
					m_MRI[img].m_RforI[Mat_Sizexi+j]=(rgb&0xFF);
					m_MRI[img].m_GforI[Mat_Sizexi+j]=((rgb>>8)&0xFF);
					m_MRI[img].m_BforI[Mat_Sizexi+j]=((rgb>>16)&0xFF);
			}	}
			PicLoaded=1;
		}
	}
	if(PicLoaded==1){
		m_Pic_Loaded=1; 
		m_NeedsToSave=0;
		m_Selected_MRI_Idx=0;
	}
}


//Reads the texture files for description of neuron from “Texture” folder. 
//The icon-style textures are made and stored in the Texture folder by the user.
void CAnalysisView::Load_All_Texture() 
{
	char FileName[1024];
	FILE *fp;
	int textureIdx=0, PriorMissingFiles=0; 

	for(int i=0;i<=MAX_TEXTURE;i++){
		sprintf(FileName,"%s\\Texture\\Texture_%d.bmp",m_FolderPath,i);
		//Read data
		if((fp=fopen(FileName,"r"))!= NULL){
			fclose(fp);	
			m_Texture[textureIdx]=LoadTextureBMP( FileName, true); if(m_Texture[textureIdx]==-1){AfxMessageBox("File open error at Texture open."); return;}
			textureIdx++;

			if(PriorMissingFiles==1){
				PriorMissingFiles=-MAX_TEXTURE; //Display this message only once
				sprintf(FileName,"Texture_%d.bmp is missing. It should not be!",i);
				AfxMessageBox(FileName);
			}
		}
		else PriorMissingFiles++;

	}
	m_numTextures=textureIdx;
}

//Reads the texture files for description of neuron from “Texture” folder. 
//The icon-style textures are made and stored in the Texture folder by the user.
void CAnalysisView::Load_All_Texture_forCoronalSections() 
{
	int textureIdx=0; 

	int texture, i,j,Mat_Sizexi, sizofMem;
	CClientDC objDC(this);
	#define MAX_WIDTH  200
	#define MAX_HEIGHT  200
	BYTE data[MAX_WIDTH*MAX_WIDTH*3];
	char TextureFolder[1028], JustTextureName[100], TextureFullName[1028];
	if(m_isCoronal_Or_Sagittal==MANUAL_OPEN)sprintf(TextureFolder,"%sTexture",m_FolderPath);
	else sprintf(TextureFolder,"%sTexture",M_DataDir);
	CPoint pointXY;	HBITMAP hBmp;	BITMAP bmp;  	HDC comDC;   
	CPen myPen1(PS_INSIDEFRAME,1,RGB(255,0,0));
	TextureForCORONAL *pTxture;

	for(int texture=0;texture<MAX_TEXTURE;texture++){
		sprintf(JustTextureName,"Texture_%d.bmp",texture);
		if(IsThereFileWithThisName(TextureFolder, 0, JustTextureName)==1){//See if there is a file with the same name.
			sprintf(TextureFullName,"%s\\%s",TextureFolder,JustTextureName);
			COLORREF  rgb;
			hBmp=(HBITMAP)LoadImage(0,TextureFullName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
			GetObject(hBmp,sizeof(bmp),&bmp);	
			if(bmp.bmWidth>0 && bmp.bmHeight>0){//Only when the loading was successful.
				textureIdx++; //This means no missing textures 
				pTxture=&CoronalTexture[texture];
				comDC=CreateCompatibleDC(objDC);
				SelectObject(comDC,hBmp);
				pTxture->m_imgWidth=bmp.bmWidth; pTxture->m_imgHeight=bmp.bmHeight;
				pTxture->m_RforI.SetSize(pTxture->m_imgWidth*pTxture->m_imgHeight); pTxture->m_GforI.SetSize(pTxture->m_imgWidth*pTxture->m_imgHeight); pTxture->m_BforI.SetSize(pTxture->m_imgWidth*pTxture->m_imgHeight); 
				DeleteObject(hBmp);
				for(i=0;i<pTxture->m_imgWidth;i++){
					Mat_Sizexi=pTxture->m_imgHeight*i;
					for(j=0;j<pTxture->m_imgHeight;j++){ 
						rgb=GetPixel(comDC,i,pTxture->m_imgHeight-1-j);
						pTxture->m_RforI[Mat_Sizexi+j]=(rgb&0xFF);
						pTxture->m_GforI[Mat_Sizexi+j]=((rgb>>8 )&0xFF);
						pTxture->m_BforI[Mat_Sizexi+j]=((rgb>>16)&0xFF);
				}	}
			}
		}
	}
	m_numTextures=textureIdx;
	m_CoronalTexture_Loaded=1;
}
	
//Reads the description of neurons’ data from “_Neuron_Data.txt”, and reads the texture data from "Texture" folder. 
//The Dialog that accepts neuronal data can be evoked by putting the tip of the electrode at the intended coordinate and hitting the “Return” key.
void CAnalysisView::Load_Neuron_Data_and_Texture(double scaleX, double scaleY, double scaleZ) 
{
	int strLength,NnNumber;
	double x,y,z;
	double dummyD;
	int dummyInt, i;
	char dummySt[1024];
	m_Nn.SetSize(100);
	m_HowManyNeurons=0;

	char FileName[512], Dump[512], JustName[64];
	sprintf(FileName,"%s\\_Neuron_Data.txt",M_DataDir);
	FILE *fp;

	if((fp=fopen(FileName,"r"))!= NULL){
		fscanf(fp,"%s",dummySt );//	Version_number:	
		fscanf(fp,"%lf",&dummyD);// Version numbere
		fscanf(fp,"%s",dummySt );//	Future use:	
		fscanf(fp,"%d",&dummyInt);// Future use dummyInt
		fscanf(fp,"%s",dummySt );//	Future use:	
		fscanf(fp,"%d",&dummyInt);// Future use dummyInt
		while(fscanf( fp,"%d",&NnNumber)!=EOF){//Nuuron #
			//	Neuron_name:	
			fscanf(fp,"%s",dummySt );
			if(wildcmp("Neuron_name:*",dummySt)!=1){AfxMessageBox("wildcmp(\"Neuron_name:\",dummySt)!=1"); break;}
			fgets(dummySt,63,fp);	 //	the name	
			m_Nn[m_HowManyNeurons].m_NnName=dummySt;	 //	the name	
			//	Coordinate:	
			fscanf(fp,"%s",dummySt );
			fscanf(fp,"%lf%lf%lf",&m_Nn[m_HowManyNeurons].m_x,&m_Nn[m_HowManyNeurons].m_y,&m_Nn[m_HowManyNeurons].m_z);	
			//	Category:	
			fscanf(fp,"%s",dummySt );
			fscanf(fp,"%d",&m_Nn[m_HowManyNeurons].m_type);
			//Variables
			for(i=0; i<10; i++){
				fscanf(fp,"%s",dummySt );//Variable_x:
				fscanf(fp,"%lf",&m_Nn[m_HowManyNeurons].m_NnVariables[i]);	
			}
			//NOTE
			fscanf(fp,"%s",dummySt );//	Note:	
			while(1){
				fgets(dummySt,1023,fp);		
				if(wildcmp("*End of note*",dummySt)==1|| wildcmp("*End of Note*",dummySt)==1|| wildcmp("*end of note*",dummySt)==1|| wildcmp("*end of Note*",dummySt)==1)break;
				if(strcmp(dummySt,"\n")!=0){
					m_Nn[m_HowManyNeurons].m_Note+=dummySt;
				}
			}
			if(m_Nn.GetSize()-2<m_HowManyNeurons)m_Nn.SetSize(2*m_Nn.GetSize());
			m_HowManyNeurons++;
		}
		fclose(fp);	
	}
	Load_All_Texture_forCoronalSections();
}

//Writes the description of neurons’ data to “_Neuron_Data.txt”. The Dialog that accepts neuronal data can be evoked by putting the tip of the electrode at the intended coordinate and hitting the “Return” key.
void CAnalysisView::Save_Neuron_Data(double scaleX, double scaleY, double scaleZ) 
{
	int i,j, intVar, NumOfIntVar=1;
	char FileName[512], Dump[512], JustName[64];
	sprintf(FileName,"%s\\_Neuron_Data.txt",m_FolderPath);
	FILE *fp;

	if((fp=fopen(FileName,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)VERSION_NUM_OF_VIRTUAL_ELECTRODE );		//Version number
		fprintf(fp,"NumberOfIntVariables: %d\n", NumOfIntVar);//	Number of int variables	
		for(intVar=0; intVar<NumOfIntVar; intVar++){fprintf(fp,"Future_use: %d\n\n",0 );}//	Future use:	
		for(i=0; i<m_HowManyNeurons; i++){
			fprintf( fp,"%d\n",i+1);//Nuuron #
			//	Neuron_name:	
			fprintf(fp,"Neuron_name: %s\n", m_Nn[i].m_NnName);
			//	Coordinate:	
			fprintf(fp,"Coordinate: %0.2lf %0.2lf %0.2lf\n",m_Nn[i].m_x, m_Nn[i].m_y, m_Nn[i].m_z );
			//	Category:	
			fprintf(fp,"Category: %d\n",m_Nn[i].m_type );
			//Variables
			for(j=0; j<MAX_NN_VARs; j++){
				fprintf(fp,"Variable_%d: %0.2lf\n",j,m_Nn[i].m_NnVariables[j]);	
			}
			//NOTE
			fprintf(fp,"Note:\n" );
			fprintf(fp,"%s\n", m_Nn[i].m_Note);
			fprintf(fp,"End of note.\n\n" );
		}
		fclose(fp);	
	}
}


//The following two are dummy functions.
void CAnalysisView::OnFileOpen()
{
	sprintf(m_FileName," ");
	sprintf(m_JustFileName," ");
	m_NewlyLoaded=1;
	CFigure *pFig;
	m_GlobalTitle="Virtual_Electrode";
	m_dHowManyRows=1;
	m_dHowManyClmns=1;
	pFig=&m_Fig[0][0][0];
	pFig->m_Title.NameString="";
	pFig->m_nTx=5; 
	pFig->m_nTy=41;
	pFig->m_nBx=1267;
	pFig->m_nBy=707;	
	pFig->m_FontWidth=6; 
	pFig->m_FontHeight=12;	
	pFig->m_howManyX=-1; 
	pFig->m_howManyY=-1; 
	sprintf(pFig->m_cXaxis[0],"");
	sprintf(pFig->m_cYaxis[0],"");
	pFig->m_dMinX=0; pFig->m_dMaxX=10; pFig->m_dMinY=0; pFig->m_dMaxY=6000;	 
	pFig->m_Deleted=0;	
	if(m_dHowManyClmns!=-1){
		EliminateExcluded();
	}
}

void CAnalysisView::OnFileSave()
{
	AfxMessageBox("It doesn't do anything, really.");
}


//#####################################################################################################################################################################################################################################
//#####################################################################################################################################################################################################################################
//#####################################################################################################################################################################################################################################

//	CString ExeDirString=GetExecutableName();
int CAnalysisView::FromA_findB_addC_giveD(char fromA[], char *findB, char addC[], char giveD[])
{
	char tmpStr[1024], FindNameWild[100],FindName[100];
	strcpy(tmpStr,fromA);
	sprintf(FindNameWild,"*\\%s*",findB);
	sprintf(FindName,"*\\%s",findB);
	if(wildcmp(FindNameWild,tmpStr)==1){
		RE_Search:;
		if(wildcmp(FindName,tmpStr)==1){
			sprintf(giveD,"%s\\",tmpStr);
		}
		else {GetGrandParentFolderName(tmpStr,tmpStr); goto RE_Search;}
		if(addC[0]=='\\')RemoveForwardSlashAtEnd(giveD);
		strcat(giveD,addC);
	}		
	else{
		//See if the parent folder of fromA has "addC".
		GetGrandParentFolderName(fromA,tmpStr);
		if(IsThereFileWithThisName(tmpStr,0,addC)==1){
			AddFrwdSlashAtEndIfNotHave(tmpStr);
			strcpy(giveD,tmpStr);
			strcat(giveD,addC);
		}
		else{
			sprintf(tmpStr,"Couldn't find %s",findB);
			AfxMessageBox(tmpStr);
			return 0;
		}
	}
	return 1;
}

CString CAnalysisView::GetExecutableName()
{
	CString strFileName;
	GetModuleFileName(NULL, strFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	return strFileName;
}

//This one removes the parent folder name out of the full pathway "FolderPath"
int CAnalysisView::GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=1;i<sizeofChar-1;i++){//ignore the first two (\*) letters
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(strcmp("\\", FldrName)==0)break;
	}
	_strrev(tmpStr); //It reverses the order of the string
	tmpStr[strlen(tmpStr)-(i+1)]='\0';
	strcpy(GrandParentFolderName,tmpStr);
	return 1;
}

void CAnalysisView::SaveFolderPath(char FolderPath[])
{
	// Get the full path of current exe file.
	TCHAR FileName_ToWriteFolderName[MAX_PATH] = { 0 };
	GetModuleFileName( 0, FileName_ToWriteFolderName, MAX_PATH );
	// Strip the exe filename from path and get folder name.
	PathRemoveFileSpec( FileName_ToWriteFolderName );
	// Add the file name.
	strcat(FileName_ToWriteFolderName,"\\SavedDir.txt");
	FILE *fp;
	if((fp=fopen(FileName_ToWriteFolderName,"w"))!= NULL){
		fprintf( fp, FolderPath );		
		fclose(fp);
	}
}

int CAnalysisView::ReadSavedFolderPath(char FolderPath[])
{
	char tmpStr[1024], FileName_ToWriteFolderName[1024];
	CString ExeDirString=GetExecutableName();
	strcpy(tmpStr,ExeDirString);
	GetGrandParentFolderName(tmpStr,FileName_ToWriteFolderName);
	strcat(FileName_ToWriteFolderName,"\\SavedDir.txt");
	FILE *fp;
	if((fp=fopen(FileName_ToWriteFolderName,"r"))!= NULL){
		fgets(m_FolderPath,499,fp);
		fscanf(fp,"%s", m_FolderPath );		
		fclose(fp);
		return 1;
	}
	else return 0;
}

int CAnalysisView::Convert_6digitDate_ToNumber(char *DateString) 
{	
	char date[64];
	int DateInNum;
	//strcpy(date,DateString);
	Copy_fromNth_HowMany(date,DateString,5,2);
	DateInNum=atoi(date)*10000;
	Copy_fromNth_HowMany(date,DateString,3,2);
	DateInNum+=atoi(date);
	Copy_fromNth_HowMany(date,DateString,1,2);
	DateInNum+=atoi(date)*100;
	m_FileYearMonthDate=DateInNum;
	return DateInNum;
}

int CAnalysisView::Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany) 
{	
	strcpy(Dest,Source);
	strrev(Dest);
	Dest[strlen(Dest)-(Nth-1)]='\0';
	strrev(Dest);
	Dest[HowMany]='\0';
	return 0;
}

int CAnalysisView::Find_first_6Digit_unintrupted_number(char *SixDigitString, char *NameString) 
{	
	int numDigits,Start, endI, i=0;
	endI=strlen(NameString);
	Re_Search:;
	numDigits=0; Start=i;
	for(i=Start;i<endI;i++){
		if(NameString[i]<'0' || NameString[i]>'9'){i++; goto Re_Search;}
		numDigits++;
		if(numDigits==6){
			Copy_fromNth_HowMany(SixDigitString,NameString,Start+1,6);
			break;
		}
	}

	if(numDigits==6)return 1;
	else  return 0;//Failed to Find
}

#define STRICT
#include <windows.h>
#include <shlobj.h>

INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) 
{
   TCHAR szDir[MAX_PATH];

   switch(uMsg) 
   {
   case BFFM_INITIALIZED: 
      if (GetCurrentDirectory(sizeof(szDir)/sizeof(TCHAR), szDir))
      {
         // WParam is TRUE since you are passing a path.
         // It would be FALSE if you were passing a pidl.
         SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
      }
      break;

   case BFFM_SELCHANGED: 
      // Set the status window to the currently selected path.
      if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir))
      {
         SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
      }
      break;
   }
   return 0;
}
int CAnalysisView::Pick_Directory()
{
	BROWSEINFO bi = { 0 };
	TCHAR szDir[MAX_PATH];
    bi.lpszTitle = _T("Pick a Directory");
	LPITEMIDLIST pidl;
	LPMALLOC  pMalloc;
	if (SUCCEEDED(SHGetMalloc(&pMalloc))){
		ZeroMemory(&bi,sizeof(bi));
		bi.hwndOwner = NULL;
		bi.pszDisplayName = 0;
		bi.pidlRoot = 0;
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
		bi.lpfn = BrowseCallbackProc;

		pidl = SHBrowseForFolder(&bi); 
		if (pidl){ 
			// get the name of the folder
			SHGetPathFromIDList( pidl, m_FolderPath );
			pMalloc->Free(pidl); 
		} 
		pMalloc->Release();
      }
	if(pidl==NULL)return 0;
	GetFolderName(m_FolderPath,m_JustFolderName);
	return 1;
}

//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPath & m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath
int CAnalysisView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName)
{	
	CFileDialog dialog(IsToREAD, "txt", initialFileName, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.txt)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){m_bIsON=0;return 0;}
	sprintf(m_FileName,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPath,m_FileName);
	m_FolderPath[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	
	//Extract file name without extension (.txt)
	#define dotSizePlus1 2 //The size of the identifier ".txt"
	char strbuf[1000];
	int i,sizeofChar,foundExt=0;
	sizeofChar=strlen(m_JustFileName);
	for(i=0;i<sizeofChar;i++){
		strncpy_s(strbuf,dotSizePlus1,m_JustFileName+i,_TRUNCATE);
		if(wildcmp(".", strbuf)){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileName);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileName)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}

//Upon the selection of a file from the dialogue, this function finds and stors:
//The full file name with the full pathway:            m_FolderPath & m_FileName
//Just the file name itself:                           m_JustFileName
//Just the folder path excluding the name of the file: m_FolderPath
int CAnalysisView::GetFile_WholePath_Name_CurPath(BOOL IsToREAD)
{	
	CFileDialog dialog(IsToREAD, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "All files(*.*)|*.*|| ",NULL);
	//CFileDialog dialog(IsToREAD, "rst", "*.rst", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, "Data files (*.rst)|All files(*.*)|*.*|| ",NULL);
	if(dialog.DoModal()==IDCANCEL){m_bIsON=0;return 0;}
	sprintf(m_FileName,"%s", dialog.GetPathName()); 
	sprintf(m_JustFileName,"%s", dialog.GetFileName()); 
	strcpy_s(m_FolderPath,m_FileName);
	m_FolderPath[strlen(m_FileName)-strlen(m_JustFileName)]='\0';
	
	//Extract file name without extension (.txt)
	#define dotSizePlus1 2 //The size of the identifier ".txt"
	char strbuf[1000];
	int i,sizeofChar,foundExt=0;
	sizeofChar=strlen(m_JustFileName);
	for(i=0;i<sizeofChar;i++){
		strncpy_s(strbuf,dotSizePlus1,m_JustFileName+i,_TRUNCATE);
		if(wildcmp(".", strbuf)){foundExt=1;break;}
	}
	strcpy_s(m_FileNameWithoutEXT,m_FileName);
	if(foundExt)m_FileNameWithoutEXT[strlen(m_FileName)-strlen(m_JustFileName)+i]='\0';	
	
	return 1;
}

//This one extracts the folder name "FolderName" from the given full pathway "FolderPath"
int CAnalysisView::GetFolderName(char* FolderPath, char *FolderName)
{
	char tmpStr[500],FldrName[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpStr,FolderPath);
	_strrev(tmpStr); //It reverses the order of the string
	int	sizeofChar=strlen(tmpStr);
	for(i=2;i<sizeofChar-1;i++){//ignore the first two (\*) letters
		strncpy_s(FldrName,2,&tmpStr[i],_TRUNCATE);
		if(wildcmp("\\", FldrName))break;
	}
	strncpy_s(FldrName,i+1,&tmpStr[0],_TRUNCATE);
	_strrev(FldrName);
	strcpy_s(FolderName,100,FldrName);
	return 1;
}

//This one extracts just the file name "JustFileName" from the given full file name "FullFileName"
int CAnalysisView::Get_JustFileName(char* FullFileName, char *JustFileName)
{
	char tmpFullName[500],tmpJustName[500],tmpChar[500],tmpLowercase[500];
	int i;
	strcpy_s(tmpFullName,FullFileName); RemoveForwardSlashAtEnd(tmpFullName);
	_strrev(tmpFullName); //It reverses the order of the string
	int	sizeofChar=strlen(tmpFullName);
	for(i=0;i<sizeofChar-1;i++){
		strncpy_s(tmpChar,2,&tmpFullName[i],_TRUNCATE);
		if(wildcmp("\\", tmpChar))break;
	}
	strncpy_s(tmpJustName,i+1,&tmpFullName[0],_TRUNCATE);
	_strrev(tmpJustName);
	strcpy_s(JustFileName,100,tmpJustName);
	return 1;
}

int CAnalysisView::wildcmp(char *wild, char *string) 
{
  char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } 
	else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } 
	else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  int a=strlen(wild);
  if(a==0)return 1;
  return !*wild;
}

// This function will find all files in a directory and its subdirectory
int CAnalysisView::IsThereFileWithThisName(char* searchThisDir, bool isToSearchSubDirs, char* FindJustFileName)
{
	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Build the file search string
	char searchDir[2048] = {0};

	// If it already is a path that ends with \, only add the *
	if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", searchThisDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", searchThisDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return 0;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		

		// If we find a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(isToSearchSubDirs){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				// If it already is a path that ends with \, only add the *
				if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir2, 2047, "%s%s", searchThisDir, FindFileData.cFileName);
				// Otherwise, add the \* to the end of the path
				else _snprintf(searchDir2, 2047, "%s\\%s", searchThisDir, FindFileData.cFileName);
				IsThereFileWithThisName(searchDir2, isToSearchSubDirs,FindJustFileName);
			}
			// Do not need to process anymore
			continue;
		}
		//Found a file name.
		if(strcmp(FindFileData.cFileName, FindJustFileName)==0)return 1;
	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
	return 0;
}


int CAnalysisView::Find_Drawing_Files(char* searchThisDir, bool isToSearchSubDirs, int CallOrigin)
{
	char BeforeNum[1024], AfterNum[1024];

	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Build the file search string
	char searchDir[2048] = {0}, tmpName[2048];
	double FoundNumber;

	// If it already is a path that ends with \, only add the *
	if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", searchThisDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", searchThisDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return 0;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		
		// If we find a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(isToSearchSubDirs){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				// If it already is a path that ends with \, only add the *
				if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir2, 2047, "%s%s", searchThisDir, FindFileData.cFileName);
				// Otherwise, add the \* to the end of the path
				else _snprintf(searchDir2, 2047, "%s\\%s", searchThisDir, FindFileData.cFileName);
				Find_Drawing_Files(searchDir2, isToSearchSubDirs, CallOrigin);
			}
			// Do not need to process anymore
			continue;
		}

		//##############  Found a file name ==> Do someting!! ####################
		if(m_NumDrawings>=Max_Slices-1){AfxMessageBox("m_NumDrawings>=Max_Slices-1"); return 0;}
		if(1==Find_Last_Double_inString(FindFileData.cFileName, FoundNumber) && wildcmp("*.txt",FindFileData.cFileName)==1 && wildcmp("*Config*",FindFileData.cFileName)==0  && wildcmp("*DATA*",FindFileData.cFileName)==0){
			AddFrwdSlashAtEndIfNotHave(searchThisDir);
			sprintf(tmpName,"%s%s",searchThisDir,FindFileData.cFileName); 
			strcpy(M_DrawingFileInfo[m_NumDrawings].m_SliceFileName,tmpName);
			M_DrawingFileInfo[m_NumDrawings].m_Depth=FoundNumber;
			m_NumDrawings++; 
		}
		//########################################################################

	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
	return 0;
}


int CAnalysisView::Find_MRI_Files(char* searchThisDir, bool isToSearchSubDirs, int CallOrigin)
{
	char BeforeNum[1024], AfterNum[1024];

	// What we need to search for files
	WIN32_FIND_DATA FindFileData = {0};
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// Build the file search string
	char searchDir[2048] = {0}, tmpName[2048];
	double FoundNumber;

	// If it already is a path that ends with \, only add the *
	if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir, 2047, "%s*", searchThisDir);
	// Otherwise, add the \* to the end of the path
	else _snprintf(searchDir, 2047, "%s\\*", searchThisDir);

	// Find the first file in the directory.
	hFind = FindFirstFile(searchDir, &FindFileData);

	// If there is no file, return
	if (hFind == INVALID_HANDLE_VALUE) return 0;

	// Find files!
	do{
		// If it's the "." directory, continue
		if(strcmp(FindFileData.cFileName, ".") == 0) continue;
		// If it's the ".." directory, continue
		if(strcmp(FindFileData.cFileName, "..") == 0) continue;		
		// If we find a directory
		if(FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY){
			// If we want to search subdirectories
			if(isToSearchSubDirs){
				// Holds the new directory to search
				char searchDir2[2048] = {0};
				// If it already is a path that ends with \, only add the *
				if(searchThisDir[strlen(searchThisDir) - 1] == '\\') _snprintf(searchDir2, 2047, "%s%s", searchThisDir, FindFileData.cFileName);
				// Otherwise, add the \* to the end of the path
				else _snprintf(searchDir2, 2047, "%s\\%s", searchThisDir, FindFileData.cFileName);
				Find_MRI_Files(searchDir2, isToSearchSubDirs, CallOrigin);
			}
			// Do not need to process anymore
			continue;
		}

		//##############  Found a file name ==> Do someting!! ####################
		if(m_NumMRIs>=Max_Slices-1){AfxMessageBox("[m_NumMRIs]>=Max_Slices-1"); return 0;}
		if(CallOrigin==CORONAL){
			if(1==Find_Last_Double_inString_and_give_strings_before_and_after_it(FindFileData.cFileName, BeforeNum, FoundNumber, AfterNum) && wildcmp("*L",BeforeNum)==0 && wildcmp("*.bmp",FindFileData.cFileName)==1){
				if(m_MRI.GetSize()<m_NumMRIs+1)m_MRI.SetSize(m_MRI.GetSize()+100);
				AddFrwdSlashAtEndIfNotHave(searchThisDir);
				sprintf(tmpName,"%s%s",searchThisDir,FindFileData.cFileName); 
				strcpy(m_MRI[m_NumMRIs].m_FigureNameWithPath,tmpName);
				Get_JustFileName(m_MRI[m_NumMRIs].m_FigureNameWithPath,m_MRI[m_NumMRIs].m_JustMRI_FileName);
				m_MRI[m_NumMRIs].m_Depth=FoundNumber;
				m_NumMRIs++;
			}		
		}
		else if(CallOrigin==SAGITTAL){
			if(1==Find_Last_Double_inString_and_give_strings_before_and_after_it(FindFileData.cFileName, BeforeNum, FoundNumber, AfterNum) && wildcmp("*L",BeforeNum)==1 && wildcmp("*.bmp",FindFileData.cFileName)==1){
				if(m_MRI.GetSize()<m_NumMRIs+1)m_MRI.SetSize(m_MRI.GetSize()+100);
				AddFrwdSlashAtEndIfNotHave(searchThisDir);
				sprintf(tmpName,"%s%s",searchThisDir,FindFileData.cFileName); 
				strcpy(m_MRI[m_NumMRIs].m_FigureNameWithPath,tmpName);
				Get_JustFileName(m_MRI[m_NumMRIs].m_FigureNameWithPath,m_MRI[m_NumMRIs].m_JustMRI_FileName);
				m_MRI[m_NumMRIs].m_Depth=FoundNumber;
				m_NumMRIs++;
			}		
		}
		//########################################################################

	}while(FindNextFile(hFind, &FindFileData) != 0);// Loop while we find more files

	// We are done with the finding
	FindClose(hFind);
	return 0;
}


//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. -123.5 from 76DR-123.5.txt).
int CAnalysisView::Find_Last_Double_inString(char *GivenString, double &NumToReturn)
//int CAnalysisView::Find_Last_Integer_inString(char *GivenString, int &NumToReturn)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1, PointCount=0, foundNum, FirstChar=3;
	int sign=0, point=0;
	double num = 0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			if(FirstChar!=3 && *(p-1)=='.'){
				point=1; 
				if(FirstChar<2){
					if(*(p-2)<'0' || *(p-2)>'9'){
						num=0; 
						if(*(p-2)=='-')sign=1; else sign=0;
					}
				}
			}
			else if(FirstChar!=3 && *(p-1)=='-'){sign=1;point=0;num=0; } 
			else {sign=0;point=0;num=0;}
			PointCount=0;
			q=p;
			while(*q>='0' && *q<='9'){
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				if(point==0)num = num + (*q-48) * pow((double)10,(double)iCount);
				else num = num + (*q-48) * pow((double)10,(double)(--PointCount));
				--iCount;
				++q ;
				++p;
			}
			foundNum = 1 ;
		}
		else{
			++p ;
		}
		FirstChar--;
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return 0;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	return 1;
}


//This routine extracts numbers from a string. 
//It registers only the last one (number e.g. -123 from 76DR-123.txt).
int CAnalysisView::Find_Last_Double_inString_and_give_strings_before_and_after_it(char *GivenString, char* BeforeNum, double &NumToReturn, char *AfterNum)
{
	char *p = NULL;
	char *q = NULL;
	int iCount=-1, PointCount=0, foundNum, FirstChar=3, NumStartPos=0, NumEndPos=0,charPos=0;
	int sign=0, point=0;
	double num = 0;
	char *szString;

	foundNum=0;
	szString=GivenString;
	p=szString;
	while(*p){
		if(*p>='0' && *p<='9'){
			if(FirstChar!=3 && *(p-1)=='.'){
				point=1; NumEndPos++;
				if(FirstChar<2){
					if(*(p-2)<'0' || *(p-2)>'9'){
						num=0; NumStartPos=charPos; NumEndPos=charPos;
						if(*(p-2)=='-'){
							sign=1; 
							NumStartPos-=2;NumEndPos=charPos;
						}else sign=0;
					}
				}
			}
			else if(FirstChar!=3 && *(p-1)=='-'){
				sign=1;point=0;num=0; NumStartPos=charPos-1; NumEndPos=charPos;
			} 
			else {sign=0;point=0;num=0;NumStartPos=charPos; NumEndPos=charPos;}
			PointCount=0;
			q=p;
			while(*q>='0' && *q<='9'){
				++NumEndPos;
				++iCount ;
				++q ;
			}
			q=p;
			while(*q>='0' && *q<='9'){
				if(point==0)num = num + (*q-48) * pow((double)10,(double)iCount);
				else num = num + (*q-48) * pow((double)10,(double)(--PointCount));
				--iCount;
				++q ;
				++p;
			++charPos;
			}
			foundNum = 1 ;
		}
		else{
			++charPos;
			++p ;
		}
		FirstChar--;
	}		
	if(foundNum==0 || num<0){
		//char strbuf[1000];
		//_snprintf(strbuf, 999, "The name does not contain a number", GivenString);
		//AfxMessageBox(strbuf);
		return 0;
	}
	if(sign==1) NumToReturn= -num;
	else		NumToReturn=  num;
	Copy_fromNth_HowMany(BeforeNum, GivenString, 1, NumStartPos); 
	Copy_fromNth_HowMany(AfterNum, GivenString, NumEndPos+1, strlen(GivenString)-NumEndPos); 
	return 1;
}


//After finding all files and their paths, 
//Sort the files in an ascending order using the number parts of the folder names,  (e.g. DR1, DR2...DR55)
void CAnalysisView::Rearrage_MRI_Files()
{
	int i, SortedFileIdx[Max_Slices];
	double TmpCoordinate[Max_Slices];
	char	tmpDataNames[Max_Slices][512];
	char	tmpJustFileNames[Max_Slices][512];

	//First make copies of variables
	for(i=0; i<m_NumMRIs; i++){
		strcpy_s(tmpDataNames[i],m_MRI[i].m_FigureNameWithPath);
		strcpy_s(tmpJustFileNames[i],m_MRI[i].m_JustMRI_FileName);
		TmpCoordinate[i]=m_MRI[i].m_Depth;
	}
	//Sort the files in an ascending order using the number parts of the folder names,  (e.g. DR1, DR2...)
	Sort_inAscendingOrder(TmpCoordinate,m_NumMRIs,SortedFileIdx);
	//Rearrange the variables using the sorted order 'SortedFileIdx[i]'.
	for(i=0; i<m_NumMRIs; i++){
		strcpy_s(m_MRI[SortedFileIdx[i]].m_FigureNameWithPath,tmpDataNames[i]);
		strcpy_s(m_MRI[SortedFileIdx[i]].m_JustMRI_FileName,tmpJustFileNames[i]);
		m_MRI[SortedFileIdx[i]].m_Depth=TmpCoordinate[i];
	}
}

