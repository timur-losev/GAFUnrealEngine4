#pragma once

#include "GAFTagBase.h"

class FGAFLoader;

class FGAFTagDefineTimeline : public FTagBase
{
private:
    FGAFLoader*  Loader; // weak
public:

    FGAFTagDefineTimeline(FGAFLoader*);

    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;

};