#include <QtGui/QGuiApplication>
<<<<<<< HEAD
#include <QtCore/QThread>
#include <QtCore/QDebug>

#include "freenectdev.h"

//#include <libfreenect/libfreenect.h>

int main(int argc, char * argv[]) {

    QGuiApplication app(argc, argv);

    QThread kinectThread;

    Freenect::Freenect freenect;
    FreenectDev & device = freenect.createDevice<FreenectDev>(0);

    device.moveToThread(&kinectThread);

    QObject::connect(&kinectThread, &QThread::started, &device, &FreenectDev::start);

    kinectThread.start();

    return app.exec();
=======

#include <QtQml/QQmlApplicationEngine>

#include "dicomreader.h"
#include "glviewer.h"

int main(int argc, char * argv[]) {
    QGuiApplication a(argc, argv);

    QQmlApplicationEngine engine(QUrl("qrc:/qml/MainWindow.qml"));
    QObject * appWindow = engine.rootObjects().at(0);

    DicomReader dicomReader;

    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);

    GLviewer glviewer(format);

    glviewer.resize(640, 640);

    QObject::connect(appWindow, SIGNAL(fileOpened(QString)), &dicomReader, SLOT(readFile(QString)));
    QObject::connect(appWindow, SIGNAL(sliceNumberChanged(int)), &dicomReader, SLOT(changeSliceNumber(int)));

    QObject::connect(&dicomReader, &DicomReader::slicesProcessed, &glviewer, &GLviewer::drawSlices);

    static_cast<QWindow*>(appWindow)->show();

    return a.exec();
>>>>>>> fb481e1f758942f5d9a396b881637484aff42b24
}
