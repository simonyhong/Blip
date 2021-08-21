
#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"

void CAnalysisView::LinearColorMix(COLORREF RGB_array[],int arraySize) 
{		
	double  SqDist,Ker, slope=1/0.25, redB=-slope*0.5,grnB=0,bluB=0, redBB=-slope*0.5,grnBB=slope,bluBB=slope*.50;
	double RedKer,GrnKer,BluKer,shift=0.08;
	double startRED=arraySize*0.75, endRED=arraySize*1.0   ,sigSqRED=300 ;
	double startGRN=arraySize*0.25, endGRN=arraySize*0.75,  sigSqGRN=300 ;
	double startBLU=arraySize*0.0, endBLU=arraySize*0.25, sigSqBLU=300 ;
	//########  RED
	for(int i=0; i<arraySize; i++){
		m_DC[0].SelectObject(m_RED);
		if(i<0.5*arraySize)Ker=0;
		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope*(i/(float)arraySize)+redB;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=1;
		RedKer=255*Ker;
		//########  GREEN
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=0;
		else if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope*(i/(float)arraySize)+grnB;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope*(i/(float)arraySize)+grnBB;
		GrnKer=255*Ker;
		//########  BLUE
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope*(i/(float)arraySize)+bluB;
		else if(i>=startBLU && i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*arraySize)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope*(i/(float)arraySize)+bluBB;
		else 
			Ker=0;
		BluKer=255*Ker;
		RGB_array[i]=RGB(RedKer, GrnKer, BluKer);
	}		
}

