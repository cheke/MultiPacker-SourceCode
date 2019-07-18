/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "Widgets/SPanel.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UMaterialInterface;
class UTexture2D;
struct FSlateImageBrush;

class SMPTexturePreviewZoomer : public SPanel
{
public:

	class FRMPreviewPanelSlot : public TSupportsOneChildMixin<FRMPreviewPanelSlot>
	{
	public:
#if (ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION <= 19)
		FRMPreviewPanelSlot()
			: TSupportsOneChildMixin<FRMPreviewPanelSlot>()
		{
		}
	};
#endif
#if (ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION > 19)
	FRMPreviewPanelSlot(SWidget* InOwner)
			: TSupportsOneChildMixin<FRMPreviewPanelSlot>(InOwner)
		{
		}
	};
#endif
	SLATE_BEGIN_ARGS(SMPTexturePreviewZoomer) {}
	SLATE_END_ARGS()

	SMPTexturePreviewZoomer()
#if (ENGINE_MAJOR_VERSION == 4) && (ENGINE_MINOR_VERSION > 19)
	: ChildSlot(this)
#endif
	{
	}

	void Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture);

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual FChildren* GetChildren() override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	bool ZoomBy(const float Amount);
	float GetZoomLevel() const;

	void SetPreviewSize(const FVector2D PreviewSize);
	void SetPreviewTexture(UTexture2D* InPreviewTexture);
private:
	//FLinearColor Tint= FLinearColor(1,1,1,1);
	mutable FVector2D CachedSize;
	float ZoomLevel;

	FRMPreviewPanelSlot ChildSlot;
	TSharedPtr<FSlateImageBrush> PreviewBrush;
	TSharedPtr<SImage> ImageWidget;
};