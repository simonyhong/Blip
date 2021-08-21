// AnalysisDoc.h : interface of the CAnalysisDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISDOC_H__4E7AB3C6_4618_11D3_8364_C9075A873F60__INCLUDED_)
#define AFX_ANALYSISDOC_H__4E7AB3C6_4618_11D3_8364_C9075A873F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAnalysisDoc : public CDocument
{
protected: // create from serialization only
	CAnalysisDoc();
	DECLARE_DYNCREATE(CAnalysisDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalysisDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnalysisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAnalysisDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYSISDOC_H__4E7AB3C6_4618_11D3_8364_C9075A873F60__INCLUDED_)
