
#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFAsset.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"
#include "GAFLoader.h"
#include "GAFTextureAtlas.h"
#include "GAFTextureAtlasElement.h"

#include "KismetProceduralMeshLibrary.h"

DEFINE_LOG_CATEGORY(LogGAFAsset);

UGAFAsset::UGAFAsset(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

#if WITH_EDITORONLY_DATA

void UGAFAsset::PostInitProperties()
{
    if (!HasAnyFlags(RF_ClassDefaultObject))
    {
        AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }

    Super::PostInitProperties();
}

void UGAFAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
    if (AssetImportData)
    {
        OutTags.Add(FAssetRegistryTag(SourceFileTagName(), AssetImportData->GetSourceData().ToJson(), FAssetRegistryTag::TT_Hidden));
    }

    Super::GetAssetRegistryTags(OutTags);
}

void UGAFAsset::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);

    const bool IsLoading = Ar.IsLoading();
    const bool IsSaving = Ar.IsSaving();

    if (Ar.IsLoading() && Ar.UE4Ver() < VER_UE4_ASSET_IMPORT_DATA_AS_JSON && !AssetImportData)
    {
        AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
    }

    if (IsSaving)
    {

    }
}

void UGAFAsset::PushTimeline(uint32 timelineIdRef, FGAFTimelinePtr timeline)
{
    Timelines[timelineIdRef] = timeline;
}

void UGAFAsset::PushTextureAtlas(FGAFTextureAtlas* atlas)
{
    TextureAtlases.Add(atlas);
}

void UGAFAsset::SetRootTimeline(FGAFTimelinePtr tl)
{
    RootTimeline = tl;
    Header.Pivot = RootTimeline->GetPivot();
    Header.FrameSize = RootTimeline->GetRect();
}

bool UGAFAsset::SetRootTimeline(const FString& name)
{
    for (FGAFTimelines_t::iterator i = Timelines.begin(), e = Timelines.end(); i != e; i++)
    {
        const FString& tl_name = i->second->GetLinkageName();
        if (tl_name == name)
        {
            SetRootTimeline(i->second);
            return true;
        }
    }
    return false;
}

bool UGAFAsset::SetRootTimeline(uint32 id)
{
    FGAFTimelines_t::iterator timeline = Timelines.find(id);
    if (timeline != Timelines.end())
    {
        SetRootTimeline(timeline->second);
        return true;
    }
    return false;
}


void UGAFAsset::GenerateQuad(float w, float h, TArray<FVector>& Vertices, TArray<int32>& Triangles,
    TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents)
{
    Vertices.Reset();
    Triangles.Reset();
    Normals.Reset();
    UVs.Reset();
    Tangents.Reset();

    Vertices.AddUninitialized(4);
    Normals.AddUninitialized(4);
    Tangents.AddUninitialized(4);

    // Generate verts
    FVector QuadVerts[4];

    /*
    0,0        w,0
    .     .

    .     .
    0,h        w,h
    */

    QuadVerts[0] = FVector(0.f, 0.f, 0.f);
    QuadVerts[1] = FVector(w, 0.f, 0.f);
    QuadVerts[2] = FVector(0.f, 0.f, h);
    QuadVerts[3] = FVector(w, 0.f, h);

    Vertices[0] = QuadVerts[0];
    Vertices[1] = QuadVerts[1];
    Vertices[2] = QuadVerts[2];
    Vertices[3] = QuadVerts[3];

    Triangles.Add(0);
    Triangles.Add(1);
    Triangles.Add(2);

    Triangles.Add(3);
    Triangles.Add(2);
    Triangles.Add(1);

    Normals[0] = Normals[1] = Normals[2] = Normals[3] = FVector(0, 0, 1);
    Tangents[0] = Tangents[1] = Tangents[2] = Tangents[3] = FProcMeshTangent(0.f, -1.f, 0.f);
}

