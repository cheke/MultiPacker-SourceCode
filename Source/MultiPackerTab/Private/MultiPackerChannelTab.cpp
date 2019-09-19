/* Copyright 2019 @TurboCheke, Estudio Cheke  - All Rights Reserved */
#include "MultiPackerChannelTab.h"
#include "MultiPackerDetailPanel.h"
#include "MultiPackerSettings.h"
#include "MultiPackerCommands.h"
#include "TileUtils/TilePointer.h"
#include <PropertyEditorModule.h>
#include <IDetailsView.h>
#include <AssetToolsModule.h>
#include <AssetRegistryModule.h>
#include <ImageUtils.h>
#include <Editor.h>
#include <EditorFontGlyphs.h>
#include "IAssetTools.h"
#include <AssetToolsModule.h>
#include <Brushes/SlateImageBrush.h>
#include <Widgets/Input/SButton.h>
#include <Widgets/Docking/SDockTab.h>
#include <Widgets/Layout/SBox.h>
#include <Widgets/Text/STextBlock.h>
#include "Widgets/Images/SImage.h"
#include <Engine/TextureRenderTarget2D.h>
#include <Framework/Docking/TabManager.h>
#include <Framework/MultiBox/MultiBoxExtender.h>
#include <Framework/MultiBox/MultiBoxBuilder.h>

#define LOCTEXT_NAMESPACE "FMultiPackerChannelTab"

const FName MPChannelTabsEditor = FName(TEXT("MPChannelTabsEditor"));

struct FMultiPackerChannelTabs
{
	// Tab identifiers
	static const FName DetailsID;
	static const FName ViewportID;
};

const FName FMultiPackerChannelTabs::DetailsID(TEXT("Details"));
const FName FMultiPackerChannelTabs::ViewportID(TEXT("Viewport"));

FMultiPackerChannelTab::FMultiPackerChannelTab() {}

FMultiPackerChannelTab::~FMultiPackerChannelTab() {}

void FMultiPackerChannelTab::InitGenericGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost)
{
	FDetailsViewArgs DetailArgs;
	DetailArgs.bHideSelectionTip = true;
	DetailArgs.bAllowSearch = false;

	const UMultiPackerSettings* Settings = GetDefault<UMultiPackerSettings>();
	PropertyMPChannel = NewObject<UMultiPackerDetailPanel>(UMultiPackerDetailPanel::StaticClass());
	PropertyMPChannel->TargetDirectory = Settings->GetTargetDirectory();
	PropertyMPChannel->TextureName = Settings->GetTextureChannelName();
	PropertyMPChannel->AddToRoot();

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(DetailArgs);
	PropertyWidget->SetObject(PropertyMPChannel, true);

	//Fill Toolbar
	/*TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::Before, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FMultiPackerChannelTab::FillToolbar));
	AddToolbarExtender(ToolbarExtender);*/
	FillToolbar();
	// Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("MultiPackerChannelTabs_Editor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				//->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split				//Buttons
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.90f)
				->Split
				(
					FTabManager::NewStack()
					->SetHideTabWell(true)
					->AddTab(FMultiPackerChannelTabs::ViewportID, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.37f)
					->SetHideTabWell(true)
					->AddTab(FMultiPackerChannelTabs::DetailsID, ETabState::OpenedTab)
				)
			)
		);
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, MPChannelTabsEditor, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, PropertyMPChannel, false);
}

void FMultiPackerChannelTab::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_MultiPackerChannelTab", "ChannelTabEditor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FMultiPackerChannelTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FMultiPackerChannelTab::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FMultiPackerChannelTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FMultiPackerChannelTab::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.Tabs.Details"));
}

void FMultiPackerChannelTab::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FMultiPackerChannelTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FMultiPackerChannelTabs::DetailsID);
}

FName FMultiPackerChannelTab::GetToolkitFName() const
{
	return FName("FMultiPackerChannelTab");
}

FText FMultiPackerChannelTab::GetBaseToolkitName() const
{
	return LOCTEXT("MultiPackerChannelTabLabel", "MultiPackerChannelTabEditor");
}

FString FMultiPackerChannelTab::GetWorldCentricTabPrefix() const
{
	return TEXT("MPChannelTab");
}

FLinearColor FMultiPackerChannelTab::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

void FMultiPackerChannelTab::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(PropertyMPChannel);
}

TSharedRef<SDockTab> FMultiPackerChannelTab::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FMultiPackerChannelTabs::ViewportID);

	PreviewBrush = MakeShareable(new FSlateImageBrush(PropertyMPChannel->TextureRed, FVector2D(250, 250)));
	return SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"))
		.OnCanCloseTab(false)
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
				SAssignNew(PreviewWidget, SBorder)
				.HAlign(HAlign_Fill)//HAlign_Fill HAlign_Center
				.VAlign(VAlign_Fill)
				.BorderImage(FEditorStyle::GetBrush("BlackBrush"))
				[
					SNew(SBorder)
					.HAlign(HAlign_Center)//HAlign_Fill HAlign_Center
					.VAlign(VAlign_Center)
					[
						SAssignNew(ImageWidget, SImage)
						.Image(PreviewBrush.Get())
					]
				]
			]
		];
}

