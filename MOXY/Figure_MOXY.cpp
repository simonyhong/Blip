#include "stdafx.h"
#include <math.h>

#include "MOXY_Doc.h"
#include "Figure_MOXY.h"
#include "RunningLineDisplay.h"
#include "MOXY_View.h"


BOOL CFigure::m_bUsingDC=0;
BOOL CFigure::m_bUsingDC2=0;
BOOL CFigure::m_bUsingDC3=0;

CPen CFigure::m_OrangePen,CFigure::m_RedPen,CFigure::m_RedPen2,CFigure::m_BluePen,CFigure::m_GreenPen;
CPen CFigure::m_BlackPen2,CFigure::m_BlackPen3,CFigure::m_GrayPen,CFigure::m_LightGrayPen,CFigure::m_Null_Pen,CFigure::m_VIOLETpen;
CBrush CFigure::m_BlackBrush,CFigure::m_WhiteBrush,CFigure::m_RedBrush,CFigure::m_LightGrayBrush, CFigure::m_TmpBrush;
int	CFigure::m_DiviceContaxtCreatedAlready=0;

#define Dx         ((m_nBx)-(m_nTx))
#define Dy         ((m_nBy)-(m_nTy))
#define DoubleGapX                    ((m_dGapX)+(m_dGapX))
#define DoubleGapY                    ((m_dGapY)+(m_dGapY))
#define Dy_DoubleGapY                 (Dy-DoubleGapY)
#define Dx_DoubleGapX                 (Dx-DoubleGapX)

//#define lX(X) (int)((X-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_nTranslateX) //They are in (int) form because it is in screen pixel unit
//#define lY(Y) (int)(m_nBy_GapY-(Y-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY)//They are in (int) form because it is in screen pixel unit
//#define lY2(Y) (int)(m_nBy_GapY-(Y-m_dMinY2)*m_dDy_DoubleGapY_Per_Max_MinY2)//They are in (int) form because it is in screen pixel unit

#define lX(X)  (int)(( X)*m_dDx_DoubleGapX_Per_Max_MinX +m_TransformX )//They are in (int) form because it is in screen pixel unit
#define lY(Y)  (int)((-Y)*m_dDy_DoubleGapY_Per_Max_MinY +m_TransformY )//They are in (int) form because it is in screen pixel unit
#define lY2(Y) (int)((-Y)*m_dDy_DoubleGapY_Per_Max_MinY2+m_TransformY2)//They are in (int) form because it is in screen pixel unit


CFigure::CFigure()
{
	m_Is2ndAxis=0;
	m_TriggerOptionStatus.NameString="Free-Run  ";	m_TriggerOptionStatus.m_RelativePos=0.25;  m_TriggerOptionStatus.AtTopOrBottom=1;
	m_SuperImposeStatus.NameString="No Superimposition";	m_SuperImposeStatus.m_RelativePos=0.7;  m_SuperImposeStatus.AtTopOrBottom=1;
	m_NnRecordingChan.NameString="Chan:0/Nn:0";		m_NnRecordingChan.m_RelativePos=0.5;  m_NnRecordingChan.AtTopOrBottom=1;
	m_GridNet.NameString="No Mash"; 	m_GridNet.status=0; m_GridNet.m_RelativePos=0.7;  m_GridNet.AtTopOrBottom=0;
	m_RwdAmount.NameString="RWD:300"; 	m_RwdAmount.m_RelativePos=0.7;  m_RwdAmount.AtTopOrBottom=1;
	m_BehavioralObj.NameString="B-Obj: 0";m_BehavioralObj.m_RelativePos=0.25;  m_BehavioralObj.AtTopOrBottom=0;
	m_FileName.m_RelativePos=0.1;  m_FileName.AtTopOrBottom=1;
	/////////////////////////
	m_IsTickY=0;
	m_nTx=500; m_nTy=0; m_nBx=1000; m_nBy=500;
	m_BlownUpTx=200;m_BlownUpTy=5; m_BlownUpBx=820; m_BlownUpBy=605;
	m_dGapX=(Dx)/20.; m_dGapY=(Dy)/20.; //Gap calculation should come after Tx, ...
	m_nBy_GapY=m_nBy-m_dGapY;
	m_dStepX=0;m_dStepY=0;
	m_howManyX=-1; m_howManyY=-1;
	m_page=0; m_NumPages=1;
	m_FontWidth=5;	m_FontHeight=-1;// m_FontHeight is set to -1 to warn the user to set it first.
	m_NumVariables_onLeftAxes=0;m_NumVariables_onRightAxes=0;

	if(m_DiviceContaxtCreatedAlready==0){
		m_DiviceContaxtCreatedAlready=1;
		m_OrangePen.CreatePen(PS_SOLID, 1,RGB(255,190,50));
		m_RedPen.CreatePen(PS_SOLID, 1,RGB(255,50,50));
		m_RedPen2.CreatePen(PS_SOLID, 2,RGB(255,50,50));
		m_BluePen.CreatePen(PS_SOLID, 1,RGB(0,0,255));
		m_VIOLETpen.CreatePen(PS_SOLID, 1,RGB(200,90,250));
		m_GreenPen.CreatePen(PS_SOLID, 1,RGB(0,200,0));
		m_BlackPen2.CreatePen(PS_SOLID, 2,RGB(0,0,0));
		m_BlackPen3.CreatePen(PS_SOLID, 3,RGB(0,0,0));
		m_Null_Pen.CreatePen(PS_NULL, 1,RGB(0,0,0));
		m_GrayPen.CreatePen(PS_SOLID, 1,RGB(70,70,70));
		m_LightGrayPen.CreatePen(PS_SOLID, 1,RGB(200,200,200));
	
		LOGBRUSH LogBrh;
		LogBrh.lbStyle=BS_SOLID;	LogBrh.lbColor=RGB(200,200,200);	
		LogBrh.lbColor=RGB(220,220,220);
		m_LightGrayBrush.CreateBrushIndirect( &LogBrh );
		LogBrh.lbColor=RGB(255,0,0);	
		m_RedBrush.CreateBrushIndirect( &LogBrh );
		LogBrh.lbColor=RGB(0,0,0);	
		m_BlackBrush.CreateBrushIndirect( &LogBrh );
		LogBrh.lbColor=RGB(255,255,255);	
		m_WhiteBrush.CreateBrushIndirect( &LogBrh );
	}	
	int i;
	for(i=0;i<MaxDC;i++)sprintf(m_cXaxis[i],"");
	for(i=0;i<MaxDC;i++)sprintf(m_cYaxis[i],"");

	m_dMinX=0., m_dMaxX=1000., m_dMinY=0., m_dMaxY=1.; m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY; 
	m_dMinY2=0., m_dMaxY2=100.; m_dSpanY2=m_dMaxY2-m_dMinY2; 
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
	m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.
	m_dLabelBias=0;m_dLabelScale=1;
	m_Red=RGB(255,0,0); m_Blue=RGB(100,100,255); m_Green=RGB(0,255,0); m_DkGreenPen=RGB(0,155,0); m_Pink=RGB(255,150,150); 
	m_Cyan=RGB(0,255,255); m_Yellow=RGB(240,240,0); m_Brown=RGB(240,130,0); m_Violet=RGB(200,90,250);m_BLACK=RGB(0,0,0);
	m_dDy_DoubleGapY_Per_Max_MinY2 =   ((Dy-DoubleGapY)/(m_dSpanY2));
	m_VarToDispRECT.SetSize(10);
	m_VarToDispLineEnd.SetSize(10);
	m_VarToDispString.SetSize(10);
	m_numUserPickedVariables=0;

	//############### Angular-projected coordinates
	m_ScreenScale=100; //Later it will adjust to match that of the actual pane size

}

