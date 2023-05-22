// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/InputSettings.h"
#include "InventoryWidget.h"
#include "SlotStruct.h"
#include "InventoryWindow.generated.h"

class UUniformGridPanel;

UCLASS()
class INVENTORYSYSTEM_API UInventoryWindow : public UInventoryWidget
{
	GENERATED_BODY()
	
public:	

	void CreateSlots(int32 InNumberOfSlots, UClass* InventorySlotClass);
	void AddItemToInventory(int32 SlotIndex, FSlotStruct ItemToAdd);
	void RemoveItemFromInventory(int32 Index);
	void EmptyInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetInventoryGrid(UUniformGridPanel* NewInventoryGrid) { InventoryGrid = NewInventoryGrid; };
	void SetInventory(TArray<FSlotStruct> NewInventory);

protected:

	virtual void OnSlotClicked(USlotWidget* InSlotWidget) final;

private:

	int32 NumberOfSlots;

	UPROPERTY()
	UUniformGridPanel* InventoryGrid;

	int32 GetNumberOfSlots() { return NumberOfSlots; };

	void SetNumberOfSlots(int32 NewNumberOfSlots) { NumberOfSlots = NewNumberOfSlots; };
};
