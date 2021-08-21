// AnalysisView.h : interface of the CAnalysisView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//###############################################################################################################################
//-----------------------------  Constants --------------------------------------
#define WHOLE_NET     0
#define NO_CONNECTION 1
//#define GRAPH_3D_STOP	0
//#define SIMULATION	1
//#define GRAPH_2D		2
//#define GRAPH_3D		3
//----------------------------------------   My_Math.h --------------------------------------------------
#define RAND_RATE       ( (double)rand()/(double)RAND_MAX )
#define PROBABILITY(P)  (RAND_RATE<(double)P)
#define Random(N)       (rand()%(N))  // N should be an integer.

#define Sq2D(x,y)   ((x)*(x)+(y)*(y))        //Distance from the original to the given vector A
#define Sq3D(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))//Distance from the original to the given vector A
#define M3D(x,y,z)  sqrt((x)*(x)+(y)*(y)+(z)*(z))
#define M2D(x,y)    sqrt(((x)*(x)+(y)*(y)))
#define SQ_DISTANCE_AB_1D(x1,  x2)          ( ((x1)-(x2))*((x1)-(x2))  )
#define DISTANCE_AB_1D(x1,  x2)          sqrt( SQ_DISTANCE_AB_1D(x1,  x2) )
#define SQ_DISTANCE_AB(x1,y1,  x2,y2)          ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) )
#define DISTANCE_AB(x1,y1,  x2,y2) ( sqrt(           SQ_DISTANCE_AB(x1,y1,  x2,y2)                )  )
#define DISTANCE_OA(x1,y1) ( sqrt(   Sq2D(x1,y1)    )  )
#define	SQ_DISTANCE_AB_2D(x1,y1,  x2,y2)          ( ((x1)-(x2))*((x1)-(x2)) + ((y1)-(y2))*((y1)-(y2)) )
inline double CrossProduct(int a1, int a2, int b1, int b2 ){return a1*b2 - b1*a2; }
inline double CrossProduct(float a1, float a2, int b1, int b2 ){return a1*b2 - b1*a2; }
inline double CrossProduct(double a1, double a2, double b1, double b2 ){return a1*b2 - b1*a2; }
inline double DotProduct( int a1, int a2, int b1, int b2 ){return a1*b1 + a2*b2; }
inline double DotProduct( double a1, double a2, double b1, double b2 ){return a1*b1 + a2*b2; }
inline double DotProduct( float a1, float a2, float b1, float b2 ){return a1*b1 + a2*b2; }
inline double DotProduct3D( double a1, double a2, double a3, double b1, double b2, double b3 ){return a1*b1 + a2*b2+ a3*b3; }
inline double MAXab(double a, double b ){if(a>b)return a; else return b; }
inline double MINab(double a, double b ){if(a<b)return a; else return b; }

#define WM_MY_MESSAGE (WM_USER + 1)
#define SimDurationx10    1
#define MsgPosX(pB, dataX) (pB->m_dMinX+(dataX)*(pB->m_dMaxX-pB->m_dMinX))
#define MsgPosY(pB, dataY) (pB->m_dMinY+(dataY)*(pB->m_dMaxY-pB->m_dMinY))

#define RemoveForwardSlashAtEnd(Dir) if(Dir[strlen(Dir) - 1] == '\\') Dir[strlen(Dir) - 1] = '\0'; //Remove the forward slash.
#define AddFrwdSlashAtEndIfNotHave(Dir)    if(Dir[strlen(Dir) - 1] != '\\') strcat(Dir,"\\"); //Add the forward slash.

#define	MAX_DAYS 100
#define	MAX_SESSIONS 50
#define MAX_TRIALS 10000
#define MAX_TRIALS_OF_LEVEL (MAX_TRIALS/10)
#define MAX_INBLOCK_TRLS 20


#define CLICK_PRECISION_ON_SCREEN (4)
#define HEAD (-1)
#define BODY (0)
#define TAIL (1)

class CVertex{
public:
	double m_x, m_y, m_z;
	int m_type, m_LineHeadOrTail;
	int m_rgbID;
	double r,g,b;
	CVertex(){ m_LineHeadOrTail=0; };
	CVertex(double x, double y, double z, int Type, int rgbID){
		m_x=x; m_y=y; m_z=z; m_type=Type; m_rgbID=rgbID; m_LineHeadOrTail=0; 
	};
	virtual ~CVertex(){};
};


