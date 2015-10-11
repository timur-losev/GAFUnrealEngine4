#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTimeline.h"
#include "GAFTextureAtlas.h"
#include "GAFAnimationFrame.h"

FGAFTimeline::FGAFTimeline(FGAFTimelinePtr parent, uint32 id, const FGAFRect& aabb, const FGAFPoint_t& pivot, uint32 framesCount) :
    Id(id)
    ,Aabb(aabb)
    ,Pivot(pivot)
    ,FramesCount(framesCount)
    ,Parent(parent)
    ,SceneFps(0)
    ,SceneWidth(0)
    ,SceneHeight(0)
{

}

FGAFTimeline::~FGAFTimeline()
{
    GAF_RELEASE_ARRAY(FGAFTextureAtlases_t, TextureAtlases);
    GAF_RELEASE_ARRAY(FGAFAnimationFrames_t, AnimationFrames);
}

void FGAFTimeline::PushTextureAtlas(FGAFTextureAtlas* atlas)
{
    TextureAtlases.Add(atlas);
}

void FGAFTimeline::PushAnimationMask(uint32 objectId, uint32 elementAtlasIdRef, FGAFCharacterType charType)
{
    AnimationMasks[objectId] = std::make_tuple(elementAtlasIdRef, charType);
}

void FGAFTimeline::PushAnimationObject(uint32 objectId, uint32 elementAtlasIdRef, FGAFCharacterType charType)
{
    AnimationObjects[objectId] = std::make_tuple(elementAtlasIdRef, charType);
}

void FGAFTimeline::PushAnimationFrame(FGAFAnimationFrame* frame)
{
    AnimationFrames.Add(frame);
}

void FGAFTimeline::PushAnimationSequence(const FString& nameId, int32 start, int32 end)
{
    FGAFAnimationSequence seq{ nameId, start, end };
    AnimationSequences[nameId] = seq;
}

void FGAFTimeline::PushNamedPart(uint32 objectIdRef, const FString& name)
{
    NamedParts[name] = objectIdRef;
}

void FGAFTimeline::SetSceneFps(uint32 val)
{
    SceneFps = val;
}

void FGAFTimeline::SetSceneWidth(uint32 val)
{
    SceneWidth = val;
}

void FGAFTimeline::SetSceneHeight(uint32 val)
{
    SceneHeight = val;
}

void FGAFTimeline::SetSceneColor(const FColor& val)
{
    SceneColor = val;
}

void FGAFTimeline::SetLinkageName(const FString& linkageName)
{
    LinkageName = linkageName;
}

const FGAFAnimationObjects_t& FGAFTimeline::GetAnimationObjects() const
{
    return AnimationObjects;
}

const FGAFAnimationMasks_t& FGAFTimeline::GetAnimationMasks() const
{
    return AnimationMasks;
}

const FGAFAnimationFrames_t& FGAFTimeline::GetAnimationFrames() const
{
    return AnimationFrames;
}

const FGAFAnimationSequences_t& FGAFTimeline::GetAnimationSequences() const
{
    return AnimationSequences;
}

const FGAFNamedParts_t& FGAFTimeline::GetNamedParts() const
{
    return NamedParts;
}

const FGAFTextureAtlases_t& FGAFTimeline::GetTextureAtlases() const
{
    return TextureAtlases;
}

uint32 FGAFTimeline::GetFramesCount() const
{
    return FramesCount;
}

const FGAFRect& FGAFTimeline::GetRect() const
{
    return Aabb;
}

const FGAFPoint_t& FGAFTimeline::GetPivot() const
{
    return Pivot;
}

const FString& FGAFTimeline::GetLinkageName() const
{
    return LinkageName;
}

const FGAFAnimationSequence* FGAFTimeline::GetSequence(const FString& name) const
{
    FGAFAnimationSequences_t::const_iterator it = AnimationSequences.find(name);

    if (it != AnimationSequences.end())
    {
        return &it->second;
    }

    return nullptr;
}

const FGAFAnimationSequence* FGAFTimeline::GetSequenceByLastFrame(uint32 frame) const
{
    if (AnimationSequences.empty())
    {
        return nullptr;
    }

    for (FGAFAnimationSequences_t::const_iterator i = AnimationSequences.begin(), e = AnimationSequences.end(); i != e; ++i)
    {
        if (i->second.EndFrameNumber == frame + 1)
        {
            return &i->second;
        }
    }

    return nullptr;
}

const FGAFAnimationSequence* FGAFTimeline::GetSequenceByFirstFrame(uint32 frame) const
{
    if (AnimationSequences.empty())
    {
        return nullptr;
    }

    for (FGAFAnimationSequences_t::const_iterator i = AnimationSequences.begin(), e = AnimationSequences.end(); i != e; ++i)
    {
        if (i->second.StartFrameNumber == frame + 1)
        {
            return &i->second;
        }
    }

    return nullptr;
}

FGAFTimelineWeakPtr FGAFTimeline::GetParent() const
{
    return Parent;
}

FGAFTextureAtlas* FGAFTimeline::GetTextureAtlas()
{
    return CurrentTextureAtlas;
}

void FGAFTimeline::LoadImages(float desiredAtlasScale)
{
    if (TextureAtlases.Num() > 0)
    {
        CurrentTextureAtlas = nullptr;
        UsedAtlasContentScaleFactor = desiredAtlasScale;
        return;
    }

    ChooseTextureAtlas(desiredAtlasScale);
}

void FGAFTimeline::ChooseTextureAtlas(float desiredAtlasScale)
{
    float atlasScale = TextureAtlases[0]->GetScale();

    CurrentTextureAtlas = TextureAtlases[0];

    const int32 count = TextureAtlases.Num();

    for (int32 i = 1; i < count; ++i)
    {
        float as = TextureAtlases[i]->GetScale();
        if (FMath::Abs(atlasScale - desiredAtlasScale) > FMath::Abs(as - desiredAtlasScale))
        {
            CurrentTextureAtlas = TextureAtlases[i];
            atlasScale = as;
        }
    }

    UsedAtlasContentScaleFactor = atlasScale;
}
