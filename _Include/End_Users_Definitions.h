#if !defined (END_USER_DEFINITIONS)
#define END_USER_DEFINITIONS

//###########  Dear End Users, Define your channel configurations in the following         ################################################
//###########  After changing the parameters below, RECOMPILE ALL THE RELATED PROGRAMS.



//How many analog input neural channels do you want?
#define NUM_NN_AI_CHAN			2	

//How many analog input 2D behavioral objects (e.g. 1 eye with x, y channels) do you want to monitor?
//One object occupties 2 analog channels. Blip can monitor these objects and plot in the X-Y coordinate in realtime in MOXY.
#define NUM_2D_BEHAVIOR_OBJS	2	

//How many OTHER AI channels do you want? (e.g., photo diode, actuator positions...)
//Any voltage changes can be monitored with this.
#define NUM_OTHER_AI_CH			2	















//###########  End of definitions #########################################################################################################
#endif //Of (END_USER_DEFINITIONS)