
#include "GAFPluginLoaderPrivatePCH.h"

#include "AssetToolsModule.h"

#include "GAFFileImportFactory.h"
#include "GAFAsset.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/CompilerResultsLog.h"

UGAFFileImportFactory::UGAFFileImportFactory(const FObjectInitializer& init) :
Super(init)
{
    bCreateNew = false;
    bEditorImport = true;
    bText = false;

    SupportedClass = UGAFAsset::StaticClass();
    Formats.Add(TEXT("gaf;Generic 2D Animation Format"));
}

FText UGAFFileImportFactory::GetToolTip() const
{
    return NSLOCTEXT("GAF", "GAFFileImportFactory", "I'm GAF!");
}

bool UGAFFileImportFactory::FactoryCanImport(const FString& Filename)
{
    return true;
}

UObject* UGAFFileImportFactory::FactoryCreateBinary(UClass* InClass,
    UObject* InParent, FName InName, 
    EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer,
    const uint8* BufferEnd, FFeedbackContext* Warn)
{
    Flags |= RF_Transactional;

    FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

    const FString LongPackagePath = FPackageName::GetLongPackagePath(
        InParent->GetOutermost()->GetPathName());
    UGAFAsset* resultObject = NewObject<UGAFAsset>(InParent, InName, Flags);

    const FString CurrentFilename = UFactory::GetCurrentFilename();

    if (!resultObject->InitWithGAFData(Buffer, BufferEnd - Buffer, LongPackagePath))
        return nullptr;

    //////////////////////////////////////////////////////////////////////////

    FAssetToolsModule& AssetToolsModule = 
        FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");

    UFactory* BlueprintCreationFactory = NewObject<UFactory>(GetTransientPackage(), UBlueprintFactory::StaticClass());
    
    FString DefaultAssetName;
    FString PackageNameToUse;

    AssetToolsModule.Get().CreateUniqueAssetName(
        LongPackagePath + TEXT("/") + BlueprintCreationFactory->GetDefaultNewAssetName(),
        TEXT(""), PackageNameToUse, DefaultAssetName
        );

    UBlueprint* NewAssetBlueprint =  ExactCast<UBlueprint>(AssetToolsModule.Get().CreateAsset(
        DefaultAssetName, 
        LongPackagePath, 
        BlueprintCreationFactory->GetSupportedClass(), 
        BlueprintCreationFactory
        ));

    resultObject->ConstructObject(NewAssetBlueprint);


    {
        FCompilerResultsLog LogResults;
        LogResults.bLogDetailedResults = true;

        FKismetEditorUtilities::CompileBlueprint(NewAssetBlueprint, false, false, false, &LogResults);

        bool bForceMessageDisplay = ((LogResults.NumWarnings > 0) || (LogResults.NumErrors > 0)) && !NewAssetBlueprint->bIsRegeneratingOnLoad;
    }

    //////////////////////////////////////////////////////////////////////////

    FEditorDelegates::OnAssetPostImport.Broadcast(this, resultObject);
    return resultObject;
}