class CLinedObj{
public:
	double m_x, m_y, m_z;
	int m_rgbID;
	double r,g,b;
	double m_Left, m_Right, m_Top, m_Bottom;
	int m_type;
	static int *m_pNeedsToSave;
	CArray<CVertex, CVertex& >m_Vtx; int m_NumVertices;

	void	InitObj();
	void	Draw_Obj(CFigure *pFig, CDC *pDC,  CDC *ObjDC, CPen *pPen, int isToEditPoints, int isCoronal_Or_Sagittal);
	void	Draw_Obj_RelativeToXY_CORONAL(double X, double Y, CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen);
	void	Draw_Obj_RelativeToXY_SAGITTAL(double X, double Y, CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen);
	void	Highlight_Obj(CFigure *pFig, CDC *pDC,  CDC *ObjDC, CPen *pPen, int isCoronal_Or_Sagittal);
	void	Draw_lastLineSegment(CFigure *pFig, CDC *pDC, CDC *pObjDC, CPen *pPen, int isCoronal_Or_Sagittal);
	void	Add_VertexIn3D(double x, double y, double z, int RGBID, int isCoronal_Or_Sagittal);
	void	Insert_Vertex_CORONAL(int hitSegmentIdx, double x, double y, double z, int RGBID);
	void	Insert_Vertex_SAGITTAL(int hitSegmentIdx, double x, double y, double z, int RGBID);
	int		LineHitTest(double clickX, double clickY, CFigure *pFig, int isCoronal_Or_Sagittal);
	int		LineHitTest_AND_Insert_Vertex(double x, double y, double z, CFigure *pFig, int isCoronal_Or_Sagittal);
	int		PointHitTest(double Horizontal, double Z, CFigure *pFig, int isCoronal_Or_Sagittal);
	void	Translate_Horizontally_CORONAL(double x);
	void	Translate_Horizontally_SAGITTAL(double y);
	void	TranslateVertically(double y);
	void	Translate_In2D_CORONAL(double horizontal, double vertical);
	void	Translate_In2D_SAGITTAL(double horizontal, double vertical);
	void	UpdateBoundary(int isCoronal_Or_Sagittal);
	void	Change_Color(int RGBID);
	double	Distance_Point_toLineSeg(double x1,double y1,double x2,double y2,double x3,double y3);
	CLinedObj(){
		m_rgbID=MaxRGB60-1; m_Vtx.SetSize(10); 
		m_Left=INT_MAX; m_Right=-INT_MAX; m_Top=-INT_MAX; m_Bottom=INT_MAX;
	};
	virtual ~CLinedObj(){m_NumVertices=0; m_Vtx.RemoveAll(); };
};

#define MAX_CHAR_FOR_NOTE 1024
#define MAX_TEXTURE 100
#define MAX_NN_VARs 10
#define m_AntidromicStimulatedFrom	m_NnVariables[0]
#define m_MarkingLesion	m_NnVariables[1]
class CNeuron{
public:
	double m_x, m_y, m_z;
	CString m_NnName;
	CString m_Note;
	int m_type;
	double m_NnVariables[MAX_NN_VARs];
	CNeuron(){
		m_type=-1; 
		m_x=0; m_y=0; m_z=0;  
		m_NnName="";
		m_Note="";
		for(int i=0; i<MAX_NN_VARs; i++)m_NnVariables[i]=-1;
	};
	virtual ~CNeuron(){};
};

class TextureForCORONAL{
public:
	CArray<float, float& > m_RforI, m_GforI, m_BforI;
	double m_imgWidth, m_imgHeight;
	TextureForCORONAL(){
		m_imgWidth=0; m_imgHeight=0;
	};
	virtual ~TextureForCORONAL(){
		m_RforI.RemoveAll(); 	m_GforI.RemoveAll(); 	m_BforI.RemoveAll(); 
	};
};


