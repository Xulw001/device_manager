
// DeviceManagerDlg.h: header file
//

#pragma once
#include "DeviceController.h"
#include "DeviceMonitor.h"

// CDeviceManagerDlg
class CDeviceManagerDlg : public CDialogEx {
 public:
  CDeviceManagerDlg(CWnd* pParent = nullptr);  //

#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_DEVICEMANAGER_DIALOG };
#endif

 protected:
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV Support

 private:
  void ButtonListener(int btn_idx, device::DeviceType dev_type);
  void OnReceiveRecord(std::wstring record);
  void OnReceiveRecord(device::DeviceType dev_type, const std::wstring& uuid);

 protected:
  HICON m_hIcon;
  device::DeviceController controller_;
  device::DeviceMonitor monitor_;
  std::list<std::wstring> record_;

  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
 public:
  afx_msg void OnClickedCom();
  afx_msg void OnClickedLpt();
  afx_msg void OnClickedNet();
  afx_msg void OnClickedUsb();
  afx_msg void OnClickedBlueTooth();
  afx_msg void OnClickedCdrom();
  afx_msg void OnClickedFloppy();
  afx_msg void OnClickedWiredNet();
  afx_msg void OnClickedWirelessNet();
  afx_msg void OnClickedHotSpotNet();
  afx_msg void OnClickedVirtualNet();
  afx_msg void OnClickedBlueToothNet();
  afx_msg void OnClickedWifi();
  afx_msg LRESULT CALLBACK OnDeviceChange(WPARAM wParam, LPARAM lParam);
};
