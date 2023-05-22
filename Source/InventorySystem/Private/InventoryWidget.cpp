// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "InventoryWidget.h"
#include "InventorySystem.h"
#include "Components/Button.h"
#include "SlotStruct.h"

void UInventoryWidget::SetFirstWidgetToFocus(UWidget* WidgetToFocus)
{
	FirstWidgetToFocusInternal = WidgetToFocus;
	WidgetToFocusInternal = WidgetToFocus;
}

void UInventoryWidget::OnSlotClicked(USlotWidget* InSlotWidget)
{
	FSlotStruct SlotStruct = InSlotWidget->GetSlotStruct();
	InSlotWidget->Empty();
	SlotClicked.ExecuteIfBound(this, SlotIndex, SlotStruct);
}

void UInventoryWidget::OnSlotAddedToFocusPath(USlotWidget* InSlotWidget)
{
	SetCurrentSlot(InSlotWidget);
	SetWidgetToFocus(InSlotWidget->GetButton());	
	
	//FSlotStruct SlotStruct = InSlotWidget->GetSlotStruct();
	//SlotAddedToFocusPath.ExecuteIfBound(SlotStruct);
}

void UInventoryWidget::OnSlotRemovedFromFocusPath(USlotWidget* InSlotWidget)
{
	//InSlotWidget->HideShadow();
	SlotRemovedFromFocusPath.ExecuteIfBound();

	SetCurrentSlot(nullptr);
}

void UInventoryWidget::OnSlotHovered(USlotWidget* InSlotWidget)
{
	//SetCurrentSlot(InSlotWidget);
	InSlotWidget->GetButton()->SetKeyboardFocus();
}

void UInventoryWidget::OnSlotUnhovered(USlotWidget* InSlotWidget)
{
	SetCurrentSlot(nullptr);
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	KeyDown.ExecuteIfBound(InKeyEvent.GetKey());
	return FReply::Handled();
}

void UInventoryWidget::SetCurrentSlot(USlotWidget* InSlot)
{
	if (CurrentSlot)
	{
		CurrentSlot->HideShadow();
	}

	CurrentSlot = InSlot;
	if (CurrentSlot)
	{
		CurrentSlot->DisplayShadow();
	}
}

USlotWidget* UInventoryWidget::GetCurrentSlot()
{
	return CurrentSlot;
}