class CAnalysisView;
class CMRI{
public:
	//####  Caution: Anything that was loaded before Load_MRIs() needs to be transferred using Rearrage_MRI_Files()
	double m_Depth;
	char m_FigureNameWithPath[1028];
	char m_JustMRI_FileName[1028];
	//####  Caution: Anything that was loaded before Load_MRIs() needs to be transferred using Rearrage_MRI_Files()

	static CAnalysisView *m_pView;
	static double	m_LowerLeftFirstPixel_Abscissa_3D, m_LowerLeftFirstPixel_Ordinate_3D;	
	CArray<float, float& > m_RforI, m_GforI, m_BforI;
	void Draw_MRI(CFigure *pFig, CClientDC *pDC, int toDisplayNow);
	CMRI(){
	};
	virtual ~CMRI(){m_RforI.RemoveAll(); 	m_GforI.RemoveAll(); 	m_BforI.RemoveAll();};
};


class CDrawing{
public:
	//####  Caution: Anything that was loaded before Load_MRIs() needs to be transferred using Rearrage_MRI_Files()
	double m_Depth;
	//####  Caution: Anything that was loaded before Load_MRIs() needs to be transferred using Rearrage_MRI_Files()

	CArray<CLinedObj, CLinedObj& >m_LinedObj; int m_NumLinedObj;
	CArray<CVertex, CVertex& >m_Dot; int m_NumDots;
	CDrawing(){m_NumDots=0; m_NumLinedObj=0; }
	virtual ~CDrawing(){};
};


#define NEURAL_DEPTH_RANGE_IN_MRI (0.5*m_MRI_Pixel_Dimension_In_mm)

//Color Palette
#define BCGYRV_COLORS 12
#define BRIGHTNESS_LEVELS 5

class CFigure;
class CThreeD;
class CAnalysisView : public CView
{
protected: // create from serialization only
	CAnalysisView();
	DECLARE_DYNCREATE(CAnalysisView)
//-------------------------------------------
public:
	//#####################  Application starts from HERE  ##################################################################################
	CArray<CNeuron, CNeuron& >m_Nn; 
	CArray<CMRI, CMRI& >m_MRI; int m_NumMRIs;
	CArray<CDrawing, CDrawing& >m_Drawing; int m_NumDrawings;
	int		m_NnType, m_numTextures, m_HowManyNeurons;
	int		m_Selected_MRI_Idx, m_Selected_Drawing_Idx, m_DrawingLoaded;
	int		m_SortedDrawingIdx[Max_Slices];
	int		m_NeedsToSave, m_toRegisterOrign, m_toRegisterGridOrign;
	unsigned int m_Texture[MAX_TEXTURE];
	void	Load_Neuron_Data_and_Texture(double scaleX, double scaleY, double scaleZ);
	void	Save_Neuron_Data(double scaleX, double scaleY, double scaleZ);
	void	Modify_Neuron_Data(double scaleX, double scaleY, double scaleZ);
	void	Load_All_Texture();
	int		Accept_NeuronInfo(CNeuron *pNn);
	void	DeleteNn(int NnID);
	double	m_MRI_Pixel_Dimension_In_mm;
	void	UnMarkObjects_and_RedrawEveryting();
	void	Calculate_Electrode_and_Grid_Coordinates();
	double	m_OriginOfAbscissa_MRIPixelCoordinate_Coronal_JustForSaveAndRead, m_OriginOfOrdinate_MRIPixelCoordinate_Coronal_JustForSaveAndRead;
	double	m_OriginOfAbscissa_MRIPixelCoordinate_Sagittal_JustForSaveAndRead, m_OriginOfOrdinate_MRIPixelCoordinate_Sagittal_JustForSaveAndRead;
	CDrawing *Select_Current_Drawing(int Selected_Drawing_Idx);
	void	Read_3D_2D_Config();
	void	Save_3D_2D_Config();
	int		Launch_Coronal_Or_Sagittal(char ModeName[]);
	void	Draw_Neural_Info_CORONAL(double MRIdepth);
	void	Draw_Neural_Info_SAGITTAL(double MRIdepth);
	double	m_PrevMag2D, m_Magnification2D, m_MinX_corner, m_MinZ_corner;
	double	MRI_Pixel_to_3D_Coordinate_AbscissaAxis(double X);
	double	MRI_Pixel_to_3D_Coordinate_OrdinateAxis(double Y);
	double	ThD_to_MRI_Pixel_Coordinate_AbscissaAxis(double X);
	double	ThD_to_MRI_Pixel_Coordinate_OrdinateAxis(double Y);
	int		m_isCoronal_Or_Sagittal, m_toShowRuller;
	double	m_DistanceFromElectrode;
	void	Edit_NnInfo_from2D(double x, double y, double z);
	//#####################  Application starts from HERE  ##################################################################################
	CArray<int, int& > m_SelectedMarkings; int m_SizeOfSelectedMarkings, m_NumSelectedMarkings;  //They are operated on the currently displayed m_Drawing[]
	CArray<int, int& > m_SelectedDots; int m_SizeOfSelectedDots, m_NumSelectedDots; //They are operated on the currently displayed m_Drawing[]
	CArray<int, int& > m_SelectedObjs; int m_SizeOfSelectedObjs, m_NumSelectedObjs; //They are operated on the currently displayed m_Drawing[]
	int m_UnderConstructionObjID,m_BiggerVertexIdxOfHitLine, m_isToEditObj;//They are operated on the currently displayed m_Drawing[]
 
