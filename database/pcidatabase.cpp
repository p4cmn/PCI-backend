#include "PciDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

vector<vector<string>> PciDatabase::splitFileIntoBlocks(const vector<string>& lines) {
  vector<vector<string>> blocks;
  vector<string> currentBlock;
  for (const auto& line : lines) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    if (line[0] != '\t') {
      if (!currentBlock.empty()) {
        blocks.push_back(currentBlock);
        currentBlock.clear();
      }
    }
    currentBlock.push_back(line);
  }
  if (!currentBlock.empty()) {
    blocks.push_back(currentBlock);
  }
  return blocks;
}

void PciDatabase::parseVendor(const string& line, ULONG& currentVendorID, map<ULONG, string>& localVendorMap) {
  std::istringstream ss(line);
  ss >> std::hex >> currentVendorID;
  string vendorName;
  std::getline(ss, vendorName);
  vendorName.erase(0, vendorName.find_first_not_of(" \t"));
  localVendorMap[currentVendorID] = vendorName;
}

void PciDatabase::parseDevice(const string& line, ULONG currentVendorID, ULONG& currentDeviceID, map<pair<ULONG, ULONG>, string>& localDeviceMap) {
  std::istringstream ss(line.substr(1));
  ULONG deviceID;
  ss >> std::hex >> deviceID;
  string deviceName;
  std::getline(ss, deviceName);
  deviceName.erase(0, deviceName.find_first_not_of(" \t"));
  localDeviceMap[{currentVendorID, deviceID}] = deviceName;
  currentDeviceID = deviceID;  // Запоминаем deviceID для subsystem
}

void PciDatabase::parseClass(const string& line, ULONG& currentClassID, map<ULONG, string>& localClassMap) {
  std::istringstream ss(line.substr(1));
  ss >> std::hex >> currentClassID;
  string className;
  std::getline(ss, className);
  className.erase(0, className.find_first_not_of(" \t"));
  localClassMap[currentClassID] = className;
}

void PciDatabase::parseSubClass(const string& line, ULONG currentClassID, ULONG& currentSubClassID, map<tuple<ULONG, ULONG>, string>& localSubClassMap) {
  std::istringstream ss(line.substr(1));
  ULONG subClassID;
  ss >> std::hex >> subClassID;
  string subClassName;
  std::getline(ss, subClassName);
  subClassName.erase(0, subClassName.find_first_not_of(" \t"));
  localSubClassMap[{currentClassID, subClassID}] = subClassName;
  currentSubClassID = subClassID;  // Запоминаем для progIf
}

void PciDatabase::parseProgIf(const string& line, ULONG currentClassID, ULONG currentSubClassID, map<tuple<ULONG, ULONG, ULONG>, string>& localProgIfMap) {
  std::istringstream ss(line.substr(2));
  ULONG progIfID;
  ss >> std::hex >> progIfID;
  string progIfName;
  std::getline(ss, progIfName);
  progIfName.erase(0, progIfName.find_first_not_of(" \t"));
  localProgIfMap[{currentClassID, currentSubClassID, progIfID}] = progIfName;
}

void PciDatabase::parseSubsystem(const string& line, ULONG currentVendorID, ULONG currentDeviceID, map<tuple<ULONG, ULONG, ULONG, ULONG>, string>& localSubsystemMap) {
  std::istringstream ss(line.substr(2));
  ULONG subVendorID, subDeviceID;
  ss >> std::hex >> subVendorID >> subDeviceID;
  string subsystemName;
  std::getline(ss, subsystemName);
  subsystemName.erase(0, subsystemName.find_first_not_of(" \t"));
  localSubsystemMap[{currentVendorID, currentDeviceID, subVendorID, subDeviceID}] = subsystemName;
}

