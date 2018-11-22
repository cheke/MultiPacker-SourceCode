/* Copyright 2018 TurboCheke, Estudio Cheke  - All Rights Reserved */

#include "Graph/MultiPacker.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Graph/MultiPackerTextureNode.h"
#include "Graph/MultiPackerOutputNodeBase.h"
#include "Graph/MultiPackerMatNode.h"
#include "MultiPackerSettings.h"
#include <UObject/UObjectGlobals.h>
#define LOCTEXT_NAMESPACE "MultiPacker"

UMultiPacker::UMultiPacker()
{
	NodeType = UMultiPackerTextureNode::StaticClass();
	MPNodeType = UMultiPackerOutputNodeBase::StaticClass();
	MatNodeType = UMultiPackerMatNode::StaticClass();
#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
#endif
	const UMultiPackerSettings* Settings = GetDefault<UMultiPackerSettings>();
	ChannelMethod = Settings->GetChannelMethod();
	SaveMaterialCollection = Settings->GetSaveMaterialCollection();
	SaveDatabase = Settings->GetSaveDatabase();
	TargetDirectory = Settings->GetTargetDirectory();
	DatabasePrefix = Settings->GetDatabasePrefix();
	TexturePrefix = Settings->GetTexturePrefix();
	MaterialcollectionPrefix = Settings->GetMaterialcollectionPrefix();
}

UMultiPacker::~UMultiPacker()
{
}

void UMultiPacker::ClearGraph()
{
	for (int i = 0; i < TexNodes.Num(); ++i)
	{
		UMultiPackerTextureNode* Node = TexNodes[i];

		Node->ParentNode= nullptr;
	}
	for (int i = 0; i < MatNodes.Num(); ++i)
	{
		UMultiPackerMatNode* Node = MatNodes[i];

		Node->ParentNode = nullptr;
	}
	TexNodes.Reset();
	MatNodes.Reset();
}

 #if WITH_EDITOR
 void UMultiPacker::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
 {
 	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
 	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UMultiPacker, RectangleTiles)))
 	{
		NodeOutput->RectangleTiles = RectangleTiles;
	}
	uint32 SizeOut = (uint32)0;
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UMultiPacker, TextureTileSizeOutput)))
	{
		SizeOut = (uint32)TextureTileSizeOutput * 32;
	}
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UMultiPacker, RectangleTiles)))
	{
		for (UMultiPackerTextureNode* Node : TexNodes)
		{
			Node->RectangleSize = RectangleTiles;
			Node->CompareSizes(UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeY), UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeX));
		}
		for (UMultiPackerMatNode* Node : MatNodes)
		{
			Node->RectangleSize = RectangleTiles;
			Node->CompareSizes(UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeY), UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeX));
		}
	}
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UMultiPacker, OutputSizeX)))
	{
		for (UMultiPackerTextureNode* Node : TexNodes)
		{
			Node->CompareSizes(UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeY), UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeX));
		}
		for (UMultiPackerMatNode* Node : MatNodes)
		{
			Node->CompareSizes(UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeY), UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeX));
		}
	}
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UMultiPacker, OutputSizeY)))
	{
		for (UMultiPackerTextureNode* Node : TexNodes)
		{
			Node->CompareSizes(UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeY), UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeX));
		}
		for (UMultiPackerMatNode* Node : MatNodes)
		{
			Node->CompareSizes(UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeY), UMultiPackerBaseEnums::GetTextureSizeOutputEnum(OutputSizeX));
		}
	}
 	Super::PostEditChangeProperty(PropertyChangedEvent);
 }
 #endif

#undef LOCTEXT_NAMESPACE
