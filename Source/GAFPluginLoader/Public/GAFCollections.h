#pragma once

#include "GAFAnimationSequence.h"

class FGAFTextureAtlas;
class FGAFAnimationFrame;

ForwardSharedPointer(FGAFTimeline);

enum class FGAFFilterType : uint32
{
    DropShadow = 0,
    Blur = 1,
    Glow = 2,
    ColorMatrix = 6
};

enum class FGAFCharacterType : uint32
{
    Texture = 0,
    TextField,
    Timeline
};

enum class FGAFObjectType : uint32
{
    None = 0,
    MovieClip,
    Mask
};

enum class FGAFRotation : int8
{
    CCW_90 = -1,
    NONE = 0,
    CW_90 = 1,
};

enum class FGAFActionType : int32
{
    //0 - stop, 1 - play, 2 - gotoAndStop, 3 - gotoAndPlay, 4 - dispatchEvent
    None = -1,
    Stop = 0,
    Play,
    GotoAndStop,
    GotoAndPlay,
    DispatchEvent,
};

enum
{
    GAFFirstFrameIndex = 0
};

struct FStringHasher
{
    uint32 operator()(const FString& str) const
    {
        return FCrc::StrCrc32(*str);
    }
};

typedef std::tuple<uint32, FGAFCharacterType>               FGAFAnimationObjectEx_t;
typedef TArray<FGAFTextureAtlas*>                           FGAFTextureAtlases_t;
typedef std::unordered_map<uint32, FGAFAnimationObjectEx_t> FGAFAnimationMasks_t;
typedef std::unordered_map<uint32, FGAFAnimationObjectEx_t> FGAFAnimationObjects_t;
typedef TArray<FGAFAnimationFrame*>                         FGAFAnimationFrames_t;
typedef std::unordered_map<FString, uint32, FStringHasher>                 FGAFNamedParts_t;
typedef std::unordered_map<FString, FGAFAnimationSequence, FStringHasher>  FGAFAnimationSequences_t;
typedef std::unordered_map<uint32, FGAFTimelinePtr>           FGAFTimelines_t;


#define GAF_RELEASE_ARRAY(__type, __instance)            \
        for (__type::TIterator it(__instance); it; ++it) \
        {                                                \
            delete *it;                                  \
        }                                                \
        __instance.Empty();

#define GAF_RELEASE_MAP(__type, __instance)             \
        for (__type::iterator i = __instance.begin(), e = __instance.end(); i != e; ++i) {\
            delete i->second;                                   \
        } __instance.clear()