#include "service/PciService.h"
#include <iostream>

PciService::PciService(HANDLE hDevice, const PciDatabase &database)
    : hDevice(hDevice), database(database) {}

vector<PciDevice> PciService::getPciDevices() const {
  vector<PciDevice> devices;
  for (ULONG bus = 0; bus < 256; ++bus) {
    for (ULONG slot = 0; slot < 32; ++slot) {
      for (ULONG function = 0; function < 8; ++function) {
        ULONG vendorID = readPciConfig(bus, slot, function, 0x00) & 0xFFFF;
        if (vendorID == 0xFFFF) {
          continue;
        }
        ULONG deviceID = (readPciConfig(bus, slot, function, 0x00) >> 16) & 0xFFFF;
        if (deviceID == 0xFFFF) {
          deviceID = 0xFFFFFFFF;
        }
        ULONG subVendorID = readPciConfig(bus, slot, function, 0x2C) & 0xFFFF;
        if (subVendorID == 0xFFFF) {
          subVendorID = 0xFFFFFFFF;
        }
        ULONG subDeviceID = (readPciConfig(bus, slot, function, 0x2C) >> 16) & 0xFFFF;
        if (subDeviceID == 0xFFFF) {
          subDeviceID = 0xFFFFFFFF;
        }
        ULONG classCode = (readPciConfig(bus, slot, function, 0x08) >> 24) & 0xFF;
        if (classCode == 0xFF) {
          classCode = 0xFFFFFFFF;
        }
        ULONG subclassCode = (readPciConfig(bus, slot, function, 0x08) >> 16) & 0xFF;
        if (subclassCode == 0xFF) {
          subclassCode = 0xFFFFFFFF;
        }
        ULONG progIf = (readPciConfig(bus, slot, function, 0x08) >> 8) & 0xFF;
        if (progIf == 0xFF) {
          progIf = 0xFFFFFFFF;
        }
        string vendorName = database.getVendorName(vendorID);
        string deviceName = database.getDeviceName(vendorID, deviceID);
        string subsystemName = database.getSubsystemName(vendorID, deviceID, subVendorID, subDeviceID);
        string className = database.getClassName(classCode);
        string subclassName = database.getSubClassName(classCode, subclassCode);
        string progIfName = database.getProgIfName(classCode, subclassCode, progIf);

	PciDevice device(bus, slot, function, vendorID, deviceID, subVendorID, subDeviceID,
			 classCode, subclassCode, progIf, vendorName, deviceName, subsystemName,
			 className, subclassName, progIfName);
	devices.push_back(device);
      }
    }
  }

  return devices;
}

ULONG PciService::readPciConfig(ULONG bus, ULONG slot, ULONG function, ULONG offset) const {
  PCIRequest request = { bus, slot, function, offset };
  ULONG outputData = 0;
  DWORD bytesReturned = 0;

  BOOL result = DeviceIoControl(
      hDevice,
      IOCTL_READ_PCI_CONFIG,
      &request,
      sizeof(PCIRequest),
      &outputData,
      sizeof(ULONG),
      &bytesReturned,
      NULL
      );
  if (!result) {
    std::cerr << "DeviceIoControl failed with error: " << GetLastError() << std::endl;
    return 0xFFFFFFFF;
  }
  return outputData;
}
