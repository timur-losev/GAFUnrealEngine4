#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFTagDefineAnimationFrames.h"

#include "GAFAsset.h"
#include "GAFTimeline.h"
#include "GAFSubobjectState.h"
#include "GAFStream.h"
#include "GAFData.h"
#include "GAFAnimationFrame.h"
#include "GAFPrimitiveDeserializer.h"

FGAFSubobjectState* FGAFTagDefineAnimationFrames::ExtractState(FGAFStream* in)
{
    FGAFSubobjectState* state = new FGAFSubobjectState();

    float ctx[7];

    char hasColorTransform = in->ReadUByte();
    char hasMasks = in->ReadUByte();
    char hasEffect = in->ReadUByte();

    state->ObjectIDRef = in->ReadU32();
    state->ZIndex = in->ReadS32();
    state->GetColorMults()[GAFCTI_A] = in->ReadFloat();

    FGAFPrimitiveDeserializer::Deserialize(in, &state->AffineTransform);

    if (hasColorTransform)
    {
        in->ReadNBytesOfT(ctx, sizeof(float) * 7);

        float* ctxOff = state->GetColorOffsets();
        float* ctxMul = state->GetColorMults();

        ctxOff[GAFCTI_A] = ctx[0];

        ctxMul[GAFCTI_R] = ctx[1];
        ctxOff[GAFCTI_R] = ctx[2];

        ctxMul[GAFCTI_G] = ctx[3];
        ctxOff[GAFCTI_G] = ctx[4];

        ctxMul[GAFCTI_B] = ctx[5];
        ctxOff[GAFCTI_B] = ctx[6];
    }
    else
    {
        state->CTXMakeIdentity();
    }

    if (hasEffect)
    {
        uint8 effects = in->ReadUByte();

        for (unsigned int e = 0; e < effects; ++e)
        {
            FGAFFilterType type = static_cast<FGAFFilterType>(in->ReadU32());

            if (type == FGAFFilterType::Blur)
            {
                FGAFSize p;
                FGAFPrimitiveDeserializer::Deserialize(in, &p);
//                 FGAFBlurFilterData* blurFilter = new GAFBlurFilterData();
//                 blurFilter->blurSize = p;
//                 state->pushFilter(blurFilter);
            }
            else if (type == FGAFFilterType::ColorMatrix)
            {
                //GAFColorColorMatrixFilterData* colorFilter = new GAFColorColorMatrixFilterData();
                for (uint32 i = 0; i < 4; ++i)
                {
                    for (uint32 j = 0; j < 4; ++j)
                    {
                        /*colorFilter->matrix[j * 4 + i] = */in->ReadFloat();
                    }

                    /*colorFilter->matrix2[i] = */in->ReadFloat();/* / 255.f*/;
                }

                //state->pushFilter(colorFilter);
            }
            else if (type == FGAFFilterType::Glow)
            {
                //GAFGlowFilterData* filter = new GAFGlowFilterData();
                uint32 clr = in->ReadU32();

//                 FGAFPrimitiveDeserializer::TranslateColor(filter->color, clr);
//                 filter->color.a = 1.f;

                //FGAFPrimitiveDeserializer::deserialize(in, &filter->blurSize);
                FGAFSize dummy;
                FGAFPrimitiveDeserializer::Deserialize(in, &dummy);

                /*filter->strength = */in->ReadFloat();
                /*filter->innerGlow = */in->ReadUByte();// ? true : false;
                /*filter->knockout = */in->ReadUByte();// ? true : false;

                //state->pushFilter(filter);
            }
            else if (type == FGAFFilterType::DropShadow)
            {
                //GAFDropShadowFilterData* filter = new GAFDropShadowFilterData();
                uint32 clr = in->ReadU32();

                //FGAFPrimitiveDeserializer::TranslateColor(filter->color, clr);
                //filter->color.a = 1.f;

                FGAFSize dummy;
                //FGAFPrimitiveDeserializer::Deserialize(in, &filter->blurSize);
                FGAFPrimitiveDeserializer::Deserialize(in, &dummy);
                /*filter->angle = */in->ReadFloat();
                /*filter->distance = */in->ReadFloat();
                /*filter->strength = */in->ReadFloat();
                /*filter->innerShadow = */in->ReadUByte();// ? true : false;
                /*filter->knockout = */in->ReadUByte();// ? true : false;

                //state->pushFilter(filter);
            }
        }
    }

    if (hasMasks)
    {
        state->MaskObjectIDRef = in->ReadU32();
    }

    return state;
}

FGAFTagDefineAnimationFrames::~FGAFTagDefineAnimationFrames()
{
    for (States_t::TIterator it(CurrentStates); it; ++it)
    {
        it.Value()->Release();
    }
}

void FGAFTagDefineAnimationFrames::Read(FGAFStream* in, UGAFAsset* asset, FGAFTimelinePtr timeline)
{
    (void)asset;
    in->ReadU32(); // read count. Unused here

    if (timeline->GetAnimationObjects().empty()) return;

    for (FGAFAnimationObjects_t::const_iterator i = timeline->GetAnimationObjects().begin(), e = timeline->GetAnimationObjects().end(); i != e; ++i)
    {
        uint32 objectId = i->first;
        FGAFSubobjectState *state = new FGAFSubobjectState();
        state->InitEmpty(objectId);

        CurrentStates.Add(objectId, state);
    }

    const uint16 totalFrameCount = in->GetInput()->GetHeader().FramesCount;

    uint32 frameNumber = in->ReadU32();

    for (uint32 i = 0; i < totalFrameCount; ++i)
    {
        if ((frameNumber - 1) == i)
        {
            uint32 numObjects = in->ReadU32();

            typedef std::list<FGAFSubobjectState*> StatesList_t;
            StatesList_t statesList;

            for (unsigned int j = 0; j < numObjects; ++j)
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

            if (in->GetPosition() < in->GetTagExpectedPosition())
                frameNumber = in->ReadU32();
        }

        FGAFAnimationFrame* frame = new FGAFAnimationFrame();

        for (States_t::TIterator it(CurrentStates); it; ++it)
        {
            frame->PushObjectState(it.Value());
        }

        timeline->PushAnimationFrame(frame);
    }
}
