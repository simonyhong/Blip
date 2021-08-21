#include "stdafx.h"
#include "stdio.h"
#include "ThreeD.h"
#include "Figure.h"
#include "AnalysisDoc.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include <afxwin.h>

#define ThreeDView 200
UINT Graph3D(LPVOID WinObjPtr) //A global Function
{
#define	sigv0	0.1       
#define sigv1	1.5       //original
#define sigv2	3.0       //original
#define sigy0	4.0
#define sigy1	8.0
#define sigy2	16.0
#define sigk	PIover4
#define mu		11.
	CAnalysisView* pAnalysisView=(CAnalysisView*) WinObjPtr;
	pAnalysisView->VirMemDC();
	pAnalysisView->ThD.m_pAnalysisView=pAnalysisView;
/////////////////////// It should be adjusted by the user  ////////////////
	#define MEM    10000			  //How many dots do you want to deal with?
	pAnalysisView->m_3D1.SetFig_Position_in_ClientView ( 5,   5, 226, 226); 
	pAnalysisView->m_3D1.SetLabelsXY("","              Egocentric System"); 
	pAnalysisView->m_3D1.SetMinMaxXY(-ThreeDView/2,ThreeDView/2,-ThreeDView/2,ThreeDView/2);
	pAnalysisView->m_3D1.SetFontSize(&pAnalysisView->m_DC[0]);
	pAnalysisView->m_3D1.Prepare4FigFrame();
	pAnalysisView->m_3D1.Frame_Graph2D(&(pAnalysisView->m_DC[0]),xFRAME); 
	pAnalysisView->m_3D2.SetFig_Position_in_ClientView ( 226,   6, 447, 226); 
	pAnalysisView->m_3D2.SetLabelsXY("","              "); 
	pAnalysisView->m_3D2.SetMinMaxXY(-ThreeDView/2,ThreeDView/2,-ThreeDView/2,ThreeDView/2);
	pAnalysisView->m_3D2.SetFontSize(&pAnalysisView->m_DC[0]);
	pAnalysisView->m_3D2.Prepare4FigFrame();
	pAnalysisView->m_3D2.Frame_Graph2D(&(pAnalysisView->m_DC[0]),xFRAME);
///////////////////////////////////////////////////////////////////////////
	int g=0, Scale=0;
	double K=0.;
	double sgv[3], sgh; sgv[0]=sigv0/2; sgv[1]=sigv1/2; sgv[2]=sigv2/2; sgh=sigv1/2;
	double maxZ=0, ori=0.,shift=.30;
	double sigySQ[3]; sigySQ[0]=sigy0*sigy0; sigySQ[1]=sigy1*sigy1; sigySQ[2]=sigy2*sigy2;
	double OneOverTwoPIsigySQ[3]; OneOverTwoPIsigySQ[0]=1./TWO_PI*sigy0*sigy0; OneOverTwoPIsigySQ[1]=1./TWO_PI*sigy1*sigy1; OneOverTwoPIsigySQ[2]=1./TWO_PI*sigy2*sigy2;
	double o=2.;
	
	double rho, phi,X,Y,Z,RadiousOfSphere;
	int ite;
	for( rho=0, ite=1; rho<80;ite++ ){
		RadiousOfSphere=(ite*ite);
		rho+=RadiousOfSphere;
		for( Z=0; Z<6; Z+=1){// 2.*atan(RadiousOfSphere/(2.*r))){
			for( phi=0; phi<TWO_PI; phi+= atan(RadiousOfSphere/(2.*rho)) ){
				X=rho*cos(phi+PIover2);Y=rho*sin(phi+PIover2);
				if(Z>6||Z<-0.1)continue;
				pAnalysisView->ThD.ObjCntrd_Coordnt_SrfcPoint[g][0]=X;                             //
				pAnalysisView->ThD.ObjCntrd_Coordnt_SrfcPoint[g][1]=Y;                            //
				pAnalysisView->ThD.ObjCntrd_Coordnt_SrfcPoint[g++][2]=Z;
				if(g==MEM)goto ExitLoop;
			}
		}
	}

ExitLoop:
	pAnalysisView->ThD.ObjCntrX=10 ;pAnalysisView->ThD.ObjCntrY=5; pAnalysisView->ThD.ObjCntrZ=0;
	pAnalysisView->ThD.ObserverCntrX=10 ;pAnalysisView->ThD.ObserverCntrY=0; pAnalysisView->ThD.ObserverCntrZ=3;
	pAnalysisView->ThD.Observer2CntrX=10 ;pAnalysisView->ThD.Observer2CntrY=0; pAnalysisView->ThD.Observer2CntrZ=70;
	pAnalysisView->ThD.ThreeD3(g, 0, pAnalysisView, 0);// This function is defined in "ThreeD.h".
	return 0;
}

void CAnalysisView::On3DGraph() 
{
	if(ThD.m_nShowMode!=GRAPH_3D)ThD.m_nShowMode=GRAPH_3D; 	else ThD.m_nShowMode=GRAPH_3D_STOP;
	AfxBeginThread( Graph3D, this);
}


void CAnalysisView::OnUpdate3DGraph(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(ThD.m_nShowMode==GRAPH_3D);
}