/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerAssetEditor/Nodes/SMultiPackerTextureEdNode.h"
#include "AssetThumbnail.h"
#include "AssetRegistryModule.h"
#include "MultiPackerAssetEditor/MultiPackerNodePin.h"
#include "MultiPackerEditorTypes.h"
#include "MultiPackerAssetEditor/MultiPackerEditorThumbnail.h"

void SMultiPackerTextureEdNode::BuildThumbnailWidget()
{
	UTexture* C = EdActorNode->GenericGraphNode->TextureInput;
	FAssetData ValueAsset = FAssetData(C);
	EdActorNode->AssetThumbnail = MakeShareable(new FAssetThumbnail(FAssetData(C), ThumbnailSize.X, ThumbnailSize.Y, FMultiPackerEditorThumbnail::Get()));
}
//////////////////////////////////////////////////////////////////////////
void SMultiPackerTextureEdNode::Construct(const FArguments& InArgs, UMultiPackerTextureEdNode* InNode)
{
	ThumbnailSize = FIntPoint(128, 128);
	GraphNode = InNode;
	EdActorNode = InNode;
	BuildThumbnailWidget();
	UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMultiPackerTextureEdNode::UpdateGraphNode()
{
	const FMargin NodePadding = FMargin(2.0f);
	

	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();

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
			.BorderBackgroundColor(this, &SMultiPackerTextureEdNode::GetBorderBackgroundColor)
			[
				SNew(SOverlay)
				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
							.BorderBackgroundColor(this, &SMultiPackerTextureEdNode::GetBackgroundColor)
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
									.HAlign(HAlign_Center)
									.AutoHeight()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SMultiPackerTextureEdNode::GetErrorColor)
											.ToolTipText(this, &SMultiPackerTextureEdNode::GetErrorMsgToolTip)
										]
									+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SLevelOfDetailBranchNode)
											.UseLowDetailSlot(this, &SMultiPackerTextureEdNode::UseLowDetailNodeTitles)
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
														.OnVerifyTextChanged(this, &SMultiPackerTextureEdNode::OnVerifyNameTextChanged)
														.OnTextCommitted(this, &SMultiPackerTextureEdNode::OnNameTextCommited)
														.IsReadOnly(this, &SMultiPackerTextureEdNode::IsNameReadOnly)
														.IsSelected(this, &SMultiPackerTextureEdNode::IsSelectedExclusively)
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
														.WidthOverride(128.0f)
														.HeightOverride(128.0f)
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
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredHeight(5.0F)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							.HAlign(HAlign_Fill)
							.AutoHeight()
							.Padding(2.0F, 7.0f)
							[
								SAssignNew(OutputPinBox, SVerticalBox)
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
	ErrorReporting->SetError(GetErrorMsgToolTip());
	CreatePinWidgets();
}

void SMultiPackerTextureEdNode::CreatePinWidgets()
{
	UMultiPackerTextureEdNode* StateNode = CastChecked<UMultiPackerTextureEdNode>(GraphNode);

	TArray<FLinearColor> PinColors;
	PinColors.Add(FLinearColor(0, 0, 0, 1));
	PinColors.Add(FLinearColor(1, 0, 0, 1));
	PinColors.Add(FLinearColor(0, 1, 0, 1));
	PinColors.Add(FLinearColor(0, 0, 1, 1));
	PinColors.Add(FLinearColor(1, 1, 1, 1));
	PinColors.Add(FLinearColor(1, 1, 1, 1));
	
	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); ++PinIdx)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget(MyPin);
		NewPin->SetColorAndOpacity(PinColors[PinIdx]);
		AddPin(NewPin.ToSharedRef());
	}
}

void SMultiPackerTextureEdNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility( TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced) );
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Output)
	{
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Right)
			.AutoHeight()
			.Padding(0.0F, 3.0f)
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateColor SMultiPackerTextureEdNode::GetErrorColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

FText SMultiPackerTextureEdNode::GetErrorMsgToolTip() const
{
	FString Error = FString(TEXT(""));
	UMultiPackerTextureEdNode* MyNode = CastChecked<UMultiPackerTextureEdNode>(GraphNode);
	if (!MyNode->GenericGraphNode->TextureInput) {
		Error = FString(TEXT("Need Texture"));
	}
	if (MyNode->GenericGraphNode->TilesVertical == 0) {
		Error = Error== FString(TEXT("")) ? FString(TEXT("Set TilesVertical")): Error.Append(FString(TEXT(" & Set TilesVertical")));
	}
	if (MyNode->GenericGraphNode->TilesHorizontal == 0) {
		Error = Error == FString(TEXT("")) ? FString(TEXT("Set TilesHorizontal")) : Error.Append(FString(TEXT(" & Set TilesHorizontal")));
	}
	ErrorText->SetError(Error);
	return FText::FromString(Error);
}

FSlateColor SMultiPackerTextureEdNode::GetBorderBackgroundColor() const
{
	UMultiPackerTextureEdNode* MyNode = CastChecked<UMultiPackerTextureEdNode>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : MultiPackerColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SMultiPackerTextureEdNode::GetBackgroundColor() const
{
	return MultiPackerColors::NodeBody::Default;
}

EVisibility SMultiPackerTextureEdNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

FText SMultiPackerTextureEdNode::GetDescription() const
{
	UMultiPackerTextureEdNode* MyNode = CastChecked<UMultiPackerTextureEdNode>(GraphNode);
	return MyNode ? MyNode->GetDescription() : FText::GetEmpty();
}

EVisibility SMultiPackerTextureEdNode::GetDescriptionVisibility() const
{
	//return (GetCurrentLOD() > EGraphRenderingLOD::LowDetail) ? EVisibility::Visible : EVisibility::Collapsed;
	return EVisibility::Hidden;
}

const FSlateBrush* SMultiPackerTextureEdNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

