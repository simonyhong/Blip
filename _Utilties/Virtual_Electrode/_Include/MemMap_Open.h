	int i;
	char NameOfMap[256];
	//##################  Parameters and variables  ##################################
RESTART_IN_CREATION_MODE:
	//M_is3D_Running
	sprintf(NameOfMap,"Map_M_is3D_Running");
	if(IsToCreate==1)hFile_M_is3D_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_is3D_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_is3D_Running == NULL){ 
		if(IsToCreate==0){IsToCreate=1; goto RESTART_IN_CREATION_MODE;} 
		AfxMessageBox("Unable to create/open Map_M_is3D_Running's shared memory area.");	 exit(1);
	}
	M_is3D_Running = (int*) MapViewOfFile(hFile_M_is3D_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_is3D_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}

	//M_isCoronal_Running
	sprintf(NameOfMap,"Map_M_isCoronal_Running");
	if(IsToCreate==1)hFile_M_isCoronal_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isCoronal_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isCoronal_Running == NULL){AfxMessageBox("Unable to create/open Map_M_isCoronal_Running's shared memory area.");	 exit(1);}
	M_isCoronal_Running = (int*) MapViewOfFile(hFile_M_isCoronal_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isCoronal_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isCoronal_Running = 0;
	//M_isSagittal_Running
	sprintf(NameOfMap,"Map_M_isSagittal_Running");
	if(IsToCreate==1)hFile_M_isSagittal_Running = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isSagittal_Running = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isSagittal_Running == NULL){AfxMessageBox("Unable to create/open Map_M_isSagittal_Running's shared memory area.");	 exit(1);}
	M_isSagittal_Running = (int*) MapViewOfFile(hFile_M_isSagittal_Running, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isSagittal_Running == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isSagittal_Running = 0;
	//M_HeyCoronalFigure_RedrawElectrode
	sprintf(NameOfMap,"Map_M_HeyCoronalFigure_3DRequest_toRedrawElectrode");
	if(IsToCreate==1)hFile_M_HeyCoronalFigure_3DRequest_toRedrawElectrode = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeyCoronalFigure_3DRequest_toRedrawElectrode = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeyCoronalFigure_3DRequest_toRedrawElectrode == NULL){AfxMessageBox("Unable to create/open Map_M_HeyCoronalFigure_3DRequest_toRedrawElectrode's shared memory area.");	 exit(1);}
	M_HeyCoronalFigure_RedrawElectrode = (int*) MapViewOfFile(hFile_M_HeyCoronalFigure_3DRequest_toRedrawElectrode, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeyCoronalFigure_RedrawElectrode == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_HeyCoronalFigure_RedrawElectrode = 0;
	//M_HeySagittalFigure_RedrawElectrode
	sprintf(NameOfMap,"Map_M_HeySagittalFigure_3DRequest_toRedrawElectrode");
	if(IsToCreate==1)hFile_M_HeySagittalFigure_3DRequest_toRedrawElectrode = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_HeySagittalFigure_3DRequest_toRedrawElectrode = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_HeySagittalFigure_3DRequest_toRedrawElectrode == NULL){AfxMessageBox("Unable to create/open Map_M_HeySagittalFigure_3DRequest_toRedrawElectrode's shared memory area.");	 exit(1);}
	M_HeySagittalFigure_RedrawElectrode = (int*) MapViewOfFile(hFile_M_HeySagittalFigure_3DRequest_toRedrawElectrode, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_HeySagittalFigure_RedrawElectrode == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_HeySagittalFigure_RedrawElectrode = 0;
	//M_DrawingFileInfo
	sprintf(NameOfMap,"Map_M_DrawingFileInfo");
	if(IsToCreate==1)hFile_M_DrawingFileInfo = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, Max_Slices*sizeof(FileInfo), NameOfMap);
	else			 hFile_M_DrawingFileInfo = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_DrawingFileInfo == NULL){AfxMessageBox("Unable to create/open Map_M_DrawingFileInfo's shared memory area.");	 exit(1);}
	M_DrawingFileInfo = (FileInfo*) MapViewOfFile(hFile_M_DrawingFileInfo, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DrawingFileInfo == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_DataDir
	sprintf(NameOfMap,"Map_M_DataDir");
	if(IsToCreate==1)hFile_M_DataDir = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1024*sizeof(char), NameOfMap);
	else			 hFile_M_DataDir = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_DataDir == NULL){AfxMessageBox("Unable to create/open Map_M_DataDir's shared memory area.");	 exit(1);}
	M_DataDir = (char*) MapViewOfFile(hFile_M_DataDir, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_DataDir == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_MsgToNewWindow
	sprintf(NameOfMap,"Map_M_MsgToNewWindow");
	if(IsToCreate==1)hFile_M_MsgToNewWindow = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1024*sizeof(char), NameOfMap);
	else			 hFile_M_MsgToNewWindow = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_MsgToNewWindow == NULL){AfxMessageBox("Unable to create/open Map_M_MsgToNewWindow's shared memory area.");	 exit(1);}
	M_MsgToNewWindow = (char*) MapViewOfFile(hFile_M_MsgToNewWindow, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MsgToNewWindow == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)M_MsgToNewWindow[0] ='\0';
	//M_ElectrodeTipX_3DAtlas
	sprintf(NameOfMap,"Map_M_ElectrodeTipX_3DAtlas");
	if(IsToCreate==1)hFile_M_ElectrodeTipX_3DAtlas = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_ElectrodeTipX_3DAtlas = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ElectrodeTipX_3DAtlas == NULL){AfxMessageBox("Unable to create/open Map_M_ElectrodeTipX_3DAtlas's shared memory area.");	 exit(1);}
	M_ElectrodeTipX_3DAtlas = (double*) MapViewOfFile(hFile_M_ElectrodeTipX_3DAtlas, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ElectrodeTipX_3DAtlas == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_ElectrodeTipY_3DAtlas
	sprintf(NameOfMap,"Map_M_ElectrodeTipY_3DAtlas");
	if(IsToCreate==1)hFile_M_ElectrodeTipY_3DAtlas = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_ElectrodeTipY_3DAtlas = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ElectrodeTipY_3DAtlas == NULL){AfxMessageBox("Unable to create/open Map_M_ElectrodeTipY_3DAtlas's shared memory area.");	 exit(1);}
	M_ElectrodeTipY_3DAtlas = (double*) MapViewOfFile(hFile_M_ElectrodeTipY_3DAtlas, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ElectrodeTipY_3DAtlas == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_ElectrodeTipZ_3DAtlas
	sprintf(NameOfMap,"Map_M_ElectrodeTipZ_3DAtlas");
	if(IsToCreate==1)hFile_M_ElectrodeTipZ_3DAtlas = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_ElectrodeTipZ_3DAtlas = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ElectrodeTipZ_3DAtlas == NULL){AfxMessageBox("Unable to create/open Map_M_ElectrodeTipZ_3DAtlas's shared memory area.");	 exit(1);}
	M_ElectrodeTipZ_3DAtlas = (double*) MapViewOfFile(hFile_M_ElectrodeTipZ_3DAtlas, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ElectrodeTipZ_3DAtlas == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_ElectrodeTopX_3DAtlas
	sprintf(NameOfMap,"Map_M_ElectrodeTopX_3DAtlas");
	if(IsToCreate==1)hFile_M_ElectrodeTopX_3DAtlas = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_ElectrodeTopX_3DAtlas = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ElectrodeTopX_3DAtlas == NULL){AfxMessageBox("Unable to create/open Map_M_ElectrodeTopX_3DAtlas's shared memory area.");	 exit(1);}
	M_ElectrodeTopX_3DAtlas = (double*) MapViewOfFile(hFile_M_ElectrodeTopX_3DAtlas, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ElectrodeTopX_3DAtlas == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_ElectrodeTopY_3DAtlas
	sprintf(NameOfMap,"Map_M_ElectrodeTopY_3DAtlas");
	if(IsToCreate==1)hFile_M_ElectrodeTopY_3DAtlas = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_ElectrodeTopY_3DAtlas = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ElectrodeTopY_3DAtlas == NULL){AfxMessageBox("Unable to create/open Map_M_ElectrodeTopY_3DAtlas's shared memory area.");	 exit(1);}
	M_ElectrodeTopY_3DAtlas = (double*) MapViewOfFile(hFile_M_ElectrodeTopY_3DAtlas, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ElectrodeTopY_3DAtlas == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_ElectrodeTopZ_3DAtlas
	sprintf(NameOfMap,"Map_M_ElectrodeTopZ_3DAtlas");
	if(IsToCreate==1)hFile_M_ElectrodeTopZ_3DAtlas = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_ElectrodeTopZ_3DAtlas = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_ElectrodeTopZ_3DAtlas == NULL){AfxMessageBox("Unable to create/open Map_M_ElectrodeTopZ_3DAtlas's shared memory area.");	 exit(1);}
	M_ElectrodeTopZ_3DAtlas = (double*) MapViewOfFile(hFile_M_ElectrodeTopZ_3DAtlas, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_ElectrodeTopZ_3DAtlas == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Pitch
	sprintf(NameOfMap,"Map_M_Grid_Pitch");
	if(IsToCreate==1)hFile_M_Grid_Pitch = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Pitch = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Pitch == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Pitch's shared memory area.");	 exit(1);}
	M_Grid_Pitch = (double*) MapViewOfFile(hFile_M_Grid_Pitch, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Pitch == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Roll
	sprintf(NameOfMap,"Map_M_Grid_Roll");
	if(IsToCreate==1)hFile_M_Grid_Roll = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Roll = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Roll == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Roll's shared memory area.");	 exit(1);}
	M_Grid_Roll = (double*) MapViewOfFile(hFile_M_Grid_Roll, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Roll == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Yaw
	sprintf(NameOfMap,"Map_M_Grid_Yaw");
	if(IsToCreate==1)hFile_M_Grid_Yaw = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Yaw = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Yaw == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Yaw's shared memory area.");	 exit(1);}
	M_Grid_Yaw = (double*) MapViewOfFile(hFile_M_Grid_Yaw, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Yaw == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	
	//M_Grid_X_unit_Vector_x
	sprintf(NameOfMap,"Map_M_Grid_X_unit_Vector_x");
	if(IsToCreate==1)hFile_M_Grid_X_unit_Vector_x = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_X_unit_Vector_x = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_X_unit_Vector_x == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_X_unit_Vector_x's shared memory area.");	 exit(1);}
	M_Grid_X_unit_Vector_x = (double*) MapViewOfFile(hFile_M_Grid_X_unit_Vector_x, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_X_unit_Vector_x == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_X_unit_Vector_y
	sprintf(NameOfMap,"Map_M_Grid_X_unit_Vector_y");
	if(IsToCreate==1)hFile_M_Grid_X_unit_Vector_y = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_X_unit_Vector_y = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_X_unit_Vector_y == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_X_unit_Vector_y's shared memory area.");	 exit(1);}
	M_Grid_X_unit_Vector_y = (double*) MapViewOfFile(hFile_M_Grid_X_unit_Vector_y, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_X_unit_Vector_y == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_X_unit_Vector_z
	sprintf(NameOfMap,"Map_M_Grid_X_unit_Vector_z");
	if(IsToCreate==1)hFile_M_Grid_X_unit_Vector_z = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_X_unit_Vector_z = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_X_unit_Vector_z == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_X_unit_Vector_z's shared memory area.");	 exit(1);}
	M_Grid_X_unit_Vector_z = (double*) MapViewOfFile(hFile_M_Grid_X_unit_Vector_z, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_X_unit_Vector_z == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Y_unit_Vector_x
	sprintf(NameOfMap,"Map_M_Grid_Y_unit_Vector_x");
	if(IsToCreate==1)hFile_M_Grid_Y_unit_Vector_x = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Y_unit_Vector_x = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Y_unit_Vector_x == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Y_unit_Vector_x's shared memory area.");	 exit(1);}
	M_Grid_Y_unit_Vector_x = (double*) MapViewOfFile(hFile_M_Grid_Y_unit_Vector_x, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Y_unit_Vector_x == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Y_unit_Vector_y
	sprintf(NameOfMap,"Map_M_Grid_Y_unit_Vector_y");
	if(IsToCreate==1)hFile_M_Grid_Y_unit_Vector_y = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Y_unit_Vector_y = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Y_unit_Vector_y == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Y_unit_Vector_y's shared memory area.");	 exit(1);}
	M_Grid_Y_unit_Vector_y = (double*) MapViewOfFile(hFile_M_Grid_Y_unit_Vector_y, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Y_unit_Vector_y == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Y_unit_Vector_z
	sprintf(NameOfMap,"Map_M_Grid_Y_unit_Vector_z");
	if(IsToCreate==1)hFile_M_Grid_Y_unit_Vector_z = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Y_unit_Vector_z = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Y_unit_Vector_z == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Y_unit_Vector_z's shared memory area.");	 exit(1);}
	M_Grid_Y_unit_Vector_z = (double*) MapViewOfFile(hFile_M_Grid_Y_unit_Vector_z, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Y_unit_Vector_z == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Z_unit_Vector_x
	sprintf(NameOfMap,"Map_M_Grid_Z_unit_Vector_x");
	if(IsToCreate==1)hFile_M_Grid_Z_unit_Vector_x = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Z_unit_Vector_x = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Z_unit_Vector_x == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Z_unit_Vector_x's shared memory area.");	 exit(1);}
	M_Grid_Z_unit_Vector_x = (double*) MapViewOfFile(hFile_M_Grid_Z_unit_Vector_x, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Z_unit_Vector_x == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Z_unit_Vector_y
	sprintf(NameOfMap,"Map_M_Grid_Z_unit_Vector_y");
	if(IsToCreate==1)hFile_M_Grid_Z_unit_Vector_y = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Z_unit_Vector_y = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Z_unit_Vector_y == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Z_unit_Vector_y's shared memory area.");	 exit(1);}
	M_Grid_Z_unit_Vector_y = (double*) MapViewOfFile(hFile_M_Grid_Z_unit_Vector_y, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Z_unit_Vector_y == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Grid_Z_unit_Vector_z
	sprintf(NameOfMap,"Map_M_Grid_Z_unit_Vector_z");
	if(IsToCreate==1)hFile_M_Grid_Z_unit_Vector_z = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Grid_Z_unit_Vector_z = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Grid_Z_unit_Vector_z == NULL){AfxMessageBox("Unable to create/open Map_M_Grid_Z_unit_Vector_z's shared memory area.");	 exit(1);}
	M_Grid_Z_unit_Vector_z = (double*) MapViewOfFile(hFile_M_Grid_Z_unit_Vector_z, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Grid_Z_unit_Vector_z == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_GridCenter_X
	sprintf(NameOfMap,"Map_M_GridCenter_X");
	if(IsToCreate==1)hFile_M_GridCenter_X = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_GridCenter_X = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_GridCenter_X == NULL){AfxMessageBox("Unable to create/open Map_M_GridCenter_X's shared memory area.");	 exit(1);}
	M_GridCenter_X = (double*) MapViewOfFile(hFile_M_GridCenter_X, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_GridCenter_X == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_GridCenter_Y
	sprintf(NameOfMap,"Map_M_GridCenter_Y");
	if(IsToCreate==1)hFile_M_GridCenter_Y = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_GridCenter_Y = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_GridCenter_Y == NULL){AfxMessageBox("Unable to create/open Map_M_GridCenter_Y's shared memory area.");	 exit(1);}
	M_GridCenter_Y = (double*) MapViewOfFile(hFile_M_GridCenter_Y, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_GridCenter_Y == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_GridCenter_Z
	sprintf(NameOfMap,"Map_M_GridCenter_Z");
	if(IsToCreate==1)hFile_M_GridCenter_Z = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_GridCenter_Z = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_GridCenter_Z == NULL){AfxMessageBox("Unable to create/open Map_M_GridCenter_Z's shared memory area.");	 exit(1);}
	M_GridCenter_Z = (double*) MapViewOfFile(hFile_M_GridCenter_Z, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_GridCenter_Z == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	
	//M_InitElecBtmX
	sprintf(NameOfMap,"Map_M_InitElecBtmX");
	if(IsToCreate==1)hFile_M_InitElecBtmX = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_InitElecBtmX = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_InitElecBtmX == NULL){AfxMessageBox("Unable to create/open Map_M_InitElecBtmX's shared memory area.");	 exit(1);}
	M_InitElecBtmX = (double*) MapViewOfFile(hFile_M_InitElecBtmX, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_InitElecBtmX == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_InitElecBtmY
	sprintf(NameOfMap,"Map_M_InitElecBtmY");
	if(IsToCreate==1)hFile_M_InitElecBtmY = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_InitElecBtmY = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_InitElecBtmY == NULL){AfxMessageBox("Unable to create/open Map_M_InitElecBtmY's shared memory area.");	 exit(1);}
	M_InitElecBtmY = (double*) MapViewOfFile(hFile_M_InitElecBtmY, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_InitElecBtmY == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_InitElecBtmZ
	sprintf(NameOfMap,"Map_M_InitElecBtmZ");
	if(IsToCreate==1)hFile_M_InitElecBtmZ = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_InitElecBtmZ = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_InitElecBtmZ == NULL){AfxMessageBox("Unable to create/open Map_M_InitElecBtmZ's shared memory area.");	 exit(1);}
	M_InitElecBtmZ = (double*) MapViewOfFile(hFile_M_InitElecBtmZ, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_InitElecBtmZ == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_Z_bias_for_Home
	sprintf(NameOfMap,"Map_M_Z_bias_for_Home");
	if(IsToCreate==1)hFile_M_Z_bias_for_Home = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(double), NameOfMap);
	else			 hFile_M_Z_bias_for_Home = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_Z_bias_for_Home == NULL){AfxMessageBox("Unable to create/open Map_M_Z_bias_for_Home's shared memory area.");	 exit(1);}
	M_Z_bias_for_Home = (double*) MapViewOfFile(hFile_M_Z_bias_for_Home, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_Z_bias_for_Home == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_MarkingCORONAL
	sprintf(NameOfMap,"Map_M_MarkingCORONAL");
	if(IsToCreate==1)hFile_M_MarkingCORONAL = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_MARKINGS * sizeof(Marking), NameOfMap);
	else			 hFile_M_MarkingCORONAL = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_MarkingCORONAL == NULL){AfxMessageBox("Unable to create/open Map_M_MarkingCORONAL's shared memory area.");	 exit(1);}
	M_MarkingCORONAL = (Marking*) MapViewOfFile(hFile_M_MarkingCORONAL, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MarkingCORONAL == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NumberOfMarkingsCORONAL
	sprintf(NameOfMap,"Map_M_NumberOfMarkingsCORONAL");
	if(IsToCreate==1)hFile_M_NumberOfMarkingsCORONAL = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumberOfMarkingsCORONAL = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumberOfMarkingsCORONAL == NULL){AfxMessageBox("Unable to create/open Map_M_NumberOfMarkingsCORONAL's shared memory area.");	 exit(1);}
	M_NumberOfMarkingsCORONAL = (int*) MapViewOfFile(hFile_M_NumberOfMarkingsCORONAL, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumberOfMarkingsCORONAL == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_NumberOfMarkingsCORONAL = 0;
	//M_MarkingSAGITTAL
	sprintf(NameOfMap,"Map_M_MarkingSAGITTAL");
	if(IsToCreate==1)hFile_M_MarkingSAGITTAL = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAX_MARKINGS * sizeof(Marking), NameOfMap);
	else			 hFile_M_MarkingSAGITTAL = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_MarkingSAGITTAL == NULL){AfxMessageBox("Unable to create/open Map_M_MarkingSAGITTAL's shared memory area.");	 exit(1);}
	M_MarkingSAGITTAL = (Marking*) MapViewOfFile(hFile_M_MarkingSAGITTAL, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_MarkingSAGITTAL == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_NumberOfMarkingsSAGITTAL
	sprintf(NameOfMap,"Map_M_NumberOfMarkingsSAGITTAL");
	if(IsToCreate==1)hFile_M_NumberOfMarkingsSAGITTAL = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_NumberOfMarkingsSAGITTAL = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_NumberOfMarkingsSAGITTAL == NULL){AfxMessageBox("Unable to create/open Map_M_NumberOfMarkingsSAGITTAL's shared memory area.");	 exit(1);}
	M_NumberOfMarkingsSAGITTAL = (int*) MapViewOfFile(hFile_M_NumberOfMarkingsSAGITTAL, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_NumberOfMarkingsSAGITTAL == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_NumberOfMarkingsSAGITTAL = 0;

	//M_KeyDownEvents
	sprintf(NameOfMap,"Map_M_KeyDownEvents");
	if(IsToCreate==1)hFile_M_KeyDownEvents = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, MAXIMUM_WAIT_KeyOBJECTS * sizeof(HANDLE), NameOfMap);
	else			 hFile_M_KeyDownEvents = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_KeyDownEvents == NULL){AfxMessageBox("Unable to create/open Map_M_KeyDownEvents's shared memory area.");	 exit(1);}
	M_KeyDownEvents = (HANDLE*) MapViewOfFile(hFile_M_KeyDownEvents, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_KeyDownEvents == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	//M_isKeyDownEventsCreated
	sprintf(NameOfMap,"Map_M_isKeyDownEventsCreated");
	if(IsToCreate==1)hFile_M_isKeyDownEventsCreated = CreateFileMapping(INVALID_HANDLE_VALUE, 	NULL,PAGE_READWRITE, 0, 1 * sizeof(int), NameOfMap);
	else			 hFile_M_isKeyDownEventsCreated = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NameOfMap); 
	if(hFile_M_isKeyDownEventsCreated == NULL){AfxMessageBox("Unable to create/open Map_M_isKeyDownEventsCreated's shared memory area.");	 exit(1);}
	M_isKeyDownEventsCreated = (int*) MapViewOfFile(hFile_M_isKeyDownEventsCreated, FILE_MAP_ALL_ACCESS,  	0,	0,	0);
	if(M_isKeyDownEventsCreated == NULL){	AfxMessageBox("Unable to create/open a VIEW."); exit(1);}
	if(IsToCreate==1)*M_isKeyDownEventsCreated = 0;


