	{
		char NameOfMap[256];
		int sizeOfMap;
		if(IsToCreate==1){
			sizeOfMap=BiggerOne(DEFAULT_NUM_OF_ECODES_PER_TRIAL,*M_numEcodes+10); M_ECodesMAP_size[TrlIdxInMap]=sizeOfMap;//+10 ensures that any additional Ecodes occuring during the trial-end-registration period could still be registered.
			M_Events_ReMapped[TrlIdxInMap]=0;
		} 
		else if(IsToCreate==2){
			//First Destroy the existing one
			if(!UnmapViewOfFile(M_ECodesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_ECodesMAP[TrlIdxInMap]); 
			if(!UnmapViewOfFile(M_EventTimesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_EventTimesMAP[TrlIdxInMap]); 
			//Then create a new one		
			sizeOfMap=*M_numEcodes+10;M_ECodesMAP_size[TrlIdxInMap]=sizeOfMap;
			M_Events_ReMapped[TrlIdxInMap]++;
		}
		else if(IsToCreate==0){sizeOfMap=M_ECodesMAP_size[TrlIdxInMap];}//IsToCreate==0 => Just opening. "sizeOfMap" is not used here
		else if(IsToCreate==3){
			//First Destroy the existing one
			if(!UnmapViewOfFile(M_ECodesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_ECodesMAP[TrlIdxInMap]); 
			if(!UnmapViewOfFile(M_EventTimesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_EventTimesMAP[TrlIdxInMap]); 
		}	
		//M_ECodesMAP[]
		sprintf(NameOfMap,"Map_M_ECodesMAP%d[%d]",M_Events_ReMapped[TrlIdxInMap],TrlIdxInMap);
		if(IsToCreate==1||IsToCreate==2)hFile_M_ECodesMAP[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, sizeOfMap * sizeof(int), NameOfMap);
		else			 hFile_M_ECodesMAP[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_ECodesMAP[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_ECodesMAP[TrlIdxInMap]'s shared memory area.");	 exit(1);}
		M_ECodesMAP[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_ECodesMAP[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_ECodesMAP[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

		//M_EventTimesMAP[]
		sprintf(NameOfMap,"Map_M_EventTimesMAP%d[%d]",M_Events_ReMapped[TrlIdxInMap],TrlIdxInMap);
		if(IsToCreate==1||IsToCreate==2)hFile_M_EventTimesMAP[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, sizeOfMap * sizeof(__int64), NameOfMap);
		else			 hFile_M_EventTimesMAP[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_EventTimesMAP[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_EventTimesMAP[TrlIdxInMap]'s shared memory area.");	 exit(1);}
		M_EventTimesMAP[TrlIdxInMap] = (__int64*) MapViewOfFile(hFile_M_EventTimesMAP[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_EventTimesMAP[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	}

