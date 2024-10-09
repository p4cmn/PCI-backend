#ifndef PCISERVICE_H
#define PCISERVICE_H

#include <vector>
#include <windows.h>
#include "model/PciDevice.h"
#include "database/PciDatabase.h"
#include "PciDefinitions.h"

using std::vector;

class PciService {

private:
  HANDLE hDevice;
  const PciDatabase &database;

public:
  PciService(HANDLE hDevice, const PciDatabase &database);
  vector<PciDevice> getPciDevices() const;

private:
  ULONG readPciConfig(ULONG bus, ULONG slot, ULONG function, ULONG offset) const;
};

#endif // PCISERVICE_H
