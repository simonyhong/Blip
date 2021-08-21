#if !defined (MOXY_CONSTANTS)
#define MOXY_CONSTANTS

#include "SharedConstants.h"
#include "SharedConst_TASMOXYVIS.h"
#define VERSION_NUM_OF_MOXY 1.00
//##########################################################################################

#define DIO_DISP_Y_POSITION(y) (0.95-0.5*((2*y)/MaxDIO)) 
#define DIO_DISP_X_POSITION(x) (x*2-MaxDIO*((2*x)/MaxDIO)+0.5) 
//###########################  DISPLAY EVENTs  ##############################################
#define DIO_ON					1
#define DIO_OFF					0
#define REFRESH_SCREEN			2
#define REDRAW_ALL_FIGURES		3
#define REDRAW_RUNNINGLINE_FIG  4
#define REDRAW_XY_WINDOW_FIG	5
#define REDRAW_DIO_FIG			6
#define DISP_TRIAL_NUM			7
#define ANTIDROMIC_STIM_HAPPENED		8
#define TOGGLE_TRIGGER_STATUS			9
#define TOGGLE_SUPERIMPOSITION_STATUS  10
#define DISP_RECORDING_SOURCE  11
#define DISP_LABELS_OF_USER_PICKED_VAR  12
#define DRAW_USER_RIGHT_CLICK_POS      13
#define DECREASE_NEURON_NUM     14
#define INCREASE_NEURON_NUM     15
#define REFRESH_XY_TITLE_AREA   16
#define DISP_FILE_NAME_AND_RECORDING_STATUS			17
#define DECREASE_RWD		    18
#define INCREASE_RWD			19
#define UPDATE_RWD				20
#define DECREASE_B_CHAN		    21
#define INCREASE_B_CHAN			22
#define DISP_POSITION_BOX  100

//########### Display #########
#define MAX_DISP_REQUESTS 100
#define	USE_LEFT_AXIS	0
#define	USE_RIGHT_AXIS  1
#define	BACKGROUND_PEN_WIDTH  1
//###########################  Figure IDs  #################################################
#define DIO_FIG				0
#define XY_FIG				1
#define FIRST_RUNNINGLINE_FIG	2


#define MaxDC		NUM_RL_PANELS
#define MAX_STIM_OBJ 100

//########### XY Display #########
#define MAX_EYE_GRID_MODES 2

//########### DIO #########
#define MaxDIO 24

//########### File #########
#define TOREAD		1	
#define NOT_TOREAD	0	
#define APPEND_FILE		0	
#define OVERWRITE_FILE	1	

#endif //Of (MOXY_CONSTANTS)

