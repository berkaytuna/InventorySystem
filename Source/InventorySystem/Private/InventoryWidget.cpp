#include "InventoryWidget.h"

#include "Kismet/GameplayStatics.h"
#include "InventorySystem.h"
#include "Components/Button.h"
#include "SlotStruct.h"
	
UWidget* UInventoryWidget::GetFirstWidgetToFocus()
{
	return FirstWidgetToFocusInternal;
};

void UInventoryWidget::SetFirstWidgetToFocus(UWidget* WidgetToFocus)
{
	FirstWidgetToFocusInternal = WidgetToFocus;
	WidgetToFocusInternal = WidgetToFocus;
}

UWidget* UInventoryWidget::GetWidgetToFocus()
{
	return WidgetToFocusInternal;
};

void UInventoryWidget::SetWidgetToFocus(UWidget* WidgetToFocus)
{
	WidgetToFocusInternal = WidgetToFocus;
};

void UInventoryWidget::OnSlotAddedToFocusPath(USlotWidget* InSlotWidget)
{
	SetCurrentSlot(InSlotWidget);
	SetWidgetToFocus(InSlotWidget->GetButton());	

	SlotAddedToFocusPath.ExecuteIfBound(CurrentSlotIndex);
}

void UInventoryWidget::OnSlotRemovedFromFocusPath(USlotWidget* InSlotWidget)
{
	SlotRemovedFromFocusPath.ExecuteIfBound();
	SetCurrentSlot(nullptr);
}

void UInventoryWidget::OnSlotHovered(USlotWidget* InSlotWidget)
{
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