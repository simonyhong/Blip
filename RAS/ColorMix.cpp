
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


void CRASView::CosineColorMix(COLORREF RGB_array[],int arraySize) 
{		
	double Ker;
	double RedKer[1000],GreenKer[1000],BlueKer[1000],shift=0.08;
	double startRED=arraySize*0.75, endRED=arraySize*1;
	double startGRN=arraySize*0.25, endGRN=arraySize*0.75;
	double startBLU=arraySize*0.,  endBLU=arraySize*0.25;
	for(int i=0; i<arraySize; i++){
		//########  RED
		if(i<0.5*arraySize)Ker=0;
		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=(-cos(((float)(i-0.5*arraySize)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=1;
		RedKer[i]=255*Ker;
		//########  GREEN
		if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=(-cos(((float)i/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=(cos(((float)(i-endGRN)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		GreenKer[i]=255*Ker;
		//########  BLUE
		if(i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*arraySize)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=(cos(((float)(i-endBLU)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else 
			Ker=0;
		BlueKer[i]=255*Ker;
		RGB_array[i]=RGB(RedKer[i], GreenKer[i], BlueKer[i]);
	}		
}


void CRASView::CosineColorMix_adjusted(COLORREF RGB_array[],int arraySize)
{
	double Ker;
	double RedKer[1000],GreenKer[1000],BlueKer[1000],shift=0.08;
	double startRED=arraySize*0.75, endRED=arraySize*1;
	double startGRN=arraySize*0.25, endGRN=arraySize*0.75;
	double startBLU=arraySize*0.,  endBLU=arraySize*0.25;
	for(int i=0; i<arraySize; i++){
		//########  RED
		if(i<0.5*arraySize)Ker=0;
		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=(-cos(((float)(i-0.5*arraySize)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=1;
		RedKer[i]=255*Ker;
		//########  GREEN
		if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=(-cos(((float)i/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=(cos(((float)(i-endGRN)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		GreenKer[i]=255*Ker;
		//########  BLUE
		if(i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*arraySize)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=(cos(((float)(i-endBLU)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else 
			Ker=0;
		BlueKer[i]=255*Ker;
		RGB_array[i]=RGB(RedKer[i], GreenKer[i], BlueKer[i]);
	}		
	/////////////////////
	int I[1000],Ideal[1000];
	float lowerBnd=0, higherBnd=76, adjFctr[1000];
	float howMuchAdjust=0.3; //How much percentage to adjsut?
	for(int i=0; i<arraySize; i++){
		Ideal[i]=lowerBnd+i*(higherBnd/(float)arraySize);
		I[i]=0.30*RedKer[i]+0.59*GreenKer[i]+0.11*BlueKer[i];
		adjFctr[i]=Ideal[i]/(float)I[i];
		RedKer[i]*=adjFctr[i]*howMuchAdjust+(1-howMuchAdjust); if(RedKer[i]<0)RedKer[i]=0; else if(RedKer[i]>255)RedKer[i]=255;
		GreenKer[i]*=adjFctr[i]*howMuchAdjust+(1-howMuchAdjust); if(GreenKer[i]<0)GreenKer[i]=0; else if(GreenKer[i]>255)GreenKer[i]=255;
		BlueKer[i]*=adjFctr[i]*howMuchAdjust+(1-howMuchAdjust); if(BlueKer[i]<0)BlueKer[i]=0; else if(BlueKer[i]>255)BlueKer[i]=255;
	}
}


void CRASView::MyOriginalColorMix(COLORREF RGB_array[],int arraySize) 
{	
	int R,G,B;
	double SqDist;
	COLORREF RGB;
	for(int i=0; i<arraySize; i++){
		//########  RED
		R=(255/(1+exp(-(((double)i)/arraySize-0.6)/0.03)));
		//########  GREEN
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.55);
		G=255*exp(-SqDist/(arraySize*10.));
		//########  BLUE
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.30);
		B=255*exp(-SqDist/(arraySize*6.));
		RGB_array[i]=RGB(R, G, B);
	}
}


//This one puts a pseudo-color legend on the right side of a figure.
void CRASView::ColorLegend(CDC *pDC, CFigure *pFig) 
{	
#define WidthIn100thOfTotalXSpan 0.03 //3% of the total span of X.
#define WidthIn100thOfTotalYSpan 1 //100% of the total span of Y.
#define LeftOrRightOfX 1 //Put the legend on which side of X? (0:left, 1:right)

	int i,minScreenX,y1,x2,y2,RecHight,RecWidth,rgb,initX,dummyY;
	float initXfloat, dummyFloat;
	pFig->FromDataToScreenXY((double)pFig->m_dMinX,(double)(pFig->m_dMinY),minScreenX,y1); //It maps a relative point (i,j) of the graph to a point on the screen.	
	pFig->FromDataToScreenXY((double)(pFig->m_dMinX+pFig->m_dSpanX*WidthIn100thOfTotalXSpan),(double)(pFig->m_dMinY+WidthIn100thOfTotalYSpan*pFig->m_dSpanY/MaxRGB100),x2,y2);  //1/MaxRGB100 of the span of (WidthIn100thOfTotalYSpan*pFig->m_dSpanY)
	RecHight=(int)(abs(y2-y1)+1);//1 is added to prevent occasional chasm.
	RecWidth=(int)(abs(x2-minScreenX));
	if(LeftOrRightOfX==0){//left
		initX=-RecWidth+minScreenX;
	}
	else{
		pFig->FromDataToScreenXY((double)pFig->m_dMaxX,(double)(pFig->m_dMinY),initX,dummyY); //It maps a relative point (i,j) of the graph to a point on the screen.	
	}
	FromScrnToDataCoordinate(initX,0,initXfloat,dummyFloat,pFig);
	for( i=0; i<MaxRGB100; i++){   //Squares
		rgb=(int)(MaxRGB100*((double)(i+1)/(double)MaxRGB100)); if(rgb>MaxRGB100-1)rgb=MaxRGB100-1; if(rgb<0)rgb=0;
		pFig->FillSolidRect((double)initXfloat,((double)(i+1)/(double)MaxRGB100)*WidthIn100thOfTotalYSpan*pFig->m_dSpanY+pFig->m_dMinY,RecWidth, RecHight,m_RGB100[rgb],pDC);//m_PrevTx10-1 (-1 has been added to compensate the +error in RecWidth)
	}
	pDC->FillSolidRect(0,0,1, 1,RGB(255,255,255));//Is there any better solution?
}

//This maps a screen point (x,y) to a data point (i,j) on the selected graph.
void CRASView::FromScrnToDataCoordinate(int x, int y, float &DataX, float &DataY, CFigure *pFig) 
{
	int paneTopX, paneTopY,paneBottomX, paneBottomY;
	pFig->FromDataToScreenXY(pFig->m_dMinX,pFig->m_dMaxY,paneTopX,paneTopY);
	pFig->FromDataToScreenXY(pFig->m_dMaxX,pFig->m_dMinY,paneBottomX,paneBottomY);
	DataX=(pFig->m_dMaxX-pFig->m_dMinX)*(((double)(x   -paneTopX))/(paneBottomX-paneTopX))+pFig->m_dMinX;	
	DataY=(pFig->m_dMaxY-pFig->m_dMinY)*(((double)(paneBottomY-y))/(paneBottomY-paneTopY))+pFig->m_dMinY;
}