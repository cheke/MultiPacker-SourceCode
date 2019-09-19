/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include "Framework/Commands/Commands.h"
#include "MultiPackerTabStyle.h"


class FMultiPackerCTCommands : public TCommands<FMultiPackerCTCommands>
{
public:

	FMultiPackerCTCommands()
		: TCommands<FMultiPackerCTCommands>(TEXT("MultiPacker"), NSLOCTEXT("Compile", "MultiPacker", "MultiPacker Plugin"), NAME_None, FMultiPackerTabStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
	TSharedPtr< FUICommandInfo > ProcessTexture;
	TSharedPtr< FUICommandInfo > PreviewRed;
	TSharedPtr< FUICommandInfo > PreviewGreen;
	TSharedPtr< FUICommandInfo > PreviewBlue;
	TSharedPtr< FUICommandInfo > PreviewAlpha;
};