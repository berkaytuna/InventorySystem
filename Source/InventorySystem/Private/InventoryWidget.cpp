// Fill out your copyright notice in the Description page of Project Settings.

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
	SetWidgetToFocus(InSlotWidget->GetButton());
	InSlotWidget->DisplayShadow();

	FSlotStruct SlotStruct = InSlotWidget->GetSlotStruct();
	SlotAddedToFocusPath.ExecuteIfBound(SlotStruct);
}

void UInventoryWidget::OnSlotRemovedFromFocusPath(USlotWidget* InSlotWidget)
{
	InSlotWidget->HideShadow();
	SlotRemovedFromFocusPath.ExecuteIfBound();
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	KeyDown.ExecuteIfBound(InKeyEvent);
	return FReply::Handled();
}