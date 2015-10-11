#pragma once

struct FTags
{
    enum Enum
    {
        TagEnd = 0,
        TagDefineAtlas = 1,
        TagDefineAnimationMasks = 2,
        TagDefineAnimationObjects = 3,
        TagDefineAnimationFrames = 4,
        TagDefineNamedParts = 5,
        TagDefineSequences = 6,
        TagDefineTextFields = 7,
        TagDefineAtlas2 = 8,
        TagDefineStage = 9,
        TagDefineAnimationObjects2 = 10,
        TagDefineAnimationMasks2 = 11,
        TagDefineAnimationFrames2 = 12,
        TagDefineTimeline = 13,
        TagDefineSounds = 14,
        TagDefineAtlas3 = 15,
    };

    static FString ToString(Enum e)
    {
        switch (e)
        {
        case FTags::TagEnd:
            return TEXT("TagEnd");

        case FTags::TagDefineAtlas:
            return TEXT("TagDefineAtlas");

        case FTags::TagDefineAnimationMasks:
            return TEXT("TagDefineAnimationMasks");

        case FTags::TagDefineAnimationObjects:
            return TEXT("TagDefineAnimationObjects");

        case FTags::TagDefineAnimationFrames:
            return TEXT("TagDefineAnimationFrames");

        case FTags::TagDefineNamedParts:
            return TEXT("TagDefineNamedParts");

        case FTags::TagDefineSequences:
            return TEXT("TagDefineSequences");

        case FTags::TagDefineStage:
            return TEXT("TagDefineStage");

        case FTags::TagDefineAtlas2:
            return TEXT("TagDefineAtlas2");

        case FTags::TagDefineTimeline:
            return TEXT("TagDefineTimeline");

        case FTags::TagDefineTextFields:
            return TEXT("TagDefineTextFields");

        case FTags::TagDefineAnimationMasks2:
            return TEXT("TagDefineAnimationMasks2");

        case FTags::TagDefineAnimationObjects2:
            return TEXT("TagDefineAnimationObjects2");

        case FTags::TagDefineAnimationFrames2:
            return TEXT("TagDefineAnimationFrames2");

        case FTags::TagDefineSounds:
            return TEXT("TagDefineSounds");

        case FTags::TagDefineAtlas3:
            return TEXT("TagDefineAtlas3");

        default:
            return TEXT("This tag is not yet implemented");
        }
    }
};
