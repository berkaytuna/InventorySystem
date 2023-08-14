// Copyright notice.

#include "InventoryComponent.h"
#include "Components/CanvasPanel.h"
#include "InventoryWindow.h"
#include "CharacterSheet.h"
#include "InventorySlot.h"
#include "EquipmentSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "LootWindow.h"
#include "InventorySystemBPLibrary.h"
#include "Container.h"
#include "Item.h"
#include "InfoWindow.h"
#include "Engine/Texture2D.h"


UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (! UKismetSystemLibrary::IsStandalone(this) && GetOwner()->HasAuthority())
	{
		return;
	}

	UWorld* World = GetWorld();

	// Inventory Window
	if (BPClasses.InventoryWindow)
	{
		InventoryWindow = CreateWidget<UInventoryWindow>(World, BPClasses.InventoryWindow);
		InventoryWindow->CreateSlots(NumberOfSlots, BPClasses.InventorySlot);
		InventoryWindow->KeyDown.BindUObject(this, &UInventoryComponent::BroadcastKeyDown);
	}
	else
	{
		//TODO: Find out how to use FString here
		// e.g. UE_LOG(LogTemp, Warning, debugMessage.GetCharArray(), "Inventory Window")
		UE_LOG(LogTemp, Warning, TEXT("Inventory Window not set!"));
		// TODO: Write also to screen as user input
		return;
	}

	/*// Character Sheet
	if (BPClasses.CharacterSheet)
	{
		CharacterSheet = CreateWidget<UCharacterSheet>(World, BPClasses.CharacterSheet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Sheet not set!"));
		// TODO: Write also to screen as user input
		return;
	}*/

	/*// Info Window
	if (BPClasses.InfoWindow)
	{
		InfoWindow = CreateWidget<UInfoWindow>(World, BPClasses.InfoWindow);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Info Window not set!"));
		// TODO: Write also to screen as user input
		return;
	}*/
}

void UInventoryComponent::OnSlotAddedToFocusPath(int32 InIndex)
{
	CurrentSlotIndex = InIndex;
}

void UInventoryComponent::BroadcastKeyDown(const FKey& InKey)
{
	if (InKey == ToggleKey)
	{
		HideInventoryWindow();
	}
	else if (InKey == EquipKey)
	{
		OnEquipKeyDown.Broadcast(InventoryWindow->GetCurrentSlotIndex());
	}
}

void UInventoryComponent::ToggleInventoryWindow()
{
	if (!IsInventoryWindowVisible())
	{
		DisplayInventoryWindow();
	}
	else
	{
		HideInventoryWindow();
	}
}

void UInventoryComponent::DisplayInventoryWindow()
{
	DisplayInventoryWidget(InventoryWindow, "Inventory Window is not available!");
}

void UInventoryComponent::DisplayCharacterSheet()
{
	DisplayInventoryWidget(CharacterSheet, "Character Sheet is not available!");
}

void UInventoryComponent::DisplayInventoryWidget(UInventoryWidget* Widget, FString Message)
{
	if (Widget)
	{
		Widget->AddToViewport();
		switch (UIInputMode)
		{
		case EUIInputMode::UIOnly:
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
				UGameplayStatics::GetPlayerController(this, 0),
				Widget->GetWidgetToFocus(),
				WidgetMouseLockMode);
		case EUIInputMode::GameAndUI:
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(
				UGameplayStatics::GetPlayerController(this, 0),
				Widget->GetWidgetToFocus(),
				WidgetMouseLockMode);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}
}

void UInventoryComponent::HideInventoryWindow()
{
	HideInventoryWidget(InventoryWindow, "Inventory Window is not available!");
}

void UInventoryComponent::HideCharacterSheet()
{
	HideInventoryWidget(CharacterSheet, "Character Sheet is not available!");
}

void UInventoryComponent::HideInventoryWidget(UInventoryWidget* Widget, FString Message)
{
	if (Widget)
	{
		Widget->RemoveFromViewport();
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(this, 0));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}
}

bool UInventoryComponent::IsInventoryWindowVisible()
{
	return IsInventoryWidgetVisible(InventoryWindow, "Inventory Window is not available!");
}

bool UInventoryComponent::IsCharacterSheetVisible()
{
	return IsInventoryWidgetVisible(CharacterSheet, "Character Sheet is not available!");
}

bool UInventoryComponent::IsInventoryWidgetVisible(UInventoryWidget* Widget, FString Message)
{
	if (Widget)
	{
		return Widget->IsInViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		return false;
	}
}

void UInventoryComponent::SetInventorySlotImage(int32 Index, UTexture2D* Icon)
{
	InventoryWindow->SetSlotImage(Icon, Index);
}

void UInventoryComponent::RemoveInventorySlotImage(int32 Index)
{
	InventoryWindow->RemoveSlotImage(Index);
}

int32 UInventoryComponent::GetCurrentSlotIndex()
{
	int32 Index = -1;
	if (!InventoryWindow->IsInViewport() && !CharacterSheet->IsInViewport())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Inventory Widget in Viewport (InventoryComponent GetCurrentSlotIndex) !"));
		Index = -1;
	}
	else
	{
		Index = CurrentSlotIndex;
	}

	return Index;
}