#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineNamedParts.h"

#include "GAFAsset.h"
#include "GAFStream.h"
#include "GAFTimeline.h"

void FGAFTagDefineNamedParts::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)asset;
    uint32 count = in->ReadU32();

    for (uint32 i = 0; i < count; ++i)
    {
        uint32 objectIdRef = in->ReadU32();
        FString name;
        in->ReadString(&name);

        timeline->PushNamedPart(objectIdRef, name);
    }
}