CFigure::~CFigure()
{
	m_OrangePen.DeleteObject();m_RedPen.DeleteObject();m_RedPen2.DeleteObject();m_BluePen.DeleteObject(); m_VIOLETpen.DeleteObject();
	m_GreenPen.DeleteObject();m_BlackPen2.DeleteObject();m_BlackPen3.DeleteObject();m_GrayPen.DeleteObject();m_LightGrayPen.DeleteObject();m_Null_Pen.DeleteObject();
	m_BlackBrush.DeleteObject();m_WhiteBrush.DeleteObject();m_LightGrayBrush.DeleteObject();m_RedBrush.DeleteObject(); m_TmpBrush.DeleteObject(); 

	m_VarToDispRECT.RemoveAll();
	m_VarToDispLineEnd.RemoveAll();
	m_VarToDispString.RemoveAll();
}


void CFigure::SetLabelsXY(char Xaxis[50], char Yaxis[50])
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////
	SetFigPositionsXY(m_normTx,m_normTy,m_normBx,m_normBy);
}

void CFigure::SetFig_Pos_n_Labels_XY(int Tx, int Ty, int Bx, int By, char Xaxis[50], char Yaxis[50])
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////
	SetFigPositionsXY(Tx, Ty, Bx, By);
}

void CFigure::Set_MinMax_n_Labels_XY(char Xaxis[50], char Yaxis[50],double Min_X, double Max_X, double Min_Y, double Max_Y,int oxFRAME)
{
	////////////
	sprintf(m_cXaxis[0],"%s",Xaxis); // X coordinate name
	sprintf(m_cYaxis[0],"%s",Yaxis); // Y coordinate name
	////////////

	m_dMinX=Min_X, m_dMaxX=Max_X, m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; m_dSpanX=Max_X-Min_X; m_dSpanY=Max_Y-Min_Y; m_bDrawFRAME=oxFRAME;
	SetFigPositionsXY(m_normTx,m_normTy,m_normBx,m_normBy);
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

void CFigure::SetFigPositionsXY(int Tx, int Ty, int Bx, int By)
{
	m_nTx=Tx; m_nTy=Ty; m_nBx=Bx; m_nBy=By;

	ComputeMapping_X_Y_Y2();
}

void CFigure::Set_2ndMinMaxY(double Min_Y2, double Max_Y2)
{
	m_dMinY2=Min_Y2; m_dMaxY2=Max_Y2;
	ComputeMapping_X_Y_Y2();
}

void CFigure::ComputeMapping_X_Y_Y2()
{
	if(m_FontHeight<=0){
		AfxMessageBox("It is a good idea to set the size of font", MB_OK); m_FontHeight=10;
	}

	m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY;
	
	//Set the size of the Gap between the graph pane and the edge of the frame: width of the scale legend area.
	if(m_FigID>=FIRST_RUNNINGLINE_FIG){
		m_dGapX=m_FontWidth*20;
	}
	else m_dGapX=m_FontWidth*5;
	m_dGapY=m_FontHeight*1.5;

	m_nBy_GapY=m_nBy-m_dGapY;

	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/m_dSpanX);
	m_nTranslateX=(int)(m_dGapX+m_nTx);
	m_TransformX=-m_dMinX*m_dDx_DoubleGapX_Per_Max_MinX+m_nTranslateX;
	m_TransformY=m_nBy_GapY+m_dMinY*m_dDy_DoubleGapY_Per_Max_MinY;
	m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.

	//The Y2 axis
	m_dSpanY2=m_dMaxY2-m_dMinY2;
	m_dDy_DoubleGapY_Per_Max_MinY2 =   ((Dy-DoubleGapY)/(m_dSpanY2));
	m_TransformY2=m_nBy_GapY+m_dMinY2*m_dDy_DoubleGapY_Per_Max_MinY2;
}

