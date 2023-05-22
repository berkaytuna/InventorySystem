// Copyright notice.

#include "InventoryComponent.h"
#include "Components/CanvasPanel.h"
#include "InventoryWindow.h"
#include "CharacterSheet.h"
#include "InventorySlot.h"
#include "EquipmentSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetArrayLibrary.h"
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

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Create Inventory Widgets
	UWorld* World = GetWorld();

	//FString debugMessage = "Please set the %s of Inventory Component!";
	// Inventory Window
	PrepareInventory();
	if (BPClasses.InventoryWindow)
	{
		InventoryWindow = CreateWidget<UInventoryWindow>(World, BPClasses.InventoryWindow);
	}
	else
	{
		//TODO: Find out how to use FString here
		// e.g. UE_LOG(LogTemp, Warning, debugMessage.GetCharArray(), "Inventory Window")
		UE_LOG(LogTemp, Warning, TEXT("Inventory Window not set!"));
		// TODO: Write also to screen as user input
		return;
	}
	InventoryWindow->CreateSlots(NumberOfSlots.InventoryWindow, BPClasses.InventorySlot);

	// Character Sheet
	if (BPClasses.CharacterSheet)
	{
		CharacterSheet = CreateWidget<UCharacterSheet>(World, BPClasses.CharacterSheet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Sheet not set!"));
		// TODO: Write also to screen as user input
		return;
	}

	/*// Loot Window
	if (BPClasses.LootWindow)
	{
		LootWindow = CreateWidget<ULootWindow>(World, BPClasses.LootWindow);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Loot Window not set!"));
		// TODO: Write also to screen as user input
		return;
	}
	LootWindow->CreateSlots(NumberOfSlots.LootWindow, BPClasses.LootWindow);

	// Info Window
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

	//PrepareInventory();
	//InventoryWindow = Cast<UInventoryWindow>(CreateInventoryWidget(InventoryWindowBPClass));
	//InventoryWindow->CreateInventorySlots(InventorySlotBPClass);
	//CharacterSheet = Cast<UCharacterSheet>(CreateInventoryWidget(CharacterSheetBPClass));
	//LootWindow = Cast<UInventoryWindow>(CreateInventoryWidget(LootWindowBPClass));
	//LootWindow->CreateInventorySlots(InventorySlotBPClass);
	//UWorld* World = this->GetWorld();
	//InfoWindow = CreateWidget<UInfoWindow>(World, InfoWindowBPClass);

	// Binding delegates for Inventory Widgets
	InventoryWidgets = { InventoryWindow, CharacterSheet/*, LootWindow*/};
	for (uint8 i = 0; i < InventoryWidgets.Num(); i++) {
		BindDelegates(InventoryWidgets[i]);
	}
}

void UInventoryComponent::BindDelegates(UInventoryWidget* InventoryWidget)
{
	InventoryWidget->KeyDown.BindUObject(this, &UInventoryComponent::BroadcastKeyDown);
	//InventoryWidget->SlotClicked.BindUObject(this, &UInventoryComponent::OnSlotClicked);
	//InventoryWidget->SlotAddedToFocusPath.BindUObject(this, &UInventoryComponent::OnSlotAddedToFocusPath);
	//InventoryWidget->SlotRemovedFromFocusPath.BindUObject(this, &UInventoryComponent::OnSlotRemovedFromFocusPath);
}

void UInventoryComponent::OnSlotAddedToFocusPath(FSlotStruct InSlotStruct)
{
	InfoWindow->SetSlotStruct(InSlotStruct);
	InfoWindow->SetClickReason(ClickReason);
	if (InSlotStruct.Quantity > 0)
		InfoWindow->AddToViewport();
}

void UInventoryComponent::OnSlotRemovedFromFocusPath()
{
	InfoWindow->RemoveFromViewport();
	FSlotStruct SlotStruct = { };
	InfoWindow->SetSlotStruct(SlotStruct);
}

