#pragma once
#include <string>

namespace device {
enum DeviceType {
  kCom,
  kLPT,
  kNetwork,
  kUsb,
  kBlueTooth,
  kCdRom,
  kFloppy,

  kWiredNetwork = 0x10,
  kWirelessNetwork,
  kHotSpot,
  kVirtualNetwork,
  kBluetoothNetwork,
  kUsbWiredNetwork,

  kDefault = 0xFF,
};

namespace tool {
class DeviceInterface {
 public:
  DeviceInterface(DeviceType device_type, std::wstring uuid)
      : device_type_(device_type), uuid_(uuid) {
    ;
  }

 public:
  virtual bool device_match() = 0;

 protected:
  DeviceType device_type_;
  std::wstring uuid_;
};

class DeviceFactory {
 public:
  static DeviceInterface *GetDeviceInstance(DeviceType device_type,
                                            std::wstring uuid,
                                            std::wstring description);
};

class CPnp : public DeviceInterface {
 public:
  CPnp(DeviceType device_type, std::wstring uuid, std::wstring description)
      : DeviceInterface(device_type, uuid), description_(description) {
    ;
  }
  virtual bool device_match();
  virtual DeviceType device_type();

 public:
  std::wstring description_;
};

class CCom : public CPnp {
 public:
  CCom(std::wstring uuid, std::wstring description)
      : CPnp(kCom, uuid, description) {
    ;
  }
  virtual bool device_match();
};

class CLPT : public CPnp {
 public:
  CLPT(std::wstring uuid, std::wstring description)
      : CPnp(kLPT, uuid, description) {
    ;
  }
  virtual bool device_match();
};

class CNet : public DeviceInterface {
 public:
  CNet(DeviceType device_type, std::wstring uuid)
      : DeviceInterface(device_type, uuid) {
    ;
  }

  virtual bool device_match();
  virtual DeviceType device_type();

 private:
  bool IsWirelessNetWork();
};

class CUsb : public DeviceInterface {
 public:
  CUsb(std::wstring uuid) : DeviceInterface(kUsb, uuid) { ; }
  virtual bool device_match();
};

class CBluetooth : public DeviceInterface {
 public:
  CBluetooth(std::wstring uuid) : DeviceInterface(kBlueTooth, uuid) { ; }
  virtual bool device_match();
};

class CCdrom : public DeviceInterface {
 public:
  CCdrom(std::wstring uuid) : DeviceInterface(kCdRom, uuid) { ; }
  virtual bool device_match();
};

class CFloppy : public DeviceInterface {
 public:
  CFloppy(std::wstring uuid) : DeviceInterface(kFloppy, uuid) { ; }
  virtual bool device_match();
};

class CWiredNetWork : public CNet {
 public:
  CWiredNetWork(std::wstring uuid) : CNet(kWiredNetwork, uuid) { ; }
  virtual bool device_match();
};

class CWirelessNetwork : public CNet {
 public:
  CWirelessNetwork(std::wstring uuid) : CNet(kWirelessNetwork, uuid) { ; }
  virtual bool device_match();
};

class CHotSpot : public CNet {
 public:
  CHotSpot(std::wstring uuid) : CNet(kHotSpot, uuid) { ; }
  virtual bool device_match();
};

class CVirtualNetwork : public CNet {
 public:
  CVirtualNetwork(std::wstring uuid) : CNet(kVirtualNetwork, uuid) { ; }
  virtual bool device_match();
};

class CBluetoothNetwork : public CNet {
 public:
  CBluetoothNetwork(std::wstring uuid) : CNet(kBluetoothNetwork, uuid) { ; }
  virtual bool device_match();
};

class CUsbWiredNetwork : public CNet {
 public:
  CUsbWiredNetwork(std::wstring uuid) : CNet(kUsbWiredNetwork, uuid) { ; }
  virtual bool device_match();
};
}  // namespace tool
}  // namespace device