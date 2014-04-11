#include <QtGui/QGuiApplication>
#include <QtCore/QThread>
#include <QtCore/QDebug>

#include "freenectdev.h"

int main(int argc, char * argv[]) {

    QGuiApplication app(argc, argv);

    QThread kinectThread;

    Freenect::Freenect freenect;
    FreenectDev & device = freenect.createDevice<FreenectDev>(0);

    device.moveToThread(&kinectThread);

    QObject::connect(&kinectThread, &QThread::started, &device, &FreenectDev::start);

    kinectThread.start();

    return app.exec();
}
