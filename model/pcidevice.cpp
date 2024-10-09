#include "model/PciDevice.h"

PciDevice::PciDevice(ULONG bus,
		     ULONG slot,
		     ULONG function,
		     ULONG vendorId,
		     ULONG deviceId,
		     ULONG subVendorId,
		     ULONG subDeviceId,
		     const string &vendorName,
		     const string &deviceName,
		     const string &subsystemName)
  : bus(bus),
    slot(slot),
    function(function),
    vendorId(vendorId),
    deviceId(deviceId),
    subVendorId(subVendorId),
    subDeviceId(subDeviceId),
    vendorName(vendorName),
    deviceName(deviceName),
    subsystemName(subsystemName) {}

// Геттеры
ULONG PciDevice::getBus() const { return bus; }
ULONG PciDevice::getSlot() const { return slot; }
ULONG PciDevice::getFunction() const { return function; }
ULONG PciDevice::getVendorId() const { return vendorId; }
ULONG PciDevice::getDeviceId() const { return deviceId; }
ULONG PciDevice::getSubVendorId() const { return subVendorId; }
ULONG PciDevice::getSubDeviceId() const { return subDeviceId; }
string PciDevice::getVendorName() const { return vendorName; }
string PciDevice::getDeviceName() const { return deviceName; }
string PciDevice::getSubsystemName() const { return subsystemName; }

// Сеттеры
void PciDevice::setBus(ULONG bus) { this->bus = bus; }
void PciDevice::setSlot(ULONG slot) { this->slot = slot; }
void PciDevice::setFunction(ULONG function) { this->function = function; }
void PciDevice::setVendorId(ULONG vendorId) { this->vendorId = vendorId; }
void PciDevice::setDeviceId(ULONG deviceId) { this->deviceId = deviceId; }
void PciDevice::setSubVendorId(ULONG subVendorId) { this->subVendorId = subVendorId; }
void PciDevice::setSubDeviceId(ULONG subDeviceId) { this->subDeviceId = subDeviceId; }
void PciDevice::setVendorName(const string &vendorName) { this->vendorName = vendorName; }
void PciDevice::setDeviceName(const string &deviceName) { this->deviceName = deviceName; }
void PciDevice::setSubsystemName(const string &subsystemName) { this->subsystemName = subsystemName; }
