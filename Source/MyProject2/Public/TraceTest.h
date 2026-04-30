// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraceTest.generated.h"

UCLASS()
class MYPROJECT2_API ATraceTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATraceTest();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void StartSingleTrace();
	void StartAsyncTrace();
	
	void OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);

	FTraceHandle AsyncTraceHandle;
};
