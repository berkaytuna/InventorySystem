// Written by Berkay Tuna, November 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlotStruct.h"
#include "GameFramework/InputSettings.h"
#include "InventoryInterface.h"

#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyDownUI, const FKey&, Key);

class UWidget;
class UInventoryWindow;
class UCharacterSheet;
class UInventoryWidget;
class ULootWindow;
class USlotWindow;
class UTexture2D;
class UContainer;
class UInfoWindow;

UENUM(BlueprintType)
enum EClickReason
{
	Use,
	Equip,
	UnEquip,
	Drop,
	Take,
};

UENUM()
enum ERemoveReason
{
	Placeholder,
	/*Use,
	Equip,
	Drop,
	Destroy*/
};

UENUM()
enum EToggleAction
{
	Inventory,
	CharacterSheet,
	LootWindow,   
};

UENUM()
enum EInputAction
{
	ChangeFocusedWindow,
	ToggleInventory,
};

UENUM()
enum EUIInputMode
{
	UIOnly,
	GameAndUI
};

USTRUCT()
struct FNumberOfSlots
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Number of Slots")
	int32 InventoryWindow;

	UPROPERTY(EditAnywhere, Category = "Number of Slots")
	int32 LootWindow;
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
	UClass* LootWindow;

	UPROPERTY(EditAnywhere, Category = "BP Classes")
	UClass* InfoWindow;

	UPROPERTY(EditAnywhere, Category = "BP Classes")
	UClass* InventorySlot;
};

USTRUCT()
struct FActionMappingNames
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Action Mapping Names")
	FName ToggleInventory;

	UPROPERTY(EditAnywhere, Category = "Action Mapping Names")
	FName ToggleCharacterSheet;

	UPROPERTY(EditAnywhere, Category = "Action Mapping Names")
	FName ToggleLootWindow;

	UPROPERTY(EditAnywhere, Category = "Action Mapping Names")
	FName SwitchFocusedWindow;
};

UCLASS(Blueprintable, ClassGroup = (InventorySystem), meta = (BlueprintSpawnableComponent, DisplayName = "Inventory Component"))
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

	/* Broadcasted on key down during UI Focus */
	UPROPERTY(BlueprintAssignable, Category = "Inventory System")
	FOnKeyDownUI OnKeyDown;

	UPROPERTY(EditAnywhere, Category = "Inventory System")
	TEnumAsByte<EUIInputMode> UIInputMode;
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	EMouseLockMode WidgetMouseLockMode;
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FBPClasses BPClasses;
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FActionMappingNames ActionMappingNames;
	UPROPERTY(EditAnywhere, Category = "Inventory System")
	//FNumberOfSlots NumberOfSlots;
	uint8 NumberOfSlots = 0;

	UPROPERTY(EditAnywhere, Category = "Inventory System")
	FString SlotStruct;

	// Inventory Widgets are the main widgets on screen
	UPROPERTY()
	UInventoryWindow* InventoryWindow;
	UPROPERTY()
	UCharacterSheet* CharacterSheet;
	UPROPERTY()
	UInventoryWindow* LootWindow;
	UPROPERTY()
	UInfoWindow* InfoWindow;

	/** Interact function to use with the items and containers */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void Interact();
	/** Gets the reason a slot is clicked */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	EClickReason GetClickReason() { return ClickReason; };
	TArray<FInputActionKeyMapping> GetToggleActionKeyMappings(EToggleAction ToggleAction);
	TArray<FInputActionKeyMapping> GetInputActionKeyMappings(EInputAction InputAction);
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void ToggleWidget(EToggleAction ToggleAction);
	void ToggleWidgetNative(EToggleAction ToggleAction);
	/** Sets the loot window with the inventory of the container */
	void SetLootWindow(TArray<FSlotStruct> InInventory);
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void ToggleInventory();
	void BroadcastKeyDown(const FKey& InKey);
	//UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Inventory System")
	//void OnSlotClicked(FSlotStruct SlotStruct);

	/** Displays Inventory Window */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void DisplayInventoryWindow();

	/** Displays Character Sheet */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void DisplayCharacterSheet();

	/** Hides Inventory Window */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void HideInventoryWindow();

	/** Hides Character Sheet */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void HideCharacterSheet();

	/** Returns if Inventory Window is currently visible */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool IsInventoryWindowVisible();

	/** Returns if Character Sheet is currently visible */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	bool IsCharacterSheetVisible();

	/** Returns Inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	TArray<AActor*> GetInventory();

	/** Adds an actor to the Inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory System", meta = (AdvancedDisplay = "InIndex"))
	void AddToInventory(AActor* InActor = NULL, UTexture2D* InIcon = NULL, int32 InIndex = -1);

	/** Removes an actor from the Inventory by index */
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void RemoveFromInventory(uint8 InIndex);

protected:
	virtual void BeginPlay() override;

private:	
	EClickReason ClickReason;
	AContainer* ActiveContainer;
	uint8 WidgetCount;
	TArray<UInventoryWidget*> InventoryWidgets;
	// Widget array containing to be focused widgets when their owning Inventory Widget is active
	TArray<TWeakObjectPtr<UWidget>> FocusedWidgets;
	TArray<TWeakObjectPtr<UInventoryWidget>> InventoryWidgetsInViewport;

	TArray<AActor*> Inventory;

	TArray<UTexture2D*> Icons;

	/** Displays Inventory Widget */
	void DisplayInventoryWidget(UInventoryWidget* Widget, FString Message);

	/** Hides Inventory Widget */
	void HideInventoryWidget(UInventoryWidget* Widget, FString Message);

	/** Returns if Inventory Widget is currently visible */
	bool IsInventoryWidgetVisible(UInventoryWidget* Widget, FString Message);

	void BindDelegates(UInventoryWidget* InventoryWidget);
	void Equip(FItemStruct Item);
	void InitializeInventory();
	void OnSlotAddedToFocusPath(FSlotStruct InSlotStruct);
	void OnSlotRemovedFromFocusPath();
	void OnSlotClicked(UInventoryWidget* InInventoryWidget, uint8 InSlotIndex, FSlotStruct InSlotStruct);
	UInventoryWidget* CreateInventoryWidget(UClass* WidgetClass);
	void ToggleWidgetInternal(UInventoryWidget* InWidget, EMouseLockMode InMouseLockMode);
	bool CheckIfKeyPressed(TArray<FInputActionKeyMapping> InInputActionKeyMappings, FKey PressedKey);

	void InitializeInventoryWindow();

	int32 NextAvailableInventoryIndex;
};