void CFigure::Prepare4FigFrame(CDC *pDC)
{
	ComputeMapping_X_Y_Y2();

	//Other stuff
	if(m_FigID>=FIRST_RUNNINGLINE_FIG){
		RectOfLabel(m_TriggerOptionStatus, pDC);
		RectOfLabel(m_SuperImposeStatus, pDC);
		RectOfLabel(m_NnRecordingChan, pDC);
	}
	else if(m_FigID==XY_FIG){
		RectOfLabel(m_FileName, pDC);	m_FileName.PrvWholeRect=m_FileName.StringRect;
		RectOfLabel(m_RwdAmount, pDC);	m_RwdAmount.PrvWholeRect=m_RwdAmount.StringRect;
		RectOfLabel(m_GridNet, pDC);
		RectOfLabel(m_BehavioralObj, pDC);
	}

	m_TitleAreaRect.top=m_nTy; m_TitleAreaRect.bottom=m_TitleAreaRect.top+m_FontHeight*1.3;
	m_TitleAreaRect.left=lX(m_dMinX); m_TitleAreaRect.right=lX(m_dMaxX);
}

void CFigure::RectOfLabel(ClickableLabel &Label, CDC *pDC)
{
	CSize size=pDC->GetOutputTextExtent(Label.NameString);
	Label.StringRect.left   =(int)lX(m_dMinX+m_dSpanX*Label.m_RelativePos);
	Label.StringRect.right  =Label.StringRect.left+size.cx;
	if(Label.AtTopOrBottom==1){//At the top portion of the figure
		Label.StringRect.top    =(int)(m_nTy+m_FontHeight*0.3); 
		Label.StringRect.bottom = Label.StringRect.top+m_FontHeight;
	}
	else{
		Label.StringRect.top    =(int)(m_nBy-m_FontHeight*1.3); 
		Label.StringRect.bottom = Label.StringRect.top+m_FontHeight;
	}

	Label.ControlRect.right=Label.StringRect.left-m_FontWidth;
	Label.ControlRect.left=Label.ControlRect.right-2*m_FontWidth;
	Label.ControlRect.top=Label.StringRect.top;
	Label.ControlRect.bottom=Label.StringRect.bottom;
}

void CFigure::RefreshXYTitleArea(CDC *pObjDC)
{
	pObjDC->BitBlt(BIT_BLT(m_TitleAreaRect,m_pDC));
}

void CFigure::PositionOfUserPickedVariables(int VarID, int Left0_Or_Right1_axis)
{
	if(Left0_Or_Right1_axis==USE_LEFT_AXIS){
		m_VarToDispRECT[VarID].left=(int)(m_nTx+m_FontWidth);
		m_VarToDispRECT[VarID].right=m_VarToDispRECT[VarID].left+m_FontWidth*m_VarToDispString[VarID].GetLength();
		m_VarToDispLineEnd[VarID]=m_VarToDispRECT[VarID].right+3*m_FontWidth;
		m_VarToDispRECT[VarID].top=(int)(m_nTy+m_FontHeight*(2+3*m_NumVariables_onLeftAxes)); m_VarToDispRECT[VarID].bottom=m_VarToDispRECT[VarID].top+m_FontHeight;
		m_NumVariables_onLeftAxes++;

	}
	else{
		m_VarToDispRECT[VarID].left=(int)(lX(m_dMaxX)+4*m_FontWidth);
		m_VarToDispRECT[VarID].right=m_VarToDispRECT[VarID].left+m_FontWidth*m_VarToDispString[VarID].GetLength();
		m_VarToDispLineEnd[VarID]=m_VarToDispRECT[VarID].right+3*m_FontWidth;
		m_VarToDispRECT[VarID].top=(int)(m_nTy+m_FontHeight*(2+3*m_NumVariables_onRightAxes)); m_VarToDispRECT[VarID].bottom=m_VarToDispRECT[VarID].top+m_FontHeight;
		m_NumVariables_onRightAxes++;
	}
}

void CFigure::Frame_Graph2D(double Min_X, double Max_X, double Min_Y, double Max_Y, CDC* pDC,int oxFRAME)
{
	m_dMinX=Min_X, m_dMaxX=Max_X, m_dMinY=Min_Y, m_dMaxY=Max_Y; m_dMeanY=(Min_Y+Max_Y)/2.; m_dSpanX=Max_X-Min_X; m_dSpanY=Max_Y-Min_Y; 
	m_bDrawFRAME=oxFRAME;
	Frame_Graph2D(pDC);
}

