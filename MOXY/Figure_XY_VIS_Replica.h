
//Tangential scale
#define lXvis(X)  ((int)(m_ScreenScale*(tan((double)( (X)*0.0174532925))) +m_ScreenCenterX ))//1 degree == 0.0174532925 radians
#define lYvis(Y)  ((int)(m_ScreenScale*(tan((double)(-(Y)*0.0174532925))) +m_ScreenCenterY ))//1 degree == 0.0174532925 radians

void CFigure::Prepare4VisReplica()
{
	m_ScreenCenterX=(m_normTx+m_normBx)*0.5;
	m_ScreenCenterY=(m_normTy+m_normBy)*0.5;
	m_lXvis0=lXvis(0);m_lYvis0=lYvis(0);//These saved lXvis(0), lYvis(0) will be used to calculate width and height of some measure later.
	m_ScreenScale = (lX(m_dMaxX)-m_ScreenCenterX)/tan((double)( m_dMaxX*0.0174532925)); // lXvis(m_dMaxX) == lX(m_dMaxX)
}
void CFigure::FillSolidRect_SizeAroundFovia_ReportBackTopLeftXY_VisReplica(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lXvis(0.5*width_inDegrees )-m_lXvis0)); //The size of the object is defined around the fovial area of the visual field.
	height_inPixels=abs(2*(lYvis(0.5*height_inDegrees)-m_lYvis0));
	topleftX=lXvis(cntX)-0.5*width_inPixels;
	topleftY=lYvis(cntY)-0.5*height_inPixels;
	pDC->FillSolidRect(topleftX,topleftY,width_inPixels,height_inPixels,	rgb);
}
void CFigure::FillSolidRect_ReportBackTopLeft_Eye(double cntX, double cntY, double EyeSquareWidth, double EyeSquareheight, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY)
{  
	topleftX=lXvis(cntX)-0.5*EyeSquareWidth;
	topleftY=lYvis(cntY)-0.5*EyeSquareheight;
	pDC->FillSolidRect(topleftX,topleftY,EyeSquareWidth,EyeSquareheight,	rgb);
}

void CFigure::Ellipse_SizeAroundFovia_ReportBackTopLeftXY_VisReplica(double cntX, double cntY, double width_inDegrees, double height_inDegrees, COLORREF rgb ,CDC* pDC, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lXvis(0.5*width_inDegrees )-m_lXvis0)); //The size of the object is defined around the fovial area of the visual field.
	height_inPixels=abs(2*(lYvis(0.5*height_inDegrees)-m_lYvis0));
	topleftX=lXvis(cntX)-0.5*width_inPixels;
	topleftY=lYvis(cntY)-0.5*height_inPixels;

	LOGBRUSH LogBrh;
	LogBrh.lbStyle=BS_SOLID;
	LogBrh.lbColor=rgb;
	m_TmpBrush.CreateBrushIndirect( &LogBrh );			
	pDC->SelectObject(&m_TmpBrush);
	pDC->Ellipse(topleftX,topleftY,topleftX+width_inPixels,topleftY+height_inPixels);
	m_TmpBrush.DeleteObject();
}
void CFigure::ReportBackTopLeftXY_Eye(double cntX, double cntY,int EyeSquareWidth, int EyeSquareheight, int &topleftX, int &topleftY)
{ 
	topleftX=lXvis(cntX)-0.5*EyeSquareWidth;
	topleftY=lYvis(cntY)-0.5*EyeSquareheight;
}

void CFigure::ReportBackTopLeftXY_VisReplica(double cntX, double cntY, double width_inDegrees, double height_inDegrees, int &topleftX, int &topleftY, int &width_inPixels, int &height_inPixels)
{  
	width_inPixels =abs(2*(lXvis(0.5*width_inDegrees )-m_lXvis0));
	height_inPixels=abs(2*(lYvis(0.5*height_inDegrees)-m_lYvis0));
	topleftX=lXvis(cntX)-0.5*width_inPixels;
	topleftY=lYvis(cntY)-0.5*height_inPixels;
}

double CFigure::FromScreenToDataX_VisReplica(int screenX)
{
	return atan((screenX-m_ScreenCenterX)/m_ScreenScale)/0.0174532925;
}
double CFigure::FromScreenToDataY_VisReplica(int screenY)
{
	return -atan((screenY-m_ScreenCenterY)/m_ScreenScale)/0.0174532925;
}

void CFigure::ContourRect_VisReplica(float LeftBoundary, float UpperBoundary,float RightBoundary, float LowerBoundary, CDC* pDC)
{  
	 pDC->MoveTo((int)lXvis(LeftBoundary) ,(int)lYvis(UpperBoundary)); 
	 pDC->LineTo((int)lXvis(RightBoundary),(int)lYvis(UpperBoundary));
	 pDC->LineTo((int)lXvis(RightBoundary),(int)lYvis(LowerBoundary));
	 pDC->LineTo((int)lXvis(LeftBoundary) ,(int)lYvis(LowerBoundary));
	 pDC->LineTo((int)lXvis(LeftBoundary) ,(int)lYvis(UpperBoundary));
}


