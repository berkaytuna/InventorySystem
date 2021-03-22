// Written by Berkay Tuna, March 2021

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "InventoryInterface.h"
#include "CharacterSheet.generated.h"

class UEquipmentSlot;

UCLASS()
class INVENTORYSYSTEM_API UCharacterSheet : public UInventoryWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetSlots(TArray<UEquipmentSlot*> Slots);

	TArray<UEquipmentSlot*> GetSlots() { return Slots; };

private:

	UPROPERTY()
	TArray<UEquipmentSlot*> Slots;
};