void CFigure::Frame_Graph2D(CDC* pDC)
{
#define TICK_Y 6
#define Tick 10

	int nSaveDC=pDC->SaveDC();
	int labelPosX,labelPosY;
	int firstXlabel=1, firstYlabel=1;
	double labelValue;

	pDC->SelectObject(&m_BlackPen2); 

	//pDC->Rectangle((int)m_nTx-20, (int)m_nTy, (int)m_nTx,(int)m_nTy+100);
	if(m_bDrawFRAME==1){ //Draw the frame and the grid of the pannel
		#define Defalut10 10.
		int    shift=0;
		int i, sw=1;
		int PosX2nd=(int)(lX(m_dMaxX)+3), PosY2nd;//In case there is a second Y scale for the right vertical axis
		double  stepX,stepY;
		double L,Lo;
		char charVarTemp[100];
		char charVarTemp2nd[100];
		m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
		m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/(m_dSpanX));
		m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.

		pDC->SelectObject(&m_BlackPen3); 
		pDC->MoveTo((int)m_nTx+5,(int)m_nBy);pDC->LineTo((int)m_nBx,(int)m_nBy);
		pDC->MoveTo((int)m_nBx,(int)m_nTy+5);pDC->LineTo((int)m_nBx,(int)m_nBy);
		pDC->SelectObject(&m_GrayPen); 
		pDC->Rectangle((int)m_nTx, (int)m_nTy, (int)m_nBx,(int)m_nBy);
		
		if(m_howManyY<0){ //Default setting (10 horizontal lines)
			stepY=(m_dSpanY)/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
			pDC->SelectObject(&m_LightGrayPen);
			//----------------------------------Horizontal lines
			shift=0;
			for(i=0,L=m_dMinY;L<m_dMaxY+stepY/10.;L+= stepY, i++){
				Lo=lY(L);
				pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo);
				pDC->LineTo((int)(m_nBx-m_dGapX+1),(int)Lo);
				if(i%5==0){
					if(L==(int)L)sprintf(charVarTemp, "%.0lf ", L );
					else sprintf(charVarTemp, "%.2lf ", L );	
					if(L>=0)labelPosX=(int)(m_nTx); 
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
			shift=0;
			pDC->SelectObject(&m_LightGrayPen);
			for(i=0;i<m_howManyY;i++){
				Lo=lY(m_YTics[i]);
				//Ticks
				//pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo); 
				//pDC->LineTo((int)(m_nTx+Tick+m_dGapX),(int)Lo);
				//Guidelines
				pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo);
				pDC->LineTo((int)(m_nBx-m_dGapX+1),(int)Lo);
				if(m_IsLabeledTicY[i]){
					if(m_YTics[i]==0||0.0001>fabs(m_YTics[i]-Rounded_to_nearest_Int(m_YTics[i])))sprintf(charVarTemp, "%.0lf ", m_YTics[i] );//The value is either 0, or almost the same (0.01% difference) as an integer
					else sprintf(charVarTemp, "%.1lf ", (double)m_YTics[i] );	
					labelPosX=(int)(m_nTx+m_dGapX-strlen(charVarTemp)*m_FontWidth);
					//labelPosX=(int)(m_nTx+1); 
					labelPosY=(int)Lo-5;
					pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
					if(m_Is2ndAxis==1){
						PosY2nd=labelPosY-m_FontHeight*0.3;
						if(m_Y2ndTics[i]==0||0.0001>fabs(m_Y2ndTics[i]/(int)(m_Y2ndTics[i])-1)){//The value is either 0, or almost the same (0.01% difference) as an integer
							sprintf(charVarTemp2nd, "%d ", (int)m_Y2ndTics[i] );
							pDC->TextOut(PosX2nd,PosY2nd, charVarTemp2nd);
						}
						else{
							sprintf(charVarTemp2nd, "%0.1lf ", (double)m_Y2ndTics[i] );
							pDC->TextOut(PosX2nd,PosY2nd, charVarTemp2nd);
						}
					}
					if(firstYlabel==1){//Register the position of the label. It will be used when the user clicks this area to change the scale of the axis.
						m_FirstYlabel_left=labelPosX; m_FirstYlabel_right=m_FirstYlabel_left+m_FontWidth*strlen(charVarTemp); m_FirstYlabel_top=labelPosY; m_FirstYlabel_bottom=m_FirstYlabel_top+m_FontHeight;
						firstYlabel=0;
						if(m_Is2ndAxis==1){
							m_FirstY2label_left=PosX2nd; m_FirstY2label_right=m_FirstY2label_left+m_FontWidth*strlen(charVarTemp2nd); m_FirstY2label_top=PosY2nd; m_FirstY2label_bottom=m_FirstY2label_top+m_FontHeight;
						}
					}
					if(i==m_howManyY-1){
						m_LastYlabel_left=labelPosX; m_LastYlabel_right=m_LastYlabel_left+m_FontWidth*strlen(charVarTemp); m_LastYlabel_top=labelPosY; m_LastYlabel_bottom=m_LastYlabel_top+m_FontHeight;
						if(m_Is2ndAxis==1){
							m_LastY2label_left=PosX2nd; m_LastY2label_right=m_LastY2label_left+m_FontWidth*strlen(charVarTemp2nd); m_LastY2label_top=PosY2nd; m_LastY2label_bottom=m_LastY2label_top+m_FontHeight;
						}
					}
				}
			}
		}
		if(m_howManyX<0){ //Default setting (10 vertical lines)
			stepX=m_dSpanX/10.;//Get_Var("Input interval :");
			//-------------------------Vertical lines
			for(i=0,L=m_dMinX;L<m_dMaxX+stepX/10.;L+=stepX,i++){
				Lo=lX(L);
				pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
				pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
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
			shift=0;
			if(m_XTics[m_howManyX-1]<1000)m_dLabelScale=1; 
			else if(m_XTics[m_howManyX-1]<60000)m_dLabelScale=0.001; 
			else m_dLabelScale=0.0000166666666666; 
			for(i=0;i<m_howManyX;i++){
				Lo=lX(m_XTics[i]);
				labelValue=m_XTics[i]*m_dLabelScale+m_dLabelBias;
				//Ticks
				//pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	
				//pDC->LineTo((int)Lo,(int)(m_nBy-Tick-m_dGapY));
				//Guidelines
				pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
				pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));		
				if(m_IsLabeledTicX[i]){
					if(labelValue==0||0.0001>fabs(labelValue-Rounded_to_nearest_Int(labelValue))){
						if(m_XTics[m_howManyX-1]<1000)sprintf(charVarTemp, "%.0lf", labelValue );
						else if(m_XTics[m_howManyX-1]<60000)sprintf(charVarTemp, "%.0lfs", labelValue );
						else sprintf(charVarTemp, "%.0lfm", labelValue );
					}
					else{
						if(m_XTics[m_howManyX-1]<1000)sprintf(charVarTemp, "%.1lf", labelValue );
						else if(m_XTics[m_howManyX-1]<60000)sprintf(charVarTemp, "%.1lfs", labelValue );
						else sprintf(charVarTemp, "%.1lfm", labelValue );
					}
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
		//---------------------------X,Y axis
		pDC->TextOut((int)(m_nTx+m_dGapX+45),(int)(m_nBy-m_dGapY+15),m_cXaxis[m_page]);//Horizontal coordinate Axis Name
		m_nYlabelPosX=(int)(m_nTx+m_dGapX-30);m_nYlabelPosY=(int)(m_nTy+m_dGapY-20);
		pDC->TextOut(m_nYlabelPosX, m_nYlabelPosY, m_cYaxis[m_page]);//Vertical coordinate Axis Name
	}
	else{//NO frame: Draw an EMPTY pannel.
		m_dGapX=0; m_dGapY=0; //Gap calculation should come after Tx, ...
		m_nBy_GapY=m_nBy-m_dGapY;

		m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/(m_dSpanY));
		m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/m_dSpanX);
		m_nTranslateX=(int)(m_dGapX+m_nTx);
		m_TransformX=-m_dMinX*m_dDx_DoubleGapX_Per_Max_MinX+m_nTranslateX;
		m_TransformY=m_nBy_GapY+m_dMinY*m_dDy_DoubleGapY_Per_Max_MinY;
		m_lX0=lX(0);m_lY0=lY(0);//These saved lX(0), lY(0) will be used to calculate width and height of some measure later.

		//The Y2 axis
		m_dSpanY2=m_dMaxY2-m_dMinY2;
		m_dDy_DoubleGapY_Per_Max_MinY2 =   ((Dy-DoubleGapY)/(m_dSpanY2));
		m_TransformY2=m_nBy_GapY+m_dMinY2*m_dDy_DoubleGapY_Per_Max_MinY2;

		//FromDataToScreenXY(m_dMinX, m_dMinY, m_nPaneTx, m_nPaneTy); //It maps relative point (i,j) of the graph to a point on the screen.	
		//FromDataToScreenXY(m_dMaxX, m_dMaxY, m_nPaneBx, m_nPaneBy); //It maps relative point (i,j) of the graph to a point on the screen.	
		//pDC->SelectStockObject(BLACK_BRUSH);
		//pDC->Rectangle((int)m_nTx-1, (int)m_nTy-1, (int)m_nBx+1,(int)m_nBy+1);
		//pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle((int)m_nTx, (int)m_nTy, (int)m_nBx,(int)m_nBy);
		//pDC->TextOut((int)(m_nTx+m_dGapX-10),(int)(m_nBy-m_dGapY-12),m_cXaxis[m_page]);//Horizontal coordinate Axis Name
		m_nYlabelPosX=(int)(m_nTx+m_dGapX-10);m_nYlabelPosY=(int)(m_nTy+m_dGapY-12);
		//pDC->TextOut((int)(m_nTx+m_dGapX-10),(int)(m_nTy+m_dGapY-12),m_cYaxis[m_page]);//Vertical coordinate Axis Name
	}

	pDC->RestoreDC(nSaveDC); 
} 

