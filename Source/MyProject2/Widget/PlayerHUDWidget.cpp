// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::OnHealthChanged(float NewHealth, float MaxHealth, float HealthChange)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(NewHealth / MaxHealth);  // 0.0~1.0 사이 값으로 바 채우기
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f / %.0f"), NewHealth, MaxHealth)
		));
	}
}