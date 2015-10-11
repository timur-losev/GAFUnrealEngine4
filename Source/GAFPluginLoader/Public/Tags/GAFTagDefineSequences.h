#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineSequences : public FTagBase
{
public:
    virtual void Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline) override;
};