/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Widgets/SMPTexturePreviewZoomer.h"
#include "Widgets/SMPTextureWidget.h"
#include "Widgets/Images/SImage.h"
#include <Brushes/SlateImageBrush.h>
#include <Engine/Texture2D.h>
#include <SlotBase.h>
#include <Layout/ArrangedChildren.h>

#define LOCTEXT_NAMESPACE "SMPTexturePreviewZoomer"

void SMPTexturePreviewZoomer::Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture)
{ 
	PreviewBrush = MakeShareable(new FSlateImageBrush(InPreviewTexture, FVector2D(250, 250)));
	ChildSlot
		[
			SAssignNew(ImageWidget, SImage)
			.Image(PreviewBrush.Get())
		];
}

void SMPTexturePreviewZoomer::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	CachedSize = AllottedGeometry.GetLocalSize();
	const TSharedRef<SWidget>& ChildWidget = ChildSlot.GetWidget();
	if (ChildWidget->GetVisibility() != EVisibility::Collapsed)
	{
		const FVector2D& WidgetDesiredSize =  FVector2D(ChildWidget->GetDesiredSize().Y, ChildWidget->GetDesiredSize().Y);
		ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ChildWidget, FVector2D::ZeroVector, WidgetDesiredSize ));
	}
}

FVector2D SMPTexturePreviewZoomer::ComputeDesiredSize(float) const
{
	FVector2D ThisDesiredSize = FVector2D::ZeroVector;
	const TSharedRef<SWidget>& ChildWidget = ChildSlot.GetWidget();
	if (ChildWidget->GetVisibility() != EVisibility::Collapsed)
	{
		const FVector2D  Desired = ChildWidget->GetDesiredSize();
		const float SmallestSize = Desired.X >= Desired.Y ? Desired.Y : Desired.X;
		ThisDesiredSize = FVector2D(SmallestSize, SmallestSize);
	}
	return ThisDesiredSize;
}

FChildren* SMPTexturePreviewZoomer::GetChildren()
{
	return &ChildSlot;
}

int32 SMPTexturePreviewZoomer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	PreviewBrush->ImageSize = CachedSize;
	LayerId = SPanel::OnPaint(Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	return LayerId;
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
