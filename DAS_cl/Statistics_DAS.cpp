
#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "Figure_DAS.h"
#include "Channel.h"
#include "DAS_View.h"
#include "SinTable.h"
#include "DialogueBox.h"

double CDasView::Mean(double *Array, int N)
{
	int i;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		mean+=Array[i];
	}
	return mean/N;
}

double CDasView::Mean(float *Array, int N)
{
	int i;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		mean+=Array[i];
	}
	return mean/N;
}

double CDasView::Mean(int *Array, int N)
{
	int i;
	double mean=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		mean+=Array[i];
	}
	return mean/N;
}


double CDasView::SD_GivenM(double *Array, int N,double mean)
{
	int i;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		SD+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(SD/N);
}

double CDasView::SD_GivenM(float *Array, int N,double mean)
{
	int i;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		SD+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(SD/N);
}

double CDasView::SD_GivenM(int *Array, int N, double mean)
{
	int i;
	double SD=0;

	if(N==0 && m_FirstTime0ElementErr){AfxMessageBox("N=0", MB_OK);  m_FirstTime0ElementErr=0; return 0;}
	for(i=0; i<N; i++){
		SD+=(Array[i]-mean)*(Array[i]-mean);
	}
	return sqrt(SD/N);
}


double CDasView::Mean_OnlyPositive(double *Array, int N)
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


double CDasView::SD_GivenM_OnlyPositive(double *Array, int N,double mean)
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



// random number in range 0 ~ 1 with a gaussian distribution around "Mean"
void CDasView::Gaussian_WhiteNoise_0to1(double noise[], int numSamples, double Mean, double Sig) 
{	
	srand(time(0));

	double x = 0;
	int i=0;
	double Sig2=Sig*Sig;

	while(i<numSamples){
		x=RAND_RATE;
		if(PROBABILITY(exp(-(0.5-x)*(0.5-x)/Sig2))){
			noise[i] = x-0.5+Mean;
			i++;
		}
	}
}


double CDasView::GenHist_fromMinimum(double Data[],int dataSize, int Hist[], int binNum, double binWidth) 
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


//!!Somehow the following one does not work!!
// Normal random variate generator.  mean m, standard deviation s 
// Implements the Polar form of the Box-Muller Transformation. Written by Everett F. Carter Jr.
double CDasView::box_muller(double m, double s)	
{
	double x1, x2, w, y1;
	static double y2;
	static int use_last = 0;

	if (use_last){		        /* use value from previous call */
		y1 = y2;
		use_last = 0;
	}
	else{
		do{
			x1 = 2.0 * RAND_RATE - 1.0;
			x2 = 2.0 * RAND_RATE - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}

//###############################  Sorting  ###############################
//This one sorts the x[] in an ascending order
void CDasView::BubbleSort(int x[], int count)
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
void CDasView::BubbleSort(int x[], int count, int prvOrder[])
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
void CDasView::Sort_inAscendingOrder(int x[], int count, int OrderX[])
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

