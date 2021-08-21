
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


double CMOXY_View::Mean(double *Array, int N)
{
	int i;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		mean+=Array[i];
	}
	return mean/N;
}

double CMOXY_View::Mean(float *Array, int N)
{
	int i;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		mean+=Array[i];
	}
	return mean/N;
}

double CMOXY_View::Mean(int *Array, int N)
{
	int i;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		mean+=Array[i];
	}
	return mean/N;
}


double CMOXY_View::SD_GivenM(double *Array, int N,double mean)
{
	int i;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		SD+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(SD/N);
}

double CMOXY_View::SD_GivenM(float *Array, int N,double mean)
{
	int i;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		SD+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(SD/N);
}

double CMOXY_View::SD_GivenM(int *Array, int N, double mean)
{
	int i;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		SD+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(SD/N);
}


double CMOXY_View::Mean_OnlyPositive(double *Array, int N)
{
	int i,n=0;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		if(Array[i]>=0){
			mean+=Array[i];
			n++;
		}
	}
	return mean/n;
}


double CMOXY_View::SD_GivenM_OnlyPositive(double *Array, int N,double mean)
{
	int i,n=0;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		if(Array[i]>=0){
			SD+=(Array[i]-mean)*(Array[i]-mean);
			n++;
		}
	}
	return sqrt(SD/n);
}



// random number in range 0 ~ 1 with a gaussian distribution around 0.5
void CMOXY_View::Gaussian_WhiteNoise_0to1(double noise[], int numSamples) 
{	
	srand(time(0));

	double x = 0;
	int i=0;
	#define Mean .5
	#define Sig2 .04

	while(i<numSamples){
		x=RAND_RATE;
		if(PROBABILITY(exp(-(0.5-x)*(0.5-x)/Sig2))){
			noise[i] = x;
			i++;
		}
	}
}


double CMOXY_View::GenHist(double Data[],int dataSize, int Hist[], int binNum, double binWidth) 
{
	int i, idx;
	//m_grndSpkFrq.SetSize
	double miniDatum=Data[0];
	for(i=0;i<binNum;i++){Hist[i]=0;}//Initialize the Hist[]
	for(i=0;i<dataSize;i++){if(miniDatum>Data[i])miniDatum=Data[i];}//Find the minimum of the data
	for(i=0;i<dataSize;i++){
		idx=(int)((Data[i]-miniDatum)/binWidth);
		if(idx<binNum)Hist[idx]++;
	}
	return miniDatum;
}


//###############################  Sorting  ###############################
//This one sorts the x[] in an ascending order
void CMOXY_View::BubbleSort(int x[], int count)
{
	int i, j, tempID;

	for(i=0;i<count-1;i++){
		for(j=0;j<count-i-1;j++){
			if(x[j]>x[j+1]){
				tempID=x[j+1];
				x[j+1]=x[j];
				x[j]=tempID;
}	}	}	}

//This one  sorts the x[] and generates the prvOrder[], which now has previous order of x[].  
//   Use it in the following way:
//   BubbleSort(FileIdx,numFiles,PrvIdx);  =>  strcpy_s(File[i],tmpFile[PrvIdx[i]]);
void CMOXY_View::BubbleSort(int x[], int count, int prvOrder[])
{
	int i, j, tmpID,tmpPrv;
	for(i=0;i<count;i++)prvOrder[i]=i;

	for(i=0;i<count-1;i++){
		for(j=0;j<count-i-1;j++){
			if(x[j]>x[j+1]){
				tmpID=x[j+1];
				x[j+1]=x[j];
				x[j]=tmpID;

				tmpPrv=prvOrder[j+1];
				prvOrder[j+1]=prvOrder[j];
				prvOrder[j]=tmpPrv;
}	}	}	}

//Returns the order of x[] (OrderX[]) without changing the x[] itself
//   Use it in the following way:
//   Sort_inAscendingOrder(FileIdx,numFiles,OrderX);  => strcpy_s(FileIdx[OrderX[i]],tmpFile[i]);
void CMOXY_View::Sort_inAscendingOrder(int x[], int count, int OrderX[])
{
	int i, j, tmpID,tmpPrv, copyOfX[1000];
	for(i=0;i<count;i++)OrderX[i]=i;
	for(i=0;i<count;i++){copyOfX[i]=x[i];}

	for(i=0;i<count-1;i++){
		for(j=0;j<count-i-1;j++){
			if(copyOfX[j]>copyOfX[j+1]){
				tmpID=copyOfX[j+1];
				copyOfX[j+1]=copyOfX[j];
				copyOfX[j]=tmpID;
		}	}	
	}	
	for(i=0;i<count;i++){
		for(j=0;j<count;j++){
			if(x[i]==copyOfX[j]){
				OrderX[i]=j; break;
			}	
		}	
	}	
}
//###############################################################################################	