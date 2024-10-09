// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include "database/PciDatabase.h"

// bool PciDatabase::loadFromFile(const string &filePath) {
//   using namespace std;
//   ifstream file(filePath);
//   if (!file.is_open()) {
//     cerr << "Unable to open pci.ids file." << endl;
//     return false;
//   }

//   string line;
//   ULONG currentVendorID = 0;

//   while (getline(file, line)) {
//     if (line.empty() || line[0] == '#') {
//       continue;
//     }
//     if (line[0] != '\t') {
//       istringstream ss(line);
//       ss >> hex >> currentVendorID;
//       string vendorName;
//       getline(ss, vendorName);
//       vendorName.erase(0, vendorName.find_first_not_of(" \t"));
//       vendorMap[currentVendorID] = vendorName;
//     } else if (line[1] != '\t') {
//       istringstream ss(line.substr(1));
//       ULONG deviceID;
//       ss >> hex >> deviceID;
//       string deviceName;
//       getline(ss, deviceName);
//       deviceName.erase(0, deviceName.find_first_not_of(" \t"));
//       deviceMap[{currentVendorID, deviceID}] = deviceName;
//     } else {
//       istringstream ss(line.substr(2));
//       ULONG subVendorID, subDeviceID;
//       ss >> hex >> subVendorID >> subDeviceID;
//       string subsystemName;
//       getline(ss, subsystemName);
//       subsystemName.erase(0, subsystemName.find_first_not_of(" \t"));
//       subsystemMap[{currentVendorID, subVendorID, subDeviceID}] = subsystemName;
//     }
//   }
//   return true;
// }

// std::string PciDatabase::getVendorName(ULONG vendorId) const {
//   auto it = vendorMap.find(vendorId);
//   return it != vendorMap.end() ? it->second : "N/A";
// }

// std::string PciDatabase::getDeviceName(ULONG vendorId, ULONG deviceId) const {
//   auto it = deviceMap.find({vendorId, deviceId});
//   return it != deviceMap.end() ? it->second : "N/A";
// }

// std::string PciDatabase::getSubsystemName(ULONG vendorId, ULONG subVendorId, ULONG subDeviceId) const {
//   auto it = subsystemMap.find({vendorId, subVendorId, subDeviceId});
//   return it != subsystemMap.end() ? it->second : "N/A";
// }


#include "database/PciDatabase.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex mapMutex; // Мьютекс для защиты доступа к общим ресурсам

// Функция для парсинга части файла
void parseFileChunk(const std::vector<std::string> &lines,
		    ULONG &currentVendorID,
		    map<ULONG, std::string> &localVendorMap,
		    map<std::pair<ULONG, ULONG>, std::string> &localDeviceMap,
		    map<std::tuple<ULONG, ULONG, ULONG>, std::string> &localSubsystemMap) {

  for (const auto &line : lines) {
    if (line.empty() || line[0] == '#') {
      continue;
    }
    if (line[0] != '\t') {
      std::istringstream ss(line);
      ss >> std::hex >> currentVendorID;
      std::string vendorName;
      std::getline(ss, vendorName);
      vendorName.erase(0, vendorName.find_first_not_of(" \t"));
      localVendorMap[currentVendorID] = vendorName;
    } else if (line[1] != '\t') {
      std::istringstream ss(line.substr(1));
      ULONG deviceID;
      ss >> std::hex >> deviceID;
      std::string deviceName;
      std::getline(ss, deviceName);
      deviceName.erase(0, deviceName.find_first_not_of(" \t"));
      localDeviceMap[{currentVendorID, deviceID}] = deviceName;
    } else {
      std::istringstream ss(line.substr(2));
      ULONG subVendorID, subDeviceID;
      ss >> std::hex >> subVendorID >> subDeviceID;
      std::string subsystemName;
      std::getline(ss, subsystemName);
      subsystemName.erase(0, subsystemName.find_first_not_of(" \t"));
      localSubsystemMap[{currentVendorID, subVendorID, subDeviceID}] = subsystemName;
    }
  }
}

bool PciDatabase::loadFromFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Unable to open pci.ids file." << std::endl;
    return false;
  }

  // Чтение всех строк из файла
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    if (line.length() > 10000) { // Проверка на слишком длинную строку
      std::cerr << "Line too long, skipping: " << line.length() << " characters." << std::endl;
      continue;
    }
    lines.push_back(line);
  }
  file.close();

  if (lines.empty()) {
    std::cout << "File is empty or could not be read." << std::endl;
    return false;
  }

  std::cout << "File loaded successfully, total lines: " << lines.size() << std::endl;

  // Обработка всего файла в одном потоке (пока для отладки)
  map<ULONG, std::string> localVendorMap;
  map<std::pair<ULONG, ULONG>, std::string> localDeviceMap;
  map<std::tuple<ULONG, ULONG, ULONG>, std::string> localSubsystemMap;
  ULONG currentVendorID = 0;

  try {
    parseFileChunk(lines, currentVendorID, localVendorMap, localDeviceMap, localSubsystemMap);
  } catch (const std::exception &ex) {
    std::cerr << "Exception during parsing: " << ex.what() << std::endl;
    return false;
  }

  // Перенос локальных карт в общие карты
  {
    std::lock_guard<std::mutex> lock(mapMutex);
    vendorMap.insert(localVendorMap.begin(), localVendorMap.end());
    deviceMap.insert(localDeviceMap.begin(), localDeviceMap.end());
    subsystemMap.insert(localSubsystemMap.begin(), localSubsystemMap.end());
  }

  std::cout << "PCI database loaded successfully." << std::endl;
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

