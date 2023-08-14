// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/InputSettings.h"
#include "InventoryComponent.h"
#include "SlotWidget.h"
#include "InventoryWidget.generated.h"

DECLARE_DELEGATE_OneParam(FKeyDown, const FKey&);
DECLARE_DELEGATE_OneParam(FSlotAddedToFocusPath, int32);
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

	FSlotAddedToFocusPath SlotAddedToFocusPath;

	FSlotRemovedFromFocusPath SlotRemovedFromFocusPath;

	UWidget* GetFirstWidgetToFocus();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetFirstWidgetToFocus(UWidget* WidgetToFocus);
	
	UWidget* GetWidgetToFocus();

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetWidgetToFocus(UWidget* WidgetToFocus);

	void SetCurrentSlot(USlotWidget* InSlot);
	
	USlotWidget* GetCurrentSlot();

	virtual int32 GetCurrentSlotIndex();

protected:
	USlotWidget* CurrentSlot;
	
	int32 CurrentSlotIndex;

	UWidget* WidgetToFocusInternal;
	
	UWidget* FirstWidgetToFocusInternal;

	virtual void OnSlotAddedToFocusPath(USlotWidget* InSlotWidget);

	virtual void OnSlotRemovedFromFocusPath(USlotWidget* InSlotWidget);

	virtual void OnSlotHovered(USlotWidget* InSlotWidget);

	virtual void OnSlotUnhovered(USlotWidget* InSlotWidget);
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};