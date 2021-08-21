// DAS_View.cpp : implementation of the CDasView class
//

#include "stdafx.h"
#include "DAS.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "DAS_Doc.h"
#include "VarDefinitions_SIG.h"
#include "Channel.h"
#include "DAS_View.h"
#include "SinTable.h"
#include "MainFrm_DAS.h"
#include <NIDAQmx.h>
#include "Mmsystem.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CDasView::m_bIsON;
/////////////////////////////////////////////////////////////////////////////
// CDasView

///////////////////////////////////////
UINT ShowTwoD(LPVOID WinObjPtr) ;
UINT ShowThreeD(LPVOID WinObjPtr) ;
///////////////////////////////////////


IMPLEMENT_DYNCREATE(CDasView, CView)

BEGIN_MESSAGE_MAP(CDasView, CView)
	//{{AFX_MSG_MAP(CDasView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_2DGraph, On2DGraph_I)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MYMESSAGE_CHAN_CONFIG, OnChangeOfChanConfig)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


// CDasView construction/destruction
CDasView::CDasView()
{
	m_bFistTime=1;
	m_bIsON=0;
	m_RefreshAtEveryNWaveforms=7;
	m_NumAiChannels=MAX_AI_CHAN;//neural analog channels+ behavior analog channels
	for(int i=0; i<MAX_AI_CHAN;i++){
		m_AiChanID[i]=i;
	}

	/////////////// Parameters For Applications
	m_SamplingCounter=0;
	m_EvStopProcess=NULL; m_Ev_WakeUp_ForAntidromicStimulation=NULL;
	m_HowManyDataPoints_ToFind_StartOfArtifact=0;	
	m_NumOfArtifactDataPoints_toLEARN=0; 
	m_NumOfArtifactDataPoints_toZAP=0; 
	m_ExpectedTimeOfARTIFACT_MinusHalfMs=1;
	m_StartAIidx_toLearnArtifact=-1;
	m_IsResting=0;
	m_ArrayIndex_forUsableAiId=0;
	InitializeCriticalSection(&m_CriticalSectionDAQ);
	
	char EvName[64];
	for(int i=0; i<MAX_DO_NUM; i++){
		sprintf(EvName,EventObjectsName,i); //These names should be the same as the ones in TAS
		m_DO_Active[i] =CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,EvName);
	}
}					

CDasView::~CDasView() 
{
	m_bIsON=0;    //In case the thread uses a switch (m_bIsON)
	SetEvent(m_EvStopProcess);//In case the thread uses a stop event (m_EvStopProcess)
	Sleep(10);//Give some time to finish.

	//Destroy the task threads
	DAQmxClearTask(m_pTaskHandle_DI); 
	DAQmxClearTask(m_pTaskHandle_DO); 
	DAQmxClearTask(m_pTaskHandle_AI); 

	// Wait for thread to stop, in case the thread uses a thread handle, m_hThread.
	//if(m_hSimulatedDAQmxThread!=NULL){
	//    WaitForSingleObject(m_hSimulatedDAQmxThread,1000);
	//    CloseHandle(m_hSimulatedDAQmxThread);
	//}
	//// Wait for thread to stop, in case the thread uses a thread handle, m_hThread.
	//if(m_hThread!=NULL){
	//    WaitForSingleObject(m_hThread,1000);
	//    CloseHandle(m_hThread);
	//}
	//CloseHandle(m_EvStopProcess);

	//In case the thread uses an old switch method to terminate, just wait for a sec.
	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	
	
	for(int i=0; i<MAX_DO_NUM; i++){CloseHandle(m_DO_Active[i]);	}	

	*M_isDAS_Running=0;
	CloseSharedMemoryArea();

	for(int timeOut=0; timeOut<10; timeOut++){
		Sleep(10);
	}	
	m_dRawData.RemoveAll();

	DeleteCriticalSection(&m_CriticalSectionDAQ);
}

BOOL CDasView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDasView drawing
DWORD WINAPI  Graph2D_I(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->DAS_sv_main();
	if(*(pView->M_Blip_OFF)==1){
		AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
	}

	return 0;
}
void CDasView::OnDraw(CDC* pDC)
{

#define CH_SIZE_X 8
#define CH_SIZE_Y 10
#define LINE_GAP  5
#define STR_SIZE  150
#define SFT		  0
#define SFT1      200
#define SFT2      400
#define SFT3      600

	CDasDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bFistTime==1){
		m_bFistTime=0;
		m_hThread=CreateThread(NULL,0,Graph2D_I,this,0,NULL);
		//AfxBeginThread( Graph2D_I, this);
	}
	//##########  HIDE ###########
	//##########  HIDE ###########
	CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
	pMainWnd->ShowWindow(SW_MINIMIZE);//SW_HIDE
	//##########  HIDE ###########
	//##########  HIDE ###########

}

/////////////////////////////////////////////////////////////////////////////
// CDasView printing

BOOL CDasView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDasView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDasView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDasView diagnostics

#ifdef _DEBUG
void CDasView::AssertValid() const
{
	CView::AssertValid();
}