void PciDatabase::parseFileChunk(const vector<string>& lines, ULONG& currentVendorID,
				 map<ULONG, string>& localVendorMap,
				 map<pair<ULONG, ULONG>, string>& localDeviceMap,
				 map<tuple<ULONG, ULONG, ULONG, ULONG>, string>& localSubsystemMap,
				 map<ULONG, string>& localClassMap,
				 map<tuple<ULONG, ULONG>, string>& localSubClassMap,
				 map<tuple<ULONG, ULONG, ULONG>, string>& localProgIfMap) {

  ULONG currentClassID = 0;
  ULONG currentDeviceID = 0;
  ULONG currentSubClassID = 0;

  for (const auto& line : lines) {
    if (line.empty() || line[0] == '#') {
      continue;
    }

    if (line[0] != '\t') {
      if (line[0] == 'C') {
        parseClass(line, currentClassID, localClassMap);
        currentSubClassID = 0;
      } else {
        parseVendor(line, currentVendorID, localVendorMap);
        currentDeviceID = 0;
      }
    } else if (line[1] != '\t') {
      if (currentClassID > 0) {
        parseSubClass(line, currentClassID, currentSubClassID, localSubClassMap);
      } else {
        parseDevice(line, currentVendorID, currentDeviceID, localDeviceMap);
      }
    } else {
      if (currentClassID > 0) {
        parseProgIf(line, currentClassID, currentSubClassID, localProgIfMap);
      } else if (currentDeviceID > 0) {
        parseSubsystem(line, currentVendorID, currentDeviceID, localSubsystemMap);
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
  vector<string> lines;
  string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  file.close();
  if (lines.empty()) {
    std::cerr << "File is empty or could not be read." << std::endl;
    return false;
  }
  auto blocks = splitFileIntoBlocks(lines);
  const size_t numThreads = std::min(std::thread::hardware_concurrency(), static_cast<unsigned int>(blocks.size()));
  vector<std::thread> threads;
  vector<map<ULONG, string>> localVendorMaps(numThreads);
  vector<map<pair<ULONG, ULONG>, string>> localDeviceMaps(numThreads);
  vector<map<tuple<ULONG, ULONG, ULONG, ULONG>, string>> localSubsystemMaps(numThreads);
  vector<map<ULONG, string>> localClassMaps(numThreads);
  vector<map<tuple<ULONG, ULONG>, string>> localSubClassMaps(numThreads);
  vector<map<tuple<ULONG, ULONG, ULONG>, string>> localProgIfMaps(numThreads);
  vector<ULONG> currentVendorIDs(numThreads, 0);

  size_t chunkSize = blocks.size() / numThreads;
  for (size_t i = 0; i < numThreads; ++i) {
    size_t start = i * chunkSize;
    size_t end = (i == numThreads - 1) ? blocks.size() : (i + 1) * chunkSize;
    vector<string> chunk;
    for (size_t j = start; j < end; ++j) {
      chunk.insert(chunk.end(), blocks[j].begin(), blocks[j].end());
    }
    threads.emplace_back(&PciDatabase::parseFileChunk, this, chunk, std::ref(currentVendorIDs[i]),
                         std::ref(localVendorMaps[i]), std::ref(localDeviceMaps[i]),
                         std::ref(localSubsystemMaps[i]), std::ref(localClassMaps[i]),
                         std::ref(localSubClassMaps[i]), std::ref(localProgIfMaps[i]));
  }

  for (auto& t : threads) {
    t.join();
  }

  {
    std::lock_guard<mutex> lock(mapMutex);
    for (size_t i = 0; i < numThreads; ++i) {
      vendorMap.insert(localVendorMaps[i].begin(), localVendorMaps[i].end());
      deviceMap.insert(localDeviceMaps[i].begin(), localDeviceMaps[i].end());
      subsystemMap.insert(localSubsystemMaps[i].begin(), localSubsystemMaps[i].end());
      classMap.insert(localClassMaps[i].begin(), localClassMaps[i].end());
      subclassMap.insert(localSubClassMaps[i].begin(), localSubClassMaps[i].end());
      progIfMap.insert(localProgIfMaps[i].begin(), localProgIfMaps[i].end());
    }
  }

  std::cout << "PCI database loaded successfully." << std::endl;
  return true;
}

string PciDatabase::getVendorName(ULONG vendorId) const {
  std::lock_guard<mutex> lock(mapMutex);
  auto it = vendorMap.find(vendorId);
  return it != vendorMap.end() ? it->second : "N/A";
}

string PciDatabase::getDeviceName(ULONG vendorId, ULONG deviceId) const {
  std::lock_guard<mutex> lock(mapMutex);
  auto it = deviceMap.find({vendorId, deviceId});
  return it != deviceMap.end() ? it->second : "N/A";
}

string PciDatabase::getSubsystemName(ULONG vendorId, ULONG deviceId, ULONG subVendorId, ULONG subDeviceId) const {
  std::lock_guard<mutex> lock(mapMutex);
  auto it = subsystemMap.find({vendorId, deviceId, subVendorId, subDeviceId});
  return it != subsystemMap.end() ? it->second : "N/A";
}

string PciDatabase::getClassName(ULONG classCode) const {
  std::lock_guard<mutex> lock(mapMutex);
  auto it = classMap.find(classCode);
  return it != classMap.end() ? it->second : "N/A";
}

string PciDatabase::getSubClassName(ULONG classCode, ULONG subClassCode) const {
  std::lock_guard<mutex> lock(mapMutex);
  auto it = subclassMap.find({classCode, subClassCode});
  return it != subclassMap.end() ? it->second : "N/A";
}

string PciDatabase::getProgIfName(ULONG classCode, ULONG subClassCode, ULONG progIfCode) const {
  std::lock_guard<mutex> lock(mapMutex);
  auto it = progIfMap.find({classCode, subClassCode, progIfCode});
  return it != progIfMap.end() ? it->second : "N/A";
}
