// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

class UTextBlock;

UCLASS()
class MYPROJECT2_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

	void InitDamageNumber(float Damage);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float ElapsedTime = 0.f;   // 생성 후 흐른 시간

	UPROPERTY(EditDefaultsOnly, Category = "Damage Number")
	float LifeTime = 1.2f;     // 몇 초 동안 보일지

	UPROPERTY(EditDefaultsOnly, Category = "Damage Number")
	float RiseSpeed = 60.f;    // 초당 위로 올라가는 픽셀
};
