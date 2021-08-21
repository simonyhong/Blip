#include "VarDefinitionsSDF.h"

#define MaxTICS 100
#define SQ_DISTANCE_AB_1D(x1,  x2)          ( ((x1)-(x2))*((x1)-(x2))  )
#define lX(X) (int)(( X)*m_dDx_DoubleGapX_Per_Max_MinX+m_TransformX) //They are in (int) form because it is in screen pixel unit
#define lY(Y) (int)((-Y)*m_dDy_DoubleGapY_Per_Max_MinY+m_TransformY)//They are in (int) form because it is in screen pixel unit

struct ClickableLabel 
{
	float m_RelativePos;
	int status, status2;
	int	m_AssociatedVar, m_Var2;
	CRect	StringRect,ControlRect;
	CString   NameString, NameStringStem;
};
struct ECodeCondition 
{
	int IsRange;
	int	m_ECode1, m_ECode2;
};

class CXCOR_View;
class CFigure: public CObject
{
public:
	//#########################  Task-realted   ###########################################################
	ClickableLabel m_XCOR_Label;
	int		m_ChanID, m_NnID;
	double	m_dSigma;
	void	Disp_XCOR(double *pXCOR, int DataSize, CDC *pDC);
	void	DispLabel(ClickableLabel &Lable, CDC* pobjDC, CDC* pDC);
	CXCOR_View *m_pView;
	void	Disp_Raster(CDC *pDC);
	//########################m  Drawing Basic stuff  ####################################################
	int		m_row,m_clm,m_dpth;
	int	   m_IsBlownUp,m_IsTickY; //To blow up the window
	int	   m_nTx,m_nTy,m_nBx,m_nBy, m_nSavedTx,m_nselSavedTy,m_nSavedBx,m_nSavedBy, m_BlownUpTx,m_BlownUpTy,m_BlownUpBx,m_BlownUpBy, m_normTx,m_normTy,m_normBx,m_normBy;
	int		m_Captured,m_Deleted;
	int	   m_nPaneTx,m_nPaneTy,m_nPaneBx,m_nPaneBy;
	int		m_page,m_NumPages;
	long	m_FontWidth,m_FontHeight;
	int		m_howManyYguideLines,m_howManyXguideLines,m_IsLabeledTicX[MaxTICS],m_IsLabeledTicY[MaxTICS];
	float	m_YTics[MaxTICS],m_XTics[MaxTICS];
	char   m_cXaxis[MaxDC][100], m_cYaxis[MaxDC][100];
	int		m_nYlabelPosX,m_nYlabelPosY;
	char   m_cWhatever[100];
	double m_dMinX, m_dMaxX, m_dMinY, m_dMaxY, m_dMeanY, m_dStepX,m_dStepY,m_dSpanX, m_dSpanY;
	double m_dGapX,m_dGapY,m_nBy_GapY;
	double m_dDy_DoubleGapY_Per_Max_MinY, m_TransformY;
	double m_dDx_DoubleGapX_Per_Max_MinX, m_TransformX;
	int m_nTranslateX,m_LateralMargin;
	POINT m_vert[3]; //For triangle

	//####################  Device Context stuff that needs to be erased when the program ends  ######################
	static CPen m_PinkPen,m_OrangePen,m_RedPen,m_BluePen,m_DkGreenPen;
	static CPen m_BlackPen1,m_BlackPen3,m_GrayPen,m_LightGrayPen, m_LightGrayPen2, m_LightGrayPen3,m_Null_Pen;
	static CBrush m_bkBrush,m_GrayBrush,m_LightGrayBrush;
	static int m_DiviceContaxtCreatedAlready;
	//################################################################################################################
	
	double m_dLabelBias,m_dLabelScale;
	COLORREF m_Red, m_Blue, m_Green,m_DkGreen, m_Pink, m_Cyan, m_Yellow,m_Brown,m_Violet,m_BLACK;
	BOOL m_bDrawFRAME;

