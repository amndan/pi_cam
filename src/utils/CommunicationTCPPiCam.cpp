#include "CommunicationTCPPiCam.h"

CommunicationTCPPiCam::CommunicationTCPPiCam()
{

}

CommunicationTCPPiCam::~CommunicationTCPPiCam()
{

}

qint64 CommunicationTCPPiCam::sendData(const QImage& img)
{
   if(!_socket)
   {
      qDebug() << "socket not initialized --> not sending new data!";
      return 0;
   }

   // if gui is resizing acknoledge from client (gui) is not fast enough to
   // get all data; wait for all data sent before sending new data;
   // IODevice is buffering in background (can be a memleak if not handled)
   if(_socket->bytesToWrite() > 0)
   {
      qDebug() << "bytes to write: " << _socket->bytesToWrite() << "--> not sending new data!";
      return 0;
   }

   QBuffer buffer;
   QDataStream stream(_socket);
   stream.setVersion(QDataStream::Qt_5_5);
   img.save(&buffer, COM_TCP_IMG_TYPE, -1);
   QString id_string("V1.0_QIMAGE");
   stream << qint64(sizeof(id_string)) << id_string << qint64(buffer.size());
   _socket->write(buffer.data());

   ///@todo return written bytes
   return 1;
}

void CommunicationTCPPiCam::onNewData()
{
   qDebug() << "SocketSize:   " << _socket->size();

   qint64 payload_size;

   if(_socket->size() < (qint64) sizeof(payload_size))
   {
      qDebug() << "transmission not yet complete";
      return;
   }

   QDataStream stream(_socket);
   stream.setVersion(QDataStream::Qt_5_5);
   stream.startTransaction();

   stream >> payload_size;

   payload_size += sizeof(payload_size);

   if(_socket->size() < payload_size)
   {
      qDebug() << "transmission not yet complete --> rollback";
      stream.rollbackTransaction();
      return;
   }

   QString id_string;
   stream >> id_string;
   stream >> payload_size;

   if(_socket->size() < payload_size)
   {
      qDebug() << "transmission not yet complete --> rollback";
      stream.rollbackTransaction();
      return;
   }

   if(!stream.commitTransaction())
   {
      qDebug() << "COMMIT FALSE";
      return;
   }

   if(id_string == "V1.0_QIMAGE")
   {
      QByteArray array;
      array = _socket->read(payload_size);
      QImage img;

      if(!img.loadFromData(array, COM_TCP_IMG_TYPE))
      {
         qDebug() << "READ IMG ERROR";
         return;
      }

      emit receivedQImage(img);
      qDebug() << "RECEIVED QIMAGE";

      ///@bug loading img from io device directly does not work proberly
      /// assume the it does not load payload_size number of bytes
      /// force this with loading from byte array
      //      QImage img;
      //      qDebug() << "img load " << img.load(_socket, "JPG");
      //      emit receivedQImage(img);
      //      qDebug() << "RECEIVED QIMAGE";

      if(_socket->size())
      {
         this->onNewData();
      }
   }
   else
   {
      _socket->read(payload_size);
   }
}
