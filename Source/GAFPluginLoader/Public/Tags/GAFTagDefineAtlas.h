#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineAtlas : public FTagBase
{
public:
    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};