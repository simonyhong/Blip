// ThreeD.cpp : implementation of the CThreeD class

#include "stdafx.h"
#include "stdio.h"
#include "ThreeD.h"
#include "Figures.h"
#include "AnalysisDoc.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include <afxwin.h>
#define MY_PEEK_MESSAGE {if(::PeekMessage (&message, NULL,0,0, PM_REMOVE)){::TranslateMessage(&message);::DispatchMessage(&message);}	}		


CMo::CMo():CObject()
{  };

CMo::~CMo()
{  };

CThreeD::CThreeD() : m_rectShow(0,0,800,600)
{
	m_nScreenOriginX =300;
	m_nScreenOriginY =200;
	//GetClientRect(m_rectShow);
	m_nColorShow=BLACK_BRUSH;
	//	 cocsi=0;
	m_nShowMode=GRAPH_3D_STOP;  
	m_nDrawingMode=DOT;
	m_nKey='A';
	ObjCntrX=0 ;ObjCntrY=0; ObjCntrZ=0;
	ObjCntrRotX=0;   ObjCntrRotY=0;  ObjCntrRotZ=0.;  
	ObserverCntrX=0;  ObserverCntrY=-10; ObserverCntrZ=10;	
	ObserverCntrRotX=0; ObserverCntrRotY=0;ObserverCntrRotZ=0;
	Observer2CntrRotX=-PIover2; Observer2CntrRotY=0;Observer2CntrRotZ=0;
	LtSourceX=20;  LtSourceY=5;  LtSourceZ=12; 
	Scl_Y=1;Scl_X=1;Scl_Z=1;
	h=0;
	c=0;     old_c=0;
	g=500;   m_nInitLine=0;
	for(int i=0;i<4;i++){axis[i][0]=0;axis[i][1]=0;axis[i][2]=0;} 

	//ie.SetSize(2,1); e.SetSize(2,1); mon.SetSize(2,1); old_mon.SetSize(2,1); //It's just for safety.
}


CThreeD::~CThreeD()
{ 
	m_nShowMode=GRAPH_3D_STOP;
	e.RemoveAll();
	mon.RemoveAll();
	old_mon.RemoveAll();
}

//################################################################################################
//################################    3D. Displayer   ############################################
int CThreeD::ThreeD(double* iE, int gValue, int InitLine, LPVOID objView, int disp)
{  
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)objView);										//
	CPen showPen;													//
	showPen.CreateStockObject(BLACK_PEN);							//
	showDC.SelectObject(showPen);									//
	showDC.SetTextColor(RGB(0,255,0));								//
	showDC.SetBkMode(TRANSPARENT);									//
																	//
	CClientDC eraseDC((CWnd*)objView);										//
	CPen erasePen;													//
	erasePen.CreateStockObject(WHITE_PEN);							//
	eraseDC.SelectObject(erasePen);									//
	eraseDC.SetTextColor(RGB(255,255,255));							//
	eraseDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////
	ObjCntrX=0 ;ObjCntrY=0; ObjCntrZ=0;

	//Do the following in CAnalysisView::On3DGraph() 
	//if(m_nShowMode==GRAPH_3D){m_nShowMode=GRAPH_STOP;  return 0;}	m_nShowMode=GRAPH_3D;	
	
	m_nDrawingMode=disp;
	//-------------------------------------------------
	g=gValue;	m_nInitLine=InitLine;
	ie.SetSize(3*g,1); 
	e. SetSize(3*g,1); 
	mon.SetSize(g,1);
	old_mon.SetSize(g,1);
	for(int i=0;i<g; i++){
		ie[3*i  ]=*(iE+3*i  );
		ie[3*i+1]=*(iE+3*i+1); 
		ie[3*i+2]=*(iE+3*i+2);
	}
	//------------------------------------------------
	do{
		if(m_nKey!=VK_SPACE){
 			Parameters(m_nKey);
			Working(&showDC, &eraseDC);
		}
	}while(m_nShowMode==GRAPH_3D);
	//objDC.SetBkMode(OPAQUE);
	return 1;
}

