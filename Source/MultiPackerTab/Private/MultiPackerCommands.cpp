/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "MultiPackerCommands.h"

#define LOCTEXT_NAMESPACE "FMultiPackerCTCommands"

void FMultiPackerCTCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MultiPacker", "Bring up MultiPacker Channel Pack Tab", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ProcessTexture, "MultiPackerChannelTab", "Process the Channel Pack", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PreviewRed, "MultiPackerChannelTab", "Preview the Red Texture filtered by Channel Selected", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PreviewGreen, "MultiPackerChannelTab", "Preview the Green Texture filtered by Channel Selected", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PreviewBlue, "MultiPackerChannelTab", "Preview the Blue Texture filtered by Channel Selected", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PreviewAlpha, "MultiPackerChannelTab", "Preview the Alpha Texture filtered by Channel Selected", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
