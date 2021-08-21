#include "stdafx.h"
#include <math.h>
#include "Figure.h"

int CFigure::P[1000];
int CFigure::NumP;
double CFigure::KrVal[1000]; 

void CFigure::Build_SpkHistogram_withoutSDF()
{
	int i, numSpks, spktime,endI;
	int	*pSpk=&m_SpkTimes[m_CurrIdxOfSpkTimes][0];
	double y, beginX=m_dMinX-MAX_SIGMA_x2;
	m_numTimesCalled+=1;
	numSpks=m_numSpks[m_CurrIdxOfSpkTimes];
	if(m_HistOfSpikes.GetSize()==0){AfxMessageBox("m_HistOfSpikes.GetSize()=0"); return;}	
	for(i=0;i<numSpks;i++){
		spktime=pSpk[i];
		if(spktime>=m_dMinXMinus2Sigma && spktime<m_dMaxXPlus2Sigma){
			m_HistOfSpikes[spktime-m_dMinXMinus2Sigma]+=1000;
		}
	}
	m_PrvActive=1;
}

void CFigure::Make_SDF_from_Histogram()
{
	Convolution_SpkFrq(&m_SDF[0],&m_HistOfSpikes[0],m_HistArraySize,m_dSigma);
}

//Using the stored m_numSpks[], remake Histogram and SDF 
void CFigure::ReMake_SDF()
{
	int size;
	int trl, lastTrl=0, SavedCurrIdxOfSpkTimes;
	
	//Initialize Figures
	m_dMinXMinus2Sigma=m_dMinX-MAX_SIGMA_x2; m_dMaxXPlus2Sigma=m_dMaxX+MAX_SIGMA_x2;
	size=m_dMaxX-m_dMinX+2*MAX_SIGMA_x2;
	m_HistOfSpikes.SetSize(size);
	m_SDF.SetSize(size);
	m_HistArraySize=size;
	EmptyHistogramOnly();

	if(m_numSpkArrayLists<=0)return;
	// Make SDF
	SavedCurrIdxOfSpkTimes=m_CurrIdxOfSpkTimes;
	m_CurrIdxOfSpkTimes=m_OldestSpkArrayIdx-1;
	do{
		m_CurrIdxOfSpkTimes++;
		if(m_CurrIdxOfSpkTimes>=MAX_TRLS_STORED_IN_FIG)m_CurrIdxOfSpkTimes=0;
		Build_SpkHistogram_withoutSDF();
	}while(m_CurrIdxOfSpkTimes!=SavedCurrIdxOfSpkTimes);
	Make_SDF_from_Histogram();
}

void CFigure::VoidEventMarks()
{
	if(m_numSpkArrayLists==0)return;
	int size;
	int trl, lastTrl=0, SavedCurrIdxOfSpkTimes;
	
	SavedCurrIdxOfSpkTimes=m_CurrIdxOfSpkTimes;
	int CurrIdxOfSpkTimes=m_OldestSpkArrayIdx-1;
	do{
		CurrIdxOfSpkTimes++;
		if(CurrIdxOfSpkTimes>=MAX_TRLS_STORED_IN_FIG)CurrIdxOfSpkTimes=0;
		m_EventTimeToMark[CurrIdxOfSpkTimes]=INT_MAX;
	}while(CurrIdxOfSpkTimes!=SavedCurrIdxOfSpkTimes);
}

