// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GAFPluginPrivatePCH.h"
#include "GAFPluginEdMode.h"
#include "GAFPluginEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"

const FEditorModeID FGAFPluginEdMode::EM_GAFPluginEdModeId = TEXT("EM_GAFPluginEdMode");

FGAFPluginEdMode::FGAFPluginEdMode()
{

}

FGAFPluginEdMode::~FGAFPluginEdMode()
{

}

void FGAFPluginEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FGAFPluginEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FGAFPluginEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FGAFPluginEdMode::UsesToolkits() const
{
	return true;
}




