#include "ServerPiCam.h"

ServerPiCam::ServerPiCam() :
   _img("/tmp/test3.png")
{
   _com.waitForIncommingConnection(QHostAddress::Any, 33749);

   _update_timer.setInterval(50);

   //_com_debug.connectToWaitingServer("192.168.178.54", 33749);

   this->connect(&_update_timer, SIGNAL(timeout()), this, SLOT(updateImageData()));

   _update_timer.start();
}

ServerPiCam::~ServerPiCam()
{

}

void ServerPiCam::updateImageData()
{
   static QImage img("/tmp/test3.png");

   qDebug() << "start send img with width: " << _img.width();

   _com.sendData(img);

   qDebug() << "end send img with width: " << _img.width();
}
