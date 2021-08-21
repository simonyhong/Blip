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

//#############################   3D Display   ##################################
void CAnalysisView::_1st_ThreeD_Anatomy() 
{	
	if(m_isCoronal_Or_Sagittal!=-1  && *M_is3D_Running==0){ //The current program is running a function already; yet, the function is not the one that this function that the user wants ==> create another program to run this function
		Launch_Coronal_Or_Sagittal("3D");
		SetEvent(M_KeyDownEvents[1]);
		return;
	}
	///////////////////////////

	m_bIsON=1;
	VirMemDC();
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////
	m_isCoronal_Or_Sagittal=FROM_3D; 

	if(*M_isCoronal_Running==0 && *M_isSagittal_Running==0){
		if(ReadSavedFolderPath(m_FolderPath)!=0){
			SetCurrentDirectory( m_FolderPath );
		}
		if(Pick_Directory()==0){m_bIsON=0; return;}
		strcpy(M_DataDir,m_FolderPath);
		SaveFolderPath(m_FolderPath);
	}
	m_isCoronal_Or_Sagittal=FROM_3D;
	*M_is3D_Running=1;

	Load_DrawingData(FROM_3D); //Give bias and scales for the data
	Load_Neuron_Data_and_Texture(1,1,1);

	if(m_FirstGL==1){
		*M_isKeyDownEventsCreated=1;
		char EventName[100]; 
		for(int i=0; i<MAXIMUM_WAIT_KeyOBJECTS;i++){
			sprintf(EventName,"EVENT_M_KeyDownEvents_%d",i);
			if(*M_isKeyDownEventsCreated==0){*M_isKeyDownEventsCreated=1; M_KeyDownEvents[i]=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,EventName);}
			else M_KeyDownEvents[i]=OpenEvent(EVENT_ALL_ACCESS,TRUE,EventName);
		}
		m_FirstGL=0;
		InitGraphics();
		SetEvent(M_KeyDownEvents[1]); //To trigger the first display.
	}
	
	//################################################
	Read_3D_2D_Config();
	if(*M_isCoronal_Running==0 ||*M_isSagittal_Running==0)
		Calculate_Electrode_and_Grid_Coordinates();	 // Grid rotations in degrees (pitch, roll, Yaw).
	//################################################

	m_Znear=20; 
	m_Zfar=-50;
	m_xScale3D=0.2, m_yScale3D=0.2, m_zScale3D=0.2;
	m_yAngle=0, m_xTranslate=0; m_yTranslate=0; m_zTranslate=-100; //Perspective of the viewer (camera)
	//m_yAngle=0, m_xTranslate=0; m_yTranslate=-2; m_zTranslate=-100; //Perspective of the viewer (camera)

	ResizeGraphics();
	Copy_ElectrodeCoordinate_toDisplay();
	int nIndex,vtx;
	CDrawing *pDraw;
	CLinedObj* pObj;


	//############## Make sphere objects ##############
	float radius = 0.25f, sphereX=0.00f, sphereY=0.00f, sphereZ=0.00f;
	int numSlices = 32;
	int numStacks = 8;

	GLUquadricObj* pQuadric = gluNewQuadric();
	GLUquadricObj* pDisk = gluNewQuadric();

	//assert(pQuadric!=NULL);
	//############################
	//Ambient light
    GLfloat ambientColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//Directed light
	GLfloat lightPos0[4] = {0.0f, -100.0f, 10.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	GLfloat lightColor0[] = {0.9f, 0.9f, 0.9f, 1.0f}; 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	//Directed light
	//GLfloat lightPos1[] = {0.0f, 0.0f, 10.0f, 1.0f};
	//glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	//GLfloat lightColor1[] = {0.9f, 0.0f, 0.0f, 1.0f}; 
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); //Enable light #0
    //glEnable(GL_LIGHT1); //Enable light #1
    //glEnable(GL_NORMALIZE); //Have OpenGL automatically normalize our normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading	
	//############################
    //Material
    //The color of the object
    GLfloat MaterialColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat PrvMaterialColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat materialSpecular[] = {0.2f, 0.2f, 0.2f, 1.0f};//The specular (shiny) component of the material
    GLfloat materialEmission[] = {0, 0, 0, 1.0f};//The color emitted by the material
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MaterialColor);
   // glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    //The shininess parameter.  A higher number indicates a more concentrated
    //shiny area, while a lower number indicates a larger shiny area.  The
    //shininess must be between 0 and 128.
    glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);
	//############################
	//Texture
	Load_All_Texture();
	// select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // if wrap or clamp
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );//or GL_REPEAT

	glEnable( GL_TEXTURE_2D );
	//############################
	int i, Idx, objIdx;
	double x,y,z=0;
	while(1){
		//User Key Down
		nIndex=WaitForMultipleObjects(MAXIMUM_WAIT_KeyOBJECTS,M_KeyDownEvents,FALSE/*wait for all?*/,INFINITE/*timeout*/); 
		if(nIndex==MAXIMUM_WAIT_KeyOBJECTS-1 || m_bIsON==0)break;
		//switch(nIndex){
		//case 4:
		//	if(m_ZScale-0.05>=0)m_ZScale-=0.05; 
		//	break;
		//}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_COLOR_MATERIAL); 

		//################### Camera ################
		glLoadIdentity();
		glTranslated(m_xTranslate, m_yTranslate, m_zTranslate); //The user uses the mouse dragging +CTRL to translate
		glRotated(m_yAngle,0, 1, 0); //The user uses the mouse dragging to rotate along the y axis of the OpenGL coordinate
		glRotated(-90,1, 0, 0); //To address the difference between the current 3D system and that of the OpenGL
		glScalef(m_xScale3D, m_yScale3D, m_zScale3D); //The user rolls the wheel with CTRL+

		//################# Text: Electrode Position ################
		glColor3d(1.0, 1.0, 1.0);
		wglUseFontBitmaps(m_hDC, 0, 256, 1000); // create bitmaps for the device context font's first 256 glyphs  
		glListBase(1000); // set up for a string-drawing display list call  
		Copy_ElectrodeCoordinate_toDisplay();
		glRasterPos3f(0, 0, 32.0); glCallLists(m_ElecFromCenterLen, GL_UNSIGNED_BYTE, m_ElecCoordinate_withGridCenterAsOrigin); // draw a string using font display lists  
		
		//############### Eletrode  ##################
		//Line
		glLineWidth(1);
		glBegin(GL_LINES); //paired dots
		glColor3d(0, 0, 0);
		glVertex3d((*M_ElectrodeTopX_3DAtlas),(*M_ElectrodeTopY_3DAtlas),(*M_ElectrodeTopZ_3DAtlas));
		glColor3d(0.5, 0.5, 0.5);
		glVertex3d((*M_ElectrodeTipX_3DAtlas),(*M_ElectrodeTipY_3DAtlas),(*M_ElectrodeTipZ_3DAtlas));
		glEnd();
		//The tip of the electrode
		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex3d((*M_ElectrodeTipX_3DAtlas),(*M_ElectrodeTipY_3DAtlas),(*M_ElectrodeTipZ_3DAtlas));
		glEnd();

		//################### Origin x-y-z ##################
		if(m_isToDispOrigin==1){
			glPushMatrix();
			MaterialColor[0]=0.5; MaterialColor[1]=0.5; MaterialColor[2]=0.5;
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MaterialColor);

			//3D origin
			glRasterPos3f(5, 0, 0); glCallLists(4, GL_UNSIGNED_BYTE, "X"); // draw a string using font display lists  
			glRasterPos3f(0, 5, 0); glCallLists(4, GL_UNSIGNED_BYTE, "Y"); // draw a string using font display lists  
			glRasterPos3f(0, 0, 5); glCallLists(4, GL_UNSIGNED_BYTE, "Z"); // draw a string using font display lists  
			glLineWidth(1);// Set property
			glBegin(GL_LINES); 
			glVertex3d(0, 0, 0); glVertex3d(5, 0, 0); //X
			glVertex3d(0, 0, 0); glVertex3d(0, 5, 0); //Y
			glVertex3d(0, 0, 0); glVertex3d(0, 0, 5); //Z

			//Grid origin (unit vectors)
			#define Length_of_unit_Vector_Displayed (5)
			glVertex3d(*M_GridCenter_X, *M_GridCenter_Y, *M_GridCenter_Z); glVertex3d(*M_Grid_X_unit_Vector_x*Length_of_unit_Vector_Displayed+*M_GridCenter_X,  *M_Grid_X_unit_Vector_y*Length_of_unit_Vector_Displayed+*M_GridCenter_Y, *M_Grid_X_unit_Vector_z*Length_of_unit_Vector_Displayed+*M_GridCenter_Z); //X
			glVertex3d(*M_GridCenter_X, *M_GridCenter_Y, *M_GridCenter_Z); glVertex3d(*M_Grid_Y_unit_Vector_x*Length_of_unit_Vector_Displayed+*M_GridCenter_X,  *M_Grid_Y_unit_Vector_y*Length_of_unit_Vector_Displayed+*M_GridCenter_Y, *M_Grid_Y_unit_Vector_z*Length_of_unit_Vector_Displayed+*M_GridCenter_Z); //Y
			glVertex3d(*M_GridCenter_X, *M_GridCenter_Y, *M_GridCenter_Z); glVertex3d(*M_Grid_Z_unit_Vector_x*Length_of_unit_Vector_Displayed+*M_GridCenter_X,  *M_Grid_Z_unit_Vector_y*Length_of_unit_Vector_Displayed+*M_GridCenter_Y, *M_Grid_Z_unit_Vector_z*Length_of_unit_Vector_Displayed+*M_GridCenter_Z); //Z
			glEnd();
			glPopMatrix();
		}
		glLineWidth(1);

		//########################################################  Markings  #########################################################
		for(i=0; i<*M_NumberOfMarkingsCORONAL;i++){
			glPushMatrix();
			glTranslated(M_MarkingCORONAL[i].m_x,M_MarkingCORONAL[i].m_y,M_MarkingCORONAL[i].m_z);
			MaterialColor[0]=M_MarkingCORONAL[i].r; MaterialColor[1]=M_MarkingCORONAL[i].g; MaterialColor[2]=M_MarkingCORONAL[i].b;
			glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialColor);
			gluSphere(pQuadric,radius,numSlices,numStacks);
			glPopMatrix();	
		}
		for(i=0; i<*M_NumberOfMarkingsSAGITTAL;i++){
			glPushMatrix();
			glTranslated(M_MarkingSAGITTAL[i].m_x,M_MarkingSAGITTAL[i].m_y,M_MarkingSAGITTAL[i].m_z);
			MaterialColor[0]=M_MarkingSAGITTAL[i].r; MaterialColor[1]=M_MarkingSAGITTAL[i].g; MaterialColor[2]=M_MarkingSAGITTAL[i].b;
			glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialColor);
			gluSphere(pQuadric,radius,numSlices,numStacks);
			glPopMatrix();	
		}	

		//########################################################  Drawing  #########################################################
		for(Idx=0; Idx<m_NumDrawings; Idx++){
			pDraw=&m_Drawing[Idx];
			//Draw Dots
			for(vtx=0;vtx<pDraw->m_NumDots;vtx++){
				glBegin(GL_POINTS);  
				if(Sq3D(pDraw->m_Dot[vtx].m_x-(*M_ElectrodeTipX_3DAtlas), pDraw->m_Dot[vtx].m_y-(*M_ElectrodeTipY_3DAtlas), pDraw->m_Dot[vtx].m_z-(*M_ElectrodeTipZ_3DAtlas))<1.){
					glColor3d(pDraw->m_Dot[vtx].r, pDraw->m_Dot[vtx].g, pDraw->m_Dot[vtx].b);
				}
				else{//Dim color
					glColor3d(pDraw->m_Dot[vtx].r*0.8, pDraw->m_Dot[vtx].g*0.8, pDraw->m_Dot[vtx].b*0.8);
				}				
				glVertex3d(pDraw->m_Dot[vtx].m_x, pDraw->m_Dot[vtx].m_y, pDraw->m_Dot[vtx].m_z);
				glEnd();
			}
			//Draw Lines
			for(objIdx=0;objIdx<pDraw->m_NumLinedObj;objIdx++){
				pObj=&(pDraw->m_LinedObj[objIdx]);
				glBegin(GL_LINE_STRIP);
				for(vtx=0;vtx<pObj->m_NumVertices;vtx++){
					if(Sq3D(pObj->m_Vtx[vtx].m_x-(*M_ElectrodeTipX_3DAtlas), pObj->m_Vtx[vtx].m_y-(*M_ElectrodeTipY_3DAtlas), pObj->m_Vtx[vtx].m_z-(*M_ElectrodeTipZ_3DAtlas))<1.){
						glColor3d(pObj->m_Vtx[vtx].r, pObj->m_Vtx[vtx].g, pObj->m_Vtx[vtx].b);
					}
					else{//Dim color
						glColor3d(pObj->m_Vtx[vtx].r*0.8, pObj->m_Vtx[vtx].g*0.8, pObj->m_Vtx[vtx].b*0.8);
					}				
					if(vtx>0 && (pObj->m_Vtx[vtx].r!=pObj->m_Vtx[vtx-1].r|| pObj->m_Vtx[vtx].g!=pObj->m_Vtx[vtx-1].g || pObj->m_Vtx[vtx].b!=pObj->m_Vtx[vtx-1].b)){
						glEnd();glBegin(GL_LINE_STRIP);
					}
					glVertex3d(pObj->m_Vtx[vtx].m_x, pObj->m_Vtx[vtx].m_y, pObj->m_Vtx[vtx].m_z);
				}
				glEnd();
			}
		}
		//##################################################################################################################################

	    glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work
	
		//##############################   Neuron Info.   ##################################################################
		if(m_toShowNeuralInfo==1){
			//"Temporary display" of a prospective Nn.
			if(m_NnType>-1){
				glPushMatrix();
				glBindTexture( GL_TEXTURE_2D, m_Texture[m_NnType] );
				MaterialColor[0]=1.0f; MaterialColor[1]=1.0f; MaterialColor[2]=1.0f;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MaterialColor);
				glTranslated((*M_ElectrodeTipX_3DAtlas),-(*M_ElectrodeTipZ_3DAtlas),(*M_ElectrodeTipY_3DAtlas));
				glBegin( GL_QUADS );
				glTexCoord2d(0.0,0.0); glVertex2d(-0.2,-0.2);
				glTexCoord2d(1.0,0.0); glVertex2d(0.2,-0.2);
				glTexCoord2d(1.0,1.0); glVertex2d(0.2,0.2);
				glTexCoord2d(0.0,1.0); glVertex2d(-0.2,0.2);
				glEnd();
				glPopMatrix();
				if(m_HowManyNeurons<1){
					//BLANK TEXTURE ay origin: to prevent contaminating other objects ==> Need to find a better solution
					glPushMatrix();
					glBindTexture( GL_TEXTURE_2D, m_Texture[0] );
					glBegin( GL_QUADS );
					glTexCoord2d(0.0,0.0); glVertex3d(-0.01,-0.01,0);
					glTexCoord2d(1.0,0.0); glVertex3d( 0.01,-0.01,0);
					glTexCoord2d(1.0,1.0); glVertex3d( 0.01, 0.01,0);
					glTexCoord2d(0.0,1.0); glVertex3d(-0.01, 0.01,0);
					glEnd();
					glPopMatrix();
				}
			}
			//Neurons
			glLineWidth(3);
			for(i=0;i<m_HowManyNeurons;i++){
				MaterialColor[0]=1.0f; MaterialColor[1]=1.0f; MaterialColor[2]=1.0f;
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MaterialColor);
				x=m_Nn[i].m_x; y=m_Nn[i].m_y; y=m_Nn[i].m_z;
				glPushMatrix();
				if(m_Nn[i].m_type>=0 && m_Nn[i].m_type<m_numTextures){
					glBindTexture( GL_TEXTURE_2D, m_Texture[m_Nn[i].m_type] );
					glBegin( GL_QUADS );
					glTexCoord2d(0.0,0.0); glVertex3d(x-0.2,y,-z-0.2);
					glTexCoord2d(1.0,0.0); glVertex3d(x+0.2,y,-z-0.2);
					glTexCoord2d(1.0,1.0); glVertex3d(x+0.2,y,-z+0.2);
					glTexCoord2d(0.0,1.0); glVertex3d(x-0.2,y,-z+0.2);
					glEnd();
					glPopMatrix();
				}
				//BLANK TEXTURE at origin: to prevent contaminating other objects ==> Need to find a better solution
				glPushMatrix();
				glBindTexture( GL_TEXTURE_2D, m_Texture[0] );
				glBegin( GL_QUADS );
				glTexCoord2d(0.0,0.0); glVertex3d(-0.01,-0.01,0);
				glTexCoord2d(1.0,0.0); glVertex3d( 0.01,-0.01,0);
				glTexCoord2d(1.0,1.0); glVertex3d( 0.01, 0.01,0);
				glTexCoord2d(0.0,1.0); glVertex3d(-0.01, 0.01,0);
				glEnd();
				glPopMatrix();

				//Colored -lines
				glEnable(GL_COLOR_MATERIAL);
				glColor3d(1.0, 0.3, 0.99);
				if(m_Nn[i].m_AntidromicStimulatedFrom==0){//Stimulated from Medial SNc
					// Draw a line strip
					glBegin(GL_LINE_STRIP);
					glVertex3d(x+0.0,y,-z+0.3);
					glVertex3d(x-0.3,y,-z+0.0);
					glVertex3d(x+0.0,y,-z-0.3);
					glEnd();
				} 
				else if(m_Nn[i].m_AntidromicStimulatedFrom==1){//Stimulated from Intermediate SNc
					// Draw a line strip
					glBegin(GL_LINE_STRIP);
					glVertex3d(x+0.0,y,-z+0.3);
					glVertex3d(x+0.0,y,-z-0.3);
					glEnd();
				} 
				else if(m_Nn[i].m_AntidromicStimulatedFrom==2){//Stimulated from Lateral SNc
					// Draw a line strip
					glBegin(GL_LINE_STRIP);
					glVertex3d(x+0.0,y,-z+0.3);
					glVertex3d(x+0.3,y,-z+0.0);
					glVertex3d(x+0.0,y,-z-0.3);
					glEnd();
				} 
				//Highlight!! Neuron is close to the electrode tip
				if(Sq3D(m_Nn[i].m_x-(*M_ElectrodeTipX_3DAtlas), m_Nn[i].m_y-(*M_ElectrodeTipY_3DAtlas), m_Nn[i].m_z-(*M_ElectrodeTipZ_3DAtlas))<=m_DistanceFromElectrode){
					// Draw a square
					glBegin(GL_LINE_STRIP);
					glColor3d(1.0, 0.4, 0.2);
					glVertex3d(x-0.2,y,-z-0.2);
					glVertex3d(x-0.2,y,-z+0.2);
					glVertex3d(x+0.2,y,-z+0.2);
					glVertex3d(x+0.2,y,-z-0.2);
					glVertex3d(x-0.2,y,-z-0.2);
					glEnd();
				}
				//Marking Lesion
				if(m_Nn[i].m_MarkingLesion>0){
					// Draw a square
					glBegin(GL_LINE_STRIP);
					glColor3d(1.0, 1.0, 0.0);
					glVertex3d(x-0.05,y+0.01,-z-0.05);
					glVertex3d(x-0.05,y+0.01,-z+0.05);
					glVertex3d(x+0.05,y+0.01,-z+0.05);
					glVertex3d(x+0.05,y+0.01,-z-0.05);
					glVertex3d(x-0.05,y+0.01,-z-0.05);
					glEnd();
				}
				glDisable(GL_COLOR_MATERIAL);
			}
		}

		//// Draw a Disk
		//glPushMatrix();
		//MaterialColor[0]=0.1f; MaterialColor[1]=0.1f; MaterialColor[2]=0.99f;
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, MaterialColor);
		//glTranslated(0, 0, 10);
		//gluDisk(pDisk,0,3, 20, 20);
		//glPopMatrix();
		
		//### Show the new scene
		SwapBuffers(m_hDC);		
	}

	#ifdef PROGRAMED_ORIGIN_n_SCALE 
	glDeleteTextures(m_numTextures, m_Texture );
	#endif
	m_bIsON=0;
	m_isCoronal_Or_Sagittal=-1;
	*M_is3D_Running=0;
}