void CAnalysisView::LinearColorMix_adjusted(COLORREF RGB_array[],int arraySize) 
{
	int i;
	double  Ker, slope=1/0.25, redB=-slope*0.5,grnB=0,bluB=0, redBB=-slope*0.5,grnBB=slope,bluBB=slope*.50;
	double RedKer[1000],GrnKer[1000],BluKer[1000];
	COLORREF RGB;
	double startRED=arraySize*0.75, endRED=arraySize*1.0   ,sigSqRED=300 ;
	double startGRN=arraySize*0.25, endGRN=arraySize*0.75,  sigSqGRN=300 ;
	double startBLU=arraySize*0.0, endBLU=arraySize*0.25, sigSqBLU=300 ;
	//########  RED
	for(i=0; i<arraySize; i++){
		if(i<0.5*arraySize)Ker=0;
		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope*(i/(float)arraySize)+redB;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=1;
		RedKer[i]=255*Ker;
		//########  GREEN
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=0;
		else if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope*(i/(float)arraySize)+grnB;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope*(i/(float)arraySize)+grnBB;
		GrnKer[i]=255*Ker;
		//########  BLUE
		if(i<startBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=slope*(i/(float)arraySize)+bluB;
		else if(i>=startBLU && i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*arraySize)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=-slope*(i/(float)arraySize)+bluBB;
		else 
			Ker=0;
		BluKer[i]=255*Ker;
	}		
	
	/////////////////////
	float I[1000];
	float Ideal[1000];
	float lowerBnd=28, higherBnd=56, adjFctr[1000];
	for(i=0; i<arraySize; i++){
		Ideal[i]=lowerBnd+i*(higherBnd/(float)arraySize);
	}
	for(i=0; i<arraySize; i++){
		I[i]=0.30*RedKer[i]+0.59*GrnKer[i]+0.11*BluKer[i];
		adjFctr[i]=Divide(Ideal[i],(float)I[i]);
	}		
	/////////////////////
	//Adjust
	/////////////////////
#define BASEADJUST 0.3
	float howMuchAdjust; //How much percentage to adjsut?
	float prevBlue,a,K;
	for(i=arraySize-1; i>=0; i--){
		howMuchAdjust=BASEADJUST+BASEADJUST*((arraySize-i)/(float)arraySize);
		//howMuchAdjust=BASEADJUST;
		RedKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(RedKer[i]<0)RedKer[i]=0; else if(RedKer[i]>255)RedKer[i]=255;
		GrnKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(GrnKer[i]<0)GrnKer[i]=0; else if(GrnKer[i]>255)GrnKer[i]=255;
		BluKer[i]*=pow((double)adjFctr[i],2)*howMuchAdjust+(1-howMuchAdjust); if(BluKer[i]<0)BluKer[i]=0; else if(BluKer[i]>255)BluKer[i]=255;
		
		if(i==endBLU){prevBlue=BluKer[i]; a=prevBlue/(7.*i);K=6*prevBlue/(float)(7.);}
		if(i<endBLU){
			BluKer[i]=prevBlue;
		}
		RGB_array[i]=RGB(RedKer[i], GrnKer[i], BluKer[i]);
	}		
}


void CAnalysisView::CosineColorMix(COLORREF RGB_array[],int arraySize) 
{		
	double Ker;
	double RedKer,GreenKer,BlueKer,shift=0.08;
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
		RedKer=255*Ker;
		//########  GREEN
		if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=(-cos(((float)i/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
			Ker=1;
		else //When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=(cos(((float)(i-endGRN)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		GreenKer=255*Ker;
		//########  BLUE
		if(i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
			Ker=1;
		else if(i<0.5*arraySize)//When the i is on the right side of the plateau (maximum) area of a kernel
			Ker=(cos(((float)(i-endBLU)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
		else 
			Ker=0;
		BlueKer=255*Ker;
		RGB_array[i]=RGB(RedKer, GreenKer, BlueKer);
	}		
}
void CAnalysisView::CosineColorMix_AxB(COLORREF RGB_array[],int arraySize, int C, int B) 
{		
	if(C*B>=arraySize){AfxMessageBox("C*B>=arraySize"); return;}
	int i,j;
	int OneSixth=C*1.0/6.0, TwoSixths=C*2.0/6.0, ThreeSixths=C*3.0/6.0, FourSixths=C*4.0/6.0, FiveSixths=C*5.0/6.0;
	int OneThird=TwoSixths, Half=ThreeSixths; 
	double Ker;
	CArray<double, double&> RedKer,GreenKer,BlueKer;
	RedKer.SetSize(C+1); GreenKer.SetSize(C+1); BlueKer.SetSize(C+1);
#define POS_COSINE(Border) ((1 +cos( TWO_PI*(i-Border)/((double)OneThird))  )*0.5)
#define NEG_COSINE(Border) ((1 -cos( TWO_PI*(i-Border)/((double)OneThird))  )*0.5)
	//########  RED
	for(i=0; i<C; i++){
		if(i<OneSixth)Ker=POS_COSINE(0);
		else if(i<Half)Ker=0;
		else if(i<FourSixths)Ker=NEG_COSINE(Half);
		else Ker=1;
		RedKer[i]=255*Ker;
	}		
	//########  GREEN
	for(i=0; i<C; i++){
		if(i<OneSixth)Ker=0;
		else if(i<TwoSixths)Ker=NEG_COSINE(OneSixth);
		else if(i<FourSixths)Ker=1;
		else if(i<FiveSixths)Ker=POS_COSINE(FourSixths);
		else Ker=0;
		GreenKer[i]=255*Ker;
	}		
	//########  BLUE
	for(i=0; i<C; i++){
		if(i<TwoSixths)Ker=1;
		else if(i<Half)Ker=POS_COSINE(TwoSixths);
		else if(i<FiveSixths)Ker=0;
		else Ker=NEG_COSINE(FiveSixths);
		BlueKer[i]=255*Ker;
	}		

	double brightness;
	for(i=0; i<B; i++){
		brightness=(B-i)/(double)B;
		for(j=0; j<C; j++){
			RGB_array[C*i+j]=RGB(RedKer[j]*brightness, GreenKer[j]*brightness, BlueKer[j]*brightness);
		}
	}	
	RedKer.RemoveAll(); GreenKer.RemoveAll(); BlueKer.RemoveAll();
}

void CAnalysisView::MyOriginalColorMix(COLORREF RGB_array[],int arraySize) 
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
void CAnalysisView::MyOriginalColorMix_forGL(double rGL[],double gGL[],double bGL[], int arraySize)
{	
	double SqDist;
	for(int i=0; i<arraySize; i++){
		//########  RED
		rGL[i]=(1./(1+exp(-(((double)i)/arraySize-0.6)/0.03)));
		//########  GREEN
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.55);
		gGL[i]=exp(-SqDist/(arraySize*10.));
		//########  BLUE
		SqDist=SQ_DISTANCE_AB_1D(i,arraySize*0.30);
		bGL[i]=exp(-SqDist/(arraySize*6.));
	}
}


//This one puts a pseudo-color legend on the right side of a figure.
void CAnalysisView::ColorLegend(CDC *pDC, CFigure *pFig) 
{	
#define WidthIn100thOfTotalXSpan 0.03 //3% of the total span of X.
#define WidthIn100thOfTotalYSpan 1 //100% of the total span of Y.
#define LeftOrRightOfX 1 //Put the legend on which side of X? (0:left, 1:right)

	int i,minScreenX,y1,x2,y2,RecHight,RecWidth,rgb,initX,dummyY;
	double initXdouble, dummyDouble;
	pFig->FromDataToScreenXY((double)pFig->m_dMinX,(double)(pFig->m_dMinY),minScreenX,y1); //It maps a relative point (i,j) of the graph to a point on the screen.	
	pFig->FromDataToScreenXY((double)(pFig->m_dMinX+pFig->m_dSpanX*WidthIn100thOfTotalXSpan),(double)(pFig->m_dMinY+WidthIn100thOfTotalYSpan*pFig->m_dSpanY/MaxRGB60),x2,y2);  //1/MaxRGB60 of the span of (WidthIn100thOfTotalYSpan*pFig->m_dSpanY)
	RecHight=(int)(abs(y2-y1)+1);//1 is added to prevent occasional chasm.
	RecWidth=(int)(abs(x2-minScreenX));
	if(LeftOrRightOfX==0){//left
		initX=-RecWidth+minScreenX;
	}
	else{
		pFig->FromDataToScreenXY((double)pFig->m_dMaxX,(double)(pFig->m_dMinY),initX,dummyY); //It maps a relative point (i,j) of the graph to a point on the screen.	
	}
	FromScrnToDataCoordinate(initX,0,initXdouble,dummyDouble,pFig);
	for( i=0; i<MaxRGB60; i++){   //Squares
		rgb=(int)(MaxRGB60*((double)(i+1)/(double)MaxRGB60)); if(rgb>MaxRGB60-1)rgb=MaxRGB60-1; if(rgb<0)rgb=0;
		pFig->FillSolidRect_XY_Centered((double)initXdouble,((double)(i+1)/(double)MaxRGB60)*WidthIn100thOfTotalYSpan*pFig->m_dSpanY+pFig->m_dMinY,RecWidth, RecHight,m_RGB60[rgb],pDC);//m_PrevTx10-1 (-1 has been added to compensate the +error in RecWidth)
	}
	pDC->FillSolidRect(0,0,1, 1,RGB(255,255,255));//Is there any better solution?
}

//This maps a screen point (x,y) to a data point (i,j) on the selected graph.
void CAnalysisView::FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY, CFigure *pFig) 
{
	int paneTopX, paneTopY,paneBottomX, paneBottomY;
	pFig->FromDataToScreenXY(pFig->m_dMinX,pFig->m_dMaxY,paneTopX,paneTopY);
	pFig->FromDataToScreenXY(pFig->m_dMaxX,pFig->m_dMinY,paneBottomX,paneBottomY);
	DataX=(pFig->m_dMaxX-pFig->m_dMinX)*(((double)(x   -paneTopX))/(paneBottomX-paneTopX))+pFig->m_dMinX;	
	DataY=(pFig->m_dMaxY-pFig->m_dMinY)*(((double)(paneBottomY-y))/(paneBottomY-paneTopY))+pFig->m_dMinY;
}