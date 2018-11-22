/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "CoreMinimal.h"
#include "MultiPackerRuntimeOutputNodeBase.generated.h"

class UMultiPackerRuntimeGraph;
class UMultiPackerRuntimeMatNode;
class FMultiPackerEditorThumbnail;
class UTextureRenderTarget2D;

UCLASS()
class MULTIPACKERRUNTIME_API UMultiPackerRuntimeOutputNodeBase : public UObject
{
	GENERATED_BODY()

public:
	void SetThumnailOutput(UTexture2D* Input, bool rectangle);
	TArray<UMultiPackerRuntimeMatNode*> ChildrenMatNodes;
	UTexture2D* TextureInput;
	bool Thumbnailrectangled = false;
	bool RectangleTiles = false;
	bool CanProcess = false;
	int32 TotalTiles = 1;

private:
	int32 VSize = 32;
	int32 HSize = 32;
	int32 sizeOut = 32;
	FString GetNodeTitle() const;
	UMultiPackerRuntimeGraph* GetGraph() const;
	bool NodeTileSearchFail();
};