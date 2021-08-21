
#include "stdafx.h"
#include "MOXY.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "MOXY_Doc.h"
#include "Figure_MOXY.h"

#include "RunningLineDisplay.h"
#include "MOXY_View.h"
#include "SinTable.h"


void CMOXY_View::CosineColorMix(COLORREF RGB_array[],int arraySize) 
{
	int i;
	int OneSixth=arraySize*1.0/6.0, TwoSixths=arraySize*2.0/6.0, ThreeSixths=arraySize*3.0/6.0, FourSixths=arraySize*4.0/6.0, FiveSixths=arraySize*5.0/6.0;
	int OneThird=TwoSixths, Half=ThreeSixths; 
	double Ker;
	double RedKer[1000],GreenKer[1000],BlueKer[1000];
#define POS_COSINE(Border) ((1 +cos( TWO_PI*(i-Border)/((double)OneThird))  )*0.5)
#define NEG_COSINE(Border) ((1 -cos( TWO_PI*(i-Border)/((double)OneThird))  )*0.5)
	//########  RED
	for(i=0; i<arraySize; i++){
		if(i<OneSixth)Ker=POS_COSINE(0);
		else if(i<Half)Ker=0;
		else if(i<FourSixths)Ker=NEG_COSINE(Half);
		else Ker=1;
		RedKer[i]=255*Ker;
	}		
	//########  GREEN
	for(i=0; i<arraySize; i++){
		if(i<OneSixth)Ker=0;
		else if(i<TwoSixths)Ker=NEG_COSINE(OneSixth);
		else if(i<FourSixths)Ker=1;
		else if(i<FiveSixths)Ker=POS_COSINE(FourSixths);
		else Ker=0;
		GreenKer[i]=255*Ker;
	}		
	//########  BLUE
	for(i=0; i<arraySize; i++){
		if(i<TwoSixths)Ker=1;
		else if(i<Half)Ker=POS_COSINE(TwoSixths);
		else if(i<FiveSixths)Ker=0;
		else Ker=NEG_COSINE(FiveSixths);
		BlueKer[i]=255*Ker;
	}		
	for(int i=0; i<arraySize; i++){
		RGB_array[i]=RGB(RedKer[i], GreenKer[i], BlueKer[i]);
	}		
}



//void CMOXY_View::CosineColorMix(COLORREF RGB_array[],int arraySize) 
//{		
//	double Ker;
//	double RedKer[1000],GreenKer[1000],BlueKer[1000],shift=0.08;
//	double startRED=arraySize*0.75, endRED=arraySize*1;
//	double startGRN=arraySize*0.25, endGRN=arraySize*0.75;
//	double startBLU=arraySize*0.,  endBLU=arraySize*0.25;
//	for(int i=0; i<arraySize; i++){
//		//########  RED
//		if(i<0.5*arraySize)Ker=0;
//		else if(i<startRED)//When the i is on the left side of the plateau (maximum) area of a kernel
//			Ker=(-cos(((float)(i-0.5*arraySize)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
//		else //When the i is on the right side of the plateau (maximum) area of a kernel
//			Ker=1;
//		RedKer[i]=255*Ker;
//		//########  GREEN
//		if(i<startGRN)//When the i is on the left side of the plateau (maximum) area of a kernel
//			Ker=(-cos(((float)i/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
//		else if(i>=startGRN && i<endGRN)//When the i is somewhere inside the plateau (maximum) of a kernel
//			Ker=1;
//		else //When the i is on the right side of the plateau (maximum) area of a kernel
//			Ker=(cos(((float)(i-endGRN)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
//		GreenKer[i]=255*Ker;
//		//########  BLUE
//		if(i<endBLU)//When the i is on the left side of the plateau (maximum) area of a kernel
//			Ker=1;
//		else if(i<0.5*arraySize)//When the i is on the right side of the plateau (maximum) area of a kernel
//			Ker=(cos(((float)(i-endBLU)/(float)(arraySize*0.25))*ONE_PI)+1)*0.5;
//		else 
//			Ker=0;
//		BlueKer[i]=255*Ker;
//		RGB_array[i]=RGB(RedKer[i], GreenKer[i], BlueKer[i]);
//	}		
//}


void CMOXY_View::CosineColorMix_adjusted(COLORREF RGB_array[],int arraySize)
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

void CMOXY_View::MyOriginalColorMix(COLORREF RGB_array[],int arraySize) 
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

