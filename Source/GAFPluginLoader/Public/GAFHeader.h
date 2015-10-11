#pragma once

#include "GAFGeometry.h"

class FGAFHeader
{
public:
    enum Compression
    {
        __CompressionDefault = 0, // Internal
        CompressedNone = 0x00474146, // GAF
        CompressedZip = 0x00474143,  // GAC
    };

public:

    // Common
    Compression     Compression = Compression::__CompressionDefault;
    uint16          Version = 0;
    uint32          FileLenght = 0;

    // Version 3
    uint16          FramesCount = 0;
    FGAFRect        FrameSize;
    FGAFPoint_t     Pivot;

    // Version 4
    TArray<float>   ScaleValues;
    TArray<float>   CsfValues;

    inline uint16 GetMajorVersion() const
    {
        return Version & 0x00FF;
    }

    inline uint16 GetMinorVersion() const
    {
        return Version >> 8;
    }
};