void CDasView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDasDoc* CDasView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDasDoc)));
	return (CDasDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDasView message handlers

void CDasView::On2DGraph_I() 
{
		//AfxBeginThread( Graph2D_I, this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//#####################################################################################################
		
DWORD WINAPI SimulatedDAQmx(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Simulated_DAQmx() ;
	return 0;
}
DWORD WINAPI CheckToEndDASsv(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->Check_to_End_DASsv() ;
	return 0;
}
DWORD WINAPI Digital_Out(LPVOID WinObjPtr) //A global Function
{
	CDasView* pView=(CDasView*) WinObjPtr;
	pView->DigitalOut() ;
	return 0;
}

long CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
long CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

//This function is called to initialize all the "DYNAMIC" variables about the shard memory map
//Other programs use these variables; so they need to WAIT until DAS_sv is up and running (i.e., *M_isDAS_Running==1).
void CDasView::Init_MemoryMapVariables() 
{
	int i, microSecPerPoint;
	//Shared variables
	*M_SamplingRate=DATA_SAMPLING_RATE;
	*M_BehaviorSamplingRate = BEHAVIOR_DATA_SAMPLING_RATE;
	*M_SamplesPerRead=SAMPLES_PER_READ; 
	*M_SamplingEpoch_us=(int)(1000000*(*M_SamplesPerRead)/(float)(*M_SamplingRate));
	*M_microSecPerPoint = *M_SamplingEpoch_us/(*M_SamplesPerRead);
	if(*M_microSecPerPoint != *M_SamplingEpoch_us/(double)(*M_SamplesPerRead))AfxMessageBox("*M_microSecPerPoint != *M_SamplingEpoch_us/(*M_SamplesPerRead)");

	*M_numNeuralChannels = NUM_NN_AI_CHAN;
	*M_numBehaviorChans  = NUM_2D_BHVR_AI_CHANS;
	*M_CurrIdx_Of_StimGateRelayID=0;
	*M_is_LFP_saving=0;

	for(i=0; i<MAX_NUM_STIM_CHANNELS;i++){	
		M_isFocusedChannel[i] = 0;
	}	
	*M_NumOfFocusChannels=0;

	for(i=0; i<m_NumAiChannels;i++){	
		M_AIQueueSize[i] = AI_Q_SIZE; 
		if((M_AIQueueSize[i])%(*M_SamplesPerRead)!=0){AfxMessageBox("*M_AIQueueSize[i] is not a multiple of *M_SamplesPerRead: Exiting..."); exit(1);}
		*M_usableAiIdx[i]=-1;
	}
	for(i=0; i<NUM_NN_AI_CHAN;i++){	
		M_WaveSpanPost_ms[i] = DEFAULT_WAVESPAN_POST_THRESHOLD;
		M_WaveSpanPre_ms[i] = DEFAULT_WAVESPAN_PRE_THRESHOLD;
		M_preWaveSpanDataSize[i] =TIME_TO_DATA_POINTS(DEFAULT_WAVESPAN_PRE_THRESHOLD);
		M_postWaveSpanDataSize[i]=TIME_TO_DATA_POINTS(DEFAULT_WAVESPAN_POST_THRESHOLD);
		M_DataPointsPerWave[i]   =M_preWaveSpanDataSize[i]+M_postWaveSpanDataSize[i];
		*M_usableSIGi[i]=-1;
		M_usableWaveIdx[i]=-1;
		*M_usableSpikeIdxOfNn1[i]=-1; *M_usableSpikeIdxOfNn2[i]=-1; 	*M_usableSpikeIdxOfNn3[i]=-1;
		M_spike_IdxOfAIOfNn1[i][MAX_SPIKEs-1]=-1; 
		M_spike_IdxOfAIOfNn2[i][MAX_SPIKEs-1]=-1; 
		M_spike_IdxOfAIOfNn3[i][MAX_SPIKEs-1]=-1;//This info is used in XCOR to block any backward search of spikes, when there is no prior spike.
		if(M_NumOfFigsUsingThisChan[i]<=0)M_Threshold[i]=0.5;//Set the default threshold only when the DAS client hasn't 
	}
	if(M_Threshold[NUM_NN_AI_CHAN]==INT_MAX)ReadThresholds();//Read the threshold only when the DAS server hasn't. 
	*M_WavePointer_QueueSize = MAX_WAVE_PTRs;
	*M_Spike_QueueSize = MAX_SPIKEs;
	*M_DO_QueueSize = MAX_DO_NUM;
	*M_Ecode_QueueSize = ECODE_Q_SIZE;
}

//######## Global variables for the call-back fuction; to speed up the call back function
int g_IsSimulatedDAQmx=0, m_SimulateNeuron=0, m_SimulateBehavior=0, *g_Flip_AI_SignalSign;
CDasView * g_pView;
double *g_rawAI, *g_AI, g_1000000perCPU;
CAiChannel *g_pChan, *g_pInitChan;
int g_numAiChan,g_MaxGitterInTime, g_ArraySize_RawData, g_SamplesPerRead, g_NumRead,g_SamplingEpochInMicS, g_SampleBehaviorAtEveryXmicS, *g_SampTimeArrayIdx;
__int64 *g_timeStampMicS, g_beginValInMicS, *g_LatestTimeStampInMicroS, g_PrvTime2RetrieveData, *g_firstAIindex, *g_SampTimeArrayInMicS;
int *g_SomeChanBoxInfoChanged, *g_SamplingCounter, *g_NumOfFigsUsingThisChan, *g_ThreadLaggingCounter, *g_Inter_DataRetrival_Interval_inMicroSec;
__int64 samplCounter=0,*pChan0ThreadCounter, g_TimeStepPerPointInThisReadingInterval, g_SimulationCounter, g_SimulationCounter2;
unsigned char *g_DO_Array, *g_DI_Array;
TaskHandle	*g_pTaskHandle_DO;
DWORD_PTR g_threadAffMask=1;// 64 bit mask (00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000001)
//########

void CDasView::DAS_sv_main() 
{
	if(m_bIsON==1){m_bIsON=0;return; }
	m_bIsON=1;
	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////

	//Set the priority
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);// REALTIME_PRIORITY_CLASS (may make the Windows freeze), HIGH_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS 
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);// THREAD_PRIORITY_TIME_CRITICAL  THREAD_PRIORITY_HIGHEST  THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_NORMAL  THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_LOWEST  THREAD_PRIORITY_IDLE 

	//You may force this thread to run on a core specified by the mask in the following line of command to use the same time counter attached to one core to query the high performance ticks (QueryPerformanceCounter)
	//I don't think it is necessary, because, at least intel x86 CPUs seem to share the same "mother board clock?" to provide high performance time.
	//Refer to "Time Stamp Counter" in Wikipedia, and "Game Timing and Multicore Processors" in online MSDN.
	//SetThreadAffinityMask(GetCurrentThread(), g_threadAffMask); 

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_CPUfrq);//Do this only once. See "Game Timing and Multicore Processors" in online MSDN to read the document on this.
	if(m_CPUfrq<=0){AfxMessageBox("This system does not support the high-resolution performance counter"); exit(1);}
	m_1000000perCPU=(float)1000000/(float)m_CPUfrq;

	//High frequency processor tick monitor. 
	//QueryPerformanceCounter() --> Microsoft: "On a multiprocessor computer, it should not matter which processor is called. However, you can get different results on different processors due to bugs in the basic input/output system (BIOS) or the hardware abstraction layer (HAL). To specify processor affinity for a thread, use the SetThreadAffinityMask function."
	//Read "Game Timing and Multicore Processors" in online MSDN to read the document on this.
	//I use this instead of the NI board counter. 
	//The reason is that NI board counter only gives the number of samples acquired. Therefore yielding low time resolution. Sometimes, especially during the antidromic stimulation, DAS_sv needs to be able to measure micro-second level delays.
	//By using this high performance counter, I can always measure the relative time of an event in micro-second accuracy (mostly).
	//As long as the acquring process by the NI board and the hand-over to the DAS_sv program is close enough, this mechanism will work.
	//Of course this assumption is based on the performance of the computer. So far, I have not observed too much deviation from the expected measures. And hopefully, as the computer gets better, it gets more and more accurate.
	//The usage of the computer colck vastly simplifes the software to measure and time-stamp events.
	QueryPerformanceCounter((LARGE_INTEGER*)&m_beginVal);

	//############ Create the memory map
	CreateSharedMemoryArea();
	Init_MemoryMapVariables();//This needs to be here (only for the server).

	ReportSummaryOfBoards(); //By default this function finds devices and uses the first one. In the case you want to change is behavior, disable this line and use the next line.
	//m_BoardIdxUsed=0; m_BoardName[0]="Dev1";
	if(MAX_AI_CHAN>32){AfxMessageBox("Currently Blip supports only one NI board, so you can have up to 32 (=Neural+Behavioral) AI channels."); exit(1);}

	//########################################################
	//						Analog Input  
	//########################################################
	//##########  Create an Analog Input task
	long	error=0;
	m_pTaskHandle_AI=0;
	DAQmxCreateTask("",&m_pTaskHandle_AI);

	//########## Initialize the SOFTWARE Channel configuration
	int i,WaveCnt;
	char chanID[256];
	CAiChannel *pChan;
	for(i=0; i<m_NumAiChannels;i++){
		if(IsToCreate==1)M_NumOfFigsUsingThisChan[i]=0;//If this process created the memory map, the initial assumption: all are NOT used
		pChan=&(AiChan[i]);
		pChan->pView=this;
		pChan->m_ChanID=i;
		pChan->M_MiniRequiredLengthOfwaveForExamination=&M_MiniRequiredLengthOfwaveForExamination[i];
		pChan->m_AiIdx=0;//Initialize the data index.
		//Shared memory map
		pChan->M_AI=M_AI[i];
		pChan->M_timeStampMicS=M_timeStampMicS[i];
		pChan->M_AIQueueSize=&M_AIQueueSize[i];
		pChan->M_usableAiIdx=M_usableAiIdx[i];
		//Create a Voltage channel
		if(m_NumOfBoards>0){
			sprintf(chanID,"%s/ai%d",m_BoardName[0],m_AiChanID[i]); //m_BoardName[0] should be something like (Dev1, Dev2, Dev3 ...)
			DAQmxCreateAIVoltageChan(m_pTaskHandle_AI,
				chanID,
				"",
				DAQmx_Val_RSE/*Referenced single-ended mode*/,
				-pChan->gain,pChan->gain,
				DAQmx_Val_Volts,NULL);
		}
		//To speed up
		pChan->m_AIQ_size  =M_AIQueueSize[i];
		pChan->m_AIQ_size_1=M_AIQueueSize[i]-1;
	}
	for(i=0; i<NUM_NN_AI_CHAN;i++){	
		pChan=&(AiChan[i]);
		pChan->M_usableSIGi			=M_usableSIGi[i];

		pChan->M_preWaveSpanDataSize=&M_preWaveSpanDataSize[i];
		pChan->M_postWaveSpanDataSize=&M_postWaveSpanDataSize[i];
		pChan->M_DataPointsPerWave=&M_DataPointsPerWave[i];
		pChan->M_Threshold			=&M_Threshold[i];

		pChan->M_firstIdxOfWavelet	=M_firstIdxOfWavelet[i];
		pChan->M_lengthOfSignal		=M_lengthOfSignal[i];
		pChan->M_AdjustXToXthreshold=M_AdjustXToXthreshold[i];
		pChan->M_usableWaveIdx		=&M_usableWaveIdx[i];
		pChan->M_usableSpikeIdxOfNn1=M_usableSpikeIdxOfNn1[i];
		pChan->M_usableSpikeIdxOfNn2=M_usableSpikeIdxOfNn2[i];
		pChan->M_usableSpikeIdxOfNn3=M_usableSpikeIdxOfNn3[i];
		pChan->M_spikeWaveIdxOfNn1	=M_spikeWaveIdxOfNn1[i];
		pChan->M_spikeWaveIdxOfNn2	=M_spikeWaveIdxOfNn2[i];
		pChan->M_spikeWaveIdxOfNn3	=M_spikeWaveIdxOfNn3[i];
		pChan->M_spikeTimeStampOfNn1=M_spikeTimeStampOfNn1[i];
		pChan->M_spikeTimeStampOfNn2=M_spikeTimeStampOfNn2[i];
		pChan->M_spikeTimeStampOfNn3=M_spikeTimeStampOfNn3[i];
		pChan->M_spike_IdxOfAIOfNn1	=M_spike_IdxOfAIOfNn1[i];
		pChan->M_spike_IdxOfAIOfNn2	=M_spike_IdxOfAIOfNn2[i];
		pChan->M_spike_IdxOfAIOfNn3	=M_spike_IdxOfAIOfNn3[i];
		pChan->M_numSpikesOfNn1		=M_numSpikesOfNn1[i];
		pChan->M_numSpikesOfNn2		=M_numSpikesOfNn2[i];
		pChan->M_numSpikesOfNn3		=M_numSpikesOfNn3[i];
		pChan->M_firstSpkIdxInTrialOfNn1=M_firstSpkIdxInTrialOfNn1[i];
		pChan->M_firstSpkIdxInTrialOfNn2=M_firstSpkIdxInTrialOfNn2[i];
		pChan->M_firstSpkIdxInTrialOfNn3=M_firstSpkIdxInTrialOfNn3[i];
		pChan->M_Spike_QueueSize		=M_Spike_QueueSize;
		pChan->M_WvEcode				=M_WvEcode[i];
		for(WaveCnt=0 ; WaveCnt<MAX_WAVE_PTRs; WaveCnt++){
			pChan->M_WvEcode[WaveCnt]=ECODE_UNSELECT;		
		}
		pChan->MiniRequiredLengthOfwaveForExamination();//For now all 0
		pChan->M_AntidromicTriggeringChanID	=M_AntidromicTriggeringChanID;
		pChan->M_IsAntidromicStim			=M_IsAntidromicStim;
		pChan->M_WhichAntidromicMode				=M_WhichAntidromicMode;
		pChan->m_pSamplingCounter			=&m_SamplingCounter; //All channels share the same sampling counter as a means of measuring time.
	}
	pChan0ThreadCounter=&(AiChan[0].ChanSamplCounter);

	//########## configure the NI board clock
	int result;
	if(m_NumOfBoards>0){
		result=DAQmxCfgSampClkTiming(m_pTaskHandle_AI,""
							#ifdef _M_X64 //x64
							,(float64)(*M_SamplingRate)/*Samples/sec*/
							#else _M_X86  // x32
							,(*M_SamplingRate)/*Samples/sec*/
							#endif							
							,DAQmx_Val_Rising/*start reading at the rising phase of the clock pulse*/
							,DAQmx_Val_ContSamps/*continuous sampling*/
							,(*M_SamplesPerRead)/*sampls per channel*/
							 );
		DAQmxRegisterEveryNSamplesEvent(m_pTaskHandle_AI,DAQmx_Val_Acquired_Into_Buffer,(unsigned __int64)(*M_SamplesPerRead),0,EveryNCallback,NULL);
		DAQmxRegisterDoneEvent(m_pTaskHandle_AI,0,DoneCallback,NULL);
	}
	else result=-1;

	//####### Some initialization
	m_ArraySize_RawData=m_NumAiChannels*(*M_SamplesPerRead);
	m_dRawData.SetSize(m_ArraySize_RawData);
	for(i=0; i<MAX_DO_NUM-1;i++){M_DO_Array[i]=RELAY_OFF;}//First turn off all RELAY switches.
	M_DO_Array[DO_STIM_TRIGGER_CHAN]=0;//The channel of index 8 is for the stimulation, which is not via the RELAY in my (Simon's) system.


	//####### implementation of the global variables (define these after all of the above)
	g_pView=this;
	g_rawAI=&(m_dRawData[0]);//the global variable
	g_pInitChan=AiChan;
	g_numAiChan=m_NumAiChannels;
	m_beginValInMicS=(__int64)(m_beginVal*m_1000000perCPU); 
	g_beginValInMicS=m_beginValInMicS; 
	g_PrvTime2RetrieveData=0;// == g_beginValInMicS-g_beginValInMicS;
	g_Inter_DataRetrival_Interval_inMicroSec=M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec;
	g_1000000perCPU=m_1000000perCPU;
	g_LatestTimeStampInMicroS=M_LatestTimeStampInMicroS;
	g_MaxGitterInTime=0;
	g_ArraySize_RawData=m_ArraySize_RawData;
	g_SamplesPerRead=*M_SamplesPerRead;
	g_firstAIindex=&(AiChan[0].m_AiIdx);
	g_NumRead=0; g_SamplingEpochInMicS=*M_SamplingEpoch_us; g_SampleBehaviorAtEveryXmicS=1000000/BEHAVIOR_DATA_SAMPLING_RATE;
	g_SampTimeArrayInMicS=M_SampTimeArrayInMicS;
	g_SampTimeArrayIdx=M_SampTimeArrayIdx; *g_SampTimeArrayIdx=0;
	g_pTaskHandle_DO=&m_pTaskHandle_DO;
	g_DO_Array=&M_DO_Array[0];
	g_DI_Array=&M_DI_Array[0];
	g_SomeChanBoxInfoChanged=M_HeyDASsv_SomeChanBoxInfoChanged;
	g_SamplingCounter=&m_SamplingCounter;
	g_NumOfFigsUsingThisChan=M_NumOfFigsUsingThisChan;
	g_ThreadLaggingCounter=M_Variable_For_Anything;
	g_Flip_AI_SignalSign=M_Flip_AI_SignalSign;

	//########################################################
	//						Digital Output (Write) 
	//########################################################
	if(m_NumOfBoards>0){
		char ChanName[512]; 
		m_pTaskHandle_DO=0;
		DAQmxCreateTask("",&m_pTaskHandle_DO);
		sprintf(ChanName,"%s/line0:7"   ,m_BoardName[m_BoardIdxUsed]);//== First SCB-68: "%s/port0/line0:7"
		DAQmxCreateDOChan(m_pTaskHandle_DO,ChanName,"",DAQmx_Val_ChanForAllLines);
		sprintf(ChanName,"%s/line32:39" ,m_BoardName[m_BoardIdxUsed]);//== First SCB-68: "%s/port1/line0:7"
		DAQmxCreateDOChan(m_pTaskHandle_DO,ChanName,"",DAQmx_Val_ChanForAllLines);
		sprintf(ChanName,"%s/line40:47" ,m_BoardName[m_BoardIdxUsed]);//== First SCB-68: "%s/port2/line0:7"
		DAQmxCreateDOChan(m_pTaskHandle_DO,ChanName,"",DAQmx_Val_ChanForAllLines);
		DAQmxStartTask(m_pTaskHandle_DO);
	}
	//########################################################
	//						Digital Input (Read) 
	//########################################################
	if(m_NumOfBoards>0){
		char ChanName[512]; 
		m_pTaskHandle_DI=0;
		DAQmxCreateTask("",&m_pTaskHandle_DI);
		sprintf(ChanName,"%s/line8:15" ,m_BoardName[m_BoardIdxUsed]);//== Second SCB-68: "%s/port0/line0:7"
		DAQmxCreateDIChan(m_pTaskHandle_DI,ChanName,"",DAQmx_Val_ChanForAllLines);
		sprintf(ChanName,"%s/line24:31" ,m_BoardName[m_BoardIdxUsed]);//== Second SCB-68: "%s/port2/line0:7"
		DAQmxCreateDIChan(m_pTaskHandle_DI,ChanName,"",DAQmx_Val_ChanForAllLines);
		sprintf(ChanName,"%s/line16:23" ,m_BoardName[m_BoardIdxUsed]);//== Second SCB-68: "%s/port1/line0:7"
		DAQmxCreateDIChan(m_pTaskHandle_DI,ChanName,"",DAQmx_Val_ChanForAllLines);
		//int k=DAQmxCfgChangeDetectionTiming(m_pTaskHandle_DI, ChanName,ChanName,DAQmx_Val_FiniteSamps,1);
		DAQmxStartTask(m_pTaskHandle_DI);
	}
	//########### DAQmx AI START Code
	if(m_NumOfBoards>0)DAQmxStartTask(m_pTaskHandle_AI);

	//########### Thread for Digital Output
	m_hDigitalOutThread=CreateThread(NULL,0,Digital_Out,this,0,NULL);

	//########### The following needs to come after the Digital_Out thread launch, otherwise the IO would not work properly. This is because the event objects need to synchronize with the ones in TAS ==> Not sure why.
	*M_isDAS_Running=1; //Starting now!

	//DAQmx is not running, simulate!!
	if(result!=0){
		m_hSimulatedDAQmxThread=CreateThread(NULL,0, SimulatedDAQmx,this,0,NULL);
	}

    m_EvStopProcess	=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,NULL/*use name?*/);
	m_hCheckToEnd=CreateThread(NULL,0, CheckToEndDASsv,this,0,NULL);
	//########################################################
	//############ Anti-/Ortho-dromic Stimulation ############
    *M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,"EVENT_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation");
    m_Ev_WakeUp_ForAntidromicStimulation=CreateEvent(NULL/*A pointer to a SECURITY_ATTRIBUTES structure*/,FALSE/*manually reset?*/,FALSE/*Event set initially?*/,NULL/*use name?*/);
	Anti_Ortho_Stimulation_Loop(); //When this is activated, DAS_sv_main() and EveryNCallback() should not both be forced to run on the same core using SetThreadAffinityMask(), because,  this will prevent one of them from running.
	//########################################################
}

