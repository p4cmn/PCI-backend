#ifndef PCIDATABASE_H
#define PCIDATABASE_H

#include <map>
#include <tuple>
#include <string>
#include <windows.h>

using std::map;
using std::pair;
using std::tuple;
using std::string;

class PciDatabase {

private:
  map<ULONG, string> vendorMap;
  map<pair<ULONG, ULONG>, string> deviceMap;
  map<tuple<ULONG, ULONG, ULONG>, string> subsystemMap;

public:
  bool loadFromFile(const string &filePath);
  string getVendorName(ULONG vendorId) const;
  string getDeviceName(ULONG vendorId, ULONG deviceId) const;
  string getSubsystemName(ULONG vendorId, ULONG subVendorId, ULONG subDeviceId) const;
};

#endif // PCIDATABASE_H