TSharedRef<SDockTab> FMultiPackerChannelTab::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FMultiPackerChannelTabs::DetailsID);

// 	FDetailsViewArgs DetailArgs;
// 	DetailArgs.bHideSelectionTip = true;
// 	DetailArgs.bAllowSearch = false;
// 
// 	const UMultiPackerSettings* Settings = GetDefault<UMultiPackerSettings>();
// 	PropertyMPChannel = NewObject<UMultiPackerDetailPanel>(UMultiPackerDetailPanel::StaticClass());
// 	PropertyMPChannel->TargetDirectory = Settings->GetTargetDirectory();
// 	PropertyMPChannel->TextureName = Settings->GetTextureChannelName();
// 	PropertyMPChannel->AddToRoot();
// 
// 	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
// 	PropertyWidget = PropertyModule.CreateDetailView(DetailArgs);
// 	PropertyWidget->SetObject(PropertyMPChannel, true);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Details"))
		.ShouldAutosize(true)
		.OnCanCloseTab(false)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(3.0f)
			[
				PropertyWidget.ToSharedRef()
			]
		];
}

void FMultiPackerChannelTab::FillToolbar(/*FToolBarBuilder& ToolbarBuilder*/)
{
	//Bind Toolbar Buttons
	ToolkitCommands->MapAction(FMultiPackerCTCommands::Get().ProcessTexture,
		FExecuteAction::CreateSP(this, &FMultiPackerChannelTab::OnClickButton),
		FCanExecuteAction::CreateSP(this, &FMultiPackerChannelTab::CanProcessTexture)
	);
	ToolkitCommands->MapAction(FMultiPackerCTCommands::Get().PreviewRed,
		FExecuteAction::CreateSP(this, &FMultiPackerChannelTab::PreviewRedTexture),
		FCanExecuteAction::CreateSP(this, &FMultiPackerChannelTab::CanPreviewRedTexture)
	);
	ToolkitCommands->MapAction(FMultiPackerCTCommands::Get().PreviewGreen,
		FExecuteAction::CreateSP(this, &FMultiPackerChannelTab::PreviewGreenTexture),
		FCanExecuteAction::CreateSP(this, &FMultiPackerChannelTab::CanPreviewGreenTexture)
	);
	ToolkitCommands->MapAction(FMultiPackerCTCommands::Get().PreviewBlue,
		FExecuteAction::CreateSP(this, &FMultiPackerChannelTab::PreviewBlueTexture),
		FCanExecuteAction::CreateSP(this, &FMultiPackerChannelTab::CanPreviewBlueTexture)
	);
	ToolkitCommands->MapAction(FMultiPackerCTCommands::Get().PreviewAlpha,
		FExecuteAction::CreateSP(this, &FMultiPackerChannelTab::PreviewAlphaTexture),
		FCanExecuteAction::CreateSP(this, &FMultiPackerChannelTab::CanPreviewAlphaTexture)
	);
	//Fill ToolBar
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	FToolBarBuilder ToolbarBuilder(GetToolkitCommands(), FMultiBoxCustomization::AllowCustomization(GetToolkitFName()), ToolbarExtender);
	ToolbarBuilder.BeginSection("MultiPacker");
	{
		ToolbarBuilder.AddToolBarButton(FMultiPackerCTCommands::Get().ProcessTexture,
			NAME_None,
			LOCTEXT("MP_CT_Process", "Process Texture"),
			LOCTEXT("MP_CT_Process_Tooltip", "Process the Channel Pack"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Recompile"));
		ToolbarBuilder.AddToolBarButton(FMultiPackerCTCommands::Get().PreviewRed,
			NAME_None,
			LOCTEXT("MP_CT_PreviewRed", "Preview Red Texture"),
			LOCTEXT("MP_CT_PreviewRed_Tooltip", "Preview the Red Texture filtered by Channel Selected"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "PropertyEditor.SlateBrushPreview"));
		ToolbarBuilder.AddToolBarButton(FMultiPackerCTCommands::Get().PreviewGreen,
			NAME_None,
			LOCTEXT("MP_CT_PreviewGreen", "Preview Green Texture"),
			LOCTEXT("MP_CT_PreviewGreen_Tooltip", "Preview the Green Texture filtered by Channel Selected"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "Persona.TogglePreviewAsset"));
		ToolbarBuilder.AddToolBarButton(FMultiPackerCTCommands::Get().PreviewBlue,
			NAME_None,
			LOCTEXT("MP_CT_PreviewBlue", "Preview Blue Texture"),
			LOCTEXT("MP_CT_PreviewBlue_Tooltip", "Preview the Blue Texture filtered by Channel Selected"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "Kismet.SetRealtimePreview"));
		ToolbarBuilder.AddToolBarButton(FMultiPackerCTCommands::Get().PreviewAlpha,
			NAME_None,
			LOCTEXT("MP_CT_PreviewAlpha", "Preview Alpha Texture"),
			LOCTEXT("MP_CT_PreviewAlpha_Tooltip", "Preview the Alpha Texture filtered by Channel Selected"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "PlayWorld.PlayInMobilePreview"));
	}
	ToolbarBuilder.EndSection();
	//ToolbarBuilder.GetMultiBox().Get().
	//TSharedRef<SHorizontalBox> MiscWidgets = SNew(SHorizontalBox);

	/*for (int32 WidgetIdx = 0; WidgetIdx < ToolbarWidgets.Num(); ++WidgetIdx)
	{
		MiscWidgets->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 2.0f, 0.0f, 2.0f)
			[
				ToolbarWidgets[WidgetIdx]
			];
	}*/

	TSharedPtr<SWidget> Toolbar;
	Toolbar =
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Bottom)
			[
				ToolbarBuilder.MakeWidget()
			]
		]
		/*+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(VAlign_Bottom)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush(TEXT("Toolbar.Background")))
				[
					MiscWidgets
				]
			]
		]*/;
		AddToolbarWidget(Toolbar.ToSharedRef());
	/*if (ToolbarWidgetContent.IsValid())
	{
		ToolbarWidgetContent->SetContent(Toolbar.ToSharedRef());
	}*/
}

