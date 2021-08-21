
#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"
#include "MainFrm.h"

//This function assumes there the spike time and the SDF array have the same dimention.
void CAnalysisView::InitSDF(int SizeOfSDF)
{
	//Make the Gaussian Kernel
	m_halfGaussKerMs=2*m_fSigma;//Half the size of the Gaussian
	m_fullGaussianMs=2*m_halfGaussKerMs;
	CreateKer(m_fSigma, m_halfGaussKerMs);
	
	//Show the Kernel
	//m_DC[0].SelectObject(&m_RED); 
	//pFig->myMoveTo(0, m_KrVal[0],&m_DC[0]);
	//for(i=1;i<=m_fullGaussianMs;i++){
	//	pFig->myLineTo(i, m_KrVal[i],&m_DC[0]);
	//}

	//Set the size of SDF
	if(SizeOfSDF<m_fullGaussianMs+1)SizeOfSDF=m_fullGaussianMs+1;
	if(SizeOfSDF>1000000){AfxMessageBox("The size of SDF is more than 1000sec.\nAdjusted to 1000sec."); SizeOfSDF=1000000;}
	m_SDF.SetSize(SizeOfSDF);
	m_SDFArraySize=SizeOfSDF;

	//Calculate the Multiplication factors
	int i, initI, p;
	m_Compensation.SetSize(m_halfGaussKerMs+10);
	for(i=0;i<=m_halfGaussKerMs;i++)m_Compensation[i]=0;
	for(i=0;i<=m_halfGaussKerMs+m_halfGaussKerMs+1;i++){
		initI=i-m_halfGaussKerMs;
		for(p=0;p<=m_fullGaussianMs;p++){
			if(p+initI>=0 && p+initI<=m_halfGaussKerMs){
				m_Compensation[p+initI]+=m_KrVal[p];
			}
		}
	}
	for(i=0;i<=m_halfGaussKerMs;i++){
		m_Compensation[i]=GAUSS_SUM/m_Compensation[i];
	}
}

//This function assumes there the spike time and the SDF array have the same dimention.
void CAnalysisView::ComputeSDF(int SpkTime[], int SizeOfSpkArray, int TimeToSubtract)
{
	int i, p, initI, SpkTm;
	//Do the SDF calculation
	if(SpkTime[SizeOfSpkArray-1]-m_initTime+m_fullGaussianMs < m_SDFArraySize)m_SDFArraySizeUsed=SpkTime[SizeOfSpkArray-1]-m_initTime+m_fullGaussianMs;
	else m_SDFArraySizeUsed=m_SDFArraySize;
	//Initialize
	for(i=0;i<m_SDFArraySizeUsed;i++)m_SDF[i]=0;

	for(i=0;i<SizeOfSpkArray;i++){
		SpkTm=SpkTime[i]-TimeToSubtract;
		if(SpkTm>=m_SDFArraySize||SpkTm<0)continue;
		initI=SpkTm-m_halfGaussKerMs; //Map the SpkTime to SDF index (in ms)
		for(p=0;p<=m_fullGaussianMs;p++){
			if(p+initI>=0 && p+initI<m_SDFArraySize)m_SDF[p+initI]+=m_KrVal[p];
		}
	}
	//Compensate the edges for their incompleteness
	for(i=0;i<m_halfGaussKerMs;i++){ //i=0 should not be here because it is not part of the missing tail of the Gaussian kernel.
		m_SDF[i]*=m_Compensation[i];
	}
	for(i=m_SDFArraySize-1-m_halfGaussKerMs, p=m_halfGaussKerMs;i<m_SDFArraySize;i++,p--){
		m_SDF[i]*=m_Compensation[p];
	}
}


//Size of Kernel=2*RangeX+1  (+1 is added to include 0: -RangeX ~ 0 ~ +RangeX)
void CAnalysisView::CreateKer(float sig, float RangeX)
{	
	#define KrCntr 0
	int p, IniP,FinP;
	float RangeXSq, sigSq, SqDist;
	sigSq=sig*sig; RangeXSq=RangeX*RangeX;
	//////////////////////////////////
	m_GaussianAmp=Kernel_1D(GAUSS_SUM, sig, RangeX);	//KrSum==1 means no amplification
	int NumP=0;
	IniP=KrCntr-RangeX; FinP=KrCntr+RangeX;
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(KrCntr,p))>(float)RangeXSq)continue;
		m_KrVal[NumP]=m_GaussianAmp*exp(-SqDist/sigSq);
		m_P[NumP]=p; NumP++;
	}
}
