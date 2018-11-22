/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Graph/MultiPackerOutputNodeBase.h"
#include "Engine/TextureRenderTarget2D.h"

#define LOCTEXT_NAMESPACE "MultiPackerOutputNode"

FString UMultiPackerOutputNodeBase::GetNodeTitle() const
{
		return "Texture Output";
}

void UMultiPackerOutputNodeBase::SetThumnailOutput(UTexture2D* Input, bool rectangle)
{
	TextureInput = Input;
	Thumbnailrectangled = rectangle;
}

UMultiPacker* UMultiPackerOutputNodeBase::GetGraph() const
{
	return Cast<UMultiPacker>(GetOuter());
}

#undef LOCTEXT_NAMESPACE