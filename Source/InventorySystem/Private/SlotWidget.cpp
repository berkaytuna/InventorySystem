// Copyright

#include "SlotWidget.h"
#include "InventorySystem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"

void USlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetButton()->OnHovered.AddDynamic(this, &USlotWidget::OnHovered);
	GetButton()->OnUnhovered.AddDynamic(this, &USlotWidget::OnUnhovered);

	if (Button != nullptr)
		Button->OnClicked.AddDynamic(this, &USlotWidget::OnClicked);
	else
		UE_LOG(LogTemp,Warning, TEXT("SlotWidget - Please use SetButtonAndImage function on initialization to set Button!"));
}

void USlotWidget::SetButtonAndImages(UButton* InButton, UImage* InImage, UImage* InShadowImage, UTexture2D* InEmptySlotTexture)
{
	Button = InButton;
	Image = InImage;
	ShadowImage = InShadowImage;
	EmptySlotTexture = InEmptySlotTexture;
}

void USlotWidget::DisplayShadow()
{
	FSlateColor NewTintColor{ FLinearColor(0.0f, 0.0f, 0.0f, 0.35f) };
	ShadowImage->SetBrushTintColor(NewTintColor);
}

void USlotWidget::HideShadow()
{
	FSlateColor NewTintColor{ FLinearColor(0.0f, 0.0f, 0.0f, 0.0f) };
	ShadowImage->SetBrushTintColor(NewTintColor);
}

void USlotWidget::Empty()
{
	SlotStruct = {};
	Image->SetBrushFromTexture(EmptySlotTexture);
}

void USlotWidget::SetSlotStruct(FSlotStruct InSlotStruct)
{
	SlotStruct = InSlotStruct;
	UTexture2D* NewThumbnail = SlotStruct.Item.Thumbnail;
	Image->SetBrushFromTexture(NewThumbnail);
}

void USlotWidget::OnClicked()
{
	if (SlotStruct.Quantity > 0)
		Clicked.ExecuteIfBound(this);
}

void USlotWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	AddedToFocusPath.ExecuteIfBound(this);
}

void USlotWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	RemovedFromFocusPath.ExecuteIfBound(this);
}

void USlotWidget::OnHovered()
{
	Hovered.ExecuteIfBound(this);
}

void USlotWidget::OnUnhovered()
{
	Unhovered.ExecuteIfBound(this);
}