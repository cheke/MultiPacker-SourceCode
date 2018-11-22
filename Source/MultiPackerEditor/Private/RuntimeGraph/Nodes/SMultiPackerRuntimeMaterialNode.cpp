/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "RuntimeGraph/Nodes/SMultiPackerRuntimeMaterialNode.h"
#include "AssetThumbnail.h"
#include "AssetRegistryModule.h"
#include "MultiPackerAssetEditor/MultiPackerNodePin.h"
#include "MultiPackerAssetEditor/MultiPackerEditorThumbnail.h"
#include <GraphEditorSettings.h>

void SMultiPackerRuntimeMaterialNode::BuildThumbnailWidget()
{
	EdActorNode->AssetThumbnail = MakeShareable(new FAssetThumbnail(FAssetData(EdActorNode->MultiPackerRuntimeMatNode->MaterialBaseInput), ThumbnailSize.X, ThumbnailSize.Y, FMultiPackerEditorThumbnail::Get()));
}
//////////////////////////////////////////////////////////////////////////
void SMultiPackerRuntimeMaterialNode::Construct(const FArguments& InArgs, UMultiPackerRuntimeMaterialNode* InNode)
{
	ThumbnailSize = FIntPoint(128, 128);
	GraphNode = InNode;
	EdActorNode = InNode;
	BuildThumbnailWidget();
	UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMultiPackerRuntimeMaterialNode::UpdateGraphNode()
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
		.BorderBackgroundColor(this, &SMultiPackerRuntimeMaterialNode::GetBorderBackgroundColor)
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
		.BorderBackgroundColor(this, &SMultiPackerRuntimeMaterialNode::GetBackgroundColor)
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
			.BackgroundColor(this, &SMultiPackerRuntimeMaterialNode::GetErrorColor)
		.ToolTipText(this, &SMultiPackerRuntimeMaterialNode::GetErrorMsgToolTip)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SLevelOfDetailBranchNode)
			.UseLowDetailSlot(this, &SMultiPackerRuntimeMaterialNode::UseLowDetailNodeTitles)
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
		.OnVerifyTextChanged(this, &SMultiPackerRuntimeMaterialNode::OnVerifyNameTextChanged)
		.OnTextCommitted(this, &SMultiPackerRuntimeMaterialNode::OnNameTextCommited)
		.IsReadOnly(this, &SMultiPackerRuntimeMaterialNode::IsNameReadOnly)
		.IsSelected(this, &SMultiPackerRuntimeMaterialNode::IsSelectedExclusively)
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
		.VAlign(VAlign_Fill)
		.Padding(2.0F, 50.0f)
		.FillHeight(1.0f)
		[
			SAssignNew(OutputPinBox, SHorizontalBox)
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

void SMultiPackerRuntimeMaterialNode::CreatePinWidgets()
{
	UMultiPackerRuntimeMaterialNode* StateNode = CastChecked<UMultiPackerRuntimeMaterialNode>(GraphNode);

	for (int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if (!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = CreatePinWidget(MyPin); //SNew(SMultiPackerNodePin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SMultiPackerRuntimeMaterialNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Output)
	{
		OutputPinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.0f)
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateColor SMultiPackerRuntimeMaterialNode::GetErrorColor() const
{
	return FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

FText SMultiPackerRuntimeMaterialNode::GetErrorMsgToolTip() const
{
	FString Error = FString(TEXT(""));
	UMultiPackerRuntimeMaterialNode* MyNode = CastChecked<UMultiPackerRuntimeMaterialNode>(GraphNode);
	if (!MyNode->MultiPackerRuntimeMatNode->MaterialBaseInput) {
		Error = FString(TEXT("Need Material"));
	}
	ErrorText->SetError(Error);
	return FText::FromString(Error);
}

FSlateColor SMultiPackerRuntimeMaterialNode::GetBorderBackgroundColor() const
{
	UMultiPackerRuntimeMaterialNode* MyNode = CastChecked<UMultiPackerRuntimeMaterialNode>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : MultiPackerColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SMultiPackerRuntimeMaterialNode::GetBackgroundColor() const
{
	return MultiPackerColors::NodeBody::Default;
}

EVisibility SMultiPackerRuntimeMaterialNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

FText SMultiPackerRuntimeMaterialNode::GetDescription() const
{
	UMultiPackerRuntimeMaterialNode* MyNode = CastChecked<UMultiPackerRuntimeMaterialNode>(GraphNode);
	return MyNode ? MyNode->GetDescription() : FText::GetEmpty();
}

EVisibility SMultiPackerRuntimeMaterialNode::GetDescriptionVisibility() const
{
	return EVisibility::Hidden;
}

const FSlateBrush* SMultiPackerRuntimeMaterialNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}
