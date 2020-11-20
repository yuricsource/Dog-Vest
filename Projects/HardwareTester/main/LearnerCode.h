#pragma once

#include <cstring>
#include <string>
#include "stdio.h"
#include <cstdint>
#include <cstdarg>
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"
#include "Timer.h"

class LearnerCode : Hal::Timer::Callback
{

public:
    LearnerCode(Hal::Gpio* gpio, Hal::Gpio::GpioIndex pin, Hal::Timer* timer) : _gpio(gpio), _pin(pin), _timer(timer)
    {
        _timer->AddCallback(this);
        _gpio->ConfigInput(_pin);
    }
    
    enum class CodeLearnerState : uint8_t
    {
        None,
        Ready,
        Logging,
        Finished
    };

    void Stop();

    void Start();

    void TimerCallback() override;

    void PrintResult();
    void PrintCode();

    CodeLearnerState GetState()
    {
        return _state;
    }
    
    // Sampling 16k per second => period of 62.5 us
    static constexpr uint32_t SamplingFrequency = 16000; 

private:
    static constexpr uint32_t MinimunBitsAllowed = 60;
    static constexpr uint32_t DelayLowInMiliseconds = 3;
    static constexpr float TOnLowInMiliseconds = 0.35;
    static constexpr float TOnHighInMiliseconds = 0.7;
    static constexpr float TOnMiddleLevel = ((TOnHighInMiliseconds + TOnLowInMiliseconds) / 2);
    static constexpr uint32_t WaitCount = (DelayLowInMiliseconds * SamplingFrequency / 1000);
    static constexpr uint32_t HighOrLowCount = (TOnMiddleLevel * SamplingFrequency / 1000);
    Hal::Gpio* _gpio;
    Hal::Gpio::GpioIndex _pin;
    Hal::Timer* _timer;
    CodeLearnerState _state = CodeLearnerState::None;
    uint32_t _waitCounter = 0;
    uint8_t _data[200] = {};
    uint16_t _bufferIndex = 0;
    uint32_t _tOnCounter = 0;
};


