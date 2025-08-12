#include "Widgets/StaminaWidget.h"
#include "Components/ProgressBar.h"

void UStaminaWidget::UpdateStaminaBar(float value)
{
	StaminaBar->SetPercent(value);
}