#include "Widgets/InventoryWidget.h"
#include "Widgets/ItemSlotWidget.h"
//#include "Framework/Application/NavigationConfig.h"
#include "Components/WrapBox.h"
#include "Collectables/ItemStruct.h"
#include "Components/Button.h"


void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	selectedColor = FLinearColor(0.7f, 0.f, 1.f, 1.f);
	unselectedColor = FLinearColor(1.f, 1.f, 1.f, 0.f);

	//	TSharedRef<FNavigationConfig> Navigation = FSlateApplication::Get().GetNavigationConfig();
	//	//TSharedRef<FNavigationConfig> Navigation = MakeShared<FNavigationConfig>();
	//	Navigation->bKeyNavigation = false;
	//	Navigation->bTabNavigation = false;
	//	Navigation->bAnalogNavigation = false;
	//
	//	//Navigation.Get().AnalogHorizontalKey = EKeys::Gamepad_RightX;
	//	//Navigation.Get().AnalogVerticalKey = EKeys::Gamepad_RightY;
	//
	//
	//	FSlateApplication::Get().SetNavigationConfig(Navigation);
}


void UInventoryWidget::LoadInventory(TArray<FItemStruct> Items, int32 selectedIdx)
{
	WrapBox->ClearChildren();

	if (ItemSlotWidgetClass != nullptr)
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			FItemStruct item = Items[i];
			UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(this, ItemSlotWidgetClass);
			ItemSlotWidget->Item = item;

			FLinearColor color = (selectedIdx == i) ? selectedColor : unselectedColor;
			ItemSlotWidget->Button->SetBackgroundColor(color);

			WrapBox->AddChildToWrapBox(ItemSlotWidget);
		}
	}
}