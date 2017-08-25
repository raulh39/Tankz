#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"
#include "Data/TankzMapData.h"
#include "JsonObjectConverter.h"

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LoadJson();

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Tank Spawning"));
	FTransform SpawnTransform;

	SpawnTransform.SetTranslation(FVector(0.f, 0.f, 10.831337f));
	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);

	SpawnTransform.SetTranslation(FVector(80.f, 80.f, 10.831337f));
	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);
}

void ATankzGameModeBase::LoadJson()
{
	FString JsonString;
	const FString fileName = FPaths::Combine(FPaths::GameConfigDir(), TEXT("map.json"));
	UE_LOG(LogTemp, Log, TEXT("fileName: %s"),*fileName);
	FFileHelper::LoadFileToString(JsonString, *fileName);

	FTankzMapData TankzMapData;

	FJsonObjectConverter::JsonObjectStringToUStruct<FTankzMapData>(JsonString, &TankzMapData, 0, 0);
	
	UE_LOG(LogTemp, Log, TEXT("TankzMapData: %s"),*(TankzMapData.ToString()));
}
