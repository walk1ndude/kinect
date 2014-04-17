#include <opencv2/highgui/highgui.hpp>

#include "freenectdev.h"

#define WINDOW_RGB "rgb"
#define WINDOW_DEPTH "depth"

FreenectDev::FreenectDev(freenect_context * ctx, int index, QObject * parent) :
    QObject(parent),
    Freenect::FreenectDevice(ctx, index),
    _bufferRgb(FREENECT_VIDEO_RGB),
    _bufferDepth(FREENECT_DEPTH_11BIT),
    _newDepthFrame(false),
    _newRgbFrame(false) {

    setGamma(2048);
    setResolution(cv::Size(640, 480));

    cv::namedWindow(WINDOW_RGB, CV_WINDOW_AUTOSIZE);
    cv::namedWindow(WINDOW_DEPTH, CV_WINDOW_AUTOSIZE);
}

FreenectDev::~FreenectDev() {

}

void FreenectDev::setGamma(const size_t & size) {
    _gamma.clear();
    float v;
    for (size_t i = 0 ; i < size ; ++ i) {
        v = i / (float) size;
        v = std::pow(v, 3) * 6;
        _gamma.push_back(v * 6 * 256);
    }
}

void FreenectDev::setResolution(const cv::Size & resolution) {
    _depthMat = cv::Mat::zeros(resolution, CV_16UC1);
    _rgbMat = cv::Mat::zeros(resolution, CV_8UC3);
    _depthMat8 = cv::Mat::zeros(resolution, CV_8UC1);
}

void FreenectDev::VideoCallback(void * rgb, u_int32_t timestamp) {
    //qDebug() << "RGB callback";

    _rgbMutex.lock();

    if (!_newRgbFrame) {
        _rgbMat.data = static_cast<u_int8_t *>(rgb);
        _newRgbFrame = true;
    }

    _rgbMutex.unlock();
}

void FreenectDev::DepthCallback(void * depth, u_int32_t timestamp) {
    //qDebug() << "Depth callback";

    _depthMutex.lock();

    if (!_newDepthFrame) {
        _depthMat.data = (uchar*) static_cast<u_int16_t *>(depth);
        _newDepthFrame = true;
    }

    _depthMutex.unlock();
}

void FreenectDev::showVideo() {
    //qDebug() << _newRgbFrame;

    _rgbMutex.lock();

    if (_newRgbFrame) {
        _newRgbFrame = false;

        cv::cvtColor(_rgbMat, _rgbMat, CV_RGB2BGR);
        cv::imshow(WINDOW_RGB, _rgbMat);
        //cv::waitKey(1);
    }

    _rgbMutex.unlock();
}

void FreenectDev::showDepth() {
    _depthMutex.lock();

    if (_newDepthFrame) {
        _newDepthFrame = false;

        _depthMat.convertTo(_depthMat8, CV_8UC1, 255.0 / 2048.0);
        cv::imshow(WINDOW_DEPTH, _depthMat8);
        //cv::waitKey(1);
    }

    _depthMutex.unlock();
}

void FreenectDev::start() {
    _timerVideo = new QTimer;
    _timerDepth = new QTimer;

    QObject::connect(_timerVideo, &QTimer::timeout, this, &FreenectDev::showVideo);
    QObject::connect(_timerDepth, &QTimer::timeout, this, &FreenectDev::showDepth);

    _timerVideo->start(80);
    _timerDepth->start(80);

    startVideo();
    startDepth();
}

void FreenectDev::stop() {
    stopVideo();
    stopDepth();
}