	TextureForCORONAL CoronalTexture[MAX_TEXTURE];
	int	m_CoronalTexture_Loaded, m_toShowNeuralInfo, m_toShowDrawing;
	void	Draw_Texture(TextureForCORONAL *pTexture, double X, double Y);
	double m_Znear, m_Zfar;
	double m_OriginOfAbscissa_PixelCoordinate,m_OriginOfOrdinate_PixelCoordinate, m_CalibrationX1,m_CalibrationY1,m_CalibrationX2,m_CalibrationY2;
	double m_PixelScale_in_mm;
	char m_ElecCoordinate_withGridCenterAsOrigin[1024], m_ElecDepth[128]; int m_ElecFromCenterLen;
	void	Copy_ElectrodeCoordinate_toDisplay();
	int		m_savedX, m_savedY, m_DrewTmpLine, m_EditMode;
	int		m_OriginDone, m_isToDispOrigin, m_ShowCrosshairOnce;
	int		m_selPaneID;
	int		m_TYPE;
	int		m_GrabbedObjectIdx, m_GrabbedDotIdx, m_GrabbedMarkingIdx, m_CurrentlyEditedObjIdx, m_GrabedVertex; double m_SelectedAtX, m_SelectedAtVertical;
	int		m_RGBID;
	double	m_CapturRect_Top, m_CapturRect_Bottom,m_CapturRect_Left,m_CapturRect_Right;
	void	Load_DrawingData(int mode);
	void	Update_DrawingData();
	void	Convert_to_HeadShape();
	void	Include_test();
	void	InformOtherProgramsToUpdate();
	int		Find_RGB_IDX(int rgb);
	void	MoveHorizontally(double horizontal);
	void	MoveVertically(double vertical);
	void	Add_VertexIn3D(double x, double y, double z, int RGBID, int type, int LineHeadOrTail, int idx);
	void	Add_MarkingIn3D(double x, double y, double z, int RGBID, int type, int idx);
	void	Redraw_Everything();
	void	Draw_Drawing();
	int		m_imgWidth,m_imgHeight;
	void	Init_Figure_AFTER_MRI_loading();
	void	Delete_MarkedOnes();
	void	Delete_AllDrawingObjects();
	void	ProperlyFinish();
	void	Highlight_MarkedOnes();
	void	Change_Color_of_SelectedOnes(int RGBID);
	void	RegisterMarkingForSelection(int PointIdxToRegister);
	void	RegisterDotForSelection(int PointIdxToRegister);
	void	RegisterObjForSelection(int ObjIdxToRegister);
	void	Highlight_MarkedOnes_withTranslation(double TransX, double TransY);
	int		LineHitTest(double clickX, double clickY, CFigure *pFig);
	int		PointHitTest(double clickX, double clickY, CFigure *pFig);
	int		MarkingHitTest(double clickX, double clickY, CFigure *pFig);
	int		CheckIfThisObjIsSelected(int ObjIdx);
	int		CheckIfThisPointIsSelected(int PointIdx);
	int		CheckIfThisMarkingIsSelected(int PointIdx);
	void	Find_MRI_Closest_To_CoronalSection_ToElectrodeTip();
	void	Find_SliceClosest_SagittalSection_ToElectrodeTip();
	void	Find_a_Matching_Drawing_toMRI();
	void	Move_Retract_Electrode(double Dist_mm);
	void	Move_Elec_along_Grid_axis(int AxisID, double Dist_mm);
	void	Translate_Electrode(int AxisID, double Dist_mm);
	//#####################  Application starts from HERE  ##################################################################################
	int		Find_first_6Digit_unintrupted_number(char *SixDigitString, char *NameString); 
	int		Find_Last_Double_inString(char *GivenString, double &NumToReturn);
	int		Copy_fromNth_HowMany(char *Dest, char *Source, int Nth, int HowMany); 
	int		Convert_6digitDate_ToNumber(char *DateString);
	int		m_FileYearMonthDate, m_NumTrialsInList, m_BinWidth, m_IsTextOnBoard;
	void	Disply_MRI_and_ColorLegend();
	void	Load_MRIs(int OpenOption);

