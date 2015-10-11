#pragma once

#include "GAFGeometry.h"

enum FGAFColorTransformIndex
{
    GAFCTI_R,
    GAFCTI_G,
    GAFCTI_B,
    GAFCTI_A
};

class FGAFSubobjectState
{
private:
    float ColorMults[4];
    float ColorOffsets[4];

    uint32 RefCount;
public:

    uint32 ObjectIDRef = UINT_MAX;
    uint32 MaskObjectIDRef = UINT_MAX;

    FGAFAffineTransform AffineTransform;

    int32 ZIndex = 0;

    void InitEmpty(uint32 objectIdRef);

    FGAFSubobjectState();
    ~FGAFSubobjectState();

    inline float* GetColorMults()
    {
        return ColorMults;
    }

    inline float* GetColorOffsets()
    {
        return ColorOffsets;
    }

    inline const float* GetColorMults() const
    {
        return ColorMults;
    }

    inline const float* GetColorOffsets() const
    {
        return ColorOffsets;
    }

    inline bool IsVisible() const
    {
        return ColorMults[GAFCTI_A] > std::numeric_limits<float>::epsilon() ||
            ColorOffsets[GAFCTI_A] > std::numeric_limits<float>::epsilon();
    }

    void CTXMakeIdentity();

    void AddRef();
    void Release();
};