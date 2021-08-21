		int sizeOfMap;
		//M_TrialID[]
		char NameOfMap[256];
		sprintf(NameOfMap,"Map_M_TrialID[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_TrialID[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_TrialID[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_TrialID[TrlIdxInMap] == NULL){	
			if(IsToCreate==0){if(FirstTimeMsg==1){AfxMessageBox("Unable to open Map_M_TrialID[TrlIdxInMap]'s shared memory area.");	 FirstTimeMsg=0;} break;}
			else if(IsToCreate==1){AfxMessageBox("Unable to create Map_M_TrialID[TrlIdxInMap]'s shared memory area.");	 exit(1);}
			else{AfxMessageBox("IsToCreate==?");}
		}
		M_TrialID[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_TrialID[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_TrialID[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}


		//M_StartTime[]
		sprintf(NameOfMap,"Map_M_StartTime[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_StartTime[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
		else			 hFile_M_StartTime[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_StartTime[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_StartTime[TrlIdxInMap]'s shared memory area.");	 exit(1);}
		M_StartTime[TrlIdxInMap] = (__int64*) MapViewOfFile(hFile_M_StartTime[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_StartTime[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_EndTime[]
		sprintf(NameOfMap,"Map_M_EndTime[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_EndTime[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(__int64), NameOfMap);
		else			 hFile_M_EndTime[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_EndTime[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_EndTime[TrlIdxInMap]'s shared memory area.");	exit(1);}
		M_EndTime[TrlIdxInMap] = (__int64*) MapViewOfFile(hFile_M_EndTime[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_EndTime[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_numEcodesMAP[]
		sprintf(NameOfMap,"Map_M_numEcodesMAP[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_numEcodesMAP[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_numEcodesMAP[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_numEcodesMAP[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_numEcodesMAP[TrlIdxInMap]'s shared memory area."); exit(1);}
		M_numEcodesMAP[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_numEcodesMAP[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_numEcodesMAP[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

		//M_ECodesMAP_size
		sprintf(NameOfMap,"Map_M_ECodesMAP_size");
		if(IsToCreate==1)hFile_M_ECodesMAP_size = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_TRLS_IN_MAP * sizeof(int), NameOfMap);
		else			 hFile_M_ECodesMAP_size = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_ECodesMAP_size == NULL){	AfxMessageBox("Unable to create/open Map_M_ECodesMAP_size's shared memory area.");	 exit(1);}
		M_ECodesMAP_size = (int*) MapViewOfFile(hFile_M_ECodesMAP_size, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_ECodesMAP_size == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_Events_ReMapped
		sprintf(NameOfMap,"Map_M_Events_ReMapped");
		if(IsToCreate==1)hFile_M_Events_ReMapped = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_TRLS_IN_MAP * sizeof(int), NameOfMap);
		else			 hFile_M_Events_ReMapped = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_Events_ReMapped == NULL){	AfxMessageBox("Unable to create/open Map_M_Events_ReMapped's shared memory area.");	 exit(1);}
		M_Events_ReMapped = (int*) MapViewOfFile(hFile_M_Events_ReMapped, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_Events_ReMapped == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	
		#include "MemMap_Open_Ecodes.h"

		//M_NumChansHavingSpikes[]
		sprintf(NameOfMap,"Map_M_NumChansHavingSpikes[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_NumChansHavingSpikes[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
		else			 hFile_M_NumChansHavingSpikes[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_NumChansHavingSpikes[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_NumChansHavingSpikes[TrlIdxInMap]'s shared memory area."); exit(1);}
		M_NumChansHavingSpikes[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_NumChansHavingSpikes[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_NumChansHavingSpikes[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		
		//M_SpkTimeOfNn1MAP_ReMapped[]
		sprintf(NameOfMap,"Map_M_SpkTimeOfNn1MAP_ReMapped[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
		else			 hFile_M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap]'s shared memory area."); exit(1);}
		M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_SpkTimeOfNn1MAP_ReMapped[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_SpkTimeOfNn2MAP_ReMapped[]
		sprintf(NameOfMap,"Map_M_SpkTimeOfNn2MAP_ReMapped[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
		else			 hFile_M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap]'s shared memory area."); exit(1);}
		M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_SpkTimeOfNn2MAP_ReMapped[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_SpkTimeOfNn3MAP_ReMapped[]
		sprintf(NameOfMap,"Map_M_SpkTimeOfNn3MAP_ReMapped[%d]",TrlIdxInMap);
		if(IsToCreate==1)hFile_M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_NN_AI_CHAN * sizeof(int), NameOfMap);
		else			 hFile_M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
		if(hFile_M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap]'s shared memory area."); exit(1);}
		M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap] = (int*) MapViewOfFile(hFile_M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_SpkTimeOfNn3MAP_ReMapped[TrlIdxInMap] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

		for(int ch=0;ch<NUM_NN_AI_CHAN;ch++){
			//M_ChanIDsHavingSpikes[][]
			sprintf(NameOfMap,"Map_M_ChanIDsHavingSpikes[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_ChanIDsHavingSpikes[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_ChanIDsHavingSpikes[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_ChanIDsHavingSpikes[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_ChanIDsHavingSpikes[TrlIdxInMap][ch]'s shared memory area.");	exit(1);}
			M_ChanIDsHavingSpikes[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_ChanIDsHavingSpikes[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_ChanIDsHavingSpikes[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_NumNnsInChanHavingSpikes[][]
			sprintf(NameOfMap,"Map_M_NumNnsInChanHavingSpikes[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch]'s shared memory area.");	 exit(1);}
			M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_NumNnsInChanHavingSpikes[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			for(int Nn=0;Nn<MAX_NNs_PerCHAN;Nn++){
				//M_NnIdWithSpk[][][]
				sprintf(NameOfMap,"Map_M_NnIdWithSpk[%d][%d][%d]",TrlIdxInMap,ch,Nn);
				if(IsToCreate==1)hFile_M_NnIdWithSpk[TrlIdxInMap][ch][Nn] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
				else			 hFile_M_NnIdWithSpk[TrlIdxInMap][ch][Nn] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
				if(hFile_M_NnIdWithSpk[TrlIdxInMap][ch][Nn] == NULL){	AfxMessageBox("Unable to create/open Map_M_NnIdWithSpk[TrlIdxInMap][ch][Nn]'s shared memory area.");	 exit(1);}
				M_NnIdWithSpk[TrlIdxInMap][ch][Nn] = (int*) MapViewOfFile(hFile_M_NnIdWithSpk[TrlIdxInMap][ch][Nn], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
				if(M_NnIdWithSpk[TrlIdxInMap][ch][Nn] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			}
			//M_numSpksOfNn1MAP[][]
			sprintf(NameOfMap,"Map_M_numSpksOfNn1MAP[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_numSpksOfNn1MAP[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_numSpksOfNn1MAP[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_numSpksOfNn1MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_numSpksOfNn1MAP[TrlIdxInMap][ch]'s shared memory area.");	 exit(1);}
			M_numSpksOfNn1MAP[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_numSpksOfNn1MAP[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_numSpksOfNn1MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_numSpksOfNn2MAP[][]
			sprintf(NameOfMap,"Map_M_numSpksOfNn2MAP[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_numSpksOfNn2MAP[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_numSpksOfNn2MAP[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_numSpksOfNn2MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_numSpksOfNn2MAP[TrlIdxInMap][ch]'s shared memory area.");	 exit(1);}
			M_numSpksOfNn2MAP[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_numSpksOfNn2MAP[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_numSpksOfNn2MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_numSpksOfNn3MAP[][]
			sprintf(NameOfMap,"Map_M_numSpksOfNn3MAP[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_numSpksOfNn3MAP[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_numSpksOfNn3MAP[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_numSpksOfNn3MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_numSpksOfNn3MAP[TrlIdxInMap][ch]'s shared memory area.");	 exit(1);}
			M_numSpksOfNn3MAP[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_numSpksOfNn3MAP[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_numSpksOfNn3MAP[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

			//M_SpkTimeOfNn1MAP_size[][]
			sprintf(NameOfMap,"Map_M_SpkTimeOfNn1MAP_size[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch]'s shared memory area."); exit(1);}
			M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SpkTimeOfNn1MAP_size[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SpkTimeOfNn2MAP_size[][]
			sprintf(NameOfMap,"Map_M_SpkTimeOfNn2MAP_size[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch]'s shared memory area."); exit(1);}
			M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SpkTimeOfNn2MAP_size[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
			//M_SpkTimeOfNn3MAP_size[][]
			sprintf(NameOfMap,"Map_M_SpkTimeOfNn3MAP_size[%d][%d]",TrlIdxInMap,ch);
			if(IsToCreate==1)hFile_M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
			else			 hFile_M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
			if(hFile_M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open Map_M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch]'s shared memory area."); exit(1);}
			M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch] = (int*) MapViewOfFile(hFile_M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
			if(M_SpkTimeOfNn3MAP_size[TrlIdxInMap][ch] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

			#include "MemMap_Open_SpikesNn1.h"
			#include "MemMap_Open_SpikesNn2.h"
			#include "MemMap_Open_SpikesNn3.h"

			//####### The alias of the ECode
		}