void CThreeD::Working(CClientDC* showDC, CClientDC* eraseDC)
{
	double AlloCtrcCoordt_SrfcPnt[3];//,vv,gr[3],ggr,li[3],lli,l2[3],ll2;)
		    
	ObjectRotation();
	for(int l=0;l<g;l++){
		//if(e[l][0] <TOLERANCE && e[l][1] <TOLERANCE && e[l][2] <TOLERANCE &&e[l][0] >-TOLERANCE && e[l][1] >-TOLERANCE && e[l][2] >-TOLERANCE){
	 	//if(l==200){
		//Delay(200);
		//}
		//Vectors(l);!!!
		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+e[l*3]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+e[l*3+1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+e[l*3+2];
		preRot_EgoCtrcCoordt_SrfcPnt[0]=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPnt[1]=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY; preRot_EgoCtrcCoordt_SrfcPnt[2]=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ;
		//vv =M3D(preRot_EgoCtrcCoordt_SrfcPnt[0],preRot_EgoCtrcCoordt_SrfcPnt[1],preRot_EgoCtrcCoordt_SrfcPnt[2]);
		//gr[0]=2*e[l*3]; gr[1]=2*e[l*3+1]; gr[2]=0; ggr=M3D(gr[0],gr[1],gr[2]);
		//li[0]=AlloCtrcCoordt_SrfcPnt[0]-LtSourceX;    li[1]=AlloCtrcCoordt_SrfcPnt[1]-LtSourceY;    li[2]=AlloCtrcCoordt_SrfcPnt[2]-LtSourceZ;  lli=M3D(li[0],li[1],li[2]);
		/*
		if(ggr==0){cozeta=0;l2[0]=li[0];l2[1]=li[1];l2[2]=li[2];ll2=M3D(l2[0],l2[1],l2[2]);
			coalpha=0;cocsi=(l2[0]*(-preRot_EgoCtrcCoordt_SrfcPnt[0])+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPnt[1])+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPnt[2]))/(ll2*vv);
		}
		else {
			cozeta=Divide(-(gr[0]*li[0]+gr[1]*li[1]+gr[2]*li[2]),(ggr*lli));
			l2[0]=li[0]+2*(gr[0]/ggr)*lli*cozeta;
			l2[1]=li[1]+2*(gr[1]/ggr)*lli*cozeta;
			l2[2]=li[2]+2*(gr[2]/ggr)*lli*cozeta;
			ll2=M3D(l2[0],l2[1],l2[2]);
			cocsi=Divide((l2[0]*(-preRot_EgoCtrcCoordt_SrfcPnt[0])+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPnt[1])+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPnt[2])),(ll2*vv));
			coalpha=Divide((preRot_EgoCtrcCoordt_SrfcPnt[0]*gr[0]+preRot_EgoCtrcCoordt_SrfcPnt[1]*gr[1]+preRot_EgoCtrcCoordt_SrfcPnt[2]*gr[2]),(ggr*vv));
		}
		*/
		//End of Vectors(l);!!!
		/*
		if(CHOICE==9){          //9: Sphere()
			if(coalpha<=0)	VisibleArea();
		}
		else */VisibleArea();
	//}
	}
	if(old_c!=0)m_pAnalysisView->m_3D1.EraseFrameBlack(&(m_pAnalysisView->m_DC[0]));
	if(m_nDrawingMode!=NETWORK)CoordinateAxis(showDC);
	//Light();
	Show(showDC);
	// if(disp==LINE)Line(c);
	old_c=c;
	c=0;
}

void CThreeD::VisibleArea()
{
/*	if(CHOICE==9){                    //9: Sphere()
		mon[c].color=RGB(0,0,0);
		ObserverRotation();

		if(cozeta>=0){        // lighted area
			mon[c].color=RGB(255,0,0);
			ThreeD_to_2D_Mapping();
			//if(cocsi>=.95){
			//Reflection();
			//}
		}
		else ThreeD_to_2D_Mapping();
	}
	else{//구가 아니면
*/		ObserverRotation();
		mon[c].color=RGB(200,0,0);
		ThreeD_to_2D_Mapping();
//	}
}

void CThreeD::Parameters(int Key)
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;

	switch(Key){
		// Observer
	//	case VK_LEFT :ObserverCntrX-=1;break;			case VK_RIGHT:ObserverCntrX+=1;break;
	//	case VK_F11  :ObserverCntrY-=0.3;break;			case VK_F12:ObserverCntrY+=0.3;break;
	//	case VK_DOWN :ObserverCntrZ-=1;break;			case VK_UP :ObserverCntrZ+=1;break;
		case VK_F11  :ObserverCntrY-=0.3;break;			case VK_F12:ObserverCntrY+=0.3;break;

		case VK_LEFT :ObserverCntrRotZ+=0.001;break;	case VK_RIGHT:ObserverCntrRotZ-=0.001;break;
		case VK_UP  :ObserverCntrX+=0.1*cos(ObserverCntrRotZ); ObserverCntrY+=0.1*sin(ObserverCntrRotZ); break;		
		case VK_DOWN:ObserverCntrX-=0.1*cos(ObserverCntrRotZ); ObserverCntrY-=0.1*sin(ObserverCntrRotZ); break;	

		// Light Source
		case 'g':LtSourceX+=3;break;			    case 'd':LtSourceX-= 3;break;
		case 'f':LtSourceY+=3;break;				case ' ':LtSourceY-= 3;break;
		//case 'r':LtSourceZ+=3;break;				case 'v':LtSourceZ-= 3;break;
		
		// 
		//case 'u':ObserverCntrRotZ+=.06;break;    		case 'm':ObserverCntrRotZ-=.06;break;
		case 'VK_H':ObserverCntrRotX+=.0006;break;    		case 'VK_K':ObserverCntrRotX-=.0006;break;
		case 'j':ObserverCntrRotY+=.06;break;    		case 'n':ObserverCntrRotY-=.06;break;

		case VK_NUMPAD2:ObjCntrRotX+=0.05;break;      case '*':ObjCntrRotX-=.1;break;
		//case VK_UP:ObjCntrRotY+=0.05;break;      //case VK_DOWN:ObjCntrRotY-=.1;break;
		case VK_HOME:ObjCntrRotZ+=0.01;break;      case VK_END:ObjCntrRotZ-=0.01;break;

		case VK_F2 :Scl_X+=.01;            break;
		case VK_F1 :if(Scl_X>.3)Scl_X-=.01; break;
		case VK_F4 :Scl_Z+=.01;            break;
		case VK_F3 :if(Scl_Z>.3)Scl_Z-=.01; break;
		case VK_F6:Scl_Y+=.01;		    break;
		case VK_F5:if(Scl_Y>.5)Scl_Y-=.01; break;

		default:break;
	}

	coswx=cos(ObjCntrRotX);coswy=cos(ObjCntrRotY);coswz=cos(ObjCntrRotZ);sinwx=sin(ObjCntrRotX);sinwy=sin(ObjCntrRotY);sinwz=sin(ObjCntrRotZ);	
	e11=  coswy*coswz*Scl_X;
	e12=( coswz*sinwy*sinwx+coswx*sinwz)*Scl_Z;
	e13=(-coswz*sinwy*coswx+sinwz*sinwx )*Scl_Y;
	e21= -coswy*sinwz*Scl_X;
	e22=(-sinwz*sinwy*sinwx+coswx*coswz)*Scl_Z;
	e23=( sinwz*sinwy*coswx+coswz*sinwx)*Scl_Y;
	e31=  sinwy*Scl_X;
	e32=(-sinwx*coswy)*Scl_Z;
	e33=  coswx*coswy*Scl_Y;
	
	coswx=cos(ObserverCntrRotX);coswy=cos(ObserverCntrRotY);coswz=cos(ObserverCntrRotZ);sinwx=sin(ObserverCntrRotX);sinwy=sin(ObserverCntrRotY);sinwz=sin(ObserverCntrRotZ);
	ev11= coswy*coswz;
	ev12= coswz*sinwy*sinwx+coswx*sinwz;
	ev13=-coswz*sinwy*coswx+sinwz*sinwx;
	ev21=-coswy*sinwz;
	ev22=-sinwz*sinwy*sinwx+coswx*coswz;
	ev23= sinwz*sinwy*coswx+coswz*sinwx;
	ev31= sinwy;
	ev32=-sinwx*coswy;
	ev33= coswx*coswy;
}

