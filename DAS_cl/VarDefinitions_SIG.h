#if !defined (VAR_DEFINITIONS)
#define VAR_DEFINITIONS

#include "SharedConstants.h"
#define dt 0.01 	//Let's assume: t of 100 equals 1000ms. ( dt of 0.01 = 0.1 ms, or 1 = 10ms)
#define dtx0_5 0.002
#define SAMPLE_FRQ 30 //Sample the data and display every 30 iteration (not in time)
#define TH4AveBSD 0.03
#define MaxDC 1

#define SAC_TASK 1
#define FIX_TASK 2
#define OVR_TASK 3
#define SimDuration    300 // 3s=1sec (task) +2sec (rest)
#define EndOfDisplay    1000 // 1sec just for display setting
#define TGT_ONSET    10 // 100 ms
#define TGT_OFF      20 // 200 ms

#define trTau 0.001 //dt of 0.01*(1/"10"),  "10" ==>100 ms
#define CTxtrTau 0.01//
#define DAtrTau 0.03
#define slowT 0.00001
#define SimBeginT 0
#define centerX (X_resol/2)
#define centerY (Y_resol/2)
#define P_TH2GSD 0.25
#define SC_SacTH  0.7 //Eq. (26):  0.6
#define SC_TH4Fo  0.5 //Eq. (22):  0.4
#define SC_TH4Fp  0.4 //Eq. (15):  0.25.  This means Pxy-->SC-->Fp can drive the eye
#define SC_TH4GSI 0.  //Eq. (30):  0
							 //In Brown04, 
#define Fo_act_TH  0.4       //0.4 was used as a Hill's half point to excite P, SC, GSD and for all lateral competitions within Fo; 
#define Fo_act_TH4BSTN  0.5	 //0.5 was used to excite BSTN; 
#define Fo_act_TH4Rcrr  0.52	 //0.6 was used to suppress the Fo11 fixation-related neurons; 
							 
#define Th_TH4Fo 0.5  //Eq. (30):  0
#define FPAtr_TH 0.3
#define BSItr_TH 0.3

#define REC_TOP 0
#define REC_LEFT 1
#define REC_BOTTOM 2
#define REC_RIGHT 3

#define ToDataCoordSpkWV(i) (((double)(i)/((*M_SamplingRate)*0.001))+pFig1->m_dMinX) //conver index to the data coordinate

//###############     User Message    ####################
#define WM_MYMESSAGE_NEW_AI (WM_USER + 1)
#define WM_MYMESSAGE_NEW_SIG (WM_USER + 2)
#define WM_MYMESSAGE_NEW_DISP_EVENT (WM_USER + 3)
#define WM_MYMESSAGE_REFRESH_FRAME (WM_USER + 4)

//###############     Mouse+    ####################
#define LEFT_EDGE			0     
#define TOP_EDGE			1     
#define RIGHT_EDGE			2     
#define BOTTOM_EDGE			3     
#define LEFT_TOP_CORNER		4     
#define RIGHT_TOP_CORNER	5     
#define RIGHT_BOTTOM_CORNER	6     
#define LEFT_BOTTOM_CORNER	7     
//######################
typedef void *TaskHandle; 

//Dispay mode message
#define RESIZING_WIN	1	
#define RENAMING_TITLE	2	
#define RENAMING_FIG	3	
#define RESCALING_AXES	4	
#define DISP_FILE_NAME	5	
#define REFRESH_SCREEN	6	
#define DISP_Y_SCALE	7	

//########### File #########
#define TOREAD		1	
#define NOT_TOREAD	0	
#define APPEND_FILE		0	
#define OVERWRITE_FILE	1	

//Screen
#define ViewMargin4FigsAlongEdges 1//This will let some room for the figures along the left and bottom edge of the view to display properly.

//###########  Simulated Data acquisition
#define WAVE_FRQ 1000

#endif //Of (VAR_DEFINITIONS)


