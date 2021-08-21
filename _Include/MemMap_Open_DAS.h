	int i;
	char NameOfMap[256];
	//##################  Parameters and variables  ##################################
RESTART_IN_CREATION_MODE:
	//M_SamplingRate
	sprintf(NameOfMap,"Map_M_SamplingRate");
	if(IsToCreate==1)hFile_M_SamplingRate = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_SamplingRate = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SamplingRate == NULL){ 
		if(IsToCreate==0){IsToCreate=1; goto RESTART_IN_CREATION_MODE;} 
		AfxMessageBox("Unable to create/open Map_M_SamplingRate's shared memory area.");	 exit(1);
	}
	M_SamplingRate = (int*) MapViewOfFile(hFile_M_SamplingRate, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SamplingRate == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

	//M_AIQueueSize
	sprintf(NameOfMap,"Map_M_AIQueueSize");
	if(IsToCreate==1)hFile_M_AIQueueSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_AI_CHAN * sizeof(__int64), NameOfMap);
	else			 hFile_M_AIQueueSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_AIQueueSize == NULL){	
		//AfxMessageBox("Unable to create/open Map_M_AIQueueSize's shared memory area.");	 
		exit(1);
	}
	M_AIQueueSize = (__int64*) MapViewOfFile(hFile_M_AIQueueSize, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_AIQueueSize == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	
	//M_DO_QueueSize
	sprintf(NameOfMap,"Map_M_DO_QueueSize");
	if(IsToCreate==1)hFile_M_DO_QueueSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_DO_QueueSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_DO_QueueSize == NULL){	AfxMessageBox("Unable to create/open Map_M_DO_QueueSize's shared memory area.");	 exit(1);}
	M_DO_QueueSize = (int*) MapViewOfFile(hFile_M_DO_QueueSize, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DO_QueueSize == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_WavePointer_QueueSize
	sprintf(NameOfMap,"Map_M_WavePointer_QueueSize");
	if(IsToCreate==1)hFile_M_WavePointer_QueueSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_WavePointer_QueueSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_WavePointer_QueueSize == NULL){	AfxMessageBox("Unable to create/open Map_M_WavePointer_QueueSize's shared memory area.");	 exit(1);}
	M_WavePointer_QueueSize = (int*) MapViewOfFile(hFile_M_WavePointer_QueueSize, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_WavePointer_QueueSize == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Spike_QueueSize
	sprintf(NameOfMap,"Map_M_Spike_QueueSize");
	if(IsToCreate==1)hFile_M_Spike_QueueSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_Spike_QueueSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Spike_QueueSize == NULL){	AfxMessageBox("Unable to create/open Map_M_Spike_QueueSize's shared memory area.");	 exit(1);}
	M_Spike_QueueSize = (int*) MapViewOfFile(hFile_M_Spike_QueueSize, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Spike_QueueSize == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_usableTrialIdxInMem
	sprintf(NameOfMap,"Map_M_usableTrialIdxInMem");
	if(IsToCreate==1)hFile_M_usableTrialIdxInMem = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_usableTrialIdxInMem = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_usableTrialIdxInMem == NULL){	AfxMessageBox("Unable to create/open Map_M_usableTrialIdxInMem's shared memory area.");	 exit(1);}
	M_usableTrialIdxInMem = (int*) MapViewOfFile(hFile_M_usableTrialIdxInMem, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_usableTrialIdxInMem == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Ecode_QueueSize
	sprintf(NameOfMap,"Map_M_Ecode_QueueSize");
	if(IsToCreate==1)hFile_M_Ecode_QueueSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_Ecode_QueueSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Ecode_QueueSize == NULL){	AfxMessageBox("Unable to create/open Map_M_Ecode_QueueSize's shared memory area.");	 exit(1);}
	M_Ecode_QueueSize = (int*) MapViewOfFile(hFile_M_Ecode_QueueSize, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Ecode_QueueSize == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_SamplesPerRead
	sprintf(NameOfMap,"Map_M_SamplesPerRead");
	if(IsToCreate==1)hFile_M_SamplesPerRead = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_SamplesPerRead = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SamplesPerRead == NULL){	AfxMessageBox("Unable to create/open Map_M_SamplesPerRead's shared memory area.");	 exit(1);}
	M_SamplesPerRead = (int*) MapViewOfFile(hFile_M_SamplesPerRead, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SamplesPerRead == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_SamplingEpoch_us
	sprintf(NameOfMap,"Map_M_SamplingEpoch_us");
	if(IsToCreate==1)hFile_M_SamplingEpoch_us = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_SamplingEpoch_us = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SamplingEpoch_us == NULL){	AfxMessageBox("Unable to create/open Map_M_SamplingEpoch_us's shared memory area.");	 exit(1);}
	M_SamplingEpoch_us = (int*) MapViewOfFile(hFile_M_SamplingEpoch_us, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SamplingEpoch_us == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_microSecPerPoint
	sprintf(NameOfMap,"Map_M_microSecPerPoint");
	if(IsToCreate==1)hFile_M_microSecPerPoint = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_microSecPerPoint = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_microSecPerPoint == NULL){	AfxMessageBox("Unable to create/open Map_M_microSecPerPoint's shared memory area.");	 exit(1);}
	M_microSecPerPoint = (int*) MapViewOfFile(hFile_M_microSecPerPoint, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_microSecPerPoint == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_numNeuralChannels
	sprintf(NameOfMap,"Map_M_numNeuralChannels");
	if(IsToCreate==1)hFile_M_numNeuralChannels = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_numNeuralChannels = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_numNeuralChannels == NULL){	AfxMessageBox("Unable to create/open Map_M_numNeuralChannels's shared memory area.");	 exit(1);}
	M_numNeuralChannels = (int*) MapViewOfFile(hFile_M_numNeuralChannels, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_numNeuralChannels == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_numBehaviorChans
	sprintf(NameOfMap,"Map_M_numBehaviorChans");
	if(IsToCreate==1)hFile_M_numBehaviorChans = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_numBehaviorChans = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_numBehaviorChans == NULL){	AfxMessageBox("Unable to create/open Map_M_numBehaviorChans's shared memory area.");	 exit(1);}
	M_numBehaviorChans = (int*) MapViewOfFile(hFile_M_numBehaviorChans, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_numBehaviorChans == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//Initialize this, only by the one that creates: This is needed for saving operation when DAS is not running.
	if(IsToCreate==1)*M_numBehaviorChans  = NUM_2D_BHVR_AI_CHANS;

	//M_WaveSpanPre_ms
	sprintf(NameOfMap,"Map_M_WaveSpanPre_ms");
	if(IsToCreate==1)hFile_M_WaveSpanPre_ms = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(float), NameOfMap);
	else			 hFile_M_WaveSpanPre_ms = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_WaveSpanPre_ms == NULL){	AfxMessageBox("Unable to create/open Map_M_WaveSpanPre_ms's shared memory area.");	 exit(1);}
	M_WaveSpanPre_ms = (float*) MapViewOfFile(hFile_M_WaveSpanPre_ms, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_WaveSpanPre_ms == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_WaveSpanPost_ms
	sprintf(NameOfMap,"Map_M_WaveSpanPost_ms");
	if(IsToCreate==1)hFile_M_WaveSpanPost_ms = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(float), NameOfMap);
	else			 hFile_M_WaveSpanPost_ms = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_WaveSpanPost_ms == NULL){	AfxMessageBox("Unable to create/open Map_M_WaveSpanPost_ms's shared memory area.");	 exit(1);}
	M_WaveSpanPost_ms = (float*) MapViewOfFile(hFile_M_WaveSpanPost_ms, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_WaveSpanPost_ms == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_MiniRequiredLengthOfwaveForExamination
	sprintf(NameOfMap,"Map_M_MiniRequiredLengthOfwaveForExamination");
	if(IsToCreate==1)hFile_M_MiniRequiredLengthOfwaveForExamination = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
	else			 hFile_M_MiniRequiredLengthOfwaveForExamination = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_MiniRequiredLengthOfwaveForExamination == NULL){	AfxMessageBox("Unable to create/open Map_M_MiniRequiredLengthOfwaveForExamination's shared memory area.");	 exit(1);}
	M_MiniRequiredLengthOfwaveForExamination = (int*) MapViewOfFile(hFile_M_MiniRequiredLengthOfwaveForExamination, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MiniRequiredLengthOfwaveForExamination == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_LatestTimeStampInMicroS
	sprintf(NameOfMap,"Map_M_LatestTimeStampInMicroS");
	if(IsToCreate==1)hFile_M_LatestTimeStampInMicroS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
	else			 hFile_M_LatestTimeStampInMicroS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_LatestTimeStampInMicroS == NULL){	AfxMessageBox("Unable to create/open Map_M_LatestTimeStampInMicroS's shared memory area.");	 exit(1);}
	M_LatestTimeStampInMicroS = (__int64*) MapViewOfFile(hFile_M_LatestTimeStampInMicroS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_LatestTimeStampInMicroS == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec
	sprintf(NameOfMap,"Map_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec");
	if(IsToCreate==1)hFile_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec's shared memory area.");	 exit(1);}
	M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec = (int*) MapViewOfFile(hFile_M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyDAScl_Inter_DataRetrival_Interval_inMicroSec == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Variable_For_Anything
	sprintf(NameOfMap,"Map_M_Variable_For_Anything");
	if(IsToCreate==1)hFile_M_Variable_For_Anything = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_Variable_For_Anything = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Variable_For_Anything == NULL){	AfxMessageBox("Unable to create/open Map_M_Variable_For_Anything's shared memory area.");	 exit(1);}
	M_Variable_For_Anything = (int*) MapViewOfFile(hFile_M_Variable_For_Anything, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Variable_For_Anything == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Blip_OFF
	sprintf(NameOfMap,"Map_M_Blip_OFF");
	if(IsToCreate==1)hFile_M_Blip_OFF = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_Blip_OFF = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Blip_OFF == NULL){	AfxMessageBox("Unable to create/open Map_M_Blip_OFF's shared memory area.");	 exit(1);}
	M_Blip_OFF = (int*) MapViewOfFile(hFile_M_Blip_OFF, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Blip_OFF == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_Blip_OFF = 0;

	//M_isDAS_Running
	sprintf(NameOfMap,"Map_M_isDAS_Running");
	if(IsToCreate==1)hFile_M_isDAS_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isDAS_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isDAS_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_isDAS_Running's shared memory area.");	 exit(1);}
	M_isDAS_Running = (int*) MapViewOfFile(hFile_M_isDAS_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isDAS_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isDAS_Running = 0;
	//M_isDAScl_Running
	sprintf(NameOfMap,"Map_M_isDAScl_Running");
	if(IsToCreate==1)hFile_M_isDAScl_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isDAScl_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isDAScl_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_isDAScl_Running's shared memory area.");	 exit(1);}
	M_isDAScl_Running = (int*) MapViewOfFile(hFile_M_isDAScl_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isDAScl_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isDAScl_Running = 0;
	//M_isANTS_Running
	sprintf(NameOfMap,"Map_M_isANTS_Running");
	if(IsToCreate==1)hFile_M_isANTS_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isANTS_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isANTS_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_isANTS_Running's shared memory area.");	 exit(1);}
	M_isANTS_Running = (int*) MapViewOfFile(hFile_M_isANTS_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isANTS_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isANTS_Running = 0;

	//##############   Raw Data   #####################
	for(i=0;i<MAX_AI_CHAN;i++){
		//M_usableAiIdx[i]
		sprintf(NameOfMap,"Map_M_usableAiIdx[%d]",i);
		if(IsToCreate==1)hFile_M_usableAiIdx[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
		else			 hFile_M_usableAiIdx[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_usableAiIdx[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_usableAiIdx[i]'s shared memory area.\n Simon: It is possible that you may have set both the AI_Q_SIZE and MAX_AI_CHAN too big that the operating system does not allow the memory allocation.");	 exit(1);}
		M_usableAiIdx[i] = (__int64*) MapViewOfFile(hFile_M_usableAiIdx[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_usableAiIdx[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW.\n Simon: It is possible that you may have set both the AI_Q_SIZE and MAX_AI_CHAN too big that the operating system does not allow the memory allocation."); exit(1);}
		//M_AI[i]
		sprintf(NameOfMap,"Map_M_AI[%d]",i);
		if(IsToCreate==1)hFile_M_AI[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, AI_Q_SIZE * sizeof(double), NameOfMap);
		else			 hFile_M_AI[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_AI[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_AI[i]'s shared memory area.\n Simon: It is possible that you may have set both the AI_Q_SIZE and MAX_AI_CHAN too big that the operating system does not allow the memory allocation.");	 exit(1);}
		M_AI[i] = (double*) MapViewOfFile(hFile_M_AI[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_AI[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW.\n Simon: It is possible that you may have set both the AI_Q_SIZE and MAX_AI_CHAN too big that the operating system does not allow the memory allocation."); exit(1);}
		//M_timeStampMicS[i]
		sprintf(NameOfMap,"Map_M_timeStampMicS[%d]",i);
		if(IsToCreate==1)hFile_M_timeStampMicS[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, AI_Q_SIZE * sizeof(__int64), NameOfMap);
		else			 hFile_M_timeStampMicS[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_timeStampMicS[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_timeStampMicS[i]'s shared memory area.\n Simon: It is possible that you may have set both the AI_Q_SIZE and MAX_AI_CHAN too big that the operating system does not allow the memory allocation.");	 exit(1);}
		M_timeStampMicS[i] = (__int64*) MapViewOfFile(hFile_M_timeStampMicS[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_timeStampMicS[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW.\n Simon: It is possible that you may have set both the AI_Q_SIZE and MAX_AI_CHAN too big that the operating system does not allow the memory allocation."); exit(1);}
	}
	//M_NumOfFigsUsingThisChan
	sprintf(NameOfMap,"Map_M_NumOfFigsUsingThisChan");
	if(IsToCreate==1)hFile_M_NumOfFigsUsingThisChan = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_AI_CHAN * sizeof(int), NameOfMap);
	else			 hFile_M_NumOfFigsUsingThisChan = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumOfFigsUsingThisChan == NULL){	AfxMessageBox("Unable to create/open Map_M_NumOfFigsUsingThisChan's shared memory area.");	 exit(1);}
	M_NumOfFigsUsingThisChan = (int*) MapViewOfFile(hFile_M_NumOfFigsUsingThisChan, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumOfFigsUsingThisChan == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_SampTimeArrayInMicS
	sprintf(NameOfMap,"Map_M_SampTimeArrayInMicS");
	if(IsToCreate==1)hFile_M_SampTimeArrayInMicS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
	else			 hFile_M_SampTimeArrayInMicS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SampTimeArrayInMicS == NULL){	AfxMessageBox("Unable to create/open Map_M_SampTimeArrayInMicS's shared memory area.");	 exit(1);}
	M_SampTimeArrayInMicS = (__int64*) MapViewOfFile(hFile_M_SampTimeArrayInMicS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SampTimeArrayInMicS == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_SampTimeArrayIdx
	sprintf(NameOfMap,"Map_M_SampTimeArrayIdx");
	if(IsToCreate==1)hFile_M_SampTimeArrayIdx = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_SampTimeArrayIdx = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SampTimeArrayIdx == NULL){	AfxMessageBox("Unable to create/open Map_M_SampTimeArrayIdx's shared memory area.");	 exit(1);}
	M_SampTimeArrayIdx = (int*) MapViewOfFile(hFile_M_SampTimeArrayIdx, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SampTimeArrayIdx == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

	//##############   Digital Out    #####################
	//M_DO_Array 
	sprintf(NameOfMap,"Map_M_DO_Array");
	if(IsToCreate==1)hFile_M_DO_Array = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_DO_NUM * sizeof(unsigned char), NameOfMap);
	else			 hFile_M_DO_Array = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_DO_Array  == NULL){	AfxMessageBox("Unable to create/open Map_M_DO_Array 's shared memory area.");	 exit(1);}
	M_DO_Array  = (unsigned char*) MapViewOfFile(hFile_M_DO_Array , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DO_Array  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##############   Digital In    #####################
	//M_DI_Array 
	sprintf(NameOfMap,"Map_M_DI_Array");
	if(IsToCreate==1)hFile_M_DI_Array = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_DI_NUM * sizeof(unsigned char), NameOfMap);
	else			 hFile_M_DI_Array = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_DI_Array  == NULL){	AfxMessageBox("Unable to create/open Map_M_DI_Array's shared memory area.");	 exit(1);}
	M_DI_Array  = (unsigned char*) MapViewOfFile(hFile_M_DI_Array , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DI_Array  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1){for(i=0; i<MAX_DI_NUM;i++)M_DI_Array[i]=0;}

	//##############   Behavior Analog Inputs   #####################
	//M_BehaviorSamplingRate
	sprintf(NameOfMap,"Map_M_BehaviorSamplingRate");
	if(IsToCreate==1)hFile_M_BehaviorSamplingRate = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_BehaviorSamplingRate = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_BehaviorSamplingRate == NULL){	AfxMessageBox("Unable to create/open Map_M_BehaviorSamplingRate's shared memory area.");	 exit(1);}
	M_BehaviorSamplingRate = (int*) MapViewOfFile(hFile_M_BehaviorSamplingRate, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_BehaviorSamplingRate == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//Initialize this, only by the one that creates: This is needed for saving operation when DAS is not running.
	if(IsToCreate==1)*M_BehaviorSamplingRate = BEHAVIOR_DATA_SAMPLING_RATE;

	//M_timeStamp4BehaviorAI_MicS
	sprintf(NameOfMap,"Map_M_timeStamp4BehaviorAI_MicS");
	if(IsToCreate==1)hFile_M_timeStamp4BehaviorAI_MicS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, AI_Q_SIZE * sizeof(__int64), NameOfMap);
	else			 hFile_M_timeStamp4BehaviorAI_MicS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_timeStamp4BehaviorAI_MicS == NULL){	AfxMessageBox("Unable to create/open Map_M_timeStamp4BehaviorAI_MicS's shared memory area.");	 exit(1);}
	M_timeStamp4BehaviorAI_MicS = (__int64*) MapViewOfFile(hFile_M_timeStamp4BehaviorAI_MicS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_timeStamp4BehaviorAI_MicS == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_firstBehaviorAiIdxInTrial
	sprintf(NameOfMap,"Map_M_firstBehaviorAiIdxInTrial");
	if(IsToCreate==1)hFile_M_firstBehaviorAiIdxInTrial = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
	else			 hFile_M_firstBehaviorAiIdxInTrial = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_firstBehaviorAiIdxInTrial == NULL){	AfxMessageBox("Unable to create/open Map_M_firstBehaviorAiIdxInTrial's shared memory area.");	 exit(1);}
	M_firstBehaviorAiIdxInTrial = (__int64*) MapViewOfFile(hFile_M_firstBehaviorAiIdxInTrial, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_firstBehaviorAiIdxInTrial == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

	//##############   Voltage-Time Discriminator Setting   #####################
	//M_preWaveSpanDataSize 
	sprintf(NameOfMap,"Map_M_preWaveSpanDataSize");
	if(IsToCreate==1)hFile_M_preWaveSpanDataSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN* sizeof(int), NameOfMap);
	else			 hFile_M_preWaveSpanDataSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_preWaveSpanDataSize  == NULL){	AfxMessageBox("Unable to create/open Map_M_preWaveSpanDataSize 's shared memory area.");	 exit(1);}
	M_preWaveSpanDataSize  = (int*) MapViewOfFile(hFile_M_preWaveSpanDataSize , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_preWaveSpanDataSize  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_postWaveSpanDataSize 
	sprintf(NameOfMap,"Map_M_postWaveSpanDataSize");
	if(IsToCreate==1)hFile_M_postWaveSpanDataSize = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN* sizeof(int), NameOfMap);
	else			 hFile_M_postWaveSpanDataSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_postWaveSpanDataSize  == NULL){	AfxMessageBox("Unable to create/open Map_M_postWaveSpanDataSize 's shared memory area.");	 exit(1);}
	M_postWaveSpanDataSize  = (int*) MapViewOfFile(hFile_M_postWaveSpanDataSize , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_postWaveSpanDataSize  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_DataPointsPerWave 
	sprintf(NameOfMap,"Map_M_DataPointsPerWave");
	if(IsToCreate==1)hFile_M_DataPointsPerWave = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN* sizeof(int), NameOfMap);
	else			 hFile_M_DataPointsPerWave = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_DataPointsPerWave  == NULL){	AfxMessageBox("Unable to create/open Map_M_DataPointsPerWave 's shared memory area.");	 exit(1);}
	M_DataPointsPerWave  = (int*) MapViewOfFile(hFile_M_DataPointsPerWave , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DataPointsPerWave  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Threshold
	sprintf(NameOfMap,"Map_M_Threshold");
	if(IsToCreate==1)hFile_M_Threshold = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_AI_CHAN* sizeof(double), NameOfMap);//Every channel has a threshold, even the behavioral channels.
	else			 hFile_M_Threshold = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Threshold  == NULL){	AfxMessageBox("Unable to create/open Map_M_Threshold 's shared memory area.");	 exit(1);}
	M_Threshold  = (double*) MapViewOfFile(hFile_M_Threshold , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Threshold  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}	
	if(IsToCreate==1){for(i=NUM_NN_AI_CHAN; i<MAX_AI_CHAN;i++)M_Threshold[i]=INT_MAX;}

	//##############   Neural Analog Inputs   #####################
	for(i=0;i<NUM_NN_AI_CHAN;i++){
		//##########  Analysing the raw Neural input  #########
		//M_usableSIGi[i]
		sprintf(NameOfMap,"Map_M_usableSIGi[%d]",i);
		if(IsToCreate==1)hFile_M_usableSIGi[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
		else			 hFile_M_usableSIGi[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_usableSIGi[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_usableSIGi[i]'s shared memory area.");	 exit(1);}
		M_usableSIGi[i] = (__int64*) MapViewOfFile(hFile_M_usableSIGi[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_usableSIGi[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

		//##############   ALL wavelets   #####################
		//M_WvEcode[i] 
		sprintf(NameOfMap,"Map_M_WvEcode[%d]",i);
		if(IsToCreate==1)hFile_M_WvEcode[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_WAVE_PTRs* sizeof(int), NameOfMap);
		else			 hFile_M_WvEcode[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_WvEcode[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_WvEcode[i] 's shared memory area.");	 exit(1);}
		M_WvEcode[i]  = (int*) MapViewOfFile(hFile_M_WvEcode[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_WvEcode[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_firstIdxOfWavelet[i] 
		sprintf(NameOfMap,"Map_M_firstIdxOfWavelet[%d]",i);
		if(IsToCreate==1)hFile_M_firstIdxOfWavelet[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_WAVE_PTRs* sizeof(int), NameOfMap);
		else			 hFile_M_firstIdxOfWavelet[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_firstIdxOfWavelet[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_firstIdxOfWavelet[i] 's shared memory area.");	 exit(1);}
		M_firstIdxOfWavelet[i]  = (int*) MapViewOfFile(hFile_M_firstIdxOfWavelet[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_firstIdxOfWavelet[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_lengthOfSignal[i] 
		sprintf(NameOfMap,"Map_M_lengthOfSignal[%d]",i);
		if(IsToCreate==1)hFile_M_lengthOfSignal[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_WAVE_PTRs* sizeof(int), NameOfMap);
		else			 hFile_M_lengthOfSignal[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_lengthOfSignal[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_lengthOfSignal[i] 's shared memory area.");	 exit(1);}
		M_lengthOfSignal[i]  = (int*) MapViewOfFile(hFile_M_lengthOfSignal[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_lengthOfSignal[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_AdjustXToXthreshold[i] //This parameter is to use to shift the wavelet to align the threshold-crossing point (M_AI[x]==threshold) with the VT-discriminator's 0 point.
		sprintf(NameOfMap,"Map_M_AdjustXToXthreshold[%d]",i);
		if(IsToCreate==1)hFile_M_AdjustXToXthreshold[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_WAVE_PTRs* sizeof(double), NameOfMap);
		else			 hFile_M_AdjustXToXthreshold[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_AdjustXToXthreshold[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_AdjustXToXthreshold[i] 's shared memory area.");	 exit(1);}
		M_AdjustXToXthreshold[i]  = (double*) MapViewOfFile(hFile_M_AdjustXToXthreshold[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_AdjustXToXthreshold[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//##############   Spike data   #####################
		//M_usableSpikeIdxOfNn1[i]
		sprintf(NameOfMap,"Map_M_usableSpikeIdxOfNn1[%d]",i);
		if(IsToCreate==1)hFile_M_usableSpikeIdxOfNn1[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_usableSpikeIdxOfNn1[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_usableSpikeIdxOfNn1[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_usableSpikeIdxOfNn1[i]'s shared memory area.");	 exit(1);}
		M_usableSpikeIdxOfNn1[i] = (int*) MapViewOfFile(hFile_M_usableSpikeIdxOfNn1[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_usableSpikeIdxOfNn1[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_usableSpikeIdxOfNn2[i]
		sprintf(NameOfMap,"Map_M_usableSpikeIdxOfNn2[%d]",i);
		if(IsToCreate==1)hFile_M_usableSpikeIdxOfNn2[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_usableSpikeIdxOfNn2[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_usableSpikeIdxOfNn2[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_usableSpikeIdxOfNn2[i]'s shared memory area.");	 exit(1);}
		M_usableSpikeIdxOfNn2[i] = (int*) MapViewOfFile(hFile_M_usableSpikeIdxOfNn2[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_usableSpikeIdxOfNn2[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_usableSpikeIdxOfNn3[i]
		sprintf(NameOfMap,"Map_M_usableSpikeIdxOfNn3[%d]",i);
		if(IsToCreate==1)hFile_M_usableSpikeIdxOfNn3[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_usableSpikeIdxOfNn3[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_usableSpikeIdxOfNn3[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_usableSpikeIdxOfNn3[i]'s shared memory area.");	 exit(1);}
		M_usableSpikeIdxOfNn3[i] = (int*) MapViewOfFile(hFile_M_usableSpikeIdxOfNn3[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_usableSpikeIdxOfNn3[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_spikeWaveIdxOfNn1[i] 
		sprintf(NameOfMap,"Map_M_spikeWaveIdxOfNn1[%d]",i);
		if(IsToCreate==1)hFile_M_spikeWaveIdxOfNn1[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(int), NameOfMap);
		else			 hFile_M_spikeWaveIdxOfNn1[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spikeWaveIdxOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spikeWaveIdxOfNn1[i] 's shared memory area.");	 exit(1);}
		M_spikeWaveIdxOfNn1[i]  = (int*) MapViewOfFile(hFile_M_spikeWaveIdxOfNn1[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spikeWaveIdxOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_spikeWaveIdxOfNn2[i] 
		sprintf(NameOfMap,"Map_M_spikeWaveIdxOfNn2[%d]",i);
		if(IsToCreate==1)hFile_M_spikeWaveIdxOfNn2[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(int), NameOfMap);
		else			 hFile_M_spikeWaveIdxOfNn2[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spikeWaveIdxOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spikeWaveIdxOfNn2[i] 's shared memory area.");	 exit(1);}
		M_spikeWaveIdxOfNn2[i]  = (int*) MapViewOfFile(hFile_M_spikeWaveIdxOfNn2[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spikeWaveIdxOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_spikeWaveIdxOfNn3[i] 
		sprintf(NameOfMap,"Map_M_spikeWaveIdxOfNn3[%d]",i);
		if(IsToCreate==1)hFile_M_spikeWaveIdxOfNn3[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(int), NameOfMap);
		else			 hFile_M_spikeWaveIdxOfNn3[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spikeWaveIdxOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spikeWaveIdxOfNn3[i] 's shared memory area.");	 exit(1);}
		M_spikeWaveIdxOfNn3[i]  = (int*) MapViewOfFile(hFile_M_spikeWaveIdxOfNn3[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spikeWaveIdxOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_spikeTimeStampOfNn1[i] 
		sprintf(NameOfMap,"Map_M_spikeTimeStampOfNn1[%d]",i);
		if(IsToCreate==1)hFile_M_spikeTimeStampOfNn1[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
		else			 hFile_M_spikeTimeStampOfNn1[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spikeTimeStampOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spikeTimeStampOfNn1[i] 's shared memory area.");	 exit(1);}
		M_spikeTimeStampOfNn1[i]  = (__int64*) MapViewOfFile(hFile_M_spikeTimeStampOfNn1[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spikeTimeStampOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		if(IsToCreate==1){M_spikeTimeStampOfNn1[i][0]=-1; M_spikeTimeStampOfNn1[i][MAX_SPIKEs-1]=-1;} //This is to inform that these values are invalid ones.
		//M_spikeTimeStampOfNn2[i] 
		sprintf(NameOfMap,"Map_M_spikeTimeStampOfNn2[%d]",i);
		if(IsToCreate==1)hFile_M_spikeTimeStampOfNn2[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
		else			 hFile_M_spikeTimeStampOfNn2[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spikeTimeStampOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spikeTimeStampOfNn2[i] 's shared memory area.");	 exit(1);}
		M_spikeTimeStampOfNn2[i]  = (__int64*) MapViewOfFile(hFile_M_spikeTimeStampOfNn2[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spikeTimeStampOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		if(IsToCreate==1){M_spikeTimeStampOfNn2[i][0]=-1; M_spikeTimeStampOfNn2[i][MAX_SPIKEs-1]=-1;} //This is to inform that these values are invalid ones.
		//M_spikeTimeStampOfNn3[i] 
		sprintf(NameOfMap,"Map_M_spikeTimeStampOfNn3[%d]",i);
		if(IsToCreate==1)hFile_M_spikeTimeStampOfNn3[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
		else			 hFile_M_spikeTimeStampOfNn3[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spikeTimeStampOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spikeTimeStampOfNn3[i] 's shared memory area.");	 exit(1);}
		M_spikeTimeStampOfNn3[i]  = (__int64*) MapViewOfFile(hFile_M_spikeTimeStampOfNn3[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spikeTimeStampOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		if(IsToCreate==1){M_spikeTimeStampOfNn3[i][0]=-1; M_spikeTimeStampOfNn3[i][MAX_SPIKEs-1]=-1;} //This is to inform that these values are invalid ones.
		//M_spike_IdxOfAIOfNn1[i] 
		sprintf(NameOfMap,"Map_M_spike_IdxOfAIOfNn1[%d]",i);
		if(IsToCreate==1)hFile_M_spike_IdxOfAIOfNn1[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
		else			 hFile_M_spike_IdxOfAIOfNn1[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spike_IdxOfAIOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spike_IdxOfAIOfNn1[i] 's shared memory area.");	 exit(1);}
		M_spike_IdxOfAIOfNn1[i]  = (__int64*) MapViewOfFile(hFile_M_spike_IdxOfAIOfNn1[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spike_IdxOfAIOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_spike_IdxOfAIOfNn2[i] 
		sprintf(NameOfMap,"Map_M_spike_IdxOfAIOfNn2[%d]",i);
		if(IsToCreate==1)hFile_M_spike_IdxOfAIOfNn2[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
		else			 hFile_M_spike_IdxOfAIOfNn2[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spike_IdxOfAIOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spike_IdxOfAIOfNn2[i] 's shared memory area.");	 exit(1);}
		M_spike_IdxOfAIOfNn2[i]  = (__int64*) MapViewOfFile(hFile_M_spike_IdxOfAIOfNn2[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spike_IdxOfAIOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_spike_IdxOfAIOfNn3[i] 
		sprintf(NameOfMap,"Map_M_spike_IdxOfAIOfNn3[%d]",i);
		if(IsToCreate==1)hFile_M_spike_IdxOfAIOfNn3[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_SPIKEs * sizeof(__int64), NameOfMap);
		else			 hFile_M_spike_IdxOfAIOfNn3[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_spike_IdxOfAIOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_spike_IdxOfAIOfNn3[i] 's shared memory area.");	 exit(1);}
		M_spike_IdxOfAIOfNn3[i]  = (__int64*) MapViewOfFile(hFile_M_spike_IdxOfAIOfNn3[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_spike_IdxOfAIOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_numSpikesOfNn1[i] 
		sprintf(NameOfMap,"Map_M_numSpikesOfNn1[%d]",i);
		if(IsToCreate==1)hFile_M_numSpikesOfNn1[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_numSpikesOfNn1[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_numSpikesOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_numSpikesOfNn1[i] 's shared memory area.");	 exit(1);}
		M_numSpikesOfNn1[i]  = (int*) MapViewOfFile(hFile_M_numSpikesOfNn1[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_numSpikesOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_numSpikesOfNn2[i] 
		sprintf(NameOfMap,"Map_M_numSpikesOfNn2[%d]",i);
		if(IsToCreate==1)hFile_M_numSpikesOfNn2[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_numSpikesOfNn2[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_numSpikesOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_numSpikesOfNn2[i] 's shared memory area.");	 exit(1);}
		M_numSpikesOfNn2[i]  = (int*) MapViewOfFile(hFile_M_numSpikesOfNn2[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_numSpikesOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_numSpikesOfNn3[i] 
		sprintf(NameOfMap,"Map_M_numSpikesOfNn3[%d]",i);
		if(IsToCreate==1)hFile_M_numSpikesOfNn3[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_numSpikesOfNn3[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_numSpikesOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_numSpikesOfNn3[i] 's shared memory area.");	 exit(1);}
		M_numSpikesOfNn3[i]  = (int*) MapViewOfFile(hFile_M_numSpikesOfNn3[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_numSpikesOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_firstSpkIdxInTrialOfNn1[i] 
		sprintf(NameOfMap,"Map_M_firstSpkIdxInTrialOfNn1[%d]",i);
		if(IsToCreate==1)hFile_M_firstSpkIdxInTrialOfNn1[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_firstSpkIdxInTrialOfNn1[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_firstSpkIdxInTrialOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_firstSpkIdxInTrialOfNn1[i] 's shared memory area.");	 exit(1);}
		M_firstSpkIdxInTrialOfNn1[i]  = (int*) MapViewOfFile(hFile_M_firstSpkIdxInTrialOfNn1[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_firstSpkIdxInTrialOfNn1[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_firstSpkIdxInTrialOfNn2[i] 
		sprintf(NameOfMap,"Map_M_firstSpkIdxInTrialOfNn2[%d]",i);
		if(IsToCreate==1)hFile_M_firstSpkIdxInTrialOfNn2[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_firstSpkIdxInTrialOfNn2[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_firstSpkIdxInTrialOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_firstSpkIdxInTrialOfNn2[i] 's shared memory area.");	 exit(1);}
		M_firstSpkIdxInTrialOfNn2[i]  = (int*) MapViewOfFile(hFile_M_firstSpkIdxInTrialOfNn2[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_firstSpkIdxInTrialOfNn2[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_firstSpkIdxInTrialOfNn3[i] 
		sprintf(NameOfMap,"Map_M_firstSpkIdxInTrialOfNn3[%d]",i);
		if(IsToCreate==1)hFile_M_firstSpkIdxInTrialOfNn3[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_firstSpkIdxInTrialOfNn3[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_firstSpkIdxInTrialOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open Map_M_firstSpkIdxInTrialOfNn3[i] 's shared memory area.");	 exit(1);}
		M_firstSpkIdxInTrialOfNn3[i]  = (int*) MapViewOfFile(hFile_M_firstSpkIdxInTrialOfNn3[i] , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_firstSpkIdxInTrialOfNn3[i]  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	}
	//##############   Spike data   #####################
	//M_usableWaveIdx
	sprintf(NameOfMap,"Map_M_usableWaveIdx");
	if(IsToCreate==1)hFile_M_usableWaveIdx = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
	else			 hFile_M_usableWaveIdx = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_usableWaveIdx == NULL){	AfxMessageBox("Unable to create/open Map_M_usableWaveIdx's shared memory area.");	 exit(1);}
	M_usableWaveIdx = (int*) MapViewOfFile(hFile_M_usableWaveIdx, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_usableWaveIdx == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##############   Displayed/Non-displayed signallets   #####################
	//M_SpotlightChanID_forCLAS
	sprintf(NameOfMap,"Map_M_SpotlightChanID_forCLAS");
	if(IsToCreate==1)hFile_M_SpotlightChanID_forCLAS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_SpotlightChanID_forCLAS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SpotlightChanID_forCLAS == NULL){	AfxMessageBox("Unable to create/open Map_M_SpotlightChanID_forCLAS's shared memory area.");	 exit(1);}
	M_SpotlightChanID_forCLAS = (int*) MapViewOfFile(hFile_M_SpotlightChanID_forCLAS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SpotlightChanID_forCLAS == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_SelectedWaveFormsToDisp
	sprintf(NameOfMap,"Map_M_SelectedWaveFormsToDisp");
	if(IsToCreate==1)hFile_M_SelectedWaveFormsToDisp = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_OF_WAVEFORMS_TO_DISP * sizeof(int), NameOfMap);
	else			 hFile_M_SelectedWaveFormsToDisp = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_SelectedWaveFormsToDisp == NULL){	AfxMessageBox("Unable to create/open Map_M_SelectedWaveFormsToDisp's shared memory area.");	 exit(1);}
	M_SelectedWaveFormsToDisp = (int*) MapViewOfFile(hFile_M_SelectedWaveFormsToDisp, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SelectedWaveFormsToDisp == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NumSelectedWaveformsDisplayed
	sprintf(NameOfMap,"Map_M_NumSelectedWaveformsDisplayed");
	if(IsToCreate==1)hFile_M_NumSelectedWaveformsDisplayed = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumSelectedWaveformsDisplayed = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumSelectedWaveformsDisplayed == NULL){	AfxMessageBox("Unable to create/open Map_M_NumSelectedWaveformsDisplayed's shared memory area.");	 exit(1);}
	M_NumSelectedWaveformsDisplayed = (int*) MapViewOfFile(hFile_M_NumSelectedWaveformsDisplayed, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumSelectedWaveformsDisplayed == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NonSelectedWaveFormsToDisp
	sprintf(NameOfMap,"Map_M_NonSelectedWaveFormsToDisp");
	if(IsToCreate==1)hFile_M_NonSelectedWaveFormsToDisp = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_OF_WAVEFORMS_TO_DISP * sizeof(int), NameOfMap);
	else			 hFile_M_NonSelectedWaveFormsToDisp = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NonSelectedWaveFormsToDisp == NULL){	AfxMessageBox("Unable to create/open Map_M_NonSelectedWaveFormsToDisp's shared memory area.");	 exit(1);}
	M_NonSelectedWaveFormsToDisp = (int*) MapViewOfFile(hFile_M_NonSelectedWaveFormsToDisp, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NonSelectedWaveFormsToDisp == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NumNonSelectedWaveformsDisplayed
	sprintf(NameOfMap,"Map_M_NumNonSelectedWaveformsDisplayed");
	if(IsToCreate==1)hFile_M_NumNonSelectedWaveformsDisplayed = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumNonSelectedWaveformsDisplayed = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumNonSelectedWaveformsDisplayed == NULL){	AfxMessageBox("Unable to create/open Map_M_NumNonSelectedWaveformsDisplayed's shared memory area.");	 exit(1);}
	M_NumNonSelectedWaveformsDisplayed = (int*) MapViewOfFile(hFile_M_NumNonSelectedWaveformsDisplayed, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumNonSelectedWaveformsDisplayed == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##############   VT Boxes   #####################
	//M_Flip_AI_SignalSign
	sprintf(NameOfMap,"Map_M_Flip_AI_SignalSign");
	if(IsToCreate==1)hFile_M_Flip_AI_SignalSign = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_Flip_AI_SignalSign = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Flip_AI_SignalSign == NULL){	AfxMessageBox("Unable to create/open Map_M_Flip_AI_SignalSign's shared memory area.");	 exit(1);}
	M_Flip_AI_SignalSign = (int*) MapViewOfFile(hFile_M_Flip_AI_SignalSign, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Flip_AI_SignalSign == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
#ifndef ENABLE_THIS_FOR_DISTRIBUTION_TO_EXCLUDE_CUSTOM_CODE 
	if(IsToCreate==1)*M_Flip_AI_SignalSign = -1; //Custom definition for Lynks8, which gives inverted signals.
#endif
#ifdef ENABLE_THIS_FOR_DISTRIBUTION_TO_EXCLUDE_CUSTOM_CODE
	if(IsToCreate==1)*M_Flip_AI_SignalSign = 1; //For distribution
#endif
	//M_HeyDASsv_SomeChanBoxInfoChanged
	sprintf(NameOfMap,"Map_M_HeyDASsv_SomeChanBoxInfoChanged");
	if(IsToCreate==1)hFile_M_HeyDASsv_SomeChanBoxInfoChanged = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyDASsv_SomeChanBoxInfoChanged = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyDASsv_SomeChanBoxInfoChanged == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyDASsv_SomeChanBoxInfoChanged's shared memory area.");	 exit(1);}
	M_HeyDASsv_SomeChanBoxInfoChanged = (int*) MapViewOfFile(hFile_M_HeyDASsv_SomeChanBoxInfoChanged, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyDASsv_SomeChanBoxInfoChanged == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_HeyANTS_SomeChanBoxInfoChanged
	sprintf(NameOfMap,"Map_M_HeyANTS_SomeChanBoxInfoChanged");
	if(IsToCreate==1)hFile_M_HeyANTS_SomeChanBoxInfoChanged = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyANTS_SomeChanBoxInfoChanged = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyANTS_SomeChanBoxInfoChanged == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyANTS_SomeChanBoxInfoChanged's shared memory area.");	 exit(1);}
	M_HeyANTS_SomeChanBoxInfoChanged = (int*) MapViewOfFile(hFile_M_HeyANTS_SomeChanBoxInfoChanged, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyANTS_SomeChanBoxInfoChanged == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_ChanBoxInfoChanged
	sprintf(NameOfMap,"Map_M_ChanBoxInfoChanged");
	if(IsToCreate==1)hFile_M_ChanBoxInfoChanged = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
	else			 hFile_M_ChanBoxInfoChanged = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ChanBoxInfoChanged == NULL){	AfxMessageBox("Unable to create/open Map_M_ChanBoxInfoChanged's shared memory area.");	 exit(1);}
	M_ChanBoxInfoChanged = (int*) MapViewOfFile(hFile_M_ChanBoxInfoChanged, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ChanBoxInfoChanged == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_IsFromHighToLow
	sprintf(NameOfMap,"Map_M_IsFromHighToLow");
	if(IsToCreate==1)hFile_M_IsFromHighToLow = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
	else			 hFile_M_IsFromHighToLow = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_IsFromHighToLow == NULL){	AfxMessageBox("Unable to create/open Map_M_IsFromHighToLow's shared memory area.");	 exit(1);}
	M_IsFromHighToLow = (int*) MapViewOfFile(hFile_M_IsFromHighToLow, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_IsFromHighToLow == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_numMatchingKinds
	sprintf(NameOfMap,"Map_M_numMatchingKinds");
	if(IsToCreate==1)hFile_M_numMatchingKinds = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
	else			 hFile_M_numMatchingKinds = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_numMatchingKinds == NULL){	AfxMessageBox("Unable to create/open Map_M_numMatchingKinds's shared memory area.");	 exit(1);}
	M_numMatchingKinds = (int*) MapViewOfFile(hFile_M_numMatchingKinds, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_numMatchingKinds == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	int Ch,Nn;
	for(Ch=0;Ch<NUM_NN_AI_CHAN;Ch++){
		for(Nn=0;Nn<MAX_NNs_PerCHAN;Nn++){
			//M_SelRectBdryLtIdx
			sprintf(NameOfMap,"Map_M_SelRectBdryLtIdx[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_SelRectBdryLtIdx[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(int), NameOfMap);
			else			 hFile_M_SelRectBdryLtIdx[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SelRectBdryLtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_SelRectBdryLtIdx[][]'s shared memory area.");	 exit(1);}
			M_SelRectBdryLtIdx[Ch][Nn] = (int*) MapViewOfFile(hFile_M_SelRectBdryLtIdx[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SelRectBdryLtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SelRectBdryRtIdx
			sprintf(NameOfMap,"Map_M_SelRectBdryRtIdx[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_SelRectBdryRtIdx[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(int), NameOfMap);
			else			 hFile_M_SelRectBdryRtIdx[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SelRectBdryRtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_SelRectBdryRtIdx[][]'s shared memory area.");	 exit(1);}
			M_SelRectBdryRtIdx[Ch][Nn] = (int*) MapViewOfFile(hFile_M_SelRectBdryRtIdx[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SelRectBdryRtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_RejRectBdryLtIdx
			sprintf(NameOfMap,"Map_M_RejRectBdryLtIdx[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_RejRectBdryLtIdx[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(int), NameOfMap);
			else			 hFile_M_RejRectBdryLtIdx[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_RejRectBdryLtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_RejRectBdryLtIdx[][]'s shared memory area.");	 exit(1);}
			M_RejRectBdryLtIdx[Ch][Nn] = (int*) MapViewOfFile(hFile_M_RejRectBdryLtIdx[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_RejRectBdryLtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_RejRectBdryRtIdx
			sprintf(NameOfMap,"Map_M_RejRectBdryRtIdx[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_RejRectBdryRtIdx[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(int), NameOfMap);
			else			 hFile_M_RejRectBdryRtIdx[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_RejRectBdryRtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_RejRectBdryRtIdx[][]'s shared memory area.");	 exit(1);}
			M_RejRectBdryRtIdx[Ch][Nn] = (int*) MapViewOfFile(hFile_M_RejRectBdryRtIdx[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_RejRectBdryRtIdx[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SelRectBdryLftX_IdxInDbl
			sprintf(NameOfMap,"Map_M_SelRectBdryLftX_IdxInDbl[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_SelRectBdryLftX_IdxInDbl[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_SelRectBdryLftX_IdxInDbl[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SelRectBdryLftX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_SelRectBdryLftX_IdxInDbl[][]'s shared memory area.");	 exit(1);}
			M_SelRectBdryLftX_IdxInDbl[Ch][Nn] = (double*) MapViewOfFile(hFile_M_SelRectBdryLftX_IdxInDbl[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SelRectBdryLftX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SelRectBdryRhtX_IdxInDbl
			sprintf(NameOfMap,"Map_M_SelRectBdryRhtX_IdxInDbl[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_SelRectBdryRhtX_IdxInDbl[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_SelRectBdryRhtX_IdxInDbl[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SelRectBdryRhtX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_SelRectBdryRhtX_IdxInDbl[][]'s shared memory area.");	 exit(1);}
			M_SelRectBdryRhtX_IdxInDbl[Ch][Nn] = (double*) MapViewOfFile(hFile_M_SelRectBdryRhtX_IdxInDbl[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SelRectBdryRhtX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_RejRectBdryLftX_IdxInDbl
			sprintf(NameOfMap,"Map_M_RejRectBdryLftX_IdxInDbl[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_RejRectBdryLftX_IdxInDbl[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_RejRectBdryLftX_IdxInDbl[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_RejRectBdryLftX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_RejRectBdryLftX_IdxInDbl[][]'s shared memory area.");	 exit(1);}
			M_RejRectBdryLftX_IdxInDbl[Ch][Nn] = (double*) MapViewOfFile(hFile_M_RejRectBdryLftX_IdxInDbl[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_RejRectBdryLftX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_RejRectBdryRhtX_IdxInDbl
			sprintf(NameOfMap,"Map_M_RejRectBdryRhtX_IdxInDbl[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_RejRectBdryRhtX_IdxInDbl[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_RejRectBdryRhtX_IdxInDbl[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_RejRectBdryRhtX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_RejRectBdryRhtX_IdxInDbl[][]'s shared memory area.");	 exit(1);}
			M_RejRectBdryRhtX_IdxInDbl[Ch][Nn] = (double*) MapViewOfFile(hFile_M_RejRectBdryRhtX_IdxInDbl[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_RejRectBdryRhtX_IdxInDbl[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SelRectBdryTopYDataCoord
			sprintf(NameOfMap,"Map_M_SelRectBdryTopYDataCoord[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_SelRectBdryTopYDataCoord[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_SelRectBdryTopYDataCoord[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SelRectBdryTopYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_SelRectBdryTopYDataCoord[][]'s shared memory area.");	 exit(1);}
			M_SelRectBdryTopYDataCoord[Ch][Nn] = (double*) MapViewOfFile(hFile_M_SelRectBdryTopYDataCoord[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SelRectBdryTopYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SelRectBdryBtmYDataCoord
			sprintf(NameOfMap,"Map_M_SelRectBdryBtmYDataCoord[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_SelRectBdryBtmYDataCoord[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_SelRectBdryBtmYDataCoord[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SelRectBdryBtmYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_SelRectBdryBtmYDataCoord[][]'s shared memory area.");	 exit(1);}
			M_SelRectBdryBtmYDataCoord[Ch][Nn] = (double*) MapViewOfFile(hFile_M_SelRectBdryBtmYDataCoord[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SelRectBdryBtmYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_RejRectBdryTopYDataCoord
			sprintf(NameOfMap,"Map_M_RejRectBdryTopYDataCoord[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_RejRectBdryTopYDataCoord[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_RejRectBdryTopYDataCoord[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_RejRectBdryTopYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_RejRectBdryTopYDataCoord[][]'s shared memory area.");	 exit(1);}
			M_RejRectBdryTopYDataCoord[Ch][Nn] = (double*) MapViewOfFile(hFile_M_RejRectBdryTopYDataCoord[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_RejRectBdryTopYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_RejRectBdryBtmYDataCoord
			sprintf(NameOfMap,"Map_M_RejRectBdryBtmYDataCoord[%d][%d]",Ch,Nn);
			if(IsToCreate==1)hFile_M_RejRectBdryBtmYDataCoord[Ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VT_RECTs_PER_NEURON* sizeof(double), NameOfMap);
			else			 hFile_M_RejRectBdryBtmYDataCoord[Ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_RejRectBdryBtmYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_RejRectBdryBtmYDataCoord[][]'s shared memory area.");	 exit(1);}
			M_RejRectBdryBtmYDataCoord[Ch][Nn] = (double*) MapViewOfFile(hFile_M_RejRectBdryBtmYDataCoord[Ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_RejRectBdryBtmYDataCoord[Ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		}
		//M_numSelRects
		sprintf(NameOfMap,"Map_M_numSelRects[%d]",Ch);
		if(IsToCreate==1)hFile_M_numSelRects[Ch] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_NNs_PerCHAN* sizeof(int), NameOfMap);
		else			 hFile_M_numSelRects[Ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_numSelRects[Ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_numSelRects[]'s shared memory area.");	 exit(1);}
		M_numSelRects[Ch] = (int*) MapViewOfFile(hFile_M_numSelRects[Ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_numSelRects[Ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_numRejRects
		sprintf(NameOfMap,"Map_M_numRejRects[%d]",Ch);
		if(IsToCreate==1)hFile_M_numRejRects[Ch] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_NNs_PerCHAN* sizeof(int), NameOfMap);
		else			 hFile_M_numRejRects[Ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_numRejRects[Ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_numRejRects[]'s shared memory area.");	 exit(1);}
		M_numRejRects[Ch] = (int*) MapViewOfFile(hFile_M_numRejRects[Ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_numRejRects[Ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	}
	//##############   MOXY   #####################
	//M_isMOXY_Running
	sprintf(NameOfMap,"Map_M_isMOXY_Running");
	if(IsToCreate==1)hFile_M_isMOXY_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isMOXY_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isMOXY_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_isMOXY_Running's shared memory area.");	 exit(1);}
	M_isMOXY_Running = (int*) MapViewOfFile(hFile_M_isMOXY_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isMOXY_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##############   Trials and Event Codes   #####################
	//M_isTAS_Running
	sprintf(NameOfMap,"Map_M_isTAS_Running");
	if(IsToCreate==1)hFile_M_isTAS_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isTAS_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isTAS_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_isTAS_Running's shared memory area.");	 exit(1);}
	M_isTAS_Running = (int*) MapViewOfFile(hFile_M_isTAS_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isTAS_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_usableEcodeIdx
	sprintf(NameOfMap,"Map_M_usableEcodeIdx");
	if(IsToCreate==1)hFile_M_usableEcodeIdx = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_usableEcodeIdx = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_usableEcodeIdx == NULL){	AfxMessageBox("Unable to create/open Map_M_usableEcodeIdx's shared memory area.");	 exit(1);}
	M_usableEcodeIdx = (int*) MapViewOfFile(hFile_M_usableEcodeIdx, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_usableEcodeIdx == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_numEcodes
	sprintf(NameOfMap,"Map_M_numEcodes");
	if(IsToCreate==1)hFile_M_numEcodes = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_numEcodes = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_numEcodes == NULL){	AfxMessageBox("Unable to create/open Map_M_numEcodes's shared memory area.");	 exit(1);}
	M_numEcodes = (int*) MapViewOfFile(hFile_M_numEcodes, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_numEcodes == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_FirstEcodeIdxOfTrial
	sprintf(NameOfMap,"Map_M_FirstEcodeIdxOfTrial");
	if(IsToCreate==1)hFile_M_FirstEcodeIdxOfTrial = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_FirstEcodeIdxOfTrial = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_FirstEcodeIdxOfTrial == NULL){	AfxMessageBox("Unable to create/open Map_M_FirstEcodeIdxOfTrial's shared memory area.");	 exit(1);}
	M_FirstEcodeIdxOfTrial = (int*) MapViewOfFile(hFile_M_FirstEcodeIdxOfTrial, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_FirstEcodeIdxOfTrial == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EventCodes
	sprintf(NameOfMap,"Map_M_EventCodes");
	if(IsToCreate==1)hFile_M_EventCodes = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, ECODE_Q_SIZE * sizeof(int), NameOfMap);
	else			 hFile_M_EventCodes = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_EventCodes == NULL){	AfxMessageBox("Unable to create/open Map_M_EventCodes's shared memory area.");	 exit(1);}
	M_EventCodes = (int*) MapViewOfFile(hFile_M_EventCodes, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EventCodes == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EventTimes
	sprintf(NameOfMap,"Map_M_EventTimes");
	if(IsToCreate==1)hFile_M_EventTimes  = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, ECODE_Q_SIZE * sizeof(__int64), NameOfMap);
	else			 hFile_M_EventTimes = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_EventTimes  == NULL){	AfxMessageBox("Unable to create/open Map_M_EventTimes 's shared memory area.");	 exit(1);}
	M_EventTimes  = (__int64*) MapViewOfFile(hFile_M_EventTimes , FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EventTimes  == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

	//M_isThereNewTrial
	sprintf(NameOfMap,"Map_M_isThereNewTrial");
	if(IsToCreate==1)hFile_M_isThereNewTrial = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isThereNewTrial = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isThereNewTrial == NULL){	AfxMessageBox("Unable to create/open Map_M_isThereNewTrial's shared memory area.");	 exit(1);}
	M_isThereNewTrial = (int*) MapViewOfFile(hFile_M_isThereNewTrial, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isThereNewTrial == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NnNumberToRecord
	sprintf(NameOfMap,"Map_M_NnNumberToRecord");
	if(IsToCreate==1)hFile_M_NnNumberToRecord = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NnNumberToRecord = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NnNumberToRecord == NULL){	AfxMessageBox("Unable to create/open Map_M_NnNumberToRecord's shared memory area.");	 exit(1);}
	M_NnNumberToRecord = (int*) MapViewOfFile(hFile_M_NnNumberToRecord, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NnNumberToRecord == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##############   Trials in the Memory Map   #####################
	//M_NumTrlsInMAP
	sprintf(NameOfMap,"Map_M_NumTrlsInMAP");
	if(IsToCreate==1)hFile_M_NumTrlsInMAP = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumTrlsInMAP = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumTrlsInMAP == NULL){	AfxMessageBox("Unable to create/open Map_M_NumTrlsInMAP's shared memory area.");	 exit(1);}
	M_NumTrlsInMAP = (int*) MapViewOfFile(hFile_M_NumTrlsInMAP, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumTrlsInMAP == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##############   Antidromic Stimulations   #####################
	//M_IsAntidromicStim
	sprintf(NameOfMap,"Map_M_IsAntidromicStim");
	if(IsToCreate==1)hFile_M_IsAntidromicStim = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_IsAntidromicStim = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_IsAntidromicStim == NULL){	AfxMessageBox("Unable to create/open Map_M_IsAntidromicStim's shared memory area.");	 exit(1);}
	M_IsAntidromicStim = (int*) MapViewOfFile(hFile_M_IsAntidromicStim, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_IsAntidromicStim == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_WhichAntidromicMode
	sprintf(NameOfMap,"Map_M_WhichAntidromicMode");
	if(IsToCreate==1)hFile_M_WhichAntidromicMode = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_WhichAntidromicMode = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_WhichAntidromicMode == NULL){	AfxMessageBox("Unable to create/open Map_M_WhichAntidromicMode's shared memory area.");	 exit(1);}
	M_WhichAntidromicMode = (int*) MapViewOfFile(hFile_M_WhichAntidromicMode, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_WhichAntidromicMode == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_HeyANTS_StimGivenToGatingDIO_X
	sprintf(NameOfMap,"Map_M_HeyANTS_StimGivenToGatingDIO_X");
	if(IsToCreate==1)hFile_M_HeyANTS_StimGivenToGatingDIO_X = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyANTS_StimGivenToGatingDIO_X = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyANTS_StimGivenToGatingDIO_X == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyANTS_StimGivenToGatingDIO_X's shared memory area.");	 exit(1);}
	M_HeyANTS_StimGivenToGatingDIO_X = (int*) MapViewOfFile(hFile_M_HeyANTS_StimGivenToGatingDIO_X, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyANTS_StimGivenToGatingDIO_X == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_HeyANTS_StimGivenToGatingDIO_X = -1;
	//M_is_LFP_saving
	sprintf(NameOfMap,"Map_M_is_LFP_saving");
	if(IsToCreate==1)hFile_M_is_LFP_saving = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_is_LFP_saving = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_is_LFP_saving == NULL){	AfxMessageBox("Unable to create/open Map_M_is_LFP_saving's shared memory area.");	 exit(1);}
	M_is_LFP_saving = (int*) MapViewOfFile(hFile_M_is_LFP_saving, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_is_LFP_saving == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_HeyMOXY_StimGivenToGatingDIO_X
	sprintf(NameOfMap,"Map_M_HeyMOXY_StimGivenToGatingDIO_X");
	if(IsToCreate==1)hFile_M_HeyMOXY_StimGivenToGatingDIO_X = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyMOXY_StimGivenToGatingDIO_X = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyMOXY_StimGivenToGatingDIO_X == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyMOXY_StimGivenToGatingDIO_X's shared memory area.");	 exit(1);}
	M_HeyMOXY_StimGivenToGatingDIO_X = (int*) MapViewOfFile(hFile_M_HeyMOXY_StimGivenToGatingDIO_X, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyMOXY_StimGivenToGatingDIO_X == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_HeyMOXY_StimGivenToGatingDIO_X = -1;

	//##### All Antidromic Stimulatin channels
	//M_NumOfStimulationChannels
	sprintf(NameOfMap,"Map_M_NumOfStimulationChannels");
	if(IsToCreate==1)hFile_M_NumOfStimulationChannels = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumOfStimulationChannels = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumOfStimulationChannels == NULL){	AfxMessageBox("Unable to create/open Map_M_NumOfStimulationChannels's shared memory area.");	 exit(1);}
	M_NumOfStimulationChannels = (int*) MapViewOfFile(hFile_M_NumOfStimulationChannels, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumOfStimulationChannels == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_StimDigitalOutChannels
	sprintf(NameOfMap,"Map_M_StimDigitalOutChannels");
	if(IsToCreate==1)hFile_M_StimDigitalOutChannels = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_StimDigitalOutChannels = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_StimDigitalOutChannels == NULL){	AfxMessageBox("Unable to create/open Map_M_StimDigitalOutChannels's shared memory area.");	 exit(1);}
	M_StimDigitalOutChannels = (int*) MapViewOfFile(hFile_M_StimDigitalOutChannels, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StimDigitalOutChannels == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_StimulationRelayID
	sprintf(NameOfMap,"Map_M_StimulationRelayID");
	if(IsToCreate==1)hFile_M_StimulationRelayID = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_StimulationRelayID = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_StimulationRelayID == NULL){	AfxMessageBox("Unable to create/open Map_M_StimulationRelayID's shared memory area.");	 exit(1);}
	M_StimulationRelayID = (int*) MapViewOfFile(hFile_M_StimulationRelayID, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StimulationRelayID == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_CurrIdx_Of_StimGateRelayID
	sprintf(NameOfMap,"Map_M_CurrIdx_Of_StimGateRelayID");
	if(IsToCreate==1)hFile_M_CurrIdx_Of_StimGateRelayID = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_CurrIdx_Of_StimGateRelayID = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_CurrIdx_Of_StimGateRelayID == NULL){	AfxMessageBox("Unable to create/open Map_M_CurrIdx_Of_StimGateRelayID's shared memory area.");	 exit(1);}
	M_CurrIdx_Of_StimGateRelayID = (int*) MapViewOfFile(hFile_M_CurrIdx_Of_StimGateRelayID, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_CurrIdx_Of_StimGateRelayID == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//##### Focused Antidromic Stimulatin channels
	//M_NumOfFocusChannels
	sprintf(NameOfMap,"Map_M_NumOfFocusChannels");
	if(IsToCreate==1)hFile_M_NumOfFocusChannels = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumOfFocusChannels = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumOfFocusChannels == NULL){	AfxMessageBox("Unable to create/open Map_M_NumOfFocusChannels's shared memory area.");	 exit(1);}
	M_NumOfFocusChannels = (int*) MapViewOfFile(hFile_M_NumOfFocusChannels, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumOfFocusChannels == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_isFocusedChannel
	sprintf(NameOfMap,"Map_M_isFocusedChannel");
	if(IsToCreate==1)hFile_M_isFocusedChannel = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_isFocusedChannel = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isFocusedChannel == NULL){	AfxMessageBox("Unable to create/open Map_M_isFocusedChannel's shared memory area.");	 exit(1);}
	M_isFocusedChannel = (int*) MapViewOfFile(hFile_M_isFocusedChannel, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isFocusedChannel == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	
	//M_InterStimulationInterval
	sprintf(NameOfMap,"Map_M_InterStimulationInterval");
	if(IsToCreate==1)hFile_M_InterStimulationInterval = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_InterStimulationInterval = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_InterStimulationInterval == NULL){	AfxMessageBox("Unable to create/open Map_M_InterStimulationInterval's shared memory area.");	 exit(1);}
	M_InterStimulationInterval = (int*) MapViewOfFile(hFile_M_InterStimulationInterval, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_InterStimulationInterval == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NumOfSamplingsAfterStim
	sprintf(NameOfMap,"Map_M_NumOfSamplingsAfterStim");
	if(IsToCreate==1)hFile_M_NumOfSamplingsAfterStim = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_NumOfSamplingsAfterStim = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumOfSamplingsAfterStim == NULL){	AfxMessageBox("Unable to create/open Map_M_NumOfSamplingsAfterStim's shared memory area.");	 exit(1);}
	M_NumOfSamplingsAfterStim = (int*) MapViewOfFile(hFile_M_NumOfSamplingsAfterStim, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumOfSamplingsAfterStim == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_TriggerToStimulationLatency
	sprintf(NameOfMap,"Map_M_TriggerToStimulationLatency");
	if(IsToCreate==1)hFile_M_TriggerToStimulationLatency = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_STIM_CHANNELS * sizeof(int), NameOfMap);
	else			 hFile_M_TriggerToStimulationLatency = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_TriggerToStimulationLatency == NULL){	AfxMessageBox("Unable to create/open Map_M_TriggerToStimulationLatency's shared memory area.");	 exit(1);}
	M_TriggerToStimulationLatency = (int*) MapViewOfFile(hFile_M_TriggerToStimulationLatency, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_TriggerToStimulationLatency == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_StimulationLatencyBias_InMicroS
	sprintf(NameOfMap,"Map_M_StimulationLatencyBias_InMicroS");
	if(IsToCreate==1)hFile_M_StimulationLatencyBias_InMicroS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_StimulationLatencyBias_InMicroS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_StimulationLatencyBias_InMicroS == NULL){	AfxMessageBox("Unable to create/open Map_M_StimulationLatencyBias_InMicroS's shared memory area.");	 exit(1);}
	M_StimulationLatencyBias_InMicroS = (int*) MapViewOfFile(hFile_M_StimulationLatencyBias_InMicroS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(IsToCreate==1)*M_StimulationLatencyBias_InMicroS=300; //This is to adjust the waiting time to make the Stimulation align at time 0. 
	if(M_StimulationLatencyBias_InMicroS == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_AntidromicTriggeringChanID
	sprintf(NameOfMap,"Map_M_AntidromicTriggeringChanID");
	if(IsToCreate==1)hFile_M_AntidromicTriggeringChanID = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_AntidromicTriggeringChanID = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_AntidromicTriggeringChanID == NULL){	AfxMessageBox("Unable to create/open Map_M_AntidromicTriggeringChanID's shared memory area.");	 exit(1);}
	M_AntidromicTriggeringChanID = (int*) MapViewOfFile(hFile_M_AntidromicTriggeringChanID, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_AntidromicTriggeringChanID == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_AntiStimTime
	sprintf(NameOfMap,"Map_M_AntiStimTime");
	if(IsToCreate==1)hFile_M_AntiStimTime = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
	else			 hFile_M_AntiStimTime = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_AntiStimTime == NULL){	AfxMessageBox("Unable to create/open Map_M_AntiStimTime's shared memory area.");	 exit(1);}
	M_AntiStimTime = (__int64*) MapViewOfFile(hFile_M_AntiStimTime, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_AntiStimTime == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EstimatedAntidromicStimPointIdx
	sprintf(NameOfMap,"Map_M_EstimatedAntidromicStimPointIdx");
	if(IsToCreate==1)hFile_M_EstimatedAntidromicStimPointIdx = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN* sizeof(__int64), NameOfMap);
	else			 hFile_M_EstimatedAntidromicStimPointIdx = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_EstimatedAntidromicStimPointIdx == NULL){	AfxMessageBox("Unable to create/open Map_M_EstimatedAntidromicStimPointIdx's shared memory area.");	 exit(1);}
	M_EstimatedAntidromicStimPointIdx = (__int64*) MapViewOfFile(hFile_M_EstimatedAntidromicStimPointIdx, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EstimatedAntidromicStimPointIdx == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_RememberedTurnedOnStimRelayID
	sprintf(NameOfMap,"Map_M_RememberedTurnedOnStimRelayID");
	if(IsToCreate==1)hFile_M_RememberedTurnedOnStimRelayID = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_RememberedTurnedOnStimRelayID = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_RememberedTurnedOnStimRelayID == NULL){	AfxMessageBox("Unable to create/open Map_M_RememberedTurnedOnStimRelayID's shared memory area.");	 exit(1);}
	M_RememberedTurnedOnStimRelayID = (int*) MapViewOfFile(hFile_M_RememberedTurnedOnStimRelayID, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_RememberedTurnedOnStimRelayID == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//#### Zapper
	//M_isZapperLearning
	sprintf(NameOfMap,"Map_M_isZapperLearning");
	if(IsToCreate==1)hFile_M_isZapperLearning = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isZapperLearning = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isZapperLearning == NULL){	AfxMessageBox("Unable to create/open Map_M_isZapperLearning's shared memory area.");	 exit(1);}
	M_isZapperLearning = (int*) MapViewOfFile(hFile_M_isZapperLearning, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isZapperLearning == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isZapperLearning = 0;
	//M_StartAIidx_ofArtifact_4ANTS
	sprintf(NameOfMap,"Map_M_StartAIidx_ofArtifact_4ANTS");
	if(IsToCreate==1)hFile_M_StartAIidx_ofArtifact_4ANTS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
	else			 hFile_M_StartAIidx_ofArtifact_4ANTS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_StartAIidx_ofArtifact_4ANTS == NULL){	AfxMessageBox("Unable to create/open Map_M_StartAIidx_ofArtifact_4ANTS's shared memory area.");	 exit(1);}
	M_StartAIidx_ofArtifact_4ANTS = (__int64*) MapViewOfFile(hFile_M_StartAIidx_ofArtifact_4ANTS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StartAIidx_ofArtifact_4ANTS == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_StartAIidx_ofArtifact_4ANTS = 0;
	//M_Zapper
	sprintf(NameOfMap,"Map_M_Zapper");
	if(IsToCreate==1)hFile_M_Zapper = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(Zapper), NameOfMap);
	else			 hFile_M_Zapper = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Zapper == NULL){	AfxMessageBox("Unable to create/open Map_M_Zapper's shared memory area.");	 exit(1);}
	M_Zapper = (Zapper*) MapViewOfFile(hFile_M_Zapper, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Zapper == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)for(i=0;i<NUM_NN_AI_CHAN;i++){M_Zapper[i].M_NumGatingRelayIDs = 0;} //Other initializations are done in ANTS_main().
	//##############   Orthodromic Stimulations   #####################
	//M_IsToLearnArtifact
	sprintf(NameOfMap,"Map_M_IsToLearnArtifact");
	if(IsToCreate==1)hFile_M_IsToLearnArtifact = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_IsToLearnArtifact = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_IsToLearnArtifact == NULL){	AfxMessageBox("Unable to create/open Map_M_IsToLearnArtifact's shared memory area.");	 exit(1);}
	M_IsToLearnArtifact = (int*) MapViewOfFile(hFile_M_IsToLearnArtifact, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_IsToLearnArtifact == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_IsToLearnArtifact=0;
	//M_IsOrthoStim
	sprintf(NameOfMap,"Map_M_IsOrthoStim");
	if(IsToCreate==1)hFile_M_IsOrthoStim = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_IsOrthoStim = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_IsOrthoStim == NULL){	AfxMessageBox("Unable to create/open Map_M_IsOrthoStim's shared memory area.");	 exit(1);}
	M_IsOrthoStim = (int*) MapViewOfFile(hFile_M_IsOrthoStim, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_IsOrthoStim == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation
	sprintf(NameOfMap,"Map_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation");
	if(IsToCreate==1)hFile_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(HANDLE), NameOfMap);
	else			 hFile_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation == NULL){	AfxMessageBox("Unable to create/open Map_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation's shared memory area.");	 exit(1);}
	M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation = (HANDLE*) MapViewOfFile(hFile_M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_Ev_HeyDASsv_WakeUpForOrthodromicStimulation = 0;
	//M_HeyDAS_DoOrthodromicStimulationNOW
	sprintf(NameOfMap,"Map_M_HeyDAS_DoOrthodromicStimulationNOW");
	if(IsToCreate==1)hFile_M_HeyDAS_DoOrthodromicStimulationNOW = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyDAS_DoOrthodromicStimulationNOW = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyDAS_DoOrthodromicStimulationNOW == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyDAS_DoOrthodromicStimulationNOW's shared memory area.");	 exit(1);}
	M_HeyDAS_DoOrthodromicStimulationNOW = (int*) MapViewOfFile(hFile_M_HeyDAS_DoOrthodromicStimulationNOW, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyDAS_DoOrthodromicStimulationNOW == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_HeyDAS_DoOrthodromicStimulationNOW = 0;
	//M_HeyDASsv_StimGatingRequest
	sprintf(NameOfMap,"Map_M_HeyDASsv_StimGatingRequest");
	if(IsToCreate==1)hFile_M_HeyDASsv_StimGatingRequest = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyDASsv_StimGatingRequest = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyDASsv_StimGatingRequest == NULL){	AfxMessageBox("Unable to create/open Map_M_HeyDASsv_StimGatingRequest's shared memory area.");	 exit(1);}
	M_HeyDASsv_StimGatingRequest = (int*) MapViewOfFile(hFile_M_HeyDASsv_StimGatingRequest, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyDASsv_StimGatingRequest == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_HeyDASsv_StimGatingRequest = 0;
	//M_EstimatedOrthodromicStimPointIdx
	sprintf(NameOfMap,"Map_M_EstimatedOrthodromicStimPointIdx");
	if(IsToCreate==1)hFile_M_EstimatedOrthodromicStimPointIdx = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(__int64), NameOfMap);
	else			 hFile_M_EstimatedOrthodromicStimPointIdx = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_EstimatedOrthodromicStimPointIdx == NULL){	AfxMessageBox("Unable to create/open Map_M_EstimatedOrthodromicStimPointIdx's shared memory area.");	 exit(1);}
	M_EstimatedOrthodromicStimPointIdx = (__int64*) MapViewOfFile(hFile_M_EstimatedOrthodromicStimPointIdx, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EstimatedOrthodromicStimPointIdx == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_OrthodromicRecordingChannel
	sprintf(NameOfMap,"Map_M_OrthodromicRecordingChannel");
	if(IsToCreate==1)hFile_M_OrthodromicRecordingChannel = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_OrthodromicRecordingChannel = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_OrthodromicRecordingChannel == NULL){	AfxMessageBox("Unable to create/open Map_M_OrthodromicRecordingChannel's shared memory area.");	 exit(1);}
	M_OrthodromicRecordingChannel = (int*) MapViewOfFile(hFile_M_OrthodromicRecordingChannel, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_OrthodromicRecordingChannel == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
