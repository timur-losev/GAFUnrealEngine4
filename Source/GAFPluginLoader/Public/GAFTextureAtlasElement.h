#pragma once

#include "GAFGeometry.h"
#include "GAFCollections.h"

class FGAFTextureAtlasElement
{
private:
    float              Scale;
    float              ScaleX;
    float              ScaleY;
public:
    FString     Name;
    FVector2D   PivotPoint;

    FGAFRect    Bounds;

    uint32      AtlasIdx;
    uint32      ElementAtlasIdx;

    FGAFRotation Rotation = FGAFRotation::NONE;

    void SetScale(float s)
    {
        Scale = ScaleX = ScaleY = s;
    }
    const float GetScale() const { return Scale; }
    void SetScaleX(float s)
    {
        Scale = ScaleX = s;
    }
    const float GetScaleX() const { return ScaleX; }
    void SetScaleY(float s)
    {
        Scale = ScaleY = s;
    }
    const float GetScaleY() const { return ScaleY; }
};