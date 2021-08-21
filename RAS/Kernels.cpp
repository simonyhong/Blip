#include "stdafx.h"
#include "RAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "RAS_Doc.h"
#include "Figure.h"
#include "SharedConstants.h"
#include "RAS_View.h"
#include "SinTable.h"
#include "Dialog_Box.h"


//It calculates the constant of the Gaussian kernel.
double CRASView::Kernel_2D(double KerSum, double Sigma, double RangeX)
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
	return (KerSum/Sum);  //KerSum=KernelAmplitude*SIG(exp[.])
}

//It calculates the constant of the Gaussian kernel.
double CRASView::Kernel_1D(double KerSum, double Sigma, double RangeX)
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
int CRASView::MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX)
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
	
	return NoX;
}

void CRASView::CreateKer(float sig, float RangeX)
{	
	#define KrCntr 0
	int i, p, IniP,FinP,pId;
	float RangeXSq, sigSq, SqDist;
	sigSq=sig*sig; RangeXSq=RangeX*RangeX;
	//////////////////////////////////
	m_GaussianAmp=Kernel_1D(1, sig, RangeX);	//KrSum==1 means no amplification
	NumP=0;
	IniP=KrCntr-RangeX; FinP=KrCntr+RangeX;
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(KrCntr,p))>(float)RangeXSq)continue;
		KrVal[NumP]=m_GaussianAmp*exp(-SqDist/sigSq);
		P[NumP]=p; NumP++;
	}
}


