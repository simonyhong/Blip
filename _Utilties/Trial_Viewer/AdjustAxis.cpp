
#include "stdafx.h"
#include "Analysis.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include "SinTable.h"


void CAnalysisView::AdjustScale(double &minY, double &maxY) 
{
	int	i, howManyDivisionsY=4;
	int multiplied=0;
#define MultipliedBy 1000000.
	if(fabs(maxY)<1 && fabs(minY)<1){
		maxY*=MultipliedBy;minY*=MultipliedBy; 
		multiplied=1;
	}
	double spanY=maxY-minY;
	double cntrY=spanY/2+minY, savedCntrY=cntrY, diff;   
	double rawDivision=spanY/howManyDivisionsY;
	double ToleratDev20p=0.2*spanY;//20% of deviation may look okay.
	double DividedDev=ToleratDev20p/howManyDivisionsY;//This is the margin that a diviaion can move around
	
	char numberY0[100], numberY1[100],numberY2[100];
	int decimal, sign;
	//Translate the center
	diff=DifferenceToRound(cntrY,DividedDev);
	minY+=diff; maxY+=diff;		

	//Rescale the axis
	DifferenceToRound(maxY,DividedDev);
	minY=cntrY-((maxY+diff)-cntrY);
	spanY=maxY-minY;

	rawDivision=spanY/howManyDivisionsY;
	ToleratDev20p=0.2*spanY;//20% of deviation may look okay.
	DividedDev=ToleratDev20p/howManyDivisionsY;//This is the margin that a diviaion can move around
	diff=DifferenceToRound(minY,DividedDev);
//	maxY=cntrY+(cntrY-(minY+diff));//Don't do it.

	if(multiplied==1){maxY/=MultipliedBy;minY/=MultipliedBy;}

}
double CAnalysisView::DifferenceToRound(double &NumToAdjust, double VariableRange) 
{
	char  smallerNumStr[100],originalNumStr[100],biggerNumStr[100];
	int decimalSmall,decimalOriginal,decimalBigger, signSmall, signOriginal, signBigger;
	double NumToAdjustSaved=NumToAdjust, diff=0;
	double smallerNum=NumToAdjust-VariableRange,biggerNum=NumToAdjust+VariableRange;
	
	_fcvt_s(originalNumStr,sizeof(originalNumStr),NumToAdjust,0,&decimalOriginal, &signOriginal);//&decimal: Pointer to the stored decimal-point position.  &sign: Pointer to the stored sign indicator.
	if(fabs(NumToAdjust)<1.)sprintf(originalNumStr,"%s","0");
	_fcvt_s(smallerNumStr,sizeof(smallerNumStr),smallerNum,0,&decimalSmall, &signSmall);//&decimal: Pointer to the stored decimal-point position.  &sign: Pointer to the stored sign indicator.
	if(fabs(smallerNum)<1.)sprintf(originalNumStr,"%s","0");
	_fcvt_s(biggerNumStr,sizeof(biggerNumStr),biggerNum,0,&decimalBigger, &signBigger);//&decimal: Pointer to the stored decimal-point position.  &sign: Pointer to the stored sign indicator.
	if(fabs(biggerNum)<1.)sprintf(originalNumStr,"%s","0");
	if(*(smallerNumStr)!=*(biggerNumStr)||signSmall!=signBigger){
		if(signSmall!=signBigger){//The sign flipped: There is "0": this is a priority.
			NumToAdjust=0;
		}
		else if(*(smallerNumStr)!=*(originalNumStr)){//subtracting made a difference 
			//round down the numbers except the first one
			NumToAdjust=(int)NumToAdjust/(int)pow(10.,(int)(strlen(originalNumStr)-1))+1+(-1*signBigger);
			NumToAdjust*=pow(10.,(int)strlen(originalNumStr)-1);
		}
		else if(*(biggerNumStr)!=*(originalNumStr)){//adding made a difference ==>round up
			//round up the numbers except the first one
			NumToAdjust=(int)NumToAdjust/(int)pow(10.,(int)(strlen(originalNumStr)-1))+1+(-1*signBigger);
			NumToAdjust*=pow(10.,(int)strlen(originalNumStr)-1);
		}
		if(NumToAdjust>NumToAdjustSaved)diff=NumToAdjust-NumToAdjustSaved; else diff=NumToAdjustSaved-NumToAdjust;
	}
	else{//See if the second highest nuber can be rounded
		CString DoubleString;
		DoubleString = (originalNumStr+1);
		if(NumToAdjust<0)DoubleString.Insert(0, _T("-"));//Insert a characther like - or . to a specified location
		double secondNum = atof(DoubleString);
		diff=DifferenceToRound(secondNum, VariableRange); 
		NumToAdjust+=diff;
	}
	return diff;
}