void CFigure::Frame2D_for_VIS_Replica(int DisplayMode, CDC* pDC, CMOXY_View *pView)
{
#define Tick 5
//#define Tick 10
	int nSaveDC=pDC->SaveDC();
	int shift, i;
	double  stepX,stepY, L,Lo;
	char charVarTemp[100];
	CBrush *pOldBrush;
	CPen *pOldPen;
	m_dMeanY=(m_dMinY+m_dMaxY)/2.;  m_dSpanX=m_dMaxX-m_dMinX; m_dSpanY=m_dMaxY-m_dMinY;
	m_dDy_DoubleGapY_Per_Max_MinY =   ((Dy-DoubleGapY)/double(m_dSpanY));
	m_dDx_DoubleGapX_Per_Max_MinX =   ((Dx-DoubleGapX)/double(m_dSpanX));

	pOldPen=pDC->SelectObject(&m_BlackPen2); 
	pOldBrush=pDC->SelectObject(&m_WhiteBrush);
	pDC->Rectangle(m_nTx,m_nTy,m_nBx,m_nBy);
	pDC->SelectObject(&m_BlackBrush);
	pDC->Rectangle(lX(m_dMinX),lY(m_dMaxY),lX(m_dMaxX),lY(m_dMinY));
	pDC->SetBkColor(RGB(255, 255, 255));

	if(m_dStepX==0)stepX=m_dSpanX/10.;
	else stepX=m_dStepX;
	if(m_dStepY==0)stepY=m_dSpanY/4.;
	else stepY=m_dStepY;
	//------------------------- Horizontal lines
	shift=0;
	for(i=0;i<m_howManyY;i++){
		Lo=lY(m_YTics[i]);
		pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo); pDC->LineTo((int)(m_nTx-Tick+m_dGapX),(int)Lo);
		if(m_IsLabeledTicY[i]){
			if(m_YTics[i]==(int)m_YTics[i])sprintf(charVarTemp, "%.0lf ", m_YTics[i] );
			else sprintf(charVarTemp, "%.1lf ", m_YTics[i] );	
			pDC->TextOut((int)(m_nTx+m_FontWidth), (int)Lo-5,charVarTemp);
		}
	}
	//------------------------- Vertical lines
	for(i=0;i<m_howManyX;i++){
		Lo=lX(m_XTics[i]);
		pDC->MoveTo((int)Lo,(int)(m_nBy		-m_dGapY));	pDC->LineTo((int)Lo,(int)(m_nBy+Tick-m_dGapY));
		if(m_IsLabeledTicX[i]){
			if(m_XTics[i]*m_dLabelScale+m_dLabelBias==(int)(m_XTics[i]*m_dLabelScale+m_dLabelBias))sprintf(charVarTemp, "%.0lf", m_XTics[i]*m_dLabelScale+m_dLabelBias );
			else sprintf(charVarTemp, "%.2lf",m_XTics[i]*m_dLabelScale+m_dLabelBias );	
			pDC->TextOut((int)(Lo)-3,(int)(m_nBy-0.7*m_dGapY),charVarTemp);
		}
	}
	//################  Polar Mash
	if(m_GridNet.status==1){
		PlotGrid(DisplayMode, pDC);
		m_GridNet.NameString="GridMash";
	}
	//else if(m_GridNet.status==2){
	//	PlotPolarMash(pDC);
	//	m_GridNet.NameString="PolarMash";
	//}
	else{m_GridNet.NameString="No Mash";}
	pDC->TextOut(m_GridNet.StringRect.left, m_GridNet.StringRect.top,m_GridNet.NameString);

	DisplayFileName_and_RecordingStatus(pView);
	DisplayLabel(m_RwdAmount);
	pDC->RestoreDC(nSaveDC); 
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


