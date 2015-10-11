#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineSequences.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"


void FGAFTagDefineSequences::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)asset;
    uint32 count = in->ReadU32();

    for (uint32 i = 0; i < count; ++i)
    {
        FString id;
        in->ReadString(&id);
        int start = in->ReadU16() - 1;
        int end = in->ReadU16(); // It is not actually the last frame, but the frame after the last, like an STL iterator

        timeline->PushAnimationSequence(id, start, end);
    }
}
