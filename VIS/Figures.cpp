#include "stdafx.h"
#include <math.h>
#include "Figures.h"

BOOL CFigure::m_bUsingDC=0;
BOOL CFigure::m_bUsingDC2=0;
BOOL CFigure::m_bUsingDC3=0;
CPen CFigure::m_PinkPen,CFigure::m_OrangePen,CFigure::m_RedPen,CFigure::m_BluePen,CFigure::m_GreenPen;
CPen CFigure::m_BlackPen2,CFigure::m_BlackPen3,CFigure::m_GrayPen,CFigure::m_LightGrayPen,CFigure::m_Null_Pen;
CBrush CFigure::m_bkBrush,CFigure::m_GrayBrush,CFigure::m_LightGrayBrush, CFigure::m_TmpBrush;
int	CFigure::m_DiviceContaxtCreatedAlready=0;

#define CLIENT_VIEW_SIZE_X         ((m_ClientViewBottomX)-(m_ClientViewTopX))
#define CLIENT_VIEW_SIZE_Y         ((m_ClientViewBottomY)-(m_ClientViewTopY))
#define SHIFT_TO_X 5

//Linear scale
//#define lX(X)  (int)(( X)*m_dDy_Per_SpanY +m_ScreenCenterX )//They are in (int) form because it is in screen pixel unit
//#define lY(Y)  (int)((-Y)*m_dDy_Per_SpanY +m_ScreenCenterY )//They are in (int) form because it is in screen pixel unit

//Tangential scale
#define lX(X)  ((int)(m_ScreenScale*tan((double)( (X)*0.0174532925)) +m_ScreenCenterX ))//1 degree == 0.0174532925 radians
#define lY(Y)  ((int)(m_ScreenScale*tan((double)(-(Y)*0.0174532925)) +m_ScreenCenterY ))//1 degree == 0.0174532925 radians

CFigure::CFigure()
{
	m_IsBlownUp=0; m_IsTickY=0;
	m_ClientViewTopX=500; m_ClientViewTopY=0; m_ClientViewBottomX=1000; m_ClientViewBottomY=500;
	m_BlownUpTx=200;m_BlownUpTy=5; m_BlownUpBx=820; m_BlownUpBy=605;
	m_dStepX=0;m_dStepY=0;
	m_LateralMargin=0;
	m_howManyX=-1;m_howManyY=-1;
	m_page=0; m_NumPages=1;
	m_FontWidth=5;	m_FontHeight=-1;// m_FontHeight is set to -1 to warn the user to set it first.
	m_Captured=0; m_Deleted=0;
	m_Title="NoName";
	m_ScreenScale=100;
	if(m_DiviceContaxtCreatedAlready==0){
		m_DiviceContaxtCreatedAlready=1;
		m_PinkPen.CreatePen(PS_SOLID, 1,RGB(250,230,230));
		m_OrangePen.CreatePen(PS_SOLID, 1,RGB(255,190,50));
		m_RedPen.CreatePen(PS_SOLID, 1,RGB(255,50,50));
		m_BluePen.CreatePen(PS_SOLID, 1,RGB(0,0,255));
		m_GreenPen.CreatePen(PS_SOLID, 1,RGB(0,200,0));
		m_BlackPen2.CreatePen(PS_SOLID, 2,RGB(0,0,0));
		m_BlackPen3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
		m_Null_Pen.CreatePen(PS_NULL, 1,RGB(0,0,0));
		m_LightGrayPen.CreatePen(PS_SOLID, 1,RGB(230,230,230));
		m_GrayPen.CreatePen(PS_SOLID, 1,RGB(170,170,170));
		m_bkBrush.CreateStockObject(WHITE_BRUSH);
	
		LOGBRUSH LogBrh;
		LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(200,200,200);	
		m_GrayBrush.CreateBrushIndirect( &LogBrh );
		LogBrh.lbColor=RGB(240,240,240);
		m_LightGrayBrush.CreateBrushIndirect( &LogBrh );
	}
	
	int i;
	for(i=0;i<MaxDC;i++)sprintf(m_cXaxis[i],"");
	for(i=0;i<MaxDC;i++)sprintf(m_cYaxis[i],"");

	m_dMinX=0., m_dMaxX=1000., m_dMinY=0., m_dMaxY=1.; m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=-1;// m_dSpanY has been set to -1 to warn the user to set the "scales" of X, Y axis first
	m_dDy_Per_SpanY =   ((CLIENT_VIEW_SIZE_Y)/(m_dSpanY));
	//m_dDx_Per_SpanX =   ((CLIENT_VIEW_SIZE_X-DoubleGapX)/(m_dSpanX));
	m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.
	m_dLabelBias=0;m_dLabelScale=1;
	m_Red=RGB(255,0,0); m_Blue=RGB(100,100,255); m_Green=RGB(0,255,0); m_DkGreen=RGB(0,155,0); m_Pink=RGB(255,150,150); 
	m_Cyan=RGB(0,255,255); m_Yellow=RGB(240,240,0); m_Brown=RGB(240,130,0); m_Violet=RGB(200,90,250);m_BLACK=RGB(0,0,0);
}

