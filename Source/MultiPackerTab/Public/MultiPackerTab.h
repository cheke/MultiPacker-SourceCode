/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class UMultiPackerChannelTab;

class FMultiPackerTab : public  IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	void PluginButtonClicked();
	
private:

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
		
	// Holds the menu extensibility manager.
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;

	// Holds the tool bar extensibility manager.
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
private:
	TSharedPtr<class FUICommandList> PluginCommands;

	UMultiPackerChannelTab* TabSpawn;
};