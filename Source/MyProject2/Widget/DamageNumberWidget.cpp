// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageNumberWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

void UDamageNumberWidget::InitDamageNumber(float Damage)
{
	ElapsedTime = 0.f;

	if (DamageText)
	{
		DamageText->SetText(FText::FromString(
			FString::Printf(TEXT("-%.0f"), Damage)
		));
	}
}

void UDamageNumberWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ElapsedTime += InDeltaTime;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		FVector2D Pos = CanvasSlot->GetPosition();
		Pos.Y -= RiseSpeed * InDeltaTime;
		CanvasSlot->SetPosition(Pos);
	}

	float Alpha = 1.f - (ElapsedTime / LifeTime);
	SetRenderOpacity(FMath::Clamp(Alpha, 0.f, 1.f));

	if (ElapsedTime >= LifeTime)
	{
		RemoveFromParent();
	}
}