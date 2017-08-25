#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"
#include "JsonObjectConverter.h"

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FTankzMapData TankzMapData = LoadJson();

	for(auto tank: TankzMapData.attacker) {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, FString::Printf(TEXT("Tank '%s' spawning"), *tank.name));
		FTransform SpawnTransform;

		SpawnTransform.SetTranslation(FVector(tank.position_x, tank.position_y, 10.831337f));
		FVector Axis{0,0,1};
		SpawnTransform.SetRotation(FQuat(Axis, tank.rotation*PI/180));
		GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);
	}
}

FTankzMapData ATankzGameModeBase::LoadJson()
{
	FTankzMapData TankzMapData;

	{
		FString JsonString;
		const FString fileName = FPaths::Combine(FPaths::GameConfigDir(), TEXT("map.json"));
		UE_LOG(LogTemp, Log, TEXT("fileName: %s"),*fileName);
		FFileHelper::LoadFileToString(JsonString, *fileName);

		FJsonObjectConverter::JsonObjectStringToUStruct<FTankzMapData>(JsonString, &TankzMapData, 0, 0);
	}

	{
		FString OutJsonString;
		FJsonObjectConverter::UStructToJsonObjectString<FTankzMapData>(TankzMapData,OutJsonString);
		UE_LOG(LogTemp, Log, TEXT("TankzMapData: %s"),*(OutJsonString));
	}

	return TankzMapData;
}
