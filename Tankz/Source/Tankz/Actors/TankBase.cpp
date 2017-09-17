// No copyright notice because I don't like copyright

#include "TankBase.h"


// Sets default values
ATankBase::ATankBase(): hasActed{false}, isAlive{true}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BorderPath = CreateDefaultSubobject<USplineComponent>("BorderPath");
	RootComponent = BorderPath;
	//TODO: This values should be calculated from TankTypeData
	FVector splinePointPosition1{ -25.f,  14.f, 0.f };
	FVector splinePointPosition2{  25.f,  14.f, 0.f };
	FVector splinePointPosition3{  25.f, -14.f, 0.f };
	FVector splinePointPosition4{ -25.f, -14.f, 0.f };
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

// Called when the game starts or when spawned
void ATankBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATankBase::SetSelected_Implementation(bool isSelected)
{
	UE_LOG(LogTemp, Error, TEXT("SetSelected_Implementation called. The BP should have overrided this."));
}

void ATankBase::SetBaseColor_Implementation(FLinearColor newBaseColor)
{
	UE_LOG(LogTemp, Error, TEXT("SetBaseColor_Implementation. The BP should have overrided this."));
}
