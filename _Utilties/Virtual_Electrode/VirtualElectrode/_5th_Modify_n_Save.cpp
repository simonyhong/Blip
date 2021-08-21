#include "stdafx.h"
#include "Analysis.h"

#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "SinTable.h"
#include "VarDefinitions.h"

//#############################   Modify and Save File   ##################################
void CAnalysisView::Show2D_5() 
{	
	
	////Use the icon ID_2DGraph_5 to run this program
	return;



	if(m_bIsON==1){m_bIsON=0; m_isCoronal_Or_Sagittal=MANUAL_OPEN; SetEvent(M_KeyDownEvents[MAXIMUM_WAIT_KeyOBJECTS-1]); return; }
	m_bIsON=1;
	VirMemDC();
	CClientDC objDC(this);
	int nSaveDC=m_DC[0].SaveDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	CFigure *pFig=&m_Fig[0][0][0]; 


	//if(ReadSavedFolderPath(m_FolderPath)!=0){
	//	SetCurrentDirectory( m_FolderPath );
	//}
	if(Pick_Directory()==0){m_bIsON=0; return;}
	strcpy(M_DataDir,m_FolderPath);

	m_NumDrawings=0; 
	Find_Drawing_Files(m_FolderPath,0,FROM_3D);
	m_Drawing.SetSize(m_NumDrawings,30);


	//######## Do the corrections here #######
	//Load_DrawingData(1,1,1); //Give bias and scales for the data
	Update_DrawingData(); return; //Use this line if you would like to batch-correct data files.
	////Convert_to_HeadShape(); return; //Use this line if you would like to batch-correct data files.
	//Load_Neuron_Data_and_Texture(1,1,1); Modify_Neuron_Data(1,1,1);
	//###########################################


	m_bIsON=0;
	m_isCoronal_Or_Sagittal=-1;
}


