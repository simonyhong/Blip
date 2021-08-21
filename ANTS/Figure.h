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
#define NUM_MEASURING_LINES 2
#define BIT_BLT(Rect, pDC) Rect.left, Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top, pDC, Rect.left, Rect.top,SRCCOPY

struct ClickableLabel 
{
	int m_ChanID;
	CRect	StringRect;
	CString   NameString;
};

class CAiChannel;
class CFigure: public CObject
{
public:
	ClickableLabel m_ClickToSave,m_RecordingChan, m_StimGatingRelay, m_Focus;
	int		m_FigID, m_row,m_clm, m_lyr;
	CAiChannel *pChan;
	int		m_NumLfpDrawn;
	//####################  Measuring Lines   #######################################################################
	double	m_MeasuringLineTime[3];
	int		m_MeasureingLine_ScrnCoord[3];
	CRect	m_MeasuringTimeRect[3], prvPeriStimTimeRect, prvPostStimTimeRect;
	void	DrawMeasuringLines(CPen *pPen);
	//################################################################################################################
	int	   m_IsBlownUp,m_IsTickY; //To blow up the window
	int	   m_nTx,m_nTy,m_nBx,m_nBy, m_nSavedTx,m_nselSavedTy,m_nSavedBx,m_nSavedBy, m_BlownUpTx,m_BlownUpTy,m_BlownUpBx,m_BlownUpBy, m_normTx,m_normTy,m_normBx,m_normBy;
	int		m_Captured,m_Deleted,m_toSave, m_IsToDisplay, m_IsIt_TriggeringRecordingChannel;
	int	   m_nPaneTx,m_nPaneTy,m_nPaneBx,m_nPaneBy;
	long	m_FontWidth,m_FontHeight;
	int		m_howManyY,m_howManyX,m_IsLabeledTicX[MaxTICS],m_IsLabeledTicY[MaxTICS];
	float	m_YTics[MaxTICS],m_XTics[MaxTICS];
	char   m_cXaxis[100], m_cYaxis[100];
	int		m_nYlabelPosX,m_nYlabelPosY;
	double m_dMinX, m_dMaxX, m_dMinY, m_dMaxY, m_dMeanY, m_dStepX,m_dStepY,m_dSpanX, m_dSpanY;
	double m_dGapX,m_dGapY,m_nBy_GapY;
	double m_dDy_DoubleGapY_Per_Max_MinY,m_TransformX;
	double m_dDx_DoubleGapX_Per_Max_MinX,m_TransformY;
	int m_nTranslateX,m_LateralMargin;
	POINT m_vert[3]; //For triangle
	//Zapper
	double	m_LearnedLevel;
	//####################  Device Context stuff that needs to be erased when the program ends  ######################
	static CPen m_PinkPen,m_OrangePen,m_RedPen,m_BluePen,m_GreenPen;
	static CPen m_BlackPen1,m_BlackPen2 ,m_BlackPen3,m_GrayPen,m_LightGrayPen,m_Null_Pen;
	static CBrush m_bkBrush,m_GrayBrush,m_LightGrayBrush,m_RedBrush;
	static int m_DiviceContaxtCreatedAlready;

	//################################################################################################################
	CDC *m_pDC;
	double m_dLabelBias,m_dLabelScale;
	COLORREF m_Red, m_Blue, m_Green,m_DkGreen, m_Pink, m_Cyan, m_Yellow,m_Brown,m_Violet,m_BLACK;
	static BOOL m_bUsingDC,m_bUsingDC2, m_bUsingDC3;
	BOOL m_bDrawFRAME;

	int m_FirstYlabel_left,m_FirstYlabel_right,m_FirstYlabel_top,m_FirstYlabel_bottom;
	int m_FirstXlabel_left,m_FirstXlabel_right,m_FirstXlabel_top,m_FirstXlabel_bottom;
	int m_LastYlabel_left,m_LastYlabel_right,m_LastYlabel_top,m_LastYlabel_bottom;
	int m_LastXlabel_left,m_LastXlabel_right,m_LastXlabel_top,m_LastXlabel_bottom;

