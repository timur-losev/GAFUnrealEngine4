#pragma once

#include "GAFGeometry.h"

class FGAFStream;

class FGAFPrimitiveDeserializer
{
public:
    static void Deserialize(FGAFStream* in, FVector2D* out);
    static void Deserialize(FGAFStream* in, FGAFRect* out);
    static void Deserialize(FGAFStream* in, FGAFAffineTransform* out);
    static void Deserialize(FGAFStream* in, FGAFSize* out);
    static void Deserialize(FGAFStream* in, FColor* out);
    static void TranslateColor(FLinearColor& out, unsigned int in);
};