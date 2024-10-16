#ifndef PCIDATABASE_H
#define PCIDATABASE_H

#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <windows.h>
#include <mutex>

using std::map;
using std::pair;
using std::string;
using std::tuple;

class PciDatabase {
private:
  map<ULONG, string> vendorMap;
  map<pair<ULONG, ULONG>, string> deviceMap;
  map<tuple<ULONG, ULONG, ULONG, ULONG>, string> subsystemMap;
  map<ULONG, string> classMap;
  map<pair<ULONG, ULONG>, string> subclassMap;
  map<pair<ULONG, ULONG>, string> progIfMap;

  mutable std::mutex mapMutex;

  void parseFileChunk(const std::vector<std::string>& lines, ULONG& currentVendorID);

  void parseVendor(const std::string& line, ULONG& currentVendorID);
  void parseDevice(const std::string& line, ULONG currentVendorID, ULONG& currentDeviceID);
  void parseClass(const std::string& line, ULONG& currentClassID);
  void parseSubClass(const std::string& line, ULONG currentClassID, ULONG& currentSubClassID);
  void parseProgIf(const std::string& line, ULONG currentSubClassID);
  void parseSubsystem(const std::string& line, ULONG currentVendorID, ULONG currentDeviceID);

public:
  bool loadFromFile(const string& filePath);

  string getVendorName(ULONG vendorId) const;
  string getDeviceName(ULONG vendorId, ULONG deviceId) const;
  string getSubsystemName(ULONG vendorId, ULONG deviceId, ULONG subVendorId, ULONG subDeviceId) const;
  string getClassName(ULONG classCode) const;
  string getSubClassName(ULONG classCode, ULONG subClassCode) const;
  string getProgIfName(ULONG classCode, ULONG progIfCode) const;
};

#endif // PCIDATABASE_H
