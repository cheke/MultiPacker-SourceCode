/* Copyright 2019 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once
#include "CoreMinimal.h"
#include "MultiPackerBaseEnums.h"
#include <UObject/ObjectMacros.h>
#include "MultiPackerImageCore.generated.h"

class UMultiPacker;
class UMultiPackerDataBase;
class UMaterialInstanceDynamic;
class UMultiPackerLayerDatabase;
struct FLayerDatabase;

USTRUCT(BlueprintType)
struct FChannelDatabase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
		UMultiPackerDataBase* Database;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Name;
};

USTRUCT(BlueprintType)
struct FLayerDatabase
{
	GENERATED_USTRUCT_BODY()
		
public:

	UPROPERTY(EditAnywhere)
		FChannelDatabase Color;

	UPROPERTY(EditAnywhere)
		FChannelDatabase Alpha;

	UPROPERTY(AdvancedDisplay, VisibleInstanceOnly)
		bool SDF = false;

	UPROPERTY()
		bool MSDF = false;

	UPROPERTY()
		bool Multiple = false;

	UPROPERTY()
		bool SDF_Interior = false;

	UPROPERTY()
		bool SDF_Alpha = false;

	UPROPERTY()
		bool CanProcess_Interior = false;

	UPROPERTY()
		bool CanProcess_Alpha = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseColor = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Outline = false;

	UPROPERTY(BlueprintReadWrite)
		FLinearColor ColorOutline;

	UPROPERTY(BlueprintReadWrite)
		FLinearColor ColorInterior;

	UPROPERTY(EditAnywhere)
		float EdgeSoftness = 0.0f;

	UPROPERTY(BlueprintReadWrite)
		float OutlineThresold = 0.5f;

	UPROPERTY(BlueprintReadWrite)
		float SDFThresold = 0.5f; 
	
	//initialize the vars , dont work properly 
	void SetData(FLayerDatabase InStruct)
	{
		Color.Database = InStruct.Color.Database;
		Color.Name = InStruct.Color.Name;
		Alpha.Database = InStruct.Alpha.Database;
		Alpha.Name = InStruct.Alpha.Name;
		UseColor = InStruct.UseColor;
		Outline = InStruct.Outline;
		ColorOutline = InStruct.ColorOutline;
		ColorInterior = InStruct.ColorInterior;
		EdgeSoftness = InStruct.EdgeSoftness;
		OutlineThresold = InStruct.OutlineThresold;
		SDFThresold = InStruct.SDFThresold;
	}
};

UCLASS()
class MULTIPACKERRUNTIME_API UMultiPackerImageCore : public UObject
{
	GENERATED_BODY()
public:
	static UMaterialInstanceDynamic* CreateMaterial(EMultiPackerImageLayer Layer, UObject *InOuter, FLayerDatabase LayerBase, FLayerDatabase LayerAddition, FLayerDatabase LayerDetail);
	static FLayerDatabase ProcessChanges(FLayerDatabase Layer);
	static FLayerDatabase ProcessStyle(FLayerDatabase InLayer, UMultiPackerLayerDatabase* InStyle);
	static UMaterialInstanceDynamic* SetDataBaseLayer(UMaterialInstanceDynamic* MB_Function, FLayerDatabase LayerBase, FString SuFix);
private:
	static UMaterialInstanceDynamic* LayerMaterialInstance(UMaterialInstanceDynamic* MB_Function, FLayerDatabase LayerInfo, FTileDatabase layer_interior, FTileDatabase layer_alpha, FString SuFix);
};
