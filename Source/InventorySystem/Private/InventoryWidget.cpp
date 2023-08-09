#include "InventoryWidget.h"

#include "Kismet/GameplayStatics.h"
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
	SlotClicked.ExecuteIfBound(this, CurrentSlotIndex, SlotStruct);
}

void UInventoryWidget::OnSlotAddedToFocusPath(USlotWidget* InSlotWidget)
{
	SetCurrentSlot(InSlotWidget);
	SetWidgetToFocus(InSlotWidget->GetButton());	

	SlotAddedToFocusPath.ExecuteIfBound(CurrentSlotIndex);
	
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

int32 UInventoryWidget::GetCurrentSlotIndex()
{
	return CurrentSlotIndex;
}

USlotWidget* UInventoryWidget::GetCurrentSlot()
{
	return CurrentSlot;
}