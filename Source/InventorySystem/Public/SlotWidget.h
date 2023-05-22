// Written by Berkay Tuna, March 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotStruct.h"
#include "SlotWidget.generated.h"

DECLARE_DELEGATE_OneParam(FClicked, USlotWidget*);
DECLARE_DELEGATE_OneParam(FAddedToFocusPath, USlotWidget*);
DECLARE_DELEGATE_OneParam(FRemovedFromFocusPath, USlotWidget*);
DECLARE_DELEGATE_OneParam(FHovered, USlotWidget*);
DECLARE_DELEGATE_OneParam(FUnhovered, USlotWidget*);

class UButton;
class UImage;
class UInventoryWidget;
class UInventoryComponent;
class UTexture2D;

UCLASS()
class INVENTORYSYSTEM_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FClicked Clicked;
	FAddedToFocusPath AddedToFocusPath;
	FRemovedFromFocusPath RemovedFromFocusPath;
	FHovered Hovered;
	FUnhovered Unhovered;

	UFUNCTION(BlueprintCallable, Category = "Inventory System")
	void SetButtonAndImages(UButton* NewButton, UImage* NewImage, UImage* NewShadowImage, UTexture2D* EmptySlotTexture);

	void DisplayShadow();
	void HideShadow();

	virtual void Empty();

	//UFUNCTION(BlueprintCallable, Category = "Inventory System")
	UButton* GetButton() { return Button; };
	FSlotStruct GetSlotStruct() { return SlotStruct; };

	void SetSlotStruct(FSlotStruct NewSlotStruct);
	void SetSlotIndex(int32 InSlotIndex) { SlotIndex = InSlotIndex; };

protected:

	int32 SlotIndex;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System")
	FSlotStruct SlotStruct;

	UPROPERTY()
	UButton* Button;
	UPROPERTY()
	UImage* Image;
	UPROPERTY()
	UImage* ShadowImage;
	UPROPERTY()
	UTexture2D* EmptySlotTexture;

	UFUNCTION()
	virtual void OnClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

private:
	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnhovered();
};