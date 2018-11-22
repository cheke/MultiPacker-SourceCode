/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "MultiPackerRuntimeOutputNode.generated.h"

class UMultiPackerRuntimeEdGraph;
class UMultiPackerRuntimeOutputNodeBase;
class FAssetThumbnail;

UCLASS(MinimalAPI)
class UMultiPackerRuntimeOutputNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, instanced, Category = "MultiPacker")
	UMultiPackerRuntimeOutputNodeBase* MultiPackerNode;

	TSharedPtr<class FAssetThumbnail> AssetThumbnail;

	void SetGenericGraphNode(UMultiPackerRuntimeOutputNodeBase* InNode);
	void ProcessThumbnail(UTexture2D* Input);
	FText GetDescription() const;
	void AllocateDefaultPins() override;
	void NodeConnectionListChanged() override;
	bool CanUserDeleteNode()const override;

private:
	UMultiPackerRuntimeEdGraph* GetGenericGraphEdGraph() const;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const;
	UObject* GetNodeAssetObject(UObject* Outer) const;
	UObject* GetThumbnailAssetObject() const;
	static UEdGraphPin* PinDefault;
	bool NodeOutput = false;
};