CFigure::~CFigure()
{
	m_PinkPen.DeleteObject(); m_OrangePen.DeleteObject();m_RedPen.DeleteObject();m_BluePen.DeleteObject();m_GreenPen.DeleteObject();
	m_BlackPen2.DeleteObject();m_BlackPen3.DeleteObject();m_GrayPen.DeleteObject();m_LightGrayPen.DeleteObject();m_Null_Pen.DeleteObject();
	m_bkBrush.DeleteObject();m_GrayBrush.DeleteObject();m_LightGrayBrush.DeleteObject(); m_TmpBrush.DeleteObject();
}

void CFigure::SetLabelsXY(char Xaxis[50], char Yaxis[50])
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////
}

void CFigure::SetMinMaxXY(double Min_X, double Max_X, double Min_Y, double Max_Y)
{
	if((m_dMinX>=m_dMaxX)||(m_dMinY>=m_dMaxY)){AfxMessageBox("(m_dMinX>=m_dMaxX)||(m_dMinY>=m_dMaxY) ==> Try again", MB_OK); return;}
	m_dMinX=Min_X, m_dMaxX=Max_X, m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; 
	ComputeMappingXY();
}
void CFigure::Set_MinMax_n_Labels_XY(char Xaxis[50], char Yaxis[50],double Min_X, double Max_X, double Min_Y, double Max_Y,int oxFRAME)
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////

	m_dMinX=Min_X, m_dMaxX=Max_X, m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; m_dSpanX=Max_X-Min_X; m_dSpanY=Max_Y-Min_Y; 
	SetFig_Position_in_ClientView(m_normTx,m_normTy,m_normBx,m_normBy);
	ComputeMappingXY();
}

void CFigure::SetFig_Position_in_ClientView(int Tx, int Ty, int Bx, int By)
{
	m_ClientViewTopX=Tx; m_ClientViewTopY=Ty; m_ClientViewBottomX=Bx; m_ClientViewBottomY=By;
}

void CFigure::SetFontSize(CDC* pDC)
{
	// Get the sizes of the font
	CFont* pFont = pDC->GetCurrentFont();
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);
	m_FontWidth=LogFont.lfWidth;
	m_FontHeight=LogFont.lfHeight;
}

void CFigure::ComputeMappingXY()
{
	if(m_FontHeight<=0){
		AfxMessageBox("It is a good idea to set the size of font", MB_OK); m_FontHeight=10;
	}

	m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY;
	//Set the size of the Gap between the graph pane and the edge of the frame: width of the scale legend area.
	m_dDy_Per_SpanY =   ((CLIENT_VIEW_SIZE_Y)/(m_dSpanY));
	//m_dDx_Per_SpanX =   ((CLIENT_VIEW_SIZE_X)/(m_dSpanX));
	m_ScreenCenterX=CLIENT_VIEW_SIZE_X*0.5;
	m_ScreenCenterY=CLIENT_VIEW_SIZE_Y*0.5;

	//scale x tan(m_ScreenSize_inDegrees_4VIS) =smaller_side_screen_width/2.
	int smaller_side_screen_width_over_2;
	if(m_normBx<m_normBy)smaller_side_screen_width_over_2=m_normBx*0.5; else smaller_side_screen_width_over_2=m_normBy*0.5;
	m_ScreenScale=smaller_side_screen_width_over_2/tan(m_dMaxX*0.0174532925);
	
	m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.

	m_TitlePosX_left=m_ScreenCenterX-m_FontWidth*m_Title.GetLength()*0.5;
	m_TitlePosX_right=m_ScreenCenterX+m_FontWidth*m_Title.GetLength()*0.5;
	m_TitlePosX_top=(int)(m_ClientViewTopY+m_FontHeight*0.5); m_TitlePosX_bottom=m_TitlePosX_top+m_FontHeight;
}

void CFigure::DispNameOfFigure(CDC* pDC)
{
	pDC->TextOut(m_TitlePosX_left,m_TitlePosX_top,m_Title);//Blow-up button
} 	

void CFigure::RefreshNameBox(CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_TitlePosX_left,m_TitlePosX_top,m_TitlePosX_right-m_TitlePosX_left,m_TitlePosX_bottom-m_TitlePosX_top,	pm_memDC,m_TitlePosX_left,m_TitlePosX_top,SRCCOPY);
} 		

