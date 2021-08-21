	




	if(*M_MoxyUserDataSetID>=SETS_OF_USER_VARIABLE_BLOCKS || *M_MoxyUserDataSetID<0)return; //TAS does not seem to be running.
	MoxyUserData *pMoxyUserData=&M_MoxyUserData[(*M_MoxyUserDataSetID)];
	if(SIZE_OF_USER_DATA-1<=pMoxyUserData->m_NumUserData){
		if(m_isFirstMessage==1){
			AfxMessageBox("Too many user variables\nIncrease SIZE_OF_USER_DATA in SharedConst_TASMOXYVIS.h");
			m_isFirstMessage=0;
		}
		return;
	}
	if(strlen(Name)*sizeof(char)+1>MAX_CHARLEN_SIZE_OF_USER_DATA){
		AfxMessageBox("Too long name");
		return;
	}
	pMoxyUserData->m_TimeOfOccurrence[pMoxyUserData->m_NumUserData]=*M_LatestTimeStampInMicroS;

	//Values
	int FirstValID=pMoxyUserData->m_FirstLocationOfUserValue[pMoxyUserData->m_NumUserData];
	for(int i=0;i<N;i++){
		pMoxyUserData->m_ValuesToRecord[FirstValID+i]=ArrayOfValues[i];
	}
	
	pMoxyUserData->m_NumOfValues[pMoxyUserData->m_NumUserData]=N;
	pMoxyUserData->m_TotNumUserVlaluesToRecord+=N;

	//Name
	strcpy(pMoxyUserData->m_NameArray[pMoxyUserData->m_NumUserData],Name);
	pMoxyUserData->m_SizeOfName[pMoxyUserData->m_NumUserData]=strlen(Name)*sizeof(char)+1;

	//================
	pMoxyUserData->m_NumUserData++;
	pMoxyUserData->m_FirstLocationOfUserValue[pMoxyUserData->m_NumUserData]=pMoxyUserData->m_TotNumUserVlaluesToRecord;