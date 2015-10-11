#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFSubobjectState.h"

FGAFSubobjectState::FGAFSubobjectState():
    RefCount(1)
{

}

FGAFSubobjectState::~FGAFSubobjectState()
{

}

void FGAFSubobjectState::CTXMakeIdentity()
{
    ColorOffsets[0] = ColorOffsets[1] = ColorOffsets[2] = ColorOffsets[3] = 0;
    ColorMults[GAFCTI_R] = ColorMults[GAFCTI_G] = ColorMults[GAFCTI_B] = 1;
}

void FGAFSubobjectState::AddRef()
{
    RefCount++;
}

void FGAFSubobjectState::Release()
{
    if (0 == --RefCount)
    {
        delete this;
    }
}

void FGAFSubobjectState::InitEmpty(uint32 objectIdRef)
{
    ObjectIDRef = objectIdRef;
    ZIndex = 0;
    ColorOffsets[0] = ColorOffsets[1] = ColorOffsets[3] = ColorOffsets[4] = ColorMults[GAFCTI_A] = 0;
    ColorMults[GAFCTI_R] = ColorMults[GAFCTI_G] = ColorMults[GAFCTI_B] = 1.f;
}