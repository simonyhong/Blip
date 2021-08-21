#include "VarDefinitions.h"

#define xFRAME 0
#define oFRAME 1
#define PERI 2
#define xSCALE 0
#define oSCALE 1
#define MaxTICS 100
#define ButtonWidth 70
#define ButtonHight 13
#define SmallButtonWidth 10
#define SmallButtonHight 6
#define BIT_BLT(Rect, pDC) Rect.left, Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top, pDC, Rect.left, Rect.top,SRCCOPY

struct ClickableLabel 
{
	int status, AtTopOrBottom;
	float	m_RelativePos;
	CRect	StringRect;
	CRect	ControlRect, PrvWholeRect;
	CString   NameString;
};

class CMOXY_View;
class CFigure: public CObject
{
public:
	int		m_FigID,m_IsForVIS;
	int	   m_IsTickY;
	int	   m_nTx,m_nTy,m_nBx,m_nBy, m_nSavedTx,m_nSavedTy,m_nSavedBx,m_nSavedBy, m_BlownUpTx,m_BlownUpTy,m_BlownUpBx,m_BlownUpBy, m_normTx,m_normTy,m_normBx,m_normBy;
	double m_ScreenCenterX, m_ScreenCenterY; //For the XYmonitor Only.
	int	   m_nPaneTx,m_nPaneTy,m_nPaneBx,m_nPaneBy;
	int		m_lX0,m_lY0;
	int		m_page,m_NumPages;
	long	m_FontWidth,m_FontHeight;
	int		m_howManyY,m_howManyX,m_IsLabeledTicX[MaxTICS],m_IsLabeledTicY[MaxTICS];
	float	m_YTics[MaxTICS],m_XTics[MaxTICS], m_Y2ndTics[MaxTICS];
	char   m_cXaxis[MaxDC][100], m_cYaxis[MaxDC][100];
	ClickableLabel m_FileName, m_GridNet, m_RwdAmount, m_SuperImposeStatus,m_TriggerOptionStatus,m_NnRecordingChan, m_BehavioralObj;
	CRect	m_TitlePosRect,m_TitleAreaRect;	
	CArray<CRect, CRect& > m_VarToDispRECT;
	CArray<int, int& > m_VarToDispLineEnd;
	CArray<CString, CString& > m_VarToDispString;
	void	PositionOfUserPickedVariables(int VarID, int Left0_Or_Right1_axis);
	int		m_numUserPickedVariables;
	int		m_nYlabelPosX,m_nYlabelPosY;
	int		m_nTriggeredLabelPosX,m_nTriggeredLabelPosY;
	char *m_pBaseOfFileName, *m_pJustFileName;
	int *m_pIsSaving;
	void	ListObjIDs(CDC* pDC, int SelectedObjID);
	int		WhichObjSelected(CPoint point);
	void	DisplayLabel(ClickableLabel &Label);
	//###############################################################################################################
	char   m_cWhatever[100];
	double m_dMinX, m_dMaxX, m_dMinY, m_dMaxY, m_dMeanY, m_dStepX,m_dStepY,m_dSpanX, m_dSpanY;
	double m_dMinY2, m_dMaxY2, m_dSpanY2;
	int		m_Is2ndAxis;
	double m_dGapX,m_dGapY,m_nBy_GapY;
	double m_dDy_DoubleGapY_Per_Max_MinY, m_TransformY, m_dDy_DoubleGapY_Per_Max_MinY2, m_TransformY2;
	double m_dDx_DoubleGapX_Per_Max_MinX, m_TransformX;
	int m_nTranslateX;
	POINT m_vert[3]; //For triangle
	//####################  Device Context stuff that needs to be erased when the program ends  ######################
	CDC *m_pDC;
	static CPen m_OrangePen,m_RedPen,m_RedPen2,m_BluePen,m_GreenPen;
	static CPen m_BlackPen2,m_BlackPen3,m_GrayPen,m_LightGrayPen,m_Null_Pen,m_VIOLETpen;
	static CBrush m_BlackBrush, m_WhiteBrush, m_RedBrush,m_LightGrayBrush, m_TmpBrush;
	static int m_DiviceContaxtCreatedAlready;
	//################################################################################################################