void FMultiPackerChannelTab::OnClickButton()
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
	SetPreviewTexture(NewTexture);
}

EChannelSelectionInput FMultiPackerChannelTab::GetChannelEnum(EMPChannelMaskParameterColor InEnum)
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

UTilePointer* FMultiPackerChannelTab::ProcessTextureChannel(UTexture2D* InTexture, const int32 InSizeVertical, const int32 InSizeHorizontal, EMPChannelMaskParameterColor InChannel)
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

TArray<FString> FMultiPackerChannelTab::TexturePackageName(FAssetToolsModule& AssetToolsModule)
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

void FMultiPackerChannelTab::PreviewRedTexture()
{
	PreviewTexture(PropertyMPChannel->MaskTextureRed, PropertyMPChannel->TextureRed, PropertyMPChannel->InvertRed);
}

void FMultiPackerChannelTab::PreviewGreenTexture()
{
	PreviewTexture(PropertyMPChannel->MaskTextureGreen, PropertyMPChannel->TextureGreen, PropertyMPChannel->InvertGreen);
}

void FMultiPackerChannelTab::PreviewBlueTexture()
{
	PreviewTexture(PropertyMPChannel->MaskTextureBlue, PropertyMPChannel->TextureBlue, PropertyMPChannel->InvertBlue);
}

void FMultiPackerChannelTab::PreviewAlphaTexture()
{
	PreviewTexture(PropertyMPChannel->MaskTextureAlpha, PropertyMPChannel->TextureAlpha, PropertyMPChannel->InvertAlpha);
}

void FMultiPackerChannelTab::PreviewTexture(EMPChannelMaskParameterColor InEnum, UTexture2D* InTexture, bool InvertColors)
{
	EChannelSelectionInput InChannel = GetChannelEnum(InEnum);
	UTexture2D* Texture = Cast<UTexture2D>(InTexture);
	UTilePointer* InTile = NewObject<UTilePointer>(UTilePointer::StaticClass()); 
	InTile->GenerateFromTexture(Texture, Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
	InTile->FromChannelToTexture(InChannel);
	if (InvertColors)
		InTile->InvertAllChannels();
	SetPreviewTexture(InTile->TileTexture);
}

void FMultiPackerChannelTab::SetPreviewTexture(UTexture2D* InPreviewTexture)
{
	const FGeometry GeoWidget = PreviewWidget->GetCachedGeometry();
	const float SizeSquared = FMath::Min(GeoWidget.GetLocalSize().X, GeoWidget.GetLocalSize().Y);
	PreviewBrush = MakeShareable(new FSlateImageBrush(InPreviewTexture, FVector2D(SizeSquared, SizeSquared)));
	ImageWidget->SetImage(PreviewBrush.Get());
}

bool FMultiPackerChannelTab::CanProcessTexture()
{
	return (PropertyMPChannel->TextureRed != nullptr && PropertyMPChannel->TextureGreen != nullptr && PropertyMPChannel->TextureBlue != nullptr && (PropertyMPChannel->AlphaChannel ? PropertyMPChannel->TextureAlpha != nullptr : true));
}

bool FMultiPackerChannelTab::CanPreviewRedTexture()
{
	return (PropertyMPChannel && PropertyMPChannel->TextureRed);
}

bool FMultiPackerChannelTab::CanPreviewGreenTexture()
{
	return (PropertyMPChannel && PropertyMPChannel->TextureGreen);
}

bool FMultiPackerChannelTab::CanPreviewBlueTexture()
{
	return (PropertyMPChannel && PropertyMPChannel->TextureBlue);
}

bool FMultiPackerChannelTab::CanPreviewAlphaTexture()
{
	return (PropertyMPChannel && PropertyMPChannel->TextureAlpha);
}

#undef LOCTEXT_NAMESPACE
