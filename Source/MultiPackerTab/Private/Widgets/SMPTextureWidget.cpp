/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Widgets/SMPTextureWidget.h"
#include "Widgets/SMPTexturePreviewZoomer.h"
#include <Brushes/SlateImageBrush.h>
#include "Widgets/Images/SImage.h"
#include "Templates/SharedPointer.h"
#include "EditorStyleSet.h"
#include <Widgets/Input/SNumericEntryBox.h>

#define LOCTEXT_NAMESPACE "SMPTextureWidget"

void SMPTextureWidget::Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture)
{

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBorder)
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(3.f)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PreviewSize", "Preview Size"))
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(3.f)
					.MaxWidth(75)
					[
						SNew(SNumericEntryBox<int32>)
						.AllowSpin(true)
						.MinValue(1)
						.MaxSliderValue(4096)
						.OnValueChanged(this, &SMPTextureWidget::OnPreviewXChanged)
						.OnValueCommitted(this, &SMPTextureWidget::OnPreviewXCommitted)
						.Value(this, &SMPTextureWidget::OnGetPreviewXValue)
						.MinDesiredValueWidth(75)
						.Label()
						[
							SNew(SBox)
							.VAlign(VAlign_Center)
							[
							SNew(STextBlock)
							.Text(LOCTEXT("PreviewSize_X", "X"))
							]
						]
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(3.f)
					.MaxWidth(75)
					[
						SNew(SNumericEntryBox<int32>)
						.AllowSpin(true)
						.MinValue(1)
						.MaxSliderValue(4096)
						.MinDesiredValueWidth(75)
						.OnValueChanged(this, &SMPTextureWidget::OnPreviewYChanged)
						.OnValueCommitted(this, &SMPTextureWidget::OnPreviewYCommitted)
						.Value(this, &SMPTextureWidget::OnGetPreviewYValue)
						.Label()
						[
							SNew(SBox)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(LOCTEXT("PreviewSize_Y", "Y"))
							]
						]
					]
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SBorder)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.BorderImage(FEditorStyle::GetBrush("BlackBrush"))
				[
					SAssignNew(PreviewZoomer, SMPTexturePreviewZoomer, InPreviewTexture)
				]
			]
		];

	PreviewSize = FIntPoint(250, 250);
	PreviewZoomer->SetPreviewSize(FVector2D(PreviewSize));
}

void SMPTextureWidget::SetPreviewTexture(UTexture2D* InPreviewTexture)
{
	PreviewZoomer->SetPreviewTexture(InPreviewTexture);
}

void SMPTextureWidget::OnPreviewXChanged(int32 NewValue)
{
	PreviewSize.X = NewValue;
	PreviewZoomer->SetPreviewSize(FVector2D(PreviewSize));
}

void SMPTextureWidget::OnPreviewXCommitted(int32 NewValue, ETextCommit::Type)
{
	OnPreviewXChanged(NewValue);
}

void SMPTextureWidget::OnPreviewYChanged(int32 NewValue)
{
	PreviewSize.Y = NewValue;
	PreviewZoomer->SetPreviewSize(FVector2D(PreviewSize));
}

void SMPTextureWidget::OnPreviewYCommitted(int32 NewValue, ETextCommit::Type)
{
	OnPreviewYChanged(NewValue);
}

#undef LOCTEXT_NAMESPACE