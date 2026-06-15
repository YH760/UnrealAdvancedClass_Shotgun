#include "PoolObjectSubsystem.h"
#include "PooledObject.h"

void UPoolObjectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPoolObjectSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	SpawnPool();
}

void UPoolObjectSubsystem::SpawnPool()
{
	Pools.Empty();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 PoolIndex = 0; PoolIndex < PooledObjectData.Num(); PoolIndex++)
	{
		FSingleObjectPool CurrentPool;
		SpawnParams.Name = FName(*PooledObjectData[PoolIndex].ActorName);
		SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

		for (int32 ObjectIndex = 0; ObjectIndex < PooledObjectData[PoolIndex].PoolSize; ObjectIndex++)
		{
			AActor* SpawnedActor = GetWorld()->SpawnActor(
				PooledObjectData[PoolIndex].ActorTemplate,
				&FVector::ZeroVector,
				&FRotator::ZeroRotator,
				SpawnParams
			);
			if (!SpawnedActor) { continue; }

			SpawnedActor->SetActorLabel(SpawnedActor->GetName());

			UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
			PoolComp->RegisterComponent();
			SpawnedActor->AddInstanceComponent(PoolComp);
			PoolComp->Init(this);

			CurrentPool.PooledObjects.Add(PoolComp);

			SpawnedActor->SetActorHiddenInGame(true);
			SpawnedActor->SetActorEnableCollision(false);
			SpawnedActor->SetActorTickEnabled(false);
		}
		Pools.Add(CurrentPool);
	}
}

void UPoolObjectSubsystem::Broadcast_PoolerCleanup()
{
	OnPoolerCleanup.Broadcast();
}

AActor* UPoolObjectSubsystem::GetPooledActor(FString Name)
{
	// 요청한 이름의 풀 인덱스 찾기
	int32 CurrentPoolIndex = -1;
	for (int32 i = 0; i < PooledObjectData.Num(); i++)
	{
		if (PooledObjectData[i].ActorName == Name)
		{
			CurrentPoolIndex = i;
			break;
		}
	}
	if (CurrentPoolIndex == -1) { return nullptr; }

	// 비활성 오브젝트 탐색
	TArray<TObjectPtr<UPooledObject>>& PooledObjects = Pools[CurrentPoolIndex].PooledObjects;
	for (int32 i = 0; i < PooledObjects.Num(); i++)
	{
		if (PooledObjects[i] == nullptr)
		{
			RegenItem(CurrentPoolIndex, i);
		}

		if (!PooledObjects[i]->bIsPoolActive)
		{
			return ActivatePooledObject(PooledObjects[i]);
		}
	}

	// bCanGrow가 켜져있으면 새로 생성
	if (!PooledObjectData[CurrentPoolIndex].bCanGrow) { return nullptr; }

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(*PooledObjectData[CurrentPoolIndex].ActorName);
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(
		PooledObjectData[CurrentPoolIndex].ActorTemplate,
		&FVector::ZeroVector,
		&FRotator::ZeroRotator,
		SpawnParams
	);
	if (!SpawnedActor) { return nullptr; }

	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
	PoolComp->RegisterComponent();
	SpawnedActor->AddInstanceComponent(PoolComp);
	PoolComp->Init(this);
	PooledObjects.Add(PoolComp);

	return ActivatePooledObject(PoolComp);
}

AActor* UPoolObjectSubsystem::ActivatePooledObject(UPooledObject* PoolComp)
{
	PoolComp->bIsPoolActive = true;
	OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);

	AActor* Actor = PoolComp->GetOwner();
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
	Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	return Actor;
}

void UPoolObjectSubsystem::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);

	PoolCompRef->bIsPoolActive = false;

	AActor* ReturningActor = PoolCompRef->GetOwner();
	ReturningActor->SetActorHiddenInGame(true);
	ReturningActor->SetActorEnableCollision(false);
	ReturningActor->SetActorTickEnabled(false);
}

void UPoolObjectSubsystem::RecycleActor(AActor* PooledActor)
{
	if (UPooledObject* PoolComp = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
	{
		RecyclePooledObject(PoolComp);
	}
}

void UPoolObjectSubsystem::RegenItem(int32 PoolIndex, int32 PositionIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(*PooledObjectData[PoolIndex].ActorName);
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = GetWorld()->SpawnActor(
		PooledObjectData[PoolIndex].ActorTemplate,
		&FVector::ZeroVector,
		&FRotator::ZeroRotator,
		SpawnParams
	);
	if (!SpawnedActor) { return; }

	SpawnedActor->SetActorLabel(SpawnedActor->GetName());

	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
	PoolComp->RegisterComponent();
	SpawnedActor->AddInstanceComponent(PoolComp);
	PoolComp->Init(this);

	Pools[PoolIndex].PooledObjects.Insert(PoolComp, PositionIndex);

	SpawnedActor->SetActorHiddenInGame(true);
	SpawnedActor->SetActorEnableCollision(false);
	SpawnedActor->SetActorTickEnabled(false);
}
