/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "MultiPackerRuntimeOutputNodeBase.h"
#include "MultiPackerRuntimeMatNode.h"
#include "Engine/TextureRenderTarget2D.h"

#define LOCTEXT_NAMESPACE "MultiPackerRuntimeOutputNode"

FString UMultiPackerRuntimeOutputNodeBase::GetNodeTitle() const
{
		return "Texture Output";
}

bool UMultiPackerRuntimeOutputNodeBase::NodeTileSearchFail()
{
	bool error = false;
	for (UMultiPackerRuntimeMatNode* M_Node : ChildrenMatNodes)
	{
		
	}
	return error;
}

void UMultiPackerRuntimeOutputNodeBase::SetThumnailOutput(UTexture2D* Input, bool rectangle)
{
	TextureInput = Input;
	Thumbnailrectangled = rectangle;
}

UMultiPackerRuntimeGraph* UMultiPackerRuntimeOutputNodeBase::GetGraph() const
{
	return Cast<UMultiPackerRuntimeGraph>(GetOuter());
}

#undef LOCTEXT_NAMESPACE