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

//It sets the position and unit vectors of the electrode, using the given parameters. 
//This happens either at the begining of the program, or when the user changes Pitch, Roll, Yaw.
void CAnalysisView::Calculate_Electrode_and_Grid_Coordinates()
{
	//Set the electrode position and angle
	(*M_ElectrodeTopX_3DAtlas)=0;	(*M_ElectrodeTopY_3DAtlas)=0;	(*M_ElectrodeTopZ_3DAtlas)=50;//Electrode Length
	(*M_ElectrodeTipX_3DAtlas)=0;	(*M_ElectrodeTipY_3DAtlas)=0;	(*M_ElectrodeTipZ_3DAtlas)=0;

	double coswx,coswy,coswz,sinwx,sinwy,sinwz;
	double e11,e12,e13,e21,e22,e23,e31,e32,e33;
	double ObjCntrRotX,ObjCntrRotY,ObjCntrRotZ;
	double Scl_X=1, Scl_Y=1, Scl_Z=1;
	ObjCntrRotX=*M_Grid_Pitch*(ONE_PI/180.); //Rotation along the medio-lateral axis. (Somehow the sign of the axis is flipped.)
	ObjCntrRotY=*M_Grid_Roll *(ONE_PI/180.); //Rotation along the rostro-caudal axis.
	ObjCntrRotZ=*M_Grid_Yaw  *(ONE_PI/180.); //Rotation along the axial (dorso-ventral) axis.
	//The rotation matrix (See ThreeD3 in Eurika)
	coswx=cos(ObjCntrRotX);coswy=cos(ObjCntrRotY);coswz=cos(ObjCntrRotZ);sinwx=sin(ObjCntrRotX);sinwy=sin(ObjCntrRotY);sinwz=sin(ObjCntrRotZ);	
	e11=( coswy*coswz)*Scl_X;
	e12=( coswz*sinwy*sinwx+coswx*sinwz)*Scl_Z;
	e13=(-coswz*sinwy*coswx+sinwz*sinwx)*Scl_Y;
	e21=(-coswy*sinwz)*Scl_X;
	e22=(-sinwz*sinwy*sinwx+coswx*coswz)*Scl_Z;
	e23=( sinwz*sinwy*coswx+coswz*sinwx)*Scl_Y;
	e31=( sinwy)*Scl_X;
	e32=(-sinwx*coswy)*Scl_Z;
	e33=( coswx*coswy)*Scl_Y;

	//Rotation of the electrode.
	double ElecTopX= (*M_ElectrodeTopX_3DAtlas)*e11 + (*M_ElectrodeTopY_3DAtlas)*e12 + (*M_ElectrodeTopZ_3DAtlas)*e13;
	double ElecTopY= (*M_ElectrodeTopX_3DAtlas)*e21 + (*M_ElectrodeTopY_3DAtlas)*e22 + (*M_ElectrodeTopZ_3DAtlas)*e23;
	double ElecTopZ= (*M_ElectrodeTopX_3DAtlas)*e31 + (*M_ElectrodeTopY_3DAtlas)*e32 + (*M_ElectrodeTopZ_3DAtlas)*e33;
	double ElecBtmX= (*M_ElectrodeTipX_3DAtlas)*e11 + (*M_ElectrodeTipY_3DAtlas)*e12 + (*M_ElectrodeTipZ_3DAtlas)*e13;
	double ElecBtmY= (*M_ElectrodeTipX_3DAtlas)*e21 + (*M_ElectrodeTipY_3DAtlas)*e22 + (*M_ElectrodeTipZ_3DAtlas)*e23;
	double ElecBtmZ= (*M_ElectrodeTipX_3DAtlas)*e31 + (*M_ElectrodeTipY_3DAtlas)*e32 + (*M_ElectrodeTipZ_3DAtlas)*e33;
	(*M_ElectrodeTopX_3DAtlas)=ElecTopX;
	(*M_ElectrodeTopY_3DAtlas)=ElecTopY;
	(*M_ElectrodeTopZ_3DAtlas)=ElecTopZ;
	(*M_ElectrodeTipX_3DAtlas)=ElecBtmX;
	(*M_ElectrodeTipY_3DAtlas)=ElecBtmY;
	(*M_ElectrodeTipZ_3DAtlas)=ElecBtmZ;

	//Rotation of the grid unit vectors.
	double Grid_X_unit_Vector_x=1, Grid_X_unit_Vector_y=0, Grid_X_unit_Vector_z=0; 
	double Grid_Y_unit_Vector_x=0, Grid_Y_unit_Vector_y=1, Grid_Y_unit_Vector_z=0; 
	double Grid_Z_unit_Vector_x=0, Grid_Z_unit_Vector_y=0, Grid_Z_unit_Vector_z=1; 
	*M_Grid_X_unit_Vector_x= Grid_X_unit_Vector_x*e11 + Grid_X_unit_Vector_y*e12 + Grid_X_unit_Vector_z*e13;
	*M_Grid_X_unit_Vector_y= Grid_X_unit_Vector_x*e21 + Grid_X_unit_Vector_y*e22 + Grid_X_unit_Vector_z*e23;
	*M_Grid_X_unit_Vector_z= Grid_X_unit_Vector_x*e31 + Grid_X_unit_Vector_y*e32 + Grid_X_unit_Vector_z*e33;
	*M_Grid_Y_unit_Vector_x= Grid_Y_unit_Vector_x*e11 + Grid_Y_unit_Vector_y*e12 + Grid_Y_unit_Vector_z*e13;
	*M_Grid_Y_unit_Vector_y= Grid_Y_unit_Vector_x*e21 + Grid_Y_unit_Vector_y*e22 + Grid_Y_unit_Vector_z*e23;
	*M_Grid_Y_unit_Vector_z= Grid_Y_unit_Vector_x*e31 + Grid_Y_unit_Vector_y*e32 + Grid_Y_unit_Vector_z*e33;
	*M_Grid_Z_unit_Vector_x= Grid_Z_unit_Vector_x*e11 + Grid_Z_unit_Vector_y*e12 + Grid_Z_unit_Vector_z*e13;
	*M_Grid_Z_unit_Vector_y= Grid_Z_unit_Vector_x*e21 + Grid_Z_unit_Vector_y*e22 + Grid_Z_unit_Vector_z*e23;
	*M_Grid_Z_unit_Vector_z= Grid_Z_unit_Vector_x*e31 + Grid_Z_unit_Vector_y*e32 + Grid_Z_unit_Vector_z*e33;

	//Translation
	(*M_ElectrodeTopX_3DAtlas)+=*M_GridCenter_X;		(*M_ElectrodeTopY_3DAtlas)+=*M_GridCenter_Y;		(*M_ElectrodeTopZ_3DAtlas)+=*M_GridCenter_Z; 
	(*M_ElectrodeTipX_3DAtlas)+=*M_GridCenter_X;		(*M_ElectrodeTipY_3DAtlas)+=*M_GridCenter_Y;		(*M_ElectrodeTipZ_3DAtlas)+=*M_GridCenter_Z; 

	//Register the initial position of the electrode tip.
	*M_InitElecBtmX=(*M_ElectrodeTipX_3DAtlas); *M_InitElecBtmY=(*M_ElectrodeTipY_3DAtlas); *M_InitElecBtmZ=(*M_ElectrodeTipZ_3DAtlas); 
	InformOtherProgramsToUpdate();
}

