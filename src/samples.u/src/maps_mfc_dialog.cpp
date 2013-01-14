// mfc_dialog.cpp : Defines the initialization routines for the DLL.
//

#include "maps_mfc_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfc_dialogApp

BEGIN_MESSAGE_MAP(CMfc_dialogApp, CWinApp)
	//{{AFX_MSG_MAP(CMfc_dialogApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfc_dialogApp construction

CMfc_dialogApp::CMfc_dialogApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMfc_dialogApp object

CMfc_dialogApp theApp;
