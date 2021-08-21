#include "stdafx.h"
#include <math.h>
#include "Figure_DAS.h"

BOOL CFigure::m_bUsingDC=0;
BOOL CFigure::m_bUsingDC2=0;
BOOL CFigure::m_bUsingDC3=0;

CPen CFigure::m_CYAN1,CFigure::m_VIOLETLIGHT1,CFigure::m_VIOLET3,CFigure::m_OrangePen,CFigure::m_RedPen,CFigure::m_BluePen;
CPen CFigure::m_BluePen2,CFigure::m_BLUE3,CFigure::m_GreenPen,CFigure::m_BlackPen2,CFigure::m_BlackPen3,CFigure::m_GrayPen,CFigure::m_LightGrayPen,CFigure::m_Null_Pen;
CBrush CFigure::m_bkBrush,CFigure::m_GrayBrush,CFigure::m_LightGrayBrush;
int	CFigure::m_DiviceContaxtCreatedAlready=0;

#define Dx         ((m_nBx)-(m_nTx))
#define Dy         ((m_nBy)-(m_nTy))
#define DoubleGapX                    ((m_dGapX)+(m_dGapX))
#define DoubleGapY                    ((m_dGapY)+(m_dGapY))
#define Dy_DoubleGapY                 (Dy-DoubleGapY)
#define Dx_DoubleGapX                 (Dx-DoubleGapX)
#define SHIFT_TO_X 2

//#define lX(X) (int)((X-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_nTranslateX) //They are in (int) form because it is in screen pixel unit
//#define lY(Y) (int)(m_nBy_GapY-(Y-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY)//They are in (int) form because it is in screen pixel unit
//#define lY2(Y) (int)(m_nBy_GapY-(Y-m_dMinY2)*m_dDy_DoubleGapY_Per_Max_MinY2)//They are in (int) form because it is in screen pixel unit

#define lX(X)  (int)(( X)*m_dDx_DoubleGapX_Per_Max_MinX +m_TransformX )//They are in (int) form because it is in screen pixel unit
#define lY(Y)  (int)((-Y)*m_dDy_DoubleGapY_Per_Max_MinY +m_TransformY )//They are in (int) form because it is in screen pixel unit
#define lY2(Y) (int)((-Y)*m_dDy_DoubleGapY_Per_Max_MinY2+m_TransformY2)//They are in (int) form because it is in screen pixel unit

