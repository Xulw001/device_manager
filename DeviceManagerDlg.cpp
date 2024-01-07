
// DeviceManagerDlg.cpp
//
// pre compile
#include "pch.h"
// common
#include "framework.h"
// project
#include "DeviceManager.h"
#include "DeviceManagerDlg.h"
#include "DeviceMonitor.h"
// common
#include <Dbt.h>
#include <afxdialogex.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDeviceManagerDlg

CDeviceManagerDlg::CDeviceManagerDlg(CWnd *pParent /*=nullptr*/)
    : CDialogEx(IDD_DEVICEMANAGER_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDeviceManagerDlg::DoDataExchange(CDataExchange *pDX) {
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDeviceManagerDlg, CDialogEx)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_CHECK1, &CDeviceManagerDlg::OnClickedCom)
ON_BN_CLICKED(IDC_CHECK2, &CDeviceManagerDlg::OnClickedLpt)
ON_BN_CLICKED(IDC_CHECK3, &CDeviceManagerDlg::OnClickedNet)
ON_BN_CLICKED(IDC_CHECK4, &CDeviceManagerDlg::OnClickedUsb)
ON_BN_CLICKED(IDC_CHECK5, &CDeviceManagerDlg::OnClickedBlueTooth)
ON_BN_CLICKED(IDC_CHECK6, &CDeviceManagerDlg::OnClickedCdrom)
ON_BN_CLICKED(IDC_CHECK7, &CDeviceManagerDlg::OnClickedFloppy)
ON_BN_CLICKED(IDC_CHECK8, &CDeviceManagerDlg::OnClickedWiredNet)
ON_BN_CLICKED(IDC_CHECK9, &CDeviceManagerDlg::OnClickedWirelessNet)
ON_BN_CLICKED(IDC_CHECK10, &CDeviceManagerDlg::OnClickedHotSpotNet)
ON_BN_CLICKED(IDC_CHECK11, &CDeviceManagerDlg::OnClickedVirtualNet)
ON_BN_CLICKED(IDC_CHECK12, &CDeviceManagerDlg::OnClickedBlueToothNet)
ON_BN_CLICKED(IDC_CHECK13, &CDeviceManagerDlg::OnClickedWifi)
ON_MESSAGE(WM_DEVICECHANGE, &CDeviceManagerDlg::OnDeviceChange)
END_MESSAGE_MAP()

// CDeviceManagerDlg

BOOL CDeviceManagerDlg::OnInitDialog() {
  CDialogEx::OnInitDialog();

  SetIcon(m_hIcon, TRUE);   //
  SetIcon(m_hIcon, FALSE);  //

  ::SetWindowPos(m_hWnd, HWND_BOTTOM, 0, 0, 1000, 800,
                 SWP_NOZORDER | SWP_NOMOVE);

  CWnd *pWnd;
  pWnd = GetDlgItem(IDC_STATIC);
  pWnd->SetWindowPos(NULL, 45, 50, 900, 250, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_STATIC1);
  pWnd->SetWindowPos(NULL, 45, 350, 900, 250, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_STATIC2);
  pWnd->SetWindowPos(NULL, 75, 80, 800, 80, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_STATIC3);
  pWnd->SetWindowPos(NULL, 75, 180, 800, 80, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK1);
  pWnd->SetWindowPos(NULL, 100, 110, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK2);
  pWnd->SetWindowPos(NULL, 210, 110, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK3);
  pWnd->SetWindowPos(NULL, 320, 110, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK4);
  pWnd->SetWindowPos(NULL, 430, 110, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK5);
  pWnd->SetWindowPos(NULL, 540, 110, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK6);
  pWnd->SetWindowPos(NULL, 650, 110, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK7);
  pWnd->SetWindowPos(NULL, 760, 110, 75, 30, SWP_NOZORDER);

  //
  pWnd = GetDlgItem(IDC_CHECK8);
  pWnd->SetWindowPos(NULL, 100, 210, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK9);
  pWnd->SetWindowPos(NULL, 210, 210, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK10);
  pWnd->SetWindowPos(NULL, 320, 210, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK11);
  pWnd->SetWindowPos(NULL, 430, 210, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK12);
  pWnd->SetWindowPos(NULL, 540, 210, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_CHECK13);
  pWnd->SetWindowPos(NULL, 650, 210, 75, 30, SWP_NOZORDER);

  pWnd = GetDlgItem(IDC_EDIT2);
  pWnd->SetWindowPos(NULL, 75, 380, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

  if (!monitor_.RegisterDeviceNotication(this->GetSafeHwnd())) {
    qError << "register device notify error = " << GetLastError();
  }

  return TRUE;  //
}

void CDeviceManagerDlg::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this);  //

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()),
                0);

    //
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // paint icon
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialogEx::OnPaint();
  }
}

HCURSOR CDeviceManagerDlg::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

