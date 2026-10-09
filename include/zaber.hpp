#pragma once

#include "instrument.hpp"
#include "imotoraxis.hpp"
#include <string>
#include <cstdint>
#include <vector>
#include <memory>

struct ZaberAxisLimits {
    int index;
    int32_t minPosition;
    int32_t maxPosition;
    int32_t resolution;
};

class Zaber : public Instrument {
public:
    static ChannelSettings defaultZaberSettings();

    explicit Zaber(ViSession defaultRM);
    Zaber(
        ViSession defaultRM,
        const std::string& resourceName,
        const ChannelSettings& settings = defaultZaberSettings()
        );

    ~Zaber() override = default;

    ChannelSettings defaultSettings() const override {
        return defaultZaberSettings();
    }

    bool open(const std::string& resourceName, const ChannelSettings& settings = defaultZaberSettings()) override;

    // --- 1-Argument Methods (Direct Motion & Query Commands) ---
    void home(int index);
    void moveAbsolute(int index, int32_t microsteps);
    void moveRelative(int index, int32_t microsteps);
    void stop(int index);
    int32_t getPositionForDevice(int index);
    bool isIdle(int index);
    void waitUntilIdle(int index, int pollIntervalMs = 100, int timeoutSeconds = 30);
    int32_t getLimitMax(int index);
    int32_t getLimitMin(int index);
    int32_t getResolution(int index);

    // --- 2-Argument Methods (For backward compatibility with mainwindow.cpp) ---
    void home(int deviceAddress, int axisNumber);
    void moveAbsolute(int deviceAddress, int axisNumber, int32_t microsteps);
    void moveRelative(int deviceAddress, int axisNumber, int32_t microsteps);
    void stop(int deviceAddress, int axisNumber);
    int32_t getPositionForAxis(int deviceAddress, int axisNumber);
    bool isIdle(int deviceAddress, int axisNumber);
    void waitUntilIdle(int deviceAddress, int axisNumber, int pollIntervalMs, int timeoutSeconds);
    int32_t getLimitMax(int deviceAddress, int axisNumber);
    int32_t getLimitMin(int deviceAddress, int axisNumber);
    int32_t getResolution(int deviceAddress, int axisNumber);

    // --- Global Methods ---
    int32_t getPosition(int numDevices = 4);
    std::vector<ZaberAxisLimits> queryLimitsForAllDevices(int numDevices);

private:
    std::string sendCommand(int index, const std::string& command);
    void writeCommand(int index, const std::string& command);
    int32_t parseSettingResponse(const std::string& response);
};

// --- Zaber Motor Adapter (for RemoteControl integration) ---
class ZaberAdapter : public IMotorAxis {
public:
    explicit ZaberAdapter(std::shared_ptr<Zaber> zaber);

    void moveRelative(int axis, int distance) override;
    void moveAbsolute(int axis, int position) override;
    void stop(int axis) override;
    void setSpeed(int axis, int speed) override;
    void indexIncremental(int axis, int steps) override;
    int32_t getPosition(int axis) override;

private:
    std::shared_ptr<Zaber> m_zaber;
};