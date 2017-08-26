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
		Spawn(tank, true);
	}
	for(auto tank: TankzMapData.defender) {
		Spawn(tank, false);
	}
}

void ATankzGameModeBase::Spawn(FTankData tank, bool isAttacker) {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, FString::Printf(TEXT("Tank '%s' spawning"), *tank.name));
		FTransform SpawnTransform;

		SpawnTransform.SetTranslation(FVector(tank.position_x, tank.position_y, 10.831337f));
		FVector Axis{0,0,1};
		SpawnTransform.SetRotation(FQuat(Axis, tank.rotation*PI/180));
		auto newTank = GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);
		if(isAttacker)
			newTank->SetBaseColor(FLinearColor{.5,.05,.05});
		else
			newTank->SetBaseColor(FLinearColor{.05,.05,.5});
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
