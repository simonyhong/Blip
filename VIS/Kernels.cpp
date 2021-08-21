#include "stdafx.h"
#include "VIS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ThreeD.h"
#include "VIS_Doc.h"
#include "Figures.h"
#include "SharedConstants.h"
#include "VIS_View.h"
#include "SinTable.h"



//It calculates the constant of the Gaussian kernel.
double CVISView::Kernel_2D(double KerSum, double Sigma, double RangeX)
{
	double sigSq=Sigma*Sigma, rangeXSq=RangeX*RangeX, SqDist, Sum; 

	int p,q, IniP,FinP, IniQ,FinQ; 
	IniP=0-RangeX; FinP=0+RangeX;	IniQ=0-RangeX; FinQ=0+RangeX;
	Sum=0; 
	for(p=IniP;p<=FinP;p++){
		for(q=IniQ;q<=FinQ;q++){
			if((SqDist=SQ_DISTANCE_AB(0,0, p,q))>rangeXSq)continue;
			Sum += exp(-SqDist/sigSq);
	}	}
	return (KerSum/Sum);  //KerSum=Kernel*SIG(exp[.])
}

//It calculates the constant of the Gaussian kernel.
double CVISView::Kernel_1D(double KerSum, double Sigma, double RangeX)
{
	double sigSq=Sigma*Sigma, rangeXSq=RangeX*RangeX, SqDist, Sum; 

	int p, IniP,FinP; 
	IniP=0-RangeX; FinP=0+RangeX;
	Sum=0; 
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(0,p))>rangeXSq)continue;
		Sum += exp(-SqDist/sigSq);
	}	
	return (KerSum/Sum);  //KerSum=KerAmp*SIG(exp[.])
}

//It generates the Gaussian kernel and its field.
int CVISView::MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX)
{
	double KerAmp=Kernel_1D(KerSum, Sigma, RangeX);	
	double SqDist,sigmaSq=Sigma*Sigma,rangeXSq=RangeX*RangeX;
	int i=0,x, NoX=0;
	int IniX=i-RangeX, FinX=i+RangeX;
	for(x=IniX;x<=FinX;x++){
		if((SqDist=SQ_DISTANCE_AB_1D(i,x))>(float)rangeXSq)continue;
		KrVal[NoX]= KerAmp*exp(-SqDist/sigmaSq);
		KerFieldX[NoX]=x;  NoX++;
		if(NoX>sizeOfKer){AfxMessageBox("NoX>=sizeOfKer", MB_OK); return NoX;}
	}
	
	CFigure *pBrd=&m_Board[0];
	pBrd->Graph2D(KerFieldX[0],KrVal[0],9,&m_DC[0]);
	for(i=0; i<NoX; i++){
		pBrd->Graph2D(KerFieldX[i],KrVal[i],2,&m_DC[0]);
	}

	return NoX;
}