/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerAssetEditor/Nodes/SMultiPackerOutputNode.h"
#include "CoreMinimal.h"
#include "AssetThumbnail.h"
#include "MultiPackerAssetEditor/MultiPackerEditorThumbnail.h"
#include "SGraphPin.h"
#include "GraphEditor.h"
#include <GraphEditorSettings.h>

void SMultiPackerOutputNode::BuildThumbnailWidget()
{
	EdActorNode->ProcessThumbnail(EdActorNode->MultiPackerNode->TextureInput);
}

//////////////////////////////////////////////////////////////////////////
void SMultiPackerOutputNode::Construct(const FArguments& InArgs, UMultiPackerOutputNode* InNode)
{
	GraphNode = InNode;
	EdActorNode = InNode;
	BuildThumbnailWidget();
	UpdateGraphNode();
}

void SMultiPackerOutputNode::CreatePinWidgets()
{
	for (int32 PinIdx = 0; PinIdx < GraphNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = GraphNode->Pins[PinIdx];
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget(MyPin); 
		AddPin(NewPin.ToSharedRef());
	}
}

void SMultiPackerOutputNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
		
	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		FMargin Padding = Settings->GetInputPinPadding();
		Padding.Left *= 0.5f;
		Padding.Right = 0.0f;

		LeftNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(Padding)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
}

EVisibility SMultiPackerOutputNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

FText SMultiPackerOutputNode::GetDescription() const
{
	UMultiPackerOutputNode* MyNode = CastChecked<UMultiPackerOutputNode>(GraphNode);
	return MyNode ? MyNode->GetDescription() : FText::GetEmpty();
}

EVisibility SMultiPackerOutputNode::GetDescriptionVisibility() const
{
	return EVisibility::Hidden;
}

const FSlateBrush* SMultiPackerOutputNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

TSharedRef<SWidget> SMultiPackerOutputNode::CreateNodeContentArea()
{
	// NODE CONTENT AREA
	return SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("NoBorder"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.FillWidth(1.0f)
			[
				// LEFT
				SAssignNew(LeftNodeBox, SVerticalBox)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				// RIGHT
				SAssignNew(RightNodeBox, SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(GetThumbnailSizeX())
					.HeightOverride(128)
					[
						EdActorNode->AssetThumbnail->MakeThumbnailWidget()
					]
				]
			]
		];
	
}

float SMultiPackerOutputNode::GetThumbnailSizeX()
{
	return EdActorNode->MultiPackerNode->Thumbnailrectangled ? 256 : 128;
}