void CThreeD::SimpleParameters(int Key)
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;
#define StepSize 0.5

	switch(Key){
		// PIover2 has been added because I defined that y axis (_|_)is 0 degree.
		case VK_UP  :ObserverCntrX+=StepSize*cos(ObserverCntrRotZ+PIover2); ObserverCntrY+=StepSize*sin(ObserverCntrRotZ+PIover2); break;		
		case VK_DOWN:ObserverCntrX-=StepSize*cos(ObserverCntrRotZ+PIover2); ObserverCntrY-=StepSize*sin(ObserverCntrRotZ+PIover2); break;	

		case VK_LEFT :ObserverCntrRotZ+=0.04;break;		case VK_RIGHT:ObserverCntrRotZ-=0.04;break;
		case 'VK_H'  :ObserverCntrRotX+=.0006;break;   	case 'VK_K':ObserverCntrRotX-=.0006;break;
		case 'j'     :ObserverCntrRotY+=.06;break;  	case 'n':ObserverCntrRotY-=.06;break;

		default:break;
	}
	coswx=cos(ObserverCntrRotX);coswy=cos(ObserverCntrRotY);coswz=cos(ObserverCntrRotZ);sinwx=sin(ObserverCntrRotX);sinwy=sin(ObserverCntrRotY);sinwz=sin(ObserverCntrRotZ);
	ev11= coswy*coswz;
	ev12= coswz*sinwy*sinwx+coswx*sinwz;
	ev13=-coswz*sinwy*coswx+sinwz*sinwx;
	ev21=-coswy*sinwz;
	ev22=-sinwz*sinwy*sinwx+coswx*coswz;
	ev23= sinwz*sinwy*coswx+coswz*sinwx;
	ev31= sinwy;
	ev32=-sinwx*coswy;
	ev33= coswx*coswy;
}

void CThreeD::ParametersForObserver2()
{
	double coswx,coswy,coswz,sinwx,sinwy,sinwz;

	coswx=cos(Observer2CntrRotX);coswy=cos(Observer2CntrRotY);coswz=cos(Observer2CntrRotZ);sinwx=sin(Observer2CntrRotX);sinwy=sin(Observer2CntrRotY);sinwz=sin(Observer2CntrRotZ);
	ev11= coswy*coswz;
	ev12= coswz*sinwy*sinwx+coswx*sinwz;
	ev13=-coswz*sinwy*coswx+sinwz*sinwx;
	ev21=-coswy*sinwz;
	ev22=-sinwz*sinwy*sinwx+coswx*coswz;
	ev23= sinwz*sinwy*coswx+coswz*sinwx;
	ev31= sinwy;
	ev32=-sinwx*coswy;
	ev33= coswx*coswy;
}

void CThreeD::ObjectRotation()
{
	int k;
	for(k=0;k<(g);k++){
		e[k*3  ]= ie[k*3]*e11 + ie[k*3+1]*e12 + ie[k*3+2]*e13;
		e[k*3+1]= ie[k*3]*e21 + ie[k*3+1]*e22 + ie[k*3+2]*e23;
		e[k*3+2]= ie[k*3]*e31 + ie[k*3+1]*e32 + ie[k*3+2]*e33;
	}
}

void CThreeD::ObserverRotation()
{
	EgoCtrcCoordt_SrfcPnt[0]= preRot_EgoCtrcCoordt_SrfcPnt[0]*ev11 +preRot_EgoCtrcCoordt_SrfcPnt[1]*ev12 +preRot_EgoCtrcCoordt_SrfcPnt[2]*ev13;
	EgoCtrcCoordt_SrfcPnt[1]= preRot_EgoCtrcCoordt_SrfcPnt[0]*ev21 +preRot_EgoCtrcCoordt_SrfcPnt[1]*ev22 +preRot_EgoCtrcCoordt_SrfcPnt[2]*ev23;
	EgoCtrcCoordt_SrfcPnt[2]= preRot_EgoCtrcCoordt_SrfcPnt[0]*ev31 +preRot_EgoCtrcCoordt_SrfcPnt[1]*ev32 +preRot_EgoCtrcCoordt_SrfcPnt[2]*ev33;
}

