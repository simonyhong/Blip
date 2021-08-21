#if !defined (TASK_CONSTANTS)
#define TASK_CONSTANTS
//######################    Task  IDs       ################################################
//######################    Task  IDs       ################################################
//Please keep your function IDs somewere in this header file.
#define ONEDR_TASK			0
#define SCC_TASK			1
#define MY_NEW_FIX_TASK     2
#define ORTHO_STIM_TASK		3
//##########################################################################################
//######################    EVENT CODES     ################################################
//There is NO reserved event code. Use any integer as you wish
#define TASK_START       	0
#define SHOWFIXCD	    	2
#define FIXSTARTCD	    	3		
#define FIXOFF		    	5		
#define TGTCD               6
#define SAC_OCCURRED		7
#define PRE_RWDCD			9
#define RWDCD			   	10
#define RWDOFFCD		  	11
#define CORRECTCD	  	    12
#define ERRCD			  	13
#define NOFIX      			14
#define FAILED_SAC			15
#define AWAY_LOOK_TRL		16
#define ERR_BEEP			17
#define STIM_CD				19
#define RND_RWDCD		   	110
#define RND_RWDOFFCD	   	111
//######################    EYE POSITION BOXES   ############################################
#define posBOX0 0
#define posBOX1 1
#define posBOX2 2
#define posBOX3 3
#define posBOX4 4

//######################    Global functions    ############################################
UINT CATCH_WIND(LPVOID WinObjPtr); //A global Function
UINT ANTI_SHAKING(LPVOID WinObjPtr); //A global Function

//######################    Some useful definitions    ################################################
#define RWD					0
#define CORRECT_BEEP		1
#define WRONG_BEEP			2
#define INPUT_FROM_RELAY	6

//######################    Others    ################################################
#define STOP_LOOP -1 //This pauses the subroutine

#define HemiCenter -486
#define HemiSize 1300
#define USER_RIGHT_CLICK 3
#define MAX_EYE_GRID_MODES 3

#define BOX_ON 1
#define BOX_OFF 0
#define WD_DIRPOS 0
#define WD_SIGNAL 1
#define STOP_RECORD_ANALOG 0
#define START_RECORD_ANALOG 1
#define OPEN_LFP 1
#define CLOSE_LFP 0

#define PHOTO_DIODE (FIRST_OTHER_CH_DIX+1) //+1 is added to prevent the roll over of the signal from the previous channel (the previous channel can be grounded to prevent this)

#endif