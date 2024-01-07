// pre compile
#include "pch.h"
// project
#include "DeviceTool.h"
// common
#include <Windows.h>

namespace device {
namespace tool {
std::wstring upper(const std::wstring &str) {
  std::wstring data;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      data += str[i] - 'a' + 'A';
    } else {
      data += str[i];
    }
  }
  return data;
}

bool CPnp::device_match() {
  if (device_type() == device_type_) {
    return true;
  }
  return false;
}

DeviceType CPnp::device_type() {
  if (uuid_.find(L"PNP") != std::wstring::npos) {
    std::wstring description = upper(description_);
    if (description.find(L"LPT") != std::wstring::npos) {
      return kLPT;
    } else if (description.find(L"COM") != std::wstring::npos) {
      return kCom;
    }
  }
  return kDefault;
}

bool CCom::device_match() {
  if (device_type() == kCom) {
    return true;
  }
  return false;
}

bool CLPT::device_match() {
  if (device_type() == kLPT) {
    return true;
  }
  return false;
}

bool CUsb::device_match() {
  if (0 == _wcsnicmp(uuid_.c_str(), L"PCI", 3) ||
      0 == _wcsnicmp(uuid_.c_str(), L"USB", 3)) {
    return true;
  }
  return false;
}

bool CNet::device_match() {
  if (device_type_ == kNetwork) {
    if (0 == _wcsnicmp(uuid_.c_str(), L"PCI", 3) ||
        0 == _wcsnicmp(uuid_.c_str(), L"USB", 3)) {
      return true;
    }
  }
  return false;
}

bool CNet::IsWirelessNetWork() {
  bool result = false;
  HKEY hDeviceKey = NULL;
  HKEY hKey = NULL;
  std::wstring registry_path = L"SYSTEM\\CurrentControlSet\\Enum\\" + uuid_;
  if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, registry_path.c_str(), 0, KEY_READ,
                    &hKey) == ERROR_SUCCESS) {
    WCHAR szDriverPath[MAX_PATH];
    DWORD dwSize = sizeof(szDriverPath);
    if (RegQueryValueExW(hKey, TEXT("Driver"), NULL, NULL,
                         (LPBYTE)&szDriverPath, &dwSize) == ERROR_SUCCESS) {
      std::wstring driver_path = L"SYSTEM\\CurrentControlSet\\Control\\Class\\";
      driver_path += szDriverPath;

      RegOpenKeyExW(HKEY_LOCAL_MACHINE, driver_path.c_str(), 0, KEY_READ,
                    &hDeviceKey);
    }
    RegCloseKey(hKey);
  }

  if (hDeviceKey != NULL) {
    WCHAR szInstanceId[MAX_PATH];
    DWORD dwSize = sizeof(szInstanceId);
    if (RegQueryValueExW(hDeviceKey, TEXT("NetCfgInstanceId"), NULL, NULL,
                         (LPBYTE)szInstanceId, &dwSize) == ERROR_SUCCESS) {
      std::wstring hKeyPath =
          L"SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-"
          L"BFC1-08002BE10318}\\";
      hKeyPath += szInstanceId;
      hKeyPath += L"\\Connection";
      if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, hKeyPath.c_str(), 0, KEY_READ,
                        &hKey) == ERROR_SUCCESS) {
        DWORD dwMediaSubType = 0;
        DWORD dwSize = sizeof(dwMediaSubType);
        if (RegQueryValueExW(hKey, L"MediaSubType", NULL, NULL,
                             (LPBYTE)&dwMediaSubType,
                             &dwSize) == ERROR_SUCCESS &&
            dwMediaSubType == 0x02) {
          result = true;
        }
        qDebug << "NetAdapter " << uuid_ << ", type = " << dwMediaSubType;
        RegCloseKey(hKey);
      }
    }
    RegCloseKey(hDeviceKey);
  }

  return result;
}

DeviceType CNet::device_type() {
  if (0 == _wcsnicmp(uuid_.c_str(), L"PCI", 3)) {
    if (IsWirelessNetWork()) {
      return kWirelessNetwork;
    } else {
      return kWiredNetwork;
    }
  } else if (0 == _wcsnicmp(uuid_.c_str(), L"USB", 3)) {
    if (IsWirelessNetWork()) {
      return kWirelessNetwork;
    } else {
      return kUsbWiredNetwork;
    }
  } else if (0 == _wcsnicmp(uuid_.c_str(),
                            L"{5D624F94-8850-40C3-A3FA-A4FD2080BAF3}", 38)) {
    return kHotSpot;
  } else if (0 == _wcsnicmp(uuid_.c_str(), L"BTH", 3)) {
    return kBluetoothNetwork;
  } else {
    return kVirtualNetwork;
  }
  return kDefault;
}

bool CBluetooth::device_match() {
  if (0 == _wcsnicmp(uuid_.c_str(), L"USB", 3)) {
    return true;
  }
  return false;
}

bool CCdrom::device_match() {
  std::wstring uuid = upper(uuid_);
  if (uuid.find(L"CDROM") != std::wstring::npos) {
    return true;
  }
  return false;
}

bool CFloppy::device_match() {
  if (0 == _wcsnicmp(uuid_.c_str(), L"FDC", 3)) {
    return true;
  }
  return false;
}

bool CWiredNetWork::device_match() {
  DeviceType device_type = CNet::device_type();
  if (device_type == kWiredNetwork || device_type == kUsbWiredNetwork) {
    return true;
  }
  return false;
}

bool CWirelessNetwork::device_match() {
  if (CNet::device_type() == kWirelessNetwork) {
    return true;
  }
  return false;
}

bool CHotSpot::device_match() {
  if (CNet::device_type() == kHotSpot) {
    return true;
  }
  return false;
}

bool CVirtualNetwork::device_match() {
  if (CNet::device_type() == kVirtualNetwork) {
    return true;
  }
  return false;
}

bool CBluetoothNetwork::device_match() {
  if (CNet::device_type() == kBluetoothNetwork) {
    return true;
  }
  return false;
}

bool CUsbWiredNetwork::device_match() {
  if (CNet::device_type() == kUsbWiredNetwork) {
    return true;
  }
  return false;
}

DeviceInterface *DeviceFactory::GetDeviceInstance(DeviceType device_type,
                                                  std::wstring uuid,
                                                  std::wstring description) {
  switch (device_type) {
    case kCom:
      return new CCom(uuid, description);
    case kLPT:
      return new CLPT(uuid, description);
    case kNetwork:
      return new CNet(kNetwork, uuid);
    case kUsb:
      return new CUsb(uuid);
    case kBlueTooth:
      return new CBluetooth(uuid);
    case kCdRom:
      return new CCdrom(uuid);
    case kFloppy:
      return new CFloppy(uuid);
    case kWiredNetwork:
      return new CWiredNetWork(uuid);
    case kWirelessNetwork:
      return new CWirelessNetwork(uuid);
    case kHotSpot:
      return new CHotSpot(uuid);
    case kVirtualNetwork:
      return new CVirtualNetwork(uuid);
    case kBluetoothNetwork:
      return new CBluetoothNetwork(uuid);
    case kUsbWiredNetwork:
      return new CUsbWiredNetwork(uuid);
    default:
      break;
  }
  return NULL;
}

};  // namespace tool
}  // namespace device