int CFigure::Rounded_to_nearest_Int(double Val)
{
	int RoundedUp,RoundedDown;
	if(Val>0){
		RoundedUp=(int)(Val+0.5);
		RoundedDown=(int)(Val);
	}
	else{
		RoundedUp=(int)(Val);
		RoundedDown=(int)(Val-0.5);
	}
	if(fabs(Val-RoundedUp)>fabs(Val-RoundedDown))return RoundedDown;
	else return RoundedUp;
}

void CFigure::DisplayFileName_and_RecordingStatus(CMOXY_View *pView)
{
	//################  The "recording status LED"
	CBrush *pOldBrush;
	CPen *pOldPen=m_pDC->SelectObject(&m_Null_Pen); 

	//First, erase the previous one
	pOldBrush=m_pDC->SelectObject(&m_WhiteBrush); 
	m_pDC->Rectangle(m_FileName.StringRect.left,m_FileName.StringRect.top,m_FileName.StringRect.right+m_FontHeight,m_FileName.StringRect.bottom);
	//Copy the new name
	m_FileName.NameString=pView->m_JustFileName;
	//Measure the size of the name
	RectOfLabel(m_FileName, m_pDC);
	//Display the name
	m_pDC->TextOut(m_FileName.StringRect.left, m_FileName.StringRect.top, m_FileName.NameString);

	//Display the recording status (red/gray dot)
	if(*m_pIsSaving==0){
		pOldBrush=m_pDC->SelectObject(&m_LightGrayBrush);
	}
	else if(*m_pIsSaving==1){
		pOldBrush=m_pDC->SelectObject(&m_RedBrush); 
	}
	m_pDC->Ellipse(m_FileName.StringRect.right, m_FileName.StringRect.top, m_FileName.StringRect.right+m_FontHeight, m_FileName.StringRect.bottom);
	m_pDC->SelectObject(pOldPen); m_pDC->SelectObject(pOldBrush); 
}

void CFigure::DisplayLabel(ClickableLabel &Label)
{
	m_pDC->TextOut(Label.StringRect.left, Label.StringRect.top, Label.NameString);
}

void CFigure::ListObjIDs(CDC* pDC, int SelectedObjID)
{
#define HOW_MANY_OBJ_IDS 20
	char Idx[10];
	float OneDivision=(1.0/HOW_MANY_OBJ_IDS);
	int left,top;
	for(int i=0;i<HOW_MANY_OBJ_IDS;i++){
		itoa(i,Idx,10);
		left=m_nPaneBx+m_FontWidth; 
		top=lY(((i+0.5)*OneDivision)*m_dSpanY+m_dMinY);
		if(i==SelectedObjID){
			pDC->SetBkColor(RGB(200, 200, 200));
			//pDC->Ellipse(left,top, left+m_FontHeight, top+m_FontHeight);
		}
		else{
			pDC->SetBkColor(RGB(255, 255, 255));
		}
		pDC->TextOut(left,top, Idx);
	}
	pDC->SetBkColor(RGB(255, 255, 255));
}

//This function is for the user to select an object to be used to display when the user clicks the XY window with the right click to get attention from the monkey.
int CFigure::WhichObjSelected(CPoint point)
{
	if(point.x>m_nPaneBx && point.x<m_nBx ){
		return HOW_MANY_OBJ_IDS*(Divide(m_nPaneBy-point.y,(m_nPaneBy-m_nPaneTy)));
	}
	else return -1;
}

