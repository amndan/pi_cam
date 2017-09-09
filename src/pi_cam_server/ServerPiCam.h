#ifndef SERVERPICAM_H
#define SERVERPICAM_H

#include "../utils/CommunicationTCPPiCam.h"
#include <QTimer>
#include <QImage>

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

#include <raspicam/raspicam.h>

class ServerPiCam : public QObject
{
   Q_OBJECT

public:
   ServerPiCam();
   ~ServerPiCam();

private slots:
   void updateImageData();

private:
   QImage _img;
   CommunicationTCPPiCam _com;
   CommunicationTCPPiCam _com_debug;
   QTimer _update_timer;
   raspicam::RaspiCam Camera;
};

#endif // SERVERPICAM_H