void UInventoryComponent::Interact()
{
	TArray<AActor*> OverlappingActors;
	GetOwner()->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	for (int32 i = 0; i < OverlappingActors.Num(); i++) {
		AActor* OverlappingActor = OverlappingActors[i];
		bool IsActorInteractable = UKismetSystemLibrary::DoesImplementInterface(OverlappingActor, UInteractInterfaceInventory::StaticClass());
		if (IsActorInteractable) {
			AItemCpp* Item = Cast<AItemCpp>(OverlappingActor);
			if (Item != nullptr) {
				FItemStruct ItemStruct = Item->GetItemStruct();
				FSlotStruct SlotStruct = { ItemStruct, 1 };
				uint8 Index = AddToInventory(SlotStruct);
				InventoryWindow->AddItemToInventory(Index, SlotStruct);
				Item->Destroy();
				return;
			}
			AContainer* Container = Cast<AContainer>(OverlappingActor);
			if (Container != nullptr) {
				if (Container->IsLootable()) {
					ActiveContainer = Container;
					TArray<FSlotStruct> Loot = ActiveContainer->GetInventory();
					SetLootWindow(Loot);
				}
				return;
			}
		}
	}
}

void UInventoryComponent::OnSlotClicked(UInventoryWidget* InInventoryWidget, uint8 InSlotIndex, FSlotStruct InSlotStruct)
{
	if (InInventoryWidget == CharacterSheet)
		ClickReason = EClickReason::UnEquip;
	else if (InInventoryWidget == LootWindow)
		ClickReason = EClickReason::Take;
	else {
		// When InventoryWindow has the focus do the appropriate action according to which widget is open
		if (ClickReason == EClickReason::UnEquip || ClickReason == EClickReason::Take) {
			if (LootWindow->IsInViewport())
				ClickReason = EClickReason::Drop;
			else
				ClickReason = EClickReason::Equip;
		}
	}

	OnSlotClicked(InSlotStruct);
	uint8 Index = 0;

	switch (ClickReason)
	{
	case EClickReason::Equip:
		RemoveFromInventory(InSlotIndex);
		Equip(InSlotStruct.Item);
		break;
	case EClickReason::Use:
		RemoveFromInventory(InSlotIndex);
		break;
	case EClickReason::Drop:
		RemoveFromInventory(InSlotIndex);
		if (LootWindow->IsInViewport()) {
			Index = ActiveContainer->AddToInventory(InSlotStruct);
			LootWindow->AddItemToInventory(Index, InSlotStruct);
		}
		break;
	case EClickReason::Take:
		ActiveContainer->RemoveFromInventory(InSlotIndex);
		Index = AddToInventory(InSlotStruct);
		InventoryWindow->AddItemToInventory(Index, InSlotStruct);
		break;
	case EClickReason::UnEquip:
		Index = AddToInventory(InSlotStruct);
		InventoryWindow->AddItemToInventory(Index, InSlotStruct);
		break;
	}
}

/*UInventoryWidget* UInventoryComponent::CreateInventoryWidget(UClass* WidgetClass)
{
	UWorld* World = this->GetWorld();
	UInventoryWidget* CreatedWidget = CreateWidget<UInventoryWidget>(World, WidgetClass);

	if (CreatedWidget != nullptr)
	{
		UInventoryWindow* InventoryWindowInternal = Cast<UInventoryWindow>(CreatedWidget);
		if ( InventoryWindowInternal != nullptr)
		{
			InventoryWindowInternal->SetNumberOfSlots(NumberOfSlots);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget creation failed, please check if InventoryComponent has correct value: %s"), *WidgetClass->GetName());
	}

	return CreatedWidget;
}*/

void UInventoryComponent::ToggleInventory()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller is not available!"));
		return;
	}

	if (CharacterSheet)
	{
		CharacterSheet->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Sheet is not available!"));
	}

	if (InventoryWindow)
	{
		InventoryWindow->AddToViewport();
		UWidget* WidgetToFocus = InventoryWindow->GetWidgetToFocus();
		
		if (WidgetToFocus)
		{
			ClickReason = EClickReason::Equip;
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, WidgetToFocus, WidgetMouseLockMode);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Please use SetWidgetToFocus function of the Inventory Window and set the Widget that should be focused when Inventory is first opened!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Window is not available!"));
	}
}

bool UInventoryComponent::CheckIfKeyPressed(TArray<FInputActionKeyMapping> InInputActionKeyMappings, FKey PressedKey)
{
	for (int32 i = 0; i < InInputActionKeyMappings.Num(); i++)
	{
		FKey InputActionKey = InInputActionKeyMappings[i].Key;

		if (PressedKey == InputActionKey)
		{
			return true;
		}
	}

	return false;
}