//F1, F2, F3, F4, F5, F6, user input to move the electrode in 3D Mode; along the 3 Allocentric 3D coordinate axes (not grid coordinate)
void CAnalysisView::Translate_Electrode(int AxisID, double Distance)
{
	if(AxisID==0){
		(*M_ElectrodeTopX_3DAtlas)+=Distance; (*M_ElectrodeTipX_3DAtlas)+=Distance; 
	}
	else if(AxisID==1){
		(*M_ElectrodeTopY_3DAtlas)+=Distance; (*M_ElectrodeTipY_3DAtlas)+=Distance; 
	}
	else{//AxisID==2
		(*M_ElectrodeTopZ_3DAtlas)+=Distance; (*M_ElectrodeTipZ_3DAtlas)+=Distance; 
	}
	//Inform other programs to update
	InformOtherProgramsToUpdate();
	//Prepare for the 3D display
	SetEvent(M_KeyDownEvents[1]);
}


//Arrow up/down (2D, 3D), keys 1, 2, 3, 4 (3D only) to move the electrode; along the 3 GRID 3D coordinate axes (not 3D Allocentric coordinate) 
void CAnalysisView::Move_Elec_along_Grid_axis(int AxisID, double Distance)
{
	double dx, dy, dz;
	if(AxisID==X_GRID_AXIS){
		dx=Distance*(*M_Grid_X_unit_Vector_x);
		dy=Distance*(*M_Grid_X_unit_Vector_y);
		dz=Distance*(*M_Grid_X_unit_Vector_z);
	}
	else if(AxisID==Y_GRID_AXIS){
		dx=Distance*(*M_Grid_Y_unit_Vector_x);
		dy=Distance*(*M_Grid_Y_unit_Vector_y);
		dz=Distance*(*M_Grid_Y_unit_Vector_z);
	}
	else{
		dx=Distance*(*M_Grid_Z_unit_Vector_x);
		dy=Distance*(*M_Grid_Z_unit_Vector_y);
		dz=Distance*(*M_Grid_Z_unit_Vector_z);
	}
	(*M_ElectrodeTopX_3DAtlas)+=dx;		(*M_ElectrodeTopY_3DAtlas)+=dy;		(*M_ElectrodeTopZ_3DAtlas)+=dz;
	(*M_ElectrodeTipX_3DAtlas)+=dx;		(*M_ElectrodeTipY_3DAtlas)+=dy;		(*M_ElectrodeTipZ_3DAtlas)+=dz;
	//double Meg=M3D(dx,dy,dz);

	//Inform other programs to update
	InformOtherProgramsToUpdate();
	//Prepare for the 3D display
	SetEvent(M_KeyDownEvents[1]);			
}

