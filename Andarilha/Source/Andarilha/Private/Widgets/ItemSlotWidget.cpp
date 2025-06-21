#include "Widgets/ItemSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	ItemName->SetText(FText::FromString(Item.Name));
	ItemImage->SetBrushFromTexture(Item.Image, true);
}
