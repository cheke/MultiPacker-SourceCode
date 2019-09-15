/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerChannelTab.h"
#include <PropertyEditorModule.h>
#include <Widgets/Input/SButton.h>
#include <IDetailsView.h>
#include "MultiPackerDetailPanel.h"
#include <Framework/Docking/TabManager.h>
#include <AssetToolsModule.h>
#include <AssetRegistryModule.h>
#include <ImageUtils.h>
#include <Engine/TextureRenderTarget2D.h>
#include <Editor.h>
#include "MultiPackerSettings.h"
#include "Widgets/SMPTextureWidget.h"
#include <EditorFontGlyphs.h>
#include "TileUtils/TilePointer.h"
#include "IAssetTools.h"
#include <AssetToolsModule.h>
#include <Widgets/Docking/SDockTab.h>
#include <Widgets/Layout/SBox.h>
#include <Widgets/Text/STextBlock.h>

#define LOCTEXT_NAMESPACE "UMultiPackerChannelTab"

void UMultiPackerChannelTab::InitializeTab()
{
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.bShowScrollBar = true;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->HideFilterArea(true);

	const UMultiPackerSettings* Settings = GetDefault<UMultiPackerSettings>();
	if (PropertyMPChannel == nullptr)
	{
		PropertyMPChannel = NewObject<UMultiPackerDetailPanel>(UMultiPackerDetailPanel::StaticClass());
		PropertyMPChannel->TargetDirectory = Settings->GetTargetDirectory();
		PropertyMPChannel->TextureName = Settings->GetTextureChannelName();
		PropertyMPChannel->AddToRoot();
		PropertyWidget->SetObject(PropertyMPChannel);
	}
	PreviewTextureViewport = SNew(SMPTextureWidget, nullptr);
}

TSharedRef<class SDockTab> UMultiPackerChannelTab::OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs)
{
	//FText ProcessText = FText::FromString("Process");
	FText ProcessTooltip = FText::FromString("Make a new Channel Texture");
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Fill)
				.Padding(3.0f)
				.FillWidth(75.0f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(3.0f)
					.AutoHeight()
					[
						SNew(STextBlock)
						.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
						.Text(LOCTEXT("Output", "Output Texture"))
					]
					+ SVerticalBox::Slot()
					.Padding(3.0f)
					[
						SNew(SBorder)
						.HAlign(HAlign_Fill)//HAlign_Fill HAlign_Center
						.VAlign(VAlign_Fill)
						.BorderImage(FEditorStyle::GetBrush("BlackBrush"))
						[
							//TODO center the preview, for some reason its always gets an left padding exaggerated
							PreviewTextureViewport.ToSharedRef()
						]
					]
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Fill)
					.Padding(3.0f)
					.AutoWidth()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.Padding(3.0f)
						.AutoHeight()
						[
							PropertyWidget.ToSharedRef()
						]
						+ SVerticalBox::Slot()
						.Padding(15.f)
						.MaxHeight(50.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								CreateAndSetButton(FOnClicked::CreateUObject(this, &UMultiPackerChannelTab::PreviewRedTexture),
								FEditorFontGlyphs::File_Image_O,
								LOCTEXT("PreviewRT", "Preview Red Texture"),
								LOCTEXT("PreviewR", "Preview Red"))
							]
							+ SHorizontalBox::Slot()
							[
								CreateAndSetButton(FOnClicked::CreateUObject(this, &UMultiPackerChannelTab::PreviewGreenTexture),
								FEditorFontGlyphs::File_Image_O,
								LOCTEXT("PreviewGT", "Preview Green Texture"),
								LOCTEXT("PreviewG", "Preview Green"))
							]
							+ SHorizontalBox::Slot()
							[
								CreateAndSetButton(FOnClicked::CreateUObject(this, &UMultiPackerChannelTab::PreviewBlueTexture),
								FEditorFontGlyphs::File_Image_O,
								LOCTEXT("PreviewBT", "Preview Blue Texture"),
								LOCTEXT("PreviewB", "Preview Blue"))
							]
							+ SHorizontalBox::Slot()
							[
								CreateAndSetButton(FOnClicked::CreateUObject(this, &UMultiPackerChannelTab::PreviewAlphaTexture),
								FEditorFontGlyphs::File_Image_O,
								LOCTEXT("PreviewAT", "Preview Alpha Texture"),
								LOCTEXT("PreviewA", "Preview Alpha"))
							]
						]
						+ SVerticalBox::Slot()
						.Padding(15.f)
						.MaxHeight(50.0f)
						[
							CreateAndSetButton(FOnClicked::CreateUObject(this, &UMultiPackerChannelTab::OnClickButton),
							FEditorFontGlyphs::Floppy_O,
							LOCTEXT("ProcessTooltip", "Process TextureMake a new Channel Texture"),
							LOCTEXT("ProcessCS", "Process Texture"))
						]
					]
				]
			];
}

