
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

#include "SharedConstants.h"
#include "SharedConst_TASMOXYVIS.h"
#include <time.h>

//####  !!!!  Do not use either "clock()" or "GetTickCount()" to measure time  !!!  ###########
// Sleep(t) function is accurate: the actual idling time = t+-1ms. For example Sleep(20) would result in 19.9~20.99ms of sleep. 

//Idling function
void CMOXY_View::SleepAccurate(int FixedDur, int variableDur)
{
	if(FixedDur<0){AfxMessageBox("FixedDur<0");return;}
	__int64 newVal,lastVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastVal);
	unsigned VariablePart=0, totDelay;
	if(variableDur>0)VariablePart=Random(variableDur);
	totDelay=FixedDur+VariablePart; if(totDelay<1.)return;
	if(totDelay>=ACCURACY_OF_SLEEP_PLUS1){
		while(1){
			Sleep(1);
			if(totDelay-HighPerformanceTime(lastVal)<ACCURACY_OF_SLEEP)break;
			if(!m_bIsON)return;
		}
	}
	//Fine delay
	while(1){
		if(HighPerformanceTime(lastVal)>totDelay+0.002)break;
		if(!m_bIsON)return;
	}
}

//Idling function
void CMOXY_View::Sleep_NOT_Accurate(int FixedDur, int variableDur)
{
	if(FixedDur<0){AfxMessageBox("FixedDur<0");return;}
	Sleep(FixedDur);
	Sleep(Random(variableDur));
}
//Idling function
void CMOXY_View::SleepAccurate_1ms()
{
	__int64 lastVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastVal);
	while(1){
		if(HighPerformanceTime(lastVal)>1.002)break;
		if(!m_bIsON)return;
	}
}


double CMOXY_View::HighPerformanceTime(__int64 lastVal)
{
	__int64 newVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&newVal);
	return (1000.0 * (double)(newVal-lastVal)/ m_CPUfrq);
}


void CMOXY_View::init_IdleTimers_to0()
{
	for(int i=0;i<MaxNumTimer;i++){m_durOfIdle[i]=0;}
}


