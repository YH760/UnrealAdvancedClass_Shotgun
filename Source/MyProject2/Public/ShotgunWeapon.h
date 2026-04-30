// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShotgunWeapon.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UDamageType;
class ACharacter;

UCLASS()
class MYPROJECT2_API AShotgunWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotgunWeapon();

	void Fire(ACharacter* InstigatorCharacter);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MuzzlePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shotgun")
	int32 PelletCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shotgun")
	float SpreadAngle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shotgun")
	float Range;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shotgun")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shotgun")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* MuzzleEffect;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* FireSound;

private:
	FVector GetFireStart(ACharacter* InstigatorCharacter) const;
	FVector GetFireForward(ACharacter* InstigatorCharacter) const;
	void FireSinglePellet(const FVector& Start, const FVector& Direction, ACharacter* InstigatorCharacter);
};
