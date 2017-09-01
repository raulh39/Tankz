#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "JsonObjectConverter.h"
#include "Engine/Blueprint.h"
#include "TankzGameState.h"
#include "TankzPlayerController.h"

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
	GameState->CurrentPhase = TankzPhase_Moving;

	FTankzMapData TankzMapData = LoadJson();

	for(auto tank: TankzMapData.attacker) {
		Spawn(tank, true, GameState);
	}
	for(auto tank: TankzMapData.defender) {
		Spawn(tank, false, GameState);
	}

	if(!RecalculateActingTanks()) {
		UE_LOG(LogTemp, Error, TEXT("No Tank can Act. Program error. Aborting."));
	}
	SelectedTank=0;
	ActingTanks[SelectedTank]->SetSelected(true);
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

bool ATankzGameModeBase::RecalculateActingTanks()
{
	auto GameState = GetGameState<ATankzGameState>();

	int32 attackersFirstInitiative, defendersFirstInitiative;
	bool attackersCanAct, defendersCanAct;
	std::tie(attackersFirstInitiative, attackersCanAct) = getFirstInitiative(GameState->Attackers);
	std::tie(defendersFirstInitiative, defendersCanAct) = getFirstInitiative(GameState->Defenders);
	//UE_LOG(LogTemp, Log, TEXT("Initiatives: %s/%d    %s/%d"), attackersCanAct?TEXT("true"):TEXT("false"), attackersCanAct, defendersCanAct?TEXT("true"):TEXT("false"), defendersFirstInitiative);
	if(!attackersCanAct && !defendersCanAct) {
		resetTanks();
		incrementStatus();
		RecalculateActingTanks();
		return true;
	}
	if(!defendersCanAct) {
		SetActingTanksToAllTanksWithInitiative(attackersFirstInitiative, GameState->Attackers);
		return false;
	}
	if(!attackersCanAct) {
		SetActingTanksToAllTanksWithInitiative(defendersFirstInitiative, GameState->Defenders);
		return false;
	}
	if( (GameState->CurrentPhase == TankzPhase_Attacking && attackersFirstInitiative >= defendersFirstInitiative) ||
		(GameState->CurrentPhase != TankzPhase_Attacking && attackersFirstInitiative < defendersFirstInitiative) ) {
		SetActingTanksToAllTanksWithInitiative(attackersFirstInitiative, GameState->Attackers);
		return false;
	}
	SetActingTanksToAllTanksWithInitiative(defendersFirstInitiative, GameState->Defenders);
	return false;
}

void ATankzGameModeBase::resetTanks()
{
	auto GameState = GetGameState<ATankzGameState>();

	for(auto tank: GameState->Attackers) {
		if(tank->isAlive)
			tank->hasActed = false;
	}
	for(auto tank: GameState->Defenders) {
		if(tank->isAlive)
			tank->hasActed = false;
	}
}

void ATankzGameModeBase::incrementStatus()
{
	auto GameState = GetGameState<ATankzGameState>();
	switch (GameState->CurrentPhase)
	{
	case TankzPhase_Moving:
		GameState->CurrentPhase = TankzPhase_Attacking;
		break;
	case TankzPhase_Attacking:
		GameState->CurrentPhase = TankzPhase_Command;
		break;
	case TankzPhase_Command:
		GameState->CurrentPhase = TankzPhase_Moving;
		break;
	}
}

std::tuple<int32,bool> ATankzGameModeBase::getFirstInitiative(TArray<ATankBase*> tanks) const
{
	auto GameState = GetGameState<ATankzGameState>();
	if(GameState->CurrentPhase == TankzPhase_Attacking) { //Greater initiative goes first
		int32 currentInitiative = -1;
		for(auto tank: tanks)
			if(!tank->hasActed && tank->CurrentState.initiative > currentInitiative)
				currentInitiative = tank->CurrentState.initiative;
		if(currentInitiative == -1)
			return std::make_tuple(0, false);
		return std::make_tuple(currentInitiative, true);
	}
	//In all other cases, greater initiative goes last
	int32 currentInitiative = 1000;
	for(auto tank: tanks)
		if(!tank->hasActed && (tank->CurrentState.initiative < currentInitiative))
			currentInitiative = tank->CurrentState.initiative;
	if(currentInitiative == 1000)
		return std::make_tuple(0, false);
	return std::make_tuple(currentInitiative, true);
}

void ATankzGameModeBase::SetActingTanksToAllTanksWithInitiative(int32 initiative, TArray<ATankBase*> tanks)
{
	ActingTanks.clear();
	for(auto tank: tanks) {
		if(!tank->hasActed && tank->CurrentState.initiative == initiative)
			ActingTanks.push_back(tank);
	}
}

void ATankzGameModeBase::SelectNextTank()
{
	if(ActingTanks.size()<2) return;
	
	ActingTanks[SelectedTank]->SetSelected(false);

	if(SelectedTank==ActingTanks.size()-1)
		SelectedTank=0;
	else
		SelectedTank++;

	ActingTanks[SelectedTank]->SetSelected(true);
}

void ATankzGameModeBase::SelectPrevTank()
{
	if(ActingTanks.size()<2) return;
	
	ActingTanks[SelectedTank]->SetSelected(false);
	
	if(SelectedTank==0)
		SelectedTank=ActingTanks.size()-1;
	else
		SelectedTank--;

	ActingTanks[SelectedTank]->SetSelected(true);
}

bool ATankzGameModeBase::MakeTheSelectedTankAct()
{
	return MarkThatTheSelectedTankHasActed();
}

bool ATankzGameModeBase::MarkThatTheSelectedTankHasActed()
{
	ActingTanks[SelectedTank]->hasActed = true;
	ActingTanks[SelectedTank]->SetSelected(false);
	if(ActingTanks.size()>1) {
		ActingTanks.erase(ActingTanks.begin()+SelectedTank);
		if(SelectedTank==ActingTanks.size())
			SelectedTank=0;
		ActingTanks[SelectedTank]->SetSelected(true);
		return false;
	}
	auto hasBeenAStateChange = RecalculateActingTanks();
	SelectedTank=0;
	ActingTanks[SelectedTank]->SetSelected(true);
	return hasBeenAStateChange;
}