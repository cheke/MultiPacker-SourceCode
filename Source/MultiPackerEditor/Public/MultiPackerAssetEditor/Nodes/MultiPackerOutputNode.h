/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "MultiPackerOutputNode.generated.h"

class FAssetThumbnail;
class UMultiPackerOutputNodeBase;
class UMultiPackerEdGraph;

UCLASS(MinimalAPI)
class UMultiPackerOutputNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, instanced, Category = "MultiPacker")
	UMultiPackerOutputNodeBase* MultiPackerNode;

	TSharedPtr<class FAssetThumbnail> AssetThumbnail;

	void SetGenericGraphNode(UMultiPackerOutputNodeBase* InNode);
	void ProcessThumbnail(UTexture2D* Input);
	FText GetDescription() const;
	void AllocateDefaultPins() override;
	void NodeConnectionListChanged() override;
	bool CanUserDeleteNode()const override;

private:
	UMultiPackerEdGraph* GetGenericGraphEdGraph() const;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const;
	virtual FLinearColor GetNodeTitleColor() const override;
	UObject* GetNodeAssetObject(UObject* Outer) const;
	UObject* GetThumbnailAssetObject() const;
	static UEdGraphPin* PinDefault;
	bool NodeOutput = false;
};