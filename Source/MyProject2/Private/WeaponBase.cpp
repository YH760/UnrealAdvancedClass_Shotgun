#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComp);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(WeaponMesh);

	Range = 3000.f;
	Damage = 10.f;
	ReboundPitch = 15.f;
	ReboundRecoveryTime = 0.5f;
	bIsRecovering = false;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalPitch = GetActorRotation().Pitch;
}

void AWeaponBase::OnFire(ACharacter* InstigatorCharacter)
{
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRecovering)
	{
		HandleReboundRecovery(DeltaTime);
	}
}

void AWeaponBase::Fire(ACharacter* InstigatorCharacter)
{
	if (!InstigatorCharacter) return;

	if (MuzzleEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, GetMuzzleLocation());
	if (FireSound) UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetMuzzleLocation());

	OnFire(InstigatorCharacter);
}

void AWeaponBase::ExecuteTraceAndDamage(const FVector& Start, const FVector& Direction, ACharacter* InstigatorCharacter)
{
	FHitResult Hit;
	FVector End = Start + (Direction * Range);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(InstigatorCharacter);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor, 
				Damage, 
				Direction, 
				Hit, 
				InstigatorCharacter->GetController(), 
				this, 
				nullptr
			);
		}
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 1.f);
}

void AWeaponBase::StartRebound()
{
	//OriginalPitch = GetActorRotation().Pitch;
	ElapsedTime = 0.f;
	bIsRecovering = true;
}

void AWeaponBase::HandleReboundRecovery(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / ReboundRecoveryTime, 0.f, 1.f);

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Pitch = FMath::Lerp(OriginalPitch + ReboundPitch, OriginalPitch, Alpha);
	SetActorRotation(CurrentRotation);

	if (Alpha >= 1.f)
	{
		bIsRecovering = false;
	}
}

FVector AWeaponBase::GetMuzzleLocation() const
{
	return MuzzlePoint->GetComponentLocation();
}

FVector AWeaponBase::GetMuzzleForward() const
{
	return MuzzlePoint->GetForwardVector();
}
