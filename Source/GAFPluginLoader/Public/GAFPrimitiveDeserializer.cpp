#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFPrimitiveDeserializer.h"
#include "GAFStream.h"

void FGAFPrimitiveDeserializer::Deserialize(FGAFStream* in, FVector2D* out)
{
    out->X = in->ReadFloat();
    out->Y = in->ReadFloat();
}

void FGAFPrimitiveDeserializer::Deserialize(FGAFStream* in, FGAFRect* out)
{
    Deserialize(in, &out->Origin);
    Deserialize(in, &out->Size);
}

void FGAFPrimitiveDeserializer::Deserialize(FGAFStream* in, FGAFAffineTransform* out)
{
    in->ReadNBytesOfT(out, sizeof(FGAFAffineTransform));
}

void FGAFPrimitiveDeserializer::Deserialize(FGAFStream* in, FGAFSize* out)
{
    out->Width = in->ReadFloat();
    out->Height = in->ReadFloat();
}

void FGAFPrimitiveDeserializer::Deserialize(FGAFStream* in, FColor* out)
{
    in->ReadNBytesOfT(out, 4);
}

void FGAFPrimitiveDeserializer::TranslateColor(FLinearColor& out, unsigned int in)
{
    FColor read;
    FMemory::Memcpy(&read, &in, 4);

    out = FLinearColor::FromSRGBColor(read);
}
