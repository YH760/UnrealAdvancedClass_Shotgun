// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GameFramework/Actor.h"
#include "ShotgunWeapon.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UDamageType;
class ACharacter;

UCLASS()
class MYPROJECT2_API AShotgunWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotgunWeapon();
	
protected:
	virtual void OnFire(ACharacter* InstigatorCharacter) override;

	UPROPERTY(EditAnywhere, Category = "Shotgun")
	int32 PelletCount;
	UPROPERTY(EditAnywhere, Category = "Shotgun")
	float SpreadAngle;
};
