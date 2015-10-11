#pragma once

#include "GAFTagBase.h"

class FGAFTagDefineAtlas3 : public FTagBase
{
public:
    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};