void InverseMatrix3D(double &a, double &b, double &c, double &d, double &e, double &f, double &g, double &h, double &i)
{
	double A, B, C;
	double D, E, F;
	double G, H, I;
	double det;
	det=a*(e*i-f*h)-b*(i*d-f*g)+c*(d*h-e*g);

	A=e*i-f*h; D=-(b*i-c*h); G=b*f-c*e;
	B=-(d*i-f*g); E=a*i-c*g; H=-(a*f-c*d);
	C=d*h-e*g; F=-(a*h-b*g); I=(a*e-b*d);

	a=A/det; b=D/det; c=G/det;
	d=B/det; e=E/det; f=H/det;
	g=C/det; h=F/det; i=I/det;
}
	
#include "Dialog_Electrode_XYZ.h"
void CAnalysisView::OnUserInput_of_Electrode_Position()
{
	if(*M_isCoronal_Running==0 && *M_isSagittal_Running==0 && *M_is3D_Running==0){AfxMessageBox("Hello there:\n Run Coronal/Sagittal/3D program first."); return;}

	Dialog_Electrode_XYZ	ParameterBOX;
	double prvElecBtmX=(*M_ElectrodeTipX_3DAtlas), prvElecBtmY=(*M_ElectrodeTipY_3DAtlas), prvElecBtmZ=(*M_ElectrodeTipZ_3DAtlas);
	double a, b, c;
	double d, e, f;
	double g, h, i;

	a=*M_Grid_X_unit_Vector_x; b=*M_Grid_Y_unit_Vector_x; c=*M_Grid_Z_unit_Vector_x; 
	d=*M_Grid_X_unit_Vector_y; e=*M_Grid_Y_unit_Vector_y; f=*M_Grid_Z_unit_Vector_y; 
	g=*M_Grid_X_unit_Vector_z; h=*M_Grid_Y_unit_Vector_z; i=*M_Grid_Z_unit_Vector_z; 

	InverseMatrix3D(a,b,c,d,e,f,g,h,i);
	
	double PrvEltdPosX_inGridCoordinate=DotProduct3D(*M_Grid_X_unit_Vector_x, *M_Grid_X_unit_Vector_y, *M_Grid_X_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_GridCenter_X , (*M_ElectrodeTipY_3DAtlas)-*M_GridCenter_Y , (*M_ElectrodeTipZ_3DAtlas)-*M_GridCenter_Z );
	double PrvEltdPosY_inGridCoordinate=DotProduct3D(*M_Grid_Y_unit_Vector_x, *M_Grid_Y_unit_Vector_y, *M_Grid_Y_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_GridCenter_X , (*M_ElectrodeTipY_3DAtlas)-*M_GridCenter_Y , (*M_ElectrodeTipZ_3DAtlas)-*M_GridCenter_Z );
	double PrvEltdPosZ_inGridCoordinate=DotProduct3D(*M_Grid_Z_unit_Vector_x, *M_Grid_Z_unit_Vector_y, *M_Grid_Z_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_InitElecBtmX, (*M_ElectrodeTipY_3DAtlas)-*M_InitElecBtmY, (*M_ElectrodeTipZ_3DAtlas)-*M_InitElecBtmZ);

	//Remove the minute errors from the calculator
	PrvEltdPosX_inGridCoordinate=Rounded_to_Nearest_Int(((int)(10000*PrvEltdPosX_inGridCoordinate))*0.001)*0.1;
	PrvEltdPosY_inGridCoordinate=Rounded_to_Nearest_Int(((int)(10000*PrvEltdPosY_inGridCoordinate))*0.001)*0.1;
	PrvEltdPosZ_inGridCoordinate=Rounded_to_Nearest_Int(((int)(10000*PrvEltdPosZ_inGridCoordinate))*0.001)*0.1;

	ParameterBOX.m_X=	PrvEltdPosX_inGridCoordinate;
	ParameterBOX.m_Y=	PrvEltdPosY_inGridCoordinate;
	//###############  The displayed sign of z is flipped: Just for convenience for the user 
	ParameterBOX.m_Z=	-PrvEltdPosZ_inGridCoordinate; 
	//#########################################################################
	if(ParameterBOX.DoModal()==IDOK){
		//###################### The sign of z is flipped: Just for the convenience for the user 
		(*M_ElectrodeTipX_3DAtlas)=DotProduct3D(ParameterBOX.m_X,ParameterBOX.m_Y,-ParameterBOX.m_Z+*M_Z_bias_for_Home,a,d,g)+*M_GridCenter_X;
		(*M_ElectrodeTipY_3DAtlas)=DotProduct3D(ParameterBOX.m_X,ParameterBOX.m_Y,-ParameterBOX.m_Z+*M_Z_bias_for_Home,b,e,h)+*M_GridCenter_Y;
		(*M_ElectrodeTipZ_3DAtlas)=DotProduct3D(ParameterBOX.m_X,ParameterBOX.m_Y,-ParameterBOX.m_Z+*M_Z_bias_for_Home,c,f,i)+*M_GridCenter_Z;
		//#########################################################################
	
		//Move the TOP of the electrode the same amount of as the tip.
		(*M_ElectrodeTopX_3DAtlas)+=(*M_ElectrodeTipX_3DAtlas)-prvElecBtmX;
		(*M_ElectrodeTopY_3DAtlas)+=(*M_ElectrodeTipY_3DAtlas)-prvElecBtmY;
		(*M_ElectrodeTopZ_3DAtlas)+=(*M_ElectrodeTipZ_3DAtlas)-prvElecBtmZ;

		//Inform the 3D display
		SetEvent(M_KeyDownEvents[1]);			
		InformOtherProgramsToUpdate();
	}
}

