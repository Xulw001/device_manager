#pragma once
#include <Windows.h>

#include "DeviceTool.h"
namespace device {
class DeviceMonitor {
 public:
  DeviceMonitor() { InitDeviceNotication(); }
  ~DeviceMonitor() { UnRegisterDeviceNotication(); }

  bool RegisterDeviceNotication(HWND hWnd);
  DeviceType GetDeviceType(LPARAM lParam, std::wstring *uuid);

 private:
  void InitDeviceNotication();
  void UnRegisterDeviceNotication();
  void GetDeviceInstanceId(const std::wstring &dev_path, std::wstring *uuid);

 private:
  HDEVNOTIFY h_devnotify_[4];
};
}  // namespace device