void UInventoryComponent::BroadcastKeyDown(const FKey& InKey)
{
	//FKey PressedKey = InKeyEvent.GetKey();
	OnKeyDown.Broadcast(InKey);

	/*TArray<FInputActionKeyMapping> ChangeFocusedWindowKeyMappings = GetInputActionKeyMappings(EInputAction::ChangeFocusedWindow);
	bool IsChangeFocusedWindowKeyPressed = CheckIfKeyPressed(ChangeFocusedWindowKeyMappings, PressedKey);

	TArray<FInputActionKeyMapping> ToggleInventoryKeyMappings = GetInputActionKeyMappings(EInputAction::ToggleInventory);
	bool IsToggleInventoryKeyPressed = CheckIfKeyPressed(ToggleInventoryKeyMappings, PressedKey);

	EInputAction InputAction;
	if (IsChangeFocusedWindowKeyPressed)
		InputAction = EInputAction::ChangeFocusedWindow;
	else if (IsToggleInventoryKeyPressed)
		InputAction = EInputAction::ToggleInventory;

	// Switch according to the Key pressed
	switch (InputAction)
	{
	case EInputAction::ChangeFocusedWindow:
		if (InventoryWindow->HasFocusedDescendants())
		{
			if (CharacterSheet->IsInViewport())
			{
				CharacterSheet->GetWidgetToFocus()->SetKeyboardFocus();
				ClickReason = EClickReason::UnEquip;
			}
			else
			{
				LootWindow->GetWidgetToFocus()->SetKeyboardFocus();
				ClickReason = EClickReason::Take;
			}
		}
		else
		{
			InventoryWindow->GetWidgetToFocus()->SetKeyboardFocus();

			if (CharacterSheet->IsInViewport())
			{			
				ClickReason = EClickReason::Equip;
			}
			else
			{
				ClickReason = EClickReason::Drop;
			}
		}
		break;
	case EInputAction::ToggleInventory:
		InventoryWindow->GetFirstWidgetToFocus()->SetKeyboardFocus();
		InventoryWindow->RemoveFromViewport();
		InventoryWindow->SetWidgetToFocus(InventoryWindow->GetFirstWidgetToFocus());
		if (CharacterSheet->IsInViewport())
		{
			CharacterSheet->RemoveFromViewport();
			CharacterSheet->SetWidgetToFocus(CharacterSheet->GetFirstWidgetToFocus());
		}
		else
		{
			LootWindow->RemoveFromViewport();
			ActiveContainer = nullptr;
			LootWindow->SetWidgetToFocus(LootWindow->GetFirstWidgetToFocus());
		}
		InfoWindow->RemoveFromViewport();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		break;
	}*/
}

void UInventoryComponent::ToggleWidget(EToggleAction InToggleAction)
{
	switch (InToggleAction)
	{
	case EToggleAction::Inventory:
		ToggleWidgetInternal(InventoryWindow, WidgetMouseLockMode);
		break;
	case EToggleAction::CharacterSheet:
		ToggleWidgetInternal(CharacterSheet, WidgetMouseLockMode);
		break;
	case EToggleAction::LootWindow:
		ToggleWidgetInternal(LootWindow, WidgetMouseLockMode);
		break;
	}
}

void UInventoryComponent::ToggleWidgetNative(EToggleAction InToggleAction)
{
	switch (InToggleAction)
	{
	case EToggleAction::Inventory:
		ToggleWidgetInternal(InventoryWindow, WidgetMouseLockMode);
		break;
	case EToggleAction::CharacterSheet:
		ToggleWidgetInternal(CharacterSheet, WidgetMouseLockMode);
		break;
	case EToggleAction::LootWindow:
		// Loot Window can only be opened through Blueprints
		if (LootWindow->IsInViewport())
		{
			ToggleWidgetInternal(LootWindow, WidgetMouseLockMode);
		}
		break;
	}
}

