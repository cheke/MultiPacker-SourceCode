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

UCLASS(NonTransient, autoExpandCategories = ("InputTextures|Red", "InputTextures|Green", "InputTextures|Blue", "InputTextures|Alpha"))
class UMultiPackerDetailPanel : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "InputTextures|Red")
		UTexture2D* TextureRed;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Red")
		EMPChannelMaskParameterColor MaskTextureRed = EMPChannelMaskParameterColor::Red;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Red")
		bool InvertRed;

	UPROPERTY(EditAnywhere, Category = "InputTextures|Green")
		UTexture2D* TextureGreen;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Green")
		EMPChannelMaskParameterColor MaskTextureGreen = EMPChannelMaskParameterColor::Green;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Green")
		bool InvertGreen;

	UPROPERTY(EditAnywhere, Category = "InputTextures|Blue")
		UTexture2D* TextureBlue;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Blue")
		EMPChannelMaskParameterColor MaskTextureBlue = EMPChannelMaskParameterColor::Blue;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Blue")
		bool InvertBlue;

	UPROPERTY(EditAnywhere, Category = "InputTextures|Alpha")
		bool AlphaChannel;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Alpha", meta = (EditCondition = AlphaChannel))
		UTexture2D* TextureAlpha;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Alpha", meta = (EditCondition = AlphaChannel))
		EMPChannelMaskParameterColor MaskTextureAlpha = EMPChannelMaskParameterColor::Alpha;
	UPROPERTY(EditAnywhere, Category = "InputTextures|Alpha")
		bool InvertAlpha;
	
	UPROPERTY(EditAnywhere, Category = "OutputTexture")
		UTexture2D* Texture;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Name&Location", meta = (RelativeToGameContentDir))
		FDirectoryPath TargetDirectory;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Name&Location")
		FString TextureName = "MultiPacker_CP";

	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		ETextureSizeOutputPersonal SizeVertical = ETextureSizeOutputPersonal::EMCE_Option5;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		ETextureSizeOutputPersonal SizeHorizontal = ETextureSizeOutputPersonal::EMCE_Option5;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		TEnumAsByte<enum TextureCompressionSettings> CompressionSettings = TextureCompressionSettings::TC_Default;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		TEnumAsByte<enum TextureFilter> TextureFilter = TF_Bilinear;
	//Defines the if the Texture will be saved as a SRGB
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		bool sRGB = false;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		TEnumAsByte<enum TextureAddress> AddressX = TA_Clamp;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		TEnumAsByte<enum TextureAddress> AddressY = TA_Clamp;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		TEnumAsByte<enum ETexturePowerOfTwoSetting::Type> PowerOfTwoMode;
	UPROPERTY(EditAnywhere, Category = "OutputTexture|Settings")
		TEnumAsByte<enum TextureMipGenSettings> MipGenSettings;
};


