
#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFFileImportFactory.h"
#include "GAFAsset.h"

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

UObject* UGAFFileImportFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, 
    EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer,
    const uint8* BufferEnd, FFeedbackContext* Warn)
{
    Flags |= RF_Transactional;

    FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

    const FString LongPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetPathName());
    UGAFAsset* result = NewObject<UGAFAsset>(InParent, InName, Flags);

    result->InitWithGAFData(Buffer, BufferEnd - Buffer, LongPackagePath);

    //////////////////////////////////////////////////////////////////////////

    //FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");



    //////////////////////////////////////////////////////////////////////////

    FEditorDelegates::OnAssetPostImport.Broadcast(this, result);
    return result;
}