FReply UMultiPackerChannelTab::OnClickButton()
{
	if (PropertyMPChannel->TextureRed != nullptr && PropertyMPChannel->TextureGreen != nullptr && PropertyMPChannel->TextureBlue != nullptr && (PropertyMPChannel->AlphaChannel ? PropertyMPChannel->TextureAlpha != nullptr : true) )
	{
		const int32 SizeVertical = UMultiPackerBaseEnums::GetTextureSizeOutputEnum(PropertyMPChannel->SizeVertical);
		const int32 SizeHorizontal = UMultiPackerBaseEnums::GetTextureSizeOutputEnum(PropertyMPChannel->SizeHorizontal);
		UTilePointer* TileRed = ProcessTextureChannel(PropertyMPChannel->TextureRed, SizeVertical, SizeHorizontal, PropertyMPChannel->MaskTextureRed);
		UTilePointer* TileGreen = ProcessTextureChannel(PropertyMPChannel->TextureGreen, SizeVertical, SizeHorizontal, PropertyMPChannel->MaskTextureGreen);
		UTilePointer* TileBlue = ProcessTextureChannel(PropertyMPChannel->TextureBlue, SizeVertical, SizeHorizontal, PropertyMPChannel->MaskTextureBlue);
		UTilePointer* TileAlpha = ProcessTextureChannel(PropertyMPChannel->TextureAlpha, SizeVertical, SizeHorizontal, PropertyMPChannel->MaskTextureAlpha);
		
		//Texture Output
		TArray<UTilePointer*> OutArray = UTilePointer::DoFinalTextures({ TileRed, TileGreen, TileBlue, TileAlpha }, 4, PropertyMPChannel->AlphaChannel);
		
		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		TArray<FString> NewPackageName = TexturePackageName(AssetToolsModule);
		UPackage* package = CreatePackage(NULL, *NewPackageName[1]);
		package->FullyLoad();
		package->Modify();
		UTexture2D* NewTexture = OutArray[0]->FillTextureOutput(package, *NewPackageName[0], RF_Public | RF_Standalone);
		NewTexture->PostEditChange();
		if (NewTexture)
		{
			// package needs saving
			NewTexture->MarkPackageDirty();
			NewTexture->Filter = PropertyMPChannel->TextureFilter;
			NewTexture->SRGB = PropertyMPChannel->sRGB;
			NewTexture->AddressX = PropertyMPChannel->AddressX;
			NewTexture->AddressY = PropertyMPChannel->AddressY;
			NewTexture->PowerOfTwoMode = PropertyMPChannel->PowerOfTwoMode;
			NewTexture->CompressionNoAlpha = !PropertyMPChannel->AlphaChannel;
			NewTexture->CompressionSettings = PropertyMPChannel->CompressionSettings;
			NewTexture->PostEditChange();
			FAssetRegistryModule::AssetCreated(NewTexture);
		}
		PropertyMPChannel->Texture = NewTexture;
		PreviewTextureViewport->SetPreviewTexture(NewTexture);	
	}
	return FReply::Handled();
}

EChannelSelectionInput UMultiPackerChannelTab::GetChannelEnum(EMPChannelMaskParameterColor InEnum)
{
	switch (InEnum)
	{
	case EMPChannelMaskParameterColor::Red:
		return EChannelSelectionInput::CSI_Red;
		break;
	case EMPChannelMaskParameterColor::Green:
		return EChannelSelectionInput::CSI_Green;
		break;
	case EMPChannelMaskParameterColor::Blue:
		return EChannelSelectionInput::CSI_Blue;
		break;
	case EMPChannelMaskParameterColor::Alpha:
		return EChannelSelectionInput::CSI_Alpha;
		break;
	default:
		return EChannelSelectionInput::CSI_Red;
		break;
	}
	return EChannelSelectionInput::CSI_Red;
}

