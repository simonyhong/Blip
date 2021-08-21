#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "AnalysisDoc.h"
#include "Graph2D.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include "DialogueBox.h"


void CAnalysisView::DispVariables(double tx10) 
{		int i,j;

	CClientDC objDC(this);
	//  ACG  //
	objDC.SelectObject(m_RED);m_memDC.SelectObject(m_RED);
	m_TwoD1.Abs_Graph2D  (m_PrevTx10,m_dprvACG,tx10, m_dACG,2,&objDC  );
	m_TwoD1.Abs_Graph2D  (m_PrevTx10,m_dprvACG,tx10, m_dACG,2,&m_memDC); m_dprvACG=m_dACG;
	//  I  //
	for(i=0; i<MaxI; i++){
		j=i;
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		m_TwoD11.Abs_Graph2D  (m_PrevTx10,m_dprvI[i]+0.1*i,tx10, m_dI[i]+0.1*i,2,&objDC  );
		m_TwoD11.Abs_Graph2D  (m_PrevTx10,m_dprvI[i]+0.1*i,tx10, m_dI[i]+0.1*i,2,&m_memDC); m_dprvI[i]=m_dI[i];
		m_TwoD11.Abs_Graph2D  (m_PrevTx10,m_dprvItr[i]+0.1*i,tx10, m_dItr[i]+0.1*i,2,&objDC  );
		m_TwoD11.Abs_Graph2D  (m_PrevTx10,m_dprvItr[i]+0.1*i,tx10, m_dItr[i]+0.1*i,2,&m_memDC); m_dprvItr[i]=m_dItr[i];
		//  I->CDid  (weight) //
		m_TwoD2.MySetPixelRGB (tx10,m_dIWidR[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&objDC); 
		m_TwoD2.MySetPixelRGB (tx10,m_dIWidR[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&m_memDC); 
		//  I->CDdr  (weight) //
		m_TwoD7.MySetPixelRGB (tx10,m_dIWdr[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&objDC); 
		m_TwoD7.MySetPixelRGB (tx10,m_dIWdr[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&m_memDC); 
	}
	//  PM  //
	for(i=0; i<MaxPM; i++){
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		m_TwoD6.Abs_Graph2D  (m_PrevTx10,m_dprvPM[i]+0.1*i,tx10, m_dPM[i]+0.1*i,2,&objDC  );
		m_TwoD6.Abs_Graph2D  (m_PrevTx10,m_dprvPM[i]+0.1*i,tx10, m_dPM[i]+0.1*i,2,&m_memDC); m_dprvPM[i]=m_dPM[i];
		//m_TwoD7.MySetPixelRGB(tx10, m_dInhSumPM[i]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&objDC  );
	}	
	for(i=0; i<MaxCD; i++){
		j=i;
		//  CX2CDid  (weight)//
		//m_TwoD2.MySetPixelRGB (tx10,m_dWid[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&objDC); 
		//m_TwoD2.MySetPixelRGB (tx10,m_dWid[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&m_memDC); 
		////  CX2CDdr //
		//m_TwoD7.MySetPixelRGB (tx10,m_dWdr[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&objDC); 
		//m_TwoD7.MySetPixelRGB (tx10,m_dWdr[i][j]+0.1*i,&m_RGB[(MaxRGB-1)-i*(MaxRGB/MaxI)],&m_memDC); 

		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]); 
		//  CDid  //
		m_TwoD3.Abs_Graph2D (m_PrevTx10,m_dprvCDid[i]+0.1*i,tx10,m_dCDid[i]+0.1*i,2,&objDC); 
		m_TwoD3.Abs_Graph2D (m_PrevTx10,m_dprvCDid[i]+0.1*i,tx10,m_dCDid[i]+0.1*i,2,&m_memDC); m_dprvCDid[i]=m_dCDid[i];
		m_TwoD3.Abs_Graph2D (m_PrevTx10,m_dprvCDidTr[i]+0.1*i,tx10,m_dCDidTr[i]+0.1*i,2,&objDC); 
		m_TwoD3.Abs_Graph2D (m_PrevTx10,m_dprvCDidTr[i]+0.1*i,tx10,m_dCDidTr[i]+0.1*i,2,&m_memDC); m_dprvCDidTr[i]=m_dCDidTr[i];
		//m_TwoD2.MySetPixelPINK (tx10,m_dAveCDid[i],&objDC); m_TwoD2.MySetPixelPINK (tx10,m_dAveCDid[i],&m_memDC);
		//  CDdr //
		m_TwoD8.Abs_Graph2D (m_PrevTx10,m_dprvCDdr[i]+0.1*i,tx10,m_dCDdr[i]+0.1*i,2,&objDC); 
		m_TwoD8.Abs_Graph2D (m_PrevTx10,m_dprvCDdr[i]+0.1*i,tx10,m_dCDdr[i]+0.1*i,2,&m_memDC); m_dprvCDdr[i]=m_dCDdr[i];
		m_TwoD8.Abs_Graph2D (m_PrevTx10,m_dprvCDdrTr[i]+0.1*i,tx10,m_dCDdrTr[i]+0.1*i,2,&objDC); 
		m_TwoD8.Abs_Graph2D (m_PrevTx10,m_dprvCDdrTr[i]+0.1*i,tx10,m_dCDdrTr[i]+0.1*i,2,&m_memDC); m_dprvCDdrTr[i]=m_dCDdrTr[i];
		//m_TwoD7.MySetPixelPINK (tx10,m_dAveCDdr[i],&objDC); m_TwoD7.MySetPixelPINK (tx10,m_dAveCDdr[i],&m_memDC);
	}
	//  GPe  //
	for(i=0; i<MaxGP; i++){
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		m_TwoD4.Abs_Graph2D  (m_PrevTx10,m_dprvGPe[i]+0.1*i,tx10, m_dGPe[i]+0.1*i,2,&objDC  );
		m_TwoD4.Abs_Graph2D  (m_PrevTx10,m_dprvGPe[i]+0.1*i,tx10, m_dGPe[i]+0.1*i,2,&m_memDC); m_dprvGPe[i]=m_dGPe[i];
		//m_TwoD4.MySetPixelPINK (tx10,m_dAveGPe[i],&objDC); m_TwoD4.MySetPixelPINK (tx10,m_dAveGPe[i],&m_memDC);
		m_TwoD4.MySetPixelPINK (tx10,eCDid(m_dDlydCDid[i][m_nCD2GP_DlyF]),&objDC); m_TwoD4.MySetPixelPINK (tx10,eCDid(m_dDlydCDid[i][m_nCD2GP_DlyF]),&m_memDC);
	}
	//  STN  //
	for(i=0; i<1; i++){
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		m_TwoD5.Abs_Graph2D  (m_PrevTx10,m_dprvSTN+0.1*i,tx10, m_dSTN+0.1*i,2,&objDC  );
		m_TwoD5.Abs_Graph2D  (m_PrevTx10,m_dprvSTN+0.1*i,tx10, m_dSTN+0.1*i,2,&m_memDC); m_dprvSTN=m_dSTN;
		//m_TwoD5.MySetPixelPINK (tx10,m_dAveSTN,&objDC); m_TwoD5.MySetPixelPINK (tx10,m_dAveSTN,&m_memDC);
		m_TwoD5.MySetPixelPINK(tx10,0.5*m_dSumPM2STN,&objDC); m_TwoD5.MySetPixelPINK (tx10,0.5*m_dSumPM2STN,&m_memDC);
		m_TwoD5.MySetPixelBLACK (tx10,0.5*m_dGPe2STN,&objDC); m_TwoD5.MySetPixelBLACK (tx10,0.5*m_dGPe2STN,&m_memDC);
	}
	//  GPi  //
	for(i=0; i<MaxGP; i++){
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		m_TwoD9.Abs_Graph2D  (m_PrevTx10,m_dprvGPiFq[i]+0.1*i,tx10, m_dGPiFq[i]+0.1*i,2,&objDC  );
		m_TwoD9.Abs_Graph2D  (m_PrevTx10,m_dprvGPiFq[i]+0.1*i,tx10, m_dGPiFq[i]+0.1*i,2,&m_memDC); m_dprvGPiFq[i]=m_dGPiFq[i];
		m_TwoD10.Abs_Graph2D (m_PrevTx10,m_dprveCDdr[i]+0.1*i,tx10,eCDdr(m_dDlydCDdr[i][m_nCD2GP_DlyF])+0.1*i,2,&objDC);m_dprveCDdr[i]=eCDdr(m_dDlydCDdr[i][m_nCD2GP_DlyF]);
	}
	//  SNc  //
	for(i=0; i<1; i++){
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		objDC.SelectObject(m_RED);m_memDC.SelectObject(m_RED);
		m_TwoD15.Abs_Graph2D  (m_PrevTx10,m_dprvSNc+0.1*i,tx10, m_dSNc+0.1*i,2,&objDC  );
		m_TwoD15.Abs_Graph2D  (m_PrevTx10,m_dprvSNc+0.1*i,tx10, m_dSNc+0.1*i,2,&m_memDC); m_dprvSNc=m_dSNc;
		//m_TwoD15.Abs_Graph2D  (m_PrevTx10,m_dprvAveDA,tx10, m_dAveDA,2,&objDC  );
		//m_TwoD15.Abs_Graph2D  (m_PrevTx10,m_dprvAveDA,tx10, m_dAveDA,2,&m_memDC); m_dprvAveDA=m_dAveDA;
	}
	//  TH  //
	for(i=0; i<MaxTH; i++){
		objDC.SelectObject(m_rgbPEN1[i]);m_memDC.SelectObject(m_rgbPEN1[i]);
		m_TwoD13.Abs_Graph2D  (m_PrevTx10,m_dprvTH[i]+0.1*i,tx10, m_dTHfq[i]+0.1*i,2,&objDC  );
		m_TwoD13.Abs_Graph2D  (m_PrevTx10,m_dprvTH[i]+0.1*i,tx10, m_dTHfq[i]+0.1*i,2,&m_memDC); m_dprvTH[i]=m_dTHfq[i];
	}
}

void CAnalysisView::DrawGraphs() 
{	
	CClientDC objDC(this);
	m_TwoD1. Set2D( 5,  5,  405, 155 ,"","1 : ACG"); 
	m_TwoD2. Set2D( 5,  160,405, 310 ,"","2 : CX2CDid"); 
	m_TwoD3. Set2D( 5,  315,405, 465 ,"","3 : CDid/ "); 
	m_TwoD4. Set2D( 5,  470,405, 620 ,"","4 : GPe: eCDid(m_dDlydCDid[i][DlyF])(pink)"); 
	m_TwoD5. Set2D( 5,  625,405, 775 ,"","5 : STN: 0.5*PM2STN(pink), 0.5*GPe2STN(black)"); 
	m_TwoD6. Set2D( 410,5,  810, 155 ,"","6 : PM"); 
	m_TwoD7. Set2D( 410,160,810, 310 ,"","7 : CX2CDdr"); 
	m_TwoD8. Set2D( 410,315,810, 465 ,"","8 : CDdr/ "); 
	m_TwoD9. Set2D( 410,470,810, 620 ,"","9 : GPiFq"); 
	m_TwoD10.Set2D( 410,625,810, 775 ,"","10 : eCDdr(m_dDlydCDdr[i][DlyF])"); 
	m_TwoD11.Set2D( 815,5,  1215,155 ,"","11 : I"); 
	m_TwoD12.Set2D( 815,160,1215,310 ,"","12 : "); 
	m_TwoD13.Set2D( 815,315,1215,465 ,"","13 : TH"); 
	m_TwoD14.Set2D( 815,470,1215,620 ,"","14 : "); 
	m_TwoD15.Set2D( 815,625,1215,775 ,"","15 : SNc"); 
	//m_TwoD16.Set2D(1220,  5,1620,155 ,"","16 : pDCNINN->V(R)/IcfSUM(Br)"); 
	m_TwoD1. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD2. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME);
	m_TwoD3. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD4. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME);
	m_TwoD5. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD6. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD7. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD8. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD9. Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD10.Frame_Graph2D(0,SimDurationx10,0,3  ,&(m_memDC),1,oFRAME); 
	m_TwoD11.Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
//	m_TwoD12.Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD13.Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
//	m_TwoD14.Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
	m_TwoD15.Frame_Graph2D(0,SimDurationx10,	 0,1  ,&(m_memDC),1,oFRAME); 
	//m_TwoD16.Frame_Graph2D(0,SimDurationx10,0,1  ,&(m_memDC),1,oFRAME); 
}

