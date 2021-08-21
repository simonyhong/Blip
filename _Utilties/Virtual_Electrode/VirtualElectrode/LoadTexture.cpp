#include "stdafx.h"
#include "Analysis.h"

#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "SinTable.h"
#include "VarDefinitions.h"

int CAnalysisView::LoadTextureBMP( const char * filename, int wrap )
{
    unsigned int texture;
    int i,j,Mat_Sizexi, sizofMem;
#define MAX_WIDTH  200
#define MAX_HEIGHT  200
	BYTE data[MAX_WIDTH*MAX_WIDTH*3];

    // read texture data
	CClientDC objDC(this);
	HBITMAP hBmp;	BITMAP bmp;  	HDC comDC;   
	COLORREF  rgb;
	hBmp=(HBITMAP)LoadImage(0,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	GetObject(hBmp,sizeof(bmp),&bmp);	
	if((bmp.bmWidth>0 && bmp.bmHeight>0) && !(bmp.bmWidth>MAX_WIDTH || bmp.bmHeight>MAX_HEIGHT)){//Only when the loading was successful.
		comDC=CreateCompatibleDC(objDC);
		SelectObject(comDC,hBmp);
		m_imgWidth=bmp.bmWidth; m_imgHeight=bmp.bmHeight;
		DeleteObject(hBmp);
		for(i=0;i<m_imgHeight;i++){
			Mat_Sizexi=m_imgWidth*i;
			for(j=0;j<m_imgWidth;j++){ 
				rgb=GetPixel(comDC,j,m_imgHeight-1-i);
				data[(Mat_Sizexi+j)*3+0]=(rgb&0xFF);
				data[(Mat_Sizexi+j)*3+1]=((rgb>>8)&0xFF);
				data[(Mat_Sizexi+j)*3+2]=((rgb>>16)&0xFF);
		}	}
	}
	else{
		if(!(bmp.bmWidth>0 && bmp.bmHeight>0))AfxMessageBox("!(bmp.bmWidth>0 && bmp.bmHeight>0)");
		if(bmp.bmWidth>MAX_WIDTH || bmp.bmHeight>MAX_HEIGHT)AfxMessageBox("bmp.bmWidth>MAX_WIDTH || bmp.bmHeight>MAX_HEIGHT");
		return -1;
	}

    // allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP );

    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, m_imgWidth, m_imgHeight, GL_RGB, GL_UNSIGNED_BYTE, data );

    return texture;
}