void CFigure::Frame_Graph2D(CDC* pDC)
{
#define TICK_Y 6
#define Tick 10

	int nSaveDC=pDC->SaveDC();
	int labelPosX,labelPosY;
	int firstXlabel=1, firstYlabel=1;

	if(0){ //Draw the frame and the grid of the pannel
		int i, sw=1;
		double  stepX,stepY;
		double L,Lo;
		char charVarTemp[100];

		pDC->SelectObject(&m_BlackPen3); 
		pDC->MoveTo((int)m_ClientViewTopX+5,(int)m_ClientViewBottomY);pDC->LineTo((int)m_ClientViewBottomX,(int)m_ClientViewBottomY);
		pDC->MoveTo((int)m_ClientViewBottomX,(int)m_ClientViewTopY+5);pDC->LineTo((int)m_ClientViewBottomX,(int)m_ClientViewBottomY);
		pDC->SelectObject(&m_GrayPen); 
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle((int)m_ClientViewTopX, (int)m_ClientViewTopY, (int)m_ClientViewBottomX,(int)m_ClientViewBottomY);
		
		if(m_howManyY<0){ //Default setting (10 horizontal lines)
			#define Defalut10 10.
			stepY=(m_dSpanY)/Defalut10;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
			pDC->SelectObject(&m_LightGrayPen);
			//----------------------------------Horizontal lines
			for(i=0,L=m_dMinY;L<m_dMaxY+stepY/Defalut10;L+= stepY, i++){
				Lo=lY(L);
				pDC->MoveTo((int)(m_ClientViewTopX+SHIFT_TO_X),(int)Lo);
				pDC->LineTo((int)(m_ClientViewBottomX+SHIFT_TO_X+1),(int)Lo);
				if(i%5==0){
					if(fabs(L-(int)L)<0.01){
						sprintf(charVarTemp, "%.0lf ", L );
					}
					else sprintf(charVarTemp, "%.2lf ", L );	
					if(L>=0)labelPosX=(int)(m_ClientViewTopX+SHIFT_TO_X); 
					else    labelPosX=(int)(m_ClientViewTopX+1); 
					labelPosY=(int)Lo-5;
					pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
					if(firstYlabel==1){//Register the position of the label. It will be used when the user clicks this area to change the scale of the axis.
						m_FirstYlabel_left=labelPosX; m_FirstYlabel_right=m_FirstYlabel_left+m_FontWidth*strlen(charVarTemp); m_FirstYlabel_top=labelPosY; m_FirstYlabel_bottom=m_FirstYlabel_top+m_FontHeight;
						firstYlabel=0;
					}
					if(i==(int)(Defalut10)){
						m_LastYlabel_left=labelPosX; m_LastYlabel_right=m_LastYlabel_left+m_FontWidth*strlen(charVarTemp); m_LastYlabel_top=labelPosY; m_LastYlabel_bottom=m_LastYlabel_top+m_FontHeight;
					}
				}
			}
		}
		else{ //User defined horizontal lines & ticks
			pDC->SelectObject(&m_LightGrayPen);
			for(i=0;i<m_howManyY;i++){
				if(m_YTics[i]>m_dMaxY)continue;
				Lo=lY(m_YTics[i]);
				//Ticks
				//pDC->MoveTo((int)(m_ClientViewTopX+SHIFT_TO_X-m_LateralMargin),(int)Lo); 
				//pDC->LineTo((int)(m_ClientViewTopX+Tick+SHIFT_TO_X-m_LateralMargin),(int)Lo);
				//Guidelines
				pDC->MoveTo((int)(m_ClientViewTopX+SHIFT_TO_X),(int)Lo);
				pDC->LineTo((int)(m_ClientViewBottomX+SHIFT_TO_X+1),(int)Lo);
				if(m_IsLabeledTicY[i]){
					if(m_YTics[i]==0||fabs(m_YTics[i]/(int)(m_YTics[i])-1)<0.001){//The value is either 0, or almost the same (0.1% difference) as an integer
						sprintf(charVarTemp, "%.0lf ", m_YTics[i] ); 
					}
					else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
					labelPosX=(int)(m_ClientViewTopX+1); 
					labelPosY=(int)Lo-5;
					pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
					if(firstYlabel==1){//Register the position of the label. It will be used when the user clicks this area to change the scale of the axis.
						m_FirstYlabel_left=labelPosX; m_FirstYlabel_right=m_FirstYlabel_left+m_FontWidth*strlen(charVarTemp); m_FirstYlabel_top=labelPosY; m_FirstYlabel_bottom=m_FirstYlabel_top+m_FontHeight;
						firstYlabel=0;
					}
					if(i==m_howManyY-1){
						m_LastYlabel_left=labelPosX; m_LastYlabel_right=m_LastYlabel_left+m_FontWidth*strlen(charVarTemp); m_LastYlabel_top=labelPosY; m_LastYlabel_bottom=m_LastYlabel_top+m_FontHeight;
					}
				}
			}
		}
		if(m_howManyX<0){ //Default setting (10 vertical lines)
			stepX=m_dSpanX/Defalut10;//Get_Var("Input interval :");
			//-------------------------Vertical lines
			for(i=0,L=m_dMinX;L<m_dMaxX+stepX/Defalut10;L+=stepX,i++){
				Lo=lX(L);
				pDC->MoveTo((int)Lo,(int)(m_ClientViewTopY));
				pDC->LineTo((int)Lo,(int)(m_ClientViewBottomY));
				if(sw==0){ sw=1; continue; }; sw=0;//print when even turn
				if(i%2==0){
					if(L*m_dLabelScale+m_dLabelBias==(int)(L*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", L*m_dLabelScale+m_dLabelBias );
					else sprintf(charVarTemp, "%.2lf", L*m_dLabelScale+m_dLabelBias );	
					//if(L==(int)L)sprintf(charVarTemp, "%.0lf ", L );
					//else sprintf(charVarTemp, "%.2lf ", L );	
					if(L>=10.)Lo-=5.;
					labelPosX=(int)(Lo)-3; labelPosY=(int)(m_ClientViewBottomY+1+15*(i%2));
					pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
					if(firstXlabel==1){//Register the position of the label. It will be used when the user clicks this area to change the scale of the axis.
						m_FirstXlabel_left=labelPosX; m_FirstXlabel_right=m_FirstXlabel_left+m_FontWidth*strlen(charVarTemp); m_FirstXlabel_top=labelPosY; m_FirstXlabel_bottom=m_FirstXlabel_top+m_FontHeight;
						firstXlabel=0;
					}
					if(i==(int)(Defalut10)){
						m_LastXlabel_left=labelPosX; m_LastXlabel_right=m_LastXlabel_left+m_FontWidth*strlen(charVarTemp); m_LastXlabel_top=labelPosY; m_LastXlabel_bottom=m_LastXlabel_top+m_FontHeight;
					}
				}
			}
		}
		else{ //User defined vertical lines & ticks
			//-------------------------X axis
			for(i=0;i<m_howManyX;i++){
				if(m_XTics[i]>m_dMaxX)continue;
				Lo=lX(m_XTics[i]);
				//Ticks
				//pDC->MoveTo((int)Lo,(int)(m_ClientViewBottomY));	
				//pDC->LineTo((int)Lo,(int)(m_ClientViewBottomY-Tick));
				//Guidelines
				pDC->MoveTo((int)Lo,(int)(m_ClientViewTopY));
				pDC->LineTo((int)Lo,(int)(m_ClientViewBottomY));		
				if(m_IsLabeledTicX[i]){
					if(m_XTics[i]==0||fabs(m_XTics[i]/(int)(m_XTics[i])-1)<0.001){//The value is either 0, or almost the same (0.1% difference) as an integer
						sprintf(charVarTemp, "%.0lf ", m_XTics[i]*m_dLabelScale+m_dLabelBias ); 
					}
					else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
					labelPosX=(int)(Lo)-3; labelPosY=(int)(m_ClientViewBottomY+1);
					pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
					if(firstXlabel==1){//Register the position of the label. It will be used when the user clicks this area to change the scale of the axis.
						m_FirstXlabel_left=labelPosX; m_FirstXlabel_right=m_FirstXlabel_left+m_FontWidth*strlen(charVarTemp); m_FirstXlabel_top=labelPosY; m_FirstXlabel_bottom=m_FirstXlabel_top+m_FontHeight;
						firstXlabel=0;
					}
					if(i==m_howManyX-1){
						m_LastXlabel_left=labelPosX; m_LastXlabel_right=m_LastXlabel_left+m_FontWidth*strlen(charVarTemp); m_LastXlabel_top=labelPosY; m_LastXlabel_bottom=m_LastXlabel_top+m_FontHeight;
					}
				}
			}
		}
		//---------- Names of X,Y axes
		pDC->TextOut((int)(m_ClientViewTopX+SHIFT_TO_X+45),(int)(m_ClientViewBottomY+15),m_cXaxis[m_page]);//Horizontal coordinate Axis Name
		m_nYlabelPosX=(int)(m_ClientViewTopX+SHIFT_TO_X-30);m_nYlabelPosY=(int)(m_ClientViewTopY-20);
		pDC->TextOut((int)(m_ClientViewTopX+SHIFT_TO_X-30),(int)(m_ClientViewTopY-20),m_cYaxis[m_page]);//Vertical coordinate Axis Name
		//---------- Bold "X=0" line
		if(0>=m_dMinX && 0<=m_dMaxX){
			pDC->SelectObject(&m_BlackPen2); 
			Lo=lX(0);
			pDC->MoveTo((int)Lo,(int)lY(m_dMinY));
			pDC->LineTo((int)Lo,(int)lY(m_dMaxY));
		}
		//----------- Bold "Y=0" line
		if(0>=m_dMinY && 0<=m_dMaxY){
			pDC->SelectObject(&m_BlackPen2); 
			Lo=lY(0);
			pDC->MoveTo((int)lX(m_dMinX),(int)Lo);
			pDC->LineTo((int)lX(m_dMaxX),(int)Lo);
		}
		//----------- Draw the buttons
		//pDC->TextOut((int)(m_ClientViewBottomX-m_FontWidth*3.5),(int)(m_ClientViewTopY+m_FontHeight*0.5),"+");//Blow-up button
		//pDC->TextOut((int)(m_ClientViewBottomX-m_FontWidth*2),(int)(m_ClientViewTopY+m_FontHeight*0.4),"X");//Kill button
		//----------- The NAME of the figure
		pDC->TextOut(m_TitlePosX_left,m_TitlePosX_top,m_Title);//Blow-up button
	}
	else{//Draw an EMPTY pannel.
		pDC->Rectangle((int)m_ClientViewTopX-1, (int)m_ClientViewTopY-1, (int)m_ClientViewBottomX+1,(int)m_ClientViewBottomY+1);
	}

	pDC->RestoreDC(nSaveDC); 
} 		


#define periRATIO 0.25
#define periGAP 10
void CFigure::formal_Frame2D(CDC* pDC, int IsPeriHist)
{
//#define Tick 5
#define Tick 10
	int nSaveDC=pDC->SaveDC();
	int i;
	double  stepX,stepY, L,Lo;
	char charVarTemp[100];

	pDC->SelectObject(&m_BluePen); 
	pDC->SelectObject(m_GrayBrush);
	if(IsPeriHist)pDC->Rectangle(lX(m_dMinX)-200,lY(m_dMaxY)-200,lX(m_dMaxX)+200,lY(m_dMinY)+200);
	else pDC->Rectangle(lX(m_dMinX)-50,lY(m_dMaxY)-50,lX(m_dMaxX)+50,lY(m_dMinY)+50);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->SelectObject(&m_BlackPen3); 
	pDC->Rectangle(lX(m_dMinX)-m_LateralMargin,lY(m_dMaxY),lX(m_dMaxX)+m_LateralMargin,lY(m_dMinY));

	if(m_dStepX==0)stepX=m_dSpanX/10.;
	else stepX=m_dStepX;
	if(m_dStepY==0)stepY=m_dSpanY/4.;
	else stepY=m_dStepY;
	//-------------------------Y axis
	for(i=0;i<m_howManyY;i++){
		if(m_YTics[i]>m_dMaxY)continue;
		Lo=lY(m_YTics[i]);
		pDC->MoveTo((int)(m_ClientViewTopX+SHIFT_TO_X-m_LateralMargin),(int)Lo); pDC->LineTo((int)(m_ClientViewTopX+Tick+SHIFT_TO_X-m_LateralMargin),(int)Lo);
		if(m_IsLabeledTicY[i]){
			if(m_YTics[i]==(int)m_YTics[i])sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
			else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
			pDC->TextOut((int)(m_ClientViewTopX+SHIFT_TO_X-5), (int)Lo-5,charVarTemp);
		}
	}
	//-------------------------X axis
	for(i=0;i<m_howManyX;i++){
		if(m_XTics[i]>m_dMaxX)continue;
		Lo=lX(m_XTics[i]);
		pDC->MoveTo((int)Lo,(int)(m_ClientViewBottomY));	pDC->LineTo((int)Lo,(int)(m_ClientViewBottomY-Tick));
		if(m_IsLabeledTicX[i]){
			if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
			else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
			pDC->TextOut((int)(Lo)-3,(int)(m_ClientViewBottomY+1),charVarTemp);
		}
	}

	if(IsPeriHist==1){//peri-Histogram
		pDC->Rectangle(lX(m_dMinX),lY(m_dMaxY+periRATIO*(m_dMaxY-m_dMinY))-periGAP,lX(m_dMaxX),lY(m_dMaxY)-periGAP);//Vertical one
		pDC->Rectangle(lX(m_dMaxX)+periGAP,lY(m_dMaxY),lX(m_dMaxX+periRATIO*(m_dSpanX))+periGAP,lY(m_dMinY));//Horizontal one
	}
	pDC->RestoreDC(nSaveDC); 
}

//For the functions needing fast plot, do not use this function.
void CFigure::Graph2D(double X, double Y,int Disp, CDC* pDC)   
{  
   if(Disp>10&&Disp<16){	
		  pDC->Rectangle( lX(X)+1,lY(Y)-1,lX(X)+1+(Disp-7),	lY(Y)-1+(Disp-7));
		  	//m_bUsingDC=0;
			return;
   }
   switch(Disp){ // Plot the object with (dots,line,circle...)
/*	  case 1:
		 pDC->Rectangle(lX(prevX), lY(Y),lX(X),(int)(m_ClientViewBottomY));
		 break;
*/	  case 2:  //with Line
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 3:  //with Circle
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		 break;
	  case 33:  //with Circle
		 pDC->Ellipse(CRect(lX(X)-2,lY(Y)-2,lX(X)+7,lY(Y)+7) );
		 break;
	  case 4:
		 pDC->LineTo(lX(X),lY(Y));
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		 break;
	  case 5:
		pDC->SetPixel( lX(X),lY(Y),RGB(0,0,0));
		 break;
	  case 55:
		pDC->SetPixel( lX(X),lY(Y),RGB(0,0,0));
		 break;
	  case 56:
		pDC->SetPixel( lX(X),lY(Y),RGB(210,210,240));
		 break;
	  case 57: //  Color of IRIS
		//pDC->SetPixel( lX(X),lY(Y),RGB(19,63,17));
		//pDC->SetPixel( lX(X),lY(Y),RGB(91,150,32));
		pDC->SetPixel( lX(X),lY(Y),RGB(91,150,92));
		//pDC->SetPixel( lX(X),lY(Y),RGB(91,100,192));  //BLUE
		 break;
	  case 600:
		 pDC->MoveTo(lX(-40),lY(0));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 601:
		 pDC->MoveTo(lX(+40),lY(0));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 6:  //with Circle
		char strTemp[100];
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		sprintf(strTemp,"      %0.3lf", Y);     
		pDC->TextOut(lX(X)+5, lY(Y), strTemp);
		sprintf(strTemp,"x%d:", X+1);     
		pDC->TextOut(lX(X)+5,lY(Y), strTemp);
		 break;
	  case 7:  //with Circle
		char strTemp1[100];
		sprintf(strTemp1,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-350, lY(1-0.053*X-.21), strTemp1);
		sprintf(strTemp1,"x%d:", X+1);     
		pDC->TextOut(lX(0)-350,lY(1-0.053*X-.21), strTemp1);
		 break;
	  case 8:  //with Circle
		char strTemp2[100];
		sprintf(strTemp2,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X-.21), strTemp2);
		sprintf(strTemp2,"I%d:", X+1);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X-.21), strTemp2);
		 break;
	  case 9:   //Move the initial point to (x,y) 
		 pDC->MoveTo(lX(X),lY(Y));
		 break;
	  case 10:  //with 1x1 Rectangle  
		  pDC->Rectangle( lX(X),lY(Y),lX(X)+2,lY(Y)+2);
		 break;
	  case 12:  //with 1x1 Rectangle  
		  pDC->Rectangle( lX(X),lY(Y),lX(X)+4,lY(Y)+4);
		 break;
	  case 30: // Histogram
		  pDC->MoveTo(lX(X),lY(0));
		  pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 31: // Histogram
		  pDC->Rectangle( lX(X)-1,lY(0),lX(X)+1,lY(Y));
		 break;
	  case 32: // Histogram
		  pDC->Rectangle( lX(X)-2,lY(0),lX(X)+2,lY(Y));
		 break;
	  case 34: // Histogram (the width of the column starts at the X and extend to X+10)
		  pDC->Rectangle( lX(X),lY(0),lX(X)+10,lY(Y));
		 break;
	  case 60: // Histogram 2
		  pDC->MoveTo(lX(X),lY(m_dMeanY));
		  pDC->LineTo(lX(X),lY(Y+m_dMeanY));
		 break;
	  case 90: // Histogram 3
		  pDC->MoveTo(lX(X),lY(m_dMaxY));
		  pDC->LineTo(lX(X),lY(m_dMaxY-Y));
		 break;
	  case 91: // Histogram 3
		  pDC->MoveTo(lX(X),lY(m_dMinY));
		  pDC->LineTo(lX(X),lY(m_dMinY-Y*0.4));
		 break;

	  default:  break;
   }//End of Switch

}