	CFigure();
	virtual ~CFigure();
	void RectOfLabel(float relX, int LengthOfLabel, CRect &rect);
	void SetLabelsXY(char Xaxis[100], char Yaxis[100]);
	void SetMinMaxXY(double Min_X, double Max_X, double Min_Y, double Max_Y);
	void SetFig_Position_in_ClientView(int Tx, int Ty, int Bx, int By);
	void SetFontSize(CDC* pDC);
	void Compute_Mapping_XY();
	void Disp_Rec_Stim_Names(CDC* pDC);
	void RefreshNameBox(CDC* objDC, CDC* pm_memDC);
	void RefreshClickToSaveBox(CDC* objDC, CDC* pm_memDC);
	void Frame_Graph2D(CDC* pDC,int oxFRAME);
	void formal_Frame2D(CDC* pDC, int oxFRAME);
	void Graph2D(double X1, double Y1, double X, double Y,int Disp, CDC* pDC);
	void Graph2D(double , double ,int , CDC* ); 
	void MyMoveTo(double , double); 
	void MyLineTo(double , double); 
	void MyLineAtoB(double , double,double , double); 
	void MyMoveTo(double , double, CDC *pObjDC); 
	void MyLineTo(double , double, CDC *pObjDC); 
	void MyLineAtoB(double , double,double , double, CDC *pObjDC); 
	void Triangle(double X, double Y,int size,CDC* pDC);
	void Circle(double X, double Y,int size,CDC* pDC);
	void ErrorBar(double X, double Y, double error, CDC* pDC);
	void ErrorWing(double X, double Y, double error, CDC* pDC);
	void peri_Histogram(double X, double Y,int BinWidth, CDC* pDC);
	void Histogram4(double X, double Y, int BinWidth,int Nth, CDC* pDC);
	void Histogram4(double X, double Y, float BinWidth,int Nth, CDC* pDC);
	void peri_Hist_Horizontal(double X, double Y,int BinWidth, CDC* pDC);
	void PutIntVal(double X, double Y, int NumToPrint, CDC* pDC);
	void PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC);
	void PutVal_p2(double X, double Y, double NumToPrint, CDC* pDC);
	void PutVal_Graph2D(double X, double Y, double NumToPrint, CDC* pDC);
	void PutText(double X, double Y,char ToPrint[], CDC* pDC);
	void PutChar_After10(double X, double Y,char ToPrint[], CDC* pDC);
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
	void MySetBox(double X, double Y, CDC* pDC);
	void MySetRedBox(double X, double Y, CDC* pDC);
	void FillSolidRect(double tlX, double tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect_ShiftToX(double tlX, double tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect_CenteredY(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect(int tlX, int tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect13x13(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect8x8(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect7x7(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FromDataToScreenXY(double X, double Y, int &x, int &y);
	void FromDataToScreenXY(double X, double Y, float &x, float &y);
	int	 FromDataToScreenX(double X);
	int	 FromDataToScreenY(double Y);
	double FromScreenToDataX(int X);
	double FromScreenToDataY(int Y);
	void ErasePane_2D( CDC* pDC);
	void Abs_Ellipse(double X1, double Y1,int size, CDC* pDC);
	void EraseFrameBlack(CDC* pm_memDC);
	void EraseFrameWHITE(CDC* pm_memDC);
	void RectangleAB(double X1, double Y1, double X2, double Y2, CDC* pDC); 
	void RepaintPane_2D( CDC* pDC); 
	void RefreshPane( CDC* objDC, CDC* pm_memDC);
	void RefreshFromXandSome(double X, CDC* pObjDC, CDC* pMemDC); 
	void RefreshFrame( CDC* objDC, CDC* pm_memDC);
};