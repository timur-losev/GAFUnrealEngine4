#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineNamedParts : public FTagBase
{
public:
    virtual void Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline) override;
};