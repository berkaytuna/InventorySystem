// Written by Berkay Tuna, March 2021

#include "CharacterSheet.h"
#include "EquipmentSlot.h"
#include "GameFramework/InputSettings.h"
#include "InventoryComponent.h"
#include "InventoryWindow.h"
#include "InventorySlot.h"

void UCharacterSheet::SetSlots(TArray<UEquipmentSlot*> InSlots)
{
	for (uint8 i = 0; i < InSlots.Num(); i++) {
		Slots.Add(InSlots[i]);
		Slots[i]->AddedToFocusPath.BindUObject(this, &UCharacterSheet::OnSlotAddedToFocusPath);
		Slots[i]->RemovedFromFocusPath.BindUObject(this, &UCharacterSheet::OnSlotRemovedFromFocusPath);
	}
}