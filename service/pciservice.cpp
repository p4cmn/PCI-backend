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
	ULONG subVendorID = readPciConfig(bus, slot, function, 0x2C) & 0xFFFF;
	ULONG subDeviceID = (readPciConfig(bus, slot, function, 0x2C) >> 16) & 0xFFFF;

	string vendorName = database.getVendorName(vendorID);
	string deviceName = database.getDeviceName(vendorID, deviceID);
	string subsystemName = database.getSubsystemName(vendorID, subVendorID, subDeviceID);

	PciDevice device(bus, slot, function, vendorID, deviceID, subVendorID, subDeviceID, vendorName, deviceName, subsystemName);
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
