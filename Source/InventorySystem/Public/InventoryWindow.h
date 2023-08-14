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

	bool IsSlotEmpty(uint8 InIndex);

	void SetSlotImage(UTexture2D* InTexture, uint8 InIndex);
	
	void RemoveSlotImage(uint8 InIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetInventoryGrid(UUniformGridPanel* NewInventoryGrid) { InventoryGrid = NewInventoryGrid; };

	int32 GetCurrentSlotIndex() override final;

protected:
	virtual void OnSlotAddedToFocusPath(USlotWidget* InSlotWidget) final;

private:
	int32 NumberOfSlots;

	UPROPERTY()
	UUniformGridPanel* InventoryGrid;

	int32 GetNumberOfSlots();

	void SetNumberOfSlots(int32 NewNumberOfSlots);

	UInventorySlot* GetSlot(uint8 InIndex);
};
