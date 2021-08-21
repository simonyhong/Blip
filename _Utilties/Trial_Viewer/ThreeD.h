/* Read Me:

  It Provides the following interface:
	void	TwoD();
	int		ThreeD(double* iE, int gValue, int InitLine, LPVOID  objView, int disp, int ScreenX);
	void    KeyDown(UINT) ;
	void	Stop();
*/

// ThreeD.h : interface of the CGraphView class
//
/////////////////////////////////////////////////////////////////////////////

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<afxtempl.h>
//-----------------------------  Constants --------------------------------------
#define ENTER 0x000d
#define UP    0x4800
#define DOWN  0x5000
#define LEFT  0x4b00
#define RIGHT 0x4d00
#define BkSpace 0x0008
#define ESC   0x001b  //d27
#define PgUp  0x2100
#define PgDn  0x2200
#define F1    0x3b00
#define F2    0x3c00
#define HOME  0x4700
#define End   0x4f00
#define TOLERANCE 1000
#define Scl   100
#define EXIT  3

#define WHOLE_NET     0
#define NO_CONNECTION 1
#define SIMULATION	0
#define GRAPH_3D_STOP	0
#define GRAPH_2D	2
#define GRAPH_3D	3
#define BK_COLOR 0
#define PAINT_COLOR 15
#define DOT			0
#define LINE		1
#define NETWORK		2
//----------------------------------------   My_Math.h --------------------------------------------------
#define Sq2D(x,y)   ((x)*(x)+(y)*(y))
#define Sq3D(x,y,z) ((x)*(x)+(y)*(y)+(z)*(z))
#define M3D(x,y,z)  sqrt((x)*(x)+(y)*(y)+(z)*(z))
#define M2D(x,y)    sqrt(((x)*(x)+(y)*(y)))
//-----------------------------------------  My_3D.h  --------------------------------------------------
#define MAX_MEM 10000

class CMo:public CObject 
{
public:
	CPoint point; 
	int  color;
	CMo();
	virtual ~CMo();
};

//######################################  2D  #######################################################
class CTwoD : public CObject
{
public:
	int		TwoD();
	CTwoD();
	virtual ~CTwoD();
protected:
	int		m_nShowMode;
};

class CAnalysisView;
//######################################  3D  #######################################################
class CThreeD : public CObject
{
public:
	CAnalysisView* m_pAnalysisView;
	int		ThreeD (double* , int , int , LPVOID , int );
	int		ThreeD2(double* , int , int , LPVOID , int ,int );
	int		ThreeD3(int , int , LPVOID , int);
	int		ThreeD4(double* , int , int , LPVOID , int ,int );
	void    KeyDown(UINT) ;
	void    Stop();
	CThreeD();
	virtual ~CThreeD();


//	double  coalpha,cozeta,cocsi;

	double  ObjCntrRotX,ObjCntrRotY,ObjCntrRotZ;
	
	/////////// Observer 1  /////////////
	double  ObserverCntrX,ObserverCntrY,ObserverCntrZ;
	double  ObserverCntrRotX,ObserverCntrRotY,ObserverCntrRotZ;//Viewer1's direction vector;
	double  preRot_EgoCtrcCoordt_SrfcPnt[3],EgoCtrcCoordt_SrfcPnt[3];

	/////////// Observer 2  /////////////
	double  Observer2CntrX,Observer2CntrY,Observer2CntrZ;
	double  Observer2CntrRotX,Observer2CntrRotY,Observer2CntrRotZ;//Viewer2's direction vector;
	
	double  LtSourceX,LtSourceY,LtSourceZ,ObjCntrX,ObjCntrY,ObjCntrZ;
	int     x0[10],y0[10],x1[10],y1[10];// Light  -->these are used in Erease()
	double  axis[4][3];				 //Coord. axis 
	CPoint   AxisPoint[4];			//Coord. axis  -->these are used in Erease()
	double  Scl_Y,Scl_X,Scl_Z;
	int     m_nScreenOriginX;
	int     m_nScreenOriginY;
	
	// Object rotation along its own center
	double  e11,e12,e13;
	double  e21,e22,e23;
	double  e31,e32,e33;

	//Viewer rotation 
	double  ev11,ev12,ev13;
	double  ev21,ev22,ev23;
	double  ev31,ev32,ev33;


	int     c,old_c,h;
	//double  *ie;//,*e;
	////////////////////////
	BOOL	m_bIsON;
	int		m_nShowMode;
	int		m_nDrawingMode;
	int     m_nKey; // 
	int CHOICE;
	int     g, m_nInitLine;
	double  m_dW;
	double	m_dFar;
	int		m_nColorShow;
	CRect	m_rectShow;
	double  Divide(double x,double y){double i; i=(y==0)?0:(x/y); return i;}
	void    Working(CClientDC*, CClientDC*);         
	void    VisibleArea();						     
	void    Formulas(int );							
	void    Parameters(int );			   	       
	void	SimpleParameters(int Key);
	void	ParametersForObserver2();
	void    ObjectRotation();							   
	void    ObserverRotation();	
	void	IsoMapping(int*, int*);
	void    ThreeD_to_2D_Mapping();							   
	void    Erase(CClientDC*);					
	void    Show(CClientDC*);                      
	void    CoordinateAxis(CClientDC*);				
	CArray<double, double& > ie, e; 
	CArray<CMo,CMo&>         mon;
	CArray<CMo,CMo&>         old_mon;

	//////////////////////////////////////////	

	void    Working2(CClientDC*, CClientDC*);         
	void    VisibleArea2();						     
	void    ObjectRotation2();							 
	void    ObserverRotation2();							 
	void    Mapping2();							     
	void    Erase2(CClientDC*);						 
	void    Show2(CClientDC*);
	void    Show3(CClientDC*);
	void    Show30(CClientDC*);
	//double	Init_ObjCntrd_Coordnt_SrfcPoint[MAX_MEM][3], ObjCntrd_Coordnt_SrfcPoint[MAX_MEM][3];
	double	ObjCntrd_Coordnt_SrfcPoint[MAX_MEM][3];
	CMo		MON[MAX_MEM];
	//////////////////////////////////////////
};

