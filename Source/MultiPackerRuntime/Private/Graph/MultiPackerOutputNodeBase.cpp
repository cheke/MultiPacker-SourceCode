/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Graph/MultiPackerOutputNodeBase.h"
//#include "Engine/TextureRenderTarget2D.h"

#define LOCTEXT_NAMESPACE "MultiPackerOutputNode"

UMultiPacker* UMultiPackerOutputNodeBase::GetGraph() const
{
	return Cast<UMultiPacker>(GetOuter());
}

#undef LOCTEXT_NAMESPACE