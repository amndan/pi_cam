#ifndef CONNECTIONSINGLETCP_H
#define CONNECTIONSINGLETCP_H

#include <QTcpServer>
#include <QTcpSocket>
#include <memory>

class ConnectionSingleTCP : public QObject
{
   Q_OBJECT

public:
   ConnectionSingleTCP();
   ~ConnectionSingleTCP();

   bool connectToWaitingServer(QString ip, qint16 port);
   bool waitForIncommingConnection(QHostAddress ip, qint16 port);
   qint64 sendData(const QByteArray& data);
   void disconnect();
   void disconnectServer();
   void disconnectSocket();

protected:
   void setSocket(QTcpSocket* socket);

   QTcpServer* _server = NULL;
   QTcpSocket* _socket = NULL;


private slots:
   void onConnection();
   void onDisconnection();

protected slots:
   virtual void onNewData() = 0;
};

#endif // CONNECTIONSINGLETCP_H