void CThreeD::CoordinateAxis(CClientDC* objDC)
{
	CPen objPen;
	objPen.CreateStockObject(WHITE_PEN);
	//objDC->SelectObject(objPen);
	m_pAnalysisView->m_DC[0].SelectObject(objPen);

	int i=0,n,k,x,y;
	double AlloCtrcCoordt_SrfcPnt[3];
	//double iaxis[4][3];
	
	//Object-Centered axis
//	iaxis[i][0]=0;  iaxis[i][1]=0;  iaxis[i++][2]=0;
//	iaxis[i][0]=10; iaxis[i][1]=0;  iaxis[i++][2]=0;
//	iaxis[i][0]=0;  iaxis[i][1]=10; iaxis[i++][2]=0;
//	iaxis[i][0]=0;  iaxis[i][1]=0;  iaxis[i++][2]=10;
	axis[i][0]=0;  axis[i][1]=0;  axis[i++][2]=0;
	axis[i][0]=10; axis[i][1]=0;  axis[i++][2]=0;
	axis[i][0]=0;  axis[i][1]=10; axis[i++][2]=0;
	axis[i][0]=0;  axis[i][1]=0;  axis[i++][2]=10;

//	for(k=0;k<i;k++){ //BodyRotation , They can be loaded to itself because they are initialized each time of calling CoordinateAxis().
//		axis[k][0]= iaxis[k][0]*e11 +iaxis[k][1]*e12 +iaxis[k][2]*e13;
//		axis[k][1]= iaxis[k][0]*e21 +iaxis[k][1]*e22 +iaxis[k][2]*e23;
//		axis[k][2]= iaxis[k][0]*e31 +iaxis[k][1]*e32 +iaxis[k][2]*e33;
//	}
	#define Tolerance 3
	for(n=0;n<i;n++){         //Working
		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+axis[n][0]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+axis[n][1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+axis[n][2];
		preRot_EgoCtrcCoordt_SrfcPnt[0]=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPnt[1]=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY;    preRot_EgoCtrcCoordt_SrfcPnt[2]=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ;
		ObserverRotation();
		if(fabs(EgoCtrcCoordt_SrfcPnt[1])<Tolerance){
			if(n==0)break;
			continue;
		}
		IsoMapping(&x,&y);   // It only handles the X,Y,Z axies, not the original data.
		AxisPoint[n].x =x; AxisPoint[n].y =y;   
		if(n==0)continue;
		m_pAnalysisView->m_3D1.Graph2D(AxisPoint[0].x,AxisPoint[0].y,9, &(m_pAnalysisView->m_DC[0]));
		m_pAnalysisView->m_3D1.Graph2D(AxisPoint[n].x,AxisPoint[n].y,2, &(m_pAnalysisView->m_DC[0]));
	}
	for(n=1;n<i;n++){         //Show
		//objDC->MoveTo(AxisPoint[0]);
		//objDC->LineTo(AxisPoint[n]);
	}
}

void CThreeD::ThreeD_to_2D_Mapping()                       // Quick Mapping 
{
	mon[c  ].point.x=(int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]);
	mon[c++].point.y=(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]);
}

void CThreeD::IsoMapping(int* x,int* y)
{
	*x=(int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]);
	*y=(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]);
}

void CThreeD::Show(CClientDC* objDC)
{
	int Idx;
	char chTemp[5];

	switch(m_nDrawingMode){
	case NETWORK:
		objDC->SetTextColor(RGB(255,0,0));
		for(Idx=0;Idx<m_nInitLine && Idx<c ;Idx++){
			sprintf(chTemp,"%d",Idx);
			objDC->TextOut(mon[Idx].point.x, mon[Idx].point.y, chTemp);
			old_mon[Idx  ].point=mon[Idx  ].point;
		}
		for(;Idx<c-1; Idx+=2){
			objDC->MoveTo(mon[Idx  ].point);
			objDC->LineTo(mon[Idx+1].point);
			old_mon[Idx  ].point=mon[Idx  ].point;
			old_mon[Idx+1].point=mon[Idx+1].point;
		}
		break;
	
	default:
		for(Idx=0; Idx<c ; Idx++){
			//objDC->SetPixel(mon[Idx].point.x, mon[Idx].point.y ,RGB(255,0,0));
			m_pAnalysisView->m_3D1.Graph2D(mon[Idx].point.x,mon[Idx].point.y,5, objDC);
			old_mon[Idx].point=mon[Idx].point;
		}
	}
}


void CThreeD::Erase(CClientDC* objDC)
{
 //	CFont objFont;
//	objRont.CreateStpclObject(SYSTEM_FONT);
//	objDC->SelectObject(objFont);
	//if(disp==LINE)for(n=0;n<old_c-1;n++)line(((old_mon)+n)->mox,((old_mon)+n)->moy,((old_mon)+n+1)->mox,((old_mon)+n+1)->moy);//from dot to dot
	//if(disp==DOT)
	char chTemp[5];
	int Idx;
	switch(m_nDrawingMode){
	case NETWORK:
		for(Idx=0; Idx<m_nInitLine && Idx<old_c; Idx++){
			sprintf(chTemp,"%d",Idx);
			objDC->TextOut(old_mon[Idx].point.x, old_mon[Idx].point.y, chTemp);
		}
		for(;Idx<old_c-1; Idx+=2){
			objDC->MoveTo(old_mon[Idx  ].point);	
			objDC->LineTo(old_mon[Idx+1].point);
		}
	default:
		for(Idx=1;Idx<4;Idx++){     //Erase X,Y,Z "Axis"
			objDC->MoveTo(AxisPoint[ 0 ]);
			objDC->LineTo(AxisPoint[Idx]);
		}
		for( Idx=0; Idx<old_c ;Idx++){
			objDC->SetPixel(old_mon[Idx].point, RGB(255,255,255));
		}
	}
}

