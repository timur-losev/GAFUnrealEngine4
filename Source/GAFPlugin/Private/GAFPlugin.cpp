// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GAFPluginPrivatePCH.h"
#include "GAFPluginEdMode.h"

#define LOCTEXT_NAMESPACE "FGAFPluginModule"

void FGAFPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FGAFPluginEdMode>(FGAFPluginEdMode::EM_GAFPluginEdModeId, LOCTEXT("GAFPluginEdModeName", "GAFPluginEdMode"), FSlateIcon(), true);
}

void FGAFPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FGAFPluginEdMode::EM_GAFPluginEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGAFPluginModule, GAFPlugin)