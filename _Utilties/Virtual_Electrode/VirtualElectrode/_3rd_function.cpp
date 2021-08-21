#include "stdafx.h"
#include "Analysis.h"

#include<math.h>

#include "AnalysisDoc.h"
#include "Figure.h"
#include "AnalysisView.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include  "MainFrm.h"

//#############################   CORONAL Slice Display ##############################
void CAnalysisView::Show2D_III() 
{
	if(m_isCoronal_Or_Sagittal!=-1 && *M_isCoronal_Running==0){ //The current program is running a function already; yet, the function is not the one that this function that the user wants ==> create another program to run this function
		Launch_Coronal_Or_Sagittal("Coronal");
		SetEvent(M_KeyDownEvents[1]);
		return;
	}
	if(*M_isCoronal_Running==1)return;

	//////////// Implementation Below ///////////
	//////////// Implementation Below ///////////

	//Pick a directory only when it is the first one to run.
	if(*M_is3D_Running==0 && *M_isSagittal_Running==0){
		if(ReadSavedFolderPath(m_FolderPath)!=0){
			SetCurrentDirectory( m_FolderPath );
		}
		if(Pick_Directory()==0){m_bIsON=0; return;}
		strcpy(M_DataDir,m_FolderPath);
		SaveFolderPath(m_FolderPath);
	}

	m_isCoronal_Or_Sagittal=CORONAL;
	//################################################
	_2nd_InteractiveMRI();
	return;
}
