#include "HUDManager.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.h"
#include "DamageComponent.h"
#include "GameFramework/Pawn.h"

void AHUDManager::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 생성 + 화면 표시
	if (PlayerHUDClass)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	if (APawn* PlayerPawn = GetOwningPawn())
	{
		if (UDamageComponent* DamageComp = PlayerPawn->FindComponentByClass<UDamageComponent>())
		{
			DamageComp->OnHealthDamaged.AddDynamic(this, &AHUDManager::HandleHealthDamaged);
			
			DamageComp->InitHealth();
		}
	}
}

void AHUDManager::HandleHealthDamaged(float NewHealth, float MaxHealth, float HealthChange)
{
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->OnHealthChanged(NewHealth, MaxHealth, HealthChange);
	}
}