// The vector from Origin to the origin of the object is   
// -- BB=(ObjCntrX,ObjCntrY,ObjCntrZ) --                                  
// The vector from the Origin to the surface point G(e[l][0],e[l][1],e[l][2]) is 
//  -- TT2=BB+TT=(ObjCntrX+e[l][0],ObjCntrY+e[l][1],ObjCntrZ+e[l][2]);TT2(AlloCtrcCoordt_SrfcPnt[]x,AlloCtrcCoordt_SrfcPnt[]y,AlloCtrcCoordt_SrfcPnt[]z) 
// The observing vector, from V(ObserverCntrX,ObserverCntrY,ObserverCntrZ) to G(e[l][0],e[l][1],e[l][2]) is 
// -- VV(AlloCtrcCoordt_SrfcPnt[]-ObserverCntrX,AlloCtrcCoordt_SrfcPnt[]-ObserverCntrY,AlloCtrcCoordt_SrfcPnt[]-ObserverCntrZ) ;VV(vx,vy,vz) --            
// The gradiant at the point G(e[l][0],e[l][1],e[l][2]) is 
// -- GG=(2*e[l][0], 2*e[l][1], 2*e[l][2]);GG(gr[l][]x,gr[]y,gr[]z) -- 
//  The vector from the light-source to  the point G(e[l][0],e[l][1],e[l][2]) is
// -- LL=(e[l][0]-LtSourceX,e[l][1]-LtSourceY,e[l][2]-(*st??LtSourceZ);LL(lx,li[]y,li[]z) --
// The vector which is reflected and has the same magnitude of LL is 
// -- LL2=LL+2*(GG/gr)*lli*cos(zeta)  ;LL2(l2[]x,l2[]y,l2[]z) --  
// -- cos(zeta) = -((GG @ LL)/(gr*lli)) --                
//  The  cosine,   between  reflected light(LL2)  and  negative observing vector(-VV) is 
// -- cocsi=((LL2 @ -VV)/(ll2*vv)) --                      


/*
void Vectors(double* e,int l)
{
		double ggr,vv,AlloCtrcCoordt_SrfcPnt[3],gr[3],li[3],lli,l2[3],ll2;

		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+e[l][0]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+e[l][1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+e[l][2];
		preRot_EgoCtrcCoordt_SrfcPnt[0]=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPnt[1]=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY; preRot_EgoCtrcCoordt_SrfcPnt[2]=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ; vv =M3D(preRot_EgoCtrcCoordt_SrfcPnt[0],preRot_EgoCtrcCoordt_SrfcPnt[1],preRot_EgoCtrcCoordt_SrfcPnt[2]);
		gr[0]=2*e[l][0]; gr[1]=2*e[l][1]; gr[2]=0; ggr=M3D(gr[0],gr[1],gr[2]);
		li[0]=AlloCtrcCoordt_SrfcPnt[0]-LtSourceX;    li[1]=AlloCtrcCoordt_SrfcPnt[1]-LtSourceY;    li[2]=AlloCtrcCoordt_SrfcPnt[2]-LtSourceZ;  lli=M3D(li[0],li[1],li[2]);
	     //	if(l==17){
	     //	  Delay(20);
	     //	}
		if(ggr==0){cozeta=0;l2[0]=li[0];l2[1]=li[1];l2[2]=li[2];ll2=M3D(l2[0],l2[1],l2[2]);
				   coalpha=0;cocsi=(l2[0]*(-preRot_EgoCtrcCoordt_SrfcPnt[0])+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPnt[1])+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPnt[2]))/(ll2*vv);}
		else {
			cozeta=Divide(-(gr[0]*li[0]+gr[1]*li[1]+gr[2]*li[2]),(ggr*lli));
			l2[0]=li[0]+2*(gr[0]/ggr)*lli*cozeta;
			l2[1]=li[1]+2*(gr[1]/ggr)*lli*cozeta;
			l2[2]=li[2]+2*(gr[2]/ggr)*lli*cozeta;
			ll2=M3D(l2[0],l2[1],l2[2]);
			cocsi=Divide((l2[0]*(-preRot_EgoCtrcCoordt_SrfcPnt[0])+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPnt[1])+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPnt[2])),(ll2*vv));
			coalpha=Divide((preRot_EgoCtrcCoordt_SrfcPnt[0]*gr[0]+preRot_EgoCtrcCoordt_SrfcPnt[1]*gr[1]+preRot_EgoCtrcCoordt_SrfcPnt[2]*gr[2]),(ggr*vv));
		}
}

  void Reflection()
{
	setcolor(15);
	setfillstyle(SOLID_FILL,15);
	bar( ((mon)+(c-1))->mox,((mon)+(c-1))->moy,((mon)+(c-1))->mox+4,((mon)+(c-1))->moy+4);
	setcolor(0);
	setfillstyle(SOLID_FILL,0);
	bar(((mon)+(c-1))->mox,((mon)+(c-1))->moy,((mon)+(c-1))->mox+4,((mon)+(c-1))->moy+4);
}

 
void Light()
{
	  int ex,ey,ez;
		 int n;
//		 int x,y;

		 setcolor(15);
		 for(n=0;n<10;n++){
			ex=random(2)-1;  ey=random(2)-1;  ez=random(2)-1;
			preRot_EgoCtrcCoordt_SrfcPnt[0]=LtSourceX-ex-ObserverCntrX;   preRot_EgoCtrcCoordt_SrfcPnt[1]=LtSourceY-ey-ObserverCntrY;   preRot_EgoCtrcCoordt_SrfcPnt[2]=LtSourceZ-ez-ObserverCntrZ;
			if(preRot_EgoCtrcCoordt_SrfcPnt[1]>5 && preRot_EgoCtrcCoordt_SrfcPnt[1]<TOLERANCE){
			   ObserverRotation(sts);
			   x0[n]=getmaxx()/2+500*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1];
			   y0[n]=getmaxy()/2-500*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1];
	  //       IsoMapping(&x,&y);
			   preRot_EgoCtrcCoordt_SrfcPnt[0]=LtSourceX+ex-ObserverCntrX;   preRot_EgoCtrcCoordt_SrfcPnt[1]=LtSourceY+ey-ObserverCntrY;   preRot_EgoCtrcCoordt_SrfcPnt[2]=LtSourceZ+ez-ObserverCntrZ;
			   ObserverRotation(sts);
			   x1[n]=getmaxx()/2+500*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1];
			   y1[n]=getmaxy()/2-500*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1];
	  //	   IsoMapping(&x,&y);
			   line(x0[n],y0[n],x1[n],y1[n]);
			}
		 }
} 

void Line(int c)
{
   int n;
   setcolor(12);
   for(n=0;n<c-1;n++){           // from dot to dot
	line(((mon)+n)->mox,((mon)+n)->moy,((mon)+n+1)->mox,((mon)+n+1)->moy);
   }
}
*/


