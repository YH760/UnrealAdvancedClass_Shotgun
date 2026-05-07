// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AShotgunWeapon::AShotgunWeapon()
{
	PelletCount = 8;
	SpreadAngle = 5.0f;
    
	// 부모 변수 기본값 커스터마이징
	Damage = 10.0f;
	Range = 2500.0f;
}

void AShotgunWeapon::OnFire(ACharacter* InstigatorCharacter)
{
	// 발사 위치와 방향 정보 가져오기 (부모의 도구 사용)
	FVector StartLocation = GetMuzzleLocation();
	FVector ForwardVector = GetMuzzleForward();

	for (int32 i = 0; i < PelletCount; i++)
	{
		// 1. 무작위 퍼짐 계산
		float RandomPitch = FMath::FRandRange(-SpreadAngle, SpreadAngle);
		float RandomYaw = FMath::FRandRange(-SpreadAngle, SpreadAngle);
		FRotator SpreadRotation = FRotator(RandomPitch, RandomYaw, 0.0f);
        
		// 2. 최종 탄환 방향 계산
		FVector PelletDirection = SpreadRotation.RotateVector(ForwardVector);

		// 3. 부모가 제공하는 사격 도구 실행 (트레이스 + 데미지)
		ExecuteTraceAndDamage(StartLocation, PelletDirection, InstigatorCharacter);
	}

	// 4. 발사 후 반동 시작 (부모의 기능)
	StartRebound();
}