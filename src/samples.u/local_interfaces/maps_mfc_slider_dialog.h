#if !defined(AFX_SLIDER_DIALOG_H__B83E4A13_A197_4F37_8AD9_370E72D88A09__INCLUDED_)
#define AFX_SLIDER_DIALOG_H__B83E4A13_A197_4F37_8AD9_370E72D88A09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// slider_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// slider_dialog dialog

class slider_dialog : public CDialog
{
// Construction
public:
	slider_dialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(slider_dialog)
	enum { IDD = IDD_PROPPAGE_MEDIUM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(slider_dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(slider_dialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDER_DIALOG_H__B83E4A13_A197_4F37_8AD9_370E72D88A09__INCLUDED_)
