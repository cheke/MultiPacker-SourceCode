/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "MultiPackerAssetEditor/MultiPackerColors.h"
#include "SGraphPin.h"

class SMultiPackerNodePin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SMultiPackerNodePin) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);
		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
			.BorderImage(this, &SMultiPackerNodePin::GetPinBorder)
			.BorderBackgroundColor(this, &SMultiPackerNodePin::GetPinColor)
			.OnMouseButtonDown(this, &SMultiPackerNodePin::OnPinMouseDown)
			.Cursor(this, &SMultiPackerNodePin::GetPinCursor)
			.Padding(FMargin(10.0f))
		);
	}
protected:
	virtual FSlateColor GetPinColor() const override
	{
		return MultiPackerColors::Pin::Default;
	}

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}
private:

};


