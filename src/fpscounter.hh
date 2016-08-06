#ifndef FPSCOUNTER_HH
#define FPSCOUNTER_HH

#include <vector>
#include <numeric>
#include <iostream>
#include <ClanLib/core.h>

class FPSCounter
{
public:
    FPSCounter(size_t bufferSize) : bufferSize(bufferSize), delta(0), frameCount(0)
    {
    }

    void beginFrame()
    {
        if (frameCount == 0)
            beginTime = CL_System::get_time();
    }

    void endFrame()
    {
        frameCount++;

        if (frameCount == bufferSize)
        {
            endTime = CL_System::get_time();
            size_t difference = endTime - beginTime;

            delta = static_cast<float>(difference) / 1000;
            frameCount = 0;
        }
    }

    float getFPS()
    {
        if (delta > 0)
            return static_cast<float>(bufferSize) / delta;

        return 0;
    }
private:
    size_t beginTime;
    size_t endTime;
    size_t bufferSize;
    float delta;
    size_t frameCount;
};

#endif
