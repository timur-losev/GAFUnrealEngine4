#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineStage.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFPrimitiveDeserializer.h"


void FGAFTagDefineStage::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)timeline;
    FColor color;

    uint8 fps = in->ReadU8();  // Scene FPS
    FGAFPrimitiveDeserializer::Deserialize(in, &color);  // Scene color
    uint32 width = in->ReadU16();  // Scene width
    uint32 height = in->ReadU16();  // Scene height

    asset->SetSceneFps(fps);
    asset->SetSceneColor(color);
    asset->SetSceneWidth(width);
    asset->SetSceneHeight(height);
}