void CThreeD::KeyDown(UINT nChar) 
{
	if(nChar==34){  //PgDown 
		if(0.05>0.002)m_dW-=.001;
		if(0.1>0.1)m_dFar-=0.01;
		return;
	}
	if(nChar==33){  //PgUp
		if(0.05<0.2)m_dW+=.001;
		if(0.1<3.)m_dFar+=0.01;
		return;
	}
	m_nKey=nChar;
}

void CThreeD::Stop() 
{	m_nShowMode=GRAPH_3D_STOP;	}

//############################################  2D  Implementation ########################################
CTwoD::CTwoD()
{	m_nShowMode=GRAPH_3D_STOP;	}

CTwoD::~CTwoD()
{	m_nShowMode=GRAPH_3D_STOP;	}


int CTwoD::TwoD()
{
	if(m_nShowMode==GRAPH_2D){m_nShowMode=GRAPH_3D_STOP;  return 0;}	
	m_nShowMode=GRAPH_2D;
	return 1;
}

// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************
// ***********************************************************************************************

int CThreeD::ThreeD2(double* iE, int gValue, int InitLine, LPVOID objView, int disp, int ScreenX)
{  
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)objView);								//
	CPen showPen;													//
	showPen.CreateStockObject(BLACK_PEN);							//
	showDC.SelectObject(showPen);									//
	showDC.SetTextColor(RGB(0,255,0));								//
	showDC.SetBkMode(TRANSPARENT);									//
																	//
	CClientDC eraseDC((CWnd*)objView);								//
	CPen erasePen;													//
	erasePen.CreateStockObject(WHITE_PEN);							//
	eraseDC.SelectObject(erasePen);									//
	eraseDC.SetTextColor(RGB(255,255,255));							//
	eraseDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////
	ObjCntrX=(ScreenX>-100 && ScreenX<200)?ScreenX:200; ObjCntrY=25; ObjCntrZ=10;

	ie.SetSize(2,1); e.SetSize(2,1); mon.SetSize(2,1); old_mon.SetSize(2,1); //It's just for safety.


	//Do the following in CAnalysisView::On3DGraph() 
	//if(m_nShowMode==GRAPH_3D){m_nShowMode=GRAPH_STOP;  return 0;}	m_nShowMode=GRAPH_3D;	
	
	m_nDrawingMode=disp;
	//-------------------------------------------------
	g=gValue;	m_nInitLine=InitLine;
//	ie.SetSize(3*g,1); 
	for(int i=0;i<g; i++){
	//	Init_ObjCntrd_Coordnt_SrfcPoint[i][0]=*(iE+3*i  );
	//	Init_ObjCntrd_Coordnt_SrfcPoint[i][1]=*(iE+3*i+1); 
	//	Init_ObjCntrd_Coordnt_SrfcPoint[i][2]=*(iE+3*i+2);
	}
//	e. SetSize(3*g,1); 
//	mon.SetSize(g,1);
//	old_mon.SetSize(g,1);
	//------------------------------------------------
	MSG message;

//	CHOICE=Cho;
	
	do{
		if(m_nKey!=VK_SPACE){
 			Parameters(m_nKey);
			Working2(&showDC, &eraseDC);
		}
		MY_PEEK_MESSAGE;
	}while(m_nShowMode==GRAPH_3D);
	//objDC.SetBkMode(OPAQUE);
	return 1;
}
	