void CFigure::peri_Histogram(double X, double Y, int BinWidth, CDC* pDC)
{
	pDC->Rectangle( lX(X),lY(m_dMaxY)-periGAP,lX(X+BinWidth)-4,lY(m_dMaxY+periRATIO*(m_dMaxY-m_dMinY)*Y)-periGAP);//This "-4" in lX(XBinWidth)-4 is to extend the bin width to the next bin level
}


void CFigure::Histogram4(double X, double Y, int BinWidth,int Nth, CDC* pDC)
{
	pDC->Rectangle( lX(X+Nth*(BinWidth*0.25)),lY(Y),lX(X+(Nth+1)*(BinWidth*0.25)),lY(m_dMinY));
}

void CFigure::Histogram4(double X, double Y, float BinWidth,int Nth, CDC* pDC)
{
	pDC->Rectangle( lX(X+Nth*(BinWidth*0.25)),lY(Y),lX(X+(Nth+1)*(BinWidth*0.25)),lY(m_dMinY));
}

void CFigure::peri_Hist_Horizontal(double X, double Y, int BinWidth, CDC* pDC)
{
	pDC->Rectangle( lX(m_dMaxX)+periGAP,lY(Y),lX(m_dMaxX+periRATIO*(m_dSpanX)*X)+periGAP,lY(Y+BinWidth)+4);//This "+4" in lY(Y+BinWidth)+4 is to extend the bin width to the next bin level
}


