// VIS.h : main header file for the ANALYSIS application
//

#if !defined(AFX_ANALYSIS_H__4E7AB3C0_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_ANALYSIS_H__4E7AB3C0_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVISApp:
// See VIS.cpp for the implementation of this class
//

class CVISApp : public CWinApp
{
public:
	CVISApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVISApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CVISApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSIS_H__4E7AB3C0_4618_11D3_8364_C9075A873F60__INCLUDED_)
