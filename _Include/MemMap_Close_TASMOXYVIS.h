	//M_StringMsg_fromTAS_toVIS
	if(!UnmapViewOfFile(M_StringMsg_fromTAS_toVIS)){ return; /*AfxMessageBox("Could not unmap view of file.");*/} 
	CloseHandle(hFile_M_StringMsg_fromTAS_toVIS); 
	//M_StringMsg_fromVIS_toTAS
	if(!UnmapViewOfFile(M_StringMsg_fromVIS_toTAS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StringMsg_fromVIS_toTAS); 
	//M_StringMsg_fromTAS_toMOXY
	if(!UnmapViewOfFile(M_StringMsg_fromTAS_toMOXY)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StringMsg_fromTAS_toMOXY); 
	//M_StringMsg_fromMOXY_toTAS
	if(!UnmapViewOfFile(M_StringMsg_fromMOXY_toTAS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_StringMsg_fromMOXY_toTAS); 
	//M_MsgID_fromVIS_toTAS
	if(!UnmapViewOfFile(M_MsgID_fromVIS_toTAS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MsgID_fromVIS_toTAS); 
	//M_MsgID_fromTAS_toVIS
	if(!UnmapViewOfFile(M_MsgID_fromTAS_toVIS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MsgID_fromTAS_toVIS); 
	//M_MsgID_fromMOXY_toTAS
	if(!UnmapViewOfFile(M_MsgID_fromMOXY_toTAS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MsgID_fromMOXY_toTAS); 
	//M_Msg_from_MOXY_to_VIS
	if(!UnmapViewOfFile(M_Msg_from_MOXY_to_VIS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Msg_from_MOXY_to_VIS); 
	//M_MsgID_fromTAS_toMOXY
	if(!UnmapViewOfFile(M_MsgID_fromTAS_toMOXY)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MsgID_fromTAS_toMOXY); 
	//M_IsVisibleObj
	if(!UnmapViewOfFile(M_IsVisibleObj)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_IsVisibleObj); 

	//########  MOXY to TAS  ##########################################
	//M_MoxyUserDataSetID
	if(!UnmapViewOfFile(M_MoxyUserDataSetID)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MoxyUserDataSetID); 
	//M_MoxyUserData
	if(!UnmapViewOfFile(M_MoxyUserData)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MoxyUserData); 

	//M_Dur_BigRWD
	if(!UnmapViewOfFile(M_Dur_BigRWD)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Dur_BigRWD); 

	//M_Is_VIS_Running
	if(!UnmapViewOfFile(M_Is_VIS_Running)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Is_VIS_Running); 
	//M_Is_TAS_Running_4MOXYVIS
	if(!UnmapViewOfFile(M_Is_TAS_Running_4MOXYVIS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Is_TAS_Running_4MOXYVIS); 
	//M_N
	if(!UnmapViewOfFile(M_N)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_N); 
	//M_VisObjIDs
	if(!UnmapViewOfFile(M_VisObjIDs)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_VisObjIDs); 
	//M_UserClickedVisObjIDs
	if(!UnmapViewOfFile(M_UserClickedVisObjIDs)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_UserClickedVisObjIDs); 
	//M_UserClickedN
	if(!UnmapViewOfFile(M_UserClickedN)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_UserClickedN); 
	//M_CLOSE_VIS
	if(!UnmapViewOfFile(M_CLOSE_VIS)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_CLOSE_VIS); 
	//M_backgroundScreenRGB
	if(!UnmapViewOfFile(M_backgroundScreenRGB)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_backgroundScreenRGB); 
	//M_cntrX
	if(!UnmapViewOfFile(M_cntrX)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_cntrX); 
	//M_cntrY
	if(!UnmapViewOfFile(M_cntrY)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_cntrY); 
	//M_width
	if(!UnmapViewOfFile(M_width)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_width); 
	//M_hight
	if(!UnmapViewOfFile(M_hight)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_hight); 
	//M_angle
	if(!UnmapViewOfFile(M_angle)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_angle); 
	//M_Obj_Shape
	if(!UnmapViewOfFile(M_Obj_Shape)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Obj_Shape); 
	//M_rgb
	if(!UnmapViewOfFile(M_rgb)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_rgb); 
	//M_Ojb_Switch
	if(!UnmapViewOfFile(M_Ojb_Switch)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Ojb_Switch); 
	for(int i=0;i<MAX_STIM_OBJ;i++){
		//M_PicFileName[]
		if(!UnmapViewOfFile(M_PicFileName[i])){AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_PicFileName[i]); 
	}
	//M_PhotoDiodeFlash_PosX
	if(!UnmapViewOfFile(M_PhotoDiodeFlash_PosX)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PhotoDiodeFlash_PosX); 
	//M_PhotoDiodeFlash_PosY
	if(!UnmapViewOfFile(M_PhotoDiodeFlash_PosY)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PhotoDiodeFlash_PosY); 
	//M_PhotoDiodeFlash_PosWidth
	if(!UnmapViewOfFile(M_PhotoDiodeFlash_PosWidth)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PhotoDiodeFlash_PosWidth); 
	//M_PhotoDiodeFlash_PosHeight
	if(!UnmapViewOfFile(M_PhotoDiodeFlash_PosHeight)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PhotoDiodeFlash_PosHeight); 

	//####################   EYE position
	//M_PosFigMinX
	if(!UnmapViewOfFile(M_PosFigMinX)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PosFigMinX); 
	//M_PosFigMaxX
	if(!UnmapViewOfFile(M_PosFigMaxX)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PosFigMaxX); 
	//M_PosFigMinY
	if(!UnmapViewOfFile(M_PosFigMinY)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PosFigMinY); 
	//M_PosFigMaxY
	if(!UnmapViewOfFile(M_PosFigMaxY)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PosFigMaxY); 
	//M_EyeX_rawData
	if(!UnmapViewOfFile(M_EyeX_rawData)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeX_rawData); 
	//M_EyeY_rawData
	if(!UnmapViewOfFile(M_EyeY_rawData)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeY_rawData); 
	//M_EyeX_Bias
	if(!UnmapViewOfFile(M_EyeX_Bias)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeX_Bias); 
	//M_EyeY_Bias
	if(!UnmapViewOfFile(M_EyeY_Bias)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeY_Bias); 
	//M_EyeX_Gain
	if(!UnmapViewOfFile(M_EyeX_Gain)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeX_Gain); 
	//M_EyeY_Gain
	if(!UnmapViewOfFile(M_EyeY_Gain)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeY_Gain); 
	//M_EyePosX
	if(!UnmapViewOfFile(M_EyePosX)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyePosX); 
	//M_EyePosY
	if(!UnmapViewOfFile(M_EyePosY)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyePosY); 
	//M_EyeXsimulalted
	if(!UnmapViewOfFile(M_EyeXsimulalted)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeXsimulalted); 
	//M_EyeYsimulalted
	if(!UnmapViewOfFile(M_EyeYsimulalted)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EyeYsimulalted); 
	//M_JustClicked
	if(!UnmapViewOfFile(M_JustClicked)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_JustClicked); 
	//M_PositionWinds
	if(!UnmapViewOfFile(M_PositionWinds)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PositionWinds); 
	//M_WindToDisplayOrNot
	if(!UnmapViewOfFile(M_WindToDisplayOrNot)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_WindToDisplayOrNot); 
	//M_BehavioralObjectForXYmonitor
	if(!UnmapViewOfFile(M_BehavioralObjectForXYmonitor)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_BehavioralObjectForXYmonitor); 
	//M_DIO_State
	if(!UnmapViewOfFile(M_DIO_State)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DIO_State); 

	//####################   Ecode & alias
	//M_EcodeTable
	if(!UnmapViewOfFile(M_EcodeTable)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_EcodeTable); 
	//M_NumEcodesInTable
	if(!UnmapViewOfFile(M_NumEcodesInTable)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumEcodesInTable); 
	//M_IsTriggeredMode
	if(!UnmapViewOfFile(M_IsTriggeredMode)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_IsTriggeredMode); 
	//M_nTriggerECode
	if(!UnmapViewOfFile(M_nTriggerECode)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_nTriggerECode); 
	//M_hasTriggered
	if(!UnmapViewOfFile(M_hasTriggered)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_hasTriggered); 
	//M_TriggeredTime_ms
	if(!UnmapViewOfFile(M_TriggeredTime_ms)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_TriggeredTime_ms); 
	//####################  File Names  ##########################################
	//M_PreFix
	if(!UnmapViewOfFile(M_PreFix)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_PreFix); 
	//M_SuFFix
	if(!UnmapViewOfFile(M_SuFFix)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_SuFFix); 
	//M_BaseOfFileName
	if(!UnmapViewOfFile(M_BaseOfFileName)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_BaseOfFileName); 
	//M_JustFileName
	if(!UnmapViewOfFile(M_JustFileName)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_JustFileName); 
	//M_isSaving
	if(!UnmapViewOfFile(M_isSaving)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isSaving); 
	//M_isRenamed
	if(!UnmapViewOfFile(M_isRenamed)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isRenamed); 
	//####################  User Picked Variables  ##########################################
	//M_VarToDisp
	if(!UnmapViewOfFile(M_VarToDisp)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_VarToDisp); 
	//M_NumUserPickedVariables
	if(!UnmapViewOfFile(M_NumUserPickedVariables)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumUserPickedVariables); 
	//M_VarToDispLineEnd
	if(!UnmapViewOfFile(M_VarToDispLineEnd)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_VarToDispLineEnd); 
	//M_VarRECT
	if(!UnmapViewOfFile(M_VarRECT)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_VarRECT); 
	//####################   Message queue from TAS to MOXY
	//M_Msg_Q_Idx
	if(!UnmapViewOfFile(M_Msg_Q_Idx)){AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Msg_Q_Idx); 
	for(int i=0;i<SIZE_OF_MSG_Q;i++){
		//M_StrQ_fromTAS_toMOXY[]
		if(!UnmapViewOfFile(M_StrQ_fromTAS_toMOXY[i])){AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_StrQ_fromTAS_toMOXY[i]); 
		//M_VisObjID_Q_fromTAS_toMOXY[]
		if(!UnmapViewOfFile(M_VisObjID_Q_fromTAS_toMOXY[i])){AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_VisObjID_Q_fromTAS_toMOXY[i]); 
		//M_N_Q_fromTAS_toMOXY[]
		if(!UnmapViewOfFile(M_N_Q_fromTAS_toMOXY[i])){AfxMessageBox("Could not unmap view of file.");} 
		CloseHandle(hFile_M_N_Q_fromTAS_toMOXY[i]); 
	}