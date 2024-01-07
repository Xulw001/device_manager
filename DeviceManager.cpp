
// DeviceManager.cpp
//

// pre compile
#include "pch.h"
// common
#include "framework.h"
// project
#include "DeviceManager.h"
#include "DeviceManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDeviceManagerApp

BEGIN_MESSAGE_MAP(CDeviceManagerApp, CWinApp)
ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CDeviceManagerApp

CDeviceManagerApp::CDeviceManagerApp() {}

CDeviceManagerApp theApp;

// CDeviceManagerApp

BOOL CDeviceManagerApp::InitInstance() {
  CWinApp::InitInstance();

  CShellManager *pShellManager = new CShellManager;

  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

  SetRegistryKey(_T("DeviceManager"));

  CDeviceManagerDlg dlg;
  m_pMainWnd = &dlg;
  INT_PTR nResponse = dlg.DoModal();
  if (nResponse == IDOK) {
  } else if (nResponse == IDCANCEL) {
  } else if (nResponse == -1) {
    qWarning << "create dialog failed, application throw exception";
  }

  //
  if (pShellManager != nullptr) {
    delete pShellManager;
  }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
  ControlBarCleanUp();
#endif

  return FALSE;
}
