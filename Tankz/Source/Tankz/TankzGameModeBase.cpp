#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"
#include "JsonObjectConverter.h"
#include "Engine/Blueprint.h"

ATankzGameModeBase::ATankzGameModeBase() {
}

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

		FVector translation{float(tank.position_x), float(tank.position_y), 0.f};
		FVector Axis{0,0,1};
		FRotator rotation{FQuat(Axis, tank.rotation*PI/180)};

		ATankBase* newTank = GetWorld()->SpawnActor<ATankBase>(TankTypes[0].Blueprint, translation, rotation);
		
		//if(isAttacker) {
		//	newTank->SetBaseColor(FLinearColor{.5,.05,.05});
		//} else {
		//	newTank->SetBaseColor(FLinearColor{.05,.05,.5});
		//}
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

	return TankzMapData;
}
