#include "server/TcpServer.h"
#include <QJsonDocument>

TcpServer::TcpServer(PciController *controller, QObject *parent)
    : QTcpServer(parent), controller(controller) {}

bool TcpServer::startServer(const QHostAddress &address, int port) {
  if (!this->listen(address, port)) {
    qCritical() << "Unable to start the server:" << this->errorString();
    return false;
  }
  qDebug() << "Server started on" << address.toString() << "port" << port;
  return true;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
  QTcpSocket *clientSocket = new QTcpSocket(this);
  if (!clientSocket->setSocketDescriptor(socketDescriptor)) {
    qCritical() << "Failed to set socket descriptor";
    delete clientSocket;
    return;
  }

  connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
    handleClient(clientSocket);
  });

  connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
  qDebug() << "New connection established";
}

void TcpServer::handleClient(QTcpSocket *clientSocket) {
  QByteArray requestData = clientSocket->readAll();
  QString request = QString::fromUtf8(requestData);

  QJsonDocument responseJson = controller->handleRequest(request);
  QByteArray responseData = responseJson.toJson(QJsonDocument::Compact);

  clientSocket->write(responseData);
  clientSocket->flush();
}