void CAnalysisView::DeleteNn(int NnID) 
{
	if(NnID>=m_HowManyNeurons){AfxMessageBox("NnID>=m_HowManyNeurons"); return;}
	int i,j;
    for(i=NnID; i<m_HowManyNeurons-1;i++){
		m_Nn[i].m_x=m_Nn[i+1].m_x;
		m_Nn[i].m_y=m_Nn[i+1].m_y;
		m_Nn[i].m_z=m_Nn[i+1].m_z;
		m_Nn[i].m_type=m_Nn[i+1].m_type;
		for(j=0; j<MAX_NN_VARs; j++)m_Nn[i].m_NnVariables[j]=m_Nn[i].m_NnVariables[j+1];		
		m_Nn[i].m_NnName=m_Nn[i+1].m_NnName;
		m_Nn[i].m_Note=m_Nn[i+1].m_Note;
	}
	//Just in case the user tries to make a new neuron later, clear the room.
	m_Nn[i].m_type=-1; 
	m_Nn[i].m_x=0; m_Nn[i].m_y=0; m_Nn[i].m_z=0;  
	m_Nn[i].m_NnName="";
	m_Nn[i].m_Note="";
	for(j=0; j<MAX_NN_VARs; j++)m_Nn[i].m_NnVariables[j]=-1;

	m_HowManyNeurons--;
}
	
//Find the index of the RGB array given the value of the RGB
int CAnalysisView::Find_RGB_IDX(int rgb) 
{
	int i, foundI;
	double r,g,b, r2,g2,b2;
	double MinDist=INT_MAX, dist;
	r=(rgb&0xFF);
	g=((rgb>>8)&0xFF);
	b=((rgb>>16)&0xFF);
	for(i=0; i<MaxRGB60; i++){
		r2=(m_RGB60[i]&0xFF);
		g2=((m_RGB60[i]>>8)&0xFF);
		b2=((m_RGB60[i]>>16)&0xFF);
		dist=Sq3D((r-r2),(g-g2),(b-b2));
		if(MinDist>dist){
			MinDist=dist;
			foundI=i;
		}
	}
	return foundI;
}