//<----------------------------------------------------------------------------------|	
													//Don't Change! this allignment=>| 
void CFigure::Graph2D(double X, double Y,int Disp, CDC* pDC)//-----------------------|
{  
   switch(Disp){ // Plot the object with (dots,line,circle...)
	  case 2:  //with Line
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
		pDC->TextOut(m_lX0-350, lY(1-0.053*X-.21), strTemp1);
		sprintf(strTemp1,"x%d:", X+1);     
		pDC->TextOut(m_lX0-350,lY(1-0.053*X-.21), strTemp1);
		 break;
	  case 8:  //with Circle
		char strTemp2[100];
		sprintf(strTemp2,"       %0.3lf", Y);     
		pDC->TextOut(m_lX0-560, lY(1-0.053*X-.21), strTemp2);
		sprintf(strTemp2,"I%d:", X+1);     
		pDC->TextOut(m_lX0-560, lY(1-0.053*X-.21), strTemp2);
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
		  pDC->MoveTo(lX(X),m_lY0);
		  pDC->LineTo(lX(X),lY(Y));
		 break;
	  case 31: // Histogram
		  pDC->Rectangle( lX(X)-1,m_lY0,lX(X)+1,lY(Y));
		 break;
	  case 32: // Histogram
		  pDC->Rectangle( lX(X)-2,m_lY0,lX(X)+2,lY(Y));
		 break;
	  case 34: // Histogram (the width of the column starts at the X and extend to X+10)
		  pDC->Rectangle( lX(X),m_lY0,lX(X)+10,lY(Y));
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

	  default:  AfxMessageBox("?");break;
   }//End of Switch

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

void CFigure::Abs_Ellipse(float X, float Y,int size, CDC* pDC)
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
		pDC->TextOut(m_lX0-350, lY(1-0.053*X1-.21), strTemp1);
		sprintf(strTemp1,"x%d:", X1+1);     
		pDC->TextOut(m_lX0-350, lY(1-0.053*X1-.21), strTemp1);
		 break;
	  case 8:  //with Circle
		char strTemp2[100];
		sprintf(strTemp2,"       %0.3lf", Y);     
		pDC->TextOut(m_lX0-560, lY(1-0.053*X1-.21), strTemp2);
		sprintf(strTemp2,"I%d:", X1+1);     
		pDC->TextOut(m_lX0-560, lY(1-0.053*X1-.21), strTemp2);
		 break;
	  default:  break;
   }//End of Switch
}

void CFigure::MyMoveTo(double X, double Y)
{
	m_pDC->MoveTo(lX(X),lY(Y));
}
void CFigure::MyLineTo(double X, double Y)
{
	m_pDC->LineTo(lX(X),lY(Y));
}

void CFigure::MoveTo2nd(double X, double Y, CDC* pDC)
{
	pDC->MoveTo(lX(X),lY2(Y));
}
void CFigure::LineTo2nd(double X, double Y, CDC* pDC)
{
	pDC->LineTo(lX(X),lY2(Y));
}
void CFigure::LineAtoB(double X, double Y,double X2, double Y2, CDC* pDC)
{
	pDC->MoveTo(lX(X),lY(Y));
	pDC->LineTo(lX(X2),lY(Y2));
}
void CFigure::LineAtoB2nd(double X, double Y,double X2, double Y2, CDC* pDC)
{
	pDC->MoveTo(lX(X),lY2(Y));
	pDC->LineTo(lX(X2),lY2(Y2));
}
void CFigure::ThinVioletLine(double X, double Y,double X2, double Y2, CDC* pDC)
{
	pDC->SelectObject(m_VIOLETpen);
	pDC->MoveTo(lX(X),lY(Y));
	pDC->LineTo(lX(X2),lY(Y2));
}
void CFigure::ThickRedLine(double X, double Y,double X2, double Y2, CDC* pDC)
{
	pDC->SelectObject(m_RedPen2);
	pDC->MoveTo(lX(X),lY(Y));
	pDC->LineTo(lX(X2),lY(Y2));
}
void CFigure::PutIntVal_Custom_Graph2D(double X, double Y, int NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"Number of Nns=%d", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}

void CFigure::PutVal_p3(double X, double Y, double NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%0.3lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}

void CFigure::PutVal_p5(double X, double Y, double NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%0.5lf", NumToPrint);     
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);
}

void CFigure::PutIntVal(double X, double Y, int NumToPrint, CDC* pDC)
{  
	char strTemp[100];
	sprintf(strTemp,"%d", NumToPrint);   
	int x=(lX(X)-3), y=(int)(lY(Y)-5);
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), strTemp);

}

void CFigure::PutText(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)-3),(int)(lY(Y)-5), ToPrint);
}

void CFigure::PutText_After10(double X, double Y, char ToPrint[], CDC* pDC)
{  
	pDC->TextOut((int)(lX(X)+10),(int)(lY(Y)-5), ToPrint);
}

void CFigure::RectangleAB(double X1, double Y1, double X2, double Y2,CDC* pDC)
{  
	pDC->Rectangle(lX(X1),lY(Y1),lX(X2),lY(Y2));
}

void CFigure::FillSolidRect(int X, int Y,int width_inPixels, int height_inPixels, COLORREF rgb ,CDC* pDC)
{ 
	pDC->FillSolidRect(lX(X)-lX(width_inPixels/2),lY(Y)-lY(height_inPixels/2),width_inPixels,height_inPixels,	rgb);
}

void CFigure::FillSolidRect(double X, double Y,int width_inPixels, int height_inPixels, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y),width_inPixels,height_inPixels,	rgb);
}

