#pragma once

class FGAFStream;
class UGAFAsset;
class FGAFTimeline;

ForwardSharedPointer(FGAFTimeline);

class FTagBase
{
public:
    virtual ~FTagBase() {}
    virtual void Read(FGAFStream*, UGAFAsset*, FGAFTimelinePtr) = 0;
};