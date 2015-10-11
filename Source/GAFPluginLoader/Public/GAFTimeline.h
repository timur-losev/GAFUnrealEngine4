#pragma once

#include "GAFCollections.h"
#include "GAFGeometry.h"

ForwardSharedPointer(FGAFTimeline);
ForwardWeakPointer(FGAFTimeline);

class FGAFTimeline
{
public:
    
private:
    FGAFTextureAtlases_t        TextureAtlases;
    FGAFAnimationMasks_t        AnimationMasks;
    FGAFAnimationObjects_t      AnimationObjects;
    FGAFAnimationFrames_t       AnimationFrames;
    FGAFAnimationSequences_t    AnimationSequences;
    FGAFNamedParts_t            NamedParts;

    uint32                      Id;
    FGAFRect                    Aabb;
    FGAFPoint_t                 Pivot;
    uint32                      SceneFps;
    uint32                      SceneWidth;
    uint32                      SceneHeight;
    FColor                      SceneColor;
    uint32                      FramesCount;
    FString                     LinkageName;

    FGAFTextureAtlas*           CurrentTextureAtlas;

    float                       UsedAtlasContentScaleFactor;

    FGAFTimelineWeakPtr         Parent; // weak

    void                        ChooseTextureAtlas(float desiredAtlasScale);

public:

    FGAFTimeline(FGAFTimelinePtr parent, uint32 id, const FGAFRect& aabb, const FGAFPoint_t& pivot, uint32 framesCount);
    virtual ~FGAFTimeline();

    void                        PushTextureAtlas(FGAFTextureAtlas* atlas);
    void                        PushAnimationMask(uint32 objectId, uint32 elementAtlasIdRef, FGAFCharacterType charType);
    void                        PushAnimationObject(uint32 objectId, uint32 elementAtlasIdRef, FGAFCharacterType charType);
    void                        PushAnimationFrame(FGAFAnimationFrame* frame);
    void                        PushAnimationSequence(const FString& nameId, int32 start, int32 end);
    void                        PushNamedPart(uint32 objectIdRef, const FString& name);

    void                        SetSceneFps(uint32);
    void                        SetSceneWidth(uint32);
    void                        SetSceneHeight(uint32);
    void                        SetSceneColor(const FColor&);

    void                        SetLinkageName(const FString& linkageName);

    const FGAFAnimationObjects_t&   GetAnimationObjects() const;
    const FGAFAnimationMasks_t&     GetAnimationMasks() const;
    const FGAFAnimationFrames_t&    GetAnimationFrames() const;
    const FGAFAnimationSequences_t& GetAnimationSequences() const;
    const FGAFNamedParts_t&         GetNamedParts() const;
    //const FGAFTextsData_t&        GetTextsData() const;
    const FGAFTextureAtlases_t&     GetTextureAtlases() const;
    uint32                          GetFramesCount() const;

    const FGAFRect&                 GetRect() const;
    const FGAFPoint_t&              GetPivot() const;

    const FString&                   GetLinkageName() const;

    /// get GAFAnimationSequence by name specified in editor
    const FGAFAnimationSequence*    GetSequence(const FString& name) const;
    /// get GAFAnimationSequence by last frame number in sequence	
    const FGAFAnimationSequence*    GetSequenceByLastFrame(uint32 frame) const;
    /// get GAFAnimationSequence by first frame number in sequence	
    const FGAFAnimationSequence*    GetSequenceByFirstFrame(uint32 frame) const;

    FGAFTimelineWeakPtr             GetParent() const;

    FGAFTextureAtlas*               GetTextureAtlas();
    void                            LoadImages(float desiredAtlasScale);
};