#pragma once

#include "GAFTagBase.h"

class FGAFSubobjectState;

class FGAFTagDefineAnimationFrames : public FTagBase
{
protected:
    typedef TMap<uint32, FGAFSubobjectState*> States_t;
    States_t CurrentStates;

    FGAFSubobjectState* ExtractState(FGAFStream* in);

public:

    virtual ~FGAFTagDefineAnimationFrames();

    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) override;
};