void CFigure::Circle(double X, double Y,int size_inPixel,CDC* pDC)
{
	 pDC->Ellipse(CRect(lX(X)-size_inPixel,lY(Y)-size_inPixel,lX(X)+size_inPixel,lY(Y)+size_inPixel) );
}

void CFigure::Triangle(double X, double Y,int size_inPixel,CDC* pDC) // 
{
#define sqroot3 1.73205
#define HalfSqRoot3 0.866025
	m_vert[0].x=lX(X);					m_vert[0].y=lY(Y)-size_inPixel; 
	m_vert[1].x=lX(X)-(int)(HalfSqRoot3*size_inPixel);	m_vert[1].y=lY(Y)+(int)(0.5*size_inPixel); 
	m_vert[2].x=lX(X)+(int)(HalfSqRoot3*size_inPixel);	m_vert[2].y=lY(Y)+(int)(0.5*size_inPixel); 

	Polygon(*pDC,m_vert,3);
}

void CFigure::Abs_Ellipse(double X, double Y,int size_inPixel, CDC* pDC)
{  
	 pDC->Ellipse(CRect(lX(X)-size_inPixel,lY(Y)-size_inPixel,lX(X)+size_inPixel,lY(Y)+size_inPixel) );
}

void CFigure::ErrorBar(double X, double Y, double error, CDC* pDC)
{  
	 pDC->MoveTo(lX(X),lY(Y+error)); pDC->LineTo(lX(X),lY(Y-error)); //Vertical var
	 pDC->MoveTo(lX(X)-5,lY(Y+error)); pDC->LineTo(lX(X)+5,lY(Y+error)); //Upper horizontal bar
	 pDC->MoveTo(lX(X)-5,lY(Y-error)); pDC->LineTo(lX(X)+5,lY(Y-error)); //Lower horizontal bar
}