void CFigure::DispSDF_OneType(CDC *pMDC, int isToShowSDF)
{
	if(m_numTimesCalled<1)return;
	char Property[24];
	sprintf(Property,"N=%d",(int)m_numTimesCalled);
	pMDC->TextOut(m_TrlsPosX,m_TrlsPosY,Property);
	if(isToShowSDF==0)return;
	int i, endI;
	double	y, averageIt=1./m_numTimesCalled, beginX=m_dMinX-MAX_SIGMA_x2;
	pMDC->SelectObject(&m_DkGreenPen); 	 //pMDC->SelectObject(&m_BluePen);
	y=m_SDF[(int)(MAX_SIGMA_x2)]*averageIt; if(y<m_dMinY)y=m_dMinY; else if(y>m_dMaxY)y=m_dMaxY;
	pMDC->MoveTo(lX(m_dMinX),lY(y));
	endI=m_HistArraySize-MAX_SIGMA_x2;
	for(i=MAX_SIGMA_x2+1;i<endI;i++){
		y=m_SDF[i]*averageIt; if(y<m_dMinY)y=m_dMinY; else if(y>m_dMaxY)y=m_dMaxY;
		pMDC->LineTo(lX(i+beginX),lY(y));
	}
}

void CFigure::DispSpkRaster_OneType(CDC *pMDC, int isToShowRaster)
{		
	if(isToShowRaster==0 && m_isToShowMarks==0)return;
	if(m_numTimesCalled<1)return;
	int rstIdx,spkCnt,numSpks,FirstRasterPos;
	int	*pSpk;
	float RstrStep,whereToDispY,tm;

	FirstRasterPos=m_dSpanY*0.5+m_dMinY; RstrStep=(float)(0.5*m_dSpanY/HOW_MANY_TRLS_TO_DISP);

	if(m_numSpkArrayLists<=HOW_MANY_TRLS_TO_DISP){
		rstIdx=m_OldestSpkArrayIdx;
		if(rstIdx<0)return;//Nothing yet
	}
	else{
		rstIdx=m_CurrIdxOfSpkTimes-(HOW_MANY_TRLS_TO_DISP-1);
		if(rstIdx<0)rstIdx=MAX_TRLS_STORED_IN_FIG+rstIdx;
	}
	int	rsrOrder=0, last=0;
	do{
		if(rstIdx==m_CurrIdxOfSpkTimes)last=1;
		whereToDispY=FirstRasterPos+(rsrOrder++)*RstrStep;
		if(isToShowRaster==1)DispOneLineOfRaster(whereToDispY,&m_SpkTimes[rstIdx][0],m_numSpks[rstIdx],pMDC);
		if(m_isToShowMarks==1)MySet2x2RedBox(m_EventTimeToMark[rstIdx], whereToDispY, pMDC);
		if(++rstIdx==MAX_TRLS_STORED_IN_FIG)rstIdx=0;
	}while(last==0);
}

void CFigure::DispOneLineOfRaster(float whereToDispY, int SpkTimes[], int numSpks, CDC *pMDC)
{
	int spkCnt;
	float tm;
	for(spkCnt=0;spkCnt<numSpks;spkCnt++){
		tm=SpkTimes[spkCnt];
		if(tm>m_dMinX&&tm<m_dMaxX){
			MySet2x1BlackBox(tm, whereToDispY, pMDC);
			//MySet2x2BlackBox(tm, whereToDispY, pMDC);
		}
	}
}

void CFigure::EmptyHistogramAndSpikes()
{	
	int i;
	for(i=0;i<m_HistArraySize;i++){
		m_HistOfSpikes[i]=0;
	}
	m_numTimesCalled=0;
	m_numSpkArrayLists=0;//This is okay because the size of array is not fixed (MAX_TRLS_STORED_IN_FIG).
	m_CurrIdxOfSpkTimes=-1; m_OldestSpkArrayIdx=0;
}

void CFigure::EmptyHistogramOnly()
{	
	int i;
	for(i=0;i<m_HistArraySize;i++){
		m_HistOfSpikes[i]=0;
	}
	m_numTimesCalled=0;
}

