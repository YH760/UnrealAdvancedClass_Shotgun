// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ACharacter;
class USkeletalMeshComponent;
class USceneComponent;
class UDamageType;

UCLASS()
class MYPROJECT2_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	
	virtual void Tick(float DeltaTime) override;
	
	// 발사 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire(ACharacter* InstigatorCharacter);

protected:
	virtual void BeginPlay() override;
	
	// 자식 클래슨에서 구현할 샌드박스 메서드
	virtual void OnFire(ACharacter* InstigatorCharacter);
	
	// 샌드박스 도구
	void ExecuteTraceAndDamage(const FVector& Start, const FVector& Direction, ACharacter* InstigatorCharacter);
	void StartRebound();
	
	FVector GetMuzzleLocation() const;
	FVector GetMuzzleForward() const;
	
	// 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MuzzlePoint;
	
	// 무기 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stat")
	TSubclassOf<UDamageType> DamageTypeClass;
	
	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Rebound")
	float ReboundPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Rebound")
	float ReboundRecoveryTime;
	
	// 효과
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Effect")
	UParticleSystem* MuzzleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Effect")
	USoundBase* FireSound;
	
private:
	// 반동 계산용 내부 변수
	float OriginalPitch;
	float ElapsedTime;
	bool bIsRecovering;

	void HandleReboundRecovery(float DeltaTime);
};