void CFigure::ErrorWing(double X, double Y, double error, CDC* pDC)
{  
	 pDC->MoveTo(lX(X),lY(Y+error)); pDC->LineTo(lX(X),lY(Y-error)); //Vertical var
}


void CFigure::Graph2D(double X1, double Y1, double X, double Y,int Disp, CDC* pDC)//------------------|
{  //This function must be faster than others!!!
   switch(Disp){ // Plot the object with (dots,line,circle...)
/*	  case 1:
		 pDC->Rectangle(lX(prevX),lY(Y), lX(X),(int)(m_ClientViewBottomY));
		 break;
*/	  case 2:  //with Line
		 pDC->MoveTo(lX(X1),lY(Y1));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 3:  //with Circle
		 pDC->Ellipse(	CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3)	 );
		 break;
	  case 4:
		 pDC->LineTo(lX(X),lY(Y));
		 pDC->Ellipse(	CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3)	 );
		 break;
	  case 5:
		pDC->SetPixel( lX(X),lY(Y),RGB(200,0,0));
		 break;
	  case 6:  //with Circle
		char strTemp[100];
		 pDC->Ellipse(CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3) );
		sprintf(strTemp,"      %0.3lf", Y);     
		pDC->TextOut(lX(X)+5,	 lY(Y), strTemp);
		sprintf(strTemp,"x%d:", X1+1);     
		pDC->TextOut(lX(X)+5,	 lY(Y), strTemp);
		 break;
	  case 7:  //with Circle
		char strTemp1[100];
		sprintf(strTemp1,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-350, lY(1-0.053*X1-.21), strTemp1);
		sprintf(strTemp1,"x%d:", X1+1);     
		pDC->TextOut(lX(0)-350, lY(1-0.053*X1-.21), strTemp1);
		 break;
	  case 8:  //with Circle
		char strTemp2[100];
		sprintf(strTemp2,"       %0.3lf", Y);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X1-.21), strTemp2);
		sprintf(strTemp2,"I%d:", X1+1);     
		pDC->TextOut(lX(0)-560, lY(1-0.053*X1-.21), strTemp2);
		 break;
	  default:  break;
   }//End of Switch
}


