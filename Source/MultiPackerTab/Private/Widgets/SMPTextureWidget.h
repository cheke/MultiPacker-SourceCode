/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UTexture2D;
class SMPTexturePreviewZoomer;
/**
* A preview viewport used for 2D Textures
*/
class SMPTextureWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMPTextureWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UTexture2D* InPreviewTexture);
	void SetPreviewTexture(UTexture2D* InPreviewTexture);
private:
	FIntPoint PreviewSize;
	TSharedPtr<class SMPTexturePreviewZoomer> PreviewZoomer;
};