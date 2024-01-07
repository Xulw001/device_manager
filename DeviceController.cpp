// pre compile
#include "pch.h"
// project
#include "DeviceController.h"
#include "DeviceMonitor.h"
// common library
#include <SetupAPI.h>
#include <devguid.h>
#include <initguid.h>
#pragma comment(lib, "Setupapi.lib")
#include <vector>

namespace device {
using tool::DeviceFactory;
using tool::DeviceInterface;

bool DeviceController::SetDeviceOption(DeviceType dev_type, int option) {
  if (dev_type == kDefault) return true;
  auto it = device_config_.find(dev_type);
  if (it == device_config_.end()) {
    option = (option == Default) ? Allow : Ban;
    if (Dispatcher(dev_type, option))
      device_config_.emplace(dev_type, option);
    else
      return false;
  } else {
    if (option == Default) {
      return Dispatcher(dev_type, it->second);
    }
    if (it->second != option) {
      if (Dispatcher(dev_type, option))
        it->second = option;
      else
        return false;
    }
  }
  return true;
}

HDEVINFO GetDeviceHandle(DeviceType dev_type) {
  HDEVINFO handle = INVALID_HANDLE_VALUE;
  switch (dev_type) {
    case kCom:
    case kLPT:
      handle =
          SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
      break;
    case kNetwork:
    case kWiredNetwork:
    case kWirelessNetwork:
    case kHotSpot:
    case kVirtualNetwork:
    case kBluetoothNetwork:
    case kUsbWiredNetwork:
      handle = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, NULL);
      break;
    case kUsb:
      handle = SetupDiGetClassDevs(&GUID_DEVCLASS_USB, NULL, NULL, NULL);
      break;
    case kBlueTooth:
      handle = SetupDiGetClassDevs(&GUID_DEVCLASS_BLUETOOTH, NULL, NULL, NULL);
      break;
    case kCdRom:
      handle = SetupDiGetClassDevs(&GUID_DEVCLASS_CDROM, NULL, NULL, NULL);
      break;
    case kFloppy:
      handle = SetupDiGetClassDevs(&GUID_DEVCLASS_FLOPPYDISK, NULL, NULL,
                                   DIGCF_PRESENT);
      break;
    default:
      qError << "unhandled type!";
      return false;
  }
  return handle;
}

bool ControlDevice(HDEVINFO handle, SP_DEVINFO_DATA &dev_data, int option) {
  SP_PROPCHANGE_PARAMS params;
  params.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
  params.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
  params.StateChange = option ? DICS_DISABLE : DICS_ENABLE;
  params.Scope = DICS_FLAG_CONFIGSPECIFIC;
  params.HwProfile = 0;
  if (!SetupDiSetClassInstallParams(
          handle, &dev_data, &params.ClassInstallHeader, sizeof(params)) ||
      !SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, handle, &dev_data)) {
    // SetupDiCallClassInstaller maybe failed by 0xE000020b, ignore
    DWORD dErr = GetLastError();
    if (dErr != ERROR_NO_SUCH_DEVINST) {
      qError << "device config failed, err = " << dErr;
      return false;
    }
  }
  return true;
}

bool DeviceController::Dispatcher(DeviceType dev_type, int option) {
  HDEVINFO handle = GetDeviceHandle(dev_type);
  if (handle == INVALID_HANDLE_VALUE) return false;

  DWORD dev_index = 0;
  std::vector<SP_DEVINFO_DATA> dev_list;
  while (true) {
    SP_DEVINFO_DATA dev_data;
    dev_data.cbSize = sizeof(SP_DEVINFO_DATA);
    if (SetupDiEnumDeviceInfo(handle, dev_index, &dev_data) == false) {
      break;
    }

    DWORD len = MAX_PATH;
    wchar_t buffer[MAX_PATH] = {};
    if (SetupDiGetDeviceInstanceId(handle, &dev_data, buffer, len, &len) ==
        TRUE) {
      std::wstring uuid = buffer;
      std::wstring description;
      DWORD regDataType;
      DWORD len = MAX_PATH;
      if (SetupDiGetDeviceRegistryProperty(handle, &dev_data,
                                           SPDRP_FRIENDLYNAME, &regDataType,
                                           (PBYTE)buffer, len, &len)) {
        description = buffer;
      }

      DeviceInterface *dev =
          DeviceFactory::GetDeviceInstance(dev_type, uuid, description);
      if (dev) {
        if (dev->device_match()) {
          qDebug << "find device " << uuid << " : " << dev_type;
          dev_list.emplace_back(dev_data);
        }
        delete dev;
      }
    }
    dev_index++;
  }

  bool ret = true;
  for (auto it = dev_list.begin(); it != dev_list.end(); it++) {
    if (!ControlDevice(handle, *it, option)) {
      for (auto data = dev_list.begin(); data != it; data++) {
        ControlDevice(handle, *data, option ? Allow : Ban);
      }
      ret = false;
      break;
    }
  }
  SetupDiDestroyDeviceInfoList(handle);
  return ret;
}

void DeviceController::Reset() {
  for (auto it : device_config_) {
    if (!Dispatcher(it.first, Allow)) {
      qError << "Reset Device failed! type = " << it.first;
    }
  }
}
}  // namespace device