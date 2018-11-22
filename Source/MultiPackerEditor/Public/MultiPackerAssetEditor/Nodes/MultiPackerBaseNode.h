/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "EdGraph/EdGraphNode.h"
#include "MultiPackerBaseEnums.h"
#include "MultiPackerBaseNode.generated.h"

class FAssetThumbnail;
class UMultiPackerEdGraph;
class UTilePointer;

UCLASS(MinimalAPI)
class UMultiPackerBaseNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	TSharedPtr<class FAssetThumbnail> AssetThumbnail;

	UPROPERTY(Transient)
		TArray<UTilePointer*> Tiles;
	UPROPERTY(Transient)
		bool TilesProcessed = false;
	UPROPERTY(Transient)
		bool TilesChanged = false;

	UMultiPackerEdGraph* GetGenericGraphEdGraph();
	FText GetDescription() const;
	virtual bool GetMsdf();
	virtual bool IsNodeSelectedSdf();
	virtual FTileThumbDatabase GetTileThumbDatabase(uint16 num);
#if WITH_EDITOR  
	//TilePointer Side class
	TArray<UTilePointer*> GetTiles(const uint16 size);
	void SetDatabase();
	bool TilesNeedProcess();
	virtual void ProcessTiles();
	bool IsTilesSDF();

	virtual bool PinConnected();
	virtual bool IsObjectValid();
#endif
	bool GraphSDF = false;
	uint16 SizeTile = 0;

	virtual uint16 GetNumberTiles();
};