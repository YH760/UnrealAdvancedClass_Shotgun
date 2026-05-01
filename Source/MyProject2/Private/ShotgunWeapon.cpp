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
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Root")));
	SetRootComponent(RootComp);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("WeaponMesh")));
	WeaponMesh->SetupAttachment(RootComp);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(FName(TEXT("MuzzlePoint")));
	MuzzlePoint->SetupAttachment(WeaponMesh);

	PelletCount = 8;
	SpreadAngle = 5.f;
	Range = 3000.f;
	Damage = 10.f;
	MuzzleEffect = nullptr;
	FireSound = nullptr;

	ReboundPitch = 20.f;
	ReboundRecoveryTime = 1.f;
	Elapsed = 0.f;
	bRecovering = false;
}

void AShotgunWeapon::Fire(ACharacter* InstigatorCharacter)
{
	//UE_LOG(LogTemp, Warning, TEXT("Shotgun Fire called"));
	
	if (InstigatorCharacter == nullptr || MuzzlePoint == nullptr || GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire 실행 조건이 유효하지 않습니다."));
		return;
	}

	const FVector StartLocation = GetFireStart(InstigatorCharacter);
	const FVector ForwardVector = GetFireForward(InstigatorCharacter);

	for (int32 i = 0; i < PelletCount; i++)
	{
		float RandomYaw = FMath::FRandRange(-SpreadAngle, SpreadAngle);
		float RandomPitch = FMath::FRandRange(-SpreadAngle, SpreadAngle);
		
		FRotator SpreadRotation = FRotator(RandomPitch, RandomYaw, 0.0f);
		
		FVector PelletDirection = SpreadRotation.RotateVector(ForwardVector);

		FireSinglePellet(
			StartLocation,
			PelletDirection,
			InstigatorCharacter
		);
	}

	if (MuzzleEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MuzzleEffect,
			StartLocation
		);
	}
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FireSound,
			StartLocation
		);
	}
}

// Called when the game starts or when spawned
void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalPitch = GetActorRotation().Pitch;

}

// Called every frame
void AShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bRecovering)
	{
		return;
	}

	Elapsed += DeltaTime;
	float Alpha = FMath::Clamp(Elapsed / ReboundRecoveryTime, 0.f, 1.f);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Pitch = FMath::Lerp(OriginalPitch + ReboundPitch, OriginalPitch, Alpha);
	SetActorRotation(CurrentRotation);

	if (Alpha >= 1.f) 
	{
		Elapsed = 0.f;
		bRecovering = false;
	}
}

FVector AShotgunWeapon::GetFireStart(ACharacter* InstigatorCharacter) const
{
	// TODO: 총구 위치 반환값 정의
	return MuzzlePoint->GetComponentLocation();
}

FVector AShotgunWeapon::GetFireForward(ACharacter* InstigatorCharacter) const
{
	// TODO: 발사 기준 방향 반환값 정의
	return MuzzlePoint->GetForwardVector();
}

void AShotgunWeapon::FireSinglePellet(const FVector& Start, const FVector& Direction, ACharacter* InstigatorCharacter)
{
	FHitResult HitResult;

	const FVector TraceStart = Start;
	const FVector TraceEnd = TraceStart + (Direction * Range);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	if (InstigatorCharacter)
	{
		QueryParams.AddIgnoredActor(InstigatorCharacter);
	}

	bool bHit = false;
	bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	bRecovering = true;

	const FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		TraceEnd,
		LineColor,
		false,   // 지속 여부
		1.0f,    // 보이는 시간
		0,
		1.5f     // 두께
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		AController* InstigatorController = nullptr;
		if (InstigatorCharacter)
		{
			InstigatorController = InstigatorCharacter->GetController();
		}

		if (HitActor != nullptr)
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				Damage,
				Direction,
				HitResult,
				InstigatorController,
				this,
				UDamageType::StaticClass()
			);
		}
	}

}