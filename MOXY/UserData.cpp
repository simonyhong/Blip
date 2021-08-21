#include "stdafx.h"
#include "MOXY.h"

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"


void CMOXY_View::InitUserData()
{
	MoxyUserData *pMoxyUserData;
	int sizeOfdata=10;
	for(int i=0; i<SETS_OF_USER_VARIABLE_BLOCKS;i++){
		pMoxyUserData=&M_MoxyUserData[i];
		pMoxyUserData->m_NumUserData=0; 
		pMoxyUserData->m_TotNumUserVlaluesToRecord=0; 
		pMoxyUserData->m_FirstLocationOfUserValue[0]=pMoxyUserData->m_TotNumUserVlaluesToRecord;
	}
}

void CMOXY_View::Name_and_Value_to_record(char Name[], double value) 
{	
	double ArrayOfValues[1]={value};
	Name_and_ArrayOfValues_to_record(Name, ArrayOfValues, 1);
}

void CMOXY_View::Name_and_Value_to_record(char Name[], double V1, double V2) 
{	
	double ArrayOfValues[2]={V1, V2};
	Name_and_ArrayOfValues_to_record(Name, ArrayOfValues, 2);
}

//MOXY generates and holds userVariables for TAS to save at the end of a trial. 
//These variables are generated when the user clicks DIO pannel therefore activating a Digital Output channel.
//When that happens, "Name_and_ArrayOfValues_to_record("User-clicked DO:",m_UserClickedDIOIdx)" is called in WhichPane().
//Causing MOXY to record the ID of the DO channel and time when it occurred.
//During one trial, MOXY can record SIZE_OF_USER_DATA times of user variables.  
//The Name of the following function is limited to MAX_CHARLEN_SIZE_OF_USER_DATA, and it accepts only one integer.
void CMOXY_View::Name_and_ArrayOfValues_to_record(char Name[], int ArrayOfValues[], int N)
{	
	#include "Name_and_ArrayOfValues_to_record.h"
}

void CMOXY_View::Name_and_ArrayOfValues_to_record(char Name[], float ArrayOfValues[], int N)
{
	#include "Name_and_ArrayOfValues_to_record.h"
}

void CMOXY_View::Name_and_ArrayOfValues_to_record(char Name[], double ArrayOfValues[], int N)
{
	#include "Name_and_ArrayOfValues_to_record.h"
}


