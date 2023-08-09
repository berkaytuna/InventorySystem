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
		InventorySlot->Clicked.BindUObject(this, &UInventoryWindow::OnSlotClicked);
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

void UInventoryWindow::AddItemToInventory(int32 InSlotIndex, FSlotStruct InItemToAdd)
{
	UInventorySlot* InventorySlot = Cast<UInventorySlot>(InventoryGrid->GetChildAt(InSlotIndex));
	InventorySlot->SetSlotStruct(InItemToAdd);
}

void UInventoryWindow::RemoveItemFromInventory(int32 Index)
{
	UInventorySlot* InventorySlot = Cast<UInventorySlot>(InventoryGrid->GetChildAt(Index));
	InventorySlot->Empty();
}

void UInventoryWindow::EmptyInventory()
{
	for (int32 i = 0; i < NumberOfSlots; i++)
	{
		UInventorySlot* InventorySlot = Cast<UInventorySlot>(InventoryGrid->GetChildAt(i));
		InventorySlot->Empty();
	}
}

void UInventoryWindow::SetInventory(TArray<FSlotStruct> InInventory)
{
	for (int32 i = 0; i < InInventory.Num(); i++)
	{
		if (InInventory[i].Quantity > 0)
		{
			UInventorySlot* InventorySlot = Cast<UInventorySlot>(InventoryGrid->GetChildAt(i));
			InventorySlot->SetSlotStruct(InInventory[i]);
		}
	}
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

void UInventoryWindow::OnSlotAddedToFocusPath(USlotWidget* InSlotWidget)
{
	CurrentSlotIndex = InventoryGrid->GetChildIndex(InSlotWidget);
	Super::OnSlotAddedToFocusPath(InSlotWidget);
}

void UInventoryWindow::OnSlotClicked(USlotWidget* InSlotWidget)
{
	//CurrentSlotIndex = InventoryGrid->GetChildIndex(InSlotWidget);
	Super::OnSlotClicked(InSlotWidget);
}