void CDeviceManagerDlg::ButtonListener(int btn_idx,
                                       device::DeviceType dev_type) {
  CButton *pBtn = (CButton *)GetDlgItem(btn_idx);
  WCHAR device_name[MAX_PATH];
  pBtn->GetWindowTextW(device_name, MAX_PATH);
  int state = pBtn->GetCheck();
  if (!controller_.SetDeviceOption(dev_type,
                                   state ? device::Ban : device::Allow)) {
    pBtn->SetCheck(state ? 0 : 1);
  } else {
    CString option;
    option.LoadString(state ? IDS_BAN : IDS_ALLOW);
    std::wstring record(device_name);
    record += option;
    OnReceiveRecord(record);
    qDebug << "Button " << device_name << "clicked!";
  }
}

void CDeviceManagerDlg::OnClickedCom() {
  ButtonListener(IDC_CHECK1, device::kCom);
}

void CDeviceManagerDlg::OnClickedLpt() {
  ButtonListener(IDC_CHECK2, device::kLPT);
}

void CDeviceManagerDlg::OnClickedNet() {
  ButtonListener(IDC_CHECK3, device::kNetwork);
}

void CDeviceManagerDlg::OnClickedUsb() {
  ButtonListener(IDC_CHECK4, device::kUsb);
}

void CDeviceManagerDlg::OnClickedBlueTooth() {
  ButtonListener(IDC_CHECK5, device::kBlueTooth);
}

void CDeviceManagerDlg::OnClickedCdrom() {
  ButtonListener(IDC_CHECK6, device::kCdRom);
}

void CDeviceManagerDlg::OnClickedFloppy() {
  ButtonListener(IDC_CHECK7, device::kFloppy);
}

void CDeviceManagerDlg::OnClickedWiredNet() {
  ButtonListener(IDC_CHECK8, device::kWiredNetwork);
}

void CDeviceManagerDlg::OnClickedWirelessNet() {
  ButtonListener(IDC_CHECK9, device::kWirelessNetwork);
}

void CDeviceManagerDlg::OnClickedHotSpotNet() {
  ButtonListener(IDC_CHECK10, device::kHotSpot);
}

void CDeviceManagerDlg::OnClickedVirtualNet() {
  ButtonListener(IDC_CHECK11, device::kVirtualNetwork);
}

void CDeviceManagerDlg::OnClickedBlueToothNet() {
  ButtonListener(IDC_CHECK12, device::kBluetoothNetwork);
}

void CDeviceManagerDlg::OnClickedWifi() {
  ButtonListener(IDC_CHECK13, device::kUsbWiredNetwork);
}

LRESULT CALLBACK CDeviceManagerDlg::OnDeviceChange(WPARAM wParam,
                                                   LPARAM lParam) {
  if (wParam == DBT_DEVICEARRIVAL) {
    std::wstring uuid;
    device::DeviceType dev_type = monitor_.GetDeviceType(lParam, &uuid);
    if (!controller_.SetDeviceOption(dev_type)) {
      qError << "device dispatch failed, type = " << dev_type;
    }
    OnReceiveRecord(dev_type, uuid);
  }
  return TRUE;
}

void CDeviceManagerDlg::OnReceiveRecord(std::wstring record) {
  if (record_.size() > 10) {
    record_.pop_front();
  }
  record_.push_back(record);

  std::wstringstream ss;
  for (auto it : record_) {
    ss << it << L"\r\n";
  }
  SetDlgItemText(IDC_EDIT2, ss.str().data());
}

void CDeviceManagerDlg::OnReceiveRecord(device::DeviceType dev_type,
                                        const std::wstring &uuid) {
  std::wstring record;
  CString str;
  str.LoadString(IDS_MON);
  record += str;
  switch (dev_type) {
    case device::kCom:
      str.LoadString(IDS_COM);
      break;
    case device::kLPT:
      str.LoadString(IDS_COM + 1);
      break;
    case device::kNetwork:
      str.LoadString(IDS_COM + 2);
      break;
    case device::kUsb:
      str.LoadString(IDS_COM + 3);
      break;
    case device::kBlueTooth:
      str.LoadString(IDS_COM + 4);
      break;
    case device::kCdRom:
      str.LoadString(IDS_COM + 5);
      break;
    case device::kFloppy:
      str.LoadString(IDS_COM + 6);
      break;
    case device::kWiredNetwork:
      str.LoadString(IDS_COM + 7);
      break;
    case device::kWirelessNetwork:
      str.LoadString(IDS_COM + 8);
      break;
    case device::kHotSpot:
      str.LoadString(IDS_COM + 9);
      break;
    case device::kVirtualNetwork:
      str.LoadString(IDS_COM + 10);
      break;
    case device::kBluetoothNetwork:
      str.LoadString(IDS_COM + 11);
      break;
    case device::kUsbWiredNetwork:
      str.LoadString(IDS_COM + 12);
      break;
    default:
      break;
  }
  record += str;
  str.LoadString(IDS_INSERT);
  record += str;
  str.LoadString(IDS_UUID);
  record += str;
  record += uuid;

  OnReceiveRecord(record);
}