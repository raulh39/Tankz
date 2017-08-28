#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "JsonObjectConverter.h"
#include "Engine/Blueprint.h"
#include "TankzGameState.h"

ATankzGameModeBase::ATankzGameModeBase() {
}

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	auto GameState = GetGameState<ATankzGameState>();
	if(!GameState) {
		UE_LOG(LogTemp, Error, TEXT("GameState is not ATankzGameState class. Aborting."));
		return;
	}

	FTankzMapData TankzMapData = LoadJson();

	for(auto tank: TankzMapData.attacker) {
		Spawn(tank, true, GameState);
	}
	for(auto tank: TankzMapData.defender) {
		Spawn(tank, false, GameState);
	}


}

void ATankzGameModeBase::Spawn(FTankData tank, bool isAttacker, ATankzGameState*state) {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, FString::Printf(TEXT("Tank '%s' spawning"), *tank.name));

		FVector translation{float(tank.position_x), float(tank.position_y), 0.f};
		FVector Axis{0,0,1};
		FRotator rotation{FQuat(Axis, tank.rotation*PI/180)};

		int index = FindIndexFor(tank.mesh);

		ATankBase* newTank = GetWorld()->SpawnActor<ATankBase>(TankTypes[index].Blueprint, translation, rotation);
		newTank->InitializeState(tank);

		if(isAttacker) {
			newTank->SetBaseColor(FLinearColor{.5,.05,.05});
			state->Attackers.Add(newTank);
		} else {
			newTank->SetBaseColor(FLinearColor{.05,.05,.5});
			state->Defenders.Add(newTank);
		}
}

FTankzMapData ATankzGameModeBase::LoadJson()
{
	FTankzMapData TankzMapData;

	{
		FString JsonString;
		const FString fileName = FPaths::Combine(FPaths::GameConfigDir(), TEXT("map.json"));
		UE_LOG(LogTemp, Log, TEXT("Loading map from file: %s"),*fileName);
		FFileHelper::LoadFileToString(JsonString, *fileName);

		FJsonObjectConverter::JsonObjectStringToUStruct<FTankzMapData>(JsonString, &TankzMapData, 0, 0);
	}

	return TankzMapData;
}

int ATankzGameModeBase::FindIndexFor(FString mesh)
{
	for(int i=0; i<TankTypes.Num();++i) {
		if(TankTypes[i].Name==mesh)
			return i;
	}
	UE_LOG(LogTemp, Error, TEXT("Mesh actor not found: %s"), *mesh);
	return 0;
}
