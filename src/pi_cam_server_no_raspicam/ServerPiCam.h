#ifndef SERVERPICAM_H
#define SERVERPICAM_H

#include "../utils/CommunicationTCPPiCam.h"
#include <QTimer>
#include <QImage>

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

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
};

#endif // SERVERPICAM_H
