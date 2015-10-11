#pragma once

#include "GAFFileImportFactory.generated.h"

UCLASS()
class UGAFFileImportFactory : public UFactory
{
    GENERATED_UCLASS_BODY()

    virtual FText GetToolTip() const override;

    virtual bool FactoryCanImport(const FString& Filename) override;

    virtual UObject* FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;

protected:
    TSharedPtr<class FGAFFileImporter> m_gafFileImporter;
};