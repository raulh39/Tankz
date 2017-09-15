#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "JsonObjectConverter.h"
#include "Engine/Blueprint.h"
#include "TankzGameState.h"
#include "TankzPlayerController.h"
#include "Actors/Arrow.h"

ATankzGameModeBase::ATankzGameModeBase(): terminating(false)
{
}

ATankzGameModeBase::~ATankzGameModeBase()
{
	//When this destructor is being call, nor GameState
	//nor the pointers stored in ActingTanks can be used.
	//But the state machine will terminate (even if we don't call
	//terminate() and all the exit functions will be called.
	//So we set this variable to true and check the value
	//in the functions that use GameState or ActingTanks
	terminating=true;
	terminate();
}

//----------------------------------------------------------------------
// BeginPlay
//----------------------------------------------------------------------

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetGameState<ATankzGameState>();
	if (!GameState) {
		UE_LOG(LogTemp, Error, TEXT("GameState is not ATankzGameState class. Aborting."));
		return;
	}
	GameState->CurrentPhase = TankzPhase_Moving;

	FTankzMapData TankzMapData = LoadJson();

	for (auto tank : TankzMapData.attacker) {
		Spawn(tank, true, GameState);
	}
	for (auto tank : TankzMapData.defender) {
		Spawn(tank, false, GameState);
	}

	initiate(); //Initiates the Finite State Machine base class (GameModeStateMachine)
}

void ATankzGameModeBase::Spawn(FTankData tank, bool isAttacker, ATankzGameState*state) {
	UE_LOG(LogTemp, Log, TEXT("Tank '%s' spawning"), *tank.name);

	FVector translation{ float(tank.position_x), float(tank.position_y), 0.f };
	FVector Axis{ 0,0,1 };
	FRotator rotation{ FQuat(Axis, tank.rotation*PI / 180) };

	int index = FindIndexFor(tank.mesh);

	ATankBase* newTank = GetWorld()->SpawnActor<ATankBase>(TankTypes[index].Blueprint, translation, rotation);
	newTank->InitializeState(tank);

	if (isAttacker) {
		newTank->SetBaseColor(FLinearColor{ .5,.05,.05 });
		state->Attackers.Add(newTank);
	} else {
		newTank->SetBaseColor(FLinearColor{ .05,.05,.5 });
		state->Defenders.Add(newTank);
	}
}

FTankzMapData ATankzGameModeBase::LoadJson()
{
	FTankzMapData TankzMapData;

	{
		FString JsonString;
		const FString fileName = FPaths::Combine(FPaths::GameConfigDir(), TEXT("map.json"));
		UE_LOG(LogTemp, Log, TEXT("Loading map from file: %s"), *fileName);
		FFileHelper::LoadFileToString(JsonString, *fileName);

		FJsonObjectConverter::JsonObjectStringToUStruct<FTankzMapData>(JsonString, &TankzMapData, 0, 0);
	}

	return TankzMapData;
}

int ATankzGameModeBase::FindIndexFor(FString mesh)
{
	for (int i = 0; i < TankTypes.Num(); ++i) {
		if (TankTypes[i].Name == mesh)
			return i;
	}
	UE_LOG(LogTemp, Error, TEXT("Mesh actor not found: %s"), *mesh);
	return 0;
}

//----------------------------------------------------------------------
// FSM Functions for "acting tanks"
//----------------------------------------------------------------------

void ATankzGameModeBase::SetTanksToNotActed()
{
	if(terminating) return;
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::SetTanksToNotActed()"));

	for (auto tank : GameState->Attackers) {
		if (tank->isAlive)
			tank->hasActed = false;
	}
	for (auto tank : GameState->Defenders) {
		if (tank->isAlive)
			tank->hasActed = false;
	}
}

void ATankzGameModeBase::CalculateNextGroup()
{
	if(terminating) return;
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::CalculateNextGroup()"));

	SelectedTank=0;

	int32 attackersFirstInitiative, defendersFirstInitiative;
	bool attackersCanAct, defendersCanAct;
	std::tie(attackersFirstInitiative, attackersCanAct) = getFirstInitiative(GameState->Attackers);
	std::tie(defendersFirstInitiative, defendersCanAct) = getFirstInitiative(GameState->Defenders);

	assert(attackersCanAct || defendersCanAct);

	if (!defendersCanAct) {
		SetActingTanksToAllTanksWithInitiative(attackersFirstInitiative, GameState->Attackers);
		return;
	}
	if (!attackersCanAct) {
		SetActingTanksToAllTanksWithInitiative(defendersFirstInitiative, GameState->Defenders);
		return;
	}
	if ((GameState->CurrentPhase == TankzPhase_Attacking && attackersFirstInitiative >= defendersFirstInitiative) ||
		(GameState->CurrentPhase != TankzPhase_Attacking && attackersFirstInitiative < defendersFirstInitiative)) {
		SetActingTanksToAllTanksWithInitiative(attackersFirstInitiative, GameState->Attackers);
		return;
	}
	SetActingTanksToAllTanksWithInitiative(defendersFirstInitiative, GameState->Defenders);
}