//The sign of Z is flipped for user's convenience only.
void CAnalysisView::Copy_ElectrodeCoordinate_toDisplay()
{
	float Xgrid, Ygrid, Zgrid;

	Xgrid=DotProduct3D(*M_Grid_X_unit_Vector_x, *M_Grid_X_unit_Vector_y, *M_Grid_X_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_GridCenter_X , (*M_ElectrodeTipY_3DAtlas)-*M_GridCenter_Y , (*M_ElectrodeTipZ_3DAtlas)-*M_GridCenter_Z );
	Ygrid=DotProduct3D(*M_Grid_Y_unit_Vector_x, *M_Grid_Y_unit_Vector_y, *M_Grid_Y_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_GridCenter_X , (*M_ElectrodeTipY_3DAtlas)-*M_GridCenter_Y , (*M_ElectrodeTipZ_3DAtlas)-*M_GridCenter_Z );
	Zgrid=DotProduct3D(*M_Grid_Z_unit_Vector_x, *M_Grid_Z_unit_Vector_y, *M_Grid_Z_unit_Vector_z, (*M_ElectrodeTipX_3DAtlas)-*M_InitElecBtmX, (*M_ElectrodeTipY_3DAtlas)-*M_InitElecBtmY, (*M_ElectrodeTipZ_3DAtlas)-*M_InitElecBtmZ);

	sprintf(m_ElecCoordinate_withGridCenterAsOrigin,"GRID Coordinate ( X %0.1lf, Y %0.1lf, Z %0.1lf )       ",	Xgrid,	Ygrid,	-Zgrid); //The sign of Z is flipped for user's convenience only.
	m_Fig[0][0][0].m_Title.NameString=m_ElecCoordinate_withGridCenterAsOrigin;
	m_ElecFromCenterLen=strlen(m_ElecCoordinate_withGridCenterAsOrigin);
}

//The 3D coordinate and the pixel coordinate has a ONE-TO-ONE mapping realtionship. No image-magnification factor plays a role
double CAnalysisView::MRI_Pixel_to_3D_Coordinate_AbscissaAxis(double Xpixel)
{
	return ( (Xpixel-m_OriginOfAbscissa_PixelCoordinate) * m_MRI_Pixel_Dimension_In_mm)- 0; //0 is the 3D space origin X.
}
double CAnalysisView::MRI_Pixel_to_3D_Coordinate_OrdinateAxis(double Zpixel)
{
	return ( (Zpixel-m_OriginOfOrdinate_PixelCoordinate) * m_MRI_Pixel_Dimension_In_mm)- 0; //0 is the ordinate (Z) axis origin in 3D space.
}
double CAnalysisView::ThD_to_MRI_Pixel_Coordinate_AbscissaAxis(double X)
{
	return (X-m_MinX_corner)/ (m_MRI_Pixel_Dimension_In_mm); 
}
double CAnalysisView::ThD_to_MRI_Pixel_Coordinate_OrdinateAxis(double Z)
{
	return (Z-m_MinZ_corner)/ (m_MRI_Pixel_Dimension_In_mm);
}
