	char Name_Of_Map[256];
REATTEMPT_IN_CREATION_MODE:;
	//########  Properties of the visual object (Memory Map) ###########
	//M_cntrX
	sprintf(Name_Of_Map,"Map_M_cntrX");
	if(IsToCreate==1)hFile_M_cntrX = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(double), Name_Of_Map);
	else			 hFile_M_cntrX = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_cntrX == NULL){	
		if(IsToCreate==0){IsToCreate=1; goto REATTEMPT_IN_CREATION_MODE;} 
		AfxMessageBox("Unable to create/open Map_M_cntrX's shared memory area.");	 exit(1);
	}	
	M_cntrX = (double*) MapViewOfFile(hFile_M_cntrX, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_cntrX == NULL){	AfxMessageBox("Unable to create/open Map_M_cntrX's VIEW."); exit(1);}
	//M_cntrY
	sprintf(Name_Of_Map,"Map_M_cntrY");
	if(IsToCreate==1)hFile_M_cntrY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(double), Name_Of_Map);
	else			 hFile_M_cntrY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_cntrY == NULL){	AfxMessageBox("Unable to create/open Map_M_cntrY's shared memory area.");	 exit(1);}
	M_cntrY = (double*) MapViewOfFile(hFile_M_cntrY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_cntrY == NULL){	AfxMessageBox("Unable to create/open Map_M_cntrY's VIEW."); exit(1);}
	//M_width
	sprintf(Name_Of_Map,"Map_M_width");
	if(IsToCreate==1)hFile_M_width = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(double), Name_Of_Map);
	else			 hFile_M_width = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_width == NULL){	AfxMessageBox("Unable to create/open Map_M_width's shared memory area.");	 exit(1);}
	M_width = (double*) MapViewOfFile(hFile_M_width, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_width == NULL){	AfxMessageBox("Unable to create/open Map_M_width's VIEW."); exit(1);}
	//M_hight
	sprintf(Name_Of_Map,"Map_M_hight");
	if(IsToCreate==1)hFile_M_hight = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(double), Name_Of_Map);
	else			 hFile_M_hight = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_hight == NULL){	AfxMessageBox("Unable to create/open Map_M_hight's shared memory area.");	 exit(1);}
	M_hight = (double*) MapViewOfFile(hFile_M_hight, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_hight == NULL){	AfxMessageBox("Unable to create/open Map_M_hight's VIEW."); exit(1);}
	//M_angle
	sprintf(Name_Of_Map,"Map_M_angle");
	if(IsToCreate==1)hFile_M_angle = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(double), Name_Of_Map);
	else			 hFile_M_angle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_angle == NULL){	AfxMessageBox("Unable to create/open Map_M_angle's shared memory area.");	 exit(1);}
	M_angle = (double*) MapViewOfFile(hFile_M_angle, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_angle == NULL){	AfxMessageBox("Unable to create/open Map_M_angle's VIEW."); exit(1);}
	//M_Obj_Shape
	sprintf(Name_Of_Map,"Map_M_Obj_Shape");
	if(IsToCreate==1)hFile_M_Obj_Shape = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
	else			 hFile_M_Obj_Shape = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Obj_Shape == NULL){	AfxMessageBox("Unable to create/open Map_M_Obj_Shape' shared memory area.");	 exit(1);}
	M_Obj_Shape = (int*) MapViewOfFile(hFile_M_Obj_Shape, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Obj_Shape == NULL){	AfxMessageBox("Unable to create/open Map_M_Obj_Shape' VIEW."); exit(1);}
	//M_rgb
	sprintf(Name_Of_Map,"Map_M_rgb");
	if(IsToCreate==1)hFile_M_rgb = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(COLORREF), Name_Of_Map);
	else			 hFile_M_rgb = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_rgb == NULL){	AfxMessageBox("Unable to create/open Map_M_rgb's shared memory area.");	 exit(1);}
	M_rgb = (COLORREF*) MapViewOfFile(hFile_M_rgb, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_rgb == NULL){	AfxMessageBox("Unable to create/open Map_M_rgb's VIEW."); exit(1);}
	//M_Ojb_Switch
	sprintf(Name_Of_Map,"Map_M_Ojb_Switch");
	if(IsToCreate==1)hFile_M_Ojb_Switch = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
	else			 hFile_M_Ojb_Switch = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Ojb_Switch == NULL){	AfxMessageBox("Unable to create/open Map_M_Ojb_Switch's shared memory area.");	 exit(1);}
	M_Ojb_Switch = (int*) MapViewOfFile(hFile_M_Ojb_Switch, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Ojb_Switch == NULL){	AfxMessageBox("Unable to create/open Map_M_Ojb_Switch's VIEW."); exit(1);}

	for(int i=0;i<MAX_STIM_OBJ;i++){
		//M_PicFileName[]
		sprintf(Name_Of_Map,"Map_M_PicFileName[%d]",i);
		if(IsToCreate==1)hFile_M_PicFileName[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_CHARS_OF_PIC_FILENAME * sizeof(char), Name_Of_Map);
		else			 hFile_M_PicFileName[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
		if(hFile_M_PicFileName[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_PicFileName[i]'s shared memory area.");	 exit(1);}
		M_PicFileName[i] = (char*) MapViewOfFile(hFile_M_PicFileName[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_PicFileName[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	}
	//M_PhotoDiodeFlash_PosX
	sprintf(Name_Of_Map,"Map_M_PhotoDiodeFlash_PosX");
	if(IsToCreate==1)hFile_M_PhotoDiodeFlash_PosX = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_PhotoDiodeFlash_PosX = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PhotoDiodeFlash_PosX == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosX's shared memory area.");	 exit(1);}
	M_PhotoDiodeFlash_PosX = (int*) MapViewOfFile(hFile_M_PhotoDiodeFlash_PosX, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PhotoDiodeFlash_PosX == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosX's VIEW."); exit(1);}
	if(IsToCreate==1)*M_PhotoDiodeFlash_PosX=0;
	//M_PhotoDiodeFlash_PosY
	sprintf(Name_Of_Map,"Map_M_PhotoDiodeFlash_PosY");
	if(IsToCreate==1)hFile_M_PhotoDiodeFlash_PosY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_PhotoDiodeFlash_PosY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PhotoDiodeFlash_PosY == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosY's shared memory area.");	 exit(1);}
	M_PhotoDiodeFlash_PosY = (int*) MapViewOfFile(hFile_M_PhotoDiodeFlash_PosY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PhotoDiodeFlash_PosY == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosY's VIEW."); exit(1);}
	if(IsToCreate==1)*M_PhotoDiodeFlash_PosY=0;
	//M_PhotoDiodeFlash_PosWidth
	sprintf(Name_Of_Map,"Map_M_PhotoDiodeFlash_PosWidth");
	if(IsToCreate==1)hFile_M_PhotoDiodeFlash_PosWidth = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_PhotoDiodeFlash_PosWidth = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PhotoDiodeFlash_PosWidth == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosWidth's shared memory area.");	 exit(1);}
	M_PhotoDiodeFlash_PosWidth = (int*) MapViewOfFile(hFile_M_PhotoDiodeFlash_PosWidth, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PhotoDiodeFlash_PosWidth == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosWidth's VIEW."); exit(1);}
	if(IsToCreate==1)*M_PhotoDiodeFlash_PosWidth=30;
	//M_PhotoDiodeFlash_PosHeight
	sprintf(Name_Of_Map,"Map_M_PhotoDiodeFlash_PosHeight");
	if(IsToCreate==1)hFile_M_PhotoDiodeFlash_PosHeight = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_PhotoDiodeFlash_PosHeight = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PhotoDiodeFlash_PosHeight == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosHeight's shared memory area.");	 exit(1);}
	M_PhotoDiodeFlash_PosHeight = (int*) MapViewOfFile(hFile_M_PhotoDiodeFlash_PosHeight, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PhotoDiodeFlash_PosHeight == NULL){	AfxMessageBox("Unable to create/open Map_M_PhotoDiodeFlash_PosHeight's VIEW."); exit(1);}
	if(IsToCreate==1)*M_PhotoDiodeFlash_PosHeight=30;
	//M_backgroundScreenRGB
	sprintf(Name_Of_Map,"Map_M_backgroundScreenRGB");
	if(IsToCreate==1)hFile_M_backgroundScreenRGB = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(COLORREF), Name_Of_Map);
	else			 hFile_M_backgroundScreenRGB = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_backgroundScreenRGB == NULL){	AfxMessageBox("Unable to create/open Map_M_backgroundScreenRGB's shared memory area.");	 exit(1);}
	M_backgroundScreenRGB = (COLORREF*) MapViewOfFile(hFile_M_backgroundScreenRGB, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_backgroundScreenRGB == NULL){	AfxMessageBox("Unable to create/open Map_M_backgroundScreenRGB's VIEW."); exit(1);}	//########  Properties of the visual object (Memory Map) ###########

	//################################################
	//M_StringMsg_fromTAS_toVIS
	sprintf(Name_Of_Map,"Map_M_StringMsg_fromTAS_toVIS");
	if(IsToCreate==1)hFile_M_StringMsg_fromTAS_toVIS = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_LEN_OF_STR_MSG * sizeof(char), Name_Of_Map);
	else			 hFile_M_StringMsg_fromTAS_toVIS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_StringMsg_fromTAS_toVIS == NULL){	
		if(IsToCreate==0){IsToCreate=1; goto REATTEMPT_IN_CREATION_MODE;} 
		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromTAS_toVIS's shared memory area.");	 exit(1);
	}	
	M_StringMsg_fromTAS_toVIS = (char*) MapViewOfFile(hFile_M_StringMsg_fromTAS_toVIS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StringMsg_fromTAS_toVIS == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromTAS_toVIS' VIEW."); exit(1);}
	//M_StringMsg_fromVIS_toTAS
	sprintf(Name_Of_Map,"Map_M_StringMsg_fromVIS_toTAS");
	if(IsToCreate==1)hFile_M_StringMsg_fromVIS_toTAS = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_LEN_OF_STR_MSG * sizeof(char), Name_Of_Map);
	else			 hFile_M_StringMsg_fromVIS_toTAS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_StringMsg_fromVIS_toTAS == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromVIS_toTAS's shared memory area.");	exit(1);	}
	M_StringMsg_fromVIS_toTAS = (char*) MapViewOfFile(hFile_M_StringMsg_fromVIS_toTAS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StringMsg_fromVIS_toTAS == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromVIS_toTAS's VIEW."); exit(1);}
	//M_StringMsg_fromTAS_toMOXY
	sprintf(Name_Of_Map,"Map_M_StringMsg_fromTAS_toMOXY");
	if(IsToCreate==1)hFile_M_StringMsg_fromTAS_toMOXY = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_LEN_OF_STR_MSG * sizeof(char), Name_Of_Map);
	else			 hFile_M_StringMsg_fromTAS_toMOXY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_StringMsg_fromTAS_toMOXY == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromTAS_toMOXY's shared memory area.");	exit(1);	}
	M_StringMsg_fromTAS_toMOXY = (char*) MapViewOfFile(hFile_M_StringMsg_fromTAS_toMOXY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StringMsg_fromTAS_toMOXY == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromTAS_toMOXY's VIEW."); exit(1);}
	//M_StringMsg_fromMOXY_toTAS
	sprintf(Name_Of_Map,"Map_M_StringMsg_fromMOXY_toTAS");
	if(IsToCreate==1)hFile_M_StringMsg_fromMOXY_toTAS = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_LEN_OF_STR_MSG * sizeof(char), Name_Of_Map);
	else			 hFile_M_StringMsg_fromMOXY_toTAS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_StringMsg_fromMOXY_toTAS == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromMOXY_toTAS's shared memory area.");	exit(1);	}
	M_StringMsg_fromMOXY_toTAS = (char*) MapViewOfFile(hFile_M_StringMsg_fromMOXY_toTAS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StringMsg_fromMOXY_toTAS == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromMOXY_toTAS's VIEW."); exit(1);}

	//M_MsgID_fromVIS_toTAS
	sprintf(Name_Of_Map,"Map_M_MsgID_fromVIS_toTAS");
	if(IsToCreate==1)hFile_M_MsgID_fromVIS_toTAS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_MsgID_fromVIS_toTAS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_MsgID_fromVIS_toTAS == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromVIS_toTAS's shared memory area.");	 exit(1);}
	M_MsgID_fromVIS_toTAS = (int*) MapViewOfFile(hFile_M_MsgID_fromVIS_toTAS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MsgID_fromVIS_toTAS == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromVIS_toTAS's VIEW."); exit(1);}
	//M_MsgID_fromTAS_toVIS
	sprintf(Name_Of_Map,"Map_M_MsgID_fromTAS_toVIS");
	if(IsToCreate==1)hFile_M_MsgID_fromTAS_toVIS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_MsgID_fromTAS_toVIS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_MsgID_fromTAS_toVIS == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromTAS_toVIS's shared memory area.");	 exit(1);}
	M_MsgID_fromTAS_toVIS = (int*) MapViewOfFile(hFile_M_MsgID_fromTAS_toVIS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MsgID_fromTAS_toVIS == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromTAS_toVIS's VIEW."); exit(1);}
	//M_Msg_from_MOXY_to_VIS
	sprintf(Name_Of_Map,"Map_M_Msg_from_MOXY_to_VIS");
	if(IsToCreate==1)hFile_M_Msg_from_MOXY_to_VIS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_Msg_from_MOXY_to_VIS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Msg_from_MOXY_to_VIS == NULL){	AfxMessageBox("Unable to create/open Map_M_Msg_from_MOXY_to_VIS's shared memory area.");	 exit(1);}
	M_Msg_from_MOXY_to_VIS = (int*) MapViewOfFile(hFile_M_Msg_from_MOXY_to_VIS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Msg_from_MOXY_to_VIS == NULL){	AfxMessageBox("Unable to create/open Map_M_Msg_from_MOXY_to_VIS's VIEW."); exit(1);}
	//M_MsgID_fromMOXY_toTAS
	sprintf(Name_Of_Map,"Map_M_MsgID_fromMOXY_toTAS");
	if(IsToCreate==1)hFile_M_MsgID_fromMOXY_toTAS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_MsgID_fromMOXY_toTAS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_MsgID_fromMOXY_toTAS == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromMOXY_toTAS's shared memory area.");	 exit(1);}
	M_MsgID_fromMOXY_toTAS = (int*) MapViewOfFile(hFile_M_MsgID_fromMOXY_toTAS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MsgID_fromMOXY_toTAS == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromMOXY_toTAS's VIEW."); exit(1);}
	//M_MsgID_fromTAS_toMOXY
	sprintf(Name_Of_Map,"Map_M_MsgID_fromTAS_toMOXY");
	if(IsToCreate==1)hFile_M_MsgID_fromTAS_toMOXY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_MsgID_fromTAS_toMOXY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_MsgID_fromTAS_toMOXY == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromTAS_toMOXY's shared memory area.");	 exit(1);}
	M_MsgID_fromTAS_toMOXY = (int*) MapViewOfFile(hFile_M_MsgID_fromTAS_toMOXY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MsgID_fromTAS_toMOXY == NULL){	AfxMessageBox("Unable to create/open Map_M_MsgID_fromTAS_toMOXY's VIEW."); exit(1);}

	//M_IsVisibleObj
	sprintf(Name_Of_Map,"Map_M_IsVisibleObj");
	if(IsToCreate==1)hFile_M_IsVisibleObj = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_VISIBLE_OBJs * sizeof(BOOL), Name_Of_Map);
	else			 hFile_M_IsVisibleObj = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_IsVisibleObj == NULL){	AfxMessageBox("Unable to create/open Map_M_IsVisibleObj's shared memory area.");	 exit(1);}
	M_IsVisibleObj = (BOOL*) MapViewOfFile(hFile_M_IsVisibleObj, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_IsVisibleObj == NULL){	AfxMessageBox("Unable to create/open Map_M_IsVisibleObj's VIEW."); exit(1);}
	if(IsToCreate==1){M_IsVisibleObj[0]=1; for(int i=1; i<MAX_VISIBLE_OBJs; i++)M_IsVisibleObj[i]=0;}
	//########  MOXY to TAS  ##########################################
	//M_MoxyUserDataSetID
	sprintf(Name_Of_Map,"Map_M_MoxyUserDataSetID");
	if(IsToCreate==1)hFile_M_MoxyUserDataSetID = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_MoxyUserDataSetID = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_MoxyUserDataSetID == NULL){		AfxMessageBox("Unable to create/open Map_M_MoxyUserDataSetID's shared memory area.");	exit(1);	}
	M_MoxyUserDataSetID = (int*) MapViewOfFile(hFile_M_MoxyUserDataSetID, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MoxyUserDataSetID == NULL){		AfxMessageBox("Unable to create/open Map_M_MoxyUserDataSetID's VIEW."); exit(1);}
	//M_MoxyUserData
	sprintf(Name_Of_Map,"Map_M_MoxyUserData");
	if(IsToCreate==1)hFile_M_MoxyUserData = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, SETS_OF_USER_VARIABLE_BLOCKS * sizeof(MoxyUserData), Name_Of_Map);
	else			 hFile_M_MoxyUserData = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_MoxyUserData == NULL){		AfxMessageBox("Unable to create/open Map_M_MoxyUserData's shared memory area.");	exit(1);	}
	M_MoxyUserData = (MoxyUserData*) MapViewOfFile(hFile_M_MoxyUserData, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MoxyUserData == NULL){		AfxMessageBox("Unable to create/open Map_M_MoxyUserData's VIEW."); exit(1);}
	//M_Dur_BigRWD
	sprintf(Name_Of_Map,"Map_M_Dur_BigRWD");
	if(IsToCreate==1)hFile_M_Dur_BigRWD = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map); //SETS_OF_USER_VARIABLE_BLOCKS is to alternate between two trials
	else			 hFile_M_Dur_BigRWD = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Dur_BigRWD == NULL){	AfxMessageBox("Unable to create/open Map_M_Dur_BigRWD's shared memory area.");	 exit(1);}
	M_Dur_BigRWD = (int*) MapViewOfFile(hFile_M_Dur_BigRWD, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Dur_BigRWD == NULL){	AfxMessageBox("Unable to create/open Map_M_Dur_BigRWD's VIEW."); exit(1);}
	//###################################################################
	//M_Is_VIS_Running
	sprintf(Name_Of_Map,"Map_M_Is_VIS_Running");
	if(IsToCreate==1)hFile_M_Is_VIS_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_Is_VIS_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Is_VIS_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_Is_VIS_Running' shared memory area.");	 exit(1);}
	M_Is_VIS_Running = (int*) MapViewOfFile(hFile_M_Is_VIS_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Is_VIS_Running == NULL){	AfxMessageBox("Unable to create/open Map_M_Is_VIS_Running' VIEW."); exit(1);}
	//M_Is_TAS_Running_4MOXYVIS
	sprintf(Name_Of_Map,"Map_M_Is_TAS_Running_4MOXYVIS");
	if(IsToCreate==1)hFile_M_Is_TAS_Running_4MOXYVIS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_Is_TAS_Running_4MOXYVIS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Is_TAS_Running_4MOXYVIS == NULL){	AfxMessageBox("Unable to create/open Map_M_Is_TAS_Running_4MOXYVIS' shared memory area.");	 exit(1);}
	M_Is_TAS_Running_4MOXYVIS = (int*) MapViewOfFile(hFile_M_Is_TAS_Running_4MOXYVIS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Is_TAS_Running_4MOXYVIS == NULL){	AfxMessageBox("Unable to create/open Map_M_Is_TAS_Running_4MOXYVIS' VIEW."); exit(1);}
	//M_N
	sprintf(Name_Of_Map,"Map_M_N");
	if(IsToCreate==1)hFile_M_N = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_N = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_N == NULL){	AfxMessageBox("Unable to create/open Map_M_N's shared memory area.");	 exit(1);}
	M_N = (int*) MapViewOfFile(hFile_M_N, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_N == NULL){	AfxMessageBox("Unable to create/open Map_M_N's VIEW."); exit(1);}
	//M_VisObjIDs
	sprintf(Name_Of_Map,"Map_M_VisObjIDs");
	if(IsToCreate==1)hFile_M_VisObjIDs = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
	else			 hFile_M_VisObjIDs = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_VisObjIDs == NULL){	AfxMessageBox("Unable to create/open Map_M_VisObjIDs' shared memory area.");	 exit(1);}
	M_VisObjIDs = (int*) MapViewOfFile(hFile_M_VisObjIDs, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_VisObjIDs == NULL){	AfxMessageBox("Unable to create/open Map_M_VisObjIDs' VIEW."); exit(1);}
	//M_UserClickedVisObjIDs
	sprintf(Name_Of_Map,"Map_M_UserClickedVisObjIDs");
	if(IsToCreate==1)hFile_M_UserClickedVisObjIDs = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
	else			 hFile_M_UserClickedVisObjIDs = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_UserClickedVisObjIDs == NULL){	AfxMessageBox("Unable to create/open M_UserClickedVisObjIDs' shared memory area.");	 exit(1);}
	M_UserClickedVisObjIDs = (int*) MapViewOfFile(hFile_M_UserClickedVisObjIDs, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_UserClickedVisObjIDs == NULL){	AfxMessageBox("Unable to create/open M_UserClickedVisObjIDs' VIEW."); exit(1);}
	//M_UserClickedN
	sprintf(Name_Of_Map,"Map_M_UserClickedN");
	if(IsToCreate==1)hFile_M_UserClickedN = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
	else			 hFile_M_UserClickedN = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_UserClickedN == NULL){	AfxMessageBox("Unable to create/open Map_M_UserClickedN's shared memory area.");	 exit(1);}
	M_UserClickedN = (int*) MapViewOfFile(hFile_M_UserClickedN, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_UserClickedN == NULL){	AfxMessageBox("Unable to create/open Map_M_UserClickedN's VIEW."); exit(1);}
	//M_CLOSE_VIS
	sprintf(Name_Of_Map,"Map_M_CLOSE_VIS");
	if(IsToCreate==1)hFile_M_CLOSE_VIS = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
	else			 hFile_M_CLOSE_VIS = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_CLOSE_VIS == NULL){	AfxMessageBox("Unable to create/open Map_M_CLOSE_VIS's shared memory area.");	 exit(1);}
	M_CLOSE_VIS = (int*) MapViewOfFile(hFile_M_CLOSE_VIS, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_CLOSE_VIS == NULL){	AfxMessageBox("Unable to create/open Map_M_CLOSE_VIS's VIEW."); exit(1);}
	if(IsToCreate==1)*M_CLOSE_VIS=0;

	//########### Eye Position
	//M_PosFigMinX
	sprintf(Name_Of_Map,"Map_M_PosFigMinX");
	if(IsToCreate==1)hFile_M_PosFigMinX = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), Name_Of_Map);
	else			 hFile_M_PosFigMinX = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PosFigMinX == NULL){	AfxMessageBox("Unable to create/open Map_M_PosFigMinX's shared memory area.");	 exit(1);}
	M_PosFigMinX = (double*) MapViewOfFile(hFile_M_PosFigMinX, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PosFigMinX == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_PosFigMaxX
	sprintf(Name_Of_Map,"Map_M_PosFigMaxX");
	if(IsToCreate==1)hFile_M_PosFigMaxX = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), Name_Of_Map);
	else			 hFile_M_PosFigMaxX = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PosFigMaxX == NULL){	AfxMessageBox("Unable to create/open Map_M_PosFigMaxX's shared memory area.");	 exit(1);}
	M_PosFigMaxX = (double*) MapViewOfFile(hFile_M_PosFigMaxX, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PosFigMaxX == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_PosFigMinY
	sprintf(Name_Of_Map,"Map_M_PosFigMinY");
	if(IsToCreate==1)hFile_M_PosFigMinY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), Name_Of_Map);
	else			 hFile_M_PosFigMinY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PosFigMinY == NULL){	AfxMessageBox("Unable to create/open Map_M_PosFigMinY's shared memory area.");	 exit(1);}
	M_PosFigMinY = (double*) MapViewOfFile(hFile_M_PosFigMinY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PosFigMinY == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_PosFigMaxY
	sprintf(Name_Of_Map,"Map_M_PosFigMaxY");
	if(IsToCreate==1)hFile_M_PosFigMaxY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), Name_Of_Map);
	else			 hFile_M_PosFigMaxY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PosFigMaxY == NULL){	AfxMessageBox("Unable to create/open Map_M_PosFigMaxY's shared memory area.");	 exit(1);}
	M_PosFigMaxY = (double*) MapViewOfFile(hFile_M_PosFigMaxY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PosFigMaxY == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EyeX_rawData
	sprintf(Name_Of_Map,"Map_M_EyeX_rawData");
	if(IsToCreate==1)hFile_M_EyeX_rawData = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeX_rawData = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeX_rawData == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeX_rawData's shared memory area.");	 exit(1);}
	M_EyeX_rawData = (double*) MapViewOfFile(hFile_M_EyeX_rawData, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeX_rawData == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EyeY_rawData
	sprintf(Name_Of_Map,"Map_M_EyeY_rawData");
	if(IsToCreate==1)hFile_M_EyeY_rawData = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeY_rawData = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeY_rawData == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeY_rawData's shared memory area.");	 exit(1);}
	M_EyeY_rawData = (double*) MapViewOfFile(hFile_M_EyeY_rawData, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeY_rawData == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EyeX_Bias
	sprintf(Name_Of_Map,"Map_M_EyeX_Bias");
	if(IsToCreate==1)hFile_M_EyeX_Bias = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeX_Bias = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeX_Bias == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeX_Bias's shared memory area.");	 exit(1);}
	M_EyeX_Bias = (double*) MapViewOfFile(hFile_M_EyeX_Bias, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeX_Bias == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1){for(int i=0; i<NUM_2D_BEHAVIOR_OBJS; i++) M_EyeX_Bias[i]=0;}
	//M_EyeY_Bias
	sprintf(Name_Of_Map,"Map_M_EyeY_Bias");
	if(IsToCreate==1)hFile_M_EyeY_Bias = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeY_Bias = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeY_Bias == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeY_Bias's shared memory area.");	 exit(1);}
	M_EyeY_Bias = (double*) MapViewOfFile(hFile_M_EyeY_Bias, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeY_Bias == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1){for(int i=0; i<NUM_2D_BEHAVIOR_OBJS; i++) M_EyeY_Bias[i]=0;}
	//M_EyeX_Gain
	sprintf(Name_Of_Map,"Map_M_EyeX_Gain");
	if(IsToCreate==1)hFile_M_EyeX_Gain = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeX_Gain = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeX_Gain == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeX_Gain's shared memory area.");	 exit(1);}
	M_EyeX_Gain = (double*) MapViewOfFile(hFile_M_EyeX_Gain, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeX_Gain == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1){for(int i=0; i<NUM_2D_BEHAVIOR_OBJS; i++) M_EyeX_Gain[i]=1;}
	//M_EyeY_Gain
	sprintf(Name_Of_Map,"Map_M_EyeY_Gain");
	if(IsToCreate==1)hFile_M_EyeY_Gain = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeY_Gain = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeY_Gain == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeY_Gain's shared memory area.");	 exit(1);}
	M_EyeY_Gain = (double*) MapViewOfFile(hFile_M_EyeY_Gain, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeY_Gain == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1){for(int i=0; i<NUM_2D_BEHAVIOR_OBJS; i++) M_EyeY_Gain[i]=1;}
	//M_EyePosX
	sprintf(Name_Of_Map,"Map_M_EyePosX");
	if(IsToCreate==1)hFile_M_EyePosX = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyePosX = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyePosX == NULL){	AfxMessageBox("Unable to create/open Map_M_EyePosX's shared memory area.");	 exit(1);}
	M_EyePosX = (double*) MapViewOfFile(hFile_M_EyePosX, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyePosX == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EyePosY
	sprintf(Name_Of_Map,"Map_M_EyePosY");
	if(IsToCreate==1)hFile_M_EyePosY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyePosY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyePosY == NULL){	AfxMessageBox("Unable to create/open Map_M_EyePosY's shared memory area.");	 exit(1);}
	M_EyePosY = (double*) MapViewOfFile(hFile_M_EyePosY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyePosY == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EyeXsimulalted
	sprintf(Name_Of_Map,"Map_M_EyeXsimulalted");
	if(IsToCreate==1)hFile_M_EyeXsimulalted = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeXsimulalted = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeXsimulalted == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeXsimulalted's shared memory area.");	 exit(1);}
	M_EyeXsimulalted = (double*) MapViewOfFile(hFile_M_EyeXsimulalted, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeXsimulalted == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_EyeYsimulalted
	sprintf(Name_Of_Map,"Map_M_EyeYsimulalted");
	if(IsToCreate==1)hFile_M_EyeYsimulalted = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_2D_BEHAVIOR_OBJS * sizeof(double), Name_Of_Map);
	else			 hFile_M_EyeYsimulalted = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EyeYsimulalted == NULL){	AfxMessageBox("Unable to create/open Map_M_EyeYsimulalted's shared memory area.");	 exit(1);}
	M_EyeYsimulalted = (double*) MapViewOfFile(hFile_M_EyeYsimulalted, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EyeYsimulalted == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_JustClicked
	sprintf(Name_Of_Map,"Map_M_JustClicked");
	if(IsToCreate==1)hFile_M_JustClicked = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_JustClicked = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_JustClicked == NULL){	AfxMessageBox("Unable to create/open Map_M_JustClicked's shared memory area.");	 exit(1);}
	M_JustClicked = (int*) MapViewOfFile(hFile_M_JustClicked, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_JustClicked == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_PositionWinds
	sprintf(Name_Of_Map,"Map_M_PositionWinds");
	if(IsToCreate==1)hFile_M_PositionWinds = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_EYE_WIND * sizeof(PositionWindows), Name_Of_Map);
	else			 hFile_M_PositionWinds = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PositionWinds == NULL){	AfxMessageBox("Unable to create/open Map_M_PositionWinds's shared memory area.");	 exit(1);}
	M_PositionWinds = (PositionWindows*) MapViewOfFile(hFile_M_PositionWinds, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PositionWinds == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_WindToDisplayOrNot
	sprintf(Name_Of_Map,"Map_M_WindToDisplayOrNot");
	if(IsToCreate==1)hFile_M_WindToDisplayOrNot = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_WindToDisplayOrNot = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_WindToDisplayOrNot == NULL){	AfxMessageBox("Unable to create/open Map_M_WindToDisplayOrNot's shared memory area.");	 exit(1);}
	M_WindToDisplayOrNot = (int*) MapViewOfFile(hFile_M_WindToDisplayOrNot, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_WindToDisplayOrNot == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_BehavioralObjectForXYmonitor
	sprintf(Name_Of_Map,"Map_M_BehavioralObjectForXYmonitor");
	if(IsToCreate==1)hFile_M_BehavioralObjectForXYmonitor = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_BehavioralObjectForXYmonitor = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_BehavioralObjectForXYmonitor == NULL){	AfxMessageBox("Unable to create/open Map_M_BehavioralObjectForXYmonitor's shared memory area.");	 exit(1);}
	M_BehavioralObjectForXYmonitor = (int*) MapViewOfFile(hFile_M_BehavioralObjectForXYmonitor, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_BehavioralObjectForXYmonitor == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_DIO_State
	sprintf(Name_Of_Map,"Map_M_DIO_State");
	if(IsToCreate==1)hFile_M_DIO_State = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_DIO_LEDS * sizeof(int), Name_Of_Map);
	else			 hFile_M_DIO_State = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_DIO_State == NULL){	AfxMessageBox("Unable to create/open Map_M_DIO_State's shared memory area.");	 exit(1);}
	M_DIO_State = (int*) MapViewOfFile(hFile_M_DIO_State, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DIO_State == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//########  Ecode & alias  ##########################################
	//M_EcodeTable
	sprintf(Name_Of_Map,"Map_M_EcodeTable");
	if(IsToCreate==1)hFile_M_EcodeTable = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_OF_ECODES_IN_ALIAS_TABLE * sizeof(EcodeTable), Name_Of_Map);
	else			 hFile_M_EcodeTable = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_EcodeTable == NULL){	AfxMessageBox("Unable to create/open Map_M_EcodeTable's shared memory area.");	 exit(1);}
	M_EcodeTable = (EcodeTable*) MapViewOfFile(hFile_M_EcodeTable, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_EcodeTable == NULL){	AfxMessageBox("Unable to create/open Map_M_EcodeTable's VIEW."); exit(1);}
	//M_NumEcodesInTable
	sprintf(Name_Of_Map,"Map_M_NumEcodesInTable");
	if(IsToCreate==1)hFile_M_NumEcodesInTable = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_OF_ECODES_IN_ALIAS_TABLE * sizeof(int), Name_Of_Map);
	else			 hFile_M_NumEcodesInTable = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_NumEcodesInTable == NULL){	AfxMessageBox("Unable to create/open Map_M_NumEcodesInTable's shared memory area.");	 exit(1);}
	M_NumEcodesInTable = (int*) MapViewOfFile(hFile_M_NumEcodesInTable, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumEcodesInTable == NULL){	AfxMessageBox("Unable to create/open Map_M_NumEcodesInTable's VIEW."); exit(1);}
	//M_IsTriggeredMode
	sprintf(Name_Of_Map,"Map_M_IsTriggeredMode");
	if(IsToCreate==1)hFile_M_IsTriggeredMode = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_RL_PANELS * sizeof(int), Name_Of_Map);
	else			 hFile_M_IsTriggeredMode = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_IsTriggeredMode == NULL){	AfxMessageBox("Unable to create/open Map_M_IsTriggeredMode's shared memory area.");	 exit(1);}
	M_IsTriggeredMode = (int*) MapViewOfFile(hFile_M_IsTriggeredMode, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_IsTriggeredMode == NULL){	AfxMessageBox("Unable to create/open Map_M_IsTriggeredMode's VIEW."); exit(1);}
	//M_nTriggerECode
	sprintf(Name_Of_Map,"Map_M_nTriggerECode");
	if(IsToCreate==1)hFile_M_nTriggerECode = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_RL_PANELS * sizeof(int), Name_Of_Map);
	else			 hFile_M_nTriggerECode = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_nTriggerECode == NULL){	AfxMessageBox("Unable to create/open Map_M_nTriggerECode's shared memory area.");	 exit(1);}
	M_nTriggerECode = (int*) MapViewOfFile(hFile_M_nTriggerECode, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_nTriggerECode == NULL){	AfxMessageBox("Unable to create/open Map_M_nTriggerECode's VIEW."); exit(1);}
	//M_hasTriggered
	sprintf(Name_Of_Map,"Map_M_hasTriggered");
	if(IsToCreate==1)hFile_M_hasTriggered = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_RL_PANELS * sizeof(int), Name_Of_Map);
	else			 hFile_M_hasTriggered = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_hasTriggered == NULL){	AfxMessageBox("Unable to create/open Map_M_hasTriggered's shared memory area.");	 exit(1);}
	M_hasTriggered = (int*) MapViewOfFile(hFile_M_hasTriggered, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_hasTriggered == NULL){	AfxMessageBox("Unable to create/open Map_M_hasTriggered's VIEW."); exit(1);}
	//M_TriggeredTime_ms
	sprintf(Name_Of_Map,"Map_M_TriggeredTime_ms");
	if(IsToCreate==1)hFile_M_TriggeredTime_ms = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, NUM_RL_PANELS * sizeof(__int64), Name_Of_Map);
	else			 hFile_M_TriggeredTime_ms = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_TriggeredTime_ms == NULL){	AfxMessageBox("Unable to create/open Map_M_TriggeredTime_ms's shared memory area.");	 exit(1);}
	M_TriggeredTime_ms = (__int64*) MapViewOfFile(hFile_M_TriggeredTime_ms, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_TriggeredTime_ms == NULL){	AfxMessageBox("Unable to create/open Map_M_TriggeredTime_ms's VIEW."); exit(1);}
	//####################  File Names  ##########################################
	//M_PreFix
	sprintf(Name_Of_Map,"Map_M_PreFix");
	if(IsToCreate==1)hFile_M_PreFix = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, (MAX_LEN_FILE_NAME/3) * sizeof(char), Name_Of_Map);
	else			 hFile_M_PreFix = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_PreFix == NULL){	AfxMessageBox("Unable to create/open Map_M_PreFix's shared memory area.");	 exit(1);}
	M_PreFix = (char*) MapViewOfFile(hFile_M_PreFix, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_PreFix == NULL){	AfxMessageBox("Unable to create/open Map_M_PreFix's VIEW."); exit(1);}
	//M_SuFFix
	sprintf(Name_Of_Map,"Map_M_SuFFix");
	if(IsToCreate==1)hFile_M_SuFFix = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, (MAX_LEN_FILE_NAME/3) * sizeof(char), Name_Of_Map);
	else			 hFile_M_SuFFix = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_SuFFix == NULL){	AfxMessageBox("Unable to create/open Map_M_SuFFix's shared memory area.");	 exit(1);}
	M_SuFFix = (char*) MapViewOfFile(hFile_M_SuFFix, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_SuFFix == NULL){	AfxMessageBox("Unable to create/open Map_M_SuFFix's VIEW."); exit(1);}
	//M_BaseOfFileName
	sprintf(Name_Of_Map,"Map_M_BaseOfFileName");
	if(IsToCreate==1)hFile_M_BaseOfFileName = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_LEN_FILE_NAME * sizeof(char), Name_Of_Map);
	else			 hFile_M_BaseOfFileName = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_BaseOfFileName == NULL){	AfxMessageBox("Unable to create/open Map_M_BaseOfFileName's shared memory area.");	 exit(1);}
	M_BaseOfFileName = (char*) MapViewOfFile(hFile_M_BaseOfFileName, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_BaseOfFileName == NULL){	AfxMessageBox("Unable to create/open Map_M_BaseOfFileName's VIEW."); exit(1);}
	//M_JustFileName
	sprintf(Name_Of_Map,"Map_M_JustFileName");
	if(IsToCreate==1)hFile_M_JustFileName = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_LEN_FILE_NAME * sizeof(char), Name_Of_Map);
	else			 hFile_M_JustFileName = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_JustFileName == NULL){	AfxMessageBox("Unable to create/open Map_M_JustFileName's shared memory area.");	 exit(1);}
	M_JustFileName = (char*) MapViewOfFile(hFile_M_JustFileName, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_JustFileName == NULL){	AfxMessageBox("Unable to create/open Map_M_JustFileName's VIEW."); exit(1);}
	//M_isSaving
	sprintf(Name_Of_Map,"Map_M_isSaving");
	if(IsToCreate==1)hFile_M_isSaving = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0,  sizeof(int), Name_Of_Map);
	else			 hFile_M_isSaving = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_isSaving == NULL){	AfxMessageBox("Unable to create/open Map_M_isSaving's shared memory area.");	 exit(1);}
	M_isSaving = (int*) MapViewOfFile(hFile_M_isSaving, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isSaving == NULL){	AfxMessageBox("Unable to create/open Map_M_isSaving's VIEW."); exit(1);}
	//M_isRenamed
	sprintf(Name_Of_Map,"Map_M_isRenamed");
	if(IsToCreate==1)hFile_M_isRenamed = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0,  sizeof(int), Name_Of_Map);
	else			 hFile_M_isRenamed = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_isRenamed == NULL){	AfxMessageBox("Unable to create/open Map_M_isRenamed's shared memory area.");	 exit(1);}
	M_isRenamed = (int*) MapViewOfFile(hFile_M_isRenamed, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isRenamed == NULL){	AfxMessageBox("Unable to create/open Map_M_isRenamed's VIEW."); exit(1);}
	//########  User Picked Variables  ##########################################
	//M_VarToDisp
	sprintf(Name_Of_Map,"Map_M_VarToDisp");
	if(IsToCreate==1)hFile_M_VarToDisp = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_USER_PICKED_VARS * sizeof(UserPickedVarToDisp), Name_Of_Map);
	else			 hFile_M_VarToDisp = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_VarToDisp == NULL){	AfxMessageBox("Unable to create/open Map_M_VarToDisp's shared memory area.");	 exit(1);}
	M_VarToDisp = (UserPickedVarToDisp*) MapViewOfFile(hFile_M_VarToDisp, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_VarToDisp == NULL){	AfxMessageBox("Unable to create/open Map_M_VarToDisp's VIEW."); exit(1);}
	//M_NumUserPickedVariables
	sprintf(Name_Of_Map,"Map_M_NumUserPickedVariables");
	if(IsToCreate==1)hFile_M_NumUserPickedVariables = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_NumUserPickedVariables = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_NumUserPickedVariables == NULL){	AfxMessageBox("Unable to create/open Map_M_NumUserPickedVariables's shared memory area.");	 exit(1);}
	M_NumUserPickedVariables = (int*) MapViewOfFile(hFile_M_NumUserPickedVariables, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumUserPickedVariables == NULL){	AfxMessageBox("Unable to create/open Map_M_NumUserPickedVariables's VIEW."); exit(1);}
	//M_VarToDispLineEnd
	sprintf(Name_Of_Map,"Map_M_VarToDispLineEnd");
	if(IsToCreate==1)hFile_M_VarToDispLineEnd = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_VarToDispLineEnd = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_VarToDispLineEnd == NULL){	AfxMessageBox("Unable to create/open Map_M_VarToDispLineEnd's shared memory area.");	 exit(1);}
	M_VarToDispLineEnd = (int*) MapViewOfFile(hFile_M_VarToDispLineEnd, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_VarToDispLineEnd == NULL){	AfxMessageBox("Unable to create/open Map_M_VarToDispLineEnd's VIEW."); exit(1);}
	//M_VarRECT
	sprintf(Name_Of_Map,"Map_M_VarRECT");
	if(IsToCreate==1)hFile_M_VarRECT = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_NUM_USER_PICKED_VARS * sizeof(CRect), Name_Of_Map);
	else			 hFile_M_VarRECT = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_VarRECT == NULL){	AfxMessageBox("Unable to create/open Map_M_VarRECT's shared memory area.");	 exit(1);}
	M_VarRECT = (CRect*) MapViewOfFile(hFile_M_VarRECT, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_VarRECT == NULL){	AfxMessageBox("Unable to create/open Map_M_VarRECT's VIEW."); exit(1);}

	//########   Message queue from TAS to MOXY
	//M_Msg_Q_Idx
	sprintf(Name_Of_Map,"Map_M_Msg_Q_Idx");
	if(IsToCreate==1)hFile_M_Msg_Q_Idx = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
	else			 hFile_M_Msg_Q_Idx = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_Msg_Q_Idx == NULL){	AfxMessageBox("Unable to create/open Map_M_Msg_Q_Idx's shared memory area.");	 exit(1);}
	M_Msg_Q_Idx = (int*) MapViewOfFile(hFile_M_Msg_Q_Idx, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Msg_Q_Idx == NULL){	AfxMessageBox("Unable to create/open Map_M_Msg_Q_Idx's VIEW."); exit(1);}
	//M_StringMsg_fromTAS_toMOXY
	sprintf(Name_Of_Map,"Map_M_StrQ_fromTAS_toMOXY");
	if(IsToCreate==1)hFile_M_StringMsg_fromTAS_toMOXY = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_LEN_OF_STR_MSG * sizeof(char), Name_Of_Map);
	else			 hFile_M_StringMsg_fromTAS_toMOXY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
	if(hFile_M_StringMsg_fromTAS_toMOXY == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromTAS_toMOXY's shared memory area.");	exit(1);	}
	M_StringMsg_fromTAS_toMOXY = (char*) MapViewOfFile(hFile_M_StringMsg_fromTAS_toMOXY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_StringMsg_fromTAS_toMOXY == NULL){		AfxMessageBox("Unable to create/open Map_M_StringMsg_fromTAS_toMOXY's VIEW."); exit(1);}
	for(int i=0;i<SIZE_OF_MSG_Q;i++){
		//M_StrQ_fromTAS_toMOXY[i]
		sprintf(Name_Of_Map,"Map_M_StrQ_fromTAS_toMOXY[%d]",i);
		if(IsToCreate==1)hFile_M_StrQ_fromTAS_toMOXY[i] = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,PAGE_READWRITE, 0, MAX_LEN_OF_STR_MSG * sizeof(char), Name_Of_Map);
		else			 hFile_M_StrQ_fromTAS_toMOXY[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
		if(hFile_M_StrQ_fromTAS_toMOXY[i] == NULL){		AfxMessageBox("Unable to create/open Map_M_StrQ_fromTAS_toMOXY[i]'s shared memory area.");	exit(1);	}
		M_StrQ_fromTAS_toMOXY[i] = (char*) MapViewOfFile(hFile_M_StrQ_fromTAS_toMOXY[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_StrQ_fromTAS_toMOXY[i] == NULL){		AfxMessageBox("Unable to create/open Map_M_StrQ_fromTAS_toMOXY[i]'s VIEW."); exit(1);}
		//M_VisObjID_Q_fromTAS_toMOXY[i]
		sprintf(Name_Of_Map,"Map_M_VisObjID_Q_fromTAS_toMOXY[%d]",i);
		if(IsToCreate==1)hFile_M_VisObjID_Q_fromTAS_toMOXY[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_STIM_OBJ * sizeof(int), Name_Of_Map);
		else			 hFile_M_VisObjID_Q_fromTAS_toMOXY[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
		if(hFile_M_VisObjID_Q_fromTAS_toMOXY[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_VisObjID_Q_fromTAS_toMOXY[i]'s shared memory area.");	 exit(1);}
		M_VisObjID_Q_fromTAS_toMOXY[i] = (int*) MapViewOfFile(hFile_M_VisObjID_Q_fromTAS_toMOXY[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_VisObjID_Q_fromTAS_toMOXY[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
		//M_N_Q_fromTAS_toMOXY[i]
		sprintf(Name_Of_Map,"Map_M_N_Q_fromTAS_toMOXY[%d]",i);
		if(IsToCreate==1)hFile_M_N_Q_fromTAS_toMOXY[i] = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), Name_Of_Map);
		else			 hFile_M_N_Q_fromTAS_toMOXY[i] = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name_Of_Map); 
		if(hFile_M_N_Q_fromTAS_toMOXY[i] == NULL){	AfxMessageBox("Unable to create/open Map_M_N_Q_fromTAS_toMOXY[i]'s shared memory area.");	 exit(1);}
		M_N_Q_fromTAS_toMOXY[i] = (int*) MapViewOfFile(hFile_M_N_Q_fromTAS_toMOXY[i], FILE_MAP_ALL_ACCESS,  	0,	0,	0);
		if(M_N_Q_fromTAS_toMOXY[i] == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	}