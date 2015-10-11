#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineAnimationMasks.h"

#include "GAFData.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFTimeline.h"


void FGAFTagDefineAnimationMasks::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)asset;
    uint32 count = in->ReadU32();

    for (uint32 i = 0; i < count; ++i)
    {
        uint32 objectId = in->ReadU32();
        uint32 elementAtlasIdRef = in->ReadU32();

        if (in->GetInput()->GetHeader().GetMajorVersion() >= 4)
        {
            uint16 objType = in->ReadU16();
            timeline->PushAnimationMask(objectId, elementAtlasIdRef, static_cast<FGAFCharacterType>(objType));
        }
        else
        {
            timeline->PushAnimationMask(objectId, elementAtlasIdRef, FGAFCharacterType::Texture);
        }
    }
}
