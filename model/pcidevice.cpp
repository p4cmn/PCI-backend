#include "model/PciDevice.h"

PciDevice::PciDevice(ULONG bus,
		     ULONG slot,
		     ULONG function,
		     ULONG vendorId,
		     ULONG deviceId,
		     ULONG subVendorId,
		     ULONG subDeviceId,
		     ULONG classCode,
		     ULONG subclassCode,
		     ULONG progIf,
		     const string &vendorName,
		     const string &deviceName,
		     const string &subsystemName,
		     const string &className,
		     const string &subclassName,
		     const string &progIfName)
  : bus(bus),
    slot(slot),
    function(function),
    vendorId(vendorId),
    deviceId(deviceId),
    subVendorId(subVendorId),
    subDeviceId(subDeviceId),
    classCode(classCode),
    subclassCode(subclassCode),
    progIf(progIf),
    vendorName(vendorName),
    deviceName(deviceName),
    subsystemName(subsystemName),
    className(className),
    subclassName(subclassName),
    progIfName(progIfName){}

// Геттеры
ULONG PciDevice::getBus() const { return bus; }
ULONG PciDevice::getSlot() const { return slot; }
ULONG PciDevice::getFunction() const { return function; }
ULONG PciDevice::getVendorId() const { return vendorId; }
ULONG PciDevice::getDeviceId() const { return deviceId; }
ULONG PciDevice::getSubVendorId() const { return subVendorId; }
ULONG PciDevice::getSubDeviceId() const { return subDeviceId; }
ULONG PciDevice::getClassCode() const{ return classCode; }
ULONG PciDevice::getSubClassCode() const{ return subclassCode; }
ULONG PciDevice::getProgIf() const{ return progIf; }
string PciDevice::getVendorName() const { return vendorName; }
string PciDevice::getDeviceName() const { return deviceName; }
string PciDevice::getSubsystemName() const { return subsystemName; }
string PciDevice::getClassName() const { return className; }
string PciDevice::getSubClassName() const { return subclassName; }
string PciDevice::getProgIfName() const { return progIfName; }

// Сеттеры
void PciDevice::setBus(ULONG bus) { this->bus = bus; }
void PciDevice::setSlot(ULONG slot) { this->slot = slot; }
void PciDevice::setFunction(ULONG function) { this->function = function; }
void PciDevice::setVendorId(ULONG vendorId) { this->vendorId = vendorId; }
void PciDevice::setDeviceId(ULONG deviceId) { this->deviceId = deviceId; }
void PciDevice::setSubVendorId(ULONG subVendorId) { this->subVendorId = subVendorId; }
void PciDevice::setSubDeviceId(ULONG subDeviceId) { this->subDeviceId = subDeviceId; }
void PciDevice::setClassCode(ULONG classCode) { this->classCode = classCode; }
void PciDevice::setSubClassCode(ULONG subclassCode) { this->subclassCode = subclassCode; }
void PciDevice::setProgIf(ULONG progIf) { this->progIf = progIf; }
void PciDevice::setVendorName(const string &vendorName) { this->vendorName = vendorName; }
void PciDevice::setDeviceName(const string &deviceName) { this->deviceName = deviceName; }
void PciDevice::setSubsystemName(const string &subsystemName) { this->subsystemName = subsystemName; }
void PciDevice::setClassName(const string &className) { this->className = className; }
void PciDevice::setSubClassName(const string &subclassName) { this->subclassName = subclassName; }
void PciDevice::setProgIfName(const string &progIfName) { this->progIfName = progIfName; }
