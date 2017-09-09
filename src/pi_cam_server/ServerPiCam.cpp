#include "ServerPiCam.h"

ServerPiCam::ServerPiCam() :
   _img("/home/pi/test3.png")
{
   _com.waitForIncommingConnection(QHostAddress::Any, 33749);

   _update_timer.setInterval(50);

   //_com_debug.connectToWaitingServer("localhost", 33749);

   this->connect(&_update_timer, SIGNAL(timeout()), this, SLOT(updateImageData()));

   _update_timer.start();

   //Open camera
   qDebug() <<"Opening Camera...";
   if ( !Camera.open()) {qDebug() << "Error opening camera";}
   //wait a while until camera stabilizes
   qDebug() <<"Sleeping for 3 secs";
   usleep(3000000);
   //capture
}

ServerPiCam::~ServerPiCam()
{

}

void ServerPiCam::updateImageData()
{
   Camera.grab();
   //allocate memory
   unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
   //extract the image in rgb format
   Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_IGNORE );//get camera image

   QImage img(data, Camera.getWidth(), Camera.getHeight(), QImage::Format_RGB888);
   img = img.scaled(100,100);

   _com.sendData(img);

   qDebug() << "send img with width: " << _img.width();

   //save
   static qint64 img_cnt = 0;
   QString string;
   string = string + "raspicam_image_" + QString::number(img_cnt++) + ".ppm";
//   std::ofstream outFile ( string.toStdString().c_str(),std::ios::binary );
//   outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
//   outFile.write ( ( chard* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
   qDebug() << string;
   //free resrources
   delete data;
}
