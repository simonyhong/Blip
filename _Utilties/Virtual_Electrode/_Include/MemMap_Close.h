	int i;
	if(!UnmapViewOfFile(M_is3D_Running)){ return;/*In case it has already been closed*/} 
	CloseHandle(hFile_M_is3D_Running); 

	//M_isCoronal_Running
	if(!UnmapViewOfFile(M_isCoronal_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isCoronal_Running); 
	//M_isSagittal_Running
	if(!UnmapViewOfFile(M_isSagittal_Running)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isSagittal_Running); 
	//M_HeyCoronalFigure_RedrawElectrode
	if(!UnmapViewOfFile(M_HeyCoronalFigure_RedrawElectrode)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeyCoronalFigure_3DRequest_toRedrawElectrode); 
	//M_HeySagittalFigure_RedrawElectrode
	if(!UnmapViewOfFile(M_HeySagittalFigure_RedrawElectrode)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_HeySagittalFigure_3DRequest_toRedrawElectrode); 
	//M_DrawingFileInfo
	if(!UnmapViewOfFile(M_DrawingFileInfo)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DrawingFileInfo); 
	//M_DataDir
	if(!UnmapViewOfFile(M_DataDir)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_DataDir); 
	//M_MsgToNewWindow
	if(!UnmapViewOfFile(M_MsgToNewWindow)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MsgToNewWindow); 
	//M_ElectrodeTipX_3DAtlas
	if(!UnmapViewOfFile(M_ElectrodeTipX_3DAtlas)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ElectrodeTipX_3DAtlas); 
	//M_ElectrodeTipY_3DAtlas
	if(!UnmapViewOfFile(M_ElectrodeTipY_3DAtlas)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ElectrodeTipY_3DAtlas); 
	//M_ElectrodeTipZ_3DAtlas
	if(!UnmapViewOfFile(M_ElectrodeTipZ_3DAtlas)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ElectrodeTipZ_3DAtlas); 
	//M_ElectrodeTopX_3DAtlas
	if(!UnmapViewOfFile(M_ElectrodeTopX_3DAtlas)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ElectrodeTopX_3DAtlas); 
	//M_ElectrodeTopY_3DAtlas
	if(!UnmapViewOfFile(M_ElectrodeTopY_3DAtlas)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ElectrodeTopY_3DAtlas); 
	//M_ElectrodeTopZ_3DAtlas
	if(!UnmapViewOfFile(M_ElectrodeTopZ_3DAtlas)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_ElectrodeTopZ_3DAtlas); 
	//M_Grid_Pitch
	if(!UnmapViewOfFile(M_Grid_Pitch)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Pitch); 
	//M_Grid_Roll
	if(!UnmapViewOfFile(M_Grid_Roll)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Roll); 
	//M_Grid_Yaw
	if(!UnmapViewOfFile(M_Grid_Yaw)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Yaw); 

	//M_Grid_X_unit_Vector_x
	if(!UnmapViewOfFile(M_Grid_X_unit_Vector_x)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_X_unit_Vector_x); 
	//M_Grid_X_unit_Vector_y
	if(!UnmapViewOfFile(M_Grid_X_unit_Vector_y)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_X_unit_Vector_y); 
	//M_Grid_X_unit_Vector_z
	if(!UnmapViewOfFile(M_Grid_X_unit_Vector_z)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_X_unit_Vector_z); 
	//M_Grid_Y_unit_Vector_x
	if(!UnmapViewOfFile(M_Grid_Y_unit_Vector_x)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Y_unit_Vector_x); 
	//M_Grid_Y_unit_Vector_y
	if(!UnmapViewOfFile(M_Grid_Y_unit_Vector_y)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Y_unit_Vector_y); 
	//M_Grid_Y_unit_Vector_z
	if(!UnmapViewOfFile(M_Grid_Y_unit_Vector_z)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Y_unit_Vector_z); 
	//M_Grid_Z_unit_Vector_x
	if(!UnmapViewOfFile(M_Grid_Z_unit_Vector_x)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Z_unit_Vector_x); 
	//M_Grid_Z_unit_Vector_y
	if(!UnmapViewOfFile(M_Grid_Z_unit_Vector_y)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Z_unit_Vector_y); 
	//M_Grid_Z_unit_Vector_z
	if(!UnmapViewOfFile(M_Grid_Z_unit_Vector_z)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Grid_Z_unit_Vector_z); 
	//M_GridCenter_X
	if(!UnmapViewOfFile(M_GridCenter_X)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_GridCenter_X); 
	//M_GridCenter_Y
	if(!UnmapViewOfFile(M_GridCenter_Y)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_GridCenter_Y); 
	//M_GridCenter_Z
	if(!UnmapViewOfFile(M_GridCenter_Z)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_GridCenter_Z); 

	//M_InitElecBtmX
	if(!UnmapViewOfFile(M_InitElecBtmX)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_InitElecBtmX); 
	//M_InitElecBtmY
	if(!UnmapViewOfFile(M_InitElecBtmY)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_InitElecBtmY); 
	//M_InitElecBtmZ
	if(!UnmapViewOfFile(M_InitElecBtmZ)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_InitElecBtmZ); 
	//M_Z_bias_for_Home
	if(!UnmapViewOfFile(M_Z_bias_for_Home)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_Z_bias_for_Home); 
	//M_MarkingCORONAL
	if(!UnmapViewOfFile(M_MarkingCORONAL)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MarkingCORONAL); 
	//M_NumberOfMarkingsCORONAL
	if(!UnmapViewOfFile(M_NumberOfMarkingsCORONAL)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumberOfMarkingsCORONAL); 
	//M_MarkingSAGITTAL
	if(!UnmapViewOfFile(M_MarkingSAGITTAL)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_MarkingSAGITTAL); 
	//M_NumberOfMarkingsSAGITTAL
	if(!UnmapViewOfFile(M_NumberOfMarkingsSAGITTAL)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_NumberOfMarkingsSAGITTAL); 

	//M_KeyDownEvents
	if(!UnmapViewOfFile(M_KeyDownEvents)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_KeyDownEvents); 
	//M_isKeyDownEventsCreated
	if(!UnmapViewOfFile(M_isKeyDownEventsCreated)){ AfxMessageBox("Could not unmap view of file.");} 
	CloseHandle(hFile_M_isKeyDownEventsCreated); 