//This thread is to inform Anti_Ortho_Stimulation_Loop() to get up and quit.
void CDasView::Check_to_End_DASsv()
{
	while(1){
		if(*M_Blip_OFF==1){
			SetEvent(m_EvStopProcess);//To make the DAS server (Anti_Ortho_Stimulation_Loop()) wake up and quit.
			SetEvent(m_EvStopProcess);//To make the DAS server (Anti_Ortho_Stimulation_Loop()) wake up and quit.
			SetEvent(m_EvStopProcess);//To make the DAS server (Anti_Ortho_Stimulation_Loop()) wake up and quit.
			break;
		}
		Sleep(5);
	}
}

void CDasView::Simulated_DAQmx()
{
	//{//This is to make the sampling interval equal to 1ms to use Sleep(1). Without this modification, SleepAccurate_inMicS(*M_SamplingEpoch_us) needs to be used which slows down some computers.
	//	*M_SamplesPerRead=(int)(DATA_SAMPLING_RATE*0.001); g_SamplesPerRead=*M_SamplesPerRead;
	//	*M_SamplingEpoch_us=(int)(1000000*(*M_SamplesPerRead)/(float)(*M_SamplingRate)); g_SamplingEpochInMicS=*M_SamplingEpoch_us;
	//	*M_microSecPerPoint = *M_SamplingEpoch_us/(*M_SamplesPerRead);
	//	if(*M_SamplingEpoch_us!=1000)AfxMessageBox("*M_SamplingEpoch_us is not 1ms.");
	//	m_ArraySize_RawData=m_NumAiChannels*(*M_SamplesPerRead); g_ArraySize_RawData=m_ArraySize_RawData;
	//	m_dRawData.SetSize(m_ArraySize_RawData);
	//}

	g_IsSimulatedDAQmx=1;
	char MessageStr[1024];
	strcpy(MessageStr,"NI DAQmx is not running\n It is being SIMULATED.");
	AfxMessageBox((char*) MessageStr);

	m_SimulateNeuron=0; m_SimulateBehavior=0; g_SimulationCounter=0; g_SimulationCounter2=0;
	while(m_bIsON==1){
		if(m_SimulateNeuron>INT_MAX-2){m_SimulateNeuron=0; m_SimulateBehavior=0;}
		EveryNCallback(m_pTaskHandle_AI, 0,0,0);
		//Sleep(1);

		if(g_SimulationCounter < 20){
			if(g_SimulationCounter==0)g_SimulationCounter2=0;
			m_SimulateNeuron=0; g_SimulationCounter++; 
		}
		else{
			if(g_SimulationCounter2 < 1){g_SimulationCounter2++;}
			else g_SimulationCounter=0;
		}

		//Sometimes, this computationally expensive function interacts with other programs such as playing a sound.
		SleepAccurate_inMicS(*M_SamplingEpoch_us);
	}
}

