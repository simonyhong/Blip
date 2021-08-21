	//M_ECodesMAP_size
	if(!UnmapViewOfFile(M_ECodesMAP_size)){ return; /*AfxMessageBox("Could not unmap view of file.");*/} 
	CloseHandle(hFile_M_ECodesMAP_size); 
	//M_Events_ReMapped
	if(!UnmapViewOfFile(M_Events_ReMapped)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Events_ReMapped); 
	int i, TrlIdxInMap, Nn;
	for(TrlIdxInMap=0;TrlIdxInMap<NumTrialsInMap;TrlIdxInMap++){
		//M_TrialID[]
		if(!UnmapViewOfFile(M_TrialID[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_TrialID[TrlIdxInMap]); 
		//M_StartTime[]
		if(!UnmapViewOfFile(M_StartTime[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_StartTime[TrlIdxInMap]); 
		//M_EndTime[]
		if(!UnmapViewOfFile(M_EndTime[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_EndTime[TrlIdxInMap]); 
		//M_numEcodesMAP[]
		if(!UnmapViewOfFile(M_numEcodesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_numEcodesMAP[TrlIdxInMap]); 
		//M_ECodesMAP
		if(!UnmapViewOfFile(M_ECodesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_ECodesMAP[TrlIdxInMap]); 
		//M_EventTimesMAP
		if(!UnmapViewOfFile(M_EventTimesMAP[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_EventTimesMAP[TrlIdxInMap]); 
		//M_NumChansHavingSpikes[]
		if(!UnmapViewOfFile(M_NumChansHavingSpikes[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_NumChansHavingSpikes[TrlIdxInMap]); 
		//M_SpkTimeOfNn1MAP_ReMapped[]
		if(!UnmapViewOfFile(M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap]); 
		//M_SpkTimeOfNn2MAP_ReMapped[]
		if(!UnmapViewOfFile(M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap]); 
		//M_SpkTimeOfNn3MAP_ReMapped[]
		if(!UnmapViewOfFile(M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap])){ AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap]); 
		for(i=0;i<NUM_NN_AI_CHAN;i++){
			//M_ChanIDsHavingSpikes[][]
			if(!UnmapViewOfFile(M_ChanIDsHavingSpikes[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_ChanIDsHavingSpikes[TrlIdxInMap][i]); 
			//M_NumNnsInChanHavingSpikes[][]
			if(!UnmapViewOfFile(M_NumNnsInChanHavingSpikes[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_NumNnsInChanHavingSpikes[TrlIdxInMap][i]); 
			for(Nn=0;Nn<MAX_NNs_PerCHAN;Nn++){
				//M_NnIdWithSpk[][][]
				if(!UnmapViewOfFile(M_NnIdWithSpk[TrlIdxInMap][i][Nn])){ AfxMessageBox("Could not unmap view of file.");} 
				CloseHandle(hFile_M_NnIdWithSpk[TrlIdxInMap][i][Nn]); 
			}
			//M_numSpksOfNn1MAP[][]
			if(!UnmapViewOfFile(M_numSpksOfNn1MAP[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_numSpksOfNn1MAP[TrlIdxInMap][i]); 
			//M_numSpksOfNn2MAP[][]
			if(!UnmapViewOfFile(M_numSpksOfNn2MAP[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_numSpksOfNn2MAP[TrlIdxInMap][i]); 
			//M_numSpksOfNn3MAP[][]
			if(!UnmapViewOfFile(M_numSpksOfNn3MAP[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_numSpksOfNn3MAP[TrlIdxInMap][i]); 
			//M_SpkTimeOfNn1MAP_size[][]
			if(!UnmapViewOfFile(M_SpkTimeOfNn1MAP_size[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn1MAP_size[TrlIdxInMap][i]); 
			//M_SpkTimeOfNn2MAP_size[][]
			if(!UnmapViewOfFile(M_SpkTimeOfNn2MAP_size[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn2MAP_size[TrlIdxInMap][i]); 
			//M_SpkTimeOfNn3MAP_size[][]
			if(!UnmapViewOfFile(M_SpkTimeOfNn3MAP_size[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn3MAP_size[TrlIdxInMap][i]); 

			if(!UnmapViewOfFile(M_SpkTimeOfNn1MAP[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn1MAP[TrlIdxInMap][i]); 
			if(!UnmapViewOfFile(M_SpkTimeOfNn2MAP[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn2MAP[TrlIdxInMap][i]); 
			if(!UnmapViewOfFile(M_SpkTimeOfNn3MAP[TrlIdxInMap][i])){ AfxMessageBox("Could not unmap view of file.");} 
			CloseHandle(hFile_M_SpkTimeOfNn3MAP[TrlIdxInMap][i]); 
		}
	}