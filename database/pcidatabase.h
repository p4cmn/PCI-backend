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
using std::vector;
using std::mutex;

class PciDatabase {
private:
  map<ULONG, string> vendorMap;
  map<pair<ULONG, ULONG>, string> deviceMap;
  map<tuple<ULONG, ULONG, ULONG, ULONG>, string> subsystemMap;
  map<ULONG, string> classMap;
  map<tuple<ULONG, ULONG>, string> subclassMap;
  map<tuple<ULONG, ULONG, ULONG>, string> progIfMap;

  mutable mutex mapMutex;

  vector<vector<string>> splitFileIntoBlocks(const vector<string>& lines);

  void parseFileChunk(const vector<string>& lines, ULONG& currentVendorID,
                      map<ULONG, string>& localVendorMap,
                      map<pair<ULONG, ULONG>, string>& localDeviceMap,
                      map<tuple<ULONG, ULONG, ULONG, ULONG>, string>& localSubsystemMap,
                      map<ULONG, string>& localClassMap,
                      map<tuple<ULONG, ULONG>, string>& localSubClassMap,
                      map<tuple<ULONG, ULONG, ULONG>, string>& localProgIfMap);

  void parseVendor(const string& line, ULONG& currentVendorID, map<ULONG, string>& localVendorMap);
  void parseDevice(const string& line, ULONG currentVendorID, ULONG& currentDeviceID, map<pair<ULONG, ULONG>, string>& localDeviceMap);
  void parseClass(const string& line, ULONG& currentClassID, map<ULONG, string>& localClassMap);
  void parseSubClass(const string& line, ULONG currentClassID, ULONG& currentSubClassID, map<tuple<ULONG, ULONG>, string>& localSubClassMap);
  void parseProgIf(const string& line, ULONG currentClassID, ULONG currentSubClassID, map<tuple<ULONG, ULONG, ULONG>, string>& localProgIfMap);
  void parseSubsystem(const string& line, ULONG currentVendorID, ULONG currentDeviceID, map<tuple<ULONG, ULONG, ULONG, ULONG>, string>& localSubsystemMap);

public:
  bool loadFromFile(const string& filePath);

  string getVendorName(ULONG vendorId) const;
  string getDeviceName(ULONG vendorId, ULONG deviceId) const;
  string getSubsystemName(ULONG vendorId, ULONG deviceId, ULONG subVendorId, ULONG subDeviceId) const;
  string getClassName(ULONG classCode) const;
  string getSubClassName(ULONG classCode, ULONG subClassCode) const;
  string getProgIfName(ULONG classCode, ULONG subClassCode, ULONG progIfCode) const;
};

#endif // PCIDATABASE_H
