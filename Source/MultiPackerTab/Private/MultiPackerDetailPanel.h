/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include <Engine/Texture2D.h>
#include "MultiPackerBaseEnums.h"
#include "MultiPackerDetailPanel.generated.h"

UENUM(BlueprintType)
enum class EMPChannelMaskParameterColor : uint8
{
	Red,
	Green,
	Blue,
	Alpha,
};

UCLASS(NonTransient)
class UMultiPackerDetailPanel : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Input")
		UTexture2D* TextureRed;
	UPROPERTY(EditAnywhere, Category = "Input")
		EMPChannelMaskParameterColor MaskTextureRed;

	UPROPERTY(EditAnywhere, Category = "Input")
		UTexture2D* TextureGreen;
	UPROPERTY(EditAnywhere, Category = "Input")
		EMPChannelMaskParameterColor MaskTextureGreen;

	UPROPERTY(EditAnywhere, Category = "Input")
		UTexture2D* TextureBlue;
	UPROPERTY(EditAnywhere, Category = "Input")
		EMPChannelMaskParameterColor MaskTextureBlue;

	UPROPERTY(EditAnywhere, Category = "Input")
		bool AlphaChannel;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (EditCondition = AlphaChannel))
		UTexture2D* TextureAlpha;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (EditCondition = AlphaChannel))
		EMPChannelMaskParameterColor MaskTextureAlpha;

	UPROPERTY(EditAnywhere, Category = "Output")
		FDirectoryPath TargetDirectory;
	UPROPERTY(EditAnywhere, Category = "Output")
		FString TextureName = "MultiPacker_CP";
	UPROPERTY(EditAnywhere, Category = "Output")
		UTexture2D* Texture;

	UPROPERTY(EditAnywhere, Category = "Output")
		ETextureSizeOutputPersonal SizeVertical = ETextureSizeOutputPersonal::EMCE_Option5;
	UPROPERTY(EditAnywhere, Category = "Output")
		ETextureSizeOutputPersonal SizeHorizontal = ETextureSizeOutputPersonal::EMCE_Option5;
};


