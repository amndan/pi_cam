#include <QCoreApplication>

#include "ServerPiCam.h"

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   ServerPiCam server;

   return a.exec();
}