	int m_FirstYlabel_left,m_FirstYlabel_right,m_FirstYlabel_top,m_FirstYlabel_bottom;
	int m_FirstXlabel_left,m_FirstXlabel_right,m_FirstXlabel_top,m_FirstXlabel_bottom;
	int m_LastYlabel_left,m_LastYlabel_right,m_LastYlabel_top,m_LastYlabel_bottom;
	int m_LastXlabel_left,m_LastXlabel_right,m_LastXlabel_top,m_LastXlabel_bottom;

	//void RectOfLabelAlignedToLEFT(ClickableLabel &Label);
	void RectOfLabelAlignedAtCENTER(ClickableLabel &Label);
	void SetLabelsXY(char Xaxis[100], char Yaxis[100]);
	void SetMinMaxXY(double Min_X, double Max_X, double Min_Y, double Max_Y);
	void SetFig_Position_in_ClientView(int Tx, int Ty, int Bx, int By);
	void SetFontSize(CDC* pDC);
	void Prepare4FigFrame();
	void Frame_Graph2D(CDC* pDC,int oxFRAME, int IsToEmbose);
	void formal_Frame2D(CDC* pDC, int oxFRAME);
	void Graph2D(double X1, double Y1, double X, double Y,int Disp, CDC* pDC);
	void Graph2D(double , double ,int , CDC* ); 
	void EmboseAndHighlightFrame(CDC* pDC, int isToHighlight, int IsToEmbose);
	void LineCrossingY(double X, double Y, CDC* pDC);
	void Triangle(double X, double Y,int size,CDC* pDC);
	void Circle(double X, double Y,int size,CDC* pDC);
	void ErrorBar(double X, double Y, double error, CDC* pDC);
	void ErrorWing(double X, double Y, double error, CDC* pDC);
	int	 FromDataToScreen_X(double X);
	void FromDataToScreenXY(double X, double Y, int &x, int &y);
	void peri_Histogram(double X, double Y,int BinWidth, CDC* pDC);
	void Histogram4(double X, double Y, int BinWidth,int Nth, CDC* pDC);
	void Histogram4(double X, double Y, float BinWidth,int Nth, CDC* pDC);
	void peri_Hist_Horizontal(double X, double Y,int BinWidth, CDC* pDC);
	void PutIntVal(double X, double Y, int NumToPrint, CDC* pDC);
	void PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC);
	void PutVal_p5(double X, double Y, double NumToPrint, CDC* pDC);
	void PutVal_Graph2D(double X, double Y, double NumToPrint, CDC* pDC);
	void PutText(double X, double Y,char ToPrint[], CDC* pDC);
	void PutChar_After10(double X, double Y,char ToPrint[], CDC* pDC);
	void MySetTick(double X, double Y, CDC* pDC);
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
	void MySet2x2BlackBox(double X, double Y, CDC* pDC);
	void MySet2x1BlackBox(double X, double Y, CDC* pDC);
	void MySet2x2RedBox(double X, double Y, CDC* pDC);
	void FillSolidRect(double tlX, double tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect_CenteredY(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect2_CenteredY(double X, double Y ,CDC* pDC);
	void FillSolidRect(int tlX, int tlY,int width, int height, COLORREF rgb ,CDC* pDC);
	void FillSolidRect13x13(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect8x8(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void FillSolidRect7x7(int tlX, int tlY, COLORREF rgb ,CDC* pDC);
	void DataToScreen(double X, double Y, int &x, int &y);
	void DataToScreen(double X, double Y, float &x, float &y);
	void ErasePane_2D( CDC* pDC);
	void Abs_Ellipse(double X1, double Y1,int size, CDC* pDC);
	void EraseFrameBlack(CDC* pm_memDC);
	void EraseFrameWHITE(CDC* pm_memDC);
	void RectangleAB(double X1, double Y1, double X2, double Y2, CDC* pDC); 
	void RepaintPane_2D( CDC* pDC); 
	void RefreshPane( CDC* objDC, CDC* pm_memDC);
	void RefreshFrame( CDC* objDC, CDC* pm_memDC);

	CFigure();
	virtual ~CFigure();
};