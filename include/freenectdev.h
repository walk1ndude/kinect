#ifndef FREENECT_H
#define FREENECT_H

#if defined(_WIN32)

#include <stdint.h>

typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

#endif

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

//#include <libfreenect/libfreenect.hpp>

#include "libfreenect.hpp"

#include <opencv2/imgproc/imgproc.hpp>

class FreenectDev : public QObject, public Freenect::FreenectDevice  {
    Q_OBJECT
public:
    explicit FreenectDev(freenect_context * ctx, int index, QObject * parent = 0);
    ~FreenectDev();

    void setResolution(const cv::Size & resolution);
    void setGamma(const size_t & size);

    void VideoCallback(void * rgb, u_int32_t timestamp);
    void DepthCallback(void * depth, u_int32_t timestamp);

    void showVideo();
    void showDepth();

    void start();
    void stop();

private:
    QMutex _rgbMutex;
    QMutex _depthMutex;

    QTimer * _timerVideo;
    QTimer * _timerDepth;

    std::vector<u_int8_t> _bufferRgb;
    std::vector<u_int8_t> _bufferDepth;
    std::vector<u_int16_t> _gamma;

    cv::Mat _rgbMat;
    cv::Mat _depthMat;
    cv::Mat _depthMat8;

    bool _newDepthFrame;
    bool _newRgbFrame;

signals:
    void finished();
};

#endif // FREENECT_H
