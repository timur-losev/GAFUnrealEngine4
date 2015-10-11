#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineAnimationFrames2.h"

#include "GAFAsset.h"
#include "GAFTimeline.h"
#include "GAFSubobjectState.h"
#include "GAFStream.h"
#include "GAFData.h"
#include "GAFAnimationFrame.h"
#include "GAFPrimitiveDeserializer.h"

void FGAFTagDefineAnimationFrames2::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)asset;
    uint32 count = in->ReadU32();

    //assert(!timeline->getAnimationObjects().empty());

    for (FGAFAnimationObjects_t::const_iterator i = timeline->GetAnimationObjects().begin(), e = timeline->GetAnimationObjects().end(); i != e; ++i)
    {
        uint32 objectId = i->first;
        FGAFSubobjectState *state = new FGAFSubobjectState();
        state->InitEmpty(objectId);

        CurrentStates.Add(objectId, state);
    }

    uint32 frameNumber = in->ReadU32();

    for (uint32 i = 0; i < count; ++i)
    {
        char hasChangesInDisplayList = in->ReadUByte();
        char hasActions = in->ReadUByte();

        if (hasChangesInDisplayList)
        {
            uint32 numObjects = in->ReadU32();

            typedef std::list<FGAFSubobjectState*> StatesList_t;
            StatesList_t statesList;

            for (uint32 j = 0; j < numObjects; ++j)
            {
                FGAFSubobjectState* state = ExtractState(in);

                statesList.push_back(state);
            }

            for (StatesList_t::iterator it = statesList.begin(), ie = statesList.end(); it != ie; ++it)
            {
                FGAFSubobjectState* st = *it;

                FGAFSubobjectState** ps = CurrentStates.Find(st->ObjectIDRef);

                if (ps)
                {
                    (*ps)->Release();
                }

                CurrentStates.Add(st->ObjectIDRef, st);
            }
        }

        FGAFAnimationFrame* frame = new FGAFAnimationFrame();

        for (States_t::TIterator it(CurrentStates); it; ++it)
        {
            frame->PushObjectState(it.Value());
        }

        if (hasActions)
        {
            check("Unimplemented yet" && false);

//             uint32_t actionsCount = in->ReadU32();
//             for (uint32_t actionIdx = 0; actionIdx < actionsCount; actionIdx++)
//             {
//                 GAFTimelineAction action;
// 
//                 GAFActionType type = static_cast<GAFActionType>(in->readU32());
//                 std::string scope;
//                 in->readString(&scope);
// 
//                 std::vector<std::string> params;
// 
//                 unsigned int paramsLength = in->readU32();
//                 unsigned int startPosition = in->getPosition();
//                 while (paramsLength > in->getPosition() - startPosition)
//                 {
//                     std::string paramValue;
//                     in->readString(&paramValue);
//                     params.push_back(paramValue);
//                 }
// 
//                 action.setAction(type, params, scope);
//                 frame->pushTimelineAction(action);
//             }
        }

        if (in->GetPosition() < in->GetTagExpectedPosition())
            frameNumber = in->ReadU32();

        timeline->PushAnimationFrame(frame);
    }

    for (States_t::TIterator it(CurrentStates); it; ++it)
    {
        it.Value()->Release();
    }

    CurrentStates.Empty();
}
