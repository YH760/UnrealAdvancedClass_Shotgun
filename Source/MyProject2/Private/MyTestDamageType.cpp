// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestDamageType.h"

UMyTestDamageType::UMyTestDamageType()
{
	DamageImpulse = 5000.f;

	bScaleMomentumByMass = true;

	bCausedByWorld = true;
}
