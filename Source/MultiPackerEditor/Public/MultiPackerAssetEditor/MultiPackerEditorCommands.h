/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include "Framework/Commands/GenericCommands.h"
class FMultiPackerEditorCommands : public TCommands<FMultiPackerEditorCommands>
{
public:
	/** Constructor */
	FMultiPackerEditorCommands()
		: TCommands<FMultiPackerEditorCommands>("GenericGraphEditor", NSLOCTEXT("Contexts", "MultiPackerEditor", "MultiPacker Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> CompileGraphNodes;
	TSharedPtr<FUICommandInfo> CompileGraphMaterial;
	TSharedPtr<FUICommandInfo> PatreonButton;
	virtual void RegisterCommands() override;
};