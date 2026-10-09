#include "ic4_camera.hpp"
#include <iostream>

bool IC4Camera::s_libraryInitialized = false;

IC4Camera::IC4Camera() {
    if (!s_libraryInitialized) {
        ic4::initLibrary();
        s_libraryInitialized = true;
    }
}

IC4Camera::~IC4Camera() {
    closeDevice();
}

bool IC4Camera::openDevice(const std::string& modelName) {
    auto devices = ic4::DeviceEnum::enumDevices();
    if (devices.empty()) {
        std::cerr << "[IC4] No cameras detected.\n";
        return false;
    }

    ic4::DeviceInfo targetDevice;
    bool found = false;

    for (const auto& dev : devices) {
        if (dev.modelName().find(modelName) != std::string::npos) {
            targetDevice = dev;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "[IC4] Target model '" << modelName << "' not found. Opening first available device: "
                  << devices.front().modelName() << "\n";
        targetDevice = devices.front();
    }

    ic4::Error err;
    if (!m_grabber.deviceOpen(targetDevice, err)) {
        std::cerr << "[IC4] Device open error: " << err.message() << "\n";
        return false;
    }

    // Set default output format to unpacked BGR8 for native OpenCV display
    setPixelFormat("BGR8");

    // Create SnapSink for synchronous image acquisition
    m_sink = ic4::SnapSink::create(err);
    if (!m_sink) {
        std::cerr << "[IC4] Failed to create SnapSink: " << err.message() << "\n";
        return false;
    }

    return true;
}

void IC4Camera::closeDevice() {
    stopStream();
    if (m_grabber.isDeviceOpen()) {
        m_grabber.deviceClose();
    }
}

bool IC4Camera::isOpened() const {
    return m_grabber.isDeviceOpen();
}

bool IC4Camera::startStream() {
    if (!isOpened() || !m_sink) return false;

    ic4::Error err;
    if (!m_grabber.streamSetup(m_sink, ic4::StreamSetupOption::AcquisitionStart, err)) {
        std::cerr << "[IC4] Stream setup failed: " << err.message() << "\n";
        return false;
    }

    m_isStreaming = true;
    return true;
}

void IC4Camera::stopStream() {
    if (m_grabber.isStreaming()) {
        m_grabber.streamStop();
    }
    m_isStreaming = false;
}

bool IC4Camera::snapFrame(cv::Mat& outputFrame, bool copyData, int timeoutMs) {
    if (!m_isStreaming || !m_sink) return false;

    ic4::Error err;
    auto imageBuffer = m_sink->snapSingle(std::chrono::milliseconds(timeoutMs), err);
    if (!imageBuffer) {
        std::cerr << "[IC4] Snap timeout or error: " << err.message() << "\n";
        return false;
    }

    // Interop with OpenCV using ic4interop::OpenCV
    if (copyData) {
        // Deep copy: safe to pass buffer outside stream lifetime
        outputFrame = ic4interop::OpenCV::copy(*imageBuffer, err);
    } else {
        // Zero-copy wrapping: points directly to imageBuffer data
        if (ic4interop::OpenCV::canWrap(*imageBuffer)) {
            outputFrame = ic4interop::OpenCV::wrap(*imageBuffer, err);
        } else {
            // Fall back to copy if format isn't directly wrappable
            outputFrame = ic4interop::OpenCV::copy(*imageBuffer, err);
        }
    }

    if (outputFrame.empty()) {
        std::cerr << "[IC4] Failed to convert buffer to cv::Mat: " << err.message() << "\n";
        return false;
    }

    return true;
}

bool IC4Camera::setExposureAuto(bool enable) {
    if (!isOpened()) return false;
    ic4::Error err;
    auto map = m_grabber.devicePropertyMap(err);
    return map.setValue("ExposureAuto", enable ? "Continuous" : "Off", err);
}

bool IC4Camera::setExposureTimeUs(double microseconds) {
    if (!isOpened()) return false;
    setExposureAuto(false);
    ic4::Error err;
    auto map = m_grabber.devicePropertyMap(err);
    return map.setValue("ExposureTime", microseconds, err);
}

bool IC4Camera::setGain(double gainDb) {
    if (!isOpened()) return false;
    ic4::Error err;
    auto map = m_grabber.devicePropertyMap(err);
    return map.setValue("Gain", gainDb, err);
}

bool IC4Camera::setPixelFormat(const std::string& formatName) {
    if (!isOpened()) return false;
    ic4::Error err;
    auto map = m_grabber.devicePropertyMap(err);
    return map.setValue("PixelFormat", formatName, err);
}