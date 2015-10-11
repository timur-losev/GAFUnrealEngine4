#pragma once

class FGAFAnimationSequence
{
public:
    FString         Name;
    uint32          StartFrameNumber;
    uint32          EndFrameNumber;

    inline uint32_t Length() const
    {
        verify(EndFrameNumber > StartFrameNumber);
        return EndFrameNumber - StartFrameNumber;
    }
};
