// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDManager.generated.h"

class UPlayerHUDWidget;
class UDamageComponent;

UCLASS()
class MYPROJECT2_API AHUDManager : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;   // HUD도 BeginPlay가 있어요

	// 에디터에서 WBP를 꽂을 칸
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	// 생성된 위젯 인스턴스 보관
	UPROPERTY()
	UPlayerHUDWidget* PlayerHUDWidget;
	
	UFUNCTION()
	void HandleHealthDamaged(float NewHealth, float MaxHealth, float HealthChange);
};