CFigure::CFigure()
{
	m_cnvrt=1000./DATA_SAMPLING_RATE;
	pChan=NULL;
	m_ChanID=-1;
	m_ScaleY=0;
	m_IsScaleHeld=0;
	/////////////////////////
	
	m_IsBlownUp=0; m_IsTickY=0;
	m_nTx=500; m_nTy=0; m_nBx=1000; m_nBy=500;
	m_BlownUpTx=200;m_BlownUpTy=5; m_BlownUpBx=820; m_BlownUpBy=605;
	m_dGapX=(Dx)/8.; m_dGapY=-1; // m_dGapY has been set to -1 to warn the user to set the position of the figure first.
	m_nBy_GapY=m_nBy-m_dGapY;
	m_dStepX=0;m_dStepY=0;
	m_LateralMargin=0;
	m_howManyX=-1;m_howManyY=-1;
	m_page=0; m_NumPages=1;
	m_FontWidth=5;	m_FontHeight=-1;// m_FontHeight is set to -1 to warn the user to set it first.
	m_Captured=0; m_Deleted=0;
	m_AssociatedChan.NameString="NoChannel";
	m_toDisplayThreshold=0; m_Threshold=-0.1; m_prvThreshold_onScrn=-0.1;
	if(m_DiviceContaxtCreatedAlready==0){
		m_DiviceContaxtCreatedAlready=1;
		m_VIOLET3.CreatePen(PS_SOLID, 3,RGB(200,90,250));
		m_OrangePen.CreatePen(PS_SOLID, 1,RGB(255,190,50));
		m_RedPen.CreatePen(PS_SOLID, 1,RGB(255,50,50));
		m_BluePen.CreatePen(PS_SOLID, 1,RGB(0,0,255));
		m_BluePen2.CreatePen(PS_SOLID, 2,RGB(0,0,255));
		m_GreenPen.CreatePen(PS_SOLID, 1,RGB(0,200,0));
		m_CYAN1.CreatePen(PS_SOLID, 1,RGB(0,255,255));
		m_VIOLETLIGHT1.CreatePen(PS_SOLID, 1,RGB(205,5,255));
		m_BlackPen2.CreatePen(PS_SOLID, 2,RGB(0,0,0));
		m_BlackPen3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
		m_Null_Pen.CreatePen(PS_NULL, 1,RGB(0,0,0));
		m_LightGrayPen.CreatePen(PS_SOLID, 1,RGB(230,230,230));
		m_GrayPen.CreatePen(PS_SOLID, 1,RGB(170,170,170));
		m_BLUE3.CreatePen(PS_SOLID, 3,RGB(100,100,255));
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

	m_dMinX=0., m_dMaxX=1000., m_dMinY=-1, m_dMaxY=1.; m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=-1;// m_dSpanY has been set to -1 to warn the user to set the "scales" of X, Y axis first
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
	m_dMinY2=-9; m_dMaxY2=9; m_dSpanY2=m_dMaxY2-m_dMinY2;
	m_dDy_DoubleGapY_Per_Max_MinY2 =   ((Dy-DoubleGapY)/(m_dSpanY2));
	m_dTypedInMinBaseY=m_dMinY; m_dTypedInMaxBaseY=m_dMaxY; m_dTypedInHalfBaseY=(m_dTypedInMaxBaseY+m_dTypedInMinBaseY)/2; 
	m_dLabelBias=0;m_dLabelScale=1;
	m_Red=RGB(255,0,0); m_Blue=RGB(100,100,255); m_Green=RGB(0,255,0); m_DkGreen=RGB(0,155,0); m_Pink=RGB(255,150,150); 
	m_Cyan=RGB(0,255,255); m_Yellow=RGB(240,240,0); m_Brown=RGB(240,130,0); m_Violet=RGB(200,90,250);m_BLACK=RGB(0,0,0);
}

CFigure::~CFigure()
{
	m_CYAN1.DeleteObject();m_VIOLETLIGHT1.DeleteObject();m_VIOLET3.DeleteObject();m_OrangePen.DeleteObject();m_RedPen.DeleteObject();
	m_BluePen.DeleteObject();m_BluePen2.DeleteObject();m_BLUE3.DeleteObject();m_GreenPen.DeleteObject();m_BlackPen2.DeleteObject();
	m_BlackPen3.DeleteObject();m_GrayPen.DeleteObject();m_LightGrayPen.DeleteObject();m_Null_Pen.DeleteObject();
	m_bkBrush.DeleteObject();m_GrayBrush.DeleteObject();m_LightGrayBrush.DeleteObject();
}

void CFigure::DispOneWaveform(double *pWave, int DataPointsPerWave, int Ecode, double biasToThreshold, CDC *pDC)
{
	int dataCnt;
	double wf, bias=m_cnvrt*biasToThreshold+m_dMinX;

	if(Ecode==-1)pDC->SelectStockObject(WHITE_PEN);
	else if(Ecode==0)pDC->SelectObject(m_VIOLETLIGHT1);
	else if(Ecode==1)pDC->SelectObject(m_GreenPen);
	else if(Ecode==2)pDC->SelectObject(m_CYAN1);
	else 	AfxMessageBox("pChan->M_WvEcode[]=? 4");

	wf=*pWave; 
	if(wf>m_dMaxY)wf=m_dMaxY; else if(wf<m_dMinY)wf=m_dMinY;
	pDC->MoveTo(lX(m_dMinX) ,lY(wf));
	for(dataCnt=1;dataCnt<DataPointsPerWave;dataCnt++){
		wf=*(pWave+dataCnt); if(wf>m_dMaxY){wf=m_dMaxY;} else if(wf<m_dMinY)wf=m_dMinY;
		pDC->LineTo(lX(m_cnvrt*dataCnt+bias),lY(wf));
	}	
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
	if((Min_X>=Max_X)||(Min_Y>=Max_Y)){
		AfxMessageBox("(Min_X>=Max_X)||(Min_Y>=Max_Y) ==> Try again", MB_OK); return;
	}
	m_dMinX=Min_X, m_dMaxX=Max_X; m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; 
	ComputeMapping_X_Y_Y2();
}
void CFigure::SetMinMaxY(double Min_Y, double Max_Y)
{
	if(Min_Y>=Max_Y){AfxMessageBox("(Min_Y>=Max_Y) ==> Try again", MB_OK); return;}
	m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; 
	ComputeMapping_X_Y_Y2();
}

void CFigure::RescaleAxes(double m_dMinimumX, double m_dMaximumX, double m_dMinimumY, double m_dMaximumY)
{
	//if(m_dMinimumX>=m_dMaximumX||m_dMinimumY >=m_dMaximumY){AfxMessageBox("Wrong parameters."); return;}==>This is taken care of at the input stage.
	double relY, relX;
	int recalibrateBoxes=0;
	if(m_dMinimumX!=m_dMinX || m_dMaximumX!=m_dMaxX)recalibrateBoxes=1; 
	relY=((double)(m_Threshold-m_dMinY))/(m_dMaxY-m_dMinY);// This line and the 2 lines below together, corrects the position of the threshold after the change of the scale fo the axes.
	SetMinMaxXY(m_dMinimumX,m_dMaximumX,m_dMinimumY,m_dMaximumY);//Register the change of the scales
	RegisterMinMaxYBase(); //Reflect the change of Y-scale by the user.//This is the base of the Y scale,  which the gain change by the handle will be based upon.
	m_Threshold=m_dMinY+relY*(m_dMaxY-m_dMinY);//The threshold maintains the position of the screen.
	if(pChan!=NULL)*(pChan->M_Threshold)=m_Threshold;
	if(pChan!=NULL){
		if(recalibrateBoxes==1 ){//The scale of X axis has been changed. Reflect this for the online "line-crossing" test by the server
			pChan->m_preWaveSpanDataSize =TIME_TO_DATA_POINTS(m_dMinX);
			pChan->m_postWaveSpanDataSize=TIME_TO_DATA_POINTS(m_dMaxX);
			pChan->m_DataPointsPerWave=pChan->m_postWaveSpanDataSize+pChan->m_preWaveSpanDataSize;
		}
		pChan->Remeasure_Box_XY_Positions_ScreenAnchored();//Fixing the position of the box on the screen, recalculate the data points of the box rectangle.
		pChan->UpdateServerWithVTBoxes();//This informs the server the changes
	}

}

void CFigure::SetFig_Position_in_ClientView(int Tx, int Ty, int Bx, int By)
{
	m_nTx=Tx; m_nTy=Ty; m_nBx=Bx; m_nBy=By;
}

//This is the base of the Y scale,  which the gain change by the handle will be based upon.
void CFigure::RegisterMinMaxYBase()
{
	m_dTypedInMinBaseY=m_dMinY; 
	m_dTypedInMaxBaseY=m_dMaxY; 
	m_dTypedInHalfBaseY=(m_dTypedInMaxBaseY+m_dTypedInMinBaseY)/2; 
	m_ScaleY=0;
}

void CFigure::SetFontSize(CFont *pFont)
{
	// Get the sizes of the font
	LOGFONT LogFont;
	pFont->GetLogFont(&LogFont);
	m_FontWidth=LogFont.lfWidth;
	m_FontHeight=LogFont.lfHeight;
}

void CFigure::ComputeMapping_X_Y_Y2()
{
	if(m_FontHeight<=0){
		AfxMessageBox("It is a good idea to set the size of font", MB_OK); m_FontHeight=10;
	}

	m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY;
	//Set the size of the Gap between the graph pane and the edge of the frame: width of the scale legend area.
	m_dGapX=m_FontWidth*2.5;
	m_dGapY=m_FontHeight*1.5;
	m_nBy_GapY=m_nBy-m_dGapY;

	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
	m_nTranslateX=(int)(m_dGapX+m_nTx+SHIFT_TO_X);
	m_TransformX=-m_dMinX*m_dDx_DoubleGapX_Per_Max_MinX+m_nTranslateX;
	m_TransformY=m_nBy_GapY+m_dMinY*m_dDy_DoubleGapY_Per_Max_MinY;

	//Y2
	m_dSpanY2=m_dMaxY2-m_dMinY2;
	m_dDy_DoubleGapY_Per_Max_MinY2 =   ((Dy-DoubleGapY)/(m_dSpanY2));
	m_TransformY2=m_nBy_GapY+m_dMinY2*m_dDy_DoubleGapY_Per_Max_MinY2;

	//Title of the figure
	RectOfLabel(0.5, m_AssociatedChan.NameString.GetLength(), m_AssociatedChan.StringRect);
	FromDataToScreenY(m_Threshold, m_prvThreshold_onScrn);//m_prvThreshold_onScrn needs to reflect the current threshold. 
	//FromDataToScreenXY(m_dMinX, m_dMaxY, m_nPaneTx, m_nPaneTy); //It maps a relative data to a point on the screen.	
	//FromDataToScreenXY(m_dMaxX, m_dMinY, m_nPaneBx, m_nPaneBy); //It maps a relative data to a point on the screen.
}

void CFigure::RectOfLabel(float relX, int LengthOfLabel, CRect &rect)
{
	rect.left   =(int)lX(m_dMinX+m_dSpanX*relX)-m_FontWidth*LengthOfLabel*0.5;
	rect.right  =(int)lX(m_dMinX+m_dSpanX*relX)+m_FontWidth*LengthOfLabel*0.5;
	rect.top    =(int)(m_nTy+m_FontHeight*0.5); 
	rect.bottom = rect.top+m_FontHeight;
}

void CFigure::DispNameOfFigure(CDC* pDC)
{
	pDC->TextOut(m_AssociatedChan.StringRect.left,m_AssociatedChan.StringRect.top,m_AssociatedChan.NameString);
} 		
void CFigure::RefreshNameBox(CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_AssociatedChan.StringRect.left,m_AssociatedChan.StringRect.top,m_AssociatedChan.StringRect.right-m_AssociatedChan.StringRect.left,m_AssociatedChan.StringRect.bottom-m_AssociatedChan.StringRect.top,	pm_memDC,m_AssociatedChan.StringRect.left,m_AssociatedChan.StringRect.top,SRCCOPY);
} 	

