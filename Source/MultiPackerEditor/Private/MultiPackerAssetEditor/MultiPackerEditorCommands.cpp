/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerAssetEditor/MultiPackerEditorCommands.h"

#define LOCTEXT_NAMESPACE "GenericGraphEditorCommands"

void FMultiPackerEditorCommands::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CompileGraphNodes, "Apply Changes", "Apply Changes", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CompileGraphMaterial, "Compile Material", "Compile Material", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PatreonButton, "Support on Patreon", "Support on Patreon", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