void UInventoryComponent::ToggleWidgetInternal(UInventoryWidget* InWidget, EMouseLockMode InMouseLockMode)
{
	if (InWidget == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (InWidget->HasFocusedDescendants())
	{
		// Close Widget
		InWidget->RemoveFromViewport();
		InventoryWidgetsInViewport.Remove(InWidget);
		if (InventoryWidgetsInViewport.Num() == 0)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		}
		else
		{
			UInventoryWidget* InventoryWidgetToFocus = InventoryWidgetsInViewport[InventoryWidgetsInViewport.Num() - 1].Get();
			UWidget* WidgetToFocus = InventoryWidgetToFocus->GetWidgetToFocus();
			WidgetToFocus->SetKeyboardFocus();
		}
	}
	else
	{
		if (!InWidget->IsInViewport())
		{
			InWidget->AddToViewport();
		}
		else
		{
			InventoryWidgetsInViewport.Remove(InWidget);
		}

		InventoryWidgetsInViewport.Add(InWidget);
		UWidget* WidgetToFocus = InWidget->GetWidgetToFocus();

		if (InventoryWidgetsInViewport.Num() == 1)
		{
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, WidgetToFocus, InMouseLockMode);
		}

		if (WidgetToFocus != nullptr)
		{
			WidgetToFocus->SetKeyboardFocus();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetToFocus was not set for: %s"), *InWidget->GetName());
		}
	}
}

TArray<FInputActionKeyMapping> UInventoryComponent::GetToggleActionKeyMappings(EToggleAction InToggleAction)
{
	TArray<FInputActionKeyMapping> ToggleInventoryKeyMappings;
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	switch (InToggleAction)
	{
	case EToggleAction::Inventory:		
		InputSettings->GetActionMappingByName(ActionMappingNames.ToggleInventory, ToggleInventoryKeyMappings);
		break;
	case EToggleAction::CharacterSheet:
		InputSettings->GetActionMappingByName(ActionMappingNames.ToggleCharacterSheet, ToggleInventoryKeyMappings);
		break;
	case EToggleAction::LootWindow:
		InputSettings->GetActionMappingByName(ActionMappingNames.ToggleLootWindow, ToggleInventoryKeyMappings);
		break;
	}

	if (ToggleInventoryKeyMappings.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Action Mappings found, please check if the Action Mappings section in the Inventory Component and Input Seettings correct"));
	}

	return ToggleInventoryKeyMappings;
}

TArray<FInputActionKeyMapping> UInventoryComponent::GetInputActionKeyMappings(EInputAction InputAction)
{
	TArray<FInputActionKeyMapping> InputActionKeyMappings;
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	FName ActionName;

	switch (InputAction)
	{
	case EInputAction::ChangeFocusedWindow:
		ActionName = ActionMappingNames.SwitchFocusedWindow;
		break;
	case EInputAction::ToggleInventory:
		ActionName = ActionMappingNames.ToggleInventory;
		break;
	}

	InputSettings->GetActionMappingByName(ActionName, InputActionKeyMappings);

	return InputActionKeyMappings;
}

void UInventoryComponent::PrepareInventory()
{
	Inventory.Empty(NumberOfSlots.InventoryWindow);
	Inventory.AddZeroed(NumberOfSlots.InventoryWindow);
}

void UInventoryComponent::Equip(FItemStruct Item)
{
	TArray<UEquipmentSlot*> EquipmentSlots = CharacterSheet->GetSlots();
	FSlotStruct SlotStruct{Item, 1};

	switch (Item.Subtype)
	{
	case Head:
		EquipmentSlots[0]->SetSlotStruct(SlotStruct);
		break;
	case Chest:
		EquipmentSlots[1]->SetSlotStruct(SlotStruct);
		break;
	case Legs:
		EquipmentSlots[2]->SetSlotStruct(SlotStruct);
		break;
	case Gloves:
		EquipmentSlots[3]->SetSlotStruct(SlotStruct);
		break;
	case Shoes:
		EquipmentSlots[4]->SetSlotStruct(SlotStruct);
		break;
	}
}

void UInventoryComponent::SetLootWindow(TArray<FSlotStruct> InInventory)
{
	LootWindow->SetInventory(InInventory);

	InventoryWindow->AddToViewport();
	InventoryWindow->GetWidgetToFocus()->SetKeyboardFocus();
	LootWindow->AddToViewport();
	ClickReason = EClickReason::Take;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UWidget* WidgetToFocus = LootWindow->GetWidgetToFocus();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, WidgetToFocus, WidgetMouseLockMode);
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