
#include "stdafx.h"
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

// Set up pixel format for graphics initialization
void CAnalysisView::SetupPixelFormat()
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 16;
    ppfd->cDepthBits = 16;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    pixelformat = ChoosePixelFormat(m_hDC, ppfd);
    SetPixelFormat(m_hDC, pixelformat, ppfd);
}

// Initialize OpenGL graphics
void CAnalysisView::InitGraphics()
{
    m_hDC = ::GetDC(m_hWnd);

    SetupPixelFormat();

    m_hRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hRC);


    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glClearColor(0.9, 0.9, 0.9, 0.5);
	glClearDepth(1.0);
	glDepthRange (0,1);
    glEnable(GL_DEPTH_TEST);

	//////
    //glScissor(0,500,500,500);
    //glEnable(GL_SCISSOR_TEST);
}

#define	SCREEN_MARGIN 0//100
// Resize graphics to fit window
void CAnalysisView::ResizeGraphics()
{
    // Get new window size
    RECT rect;
	int width, height;
	GLfloat aspect;

    GetClientRect(&rect);
    width = rect.right-SCREEN_MARGIN;
    height = rect.bottom-SCREEN_MARGIN;
    aspect = (GLfloat)width / height;

    // Adjust graphics to window size
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fovy/*how far from the origin*/, aspect, m_Znear, m_Zfar);
    glMatrixMode(GL_MODELVIEW);
}

// Draw frame
void CAnalysisView::DrawGraphics(double angle)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set location in front of camera
    glLoadIdentity();
    glTranslated(0, 0, -10);
    glRotated(angle,0, 1, 0);

     // Draw a square
	glColor3d(0.8, 0.2, 0.2);
	glLineWidth(12);

    glBegin(GL_LINE_STRIP); //Connect dots
    glVertex3d(-2, 2, 2);
    glVertex3d(2, 2, 2);
    glVertex3d(2, -2, 2);
    glVertex3d(-2, -2, 2);
    glVertex3d(-2, 2, 2);
	glEnd();
   //// Draw an empty square
    //glBegin(GL_LINES); //Two paired points to a line
    //glVertex3d(-2, 2, 2);
    //glVertex3d(2, 2, 2);
    //glVertex3d(2, -2, 2);
    //glVertex3d(2, 2, 2);
    //glVertex3d(2, -2, 2);
    //glVertex3d(-2, -2, 2);
    //glVertex3d(-2, -2, 2);
    //glVertex3d(-2, 2, 2);
    //glEnd();
    //// Draw a square
    //glBegin(GL_QUADS);
    //glColor3d(0.8, 0.2, 0.2);
    //glVertex3d(-2, 2, 2);
    //glVertex3d(2, 2, 2);
    //glVertex3d(2, -2, 2);
    //glVertex3d(-2, -2, 2);
    //glEnd();
    // Show the new scene
    SwapBuffers(m_hDC);
}


