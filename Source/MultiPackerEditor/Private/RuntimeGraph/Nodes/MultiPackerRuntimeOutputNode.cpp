/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "RuntimeGraph/Nodes/MultiPackerRuntimeOutputNode.h"
#include "MultiPackerEditorTypes.h"
#include "MultiPackerAssetEditor/MultiPackerEditorThumbnail.h"
#include "Runtime/Launch/Resources/Version.h"
#include <EdGraph/EdGraphPin.h>

#define LOCTEXT_NAMESPACE "MultiPackerOutputEdNode"

void UMultiPackerRuntimeOutputNode::AllocateDefaultPins()
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

void UMultiPackerRuntimeOutputNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	GetGenericGraphEdGraph()->MultiPackerNode = true;
}

UMultiPackerRuntimeEdGraph* UMultiPackerRuntimeOutputNode::GetGenericGraphEdGraph() const
{
	return Cast<UMultiPackerRuntimeEdGraph>(GetGraph());
}

bool UMultiPackerRuntimeOutputNode::CanUserDeleteNode() const
{
	return NodeOutput;
}
FText UMultiPackerRuntimeOutputNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Output Texture");
}

void UMultiPackerRuntimeOutputNode::SetGenericGraphNode(UMultiPackerRuntimeOutputNodeBase* InNode)
{
	MultiPackerNode = InNode;
}

FText UMultiPackerRuntimeOutputNode::GetDescription() const
{
	return FText::FromString("Texture");
}

UObject* UMultiPackerRuntimeOutputNode::GetThumbnailAssetObject() const
{
	return MultiPackerNode->TextureInput;
}

UObject* UMultiPackerRuntimeOutputNode::GetNodeAssetObject(UObject* Outer) const
{
	return  MultiPackerNode->TextureInput;
}

void UMultiPackerRuntimeOutputNode::ProcessThumbnail(UTexture2D* Input)
{
	if(Input)
		MultiPackerNode->TextureInput = Input;

	AssetThumbnail = MakeShareable(new FAssetThumbnail(MultiPackerNode->TextureInput, MultiPackerNode->Thumbnailrectangled ? 256 : 128, 128, FMultiPackerEditorThumbnail::Get()));
}


#undef LOCTEXT_NAMESPACE