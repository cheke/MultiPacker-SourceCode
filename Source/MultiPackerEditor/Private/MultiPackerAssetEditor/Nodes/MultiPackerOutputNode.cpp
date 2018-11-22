/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerAssetEditor/Nodes/MultiPackerOutputNode.h"
#include "MultiPackerEditorTypes.h"
#include "MultiPackerAssetEditor/MultiPackerEditorThumbnail.h"
#include "Runtime/Launch/Resources/Version.h"
#include <EdGraph/EdGraphPin.h>

#define LOCTEXT_NAMESPACE "MultiPackerOutputEdNode"

void UMultiPackerOutputNode::AllocateDefaultPins()
{
	FEdGraphPinType newPin = FEdGraphPinType();
#if (ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION <= 18)
	newPin.PinCategory = UMultiPackerEditorTypes::SPinOutputNode;
	CreatePin(EGPD_Input, newPin, FString("Output"), 0);
#endif
#if (ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION > 18)
	newPin.PinCategory = FName(*UMultiPackerEditorTypes::SPinOutputNode);
	CreatePin(EGPD_Input, newPin, FName("Output"), 0);
#endif
}

void UMultiPackerOutputNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	GetGenericGraphEdGraph()->MultiPackerNode = true;
}

UMultiPackerEdGraph* UMultiPackerOutputNode::GetGenericGraphEdGraph() const
{
	return Cast<UMultiPackerEdGraph>(GetGraph());
}

bool UMultiPackerOutputNode::CanUserDeleteNode() const
{
	return NodeOutput;
}
FText UMultiPackerOutputNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Output Texture");
}

void UMultiPackerOutputNode::SetGenericGraphNode(UMultiPackerOutputNodeBase* InNode)
{
	MultiPackerNode = InNode;
}

FText UMultiPackerOutputNode::GetDescription() const
{
	return FText::FromString("Texture");
}

UObject* UMultiPackerOutputNode::GetThumbnailAssetObject() const
{
	return MultiPackerNode->TextureInput;
}

UObject* UMultiPackerOutputNode::GetNodeAssetObject(UObject* Outer) const
{
	return  MultiPackerNode->TextureInput;
}

void UMultiPackerOutputNode::ProcessThumbnail(UTexture2D* Input)
{
	if(Input)
		MultiPackerNode->TextureInput = Input;

	//UTexture* C = MultiPackerNode->TextureInput;
	FAssetData ValueAsset = FAssetData(MultiPackerNode->TextureInput);
	AssetThumbnail = MakeShareable(new FAssetThumbnail(ValueAsset, MultiPackerNode->Thumbnailrectangled ? 256 : 128, 128, FMultiPackerEditorThumbnail::Get()));
}


#undef LOCTEXT_NAMESPACE