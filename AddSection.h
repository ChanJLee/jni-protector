// AddSection.h : main header file for the ADDSECTION application
//

#if !defined(AFX_ADDSECTION_H__7123C0D1_9212_44DF_8A8C_F0245049B742__INCLUDED_)
#define AFX_ADDSECTION_H__7123C0D1_9212_44DF_8A8C_F0245049B742__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAddSectionApp:
// See AddSection.cpp for the implementation of this class
//

class CAddSectionApp : public CWinApp
{
public:
	CAddSectionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddSectionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAddSectionApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSECTION_H__7123C0D1_9212_44DF_8A8C_F0245049B742__INCLUDED_)
