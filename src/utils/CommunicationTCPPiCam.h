#ifndef COMMUNICATIONTCPPICAM_H
#define COMMUNICATIONTCPPICAM_H

#include "ConnectionSingleTCP.h"
#include <QImage>
#include <QBuffer>
#include <QDataStream>
#include <unistd.h>

class CommunicationTCPPiCam : public ConnectionSingleTCP
{
   Q_OBJECT

public:
   CommunicationTCPPiCam();
   ~CommunicationTCPPiCam();

   qint64 sendData(const QImage& img);

signals:
   void receivedQImage(QImage img);

protected:  // dont need "protected slots" here --> base class is enough !?
   virtual void onNewData();
};

#endif // COMMUNICATIONTCPPICAM_H
