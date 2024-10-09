#ifndef PCIDEVICE_H
#define PCIDEVICE_H

#include <string>
#include <windows.h>

using std::string;

class PciDevice {

private:
  ULONG bus, slot, function;
  ULONG vendorId, deviceId, subVendorId, subDeviceId;
  string vendorName, deviceName, subsystemName;

public:
  PciDevice(ULONG bus,
            ULONG slot,
            ULONG function,
            ULONG vendorId,
            ULONG deviceId,
            ULONG subVendorId,
            ULONG subDeviceId,
            const string &vendorName = "N/A",
            const string &deviceName = "N/A",
            const string &subsystemName = "N/A");

  // Геттеры
  ULONG getBus() const;
  ULONG getSlot() const;
  ULONG getFunction() const;
  ULONG getVendorId() const;
  ULONG getDeviceId() const;
  ULONG getSubVendorId() const;
  ULONG getSubDeviceId() const;
  string getVendorName() const;
  string getDeviceName() const;
  string getSubsystemName() const;

  // Сеттеры
  void setBus(ULONG bus);
  void setSlot(ULONG slot);
  void setFunction(ULONG function);
  void setVendorId(ULONG vendorId);
  void setDeviceId(ULONG deviceId);
  void setSubVendorId(ULONG subVendorId);
  void setSubDeviceId(ULONG subDeviceId);
  void setVendorName(const string &vendorName);
  void setDeviceName(const string &deviceName);
  void setSubsystemName(const string &subsystemName);
};

#endif // PCIDEVICE_H
