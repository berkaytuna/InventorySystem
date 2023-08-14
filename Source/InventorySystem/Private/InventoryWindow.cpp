// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWindow.h"
#include "InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlot.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "CharacterSheet.h"
#include "EquipmentSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"

void UInventoryWindow::CreateSlots(int32 InNumberOfSlots, UClass* InventorySlotClass)
{
	SetNumberOfSlots(InNumberOfSlots);

	for (int32 i = 0; i < NumberOfSlots; i++)
	{
		UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		InventorySlot->AddedToFocusPath.BindUObject(this, &UInventoryWindow::OnSlotAddedToFocusPath);
		InventorySlot->RemovedFromFocusPath.BindUObject(this, &UInventoryWindow::OnSlotRemovedFromFocusPath);
		InventorySlot->Hovered.BindUObject(this, &UInventoryWindow::OnSlotHovered);
		InventorySlot->Unhovered.BindUObject(this, &UInventoryWindow::OnSlotUnhovered);

		if (i == 0)
		{
			SetFirstWidgetToFocus(InventorySlot->GetButton());
			SetCurrentSlot(InventorySlot);
		}

		int32 InRow = i / 4;
		int32 InCollumn = i % 4;

		if (InventoryGrid != nullptr)
			InventoryGrid->AddChildToUniformGrid(InventorySlot, InRow, InCollumn);
		else
			UE_LOG(LogTemp, Warning, TEXT("InventoryGrid is nullptr!"));
	}
}

int32 UInventoryWindow::GetCurrentSlotIndex()
{
	CurrentSlotIndex = InventoryGrid->GetChildIndex(CurrentSlot);
	return Super::GetCurrentSlotIndex();
}

UInventorySlot* UInventoryWindow::GetSlot(uint8 InIndex)
{
	return Cast<UInventorySlot>(InventoryGrid->GetChildAt(InIndex));
}

bool UInventoryWindow::IsSlotEmpty(uint8 InIndex)
{
	return GetSlot(InIndex)->IsEmpty();
}

void UInventoryWindow::SetSlotImage(UTexture2D* InTexture, uint8 InIndex)
{
	GetSlot(InIndex)->SetImage(InTexture);
}

void UInventoryWindow::RemoveSlotImage(uint8 InIndex)
{
	GetSlot(InIndex)->Empty();
}

void UInventoryWindow::OnSlotAddedToFocusPath(USlotWidget* InSlotWidget)
{
	CurrentSlotIndex = InventoryGrid->GetChildIndex(InSlotWidget);
	Super::OnSlotAddedToFocusPath(InSlotWidget);
}

int32 UInventoryWindow::GetNumberOfSlots()
{
	return NumberOfSlots;
};

void UInventoryWindow::SetNumberOfSlots(int32 NewNumberOfSlots)
{
	NumberOfSlots = NewNumberOfSlots;
};