void CFigure::RedZeroAxes(CDC* pDC)
{
	double Lo;
	pDC->SelectObject(&m_RedPen); 
	//---------- "X=0" line
	if(0>=m_dMinX && 0<=m_dMaxX){
		Lo=lX(0);
		pDC->MoveTo((int)Lo,(int)lY(m_dMinY));
		pDC->LineTo((int)Lo,(int)lY(m_dMaxY));
	}
	//----------- "Y=0" line
	if(0>=m_dMinY && 0<=m_dMaxY){
		Lo=lY(0);
		pDC->MoveTo((int)lX(m_dMinX),(int)Lo);
		pDC->LineTo((int)lX(m_dMaxX),(int)Lo);
	}
} 		
void CFigure::BlackZeroAxes(CDC* pDC)
{
	double Lo;
	pDC->SelectObject(&m_BlackPen1); 
	//---------- "X=0" line
	if(0>=m_dMinX && 0<=m_dMaxX){
		Lo=lX(0);
		pDC->MoveTo((int)Lo,(int)lY(m_dMinY));
		pDC->LineTo((int)Lo,(int)lY(m_dMaxY));
	}
	//----------- "Y=0" line
	if(0>=m_dMinY && 0<=m_dMaxY){
		Lo=lY(0);
		pDC->MoveTo((int)lX(m_dMinX),(int)Lo);
		pDC->LineTo((int)lX(m_dMaxX),(int)Lo);
	}
} 	
//This is a general form of convolution
//Even though this function is slow because it handles all the 1ms bins, this is feasable because the bins are filled-in over time.
void CFigure::Convolution_SpkFrq(double SpkFreq[], double RasterArry[], int arraySize, double sigma)
{	
#define KrSum 1
	int i, p, pId, initI=0,endI=initI+arraySize;
	float RangeX,  convoluted, SumMissingKer;
	RangeX=(2*sigma); if(RangeX<1)RangeX=1;

	if(sigma!=m_SavedSig){
		CreateKer(sigma,RangeX);
		m_SavedSig=sigma;
	}
	for(i=initI;i<endI;i++){
		convoluted=0; 
		if(i<RangeX+initI||i>endI-1-RangeX){// Incomplete Kernel
			SumMissingKer=0;
			for(pId=0; pId<NumP; pId++){
				p=i+P[pId];
				if(p<initI||p>endI-1){SumMissingKer+=KrVal[pId];continue;}
				convoluted+=KrVal[pId]*RasterArry[p];
			}
			convoluted*=(KrSum/(KrSum-SumMissingKer));
		}
		else{
			for(pId=0; pId<NumP; pId++){
				p=i+P[pId];
				convoluted+=KrVal[pId]*RasterArry[p];
		}	}	
		SpkFreq[i]=convoluted;
	}
}

void CFigure::CreateKer(double sig, double RangeX)
{	
	#define KrCntr 0
	int i, p, IniP,FinP,pId;
	double GaussianAmp,RangeXSq, sigSq, SqDist;
	sigSq=sig*sig; RangeXSq=RangeX*RangeX;
	//////////////////////////////////
	GaussianAmp=KernelAmplitude(KrSum, sig, RangeX);	
	NumP=0;
	IniP=KrCntr-RangeX; FinP=KrCntr+RangeX;
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(KrCntr,p))>(double)RangeXSq)continue;
		KrVal[NumP]=GaussianAmp*exp(-SqDist/sigSq);
		P[NumP]=p; NumP++;
	}
	/////// Display the shape of Gaussian Kernel
	//CClientDC aDC(m_pView);
	//for(pId=0; pId<NumP; pId++){
	//	m_pView->m_Board[0].Abs_Graph2D(P[pId]*100, KrVal[pId]*100,30, m_pMDC);
	//}
	//m_pView->m_Board[0].RefreshFrame(&aDC,m_pMDC);
	////////////////////
}
double CFigure::KernelAmplitude(double KerSum, double Sigma, double RangeX)
{
	double sigSq=Sigma*Sigma, RngXSq=RangeX*RangeX, SqDist, Sum; 
	int p,IniP=0-RangeX,FinP=0+RangeX; 

	Sum=0; 
	for(p=IniP;p<=FinP;p++){
		if((SqDist=SQ_DISTANCE_AB_1D(0, p))>RngXSq)continue;
		Sum += exp(-SqDist/sigSq);
	}	
	return (KerSum/Sum);  //KerSum=Kernel*SIG(exp[.])
}