void CFigure::PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"Number of Nns=%d", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}


void CFigure::PutVal_Graph2D(double X, double Y, double NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%0.3lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}

void CFigure::PutVal_p2(double X, double Y, double NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%0.2lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}


void CFigure::PutIntVal(double X, double Y, int NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%d", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);

}
void CFigure::PutText(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), ToPrint);
}

void CFigure::PutChar_After10(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)+10),(int)(lY(Y)-5), ToPrint);
}

void CFigure::RectangleAB(double X1, double Y1, double X2, double Y2,CDC* pDC)
{  
	pDC->Rectangle(lX(X1),lY(Y1),lX(X2),lY(Y2));
}

////This one draws a rectangle whose size is consistent with the viewing angle: i.e. as the ractangle of a size (in viewing degree) moves periphery, it becomes larger.
//void CFigure::FillSolidRect_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
//{  
//	int bottomRightX, bottomRightY;
//	topleftX=lX(cntX+0.5*width_inDegrees);
//	topleftY=lY(cntY+0.5*height_inDegrees);
//	bottomRightX=lX(cntX-0.5*width_inDegrees);
//	bottomRightY=lY(cntY-0.5*height_inDegrees);
//
//	width_inPixels=bottomRightX-topleftX;
//	height_inPixels=bottomRightY-topleftY;
//	pDC->FillSolidRect(topleftX,topleftY,width_inPixels,height_inPixels,	rgb);
//}
//
////This one reports a rectangle whose size is consistent with the viewing angle: i.e. as the ractangle of a size (in viewing degree) moves periphery, it becomes larger.
//void CFigure::Rect_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
//{  
//	int bottomRightX, bottomRightY;
//	topleftX=lX(cntX+0.5*width_inDegrees);
//	topleftY=lY(cntY+0.5*height_inDegrees);
//	bottomRightX=lX(cntX-0.5*width_inDegrees);
//	bottomRightY=lY(cntY-0.5*height_inDegrees);
//
//	width_inPixels=bottomRightX-topleftX;
//	height_inPixels=bottomRightY-topleftY;
//}

//This one draws a rectangle whose size is constant across the viewing angle: i.e. even when the ractangle of a size (in viewing degree at the fovia) moves periphery, its actual size remains constant.
void CFigure::FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lX(0.5*width_inDegrees )-m_lX0));
	height_inPixels=abs(2*(lY(0.5*height_inDegrees)-m_lY0));
	topleftX=lX(cntX)-0.5*width_inPixels;
	topleftY=lY(cntY)-0.5*height_inPixels;
	pDC->FillSolidRect(topleftX,topleftY,width_inPixels,height_inPixels,	rgb);
}
//This one reports a rectangle whose size is constant across the viewing angle: i.e. even when the ractangle of a size (in viewing degree at the fovia) moves periphery, its actual size remains constant.
void CFigure::Rect_SizeAroundFovia_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lX(0.5*width_inDegrees )-m_lX0));
	height_inPixels=abs(2*(lY(0.5*height_inDegrees)-m_lY0));
	topleftX=lX(cntX)-0.5*width_inPixels;
	topleftY=lY(cntY)-0.5*height_inPixels;
}

//This one draws a disk whose size is constant across the viewing angle: i.e. even when the ractangle of a size (in viewing degree at the fovia) moves periphery, its actual size remains constant.
void CFigure::Ellipse_SizeAroundFovia_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lX(0.5*width_inDegrees )-m_lX0));
	height_inPixels=abs(2*(lY(0.5*height_inDegrees)-m_lY0));
	topleftX=lX(cntX)-0.5*width_inPixels;
	topleftY=lY(cntY)-0.5*height_inPixels;

	
	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;
	LogBrh.lbColor=rgb;
	m_TmpBrush.CreateBrushIndirect( &LogBrh );	
	pDC->SelectObject(&m_TmpBrush);
	pDC->Ellipse(topleftX,topleftY,topleftX+width_inPixels,topleftY+height_inPixels);
	m_TmpBrush.DeleteObject();
}




