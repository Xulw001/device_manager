// pre compile
#include "pch.h"
// project
#include "DeviceMonitor.h"

// common
#include <Dbt.h>
#include <initguid.h>
#include <ndisguid.h>
#include <usbiodef.h>
#include <winioctl.h>

namespace device {
using tool::DeviceFactory;
using tool::DeviceInterface;

bool DeviceMonitor::RegisterDeviceNotication(HWND hWnd) {
  GUID dev_guids[4] = {GUID_DEVINTERFACE_NET, GUID_DEVINTERFACE_CDROM,
                       GUID_DEVINTERFACE_FLOPPY, GUID_DEVINTERFACE_USB_DEVICE};
  for (int i = 0; i < 4; i++) {
    DEV_BROADCAST_DEVICEINTERFACE di = {0};
    di.dbcc_size = sizeof(di);
    di.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    di.dbcc_classguid = dev_guids[i];
    h_devnotify_[i] =
        RegisterDeviceNotification(hWnd, &di, DEVICE_NOTIFY_WINDOW_HANDLE);
    if (h_devnotify_[i] == NULL) {
      UnRegisterDeviceNotication();
      qError << "RegisterDeviceNotification failed! " << GetLastError();
      return false;
    }
  }
  return true;
}

DeviceType DeviceMonitor::GetDeviceType(LPARAM lParam, std::wstring *p_uuid) {
  PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
  DeviceType dev_type = kDefault;
  if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
    PDEV_BROADCAST_DEVICEINTERFACE pDev = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
    std::wstring uuid;
    GetDeviceInstanceId(pDev->dbcc_name, &uuid);
    if (memcmp(&pDev->dbcc_classguid, &GUID_DEVINTERFACE_NET,
               sizeof(GUID_DEVINTERFACE_NET)) == 0) {
      dev_type = kNetwork;
    } else if (memcmp(&pDev->dbcc_classguid, &GUID_DEVINTERFACE_CDROM,
                      sizeof(GUID_DEVINTERFACE_CDROM)) == 0) {
      dev_type = kCdRom;
    } else if (memcmp(&pDev->dbcc_classguid, &GUID_DEVINTERFACE_FLOPPY,
                      sizeof(GUID_DEVINTERFACE_FLOPPY)) == 0) {
      dev_type = kFloppy;
    } else if (memcmp(&pDev->dbcc_classguid, &GUID_DEVINTERFACE_USB_DEVICE,
                      sizeof(GUID_DEVINTERFACE_USB_DEVICE)) == 0) {
      dev_type = kUsb;
    } else {
      return dev_type;
    }
    DeviceInterface *p_dev =
        DeviceFactory::GetDeviceInstance(dev_type, uuid, L"");
    if (p_dev) {
      if (p_dev->device_match()) {
        if (dev_type == kNetwork) {
          dev_type = ((tool::CNet *)p_dev)->device_type();
        }
      }
      delete p_dev;
    }
    p_uuid->assign(uuid);
  }
  return dev_type;
}

void DeviceMonitor::GetDeviceInstanceId(const std::wstring &dev_path,
                                        std::wstring *uuid) {
  if (wcsncmp(dev_path.c_str(), L"\\\\?\\", 4)) return;

  for (size_t pos = dev_path.size() - 1; pos >= 0; pos--) {
    if (dev_path[pos] == '#') {
      if (dev_path[pos + 1] == '{' &&
          dev_path[pos + 1 + sizeof("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX")] ==
              '}') {
        for (int i = 4; i < pos; i++) {
          if (dev_path[i] == '#') {
            uuid->operator+=('\\');
          } else {
            uuid->operator+=(dev_path[i]);
          }
        }
        qInfo << "Device Arrival:" << *uuid;
        return;
      }
    }
  }
}

void DeviceMonitor::InitDeviceNotication() {
  for (auto it : h_devnotify_) {
    it = NULL;
  };
}

void DeviceMonitor::UnRegisterDeviceNotication() {
  for (auto it : h_devnotify_) {
    if (it) {
      if (!::UnregisterDeviceNotification(it)) {
        qError << "UnregisterDeviceNotification failed! " << GetLastError();
      }
    }
  }
}
}  // namespace device