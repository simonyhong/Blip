#if !defined (BLIP_VERSION)
#define BLIP_VERSION	(1.5)

//ATTENTION: ==> After changing the parameters below, RECOMPILE ALL THE RELATED PROGRAMS.
//################ NI Boards ###################
#include "End_Users_Definitions.h"

#define NUM_2D_BHVR_AI_CHANS	(NUM_2D_BEHAVIOR_OBJS*2)	//Total physical 2D behavioral AI channles to monitor.
#define MAX_AI_CHAN				(NUM_NN_AI_CHAN+NUM_2D_BHVR_AI_CHANS+NUM_OTHER_AI_CH)
#define FIRST_2D_BHVR_AI_CH_IDX (NUM_NN_AI_CHAN)//The first channel index of the behavior AI channel is the next channel of the last neural channel.
#define FIRST_OTHER_CH_DIX		(NUM_NN_AI_CHAN+NUM_2D_BHVR_AI_CHANS)//The first channel index of the OTHER AI channel is the next channel of the last 2D behavioral channel.

//###############  Memory Map  ##################
#define DATA_SAMPLING_RATE			40000       //40 KHz
#define SAMPLES_PER_READ			20			//Read every 0.5 ms (==500uS). This constant is used just to set the variable *M_SamplesPerRead. This is to make sure that when there is no NI board installed, *M_SamplesPerRead value needs to be modified to simulate the board.
#define BEHAVIOR_DATA_SAMPLING_RATE	1000		//1 KHz

//Fixed sizes: Manually tune as needed.
//At_the_End_of_a_Trial_Check_if_MemoryMap_config_meets_the_trials_config() in TAS will give you a warning if your task requires more resource than the one specified by the values below.
#ifdef _M_X64 //x64
#define MAX_TIMESPAN_IN_SEC_STORED_IN_Q	300 //in sec. Must be a positive integer. This will be the maximum duration of a trial since everything should be held in memory before writing in to the file.
#else		 // x32
#define MAX_TIMESPAN_IN_SEC_STORED_IN_Q	100  //in sec. Must be a positive integer. This will be the maximum duration of a trial since everything should be held in memory before writing in to the file.
#endif	
#define AI_Q_SIZE				((__int64)(MAX_TIMESPAN_IN_SEC_STORED_IN_Q*DATA_SAMPLING_RATE))
#define MAX_WAVE_PTRs			((int)((DATA_SAMPLING_RATE*0.5*MAX_TIMESPAN_IN_SEC_STORED_IN_Q)*0.2))// This is about 20% of the theoretical maximum number of waveforms (=DATA_SAMPLING_RATE*0.5**MAX_TIMESPAN_IN_SEC_STORED_IN_Q) at the given sampling frequency. 
#define MAX_SPIKEs				MAX_WAVE_PTRs //This one needs to be big enough not to loose the spikes happend for the trial.
#define ECODE_Q_SIZE			50000

//Dynamically changed in the program. 
#define DEFAULT_SPK_NUM_PER_TRIAL		1000
#define DEFAULT_NUM_OF_ECODES_PER_TRIAL 100
#define DEFAULT_WAVESPAN_PRE_THRESHOLD		0.5 //ms
#define DEFAULT_WAVESPAN_POST_THRESHOLD		4.  //ms
//Some constants
#define MAX_TRLS_IN_MAP			3 //This defines how many trials are kept in the memory map file. Since Blip does not make use of previous trials. This number is not meaningful. After reaching the limit, the oldest one will be replaced by a new trial. 
#define	MAX_NUM_OF_WAVEFORMS_TO_DISP 7 //=Selected + non-selected 
#define SCALE_XY_POS 20 //The scaling factor used in Get_Latest_XYpos() to convert AI signal to variables used in the BLIP 

//########### Relay #######################
#define RELAY_ON  0 //With Grayhill relay, 0 means on. Your relay may be 1 instead 0. Refer to your relay manual.
#define RELAY_OFF 1 //With Grayhill relay, 1 means off.Your relay may be 0 instead 1. Refer to your relay manual.

//#################  Digital INPUT (change this to meet your needs) ####################
#define MAX_DI_NUM			24 //==> NOTE! this is the maximum number allowed! Change this number if you want. Make sure that "DAQmxCreateDIChan()" in the "Digital Input  Registration" in DAS_sv_main() reflects this. This also means: M_DI_Array[0~(MAX_DO_NUM-1)]
#define SAMPLES_PER_READ_DI			1			//SAMPLES_PER_READ_DI out of SAMPLES_PER_READ available

