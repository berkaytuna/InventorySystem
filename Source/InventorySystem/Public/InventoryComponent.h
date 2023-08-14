// Written by Berkay Tuna, November 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlotStruct.h"
#include "GameFramework/InputSettings.h"
#include "InventoryInterface.h"
#include "Engine/UserDefinedStruct.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipKeyDownUI, int32, SlotIndex);

class UWidget;
class UInventoryWindow;
class UCharacterSheet;
class UInventoryWidget;
class ULootWindow;
class USlotWindow;
class UTexture2D;
class UContainer;
class UInfoWindow;

UENUM()
enum EUIInputMode
{
	UIOnly,
	GameAndUI
};

USTRUCT()
struct FBPClasses
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "BP Classes")
	UClass* InventoryWindow;

	UPROPERTY(EditAnywhere, Category = "BP Classes")
	UClass* CharacterSheet;

	UPROPERTY(EditAnywhere, Category = "BP Classes")
	UClass* InfoWindow;

	UPROPERTY(EditAnywhere, Category = "BP Classes")
	UClass* InventorySlot;
};

UCLASS(Blueprintable, ClassGroup = (InventorySystem), meta = (BlueprintSpawnableComponent, DisplayName = "Inventory Component"))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

	/* Broadcasted on equip key down during UI Focus */
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnEquipKeyDownUI OnEquipKeyDown;

	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FKey ToggleKey;
	
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FKey EquipKey;

	UPROPERTY(EditAnywhere, Category = "Inventory System")
	TEnumAsByte<EUIInputMode> UIInputMode;

	UPROPERTY(EditAnywhere, Category = "Inventory System")
	EMouseLockMode WidgetMouseLockMode;
	
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FBPClasses BPClasses;
	
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	int32 NumberOfSlots = 0;

	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FString SlotStruct;

	UPROPERTY()
	UInventoryWindow* InventoryWindow;
	
	UPROPERTY()
	UCharacterSheet* CharacterSheet;
	
	UPROPERTY()
	UInfoWindow* InfoWindow;
	
	/** Toggles Inventory Window */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void ToggleInventoryWindow();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetInventorySlotImage(int32 Index, UTexture2D* Icon);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void RemoveInventorySlotImage(int32 Index);

protected:
	virtual void BeginPlay() override;

private:
	int32 CurrentSlotIndex;

	/** Returns current Slot index */
	int32 GetCurrentSlotIndex();

	/** Called when a slot is added to the Focus Path */
	void OnSlotAddedToFocusPath(int32 InIndex);

	/** Broadcasted when a key is pressed during Inventory Focus */
	void BroadcastKeyDown(const FKey& InKey);

	/** Displays Inventory Window */
	void DisplayInventoryWindow();

	/** Displays Character Sheet */
	void DisplayCharacterSheet();

	/** Displays Inventory Widget */
	void DisplayInventoryWidget(UInventoryWidget* Widget, FString Message);

	/** Hides Inventory Window */
	void HideInventoryWindow();

	/** Hides Character Sheet */
	void HideCharacterSheet();

	/** Hides Inventory Widget */
	void HideInventoryWidget(UInventoryWidget* Widget, FString Message);

	/** Returns if Inventory Widget is currently visible */
	bool IsInventoryWidgetVisible(UInventoryWidget* Widget, FString Message);

	/** Returns if Inventory Window is currently visible */
	bool IsInventoryWindowVisible();

	/** Returns if Character Sheet is currently visible */
	bool IsCharacterSheetVisible();
};