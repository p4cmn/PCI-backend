#include <fstream>
#include <sstream>
#include <iostream>
#include "database/PciDatabase.h"

bool PciDatabase::loadFromFile(const string &filePath) {
  using namespace std;
  ifstream file(filePath);
  if (!file.is_open()) {
    cerr << "Unable to open pci.ids file." << endl;
    return false;
  }

  string line;
  ULONG currentVendorID = 0;

  while (getline(file, line)) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    if (line[0] != '\t') {
      istringstream ss(line);
      ss >> hex >> currentVendorID;
      string vendorName;
      getline(ss, vendorName);
      vendorName.erase(0, vendorName.find_first_not_of(" \t"));
      vendorMap[currentVendorID] = vendorName;
    } else if (line[1] != '\t') {
      istringstream ss(line.substr(1));
      ULONG deviceID;
      ss >> hex >> deviceID;
      string deviceName;
      getline(ss, deviceName);
      deviceName.erase(0, deviceName.find_first_not_of(" \t"));
      deviceMap[{currentVendorID, deviceID}] = deviceName;
    } else {
      istringstream ss(line.substr(2));
      ULONG subVendorID, subDeviceID;
      ss >> hex >> subVendorID >> subDeviceID;
      string subsystemName;
      getline(ss, subsystemName);
      subsystemName.erase(0, subsystemName.find_first_not_of(" \t"));
      subsystemMap[{currentVendorID, subVendorID, subDeviceID}] = subsystemName;
    }
  }
  return true;
}

std::string PciDatabase::getVendorName(ULONG vendorId) const {
  auto it = vendorMap.find(vendorId);
  return it != vendorMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getDeviceName(ULONG vendorId, ULONG deviceId) const {
  auto it = deviceMap.find({vendorId, deviceId});
  return it != deviceMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getSubsystemName(ULONG vendorId, ULONG subVendorId, ULONG subDeviceId) const {
  auto it = subsystemMap.find({vendorId, subVendorId, subDeviceId});
  return it != subsystemMap.end() ? it->second : "N/A";
}
