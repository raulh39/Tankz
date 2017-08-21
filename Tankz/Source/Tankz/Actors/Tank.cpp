// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
ATank::ATank()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMeshComponent");

	
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/TankMesh.TankMesh'"));
	if (MeshAsset.Object != nullptr) {
		Mesh->SetStaticMesh(MeshAsset.Object);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Mesh Asset not found"));
	}
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