void CThreeD::Working2(CClientDC* showDC, CClientDC* eraseDC)
{
	double AlloCtrcCoordt_SrfcPnt[3];//,vv,gr[3],ggr,li[3],lli,l2[3],ll2;)
		    
	ObjectRotation2();
	for(int l=0;l<g;l++){
		//if(ObjCntrd_Coordnt_SrfcPoint[l][0] <TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[l][1] <TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[l][2] <TOLERANCE &&ObjCntrd_Coordnt_SrfcPoint[l][0] >-TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[l][1] >-TOLERANCE && ObjCntrd_Coordnt_SrfcPoint[l][2] >-TOLERANCE){
	 	//if(l==200){
		//Delay(200);
		//}
		//Vectors(l);!!!
		AlloCtrcCoordt_SrfcPnt[0]=ObjCntrX+ObjCntrd_Coordnt_SrfcPoint[l][0]; AlloCtrcCoordt_SrfcPnt[1]=ObjCntrY+ObjCntrd_Coordnt_SrfcPoint[l][1]; AlloCtrcCoordt_SrfcPnt[2]=ObjCntrZ+ObjCntrd_Coordnt_SrfcPoint[l][2];
		preRot_EgoCtrcCoordt_SrfcPnt[0]=AlloCtrcCoordt_SrfcPnt[0]-ObserverCntrX;    preRot_EgoCtrcCoordt_SrfcPnt[1]=AlloCtrcCoordt_SrfcPnt[1]-ObserverCntrY; preRot_EgoCtrcCoordt_SrfcPnt[2]=AlloCtrcCoordt_SrfcPnt[2]-ObserverCntrZ;
		//vv =M3D(preRot_EgoCtrcCoordt_SrfcPnt[0],preRot_EgoCtrcCoordt_SrfcPnt[1],preRot_EgoCtrcCoordt_SrfcPnt[2]);
		//gr[0]=2*ObjCntrd_Coordnt_SrfcPoint[l][0]; gr[1]=2*ObjCntrd_Coordnt_SrfcPoint[l][1]; gr[2]=0; ggr=M3D(gr[0],gr[1],gr[2]);
		//li[0]=AlloCtrcCoordt_SrfcPnt[0]-LtSourceX;    li[1]=AlloCtrcCoordt_SrfcPnt[1]-LtSourceY;    li[2]=AlloCtrcCoordt_SrfcPnt[2]-LtSourceZ;  lli=M3D(li[0],li[1],li[2]);
		/*
		if(ggr==0){cozeta=0;l2[0]=li[0];l2[1]=li[1];l2[2]=li[2];ll2=M3D(l2[0],l2[1],l2[2]);
			coalpha=0;cocsi=(l2[0]*(-preRot_EgoCtrcCoordt_SrfcPnt[0])+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPnt[1])+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPnt[2]))/(ll2*vv);
		}
		else {
			cozeta=Divide(-(gr[0]*li[0]+gr[1]*li[1]+gr[2]*li[2]),(ggr*lli));
			l2[0]=li[0]+2*(gr[0]/ggr)*lli*cozeta;
			l2[1]=li[1]+2*(gr[1]/ggr)*lli*cozeta;
			l2[2]=li[2]+2*(gr[2]/ggr)*lli*cozeta;
			ll2=M3D(l2[0],l2[1],l2[2]);
			cocsi=Divide((l2[0]*(-preRot_EgoCtrcCoordt_SrfcPnt[0])+l2[1]*(-preRot_EgoCtrcCoordt_SrfcPnt[1])+l2[2]*(-preRot_EgoCtrcCoordt_SrfcPnt[2])),(ll2*vv));
			coalpha=Divide((preRot_EgoCtrcCoordt_SrfcPnt[0]*gr[0]+preRot_EgoCtrcCoordt_SrfcPnt[1]*gr[1]+preRot_EgoCtrcCoordt_SrfcPnt[2]*gr[2]),(ggr*vv));
		}
		*/
		//End of Vectors(l);!!!
		/*
		if(CHOICE==9){          //9: Sphere()
			if(coalpha<=0)	VisibleArea();
		}
		else */VisibleArea2();
	//}
	}
	if(old_c!=0)Erase2(eraseDC);
	if(m_nDrawingMode!=NETWORK)CoordinateAxis(showDC);
	//Light();
	Show2(showDC);
	// if(disp==LINE)Line(c);
	old_c=c;
	c=0;
}

void CThreeD::VisibleArea2()
{
/*	if(CHOICE==9){                    //9: Sphere()
		mon[c].color=RGB(0,0,0);
		ObserverRotation();

		if(cozeta>=0){        // lighted area
			mon[c].color=RGB(255,0,0);
			ThreeD_to_2D_Mapping();
			//if(cocsi>=.95){
			//Reflection();
			//}
		}
		else ThreeD_to_2D_Mapping();
	}
	else{//구가 아니면
*/		ObserverRotation();
		//MON[c].color=RGB(200,0,0);
		Mapping2();
//	}
}


void CThreeD::ObjectRotation2()
{
	int k;
	for(k=0;k<(g);k++){
	//	ObjCntrd_Coordnt_SrfcPoint[k][0]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e11 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e12 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e13;
	//	ObjCntrd_Coordnt_SrfcPoint[k][1]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e21 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e22 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e23;
	//	ObjCntrd_Coordnt_SrfcPoint[k][2]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e31 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e32 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e33;
	}
}

void CThreeD::Mapping2()                       // Quick Mapping 
{
	MON[c  ].point.x=(int)(m_nScreenOriginX + Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]);
	MON[c++].point.y=(int)(m_nScreenOriginY + Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]);
}


void CThreeD::Show2(CClientDC* objDC)
{
}


void CThreeD::Erase2(CClientDC* objDC)
{
}



// ***********************************************************************************************
// This is the currently used function.

