	//########################  Memory Map ##########################
	//Parameters and variables	
	HANDLE	hFile_M_is3D_Running;				int *M_is3D_Running;
	HANDLE	hFile_M_isCoronal_Running;				int *M_isCoronal_Running;
	HANDLE	hFile_M_isSagittal_Running;				int *M_isSagittal_Running;
	HANDLE	hFile_M_HeyCoronalFigure_3DRequest_toRedrawElectrode;int *M_HeyCoronalFigure_RedrawElectrode;
	HANDLE	hFile_M_HeySagittalFigure_3DRequest_toRedrawElectrode;int *M_HeySagittalFigure_RedrawElectrode;
	struct FileInfo{
		char m_SliceFileName[1028];
		double m_Depth;
	};
	HANDLE	hFile_M_DrawingFileInfo;				FileInfo *M_DrawingFileInfo;
	HANDLE	hFile_M_DataDir;							char *M_DataDir;
	HANDLE	hFile_M_MsgToNewWindow;						char *M_MsgToNewWindow;
	HANDLE	hFile_M_ElectrodeTipX_3DAtlas;				double *M_ElectrodeTipX_3DAtlas;
	HANDLE	hFile_M_ElectrodeTipY_3DAtlas;				double *M_ElectrodeTipY_3DAtlas;
	HANDLE	hFile_M_ElectrodeTipZ_3DAtlas;				double *M_ElectrodeTipZ_3DAtlas;
	HANDLE	hFile_M_ElectrodeTopX_3DAtlas;				double *M_ElectrodeTopX_3DAtlas;
	HANDLE	hFile_M_ElectrodeTopY_3DAtlas;				double *M_ElectrodeTopY_3DAtlas;
	HANDLE	hFile_M_ElectrodeTopZ_3DAtlas;				double *M_ElectrodeTopZ_3DAtlas;
	HANDLE	hFile_M_Grid_Pitch;							double *M_Grid_Pitch;
	HANDLE	hFile_M_Grid_Roll;							double *M_Grid_Roll;
	HANDLE	hFile_M_Grid_Yaw;							double *M_Grid_Yaw;

	HANDLE	hFile_M_Grid_X_unit_Vector_x;				double *M_Grid_X_unit_Vector_x;	
	HANDLE	hFile_M_Grid_X_unit_Vector_y;				double *M_Grid_X_unit_Vector_y;
	HANDLE	hFile_M_Grid_X_unit_Vector_z;				double *M_Grid_X_unit_Vector_z;
	HANDLE	hFile_M_Grid_Y_unit_Vector_x;				double *M_Grid_Y_unit_Vector_x;
	HANDLE	hFile_M_Grid_Y_unit_Vector_y;				double *M_Grid_Y_unit_Vector_y;
	HANDLE	hFile_M_Grid_Y_unit_Vector_z;				double *M_Grid_Y_unit_Vector_z;
	HANDLE	hFile_M_Grid_Z_unit_Vector_x;				double *M_Grid_Z_unit_Vector_x;
	HANDLE	hFile_M_Grid_Z_unit_Vector_y;				double *M_Grid_Z_unit_Vector_y;
	HANDLE	hFile_M_Grid_Z_unit_Vector_z;				double *M_Grid_Z_unit_Vector_z;
	HANDLE	hFile_M_GridCenter_X;						double *M_GridCenter_X;
	HANDLE	hFile_M_GridCenter_Y;						double *M_GridCenter_Y;
	HANDLE	hFile_M_GridCenter_Z;						double *M_GridCenter_Z;

	HANDLE	hFile_M_InitElecBtmX;						double *M_InitElecBtmX;
	HANDLE	hFile_M_InitElecBtmY;						double *M_InitElecBtmY;
	HANDLE	hFile_M_InitElecBtmZ;						double *M_InitElecBtmZ;
	HANDLE	hFile_M_Z_bias_for_Home;					double *M_Z_bias_for_Home;
#define MAX_MARKINGS 1000
	struct Marking{
		double m_x, m_y, m_z;
		int m_MarkingType;
		int m_rgbID;
		double r,g,b;
	};
	HANDLE	hFile_M_MarkingCORONAL;						Marking *M_MarkingCORONAL;
	HANDLE	hFile_M_NumberOfMarkingsCORONAL;					int		*M_NumberOfMarkingsCORONAL;
	HANDLE	hFile_M_MarkingSAGITTAL;					Marking *M_MarkingSAGITTAL;
	HANDLE	hFile_M_NumberOfMarkingsSAGITTAL;					int		*M_NumberOfMarkingsSAGITTAL;

	//EVENT
	#define MAXIMUM_WAIT_KeyOBJECTS 7
	HANDLE	hFile_M_KeyDownEvents; HANDLE  *M_KeyDownEvents;
	HANDLE	hFile_M_isKeyDownEventsCreated;				int  *M_isKeyDownEventsCreated;





