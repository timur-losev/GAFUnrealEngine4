#pragma once

#include "GAFTagBase.h"

class FGAFLoader;

class FGAFTagDefineTimeline2 : public FTagBase
{
private:
    FGAFLoader*  Loader; // weak
public:

    FGAFTagDefineTimeline2(FGAFLoader*);

    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};
