#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PooledObjectData.h"
#include "PoolObjectSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanupSignature);

USTRUCT(BlueprintType)
struct FSingleObjectPool
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UPooledObject>> PooledObjects;
};

UCLASS(Blueprintable)
class MYPROJECT2_API UPoolObjectSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UPROPERTY()
	FPoolerCleanupSignature OnPoolerCleanup;

	UFUNCTION(BlueprintCallable)
	void Broadcast_PoolerCleanup();

	UFUNCTION(BlueprintCallable)
	AActor* GetPooledActor(FString Name);

	UFUNCTION(BlueprintCallable)
	void RecyclePooledObject(class UPooledObject* PoolCompRef);

	UFUNCTION(BlueprintCallable)
	void RecycleActor(AActor* PooledActor);

	// 블루프린트에서 레시피를 설정할 수 있도록 노출
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPooledObjectData> PooledObjectData;

	UFUNCTION(BlueprintCallable)
	void SpawnPool();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<FSingleObjectPool> Pools;
	void RegenItem(int32 PoolIndex, int32 PositionIndex);
	AActor* ActivatePooledObject(UPooledObject* PoolComp);
};
