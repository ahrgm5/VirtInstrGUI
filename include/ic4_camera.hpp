#pragma once

#include <ic4/ic4.h>
#include <ic4-interop/interop-OpenCV.h>
#include <opencv2/opencv.hpp>

#include <memory>
#include <string>

class IC4Camera {
public:
    IC4Camera();
    ~IC4Camera();

    IC4Camera(const IC4Camera&) = delete;
    IC4Camera& operator=(const IC4Camera&) = delete;

    // Connection & Lifecycle
    bool openDevice(const std::string& modelName = "DFK 33UX226");
    void closeDevice();
    bool isOpened() const;

    // Acquisition Control
    bool startStream();
    void stopStream();

    // Frame Acquisition (Zero-Copy or Deep-Copy into cv::Mat)
    bool snapFrame(cv::Mat& outputFrame, bool copyData = true, int timeoutMs = 1000);

    // GenICam Feature Settings
    bool setExposureAuto(bool enable);
    bool setExposureTimeUs(double microseconds);
    bool setGain(double gainDb);
    bool setPixelFormat(const std::string& formatName = "BGR8");

private:
    ic4::Grabber m_grabber;
    std::shared_ptr<ic4::SnapSink> m_sink;
    bool m_isStreaming = false;
    static bool s_libraryInitialized;
};