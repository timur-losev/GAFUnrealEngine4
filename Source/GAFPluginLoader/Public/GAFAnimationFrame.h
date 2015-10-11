#pragma once

class FGAFSubobjectState;

class FGAFAnimationFrame
{
public:
    typedef TArray<FGAFSubobjectState*> SubobjectStates_t;
private:
    SubobjectStates_t SubObjectStates;

public:

    FGAFAnimationFrame();
    ~FGAFAnimationFrame();

    const SubobjectStates_t& GetObjectStates() const;
    void PushObjectState(FGAFSubobjectState*);
};