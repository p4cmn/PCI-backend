#ifndef PCIDEVICE_H
#define PCIDEVICE_H

#include <string>
#include <windows.h>

using std::string;

class PciDevice {

private:
  ULONG bus, slot, function;
  ULONG vendorId, deviceId, subVendorId, subDeviceId;
  ULONG classCode, subclassCode, progIf;
  string vendorName, deviceName, subsystemName;
  string className, subclassName, progIfName;

public:
  PciDevice(ULONG bus,
            ULONG slot,
            ULONG function,
            ULONG vendorId,
            ULONG deviceId,
            ULONG subVendorId,
            ULONG subDeviceId,
            ULONG classCode,
            ULONG subclassCode,
            ULONG progIf,
            const string &vendorName = "N/A",
            const string &deviceName = "N/A",
            const string &subsystemName = "N/A",
            const string &className = "N/A",
            const string &subclassName = "N/A",
            const string &progIfName = "N/A");

  ULONG getBus() const;
  ULONG getSlot() const;
  ULONG getFunction() const;
  ULONG getVendorId() const;
  ULONG getDeviceId() const;
  ULONG getSubVendorId() const;
  ULONG getSubDeviceId() const;
  ULONG getClassCode() const;
  ULONG getSubClassCode() const;
  ULONG getProgIf() const;
  string getVendorName() const;
  string getDeviceName() const;
  string getSubsystemName() const;
  string getClassName() const;
  string getSubClassName() const;
  string getProgIfName() const;

  void setBus(ULONG bus);
  void setSlot(ULONG slot);
  void setFunction(ULONG function);
  void setVendorId(ULONG vendorId);
  void setDeviceId(ULONG deviceId);
  void setSubVendorId(ULONG subVendorId);
  void setSubDeviceId(ULONG subDeviceId);
  void setClassCode(ULONG classCode);
  void setSubClassCode(ULONG subclassCode);
  void setProgIf(ULONG progIf);
  void setVendorName(const string &vendorName);
  void setDeviceName(const string &deviceName);
  void setSubsystemName(const string &subsystemName);
  void setClassName(const string &className);
  void setSubClassName(const string &subclassName);
  void setProgIfName(const string &progIfName);
};

#endif // PCIDEVICE_H
