/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Widgets/SMPTexturePreviewZoomer.h"
#include "Widgets/SMPTextureWidget.h"
#include "Widgets/Images/SImage.h"
#include "Templates/SharedPointer.h"
#include "EditorStyleSet.h"
#include <Widgets/Input/SNumericEntryBox.h>
#include <Brushes/SlateImageBrush.h>

#define LOCTEXT_NAMESPACE "SMPTexturePreviewZoomer"

void SMPTexturePreviewZoomer::Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture)
{ 
	//Tint = FLinearColor(1, 1, 1, 1);
	PreviewBrush = MakeShareable(new FSlateImageBrush(InPreviewTexture, FVector2D(250, 250)));

	ChildSlot
		[
			SAssignNew(ImageWidget, SImage)
			.Image(PreviewBrush.Get())
		];

	ZoomLevel = 1.0f;
}

void SMPTexturePreviewZoomer::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	CachedSize = AllottedGeometry.GetLocalSize();

	const TSharedRef<SWidget>& ChildWidget = ChildSlot.GetWidget();
	if (ChildWidget->GetVisibility() != EVisibility::Collapsed)
	{
		const FVector2D& WidgetDesiredSize = ChildWidget->GetDesiredSize();

		ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ChildWidget, FVector2D::ZeroVector, WidgetDesiredSize * ZoomLevel));
	}
}

FVector2D SMPTexturePreviewZoomer::ComputeDesiredSize(float) const
{
	FVector2D ThisDesiredSize = FVector2D::ZeroVector;

	const TSharedRef<SWidget>& ChildWidget = ChildSlot.GetWidget();
	if (ChildWidget->GetVisibility() != EVisibility::Collapsed)
	{
		ThisDesiredSize = ChildWidget->GetDesiredSize() * ZoomLevel;
	}

	return ThisDesiredSize;
}

FChildren* SMPTexturePreviewZoomer::GetChildren()
{
	return &ChildSlot;
}

FReply SMPTexturePreviewZoomer::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ZoomBy(MouseEvent.GetWheelDelta());

	return FReply::Handled();
}

int32 SMPTexturePreviewZoomer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = SPanel::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	return LayerId;
}

bool SMPTexturePreviewZoomer::ZoomBy(const float Amount)
{
	static const float MinZoomLevel = 0.2f;
	static const float MaxZoomLevel = 4.0f;

	const float PrevZoomLevel = ZoomLevel;
	ZoomLevel = FMath::Clamp(ZoomLevel + (Amount * 0.05f), MinZoomLevel, MaxZoomLevel);
	return ZoomLevel != PrevZoomLevel;
}

float SMPTexturePreviewZoomer::GetZoomLevel() const
{
	return ZoomLevel;
}


void SMPTexturePreviewZoomer::SetPreviewSize(const FVector2D PreviewSize)
{
	PreviewBrush->ImageSize = PreviewSize;
}

void SMPTexturePreviewZoomer::SetPreviewTexture(UTexture2D* InPreviewTexture)
{
	PreviewBrush = MakeShareable(new FSlateImageBrush(InPreviewTexture, PreviewBrush->ImageSize));
	ImageWidget->SetImage(PreviewBrush.Get());
}

#undef LOCTEXT_NAMESPACE