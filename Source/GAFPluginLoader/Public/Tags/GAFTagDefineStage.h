#pragma once
#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineStage : public FTagBase
{
public:
    virtual void Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline) override;
};