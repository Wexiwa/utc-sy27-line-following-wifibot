// maps_mfc_dialog.h : main header file for the MFC_DIALOG DLL
//

#if !defined(AFX_MFC_DIALOG_H__F9FD4901_00EF_43E1_91BA_402F01F47A0E__INCLUDED_)
#define AFX_MFC_DIALOG_H__F9FD4901_00EF_43E1_91BA_402F01F47A0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MFC core and standard components
#include <afxwin.h>

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// main symbols
#include "maps_mfc_resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMfc_dialogApp
// See mfc_dialog.cpp for the implementation of this class
//

class CMfc_dialogApp : public CWinApp
{
public:
	CMfc_dialogApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfc_dialogApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMfc_dialogApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_DIALOG_H__F9FD4901_00EF_43E1_91BA_402F01F47A0E__INCLUDED_)
