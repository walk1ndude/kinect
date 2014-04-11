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
        v = i / size;
        v = std::pow(v, 3) * 6;
        _gamma.push_back(v * 6 * 256);
    }
}

void FreenectDev::setResolution(const cv::Size & resolution) {
    _depthMat.create(resolution, CV_16UC1);
    _rgbMat = cv::Mat::zeros(resolution, CV_8UC3);
    _ownMat = cv::Mat::zeros(resolution, CV_8UC3);
}

void FreenectDev::VideoCallback(void * rgb, u_int32_t timestamp) {
    qDebug() << "RGB callback";

    _rgbMutex.lock();

    _rgbMat.data = static_cast<u_int8_t *>(rgb);
    _newRgbFrame = true;

    _rgbMutex.unlock();

    cv::imshow(WINDOW_RGB, _rgbMat);
    cv::waitKey(1);
}

void FreenectDev::DepthCallback(void * depth, u_int32_t timestamp) {
    qDebug() << "Depth callback";

    _depthMutex.lock();

    _depthMat.data = (uchar*) static_cast<u_int16_t *>(depth);
    _newDepthFrame = true;

    _depthMutex.unlock();

    cv::imshow(WINDOW_DEPTH, _depthMat);
    cv::waitKey(1);
}

bool FreenectDev::getVideo(cv::Mat & output) {
    _rgbMutex.lock();

    if(_newRgbFrame) {
        cv::cvtColor(_rgbMat, output, CV_RGB2BGR);
        _newRgbFrame = false;
        _rgbMutex.unlock();
        return true;
    }
    else {
        _rgbMutex.unlock();
        return false;
    }
}

bool FreenectDev::getDepth(cv::Mat & output) {
    _depthMutex.lock();

    if(_newRgbFrame) {
        cv::cvtColor(_depthMat, output, CV_RGB2BGR);
        _newDepthFrame = false;
        _depthMutex.unlock();
        return true;
    }
    else {
        _depthMutex.unlock();
        return false;
    }
}

void FreenectDev::start() {
    startVideo();
    startDepth();
}

void FreenectDev::stop() {
    stopVideo();
    stopDepth();
}
