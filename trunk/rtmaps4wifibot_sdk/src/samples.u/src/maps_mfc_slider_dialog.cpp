// maps_slider_dialog.cpp : implementation file
//

#include "maps_mfc_dialog.h"
#include "maps_mfc_slider_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// slider_dialog dialog


slider_dialog::slider_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(slider_dialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(slider_dialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void slider_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(slider_dialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(slider_dialog, CDialog)
	//{{AFX_MSG_MAP(slider_dialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// slider_dialog message handlers
