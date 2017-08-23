#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Tank Spawning"));
	FTransform SpawnTransform;

	SpawnTransform.SetTranslation(FVector(0.f, 0.f, 10.831337f));
	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);

	SpawnTransform.SetTranslation(FVector(80.f, 80.f, 10.831337f));
	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);
}