long CVICALLBACK EveryNCallback(TaskHandle taskHandle, long everyNsamplesEventType, unsigned long nSamples, void *callbackData)
{
	//You may force this thread to run on a core specified by the mask in the following line of command to use the same time counter attached to one core to query the high performance ticks (QueryPerformanceCounter)
	//I don't think it is necessary, because, at least intel x86 CPUs seem to share the same "mother board clock?" to provide high performance time.
	//Refer to "Time Stamp Counter" in Wikipedia, and "Game Timing and Multicore Processors" in online MSDN.
	//SetThreadAffinityMask(GetCurrentThread(), g_threadAffMask);//This command forces this thread to run on the same core as the core used by DAS_sv_main(). //Don't do this.
	
	long       error=0;
	long       read=0;
	int		IsBehaviorAItoRead;

	__int64 newVal,timeStamp,chanIdx, idx,rawIdx, relIdx, endIdx;
	QueryPerformanceCounter((LARGE_INTEGER*)&newVal); //Microsoft: On a multiprocessor computer, it should not matter which processor is called. However, you can get different results on different processors due to bugs in the basic input/output system (BIOS) or the hardware abstraction layer (HAL). To specify processor affinity for a thread, use the SetThreadAffinityMask function.//BUT don't do this.
	timeStamp=(__int64)(newVal*g_1000000perCPU-g_beginValInMicS);//Time in microS
	*g_LatestTimeStampInMicroS=timeStamp;

	//Using "g_TimeStepPerPointInThisReadingInterval", the time, at least, does not go backward.
	g_TimeStepPerPointInThisReadingInterval=(__int64)((double)(timeStamp-g_PrvTime2RetrieveData)/(double)g_SamplesPerRead);
	if(g_TimeStepPerPointInThisReadingInterval<0){AfxMessageBox("Time went backward!! The clock of the computer is not compatable with Blip.");}

	//To see the jitter of sampling intervals
	g_SampTimeArrayInMicS[*g_SampTimeArrayIdx]=timeStamp;
	if(*g_SampTimeArrayIdx+1<MAX_SPIKEs)(*g_SampTimeArrayIdx)++; else *g_SampTimeArrayIdx=0;
	
	//### NO overlapping!!
	EnterCriticalSection(&(g_pView->m_CriticalSectionDAQ));
	g_pView->m_BytesPerSample;
	//
	*g_Inter_DataRetrival_Interval_inMicroSec=timeStamp-g_PrvTime2RetrieveData;

	//This variable is to see if the channel's thread is keeping up with this call-back's request.
	if(g_NumOfFigsUsingThisChan[0]>0)*g_ThreadLaggingCounter=(samplCounter++)-(*pChan0ThreadCounter); //This one is always "0"!!!
	//################  Simulation of DAQmx
	if(g_IsSimulatedDAQmx==1){
		read=g_SamplesPerRead;
		int simIsBehaviorAItoRead;
		if((g_NumRead+1)*g_SamplingEpochInMicS>=g_SampleBehaviorAtEveryXmicS){simIsBehaviorAItoRead=1; } else simIsBehaviorAItoRead=0;
		for(chanIdx=0, rawIdx=0; chanIdx<g_numAiChan; chanIdx++){
			if(chanIdx<NUM_NN_AI_CHAN){//The neural channels occupy the channel indexes of 0~(NUM_NN_AI_CHAN-1).
				if(g_NumOfFigsUsingThisChan[chanIdx]>0){//If there is any client using this channel,
					if(g_SimulationCounter2>0){
						for(idx=0; idx<read; idx++,rawIdx++){
							g_rawAI[rawIdx]=-sin(++m_SimulateNeuron*0.23);
						}
					}
					else{
						for(idx=0; idx<read; idx++,rawIdx++){
							g_rawAI[rawIdx]=-0.2;
						}
					}
				}
				else rawIdx+=read;
			}
			else if(simIsBehaviorAItoRead){
				g_rawAI[rawIdx+(read-1)]=0.2*sin(++m_SimulateBehavior*0.003+chanIdx*1.)-0.1;
				rawIdx+=read;
			}
		}
	}
	//##############   Real data acquision by DAQmx
	else{
		// DAQmx Read Code
		DAQmxReadAnalogF64(taskHandle
			,g_SamplesPerRead/*N samples per channel*/ 
			,10.0/*time out 10 sec*/ 
			,DAQmx_Val_GroupByChannel//file mode (or  DAQmx_Val_GroupByScanNumber)
			,g_rawAI /*space to read in*/
			,g_ArraySize_RawData/*array size in samples*/ 
			,&read/*The actual number of samples read from each channel*/
			,NULL/*Reserved for future use*/
			);
	}

	//############## Distribute the retrived data.
	if( read>0 ) {
		//This is used to identify "M_usableAiIdx" of each channel at one point of time. It is to conveniently identify the index of data when a stimulation happens.
		int *UsableAiIdxArray=g_pView->m_UsableAiIdx_atTheTimeOfDataRetrival[g_pView->m_ArrayIndex_forUsableAiId]; 
		//Is it time to sample behavioral data?
		if(++g_NumRead*g_SamplingEpochInMicS>=g_SampleBehaviorAtEveryXmicS){IsBehaviorAItoRead=1; g_NumRead=0;} else IsBehaviorAItoRead=0;

		//Analog Input
		//#### Non-interleaved data ####
		for(chanIdx=0, rawIdx=0; chanIdx<g_numAiChan; chanIdx++){
			g_pChan=(g_pInitChan+chanIdx); //The address of the channel class, CAiChannel
			g_AI=g_pChan->M_AI;
			g_timeStampMicS=g_pChan->M_timeStampMicS;
			if(chanIdx<NUM_NN_AI_CHAN){//The neural channels occupy the channel indexes of 0~(NUM_NN_AI_CHAN-1).
				if(g_NumOfFigsUsingThisChan[chanIdx]<1){rawIdx+=read; continue;} //If no client is using this channel,
				endIdx=g_pChan->m_AiIdx+read;
				for(idx=g_pChan->m_AiIdx,relIdx=1; idx<endIdx; idx++,relIdx++,rawIdx++){
					g_AI[idx]=(*g_Flip_AI_SignalSign)*g_rawAI[rawIdx]; //This direct mapping (without considering the loop of the array), (with the help of the "if" clause below) is possible because *M_AIQueueSize[i] is a multiple of *M_SamplesPerRead. If not, Init_MemoryMapVariables() exits the program, after giving a warning.
					g_timeStampMicS[idx]=g_PrvTime2RetrieveData+relIdx*g_TimeStepPerPointInThisReadingInterval;//Time in microS
				}

				//Increase the indexes				
				//if(g_pChan->m_AiIdx+read> *(g_pChan->M_AIQueueSize))AfxMessageBox("g_pChan->m_AiIdx> *(g_pChan->M_AIQueueSize)");
				if(g_pChan->m_AiIdx+read<*(g_pChan->M_AIQueueSize)){
					g_pChan->m_AiIdx+=read;
				}
				else g_pChan->m_AiIdx=0;
				*(g_pChan->M_usableAiIdx)=g_pChan->qIdx(g_pChan->m_AiIdx-1);

				//It is to conveniently identify the index of data when a stimulation happens. This information is relayed to the neural channel. 
				//And when the channel requests an antidromic stimulation, it hands over this info to Anti_Ortho_Stimulation_Loop() --> Set_AntiDO_forXms_Function(), 
				//where this information is used to calculate M_EstimatedAntidromicStimPointIdx[ChanID], which is then used by ANTS to locate the AI index of antidromic stimulation.
				UsableAiIdxArray[chanIdx]=*(g_pChan->M_usableAiIdx); 

				#ifdef ZAPPER
				//################# Zapper: This operation may happen from 0~0.5ms after the triggering of the stimulation
				//Learn (Learing needs to come before the zapping)
				if(g_pView->m_NumOfArtifactDataPoints_toLEARN>0){//NEED to learn
					//if(chanIdx==*(g_pView->M_AntidromicTriggeringChanID)){ //If this recording channel is the antrdromic recording channel,
						//Every neural channel needs to learn
						g_pView->Zapper_Learn(g_pChan,g_AI);
					//}
				}
				//Zap!!
				if(g_pView->m_NumOfArtifactDataPoints_toZAP>0){//NEED to Zap
					//if(chanIdx==*(g_pView->M_AntidromicTriggeringChanID)){ //If this recording channel is the antrdromic recording channel,
					//Every neural channel needs to zap
						g_pView->Zapper_Zap(g_pChan,g_AI);
					//}
				}
				//##################
				#endif

				//Serial computation seems to be faster and more reliable than the MESSAGE-based thread, which I had implemented before.
				//When I implemented a true tread at here (instead of the MESSAGE-based thread), DAS_sv became overwhelmed and lagging behind the data feed.
				g_pChan->OnAnalogInput(*(g_pChan->M_usableAiIdx), g_pView->m_ArrayIndex_forUsableAiId);
			}
			//Behavioral input
			else if(IsBehaviorAItoRead){// This "if()" makes sure that this conforms the sampling rate given by *M_BehaviorSamplingRate)
				//This part contains "2D behavior AI" AND the "OTHER AI" data.
				//The behavior channels occupy the channel indexes of (NUM_NN_AI_CHAN)~(NUM_NN_AI_CHAN+NUM_2D_BHVR_AI_CHANS-1).
				//The OTHER channels occupy the channel indexes of (NUM_NN_AI_CHAN+NUM_2D_BHVR_AI_CHANS)~.
				//The sampling rate is given by BEHAVIOR_DATA_SAMPLING_RATE; it reads just the latest one.
				g_pView->M_timeStamp4BehaviorAI_MicS[g_pChan->m_AiIdx]=timeStamp;
				g_AI[g_pChan->m_AiIdx]=g_rawAI[rawIdx+(read-1)];
				rawIdx+=read;
				//Increase the indexes
				if(g_pChan->m_AiIdx+1<*(g_pChan->M_AIQueueSize)){
					g_pChan->m_AiIdx++;
				}
				else g_pChan->m_AiIdx=0;
				*(g_pChan->M_usableAiIdx)=g_pChan->qIdx(g_pChan->m_AiIdx-1);
			}
		}
		if(g_pView->m_ArrayIndex_forUsableAiId+1<MAX_USABLE_AI_IDX_ARRAY)(g_pView->m_ArrayIndex_forUsableAiId)++; else g_pView->m_ArrayIndex_forUsableAiId=0;//This is used to identify "M_usableAiIdx" of each channel at one point of time. It is to conveniently identify the index of data when a stimulation happens.

		//DAQmxCfgSampClkTiming() sets the data sampling rate (DATA_SAMPLING_RATE, 40kHz) and per-Sample-Reading (SAMPLES_PER_READ, 20; meaning every 0.5 ms the data stored by NI at 40kHz will be retrieved). 
		//So, I assume that DI also gets 20 data points available at 40kHz. But here I am choosing to sample only the first data point out of 20 available from the NI board.
		//In summary, AI's data sampling rate is that of NI's sampling rate (DATA_SAMPLING_RATE, 40kHz) with 2kHz retrival frequency. 
		//For DI, the data sampling rate is the same as the data retrival rate (2kHz) since it reads only one data point out of the 20 available NI's stored data.
		//Also, DI does not record the history unlike the AI.
		DAQmxReadDigitalLines(g_pView->m_pTaskHandle_DI,SAMPLES_PER_READ_DI/*# of samples/channel; can be upto SAMPLES_PER_READ*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,g_DI_Array/*array to read*/,MAX_DI_NUM ,NULL,NULL,NULL/*For future use*/);
	}
		
	//############ Channel config change by the client ##############
	//Check if there is any output-request of DO or channel config change
	//Antidromic/Orthodromic stimulation does NOT use this. It uses its own loop {Antidromic_Stimulation_EventBased() or Anti_Ortho_Stimulation_Loop()} to process the output.
	if(*g_SomeChanBoxInfoChanged==1){
		*g_SomeChanBoxInfoChanged=0;
		g_pView->PostMessage(WM_MYMESSAGE_CHAN_CONFIG, 0,0);
	}
	//Antidromic time counter: How frequently do you want to give an antidromic stimulation? 
	if(*g_SamplingCounter>0){
		(*g_SamplingCounter)--;
	}
	//###############################

	//Error handling
	if( DAQmxFailed(error) || g_pView->m_bIsON==0) {
		*(g_pView->M_isDAS_Running)=0;//Broadcast the end of data acquisition
		SetEvent(g_pView->m_EvStopProcess);

		char       errBuff[2048]={'\0'};
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
		//printf("DAQmx Error: %s\n",errBuff);
	}
	g_PrvTime2RetrieveData=timeStamp;
	
	//No overlapping!!
	LeaveCriticalSection(&(g_pView->m_CriticalSectionDAQ));
	return 0;
}

