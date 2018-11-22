/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerAssetEditor/Nodes/SMultiPackerOutputNode.h"
#include "CoreMinimal.h"
#include "MultiPackerAssetEditor/MultiPackerColors.h"
#include "AssetThumbnail.h"
#include "SCommentBubble.h"
#include "MultiPackerAssetEditor/MultiPackerEditorThumbnail.h"
#include "AssetRegistryModule.h"
#include "SGraphNode.h"
#include "SGraphPin.h"
#include "GraphEditor.h"
#include "GraphEditorActions.h"
#include "MultiPackerAssetEditor/MultiPackerNodePin.h"
#include <SLevelOfDetailBranchNode.h>
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include <GraphEditorSettings.h>

void SMultiPackerOutputNode::BuildThumbnailWidget()
{
	EdActorNode->ProcessThumbnail(EdActorNode->MultiPackerNode->TextureInput);
}
//////////////////////////////////////////////////////////////////////////
void SMultiPackerOutputNode::Construct(const FArguments& InArgs, UMultiPackerOutputNode* InNode)
{
	//ThumbnailSize = FIntPoint(128, 128);
	GraphNode = InNode;
	EdActorNode = InNode;
	BuildThumbnailWidget();
	UpdateGraphNode();
}


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMultiPackerOutputNode::UpdateGraphNode()
{
	const FMargin NodePadding = FMargin(2.0f);


	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<STextBlock> DescriptionText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	TWeakPtr<SNodeTitle> WeakNodeTitle = NodeTitle;
	auto GetNodeTitlePlaceholderWidth = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredWidth = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().X : 0.0f;
		return FMath::Max(75.0f, DesiredWidth);
	};
	auto GetNodeTitlePlaceholderHeight = [WeakNodeTitle]() -> FOptionalSize
	{
		TSharedPtr<SNodeTitle> NodeTitlePin = WeakNodeTitle.Pin();
		const float DesiredHeight = (NodeTitlePin.IsValid()) ? NodeTitlePin->GetTitleSize().Y : 0.0f;
		return FMath::Max(22.0f, DesiredHeight);
	};

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.0f)
			[
				SNew(SOverlay)
				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()///////
					.VAlign(VAlign_Fill)
					//.AutoWidth()
					[
						SNew(SBox)
						.VAlign(VAlign_Fill)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							//.FillHeight(1.0f)
							//.VAlign(VAlign_Center)
							.Padding(2.0F, 50.0f)
							[
								SAssignNew(OutputPinBox, SHorizontalBox)
							]
						]
					]//////////////
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
							.BorderBackgroundColor(this, &SMultiPackerOutputNode::GetBackgroundColor)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Visibility(EVisibility::SelfHitTestInvisible)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SMultiPackerOutputNode::GetErrorColor)
											.ToolTipText(this, &SMultiPackerOutputNode::GetErrorMsgToolTip)
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SLevelOfDetailBranchNode)
											.UseLowDetailSlot(this, &SMultiPackerOutputNode::UseLowDetailNodeTitles)
											.LowDetail()
											[
												SNew(SBox)
												.WidthOverride_Lambda(GetNodeTitlePlaceholderWidth)
												.HeightOverride_Lambda(GetNodeTitlePlaceholderHeight)
											]
											.HighDetail()
											[
												SNew(SHorizontalBox)
												+ SHorizontalBox::Slot()
												.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
												[
													SNew(SVerticalBox)
													+ SVerticalBox::Slot()
													.HAlign(HAlign_Center)
													.AutoHeight()
													[
														SAssignNew(InlineEditableText, SInlineEditableTextBlock)
														.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
														.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
														.OnVerifyTextChanged(this, &SMultiPackerOutputNode::OnVerifyNameTextChanged)
														.OnTextCommitted(this, &SMultiPackerOutputNode::OnNameTextCommited)
														.IsReadOnly(this, &SMultiPackerOutputNode::IsNameReadOnly)
														.IsSelected(this, &SMultiPackerOutputNode::IsSelectedExclusively)
													]
													+ SVerticalBox::Slot()
													.AutoHeight()
													.HAlign(HAlign_Center)
													[
														NodeTitle.ToSharedRef()
													]
													+ SVerticalBox::Slot()
													.AutoHeight()
													.HAlign(HAlign_Center)
													[
														SNew(SBox)
														.WidthOverride(GetThumbnailSizeX())
														.HeightOverride(128)
														[
															EdActorNode->AssetThumbnail->MakeThumbnailWidget()
														]
													]
												]
												
											]
										]
									]
								]
							]
						]
					]
				]
			]
		];

	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
		[
			CommentBubble.ToSharedRef()
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SMultiPackerOutputNode::CreatePinWidgets()
{
	UMultiPackerOutputNode* StateNode = CastChecked<UMultiPackerOutputNode>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SMultiPackerNodePin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SMultiPackerOutputNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateColor SMultiPackerOutputNode::GetBackgroundColor() const
{
	return MultiPackerColors::NodeBody::Default;
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

float SMultiPackerOutputNode::GetThumbnailSizeX()
{
	return EdActorNode->MultiPackerNode->Thumbnailrectangled ? 256 : 128;
}