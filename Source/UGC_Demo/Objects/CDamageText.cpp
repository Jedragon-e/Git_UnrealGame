#include "CDamageText.h"
#include "Global.h"

#include "Components/WidgetComponent.h"
#include "Widget/CWidget_DamagetText.h"

ACDamageText::ACDamageText()
{
	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &WidgetText, "WidgetText", Scene);
}

void ACDamageText::BeginPlay()
{
	UKismetSystemLibrary::K2_SetTimer(this, "TimeToDestory", 0.5f, false);
	Super::BeginPlay();
	WidgetText->InitWidget();
}

void ACDamageText::SetTextValue(float value)
{
	//Cast<UCWidget_DamagetText>(WidgetText->GetUserWidgetObject());
	Value = value;
	
}

void ACDamageText::TimeToDestory()
{
	Destroy();
}