//#################  Digital OUTPUT (change this to meet your needs) ####################
#define MAX_DO_NUM			24 //==> NOTE! this is the maximum number allowed! Change this number if you want. Make sure that "DAQmxCreateDOChan()" in the "Digital Output Registration" in DAS_sv_main() reflects this. This also means: M_DO_Array[0~(MAX_DO_NUM-1)]

//#################  Digital OUTPUT for Stimulation  ####################
//				Dear User: 
//              If you use more than "DO_STIM_TRIGGER_CHAN" (M_DO_Array[0] ~ M_DO_Array[DO_STIM_TRIGGER_CHAN-1]) number of the digital output function, you may want to modify the following "Gating channels" definitions to accommodate your needs.
//				You can shift the channels to other channels and also reduce the number of gating channels.
//				However, (DO_STIM_TRIGGER_CHAN, MAX_NUM_STIM_CHANNELS, DO_FIRST_GATING_CHAN, DO_LAST_GATING_CHAN) definitions below must be defined since Blip has functions that require them. 

//Stimulation and Gating channels 
#define DO_STIM_TRIGGER_CHAN	13 //The channel that gets the TRIGGER signal for the stimulator.
#define MAX_NUM_STIM_CHANNELS	10 //The actuall number of stimulating channels will be limited by the number of gating channels given below.
#define DO_GATING_CHAN1			14 //meaning M_DO_Array[14] is connected to a gating relay  
#define DO_GATING_CHAN2			15 //meaning M_DO_Array[15] is connected to a gating relay  
#define DO_GATING_CHAN3			16 //meaning M_DO_Array[16] is connected to a gating relay  
#define DO_GATING_CHAN4			17 //meaning M_DO_Array[17] is connected to a gating relay  
#define DO_GATING_CHAN5			18 //meaning M_DO_Array[18] is connected to a gating relay  
#define DO_GATING_CHAN6			19 //meaning M_DO_Array[19] is connected to a gating relay  
#define DO_GATING_CHAN7			20 //meaning M_DO_Array[20] is connected to a gating relay  
#define DO_GATING_CHAN8			21 //meaning M_DO_Array[21] is connected to a gating relay  
#define DO_GATING_CHAN9			22 //meaning M_DO_Array[22] is connected to a gating relay  
#define DO_GATING_CHAN10		23 //meaning M_DO_Array[23] is connected to a gating relay  
#define DO_FIRST_GATING_CHAN	DO_GATING_CHAN1 //Put the first gating channel here.
#define DO_LAST_GATING_CHAN		DO_GATING_CHAN10 //Put the last gating channel here.

//########### Event Objects #######################
#define EventObjectsName "EVENT_M_DO_Active_%d"
#define Event_DI_ObjectsName "EVENT_M_DI_Active"

//########### Spike Sorter #######################
#define MAX_NNs_PerCHAN	3
#define MAX_VT_RECTs_PER_NEURON 10
#define ECODE_SELECT	1
#define ECODE_UNSELECT	(-1)
#define TIME_TO_DATA_POINTS(t)	((int)(fabs(t)*0.001*DATA_SAMPLING_RATE))

//########### Antidromic Stimulation  ############
#define AUTO_STIM_MODE		0
#define THRESHOLD_STIM_MODE 1
#define VTBOX_STIM_MODE		2
#define TRIGGER_STIM_AT_EVERY_X_ms	1000// ms
#define ARTIFACT_TIMESPAN_IN_SEC	0.020 // sec
#define ARTIFACT_SIZE				((__int64)(ARTIFACT_TIMESPAN_IN_SEC*DATA_SAMPLING_RATE))

//##########################################################################################
//#define ZAPPER
//##########################################################################################
#define ACCURACY_OF_SLEEP		3 //(in ms). The accuracy of Sleep() function is about -1~2 ms WITH timeBeginPeriod(1) & timeEndPeriod(1) functions; -3 to +15 ms WITHOUT them (as of 2010, september)
#define ACCURACY_OF_SLEEP_PLUS1 4 //ACCURACY_OF_SLEEP + 1 ms
#define ACCURACY_OF_SLEEP_IN_MICSEC 3000
#define ACCURACY_OF_SLEEP_PLUS1_IN_MICSEC 4000

//################ ENABLE the following line for distribution ###############################
#define ENABLE_THIS_FOR_DISTRIBUTION_TO_EXCLUDE_CUSTOM_CODE





#endif //Of (BLIP_VERSION)

