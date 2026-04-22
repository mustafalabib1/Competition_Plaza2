#pragma once

#include <Arduino.h>

class NonBlockingDelay
{
private:
  unsigned long startTime;
  unsigned long delayTime;
  bool isRunning;

public:
  NonBlockingDelay() : startTime(0), delayTime(0), isRunning(false) {}

  void start(unsigned long delayMs)
  {
    delayTime = delayMs;
    startTime = millis();
    isRunning = true;
  }

  bool isFinished()
  {
    if (!isRunning)
      return true;

    if (millis() - startTime >= delayTime)
    {
      isRunning = false;
      return true;
    }
    return false;
  }

  void stop()
  {
    isRunning = false;
  }

  bool isActive() const
  {
    return isRunning;
  }

  unsigned long getRemainingTime() const
  {
    if (!isRunning)
      return 0;

    unsigned long elapsed = millis() - startTime;
    return (elapsed >= delayTime) ? 0 : (delayTime - elapsed);
  }
};