//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
//Use this if you would like to batch-correct data files.
void CAnalysisView::Update_DrawingData() 
{
	FILE *fp;
	char Dump[512],BeforeNum[512], AfterNum[512], DrawingFile[1024];
	double x,y,z, r,g,b, prevDepth=-100000000*ONE_PI*ONE_PI;
	int type, rgb, LineHeadOrTail;
	int NameIdx,i,j, Type;
	double FoundNumber;
	CLinedObj* pObj;
	CDrawing *pDraw;
	CVertex *pDot;

	for(NameIdx=0;NameIdx<m_NumDrawings;NameIdx++){
		pDraw=&m_Drawing[NameIdx];
		pDraw->m_NumDots=0; 
		pDraw->m_Dot.SetSize(10); 
		pDraw->m_NumLinedObj=0;
		pDraw->m_LinedObj.SetSize(10);
		//Read data
		if((fp=fopen(M_DrawingFileInfo[NameIdx].m_SliceFileName,"r"))!= NULL){
			//### Read ###
			fgets(Dump,512,fp);//-->This is just to get rid of the whole header line including the carriage return
			while(fscanf( fp, "%lf%lf%lf%d%lf%lf%lf%d",&x, &y, &z, &type, &r,&g,&b, &LineHeadOrTail)!=EOF){
				if(pDraw->m_NumDots>=pDraw->m_Dot.GetSize()-1){pDraw->m_Dot.SetSize(2*pDraw->m_Dot.GetSize()); }
				if(type==DOT_TYPE){//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
					pDraw->m_Dot[pDraw->m_NumDots].m_x=x;
					pDraw->m_Dot[pDraw->m_NumDots].m_y=z;
					pDraw->m_Dot[pDraw->m_NumDots].m_z=y;
					pDraw->m_Dot[pDraw->m_NumDots].m_type=type;
					pDraw->m_Dot[pDraw->m_NumDots].r=r;
					pDraw->m_Dot[pDraw->m_NumDots].g=g;
					pDraw->m_Dot[pDraw->m_NumDots].b=b;
					pDraw->m_Dot[pDraw->m_NumDots].m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
					pDraw->m_Dot[pDraw->m_NumDots].m_LineHeadOrTail=LineHeadOrTail;
					pDraw->m_NumDots++; 
					pDraw->m_Depth=z;
				}
				else if(type==LINE_TYPE){//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
					if(x==-11.23105 && y==-4.69828 && z==0.50000){
						Sleep(0);
					}
					if(LineHeadOrTail==HEAD){
						if(pDraw->m_NumLinedObj>=pDraw->m_LinedObj.GetSize()-1)pDraw->m_LinedObj.SetSize(2*pDraw->m_LinedObj.GetSize());
						pObj=&(pDraw->m_LinedObj[pDraw->m_NumLinedObj]);
						pObj->m_NumVertices=0;
						pDraw->m_NumLinedObj++;
					}
					pObj->m_Vtx[pObj->m_NumVertices].m_x=x;
					pObj->m_Vtx[pObj->m_NumVertices].m_y=z;
					pObj->m_Vtx[pObj->m_NumVertices].m_z=y;
					pObj->m_Vtx[pObj->m_NumVertices].m_type=type;
					pObj->m_Vtx[pObj->m_NumVertices].r=r;
					pObj->m_Vtx[pObj->m_NumVertices].g=g;
					pObj->m_Vtx[pObj->m_NumVertices].b=b;
					pObj->m_rgbID=Find_RGB_IDX(RGB(255*r,255*g,255*b));
					pObj->m_Vtx[pObj->m_NumVertices].m_LineHeadOrTail=LineHeadOrTail;
					pObj->m_NumVertices++;
					pDraw->m_Depth=z;
					if(pObj->m_NumVertices>=pObj->m_Vtx.GetSize()-1)pObj->m_Vtx.SetSize(2*pObj->m_Vtx.GetSize());
					if(LineHeadOrTail==TAIL){
						pObj->UpdateBoundary(m_isCoronal_Or_Sagittal);
					}
				}
			}
			fclose(fp);
		}
	}


	//### Write ###
	sprintf(DrawingFile,"%s\\_DrawingData.txt",M_DataDir);
	if((fp=fopen(DrawingFile,"w"))!= NULL){//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
		fprintf( fp, "Version: %0.1lf\n", ANATOMY_3D_VERSION);
		fprintf( fp, "%d\n", m_NumDrawings);
		for(int Idx=0; Idx<m_NumDrawings; Idx++){
			pDraw=&m_Drawing[Idx];
			fprintf( fp, "%d\n", Idx);
			fprintf( fp, "%d\n", CORONAL);
			fprintf( fp, "%0.3lf\n", pDraw->m_Depth);
			fprintf( fp, "%d\n", pDraw->m_NumDots);
			fprintf( fp, "%d\n", pDraw->m_NumLinedObj);
			//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
			for(i=0; i<pDraw->m_NumDots;i++){
				pDot=&pDraw->m_Dot[i];
				pDot->r=GetRValue(m_RGB60[pDot->m_rgbID])/255.; 
				pDot->g=GetGValue(m_RGB60[pDot->m_rgbID])/255.;
				pDot->b=GetBValue(m_RGB60[pDot->m_rgbID])/255.;
				fprintf( fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\t%d\n",pDot->m_x ,pDot->m_y, pDot->m_z, DOT_TYPE, pDot->r,pDot->g,pDot->b,BODY);
			}
			//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
			for(i=0; i<pDraw->m_NumLinedObj;i++){
				pObj=&pDraw->m_LinedObj[i];
				fprintf( fp, "%d\n", pObj->m_NumVertices);
				fprintf(     fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\t%d\n",pObj->m_Vtx[0].m_x ,pObj->m_Vtx[0].m_y, pObj->m_Vtx[0].m_z, LINE_TYPE, pObj->m_Vtx[0].r,pObj->m_Vtx[0].g,pObj->m_Vtx[0].b, HEAD);
				for(j=1; j<pObj->m_NumVertices-1;j++){
					//if(j==0)Type=HEAD; else if(j==pObj->m_NumVertices-1)Type=TAIL; else Type=0;
					fprintf( fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\t%d\n",pObj->m_Vtx[j].m_x ,pObj->m_Vtx[j].m_y, pObj->m_Vtx[j].m_z, LINE_TYPE, pObj->m_Vtx[j].r,pObj->m_Vtx[j].g,pObj->m_Vtx[j].b, BODY);
				}
				fprintf(     fp, "%0.5lf\t%0.5lf\t%0.5lf\t%d\t%0.5lf\t%0.5lf\t%0.5lf\t%d\n",pObj->m_Vtx[j].m_x ,pObj->m_Vtx[j].m_y, pObj->m_Vtx[j].m_z, LINE_TYPE, pObj->m_Vtx[j].r,pObj->m_Vtx[j].g,pObj->m_Vtx[j].b, TAIL);
			}
			fprintf( fp, "\n");
		}
		fclose(fp);
	}
	AfxMessageBox("Done.");
}

//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
//Use this if you would like to batch-correct data files.
void CAnalysisView::Convert_to_HeadShape() 
{

}


//When done with this function ==> turn it to its UNMODIFIED state so that when I visit next time I can use it for other purposes without worrying about previous twicks that may affect the current ones.
//Use this if you would like to batch-correct data files.
void CAnalysisView::Modify_Neuron_Data(double scaleX, double scaleY, double scaleZ) 
{
	int i,j,m=1;
	char FileName[512], Dump[512], JustName[64];
	sprintf(FileName,"%s\\_Neuron_Data.txt",m_FolderPath);
	FILE *fp;

	if((fp=fopen(FileName,"w"))!= NULL){
		fprintf( fp, "Version_number: %0.2f\n",(float)VERSION_NUM_OF_VIRTUAL_ELECTRODE );		//Version number
		fprintf(fp,"Future_use: %d\n",0 );//	Future use:	
		fprintf(fp,"Future_use: %d\n\n",0 );//	Future use:	
		for(i=0; i<m_HowManyNeurons; i++){
			fprintf( fp,"%d\n",m++);//Nuuron #
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
			fprintf(fp,"End of note.\n\n\n\n\n\n\n\n" );
		}
		fclose(fp);	
	}
	AfxMessageBox("Done.");
}
