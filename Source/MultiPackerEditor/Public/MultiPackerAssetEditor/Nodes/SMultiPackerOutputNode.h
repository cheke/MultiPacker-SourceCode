/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "MultiPackerAssetEditor/Nodes/MultiPackerOutputNode.h"

class SMultiPackerOutputNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SMultiPackerOutputNode) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UMultiPackerOutputNode* InNode);


	//~ Begin SGraphNode Interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	// 	virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	// 	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	// 	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	// 	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;
	// 	virtual FReply OnMouseMove(const FGeometry& SenderGeometry, const FPointerEvent& MouseEvent) override;
	// 	virtual void SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel) override;
	// 	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	//~ End SGraphNode Interface

	virtual FSlateColor GetBackgroundColor() const;

	virtual EVisibility GetDragOverMarkerVisibility() const;

	virtual FText GetDescription() const;
	virtual EVisibility GetDescriptionVisibility() const;

	virtual float GetThumbnailSizeX();

	virtual const FSlateBrush* GetNameIcon() const;

protected:
	TSharedPtr<SBorder> NodeBody;
	TSharedPtr<SHorizontalBox> OutputPinBox;

	UMultiPackerOutputNode* EdActorNode;

private:
	void BuildThumbnailWidget();
};