void CFigure::FillSolidRect_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels=abs(lX(width_inDegrees)-m_lX0);
	height_inPixels=abs(lY(height_inDegrees)-m_lY0);
	topleftX=lX(cntX)-0.5*width_inPixels;
	topleftY=lY(cntY)-0.5*height_inPixels;
	pDC->FillSolidRect(topleftX,topleftY,width_inPixels,height_inPixels,	rgb);
}
void CFigure::Ellipse_SizeAroundFovia_ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lX(0.5*width_inDegrees )-m_lX0)); //The size of the object is defined around the fovial area of the visual field.
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

void CFigure::ReportBackTopLeftXY(double cntX, double cntY, double width_inDegrees, double height_inDegrees, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels=abs(lX(width_inDegrees)-m_lX0);
	height_inPixels=abs(lY(height_inDegrees)-m_lY0);
	topleftX=lX(cntX)-0.5*width_inPixels;
	topleftY=lY(cntY)-0.5*height_inPixels;
}

void CFigure::ReportBackTopLeftXY(double cntX, double cntY,int WidthInPixel, int HeightInPixel, int &topleftX, int &topleftY)
{ 
	topleftX=lX(cntX)-0.5*WidthInPixel;
	topleftY=lY(cntY)-0.5*HeightInPixel;
}

void CFigure::FillSolidRect_CenteredY(double X, double Y,int width_inPixels, int height_inPixels, COLORREF rgb ,CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y)-lY(height_inPixels/2),width_inPixels,height_inPixels,	rgb);
}
void CFigure::FillSolidRect2_CenteredY(double X, double Y, CDC* pDC)
{  
	pDC->FillSolidRect(lX(X),lY(Y)-4,1,8,	m_BLACK);
}
void CFigure::MySetTick(double X, double Y, CDC* pDC)
{
	 pDC->MoveTo(lX(X),lY(Y));
	 pDC->LineTo(lX(X),lY(Y)-5);
}

void CFigure::ContourRect(float LeftBoundary, float UpperBoundary,float RightBoundary, float LowerBoundary, CDC* pDC)
{  
	 pDC->MoveTo((int)lX(LeftBoundary) ,(int)lY(UpperBoundary)); 
	 pDC->LineTo((int)lX(RightBoundary),(int)lY(UpperBoundary));
	 pDC->LineTo((int)lX(RightBoundary),(int)lY(LowerBoundary));
	 pDC->LineTo((int)lX(LeftBoundary) ,(int)lY(LowerBoundary));
	 pDC->LineTo((int)lX(LeftBoundary) ,(int)lY(UpperBoundary));
}

void CFigure::PolyContour(CPoint *points,int N, CDC* pDC)
{  
	 pDC->Polygon(points, 4);
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
//	while(m_bUsingDC2){Sleep(1); }	m_bUsingDC=1;
	pDC->SetPixel( lX(X),lY(Y),m_DkGreenPen);
//	m_bUsingDC=0;
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
int CFigure::FromDataToScreenX(double X)
{
	return  (lX(X)); //They are in (int) form because it is in screen pixel unit
}
int CFigure::FromDataToScreenY(double Y)
{
	return  (lY(Y)); //They are in (int) form because it is in screen pixel unit
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
   double  stepX,stepY;
   double L,Lo;

	int nSaveDC=pDC->SaveDC();
	pDC->SelectObject(&m_LightGrayPen);

	stepX=(m_dSpanX)/10.;//Get_Var("Input interval :");
	stepY=(m_dSpanY)/10.;//Get_VarXY(50,300,gx()/2,480,"Input interval :");
   //stepX=m_dMaxX/10.;//Get_Var("Input interval :");
   //stepY=m_dMaxY/10.;//Get_Var("Input interval :");
	//pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Rectangle((int)(m_nTx+m_dGapX),(int)(m_nTy+m_dGapY),(int)(m_nBx-m_dGapX+1),(int)(m_nBy-m_dGapY+1));
   //----------------------------------Horizontal lines
   for(L=m_dMinY;L<m_dMaxY;L+= stepY){
	   Lo=m_nBy - (L-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY-m_dGapY;
	   pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo);
	   pDC->LineTo((int)(m_nBx-m_dGapX+1),(int)Lo);
   }
   //----------------------------Vertical lines
   for(L=m_dMinX;L<m_dMaxX;L+=stepX){
	   Lo=(L-m_dMinX)*m_dDx_DoubleGapX_Per_Max_MinX+m_dGapX+m_nTx;
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
}


void CFigure::RefreshPane( CDC* pobjDC, CDC* pm_memDC)
{
	//pobjDC->BitBlt(lX(m_dMinX),lY(m_dMaxY),lX(m_dSpanX),lY(m_dSpanY),pm_memDC,lX(m_dMinX),lY(m_dMaxY),SRCCOPY);
	pobjDC->BitBlt(
		(int)(m_nTx+m_dGapX),
		(int)(m_nTy+m_dGapY),
		(int)((m_nBx-m_dGapX+1)-(m_nTx+m_dGapX)),
		(int)((m_nBy-m_dGapY)-(m_nTy+m_dGapY)+1),
		pm_memDC,(int)(m_nTx+m_dGapX),(int)(m_nTy+m_dGapY),SRCCOPY);
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


#include "Figure_XY_VIS_Replica.h"


#undef Dx
#undef Dy
#undef DoubleGapX
#undef DoubleGapY
#undef Dy_DoubleGapY
#undef Dx_DoubleGapX

