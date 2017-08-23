// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
ATank::ATank() : lastColor{ 0.f }, incrementing{true}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainTankBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainTankBodyMesh");
	RootComponent = MainTankBodyMesh;
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/TankMesh.TankMesh'"));
	if (MeshAsset.Object != nullptr) {
		MainTankBodyMesh->SetStaticMesh(MeshAsset.Object);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Mesh Asset not found"));
	}

	BorderPath = CreateDefaultSubobject<USplineComponent>("BorderPath");
	BorderPath->AttachToComponent(MainTankBodyMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	FVector splinePointPosition1{ -24.f, 16.f, 0.f };
	FVector splinePointPosition2{ 26.f,  16.f, 0.f };
	FVector splinePointPosition3{ 26.f,  -15.f, 0.f };
	FVector splinePointPosition4{ -24.f,  -15.f, 0.f };
	BorderPath->ClearSplinePoints(false);
	BorderPath->AddSplineLocalPoint(splinePointPosition1);
	BorderPath->SetSplinePointType(0, ESplinePointType::Linear, false);
	BorderPath->AddSplineLocalPoint(splinePointPosition2);
	BorderPath->SetSplinePointType(1, ESplinePointType::Linear, false);
	BorderPath->AddSplineLocalPoint(splinePointPosition3);
	BorderPath->SetSplinePointType(2, ESplinePointType::Linear, false);
	BorderPath->AddSplineLocalPoint(splinePointPosition4);
	BorderPath->SetSplinePointType(3, ESplinePointType::Linear, false);
	BorderPath->SetClosedLoop(true);

	auto MaterialInstance = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("MaterialInstanceConstant'/Game/Materials/TankMaterialInstance.TankMaterialInstance'"));
	if (MaterialInstance.Object != nullptr) {
		TankMaterialInstance = MaterialInstance.Object;
		MainTankBodyMesh->SetMaterial(0, TankMaterialInstance);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Material instance not found"));
	}

	auto MainTankBodyMaterialDynamicInstance = MainTankBodyMesh->CreateDynamicMaterialInstance(0);
	if (!MainTankBodyMaterialDynamicInstance) {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("dynamicMaterialInstance not created"));
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

	UMaterialInterface* MaterialInstance = MainTankBodyMesh->GetMaterial(0);
	UMaterialInstanceDynamic* MainTankBodyMaterialDynamicInstance = Cast<UMaterialInstanceDynamic>(MaterialInstance);
	if (!MainTankBodyMaterialDynamicInstance) return;

	if (incrementing) {
		lastColor += DeltaTime;
		if (lastColor > 1.f) incrementing = false;
	}
	else {
		lastColor -= DeltaTime;
		if (lastColor < 0.f) incrementing = true;
	}
	FLinearColor newColor{ lastColor, lastColor, lastColor };
	MainTankBodyMaterialDynamicInstance->SetVectorParameterValue(TEXT("EmissiveColor"), newColor);
}

