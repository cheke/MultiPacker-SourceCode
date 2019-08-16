/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include <Engine/Texture2D.h>
#include "MultiPackerBaseEnums.h"
#include <Engine/Texture.h>
#include <Engine/TextureDefines.h>
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
		EMPChannelMaskParameterColor MaskTextureRed = EMPChannelMaskParameterColor::Red;

	UPROPERTY(EditAnywhere, Category = "Input")
		UTexture2D* TextureGreen;
	UPROPERTY(EditAnywhere, Category = "Input")
		EMPChannelMaskParameterColor MaskTextureGreen = EMPChannelMaskParameterColor::Green;

	UPROPERTY(EditAnywhere, Category = "Input")
		UTexture2D* TextureBlue;
	UPROPERTY(EditAnywhere, Category = "Input")
		EMPChannelMaskParameterColor MaskTextureBlue = EMPChannelMaskParameterColor::Blue;

	UPROPERTY(EditAnywhere, Category = "Input")
		bool AlphaChannel;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (EditCondition = AlphaChannel))
		UTexture2D* TextureAlpha;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (EditCondition = AlphaChannel))
		EMPChannelMaskParameterColor MaskTextureAlpha = EMPChannelMaskParameterColor::Alpha;


	UPROPERTY(EditAnywhere, Category = "Output", meta = (RelativeToGameContentDir))
		FDirectoryPath TargetDirectory;
	UPROPERTY(EditAnywhere, Category = "Output")
		FString TextureName = "MultiPacker_CP";
	UPROPERTY(EditAnywhere, Category = "Output")
		UTexture2D* Texture;

	UPROPERTY(EditAnywhere, Category = "Output")
		ETextureSizeOutputPersonal SizeVertical = ETextureSizeOutputPersonal::EMCE_Option5;
	UPROPERTY(EditAnywhere, Category = "Output")
		ETextureSizeOutputPersonal SizeHorizontal = ETextureSizeOutputPersonal::EMCE_Option5;
	UPROPERTY(EditAnywhere, Category = "Output")
		TEnumAsByte<enum TextureCompressionSettings> CompressionSettings = TextureCompressionSettings::TC_Default;
	UPROPERTY(EditAnywhere, Category = "Output")
		TEnumAsByte<enum TextureFilter> TextureFilter = TF_Bilinear;
	//Defines the if the Texture will be saved as a SRGB
	UPROPERTY(EditAnywhere, Category = "Output")
		bool sRGB = false;
	UPROPERTY(EditAnywhere, Category = "Output")
		TEnumAsByte<enum TextureAddress> AddressX = TA_Clamp;
	UPROPERTY(EditAnywhere, Category = "Output")
		TEnumAsByte<enum TextureAddress> AddressY = TA_Clamp;
	UPROPERTY(EditAnywhere, Category = "Output")
		TEnumAsByte<enum ETexturePowerOfTwoSetting::Type> PowerOfTwoMode;
	UPROPERTY(EditAnywhere, Category = "Output")
		TEnumAsByte<enum TextureMipGenSettings> MipGenSettings;
};


