#include "ConnectionSingleTCP.h"



ConnectionSingleTCP::ConnectionSingleTCP()
{

}

ConnectionSingleTCP::~ConnectionSingleTCP()
{
   this->disconnect();
}

void ConnectionSingleTCP::disconnect()
{
   this->disconnectServer();
   this->disconnectSocket();
}

void ConnectionSingleTCP::disconnectServer()
{
   if(_server)
   {
      _server->close();
      _server->deleteLater();
      _server = NULL;
   }
}

void ConnectionSingleTCP::disconnectSocket()
{
   if(_socket)
   {
      _socket->close();
      _socket->deleteLater();
      _socket = NULL;
   }
}

void ConnectionSingleTCP::setSocket(QTcpSocket* socket)
{
   _socket = socket;
   connect(_socket, SIGNAL(readyRead()), this, SLOT(onNewData()));
   connect(_socket, SIGNAL(disconnected()), this, SLOT(onDisconnection()));
}

bool ConnectionSingleTCP::connectToWaitingServer(QString ip, qint16 port)
{
   if(_socket || _server)
   {
      this->disconnect();
   }

   this->setSocket( new QTcpSocket );

   _socket->connectToHost(ip, port);

   ///@todo error management here; can get connection state here?
   return true;
}

bool ConnectionSingleTCP::waitForIncommingConnection(QHostAddress ip, qint16 port)
{
   if(_server || _socket)
   {
      this->disconnect();
   }

   _server = new QTcpServer();

   if(!_server->listen(ip, port))
   {
      qDebug() << "Cannot open server";
      return false;
   }
   else
   {
      qDebug() << "server started...";
      connect(_server, SIGNAL(newConnection()), this, SLOT(onConnection()));
      return true;
   }
}

qint64 ConnectionSingleTCP::sendData(const QByteArray& data)
{
   if(!_socket)
   {
      qDebug() << "socket not initialized - cannot write";
      return 0;
   }

   if(_socket->bytesToWrite() > 0)
   {
      qDebug() << "Warning _socket->bytesToWrite() > 0 --> overflow";
   }

   return _socket->write(data);
}

void ConnectionSingleTCP::onConnection()
{
   this->setSocket( _server->nextPendingConnection() );
}

void ConnectionSingleTCP::onDisconnection()
{
   this->disconnectSocket();
}










