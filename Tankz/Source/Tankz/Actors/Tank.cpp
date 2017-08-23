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

	CreateComponents();
	CreateMaterials();
}

void ATank::CreateComponents() {
	MainTankBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainTankBodyMesh");
	RootComponent = MainTankBodyMesh;
	auto meshLocator = TEXT("StaticMesh'/Game/Meshes/TankMesh.TankMesh'");
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(meshLocator);
	if (MeshAsset.Object != nullptr) {
		MainTankBodyMesh->SetStaticMesh(MeshAsset.Object);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Mesh Asset not found: %s"), meshLocator);
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
}

void ATank::CreateMaterials() {
	auto matLocator = TEXT("MaterialInstanceConstant'/Game/Materials/TankMaterialInstance.TankMaterialInstance'");
	auto MaterialInstance = ConstructorHelpers::FObjectFinder<UMaterialInterface>(matLocator);
	if (MaterialInstance.Object != nullptr) {
		TankMaterialInstance = MaterialInstance.Object;
		MainTankBodyMesh->SetMaterial(0, TankMaterialInstance);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Material instance not found: %s"), matLocator);
	}

	auto MainTankBodyMaterialDynamicInstance = MainTankBodyMesh->CreateDynamicMaterialInstance(0);
	if (!MainTankBodyMaterialDynamicInstance) {
		UE_LOG(LogTemp, Error, TEXT("MainTankBodyMaterialDynamicInstance not created"));
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
