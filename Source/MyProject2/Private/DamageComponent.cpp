// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"

UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.f;
}

void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UDamageComponent::DamageTake);
}

void UDamageComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer)
{
	float FinalDamage = FMath::Min(Damage, CurrentHealth);
	CurrentHealth -= FinalDamage;
	
	OnHealthDamaged.Broadcast(CurrentHealth, MaxHealth, FinalDamage);
	if (CurrentHealth == 0.f)
	{
		OnHealthDead.Broadcast(Instigator);
	}
}

void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDamageComponent::InitHealth()
{
	CurrentHealth = MaxHealth;                          // 값 확실히 세팅
	OnHealthDamaged.Broadcast(CurrentHealth, MaxHealth, 0.f);
}