int CThreeD::ThreeD3(int gValue, int InitLine, LPVOID objView, int disp)
{
	//////////////////////////////////////////////////////////////////
	CClientDC showDC((CWnd*)objView);								//
	CPen showPen;													//
	CPen PenForActor(PS_INSIDEFRAME,1,RGB(255,0,0));				//													//
	showPen.CreateStockObject(WHITE_PEN);//BLACK_PEN);				//
	showDC.SelectObject(showPen);									//
	showDC.SetTextColor(RGB(0,255,0));								//
	showDC.SetBkMode(TRANSPARENT);									//
	//////////////////////////////////////////////////////////////////

	//Do the following in CAnalysisView::On3DGraph() 
	//if(m_nShowMode==GRAPH_3D){m_nShowMode=GRAPH_STOP;  return 0;}	m_nShowMode=GRAPH_3D;	
	
	m_nDrawingMode=disp;
	//-------------------------------------------------
	g=gValue;	m_nInitLine=InitLine;
	MSG message;

	int k,l;
	do{
		if(m_nKey!=VK_SPACE){
			/////////////////  Observer 1  //////////////
 			SimpleParameters(m_nKey);

			//for(k=0;k<(g);k++){//Object Rotation
			//	ObjCntrd_Coordnt_SrfcPoint[k][0]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e11 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e12 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e13;
			//	ObjCntrd_Coordnt_SrfcPoint[k][1]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e21 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e22 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e23;
			//	ObjCntrd_Coordnt_SrfcPoint[k][2]= Init_ObjCntrd_Coordnt_SrfcPoint[k][0]*e31 + Init_ObjCntrd_Coordnt_SrfcPoint[k][1]*e32 + Init_ObjCntrd_Coordnt_SrfcPoint[k][2]*e33;
			//}
			for(l=0;l<g;l++){
				preRot_EgoCtrcCoordt_SrfcPnt[0]=(ObjCntrX+ObjCntrd_Coordnt_SrfcPoint[l][0]) -ObserverCntrX;   
				preRot_EgoCtrcCoordt_SrfcPnt[1]=(ObjCntrY+ObjCntrd_Coordnt_SrfcPoint[l][1]) -ObserverCntrY;   
				preRot_EgoCtrcCoordt_SrfcPnt[2]=(ObjCntrZ+ObjCntrd_Coordnt_SrfcPoint[l][2]) -ObserverCntrZ;
				ObserverRotation();
				if(EgoCtrcCoordt_SrfcPnt[1]>1){
					MON[c  ].point.x=(int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]);//ThreeD_to_2D_Mapping
					MON[c++].point.y=(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]);
				}
			}
			m_pAnalysisView->m_3D1.EraseFrameBlack(&(m_pAnalysisView->m_DC[0]));
			//if(m_nDrawingMode!=NETWORK)CoordinateAxis(&showDC);
			showDC.SelectObject(&showPen);							
			Show3(&showDC);
			c=0;

			/////////////////  From the  2nd Observer  //////////////
 			ParametersForObserver2();
			for(l=0;l<g;l++){
				preRot_EgoCtrcCoordt_SrfcPnt[0]=(ObjCntrX+ObjCntrd_Coordnt_SrfcPoint[l][0]) -Observer2CntrX;   
				preRot_EgoCtrcCoordt_SrfcPnt[1]=(ObjCntrY+ObjCntrd_Coordnt_SrfcPoint[l][1]) -Observer2CntrY;   
				preRot_EgoCtrcCoordt_SrfcPnt[2]=(ObjCntrZ+ObjCntrd_Coordnt_SrfcPoint[l][2]) -Observer2CntrZ;
				ObserverRotation();
				if(EgoCtrcCoordt_SrfcPnt[1]>1){
					MON[c  ].point.x=(int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]);//ThreeD_to_2D_Mapping
					MON[c++].point.y=(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]);
				}
			}
			m_pAnalysisView->m_3D2.EraseFrameBlack(&(m_pAnalysisView->m_DC[0]));
			//if(m_nDrawingMode!=NETWORK)CoordinateAxis(&showDC);
			Show30(&showDC);
			////  Show the 1st Observer //
			preRot_EgoCtrcCoordt_SrfcPnt[0]=(ObserverCntrX) -Observer2CntrX;   
			preRot_EgoCtrcCoordt_SrfcPnt[1]=(ObserverCntrY) -Observer2CntrY;   
			preRot_EgoCtrcCoordt_SrfcPnt[2]=(ObserverCntrZ) -Observer2CntrZ;
			ObserverRotation();
			m_pAnalysisView->m_DC[0].SelectObject(&PenForActor);							
			m_pAnalysisView->m_3D2.Graph2D((int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]),(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]),3, &(m_pAnalysisView->m_DC[0]));
			m_pAnalysisView->m_3D2.Graph2D((int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]),(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]),9, &(m_pAnalysisView->m_DC[0]));
			// PIover2 has been added because I defined that y axis (_|_)is 0 degree.
			preRot_EgoCtrcCoordt_SrfcPnt[0]=(ObserverCntrX+5*cos(ObserverCntrRotZ+PIover2)) -Observer2CntrX;   
			preRot_EgoCtrcCoordt_SrfcPnt[1]=(ObserverCntrY+5*sin(ObserverCntrRotZ+PIover2)) -Observer2CntrY;   
			preRot_EgoCtrcCoordt_SrfcPnt[2]=(ObserverCntrZ) -Observer2CntrZ;
			ObserverRotation();
			m_pAnalysisView->m_3D2.Graph2D((int)(Scl*EgoCtrcCoordt_SrfcPnt[0]/EgoCtrcCoordt_SrfcPnt[1]),(int)(Scl*EgoCtrcCoordt_SrfcPnt[2]/EgoCtrcCoordt_SrfcPnt[1]),2, &(m_pAnalysisView->m_DC[0]));
			m_pAnalysisView->m_3D2.RefreshPane(&showDC,&(m_pAnalysisView->m_DC[0]));
			c=0; 
		}
		MY_PEEK_MESSAGE;
	}while(m_nShowMode==GRAPH_3D);
	return 1;
}

int CThreeD::ThreeD4(double* iE, int gValue, int InitLine, LPVOID objView, int disp, int ScreenX)
{
	return 1;
}

void CThreeD::Show3(CClientDC* objDC)
{
	int Idx;
	char chTemp[5];

	switch(m_nDrawingMode){
	case NETWORK:
		objDC->SetTextColor(RGB(255,0,0));
		for(Idx=0;Idx<m_nInitLine && Idx<c ;Idx++){
			sprintf(chTemp,"%d",Idx);
			objDC->TextOut(MON[Idx].point.x, MON[Idx].point.y, chTemp);
		}
		for(;Idx<c-1; Idx+=2){
			objDC->MoveTo(MON[Idx  ].point);
			objDC->LineTo(MON[Idx+1].point);
		}
		break;
	
	default:
		for(Idx=0; Idx<c ; Idx++){
			m_pAnalysisView->m_3D1.Graph2D(MON[Idx].point.x,MON[Idx].point.y,5, &(m_pAnalysisView->m_DC[0]));
			//objDC->SetPixel(MON[Idx].point.x, MON[Idx].point.y ,RGB(255,0,0));
		}
		m_pAnalysisView->m_3D1.RefreshPane(objDC,&(m_pAnalysisView->m_DC[0]));
	}
}

void CThreeD::Show30(CClientDC* objDC)
{
	int Idx;

	for(Idx=0; Idx<c ; Idx++){
		m_pAnalysisView->m_3D2.Graph2D(MON[Idx].point.x,MON[Idx].point.y,5, &(m_pAnalysisView->m_DC[0]));
	}
}
