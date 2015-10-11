#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineAnimationObjects : public FTagBase
{
public:
    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};