#pragma once
#include "GAFTagDefineAnimationFrames.h"

class FGAFSubobjectState;

class FGAFTagDefineAnimationFrames2 : public FGAFTagDefineAnimationFrames
{
public:

    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};