		
		if(IsToCreate==1){
			sizeOfMap=BiggerOne(DEFAULT_SPK_NUM_PER_TRIAL,*M_numSpikesOfNn1[ch]+100);*M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch]=sizeOfMap; //+100 ensures that any additional spikes occuring during the trial-end-registration period could still be registered.
			M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap][ch]=0;
		}
		else if(IsToCreate==2){
			//First Destroy the existing one
			if(!UnmapViewOfFile(M_SpkTimeOfNn1MAP[TrlIdxInMap][ch])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][ch]); 
			//Then create a new one		
			sizeOfMap=*M_numSpikesOfNn1[ch]+100;*M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch]=sizeOfMap;
			M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap][ch]++;
		}
		else if(IsToCreate==0){;}//IsToCreate==0 => Just opening. "sizeOfMap" is not used here
		else if(IsToCreate==3){// "sizeOfMap" is not used here
			//First Destroy the existing one
			if(!UnmapViewOfFile(M_SpkTimeOfNn1MAP[TrlIdxInMap][ch])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][ch]); 
			//Then inform that the view has been updated
		}
		//M_SpkTimeOfNn1MAP[][]
		sprintf(NameOfMap,"Map_M_SpkTimeOfNn1MAP%d[%d][%d]",M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap][ch],TrlIdxInMap,ch);
		if(IsToCreate==1||IsToCreate==2)hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, sizeOfMap * sizeof(__int64), NameOfMap);
		else							hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn1MAP%d[TrlIdxInMap][ch]'s shared memory area.");	 exit(1);}
		M_SpkTimeOfNn1MAP[TrlIdxInMap][ch] = (__int64*) MapViewOfFile(hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_SpkTimeOfNn1MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
