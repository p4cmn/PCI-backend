#include <QCoreApplication>
#include "server/TcpServer.h"
#include "service/PciService.h"
#include "controller/PciController.h"
#include "database/PciDatabase.h"
#include "PciDefinitions.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  PciDatabase database;
  if (!database.loadFromFile("D:\\PROGRAMMING\\C++\\PCI\\pci.ids")) {
    qCritical() << "Failed to load PCI database.";
    return -1;
  }

  HANDLE hDevice = CreateFileW(
      L"\\\\.\\PCIDriver",
      GENERIC_READ | GENERIC_WRITE,
      0,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL
      );

  if (hDevice == INVALID_HANDLE_VALUE) {
    qCritical() << "Failed to open device, error:" << GetLastError();
    return -1;
  }

  PciService service(hDevice, database);
  PciController controller(service);
  TcpServer server(&controller);

  QHostAddress address = QHostAddress::LocalHost;
  int port = 1234;
  if (!server.startServer(address, port)) {
    return -1;
  }

  return a.exec();
}