void CFigure::FillSolidRect(int X, int Y,int width_inPixel, int height_inPixel, COLORREF rgb ,CDC* pDC)
{ 
	pDC->FillSolidRect(lX(X),lY(Y),width_inPixel,height_inPixel,	rgb);
}
void CFigure::FillSolidRect(double X, double Y,int width_inPixel, int height_inPixel, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),width_inPixel,height_inPixel,	rgb);
}
void CFigure::FillSolidRect_CenteredY(double X, double Y,int width_inPixel, int height_inPixel, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y)-height_inPixel/2,width_inPixel,height_inPixel,	rgb);
}

void CFigure::FillSolidRect13x13(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),13,	13,	rgb);
}

void CFigure::FillSolidRect8x8(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),8,	8,	rgb);
}

void CFigure::FillSolidRect7x7(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),7,	7,	rgb);
}

void CFigure::MySetPixel(double X, double Y, int GRAY, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),RGB(GRAY,GRAY,GRAY));
}

void CFigure::MySetPixelRED(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Red);
}

void CFigure::MySetPixelGREEN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Green);
}

void CFigure::MySetPixelDKGRN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_DkGreen);
}

void CFigure::MySetPixelBLUE(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Blue);
}

void CFigure::MySetPixelPINK(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Pink);
}

void CFigure::MySetPixelCYAN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Cyan);
}

void CFigure::MySetPixelYELLOW(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Yellow);
}

void CFigure::MySetPixelBROWN(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Brown);
}

void CFigure::MySetPixelVIOLET(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Violet);
}
void CFigure::MySetPixelBLACK(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_BLACK);
}
void CFigure::MySetBox(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_BLACK);
	pDC->SetPixel( lX(X)+1,lY(Y),m_BLACK);
	pDC->SetPixel( lX(X),lY(Y)+1,m_BLACK);
	pDC->SetPixel( lX(X)+1,lY(Y)+1,m_BLACK);
}

void CFigure::MySetRedBox(double X, double Y, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),m_Red);
	pDC->SetPixel( lX(X)+1,lY(Y),m_Red);
	pDC->SetPixel( lX(X),lY(Y)+1,m_Red);
	pDC->SetPixel( lX(X)+1,lY(Y)+1,m_Red);
}

void CFigure::MySetPixelRGB(double X, double Y, int R, int G, int B, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),RGB(R,G,B));
}

void CFigure::MySetPixelRGB(int X, int Y, int R, int G, int B, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),RGB(R,G,B));
}


void CFigure::MySetPixelRGB(double X, double Y, COLORREF *rgb, CDC* pDC)
{	
	pDC->SetPixel( lX(X),lY(Y),*rgb);
}

void CFigure::FromDataToScreenXY(double X, double Y, int &x, int &y)
{
	x=       (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=       (lY(Y)); //They are in (int) form because it is in screen pixel unit
}
void CFigure::FromDataToScreenXY(double X, double Y, float &x, float &y)
{
	x=   (float) (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=   (float) (lY(Y)); //They are in (int) form because it is in screen pixel unit
}

int CFigure::FromDataToScreenX(double screenX)
{
	return  (lX(screenX)); //They are in (int) form because it is in screen pixel unit
}
double CFigure::FromScreenToDataX(int screenX)
{
	return m_dMinX+(m_dSpanX)*(((double)(screenX-m_nPaneTx))/(double)(m_nPaneBx-m_nPaneTx));
}
double CFigure::FromScreenToDataY(int screenY)
{
	return m_dMinY+(m_dSpanY)*(((double)(m_nPaneBy-screenY))/(double)(m_nPaneBy-m_nPaneTy));
}

void CFigure::ErasePane_2D( CDC* pDC)  
{
}

void CFigure::RepaintPane_2D( CDC* pDC)  
{
}

void CFigure::RefreshPane( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(
		(int)(m_ClientViewTopX+SHIFT_TO_X),
		(int)(m_ClientViewTopY),
		(int)((m_ClientViewBottomX+SHIFT_TO_X+1)-(m_ClientViewTopX+SHIFT_TO_X)),
		(int)((m_ClientViewBottomY)-(m_ClientViewTopY)+1),
		pm_memDC,(int)(m_ClientViewTopX+SHIFT_TO_X),(int)(m_ClientViewTopY),SRCCOPY);
}

void CFigure::RefreshFromXandSome(double X, CDC* pObjDC, CDC* pMemDC)
{
	pObjDC->BitBlt(
		lX(X),m_nPaneTy,3,m_nPaneBy-m_nPaneTy,
		pMemDC,lX(X),m_nPaneTy,SRCCOPY);
}

void CFigure::RefreshFrame( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_ClientViewTopX,m_ClientViewTopY,m_ClientViewBottomX-m_ClientViewTopX,m_ClientViewBottomY-m_ClientViewTopY,	pm_memDC,m_ClientViewTopX,m_ClientViewTopY,SRCCOPY);
}

void CFigure::EraseFrameBlack(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(BLACK_BRUSH);
	pm_memDC->Rectangle(m_ClientViewTopX,m_ClientViewTopY,m_ClientViewBottomX,m_ClientViewBottomY);
	pm_memDC->RestoreDC(nSaveDC); 
}
void CFigure::EraseFrameWHITE(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(WHITE_BRUSH);
	pm_memDC->Rectangle(m_ClientViewTopX,m_ClientViewTopY,m_ClientViewBottomX,m_ClientViewBottomY);
	pm_memDC->RestoreDC(nSaveDC); 
}

#undef CLIENT_VIEW_SIZE_X
#undef Dy