void CFigure::Disp_Y_Scale(CDC* pDC)
{

	int nSaveDC=pDC->SaveDC();
	int labelPosX,labelPosY;

	int i;
	double stepY;
	double L,Lo;
	char charVarTemp[100];

		
	#define Defalut10 10.
	stepY=(m_dSpanY)/Defalut10;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
	for(i=0,L=m_dMinY;L<m_dMaxY+stepY/Defalut10;L+= stepY, i++){
		Lo=lY(L);
		if(i%5==0){
			if(fabs(L-(int)L)<0.01){
				sprintf(charVarTemp, "%.0lf ", L );
			}
			else sprintf(charVarTemp, "%.1lf ", L );	
			if(L>=0)labelPosX=(int)(m_nTx+SHIFT_TO_X); 
			else    labelPosX=(int)(m_nTx+1); 
			labelPosY=(int)Lo-5;
			pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
		}
	}
	pDC->RestoreDC(nSaveDC); 
} 		

void CFigure::Frame_Graph2D(CDC* pDC,int oxFRAME)
{
#define TICK_Y 6
#define Tick 10

	if(m_dGapY<=0){AfxMessageBox("Run ComputeMapping_X_Y_Y2() first.", MB_OK); exit(0);}
	int nSaveDC=pDC->SaveDC();
	int labelPosX,labelPosY;
	int firstXlabel=1, firstYlabel=1;
	 m_bDrawFRAME=oxFRAME;

	if(m_bDrawFRAME==1){ //Draw the frame and the grid of the pannel
		int i, sw=1;
		double  stepX,stepY;
		double L,Lo;
		char charVarTemp[100];

		pDC->SelectObject(&m_BlackPen3); 
		pDC->MoveTo((int)m_nTx+5,(int)m_nBy);pDC->LineTo((int)m_nBx,(int)m_nBy);//Shadow
		pDC->MoveTo((int)m_nBx,(int)m_nTy+5);pDC->LineTo((int)m_nBx,(int)m_nBy);//Shadow
		pDC->SelectObject(&m_GrayPen); 
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle((int)m_nTx, (int)m_nTy, (int)m_nBx,(int)m_nBy);
		ErasePaneBlack(pDC);
		
		if(m_howManyY<0){ //Default setting (10 horizontal lines)
			#define Defalut10 10.
			stepY=(m_dSpanY)/Defalut10;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
			pDC->SelectObject(&m_LightGrayPen);
			//----------------------------------Horizontal lines
			for(i=0,L=m_dMinY;L<m_dMaxY+stepY/Defalut10;L+= stepY, i++){
				Lo=lY(L);
				//pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
				//pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
				if(i%5==0){
					if(fabs(L-(int)L)<0.01){
						sprintf(charVarTemp, "%.0lf ", L );
					}
					else sprintf(charVarTemp, "%.1lf ", L );	
					if(L>=0)labelPosX=(int)(m_nTx+SHIFT_TO_X); 
					else    labelPosX=(int)(m_nTx+1); 
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
				//pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo); 
				//pDC->LineTo((int)(m_nTx+Tick+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo);
				//Guidelines
				//pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
				//pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
				if(m_IsLabeledTicY[i]){
					if(fabs(m_YTics[i]-(int)m_YTics[i])<0.01)sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
					else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
					labelPosX=(int)(m_nTx+1); 
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
				//pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
				//pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
				if(sw==0){ sw=1; continue; }; sw=0;//print when even turn
				if(i%2==0){
					if(L*m_dLabelScale+m_dLabelBias==(int)(L*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", L*m_dLabelScale+m_dLabelBias );
					else sprintf(charVarTemp, "%.1lf", L*m_dLabelScale+m_dLabelBias );	
					//if(L==(int)L)sprintf(charVarTemp, "%.0lf ", L );
					//else sprintf(charVarTemp, "%.2lf ", L );	
					if(L>=10.)Lo-=5.;
					labelPosX=(int)(Lo)-3; labelPosY=(int)(m_nBy-m_dGapY+1+15*(i%2));
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
				//pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	
				//pDC->LineTo((int)Lo,(int)(m_nBy-Tick-m_dGapY));
				//Guidelines
				//pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
				//pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));		
				if(m_IsLabeledTicX[i]){
					if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
					else sprintf(charVarTemp, "%.1lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
					labelPosX=(int)(Lo)-3; labelPosY=(int)(m_nBy-m_dGapY+1);
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
		pDC->TextOut((int)(m_nTx+m_dGapX+SHIFT_TO_X+45),(int)(m_nBy-m_dGapY+15),m_cXaxis[m_page]);//Horizontal coordinate Axis Name
		m_nYlabelPosX=(int)(m_nTx+m_dGapX+SHIFT_TO_X-30);m_nYlabelPosY=(int)(m_nTy+m_dGapY-20);
		pDC->TextOut((int)(m_nTx+m_dGapX+SHIFT_TO_X-30),(int)(m_nTy+m_dGapY-20),m_cYaxis[m_page]);//Vertical coordinate Axis Name
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
			pDC->MoveTo(lX(m_dMinX),(int)Lo);
			pDC->LineTo(lX(m_dMaxX),(int)Lo);
		}
		//----------- Draw the buttons
		//pDC->TextOut((int)(m_nBx-m_FontWidth*3.5),(int)(m_nTy+m_FontHeight*0.5),"+");//Blow-up button
		//pDC->TextOut((int)(m_nBx-m_FontWidth*2),(int)(m_nTy+m_FontHeight*0.4),"X");//Kill button
		//----------- The NAME of the figure
		pDC->TextOut(m_AssociatedChan.StringRect.left,m_AssociatedChan.StringRect.top,m_AssociatedChan.NameString);
	}
	else{//Draw an EMPTY pannel.
		AfxMessageBox("This shouldn't happen");
	}
	//The threshold
	if(m_toDisplayThreshold==1){
		pDC->SelectObject(m_BLUE3);
		pDC->MoveTo(lX(m_dMinX)-2*m_FontWidth,lY(m_Threshold));
		pDC->LineTo(lX(m_dMaxX)-2,lY(m_Threshold));
		//m_prvThreshold_onScrn=lY(m_Threshold);
		m_prvThreshold_onScrn=(m_nBy_GapY-(m_Threshold-m_dMinY)*(( (m_nBy)-(m_nTy)- ((m_dGapY)+(m_dGapY)))/(m_dSpanY)));//They are in (int) form because it is in screen pixel unit

	}
	//The Y Scale
	if(m_toDisplayThreshold==1){
		pDC->SelectObject(m_VIOLET3);
		pDC->MoveTo(lX(m_dMaxX)+2,lY2(m_ScaleY));
		pDC->LineTo(m_nBx-2,lY2(m_ScaleY));
		m_prvScaleY_onScreen=lY2(m_ScaleY);
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
	if(m_Deleted==1){pDC->SelectObject(&m_GrayPen);pDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);return;}
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
		pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo); pDC->LineTo((int)(m_nTx+Tick+m_dGapX+SHIFT_TO_X-m_LateralMargin),(int)Lo);
		if(m_IsLabeledTicY[i]){
			if(m_YTics[i]==(int)m_YTics[i])sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
			else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
			pDC->TextOut((int)(m_nTx+SHIFT_TO_X-5), (int)Lo-5,charVarTemp);
		}
	}
	//-------------------------X axis
	for(i=0;i<m_howManyX;i++){
		if(m_XTics[i]>m_dMaxX)continue;
		Lo=lX(m_XTics[i]);
		pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	pDC->LineTo((int)Lo,(int)(m_nBy-Tick-m_dGapY));
		if(m_IsLabeledTicX[i]){
			if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
			else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
			pDC->TextOut((int)(Lo)-3,(int)(m_nBy-m_dGapY+1),charVarTemp);
		}
	}

	if(IsPeriHist==1){//peri-Histogram
		pDC->Rectangle(lX(m_dMinX),lY(m_dMaxY+periRATIO*(m_dMaxY-m_dMinY))-periGAP,lX(m_dMaxX),lY(m_dMaxY)-periGAP);//Vertical one
		pDC->Rectangle(lX(m_dMaxX)+periGAP,lY(m_dMaxY),lX(m_dMaxX+periRATIO*(m_dSpanX))+periGAP,lY(m_dMinY));//Horizontal one
	}
	pDC->RestoreDC(nSaveDC); 
}
//<----------------------------------------------------------------------------------|	
													//Don't Change! this allignment=>| 
void CFigure::Graph2D(double X, double Y,int Disp, CDC* pDC)    //------------------|
{  //This function must be faster than others!!!
//	while(m_bUsingDC2){Sleep(1);}; 	m_bUsingDC=1;

   if(Disp>10&&Disp<16){	
		  pDC->Rectangle( lX(X)+1,lY(Y)-1,lX(X)+1+(Disp-7),	lY(Y)-1+(Disp-7));
		  	//m_bUsingDC=0;
			return;
   }
   switch(Disp){ // Plot the object with (dots,line,circle...)
/*	  case 1:
		 pDC->Rectangle(lX(prevX), lY(Y),lX(X),(int)(m_nBy-m_dGapY));
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

//	m_bUsingDC=0;
}
void CFigure::MyMoveTo(double X, double Y, CDC* pDC){   
	 pDC->MoveTo(lX(X),lY(Y));
}
void CFigure::MyLineTo(double X, double Y, CDC* pDC){   
	 pDC->LineTo(lX(X),lY(Y));
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


void CFigure::Circle(double X, double Y,int size,CDC* pDC)
{
	 pDC->Ellipse(CRect(lX(X)-size,lY(Y)-size,lX(X)+size,lY(Y)+size) );
}

void CFigure::Triangle(double X, double Y,int size,CDC* pDC) // 
{
#define sqroot3 1.73205
#define HalfSqRoot3 0.866025
	m_vert[0].x=lX(X);					m_vert[0].y=lY(Y)-size; 
	m_vert[1].x=lX(X)-(int)(HalfSqRoot3*size);	m_vert[1].y=lY(Y)+(int)(0.5*size); 
	m_vert[2].x=lX(X)+(int)(HalfSqRoot3*size);	m_vert[2].y=lY(Y)+(int)(0.5*size); 

	Polygon(*pDC,m_vert,3);
}

void CFigure::Abs_Ellipse(double X, double Y,int size, CDC* pDC)
{  
	 pDC->Ellipse(CRect(lX(X)-size,lY(Y)-size,lX(X)+size,lY(Y)+size) );
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

//	while(m_bUsingDC2){Sleep(1); }
//	m_bUsingDC=1;
   switch(Disp){ // Plot the object with (dots,line,circle...)
/*	  case 1:
		 pDC->Rectangle(lX(prevX),lY(Y), lX(X),(int)(m_nBy-m_dGapY));
		 break;
*/	  case 2:  //with Line
		 pDC->MoveTo(lX(X1),lY(Y1));
		 pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 3:  //with Circle
		 pDC->Ellipse(	CRect(lX(X)-3,lY(Y)-3,lX(X)+3,lY(Y)+3)	 );
		 break;
	  case 4:
//		 pDC->MoveTo((int)((prevX-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx),lY(prevY));
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
//	m_bUsingDC=0;
}


void CFigure::PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;

	char strTemp[100];
	sprintf(strTemp,"Number of Nns=%d", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);

//	m_bUsingDC=0;
}


void CFigure::PutVal_Graph2D(double X, double Y, double NumToPrint, CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;

	char strTemp[100];
	sprintf(strTemp,"%0.3lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);

//	m_bUsingDC=0;
}

void CFigure::PutVal_p5(double X, double Y, double NumToPrint, CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;

	char strTemp[100];
	sprintf(strTemp,"%0.5lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);

//	m_bUsingDC=0;
}


void CFigure::PutIntVal(double X, double Y, int NumToPrint, CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;

	char strTemp[100];
	sprintf(strTemp,"%d  ", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);

//	m_bUsingDC=0;
}
void CFigure::PutText(double X, double Y, char ToPrint[], CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;

	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), ToPrint);

//	m_bUsingDC=0;
}

void CFigure::PutChar_After10(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)+10),(int)(lY(Y)-5), ToPrint);
}

void CFigure::RectangleAB(double X1, double Y1, double X2, double Y2,CDC* pDC)
{  
	pDC->Rectangle(lX(X1),lY(Y1),lX(X2),lY(Y2));
}



void CFigure::FillSolidRect(int X, int Y,int width, int height, COLORREF rgb ,CDC* pDC)
{ 
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;
	//pDC->FillSolidRect( lX(X),lY(Y),width,	height,	rgb);
	pDC->FillSolidRect(lX(X)-width/2,lY(Y)-height/2,width,height,	rgb);
//	m_bUsingDC=0;
}
void CFigure::FillSolidRect(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;
	//pDC->FillSolidRect( lX(X),lY(Y),width,	height,	rgb);
	pDC->FillSolidRect(lX(X),lY(Y),width,height,	rgb);
//	m_bUsingDC=0;
}
void CFigure::FillSolidRect_CenteredY(double X, double Y,int width, int height, COLORREF rgb ,CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;
	//pDC->FillSolidRect( lX(X),lY(Y),width,	height,	rgb);
	pDC->FillSolidRect(lX(X),lY(Y)-height/2,width,height,	rgb);
//	m_bUsingDC=0;
}

void CFigure::FillSolidRect13x13(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;
	//pDC->FillSolidRect( lX(X),lY(Y),width,	height,	rgb);
	pDC->FillSolidRect(lX(X),lY(Y),13,	13,	rgb);
//	m_bUsingDC=0;
}
void CFigure::FillSolidRect2_CenteredY(double X, double Y, CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y)-4,1,8,	m_BLACK);
}
void CFigure::FillSolidRect8x8(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;
	//pDC->FillSolidRect( lX(X),lY(Y),width,	height,	rgb);
	pDC->FillSolidRect(lX(X),lY(Y),8,	8,	rgb);
//	m_bUsingDC=0;
}

void CFigure::FillSolidRect7x7(int X, int Y, COLORREF rgb ,CDC* pDC)
{  
//	while(m_bUsingDC2){Sleep(1);};	m_bUsingDC=1;
	//pDC->FillSolidRect( lX(X),lY(Y),width,	height,	rgb);
	pDC->FillSolidRect(lX(X),lY(Y),7,	7,	rgb);
//	m_bUsingDC=0;
}

void CFigure::MySetPixel(double X, double Y, int GRAY, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); };	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),RGB(GRAY,GRAY,GRAY));
//	m_bUsingDC=0;
}

