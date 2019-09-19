/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include "MultiPackerBaseEnums.h"
#include "MultiPackerDetailPanel.h"
#include "Toolkits/AssetEditorToolkit.h"
#include <UObject/GCObject.h>

class IDetailsView;
class FSpawnTabArgs;
class FAssetToolsModule;
class UTilePointer;
struct FSlateImageBrush;
class SImage;
class FToolBarBuilder;
class UMultiPackerDetailPanel;

class FMultiPackerChannelTab : public FAssetEditorToolkit, public FGCObject
{
public:
	FMultiPackerChannelTab();
	virtual ~FMultiPackerChannelTab();
	
	void InitGenericGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost);
	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	// End of IToolkit interface

	UPROPERTY()
		UMultiPackerDetailPanel* PropertyMPChannel;

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// End of FAssetEditorToolkit

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface
private:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	//Extend Toolbar
	void FillToolbar(/*FToolBarBuilder& ToolbarBuilder*/);

	TSharedPtr<class IDetailsView> PropertyWidget; 
	TSharedPtr<FSlateImageBrush> PreviewBrush;
	TSharedPtr<SImage> ImageWidget;
	TSharedPtr<SBorder> PreviewWidget;
	void OnClickButton();

	EChannelSelectionInput GetChannelEnum(EMPChannelMaskParameterColor InEnum);
	UTilePointer* ProcessTextureChannel(UTexture2D* InTexture, const int32 InSizeVertical, const int32 InSizeHorizontal, EMPChannelMaskParameterColor InChannel);
	TArray<FString> TexturePackageName(FAssetToolsModule& AssetToolsModule);
	void PreviewRedTexture();
	void PreviewGreenTexture();
	void PreviewBlueTexture();
	void PreviewAlphaTexture();
	void PreviewTexture(EMPChannelMaskParameterColor InEnum, UTexture2D* InTexture, bool InvertColors);
	void SetPreviewTexture(UTexture2D* InPreviewTexture);
	//Toolbar Binds
	bool CanProcessTexture();
	bool CanPreviewRedTexture();
	bool CanPreviewGreenTexture();
	bool CanPreviewBlueTexture();
	bool CanPreviewAlphaTexture();
};