	double m_dLabelBias,m_dLabelScale;
	int m_NumVariables_onLeftAxes,m_NumVariables_onRightAxes;
	COLORREF m_Red, m_Blue, m_Green,m_DkGreenPen, m_Pink, m_Cyan, m_Yellow,m_Brown,m_Violet,m_BLACK;
	static BOOL m_bUsingDC,m_bUsingDC2, m_bUsingDC3;
	BOOL m_bDrawFRAME;

	int m_FirstYlabel_left,m_FirstYlabel_right,m_FirstYlabel_top,m_FirstYlabel_bottom;
	int m_FirstXlabel_left,m_FirstXlabel_right,m_FirstXlabel_top,m_FirstXlabel_bottom;
	int m_LastYlabel_left,m_LastYlabel_right,m_LastYlabel_top,m_LastYlabel_bottom;
	int m_LastXlabel_left,m_LastXlabel_right,m_LastXlabel_top,m_LastXlabel_bottom;
	int m_FirstY2label_left,m_FirstY2label_right,m_FirstY2label_top,m_FirstY2label_bottom;
	int m_LastY2label_left,m_LastY2label_right,m_LastY2label_top,m_LastY2label_bottom;

	CFigure();
	virtual ~CFigure();
	void RefreshXYTitleArea(CDC *pObjDC);
	void DisplayFileName_and_RecordingStatus(CMOXY_View *pView);
	void RectOfLabel(ClickableLabel &Label, CDC *pDC);
	int	Rounded_to_nearest_Int(double Val);
	void Set_2ndMinMaxY(double Min_Y2, double Max_Y2);
	void SetLabelsXY(char Xaxis[100], char Yaxis[100]);
	void SetFig_Pos_n_Labels_XY(int Tx, int Ty, int Bx, int By, char Xaxis[100], char Yaxis[100]);
	void ComputeMapping_X_Y_Y2();
	void SetFontSize(CDC* pDC);
	void Prepare4FigFrame(CDC* pDC);
	void Set_MinMax_n_Labels_XY(char Xaxis[100], char Yaxis[100],double Min_X, double Max_X, double Min_Y, double Max_Y, int oxFRAME);
	void SetFigPositionsXY(int Tx, int Ty, int Bx, int By);
	void Frame_Graph2D(double Min_X, double Max_X, double Min_Y, double Max_Y, CDC* pDC, int oxFRAME);
	void Frame_Graph2D(CDC* pDC);
	void Frame2D_for_VIS_Replica(int DisplayMode, CDC* pDC, CMOXY_View *pView);
	void Graph2D(double X1, double Y1, double X, double Y,int Disp, CDC* pDC);
	void Graph2D(double , double ,int , CDC* ); 
	void MyMoveTo(double , double); 
	void MyLineTo(double , double); 
	void MoveTo2nd(double , double, CDC* pDC); 
	void LineTo2nd(double , double, CDC* pDC); 
	void ThinVioletLine(double , double, double , double, CDC* pDC); 
	void ThickRedLine(double , double, double , double, CDC* pDC); 
	void LineAtoB(double X, double Y,double X2, double Y2, CDC* pDC); 
	void LineAtoB2nd(double X, double Y,double X2, double Y2, CDC* pDC); 
	void Triangle(double X, double Y,int size,CDC* pDC);
	void Circle(double X, double Y,int size,CDC* pDC);
	void ErrorBar(double X, double Y, double error, CDC* pDC);
	void ErrorWing(double X, double Y, double error, CDC* pDC);
	void PutIntVal(double X, double Y, int NumToPrint, CDC* pDC);
	void PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC);
	void PutVal_p5(double X, double Y, double NumToPrint, CDC* pDC);
	void PutVal_p3(double X, double Y, double NumToPrint, CDC* pDC);
	void PutText(double X, double Y,char ToPrint[], CDC* pDC);
	void PutText_After10(double X, double Y,char ToPrint[], CDC* pDC);
	void MySetPixel(double X, double Y, int GRAY, CDC* pDC);
	void MySetPixelRED(double X, double Y, CDC* pDC);
	void MySetPixelGREEN(double X, double Y, CDC* pDC);
	void MySetPixelDKGRN(double X, double Y, CDC* pDC);
	void MySetPixelBLUE(double X, double Y, CDC* pDC);
	void MySetPixelYELLOW(double X, double Y, CDC* pDC);
	void MySetPixelPINK(double X, double Y, CDC* pDC);
	void MySetPixelCYAN(double X, double Y, CDC* pDC);
	void MySetPixelBROWN(double X, double Y, CDC* pDC);
	void MySetPixelVIOLET(double X, double Y, CDC* pDC);
	void MySetPixelBLACK(double X, double Y, CDC* pDC);
	void MySetPixelRGB(double X, double Y, COLORREF *rgb, CDC* pDC);
	void MySetPixelRGB(int X, int Y, int R, int G, int B, CDC* pDC);
	void MySetPixelRGB(double X, double Y, int R, int G, int B, CDC* pDC);
	void MySetTick(double X, double Y, CDC* pDC);
	void MySetBox(double X, double Y, CDC* pDC);
	void MySetRedBox(double X, double Y, CDC* pDC);
	void FillSolidRect(int cntX, int cntY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect(double cntX, double cntY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect_CenteredY(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect2_CenteredY(double X, double Y ,CDC* pDC);
	void FillSolidRect13x13(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect8x8(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect7x7(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void ContourRect(float LeftBoundary, float UpperBoundary,float RightBoundary, float LowerBoundary, CDC* pDC);
	void PolyContour(CPoint points[],int N, CDC* pDC);
	void FromDataToScreenXY(double X, double Y, int &x, int &y);
	void FromDataToScreenXY(double X, double Y, float &x, float &y);
	int	 FromDataToScreenX(double X);
	int	 FromDataToScreenY(double X);
	double FromScreenToDataX(int X);
	double FromScreenToDataY(int Y);
	void ErasePane_2D( CDC* pDC);
	void Abs_Ellipse(float X1, float Y1,int size, CDC* pDC);
	void EraseFrameBlack(CDC* pm_memDC);
	void EraseFrameWHITE(CDC* pm_memDC);
	void RectangleAB(double X1, double Y1, double X2, double Y2, CDC* pDC); 
	void PlotGrid(int DisplayMode, CDC* pDC); 
	//void PlotPolarMash( CDC* pDC); 
	void RefreshPane( CDC* objDC, CDC* pm_memDC);
	void RefreshFrame( CDC* objDC, CDC* pm_memDC);
	void FillSolidRect_ReportBackTopLeftXY(double cntX, double cntY, double width, double height, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &widthDisp, int &heightDisp);
	void ReportBackTopLeftXY(double cntX, double cntY, double width, double height, int &topleftX, int &topleftY, int &widthDisp, int &heightDisp);
	void ReportBackTopLeftXY(double cntX, double cntY,int WidthInPixel, int HeightInPixel, int &topleftX, int &topleftY);
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}

	//############### Angular-projected coordinates
	int	m_lXvis0,m_lYvis0;
	double m_ScreenScale;
	void FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY_VisReplica(double cntX, double cntY, double width, double height, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &widthDisp, int &heightDisp);
	void Ellipse_SizeAroundFovia_ReportBackTopLeftXY_VisReplica(      double cntX, double cntY, double width, double height, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &widthDisp, int &heightDisp);
	void Ellipse_SizeAroundFovia_ReportBackTopLeftXY(      double cntX, double cntY, double width, double height, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &widthDisp, int &heightDisp);
	void ReportBackTopLeftXY_VisReplica(double cntX, double cntY, double width, double height, int &topleftX, int &topleftY, int &widthDisp, int &heightDisp);
	void FillSolidRect_ReportBackTopLeft_Eye(double cntX, double cntY, double width, double height, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY);
	void ReportBackTopLeftXY_Eye(double cntX, double cntY,int EyeSquareWidth, int EyeSquareheight, int &topleftX, int &topleftY);
	void Prepare4VisReplica(); 
	double FromScreenToDataX_VisReplica(int X);
	double FromScreenToDataY_VisReplica(int Y);
	void ContourRect_VisReplica(float LeftBoundary, float UpperBoundary,float RightBoundary, float LowerBoundary, CDC* pDC);
};

