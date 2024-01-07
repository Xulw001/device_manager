#pragma once
// project
#include "DeviceTool.h"
// common library
#include <unordered_map>

using std::unordered_map;
namespace device {
enum Option {
  Allow = 0,
  Ban = 1,
  Default = 2,
};

class DeviceController {
 public:
  bool SetDeviceOption(DeviceType dev_type, int option = Default);
  ~DeviceController() { Reset(); }

 private:
  bool Dispatcher(DeviceType dev_type, int option);
  void Reset();

 private:
  unordered_map<DeviceType, int> device_config_;
};
}  // namespace device