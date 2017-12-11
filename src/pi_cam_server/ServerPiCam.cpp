#include "ServerPiCam.h"

ServerPiCam::ServerPiCam() :
   _img("/home/pi/test3.png")
{
   _com.waitForIncommingConnection(QHostAddress::Any, 33749);

   _update_timer.setInterval(50);

   //_com_debug.connectToWaitingServer("localhost", 33749);

   this->connect(&_update_timer, SIGNAL(timeout()), this, SLOT(updateImageData()));

   //Open camera
   qDebug() <<"Opening Camera...";
   if ( !Camera.open()) {qDebug() << "Error opening camera";}

   Camera.setFormat(raspicam::RASPICAM_FORMAT_RGB);
   Camera.setWidth (1280);
   Camera.setHeight (960);

   ///@bug wrong image data with downsampling --> tested with file ppm file function of raspicam
   ///@bug image format GRAY does not change data --> tested with image data size
   //Camera.setWidth (320);
   //Camera.setHeight (240);

   //wait a while until camera stabilizes
   qDebug() <<"Sleeping for 3 secs";
   usleep(3000000);

   //capture
   _update_timer.start();
}

ServerPiCam::~ServerPiCam()
{

}

void ServerPiCam::updateImageData()
{
   qDebug() <<"grab!";
   Camera.grab();
   qDebug() <<"grabed!";

   //allocate memory
   unsigned char *data=new unsigned char[Camera.getImageTypeSize(Camera.getFormat())];

   //extract the image in rgb format
   Camera.retrieve(data);//get camera image

   QImage img(data, Camera.getWidth(), Camera.getHeight(), QImage::Format_RGB888);

   //qDebug() << "SIZEOF CAMERA DATA" << Camera.getImageTypeSize ( Camera.getFormat() );

   // scale image
   img = img.scaled(320, 240, Qt::KeepAspectRatio);

   // send data
   _com.sendData(img);

   qDebug() << "send img with width: " << img.width();

   //save image on disc
//   static qint64 img_cnt = 0;
//   QString string;
//   string = string + "raspicam_image_" + QString::number(img_cnt++) + ".ppm";
//   std::ofstream outFile ( string.toStdString().c_str(),std::ios::binary );
//   outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
//   outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
//   qDebug() << string;

   //free resrources
   delete data;
}
