#include "PciDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <mutex>

void PciDatabase::parseVendor(const std::string& line, ULONG& currentVendorID) {
  std::istringstream ss(line);
  ss >> std::hex >> currentVendorID;
  std::string vendorName;
  std::getline(ss, vendorName);
  vendorName.erase(0, vendorName.find_first_not_of(" \t"));
  vendorMap[currentVendorID] = vendorName;
}

void PciDatabase::parseDevice(const std::string& line, ULONG currentVendorID, ULONG& currentDeviceID) {
  std::istringstream ss(line.substr(1));
  ULONG deviceID;
  ss >> std::hex >> deviceID;
  std::string deviceName;
  std::getline(ss, deviceName);
  deviceName.erase(0, deviceName.find_first_not_of(" \t"));
  deviceMap[{currentVendorID, deviceID}] = deviceName;
  currentDeviceID = deviceID;  // Запоминаем deviceID для subsystem
}

void PciDatabase::parseClass(const std::string& line, ULONG& currentClassID) {
  std::istringstream ss(line.substr(1));
  ss >> std::hex >> currentClassID;
  std::string className;
  std::getline(ss, className);
  className.erase(0, className.find_first_not_of(" \t"));
  classMap[currentClassID] = className;
}

void PciDatabase::parseSubClass(const std::string& line, ULONG currentClassID, ULONG& currentSubClassID) {
  std::istringstream ss(line.substr(1));
  ULONG subClassID;
  ss >> std::hex >> subClassID;
  std::string subClassName;
  std::getline(ss, subClassName);
  subClassName.erase(0, subClassName.find_first_not_of(" \t"));
  subclassMap[{currentClassID, subClassID}] = subClassName;
  currentSubClassID = subClassID;  // Запоминаем для progIf
}


void PciDatabase::parseProgIf(const std::string& line, ULONG currentSubClassID) {
  std::istringstream ss(line.substr(2));
  ULONG progIfID;
  ss >> std::hex >> progIfID;
  std::string progIfName;
  std::getline(ss, progIfName);
  progIfName.erase(0, progIfName.find_first_not_of(" \t"));
  progIfMap[{currentSubClassID, progIfID}] = progIfName;
}


void PciDatabase::parseSubsystem(const std::string& line, ULONG currentVendorID, ULONG currentDeviceID) {
  std::istringstream ss(line.substr(2));
  ULONG subVendorID, subDeviceID;
  ss >> std::hex >> subVendorID >> subDeviceID;
  std::string subsystemName;
  std::getline(ss, subsystemName);
  subsystemName.erase(0, subsystemName.find_first_not_of(" \t"));
  subsystemMap[{currentVendorID, currentDeviceID, subVendorID, subDeviceID}] = subsystemName;
}

void PciDatabase::parseFileChunk(const std::vector<std::string>& lines, ULONG& currentVendorID) {
  ULONG currentClassID = 0;
  ULONG currentDeviceID = 0;
  ULONG currentSubClassID = 0;
  for (const auto& line : lines) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    if (line[0] != '\t') {
      if (line[0] == 'C') {
        parseClass(line, currentClassID);
      } else {
        parseVendor(line, currentVendorID);
        currentDeviceID = 0;
      }
    } else if (line[1] != '\t') {
      if (currentClassID > 0) {
        parseSubClass(line, currentClassID, currentSubClassID);
      } else {
        parseDevice(line, currentVendorID, currentDeviceID);
      }
    } else {
      if (currentClassID > 0) {
        parseProgIf(line, currentSubClassID);
      } else if (currentDeviceID > 0) {
        parseSubsystem(line, currentVendorID, currentDeviceID);
      }
    }
  }
}

bool PciDatabase::loadFromFile(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Unable to open pci.ids file." << std::endl;
    return false;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  file.close();

  if (lines.empty()) {
    std::cerr << "File is empty or could not be read." << std::endl;
    return false;
  }

  ULONG currentVendorID = 0;
  parseFileChunk(lines, currentVendorID);

  std::cout << "PCI database loaded successfully." << std::endl;
  return true;
}

std::string PciDatabase::getVendorName(ULONG vendorId) const {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto it = vendorMap.find(vendorId);
  return it != vendorMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getDeviceName(ULONG vendorId, ULONG deviceId) const {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto it = deviceMap.find({vendorId, deviceId});
  return it != deviceMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getSubsystemName(ULONG vendorId, ULONG deviceId, ULONG subVendorId, ULONG subDeviceId) const {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto it = subsystemMap.find({vendorId, deviceId, subVendorId, subDeviceId});
  return it != subsystemMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getClassName(ULONG classCode) const {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto it = classMap.find(classCode);
  return it != classMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getSubClassName(ULONG classCode, ULONG subClassCode) const {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto it = subclassMap.find({classCode, subClassCode});
  return it != subclassMap.end() ? it->second : "N/A";
}

std::string PciDatabase::getProgIfName(ULONG classCode, ULONG progIfCode) const {
  std::lock_guard<std::mutex> lock(mapMutex);
  auto it = progIfMap.find({classCode, progIfCode});
  return it != progIfMap.end() ? it->second : "N/A";
}
