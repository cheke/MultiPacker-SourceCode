/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include "MultiPackerBaseEnums.h"
#include "MultiPackerDetailPanel.h"
#include <Framework/SlateDelegates.h>
#include "MultiPackerChannelTab.generated.h"

class SMPTextureWidget;
class IDetailsView;
class FSpawnTabArgs;
class FAssetToolsModule;
class UTilePointer;

UCLASS()
class UMultiPackerChannelTab : public UObject
{
	GENERATED_BODY()
public:
	void InitializeTab();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	UPROPERTY()
		UMultiPackerDetailPanel* PropertyMPChannel;
private:
	TSharedPtr<class IDetailsView> PropertyWidget;

	TSharedPtr<class SMPTextureWidget> PreviewTextureViewport;

	FReply OnClickButton();

	EChannelSelectionInput GetChannelEnum(EMPChannelMaskParameterColor InEnum);
	UTilePointer* ProcessTextureChannel(UTexture2D* InTexture, const int32 InSizeVertical, const int32 InSizeHorizontal, EMPChannelMaskParameterColor InChannel);
	TArray<FString> TexturePackageName(FAssetToolsModule& AssetToolsModule);
	TSharedRef<class SButton> CreateAndSetButton(FOnClicked InOnClicked, const TAttribute<FText>& InIcon, const TAttribute<FText>& InTooltip, const TAttribute<FText>& InButton);
	FReply PreviewRedTexture();
	FReply PreviewGreenTexture();
	FReply PreviewBlueTexture();
	FReply PreviewAlphaTexture();
	void PreviewTexture(EMPChannelMaskParameterColor InEnum, UTexture2D* InTexture, bool InvertColors);
};
