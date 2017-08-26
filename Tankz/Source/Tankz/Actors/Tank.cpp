#include "Tank.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine.h"

// Sets default values
ATank::ATank() : isSelected{true}
{
	PrimaryActorTick.bCanEverTick = false;

	CreateComponents();
	CreateMaterials();
}

void ATank::CreateComponents() {
	MainTankBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainTankBodyMesh");
	RootComponent = MainTankBodyMesh;

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
		MainTankBodyMesh->SetMaterial(0, MaterialInstance.Object);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Material instance not found: %s"), matLocator);
	}

	MainTankBodyMaterialDynamicInstance = MainTankBodyMesh->CreateDynamicMaterialInstance(0);
	if (!MainTankBodyMaterialDynamicInstance) {
		UE_LOG(LogTemp, Error, TEXT("MainTankBodyMaterialDynamicInstance not created"));
		return;
	}
}

void ATank::SetTankMesh(UStaticMesh* NewMesh) {
	if(MainTankBodyMesh && NewMesh)
		MainTankBodyMesh->SetStaticMesh(NewMesh);
}

void ATank::SetBaseColor(FLinearColor newBaseColor) {
	if (!MainTankBodyMaterialDynamicInstance) {
		UE_LOG(LogTemp, Error, TEXT("MainTankBodyMaterial is not instance dynamic!"));
		return;
	}
	MainTankBodyMaterialDynamicInstance->SetVectorParameterValue(TEXT("BaseColor"), newBaseColor);
}

void ATank::toggleSelected()
{
	isSelected = !isSelected;

	if (!MainTankBodyMaterialDynamicInstance) {
		UE_LOG(LogTemp, Error, TEXT("MainTankBodyMaterial is not instance dynamic!"));
		return;
	}
	float newColor{0};
	if(isSelected) newColor=0.05;
	FLinearColor newColorC{ newColor, newColor, newColor };
	MainTankBodyMaterialDynamicInstance->SetVectorParameterValue(TEXT("EmissiveColor"), newColorC);
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