std::tuple<int32, bool> ATankzGameModeBase::getFirstInitiative(TArray<ATankBase*> tanks) const
{
	if (GameState->CurrentPhase == TankzPhase_Attacking) { //Greater initiative goes first
		int32 currentInitiative = -1;
		for (auto tank : tanks)
			if (!tank->hasActed && tank->CurrentState.initiative > currentInitiative)
				currentInitiative = tank->CurrentState.initiative;
		if (currentInitiative == -1)
			return std::make_tuple(0, false);
		return std::make_tuple(currentInitiative, true);
	}
	//In all other cases, greater initiative goes last
	int32 currentInitiative = 1000;
	for (auto tank : tanks)
		if (!tank->hasActed && (tank->CurrentState.initiative < currentInitiative))
			currentInitiative = tank->CurrentState.initiative;
	if (currentInitiative == 1000)
		return std::make_tuple(0, false);
	return std::make_tuple(currentInitiative, true);
}

void ATankzGameModeBase::SetActingTanksToAllTanksWithInitiative(int32 initiative, TArray<ATankBase*> tanks)
{
	ActingTanks.clear();
	for (auto tank : tanks) {
		if (!tank->hasActed && tank->CurrentState.initiative == initiative)
			ActingTanks.push_back(tank);
	}
}

//----------------------------------------------------------------------
// FSM Functions for Highlighting
//----------------------------------------------------------------------
void ATankzGameModeBase::HighlightSelectedTank()
{
	if(terminating) return;
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::HighlightSelectedTank()"));
	ActingTanks[SelectedTank]->SetSelected(true);
}

void ATankzGameModeBase::UnhighlightSelectedTank()
{
	if(terminating) return;
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::UnhighlightSelectedTank()"));
	ActingTanks[SelectedTank]->SetSelected(false);
}

void ATankzGameModeBase::IncSelected(const EvCycle&ev)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::IncSelected()"));
	if(ev.forward) {
		++SelectedTank;
		if(SelectedTank>=ActingTanks.size()) {
			SelectedTank = 0;
		}
	} else {
		if(SelectedTank==0) {
			SelectedTank = ActingTanks.size()-1;
		} else {
				--SelectedTank;
		}
	}
}

//----------------------------------------------------------------------
// FSM Functions for arrow placement and movement
//----------------------------------------------------------------------
void ATankzGameModeBase::SpawnArrow()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::SpawnArrow()"));
	//We need the position of the spline in the selected tank

	FVector translation{ 0.f, 0.f, 0.f };
	FVector Axis{ 0,0,1 };
	FRotator rotation{ FQuat(Axis, 0*PI / 180) };

	arrow = GetWorld()->SpawnActor<AArrow>(AArrow::StaticClass(), translation, rotation);
}






void ATankzGameModeBase::SwitchPhase(const EvEndPhase&ev)
{
	if(terminating) return;
	GameState->CurrentPhase = ev.newPhase;
}


void ATankzGameModeBase::AdjustArrowBase(const EvMove&ev)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AdjustArrowBase(%.2f,%.2f)"), ev.valueX, ev.valueY);
}
void ATankzGameModeBase::AdjustArrowHead(const EvPan&ev)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AdjustArrowHead(%.2f,%.2f)"), ev.valueX, ev.valueY);
}
void ATankzGameModeBase::AdjustTankPosition(const EvMove&ev)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AdjustTankPosition(%.2f,%.2f)"), ev.valueX, ev.valueY);
}
void ATankzGameModeBase::AssignDamageAndMarkTankHasActed(const EvSelect&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AssignDamageAndMarkTankHasActed()"));
}
void ATankzGameModeBase::CalculateTankCommandActions(const EvSelect&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::CalculateTankCommandActions()"));
}
void ATankzGameModeBase::ExecuteSelectedActionAndMarkTankHasActed(const EvSelect&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::ExecuteSelectedActionAndMarkTankHasActed()"));
}
void ATankzGameModeBase::IncSelectedAction(const EvCycle&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::IncSelectedAction()"));
}
void ATankzGameModeBase::IncSelectedObjective(const EvCycle&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::IncSelectedObjective()"));
}
void ATankzGameModeBase::MarkTankHasActed(const EvEsc&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::MarkTankHasActed()"));
}
void ATankzGameModeBase::SelectObjectivesGroup(const EvSelect&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::SelectObjectivesGroup()"));
}
void ATankzGameModeBase::HighlightSelectedAction()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::HighlightSelectedAction()"));
}
void ATankzGameModeBase::HighlightSelectedObjective()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::HighlightSelectedObjective()"));
}
void ATankzGameModeBase::PlaceTankOnArrowSide()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::PlaceTankOnArrowSide()"));
}
void ATankzGameModeBase::PositionArrowBase()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::PositionArrowBase()"));
}
void ATankzGameModeBase::DeleteArrowAndPlaceMovToken()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::DeleteArrowAndPlaceMovToken()"));
}
void ATankzGameModeBase::UnhighlightSelectedAction()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::UnhighlightSelectedAction()"));
}
void ATankzGameModeBase::UnhighlightSelectedObjective()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::UnhighlightSelectedObjective()"));
}

bool ATankzGameModeBase::ASideHasWon()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::ASideHasWon()"));
	return false;
}
bool ATankzGameModeBase::MoreMovesLeft()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::MoreMovesLeft()"));
	return false;
}
bool ATankzGameModeBase::MoreTanksInGroup()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::MoreTanksInGroup()"));
	return false;
}
bool ATankzGameModeBase::MoreTanksToAct()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::MoreTanksToAct()"));
	return false;
}