long CVICALLBACK DoneCallback(TaskHandle taskHandle, long status, void *callbackData)
{
	CMainFrame * pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CDasView * pView = (CDasView *)pFrame->GetActiveView();
	*(pView->M_isDAS_Running)=0;
	pView->m_bIsON=0;
	SetEvent(g_pView->m_EvStopProcess);
	AfxMessageBox("Unexpectedly stopped");
	return 0;
}

//In case there has been a request of a Digital Output
// or, a notice has been received saying that some channal configuration has been changed
LRESULT	CDasView::OnChangeOfChanConfig(WPARAM a, LPARAM b)
{
	int i;
	for(i=0; i<NUM_NN_AI_CHAN;i++){	
		if(M_ChanBoxInfoChanged[i]==1){
			AiChan[i].UpdateVTBoxes();
		}
	}
	return NULL;
}

void CDasView::DigitalOut()
{
	int	nEventIdx;
	HANDLE Events[MAX_DO_NUM+1]; 
	Events[MAX_DO_NUM]= m_EvStopProcess;
	for(int i=0; i<MAX_DO_NUM; i++){
		Events[i]= m_DO_Active[i];
	}
	
	while(1){
		nEventIdx=WaitForMultipleObjects(MAX_DO_NUM+1,Events,FALSE/*wait for all?*/,INFINITE/*timeout*/);
		if(nEventIdx==MAX_DO_NUM)break;//return;
		
		if(nEventIdx!=-1)DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL);//For future use
	}
	
	//Turn ALL RELAY OFF
	for(int i=0;i<MAX_DO_NUM; i++){	
		M_DO_Array[ i ]=RELAY_OFF; //The current gating signal for the stimulation signal via the RELAY.
	}
	DAQmxWriteDigitalLines(m_pTaskHandle_DO,1/*# of samples/channel*/,1/*automatically start?*/,1.0/*time out in sec*/,DAQmx_Val_GroupByChannel/*not interleaved*/,M_DO_Array/*data array to write*/,NULL,NULL/*For future use*/);
}

void CDasView::SleepAccurate_inMicS(int FixedDur_inMicS)
{
	if(FixedDur_inMicS<=0)return;
	__int64 newVal,lastVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastVal);
	if(FixedDur_inMicS>=ACCURACY_OF_SLEEP_PLUS1_IN_MICSEC){
		while(1){
			Sleep(1);
			if(FixedDur_inMicS-HighPerformanceTime(lastVal)<ACCURACY_OF_SLEEP_IN_MICSEC)break;
			if(!m_bIsON)return;
		}
	}
	//Fine delay
	while(1){
		if(HighPerformanceTime(lastVal)>FixedDur_inMicS+2)break;
		if(!m_bIsON)return;
	}
}

double CDasView::HighPerformanceTime(__int64 lastVal)//1 means 1ms, 0.001==1 MicS
{
	__int64 newVal;
	QueryPerformanceCounter((LARGE_INTEGER*)&newVal);
	return (1000000.0 * (double)(newVal-lastVal)/ m_CPUfrq);
}





