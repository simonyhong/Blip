	//Values
	int FirstValID=m_FirstLocationOfUserValue[m_NumUserData];
	if(FirstValID+N>=m_ValuesToRecord.GetSize()){
		m_ValuesToRecord.SetSize(2*(FirstValID+N));
	}
	for(int i=0;i<N;i++){
		m_ValuesToRecord[FirstValID+i]=ArrayOfValues[i];
	}
	m_NumOfValues[m_NumUserData]=N;
	m_TotNumUserVlaluesToRecord+=N;

	//Name
	m_NameArray[m_NumUserData]=Name;
	m_SizeOfName[m_NumUserData]=strlen(Name)*sizeof(char)+1;

	//================
	m_NumUserData++;
	if(m_SizeOfUserData<=m_NumUserData){
		if(2*m_SizeOfUserData<=m_NumUserData)m_SizeOfUserData=2*m_NumUserData; else m_SizeOfUserData*=2;
		m_NameArray.SetSize(m_SizeOfUserData);
		m_SizeOfName.SetSize(m_SizeOfUserData);
		m_FirstLocationOfUserValue.SetSize(m_SizeOfUserData);
		m_NumOfValues.SetSize(m_SizeOfUserData);
		m_TimeOfOccurrence.SetSize(m_SizeOfUserData);
	}
	m_FirstLocationOfUserValue[m_NumUserData]=m_TotNumUserVlaluesToRecord;