	#define MAX_5MIN 50
	#define UNIT_OF_INCREMENT_OF_RWD 100
	int		m_Pic_Loaded;
	int		Pick_Directory();
	void	SaveFolderPath(char FolderPath[]);
	int		ReadSavedFolderPath(char FolderPath[]);
	//#######################################################################################################################################
    // thread handling
    HANDLE	m_hThread,  m_hEventBasedThread,m_hEvStopMainThread, m_EvDisplayRequest;
    HANDLE	m_hEvKeyPressed, m_hToNextStep;
	HANDLE	m_Events[MAXIMUM_WAIT_OBJECTS]; 
	int		m_RequestIdx;
	int		m_DispEvents[MAXIMUM_WAIT_OBJECTS][4], m_DispEventIdx;
	void	RequestDisplayEvent(int Event);
	void	RequestDisplayEvent(int Event,  int SomeIntValue); 
	void	RequestDisplayEvent(int Event,  int SomeIntValue1,  int SomeIntValue2); 
	void	RequestDisplayEvent(int Event, int NumVariables,  int SomeIntValue1,  int SomeIntValue2); 
	void	DisplayRoutine_for_requested_jobs();
	//#######################################################################################################################################
	void CAnalysisView::DrawArrow(double rx1, double ry1, double rx2, double ry2, BOOL Is_Arrow, CFigure *pBd0, CClientDC *pObjDC, CDC *pDC);
	void	BubbleSort(int x[], int count);
	void	BubbleSort(int x[], int count, int prvOrder[]);
	void	Sort_inAscendingOrder(int x[], int count, int OrderX[]);
	void	Sort_inAscendingOrder(double x[], int count, int OrderX[]);
	double	Kernel_2D(double KerSum, double Sigma, double RangeX);
	double	Kernel_1D(double KerSum, double Sigma, double RangeX);
	int	MakeGssKernel_1D(float KrVal[], int sizeOfKer, int KerFieldX[], double KerSum, double Sigma, double RangeX);
	//#######################################################################################################################################
	//#####################  MOUSE  #########################################################################################
	int	m_SelectedItemID;
	int	m_CallingFrom, m_Saved_Left_MousePointFor3DX, m_Saved_Left_MousePointFor3DY, m_Saved_Right_MousePointFor3DX, m_Saved_Right_MousePointFor3DY;
	CFigure	*m_pSpotlightFig;
	#define NO_3DMOVE INT_MAX
	int m_CTRL_Pressed;
	//#######################################################################################################################################
	//#####################  File  #########################################################################################
	int FromA_findB_addC_giveD(char FromHere[], char *FindThis, char AddThis[], char toMakeThis[]);
	int	GetGrandParentFolderName(char* FolderPath, char *GrandParentFolderName);
	CString GetExecutableName();
	char	m_FileName[500],m_FileNameWithoutEXT[500],m_FolderPath[500],m_JustFileName[100],m_JustFolderName[100],m_GrandParentFolderName[500]; 
	int	GetFile_WholePath_Name_CurPath(BOOL IsToREAD);
	int GetFile_WholePath_Name_CurPath(BOOL IsToREAD, char *initialFileName);
	int	GetFolderName(char* FolderPath, char *FolderName);
	int Get_JustFileName(char* FolderPath, char *JustFileName);
	int		wildcmp(char *wild, char *string);
	int		IsThereFileWithThisName(char* searchThisDir, bool isToSearchSubDirs, char* FindJustFileName);	
	int		Find_MRI_Files(char* searchThisDir, bool isToSearchSubDirs, int CallOrigin);
	int		Find_Drawing_Files(char* searchThisDir, bool isToSearchSubDirs, int CallOrigin);
	int		Find_Last_Double_inString_and_give_strings_before_and_after_it(char *GivenString, char* BeforeNum, double &NumToReturn, char *AfterNum);
	int LoadTextureBMP( const char * filename, int wrap );
	void Load_All_Texture_forCoronalSections();
	void	Sort_Drawing_Using_Depth();
	int		Find_Drawing_Previous_Depth();
	int		Find_Drawing_Next_Depth();
	void	Rearrage_MRI_Files();
	void	RemoveEmptyDrawings();
	void	Rename_the_number_part(char *Dir, char* JustFileName);
	void	FindAllFiles(char* searchThisDir, bool isToSearchSubDirs);
	int		Find_Last_Double_inString_and_find_strings_before_and_after_it(char *GivenString, char* BeforeNum, double &NumToReturn, char *AfterNum);
	int		m_Coronal_1_Sagittal_2, m_isFirstTimeCreatingForder;
	double	m_Multiplication, m_Addition;
	char	m_DataFolderPath[1024], m_DataJustFolderName[1024], RenameFiles_Message[1028];
	//#######################################################################################################################################
	//#####################  Memory Map  #########################################################################################
	#include "MemMap_Decl.h"
	void CreateSharedMemoryArea(void);
	void CloseSharedMemoryArea(void);
	//#######################################################################################################################################
	//#####################  Statistics  #########################################################################################
	int		m_FirstTime0ElementErr;
	double	Mean(double *Array, int N);
	double	Mean(float *Array, int N);
	double	Mean(int *Array, int N);
	double	SD_GivenM(double *Array, int N,double mean);
	double	SD_GivenM(float *Array, int N,double mean);
	double	SD_GivenM(int *Array, int N,double mean);
	double	Mean_OnlyPositive(double *Array, int N);
	double	SD_GivenM_OnlyPositive(double *Array, int N,double mean);
	void	Gaussian_WhiteNoise_0to1(double noise[], int numSamples, double Mean, double Sig);
	double	GenHist_fromMinimum(double Data[],int dataSize, int Hist[], int binNum, double binWidth);
	double	box_muller(double m, double s);
	int		Rounded_to_Nearest_Int(double Val);
	//#####################  Divice Context stuff to delete later //#####################
	CRITICAL_SECTION 	m_CriticalSection, m_CriticalSection2;
	CDC m_DC[MaxDC];
	CBitmap m_bmp[MaxDC];
#define MaxBrush 10
	CBrush	m_Gray[MaxBrush];
	CBrush m_ColorBrush[MaxRGB60];
	CBrush m_bkBrush;
#define MaxPen MaxRGB60
	CPen m_rgbPEN1[MaxPen],m_rgbPEN2[MaxPen];//,m_rgbPEN3[MaxPen];
	CPen	m_NULL, m_RED, m_BLUE, m_GREEN,m_DkGREEN,m_CYAN, m_PINK,m_BROWN,m_VIOLET;
	CPen	m_ORANGE2;
	CPen	m_VIOLET2,m_VIOLET3,m_BLACK,m_BLACK2,m_BLACK3,m_bkPen,m_GRAY2,m_CYAN3,m_BlueGRAY,m_PINK3,m_RED3, m_BLUE3, m_GREEN3;
	CPen	m_RED5, m_BLUE5, m_BLACK5, m_RED8, m_BLUE8, m_BLACK8;
	CFont	m_font;
	//###################################################################################

