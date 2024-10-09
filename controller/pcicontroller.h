#ifndef PCICONTROLLER_H
#define PCICONTROLLER_H

#include <QJsonArray>
#include <QJsonObject>
#include "service/PciService.h"
#include <QJsonDocument>

class PciController {

private:
  PciService &service;

public:
  explicit PciController(PciService &service);
  QJsonDocument handleRequest(const QString &request);

private:
  QJsonDocument getPciDevicesJson() const;

};

#endif // PCICONTROLLER_H