void UGAFAsset::_InstantiateObject(uint32 Id, FGAFCharacterType Type, uint32 Reference, bool Mask, UBlueprint* BlueprintObject)
{
    if (Type == FGAFCharacterType::Timeline)
    {
        UE_LOG(LogGAFAsset, Log, TEXT("Timeline reached"));
    }
    else if (Type == FGAFCharacterType::Texture)
    {
        UE_LOG(LogGAFAsset, Log, TEXT("Texture reached"));

        FGAFTextureAtlas* Atlas = RootTimeline->GetTextureAtlas();
        const auto& ElementsMap = Atlas->GetElements();
        const FGAFTextureAtlasElement* Element = *ElementsMap.Find(Reference);

        check(Element != nullptr);

        FString NodeName = TEXT("Test") + FString::FromInt(Id);

        static bool bIsRootAdded = false;

        if (!bIsRootAdded)
        {
            USCS_Node* DefaultSceneRootNode =
                BlueprintObject->SimpleConstructionScript->CreateNode(
                    USceneComponent::StaticClass(), TEXT("DefaultSceneRootNode"));

            CastChecked<USceneComponent>(DefaultSceneRootNode->ComponentTemplate)->bVisualizeComponent = true;

            BlueprintObject->SimpleConstructionScript->AddNode(DefaultSceneRootNode);

            bIsRootAdded = true;
        }

        USCS_Node* ProceduralQuadNodeScriptNode =
            BlueprintObject->SimpleConstructionScript->CreateNode(
                UProceduralMeshComponent::StaticClass(), FName(*NodeName));

        CastChecked<USceneComponent>(ProceduralQuadNodeScriptNode->ComponentTemplate)->bVisualizeComponent = true;
        BlueprintObject->SimpleConstructionScript->AddNode(ProceduralQuadNodeScriptNode);

        {
            TArray<FVector> Vertices;
            TArray<int32> Indeces;
            TArray<FVector> Normals;
            TArray<FVector2D> UVs;
            TArray<FProcMeshTangent> Tangents;
            GenerateQuad(Element->Bounds.Size.Width, Element->Bounds.Size.Height, Vertices, Indeces, Normals, UVs, Tangents);

            UProceduralMeshComponent* ProceduralMesh = CastChecked<UProceduralMeshComponent>(ProceduralQuadNodeScriptNode->ComponentTemplate);
            ProceduralMesh->CreateMeshSection(0, Vertices, Indeces, Normals, UVs, TArray<FColor>(), Tangents, false);
        }
    }
}

UGAFAsset::~UGAFAsset()
{
    GAF_RELEASE_ARRAY(FGAFTextureAtlases_t, TextureAtlases);
}

void UGAFAsset::SetHeader(FGAFHeader& h)
{
    Header = h;
}

const FGAFHeader& UGAFAsset::GetHeader() const
{
    return Header;
}

bool UGAFAsset::InitWithGAFData(const uint8_t* data, uint32_t len, const FString& longPath)
{
    FGAFLoader* loader = new FGAFLoader();
    loader->LoadData(data, len, this);
    delete loader;

    return true;
}

void UGAFAsset::SetSceneFps(uint32 v)
{
    SceneFps = v;
}

void UGAFAsset::SetSceneWidth(uint32 v)
{
    SceneWidth = v;
}

void UGAFAsset::SetSceneHeight(uint32 v)
{
    SceneHeight = v;
}

void UGAFAsset::SetSceneColor(const FColor& v)
{
    SceneColor = v;
}

void UGAFAsset::InstantiateObject(const FGAFAnimationObjects_t& Objs, const FGAFAnimationMasks_t& Masks, UBlueprint* BlueprintObject)
{
    RootTimeline->LoadImages(1);

    for (const FGAFAnimationObjects_t::value_type& AnimObjectPair : Objs)
    {
        const FGAFAnimationObjectEx_t& AnimObject = AnimObjectPair.second;
        FGAFCharacterType CharacterType = std::get<1>(AnimObject);
        uint32 Reference = std::get<0>(AnimObject);
        uint32 ObjectId = AnimObjectPair.first;

        check(BlueprintObject != nullptr);

        _InstantiateObject(ObjectId, CharacterType, Reference, false, BlueprintObject);
    }
}

void UGAFAsset::ConstructObject(UBlueprint* BlueprintObject)
{
    InstantiateObject(RootTimeline->GetAnimationObjects(), RootTimeline->GetAnimationMasks(), BlueprintObject);
}

#endif