	//###################################################################################
	//OpenGL
	HDC     m_hDC;
	HGLRC   m_hRC;
	void	DrawGraphics(double z);
	void	ResizeGraphics();
	void	InitGraphics();
	void	SetupPixelFormat();
	int		m_FirstGL;
	double  m_rGL[MaxRGB60], m_gGL[MaxRGB60], m_bGL[MaxRGB60];
	double  m_yAngle, m_xTranslate, m_yTranslate, m_zTranslate, m_fovy; //OpenGL (coordinate; not my program's coordinate)
	float	m_xScale3D, m_yScale3D, m_zScale3D;
	//#define MAXIMUM_WAIT_KeyOBJECTS 7
	//HANDLE	M_KeyDownEvents[MAXIMUM_WAIT_KeyOBJECTS]; 

	//###################################################################################

	int		m_fontHeight;
	void	DisplayTitle();
	COLORREF m_Red, m_Blue, m_Green, m_Pink, m_Cyan, m_Yellow,m_Brown;
	COLORREF m_RGB60[MaxRGB60];
	int m_nScrMaxX, m_nScrMaxY;
	static BOOL	 m_bIsON;
	BOOL	 m_bFistTime;
	BOOL	 m_bShow2D;
	int      m_nKey; //
	int      m_nSpeed; //
	int		 m_nColorShow,m_SavedDC;
	int		m_maxPaneX,m_minPaneX, m_minPaneY,m_maxPaneY,m_InfoPosX;
	int		m_BlownUpPaneID;
	int m_StepByStep;
	//double	m_ImageScale, m_ImageCenterX, m_ImageCenterY;
	double	m_dScaleUpDn;
	BOOL m_bIsToggleON;
	CRect	 m_rectShow3D;
	CFigure m_TwoD1,m_TwoD2,m_TwoD3,m_TwoD4, m_TwoD5, m_TwoD6, m_TwoD7, m_TwoD8, m_TwoD9, m_TwoD10;
	CFigure m_TwoD11, m_TwoD12, m_TwoD13,m_TwoD14,m_TwoD15,m_TwoD16,m_TwoD17,m_TwoD18,m_TwoD19;
	#define MAX_GRAPHS 100
	CFigure m_3D1,m_3D2,m_3D3, m_TwoD101,m_TwoD102,m_TwoD103, m_TwoD104;
	#define MaxFigRow 10
	#define MaxFigCln 10
	#define MaxFigLyr 3
	CFigure m_Fig[MaxFigRow][MaxFigCln][MaxFigLyr];
	CString m_GlobalTitle;
	CRect	m_TitleArea, m_dTitleRect;
	int		m_topOfFigs,m_DoNotErase;
	int		m_NewlyLoaded;
	int		m_SelecFig_row,m_SelecFig_clm,m_SelecFig_dpt;
	void	VirMemDC();
	void	Batch_DrawFigureFrames();
	void	Tics_n_Lines(CFigure *pFig, int HowManyTicsX, int HowManyTicsY) ;
	void	Init_Fig_Positions();
	BOOL	FlagVerScreen;
	BOOL	m_bProceed,m_bIsScrXYforIJ;
	/////////////// Variables for applications (For the Dialogue Box)
	double m_dMinimumX, m_dMaximumX, m_dMinimumY, m_dMaximumY;
	//////////////////////////////////////////////////////////
	CRect	m_savedClientRect;
	void	Re_measure_ClientView();
	void	FontInRect(CDC *pDC, CString sText, CRect &rFont);
	void	Batch_FigPosition_Settings(int sizeOfOneRow, int sizeOfOneClmn, double scale) ;
	void	Batch_SetFontSize(CDC* pDC);
	void	Start_a_NEW_set_of_figures();
	void	Batch_Prepare4FigFrame();
	int		m_dHowManyRows, m_dSizeOfOneRow, m_dHowManyClmns, m_dSizeOfOneClmn;
	void	Manual_Fig_Labels_n_MinMaxXY();
	double	DifferenceToRound(double &NumToAdjust, double VariableRange);
	void	AdjustScale(double &minY, double &maxY);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void	LinearColorMix(COLORREF RGB_array[],int arraySize);
	void	LinearColorMix_adjusted(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix(COLORREF RGB_array[],int arraySize);
	void	CosineColorMix_AxB(COLORREF RGB_array[],int arraySize, int A, int B);
	void	MyOriginalColorMix(COLORREF RGB_array[],int arraySize);
	void	MyOriginalColorMix_forGL(double rGL[],double gGL[],double bGL[], int arraySize);
	void	ColorLegend(CDC *pDC, CFigure *pFig);
	void	FromScrnToDataCoordinate(int x, int y, double &DataX, double &DataY, CFigure *pFig);
	BOOL	LineCrossingRect(int x1,int y1,int x2,int y2,int recLeft,int recTop,int recRight,int recBottom);
	BOOL	RectCrossingRect(int recLeft1,int recTop1,int recRight1,int recBottom1,int recLeft,int recTop,int recRight,int recBottom);
	void	_1st_ThreeD_Anatomy();
	void	_2nd_InteractiveMRI();
	void	Show2D_III();
	void	Show2D_5();
	void	Show2D_4();
	void	Show2D_VI();
	void	Show2D_VII();
	void	Show2D_VIII();
	void	Show3D();
	void	OnMyMessage();
	double	Rectify(double Value){if (Value>0.)return Value; else return 0.;}
	void	WhichFigure(CPoint point);
	void	Figs_Captured();
	void	Hilight_AllCaptured_Figs();
	int		Find_Last_Integer_inString(char *GivenString);
	void	EliminateExcluded();
	void	DrawOneFigure(CFigure *pFig, int isToHighlight);
	void	Hilight_OneCapturedFig(CFigure *pFig);
	void	UnHilight_OneCaptured(CFigure *pFig);
	int		m_numCaptured,m_KeyCode;
	CRect	m_CapturRec;
	void	RefreshEverything();
	inline void Randomize(){
		time_t seconds;
		time(&seconds);
		srand((unsigned int) seconds);
	}
//-------------------------------------------

// Attributes
public:
	CAnalysisDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalysisView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAnalysisView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void On2DGraph_III();
	afx_msg void On2DGraph_II();
	afx_msg void On2DGraph_5();
	afx_msg void On2DGraph_4();
	afx_msg void On2DGraph_VI();
	afx_msg void On2DGraph_VII();
	afx_msg void On2DGraph_VIII();
	afx_msg void On2DGraph_I();
	afx_msg void OnChangeParameters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDeleteFigs();
	afx_msg void OnEditPoints();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileSaveDrawing();
	afx_msg void OnShow_Origin();
	afx_msg void OnUpdateShow_Origin(CCmdUI *pCmdUI);
	afx_msg void OnUpdate2dgraph3(CCmdUI *pCmdUI);
	afx_msg void OnUpdate2dgraph4(CCmdUI *pCmdUI);
	afx_msg void OnUpdate2dgraph(CCmdUI *pCmdUI);
	afx_msg void OnSelect();
	afx_msg void OnUpdateSelect(CCmdUI *pCmdUI);
	afx_msg void OnMakeLineSegments();
	afx_msg void OnUpdateMakeLineSegments(CCmdUI *pCmdUI);
	afx_msg void OnMakePoints();
	afx_msg void OnUpdateMakePoints(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void Define_3D_Origin_inMRI_byMouseClick();
	afx_msg void OnUpdate_Locate_3D_Origin_inMRI_byMouseClick(CCmdUI *pCmdUI);
	afx_msg void OnPitchRollYaw();
	afx_msg void OnUserInput_of_Electrode_Position();
	afx_msg void OnShowTexture();
	afx_msg void OnUpdateShowTexture(CCmdUI *pCmdUI);
	afx_msg void OnView_DistanceFromElectrode();
	afx_msg void OnShowDrawing();
	afx_msg void OnUpdateShowDrawing(CCmdUI *pCmdUI);
	afx_msg void OnDefineGridOrigin();
	afx_msg void OnUpdateLocateGridOrigin(CCmdUI *pCmdUI);
	afx_msg void OnToolsRenameMRIs();
	afx_msg void OnResetFigure();
	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI *pCmdUI);
	afx_msg void OnMarking();
	afx_msg void OnUpdateMarking(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in AnalysisView.cpp
inline CAnalysisDoc* CAnalysisView::GetDocument()
   { return (CAnalysisDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISVIEW_H__4E7AB3C8_4618_11D3_8364_C9075A873F60__INCLUDED_)
