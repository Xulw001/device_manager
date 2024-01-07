
// DeviceManager.h: PROJECT_NAME 
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include this file after include 'pch.h' to PCH"
#endif

#include "resource.h"		// 


// CDeviceManagerApp:
// 
//

class CDeviceManagerApp : public CWinApp
{
public:
	CDeviceManagerApp();

// overwrite
public:
	virtual BOOL InitInstance();

// 

	DECLARE_MESSAGE_MAP()
};

extern CDeviceManagerApp theApp;
