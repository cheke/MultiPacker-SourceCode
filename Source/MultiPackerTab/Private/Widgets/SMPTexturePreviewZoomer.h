/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "Widgets/SPanel.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UMaterialInterface;
class UTexture2D;
struct FSlateImageBrush;
class SImage;

class SMPTexturePreviewZoomer : public SPanel
{
public:

	class FRMPreviewPanelSlot : public TSupportsOneChildMixin<FRMPreviewPanelSlot>
	{
	public:
		FRMPreviewPanelSlot(SWidget* InOwner) : TSupportsOneChildMixin<FRMPreviewPanelSlot>(InOwner) {}
	};
	SLATE_BEGIN_ARGS(SMPTexturePreviewZoomer) {}
	SLATE_END_ARGS()

	SMPTexturePreviewZoomer() : ChildSlot(this) {};

	void Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture);

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual FVector2D ComputeDesiredSize(float = 0.0) const override;
	virtual FChildren* GetChildren() override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void SetPreviewSize(const FVector2D PreviewSize);
	void SetPreviewTexture(UTexture2D* InPreviewTexture);
private:
	mutable FVector2D CachedSize;
	FRMPreviewPanelSlot ChildSlot;
	TSharedPtr<FSlateImageBrush> PreviewBrush;
	TSharedPtr<SImage> ImageWidget;
};