UTilePointer* UMultiPackerChannelTab::ProcessTextureChannel(UTexture2D* InTexture, const int32 InSizeVertical, const int32 InSizeHorizontal, EMPChannelMaskParameterColor InChannel)
{
	EChannelSelectionInput InEnum = GetChannelEnum(InChannel);
	//set the data from the Node UTexture
	UTexture2D* Texture = Cast<UTexture2D>(InTexture);
	UTilePointer* InTile = NewObject<UTilePointer>(UTilePointer::StaticClass());
	UTilePointer* Resized = NewObject<UTilePointer>(UTilePointer::StaticClass());
	if (Texture == nullptr)
	{
		InTile->GenerateTextureCanvas(InSizeHorizontal, InSizeVertical);
	}
	else
	{
		InTile->GenerateFromTexture(Texture, Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
		InTile->FromChannelToTexture(InEnum);
		Resized->ChangeResolution(InSizeHorizontal, InSizeVertical, InTile);
	}
	return Resized;
}

TArray<FString> UMultiPackerChannelTab::TexturePackageName(FAssetToolsModule& AssetToolsModule)
{
	FString importDirectory = PropertyMPChannel->TargetDirectory.Path;
	FString ObjectName = PropertyMPChannel->TextureName;
	// last minute sanitizing, just in case we missed one
	ObjectName = ObjectName.Replace(TEXT("*"), TEXT("X"));
	ObjectName = ObjectName.Replace(TEXT("?"), TEXT("Q"));
	ObjectName = ObjectName.Replace(TEXT("!"), TEXT("I"));
	ObjectName = ObjectName.Replace(TEXT("."), TEXT("-"));
	ObjectName = ObjectName.Replace(TEXT("&"), TEXT("_"));
	ObjectName = ObjectName.Replace(TEXT(" "), TEXT("_"));
	FString TextureName = ObjectName;
	if (ObjectName == "None")
		TextureName = "MultiPacker";
	if (TextureName.Contains("/"))
		TextureName = TextureName.Mid(TextureName.Find("/", ESearchCase::IgnoreCase, ESearchDir::FromEnd) + 1);
	int index = 0;//needs a int obligatory
	FString NewPackageName = importDirectory.FindLastChar(*TEXT("/"), index) ? importDirectory : importDirectory + "/";
	NewPackageName = TEXT("/Game/") + (importDirectory == "" ? "Textures/" : NewPackageName) + TextureName;
	FString Name;
	FString PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(NewPackageName, TextureName, PackageName, Name);
	TArray<FString> NamesOut;
	NamesOut.Add(TextureName);
	NamesOut.Add(PackageName);
	return NamesOut;
}

TSharedRef<class SButton> UMultiPackerChannelTab::CreateAndSetButton(FOnClicked InOnClicked, const TAttribute<FText>& InIcon, const TAttribute<FText>& InTooltip, const TAttribute<FText>& InButton)
{
	return SNew(SButton)
		.ButtonStyle(FEditorStyle::Get(), "FlatButton")
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.ToolTipText(InTooltip)
		.ContentPadding(FMargin(6, 2))
		.OnClicked(InOnClicked)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
				.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.11"))
				.Text(InIcon)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(4, 0, 0, 0)
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
				.Text(InButton)
			]
		];
}

FReply UMultiPackerChannelTab::PreviewRedTexture()
{
	if (PropertyMPChannel && PropertyMPChannel->TextureRed)
	{
		PreviewTexture(PropertyMPChannel->MaskTextureRed, PropertyMPChannel->TextureRed, PropertyMPChannel->InvertRed);
	}
	return FReply::Handled();
}

FReply UMultiPackerChannelTab::PreviewGreenTexture()
{
	if (PropertyMPChannel && PropertyMPChannel->TextureGreen)
	{
		PreviewTexture(PropertyMPChannel->MaskTextureGreen, PropertyMPChannel->TextureGreen, PropertyMPChannel->InvertGreen);
	}
	return FReply::Handled();
}

FReply UMultiPackerChannelTab::PreviewBlueTexture()
{
	if (PropertyMPChannel && PropertyMPChannel->TextureBlue)
	{
		PreviewTexture(PropertyMPChannel->MaskTextureBlue, PropertyMPChannel->TextureBlue, PropertyMPChannel->InvertBlue);
	}
	return FReply::Handled();
}

FReply UMultiPackerChannelTab::PreviewAlphaTexture()
{
	if (PropertyMPChannel && PropertyMPChannel->TextureAlpha)
	{
		PreviewTexture(PropertyMPChannel->MaskTextureAlpha, PropertyMPChannel->TextureAlpha, PropertyMPChannel->InvertAlpha);
	}
	return FReply::Handled();
}

void UMultiPackerChannelTab::PreviewTexture(EMPChannelMaskParameterColor InEnum, UTexture2D* InTexture, bool InvertColors)
{
	EChannelSelectionInput InChannel = GetChannelEnum(InEnum);
	UTexture2D* Texture = Cast<UTexture2D>(InTexture);
	UTilePointer* InTile = NewObject<UTilePointer>(UTilePointer::StaticClass()); 
	InTile->GenerateFromTexture(Texture, Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
	InTile->FromChannelToTexture(InChannel);
	if (InvertColors)
		InTile->InvertAllChannels();
	PreviewTextureViewport->SetPreviewTexture(InTile->TileTexture);
}

#undef LOCTEXT_NAMESPACE
