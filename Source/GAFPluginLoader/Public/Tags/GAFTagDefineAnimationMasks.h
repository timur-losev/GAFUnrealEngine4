#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineAnimationMasks : public FTagBase
{
public:
    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};