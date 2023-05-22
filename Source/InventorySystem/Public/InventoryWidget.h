// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/InputSettings.h"
#include "InventoryComponent.h"
#include "SlotWidget.h"
#include "InventoryWidget.generated.h"

DECLARE_DELEGATE_OneParam(FKeyDown, const FKey&);
DECLARE_DELEGATE_ThreeParams(FSlotClicked, UInventoryWidget*, uint8, FSlotStruct);
DECLARE_DELEGATE_OneParam(FSlotAddedToFocusPath, FSlotStruct);
DECLARE_DELEGATE(FSlotRemovedFromFocusPath);

class UInventoryComponent;
class FOnSlotWidgetButtonClicked;

/** Widget class for the main Inventory System Widgets */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FKeyDown KeyDown;
	FSlotClicked SlotClicked;
	FSlotAddedToFocusPath SlotAddedToFocusPath;
	FSlotRemovedFromFocusPath SlotRemovedFromFocusPath;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetFirstWidgetToFocus(UWidget* WidgetToFocus);
	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetWidgetToFocus(UWidget* WidgetToFocus) { WidgetToFocusInternal = WidgetToFocus; };

	void SetCurrentSlot(USlotWidget* InSlot);

	UWidget* GetFirstWidgetToFocus() { return FirstWidgetToFocusInternal; };
	UWidget* GetWidgetToFocus() { return WidgetToFocusInternal; };
	
	USlotWidget* GetCurrentSlot();

protected:

	uint8 SlotIndex;

	UWidget* WidgetToFocusInternal;
	UWidget* FirstWidgetToFocusInternal;

	USlotWidget* CurrentSlot;

	virtual void OnSlotClicked(USlotWidget* InSlotWidget);
	virtual void OnSlotAddedToFocusPath(USlotWidget* InSlotWidget);
	virtual void OnSlotRemovedFromFocusPath(USlotWidget* InSlotWidget);
	virtual void OnSlotHovered(USlotWidget* InSlotWidget);
	virtual void OnSlotUnhovered(USlotWidget* InSlotWidget);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};