//void CFigure::PlotPolarMash( CDC* pDC)  
//{
//   double  stepX,stepY, StartX,StartY,labelValue;
//   double L,Lo;
//   int	labelPosX,labelPosY;
//   char charVarTemp[64];
//
//	int nSaveDC=pDC->SaveDC();
//	pDC->SelectObject(&m_GrayPen);
//
//	if(m_dSpanX<=2)stepX=0.2;
//	else if(m_dSpanX<=10)stepX=1;
//	else if(m_dSpanX<=20)stepX=2;	
//	else if(m_dSpanX<=50)stepX=5;
//	else if(m_dSpanX<=100)stepX=10;
//	else stepX=20;
//	stepY=stepX;
//	CBrush *pOldBrush=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
// 	//---------------------------Horizontal lines
//	for(L=stepY;L<m_dMaxY;L+= stepY){
//		Lo=m_nBy - (L-m_dMinY)*m_dDy_DoubleGapY_Per_Max_MinY-m_dGapY;
//		labelValue=L;
//		if(0.0001>fabs(labelValue-Rounded_to_nearest_Int(labelValue))){sprintf(charVarTemp, "%.0lf", labelValue );}
//		else{sprintf(charVarTemp, "%.1lf", labelValue );}
//		labelPosX=(int)(m_nTx+m_FontWidth); labelPosY=(int)(Lo)-5; 
//		pDC->TextOut(labelPosX,labelPosY ,charVarTemp);
//		pDC->Ellipse(lXvis(-L),lYvis(L),lXvis(L),lYvis(-L));
//	}
//	pDC->MoveTo(lXvis(m_dMinX),m_lY0); pDC->LineTo(lXvis(m_dMaxX),m_lY0); 
//	pDC->MoveTo(m_lX0,lYvis(m_dMinY)); pDC->LineTo(m_lX0,lYvis(m_dMaxY)); 
//	pDC->MoveTo(lXvis(m_dMinX),lYvis(m_dMinY)); pDC->LineTo(lXvis(m_dMaxX),lYvis(m_dMaxY)); 
//	pDC->MoveTo(lXvis(m_dMinX),lYvis(m_dMaxY)); pDC->LineTo(lXvis(m_dMaxX),lYvis(m_dMinY)); 
//
//	pDC->SelectObject(pOldBrush);
//	pDC->RestoreDC(nSaveDC); //Before deleting the registered resource, you should unregister the resource using RestoreDC(). 
//}

void CFigure::PlotGrid(int Mode, CDC* pDC)  
{
   double  stepX,stepY, StartX,StartY,labelValue;
   double L,Lo;
   int	labelPosX,labelPosY;
   char charVarTemp[64];
   CBrush *pOldBrush;

	int nSaveDC=pDC->SaveDC();
	pDC->SelectObject(&m_GrayPen);

	if(m_dSpanX<=2)stepX=0.2;
	else if(m_dSpanX<=10)stepX=1;
	else if(m_dSpanX<=20)stepX=2;	
	else if(m_dSpanX<=50)stepX=5;
	else if(m_dSpanX<=100)stepX=10;
	else stepX=20;
	stepY=stepX;
	for(int i=1;i<1000;i++){
		if(i*stepX>m_dMaxX){StartX=-(i-1)*stepX; break;}
	}
	StartY=StartX;
	pOldBrush=pDC->SelectObject(&m_BlackBrush);
 	//---------------------------Horizontal lines
	for(L=StartY;L<m_dMaxY;L+= stepY){
		if(Mode==EYE_MONITOR_MODE)Lo=lYvis(L);
		else Lo=lY(L);
		labelValue=L;
		if(0.0001>fabs(labelValue-Rounded_to_nearest_Int(labelValue))){sprintf(charVarTemp, "%.0lf", labelValue );}
		else{sprintf(charVarTemp, "%.1lf", labelValue );}
		labelPosX=(int)(m_nTx+m_FontWidth); labelPosY=(int)(Lo)-5; 
		pDC->TextOut(labelPosX,labelPosY ,charVarTemp);

		pDC->MoveTo((int)(m_nTx+m_dGapX),(int)Lo);
		pDC->LineTo((int)(m_nBx-m_dGapX+1),(int)Lo);
	}
	//---------------------------Vertical lines
	for(L=StartX;L<m_dMaxX;L+=stepX){
		if(Mode==EYE_MONITOR_MODE)Lo=lXvis(L);
		else Lo=lX(L);
		pDC->MoveTo((int)Lo,(int)(m_nTy+m_dGapY));
		pDC->LineTo((int)Lo,(int)(m_nBy-m_dGapY));
	}
	pDC->RestoreDC(nSaveDC); //Before deleting the registered resource, you should unregister the resource using RestoreDC(). 
	pDC->SelectObject(&pOldBrush);
}