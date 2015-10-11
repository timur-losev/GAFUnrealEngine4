#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFAnimationFrame.h"
#include "GAFSubobjectState.h"

FGAFAnimationFrame::FGAFAnimationFrame()
{

}

FGAFAnimationFrame::~FGAFAnimationFrame()
{
    for (SubobjectStates_t::TIterator it(SubObjectStates); it; ++it)
    {
        (*it)->Release();
    }
}

const FGAFAnimationFrame::SubobjectStates_t& FGAFAnimationFrame::GetObjectStates() const
{
    return SubObjectStates;
}

void FGAFAnimationFrame::PushObjectState(FGAFSubobjectState* State)
{
    SubObjectStates.Add(State);
    State->AddRef();
}