void CFigure::MySetPixelRED(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Red);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelGREEN(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Green);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelDKGRN(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_DkGreen);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelBLUE(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Blue);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelPINK(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Pink);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelCYAN(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Cyan);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelYELLOW(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Yellow);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelBROWN(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Brown);
//	m_bUsingDC=0;
}

void CFigure::MySetPixelVIOLET(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_Violet);
//	m_bUsingDC=0;
}
void CFigure::MySetPixelBLACK(double X, double Y, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_BLACK);
//	m_bUsingDC=0;
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
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),RGB(R,G,B));
//	m_bUsingDC=0;
}

void CFigure::MySetPixelRGB(int X, int Y, int R, int G, int B, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),RGB(R,G,B));
//	m_bUsingDC=0;
}


void CFigure::MySetPixelRGB(double X, double Y, COLORREF *rgb, CDC* pDC)
{	
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),*rgb);
//	m_bUsingDC=0;
}

void CFigure::FromDataToScreenXY(double X, double Y, int &x, int &y)
{
	x=       (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=       (lY(Y)); //They are in (int) form because it is in screen pixel unit
}

void CFigure::FromDataToScreenY(double Y, int &y)
{
	y=       (lY(Y)); //They are in (int) form because it is in screen pixel unit
}

void CFigure::FromDataToScreenXY(double X, double Y, float &x, float &y)
{
	x=   (float) (lX(X)); //They are in (int) form because it is in screen pixel unit
	y=   (float) (lY(Y)); //They are in (int) form because it is in screen pixel unit
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
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
   double  stepX,stepY;
   double L,Lo;

	int nSaveDC=pDC->SaveDC();
	pDC->SelectObject(&m_LightGrayPen);

	stepX=(m_dSpanX)/10.;//Get_Var("Input interval :");
	stepY=(m_dSpanY)/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
   //stepX=m_dMaxX/10.;//Get_Var("Input interval :");
   //stepY=m_dMaxY/10.;//Get_Var("Input interval :");
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Rectangle((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)(m_nTy+m_dGapY),(int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)(m_nBy-m_dGapY+1));
   //----------------------------------Horizontal lines
   for(L=m_dMinY;L<m_dMaxY;L+= stepY){
	   Lo=m_nBy - (L-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY-m_dGapY;
	   pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
	   pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
   }
   //----------------------------Vertical lines
   for(L=m_dMinX;L<m_dMaxX;L+=stepX){
	   Lo=(L-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx+SHIFT_TO_X;
	   pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
	   pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
   }
   //---------------------------X,Y axis
   //pDC->MoveTo((int)(m_nTx+m_dGapX),(int)(m_nBy-m_dGapY));   pDC->LineTo((int)(m_nBx-m_dGapX),(int)(m_nBy-m_dGapY));
   //pDC->MoveTo((int)(m_nTx+m_dGapX),(int)(m_nTy+m_dGapY));   pDC->LineTo((int)(m_nTx+m_dGapX),(int)(m_nBy-m_dGapY));
   /*	if(m_dMaxY>=0 && m_dMinY<=0){  //Plot zero line
   pDC->MoveTo((int)(m_nTx+m_dGapX), (int)(m_nBy-(0-m_dMinY)*Dy_DoubleGapY_Per_Max_MinY-m_dGapY));
   pDC->LineTo((int)(m_nBx-m_dGapX), (int)(m_nBy-(0-m_dMinY)*Dy_DoubleGapY_Per_Max_MinY-m_dGapY));  
   }
   */
	pDC->RestoreDC(nSaveDC); //Before deleting the registered resource, you should unregister the resource using RestoreDC(). 
//	m_bUsingDC=0;
}

void CFigure::RepaintPane_2D( CDC* pDC)  
{
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
   double  stepX,stepY;
   double L,Lo;

	int nSaveDC=pDC->SaveDC();
	pDC->SelectObject(&m_LightGrayPen);

	stepX=m_dSpanX/10.;//Get_Var("Input interval :");
	stepY=m_dSpanY/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
   //stepX=m_dMaxX/10.;//Get_Var("Input interval :");
   //stepY=m_dMaxY/10.;//Get_Var("Input interval :");
	pDC->SelectObject(&m_bkBrush);  
	pDC->Rectangle(lX(m_dMinX),lY(m_dMaxY), lX(m_dMaxY), lY(m_dMinY));
 
	//---------------------------Horizontal lines
   for(L=m_dMinY;L<m_dMaxY;L+= stepY){
	   Lo=m_nBy - (L-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY-m_dGapY;
	   pDC->MoveTo((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)Lo);
	   pDC->LineTo((int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)Lo);
   }
   //----------------------------Vertical lines
   for(L=m_dMinX;L<m_dMaxX;L+=stepX){
	   Lo=(L-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx+SHIFT_TO_X;
	   pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
	   pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
   }
   //----------------------------X, Y axis
   //pDC->MoveTo((int)(m_nTx+m_dGapX),(int)(m_nBy-m_dGapY));   pDC->LineTo((int)(m_nBx-m_dGapX),(int)(m_nBy-m_dGapY));
   //pDC->MoveTo((int)(m_nTx+m_dGapX),(int)(m_nTy+m_dGapY));   pDC->LineTo((int)(m_nTx+m_dGapX),(int)(m_nBy-m_dGapY));
   /*	if(m_dMaxY>=0 && m_dMinY<=0){  //Plot zero line
   pDC->MoveTo((int)(m_nTx+m_dGapX), (int)(m_nBy-(0-m_dMinY)*Dy_DoubleGapY_Per_Max_MinY-m_dGapY));
   pDC->LineTo((int)(m_nBx-m_dGapX), (int)(m_nBy-(0-m_dMinY)*Dy_DoubleGapY_Per_Max_MinY-m_dGapY));  
   }
   */
	pDC->RestoreDC(nSaveDC); //Before deleting the registered resource, you should unregister the resource using RestoreDC(). 
//	m_bUsingDC=0;
}

void CFigure::RefreshPane( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(
		m_nPaneTx,m_nPaneTy,m_nPaneBx-m_nPaneTx,m_nPaneBy-m_nPaneTy,
		pm_memDC,m_nPaneTx,m_nPaneTy,SRCCOPY);
}
void CFigure::RefreshPane_TRANS( CDC* pobjDC, CDC* pmTransDC,CDC* pm_memDC)
{
	pobjDC->BitBlt(	m_nPaneTx,m_nPaneTy,m_nPaneBx-m_nPaneTx,m_nPaneBy-m_nPaneTy,pm_memDC,m_nPaneTx,m_nPaneTy,SRCINVERT);
	pobjDC->BitBlt(	m_nPaneTx,m_nPaneTy,m_nPaneBx-m_nPaneTx,m_nPaneBy-m_nPaneTy,pmTransDC,m_nPaneTx,m_nPaneTy,SRCAND);
	pobjDC->BitBlt(	m_nPaneTx,m_nPaneTy,m_nPaneBx-m_nPaneTx,m_nPaneBy-m_nPaneTy,pm_memDC,m_nPaneTx,m_nPaneTy,SRCINVERT);
}
void CFigure::RefreshFromXandSome(double X1, CDC* pObjDC, CDC* pMemDC)
{
	pObjDC->BitBlt(
		lX(X1),m_nPaneTy,10,m_nPaneBy-m_nPaneTy,
		pMemDC,lX(X1),m_nPaneTy,SRCCOPY);
}

void CFigure::ErasePaneBlack(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(BLACK_BRUSH);
	pm_memDC->Rectangle((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)(m_nTy+m_dGapY),(int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)(m_nBy-m_dGapY));
	pm_memDC->RestoreDC(nSaveDC); 
}
void CFigure::ErasePaneWhite(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(WHITE_BRUSH);
	pm_memDC->Rectangle((int)(m_nTx+m_dGapX+SHIFT_TO_X),(int)(m_nTy+m_dGapY),(int)(m_nBx-m_dGapX+SHIFT_TO_X+1),(int)(m_nBy-m_dGapY));
	pm_memDC->RestoreDC(nSaveDC); 
}
void CFigure::RefreshFrame( CDC* pobjDC, CDC* pm_memDC)
{
	pobjDC->BitBlt(m_nTx,m_nTy,m_nBx-m_nTx,m_nBy-m_nTy,	pm_memDC,m_nTx,m_nTy,SRCCOPY);
}

void CFigure::EraseFrameBlack(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(BLACK_BRUSH);
	pm_memDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);
	pm_memDC->RestoreDC(nSaveDC); 
}
void CFigure::EraseFrameWHITE(CDC* pm_memDC)
{
	int nSaveDC=pm_memDC->SaveDC();
	pm_memDC->SelectStockObject(WHITE_BRUSH);
	pm_memDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);
	pm_memDC->RestoreDC(nSaveDC); 
}


#undef Dx
#undef Dy
#undef DoubleGapX
#undef DoubleGapY
#undef Dy_DoubleGapY
#undef Dx_DoubleGapX

