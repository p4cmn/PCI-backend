#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "controller/PciController.h"

class TcpServer : public QTcpServer {
  Q_OBJECT

private:
  PciController *controller;

public:
  explicit TcpServer(PciController *controller, QObject *parent = nullptr);
  bool startServer(const QHostAddress &address, int port);

protected:
  void incomingConnection(qintptr socketDescriptor) override;

private:
  void handleClient(QTcpSocket *clientSocket);

};

#endif // TCPSERVER_H
