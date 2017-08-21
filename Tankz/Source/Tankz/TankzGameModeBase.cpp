// Fill out your copyright notice in the Description page of Project Settings.

#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Tank Spawning"));
	FTransform SpawnTransform;

	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);
}
