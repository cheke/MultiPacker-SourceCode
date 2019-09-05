/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Widgets/SMPTextureWidget.h"
#include "Widgets/SMPTexturePreviewZoomer.h"

#define LOCTEXT_NAMESPACE "SMPTextureWidget"

void SMPTextureWidget::Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture)
{
	ChildSlot
		[
			SAssignNew(PreviewZoomer, SMPTexturePreviewZoomer, InPreviewTexture)
		];
	PreviewZoomer->SetPreviewSize(PreviewZoomer->ComputeDesiredSize());
}

void SMPTextureWidget::SetPreviewTexture(UTexture2D* InPreviewTexture)
{
	PreviewZoomer->SetPreviewTexture(InPreviewTexture);
}

#undef LOCTEXT_NAMESPACE
