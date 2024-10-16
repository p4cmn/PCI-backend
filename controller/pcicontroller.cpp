#include "controller/PciController.h"

PciController::PciController(PciService &service)
    : service(service) {}

QJsonDocument PciController::handleRequest(const QString &request) {
  Q_UNUSED(request);
  return getPciDevicesJson();
}

QJsonDocument PciController::getPciDevicesJson() const {
  auto devices = service.getPciDevices();
  QJsonArray jsonDevices;

  for (const auto &device : devices) {
    QJsonObject deviceJson;
    deviceJson["bus"] = static_cast<qint64>(device.getBus());
    deviceJson["slot"] = static_cast<qint64>(device.getSlot());
    deviceJson["function"] = static_cast<qint64>(device.getFunction());
    deviceJson["vendorId"] = static_cast<qint64>(device.getVendorId());
    deviceJson["deviceId"] = static_cast<qint64>(device.getDeviceId());
    deviceJson["subVendorId"] = static_cast<qint64>(device.getSubVendorId());
    deviceJson["subDeviceId"] = static_cast<qint64>(device.getSubDeviceId());
    deviceJson["vendorName"] = QString::fromStdString(device.getVendorName());
    deviceJson["deviceName"] = QString::fromStdString(device.getDeviceName());
    deviceJson["subsystemName"] = QString::fromStdString(device.getSubsystemName());
    deviceJson["classCode"] = static_cast<qint64>(device.getClassCode());
    deviceJson["subClassCode"] = static_cast<qint64>(device.getSubClassCode());
    deviceJson["progIf"] = static_cast<qint64>(device.getProgIf());
    deviceJson["className"] = QString::fromStdString(device.getClassName());
    deviceJson["subClassName"] = QString::fromStdString(device.getSubClassName());
    deviceJson["progIfName"] = QString::fromStdString(device.getProgIfName());
    jsonDevices.append(deviceJson);
  }

  QJsonObject jsonResponse;
  jsonResponse["devices"] = jsonDevices;
  return QJsonDocument(jsonResponse);
}
