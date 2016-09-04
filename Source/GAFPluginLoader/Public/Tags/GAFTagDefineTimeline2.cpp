#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineTimeline2.h"

#include "GAFLoader.h"
#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFPrimitiveDeserializer.h"

FGAFTagDefineTimeline2::FGAFTagDefineTimeline2(FGAFLoader* loader) : Loader(loader)
{

}

void FGAFTagDefineTimeline2::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    uint32 id = in->ReadU32();
    uint32 framesCount = in->ReadU32();
    FGAFRect aabb;
    FGAFPoint_t pivot;

    FGAFPrimitiveDeserializer::Deserialize(in, &aabb);
    FGAFPrimitiveDeserializer::Deserialize(in, &pivot);

    FGAFTimelinePtr tl = MakeShareable(new FGAFTimeline(timeline, id, aabb, pivot, framesCount));

    //////////////////////////////////////////////////////////////////////////

    FString temp;
    in->ReadString(&temp);
    tl->SetLinkageName(temp);

    in->ReadString(&temp);
    tl->SetBaseClass(temp);

    Loader->LoadTags(in, asset, tl);

    asset->PushTimeline(id, tl);
    if (id == 0)
    {
        asset->SetRootTimeline((uint32_t)0);
    }

}
