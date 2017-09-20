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
		currentPlayerIsAttacker = true;
		return;
	}
	if (!attackersCanAct) {
		SetActingTanksToAllTanksWithInitiative(defendersFirstInitiative, GameState->Defenders);
		currentPlayerIsAttacker = false;
		return;
	}
	if ((GameState->CurrentPhase == TankzPhase_Attacking && attackersFirstInitiative >= defendersFirstInitiative) ||
		(GameState->CurrentPhase != TankzPhase_Attacking && attackersFirstInitiative < defendersFirstInitiative)) {
		SetActingTanksToAllTanksWithInitiative(attackersFirstInitiative, GameState->Attackers);
		currentPlayerIsAttacker = true;
		return;
	}
	SetActingTanksToAllTanksWithInitiative(defendersFirstInitiative, GameState->Defenders);
	currentPlayerIsAttacker = false;
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
// FSM Functions for arrow placement
//----------------------------------------------------------------------
void ATankzGameModeBase::SpawnArrow()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::SpawnArrow()"));

	//We need the position of the spline in the selected tank
	auto border = ActingTanks[SelectedTank]->BorderPath;
	float border_length = border->GetSplineLength();
	
	positionInSplineBorderOfArrowBase = 60.f;

	auto location = border->GetLocationAtDistanceAlongSpline(positionInSplineBorderOfArrowBase, ESplineCoordinateSpace::World);
	location.Z = 0.8; //Put arrow above ground

	FVector map_center{ 0, 0, location.Z };
	auto r = (map_center-location);
	r.Normalize();
	arrowRotation = r.Rotation();

	arrow = GetWorld()->SpawnActor<AArrow>(AArrow::StaticClass(), location, arrowRotation);

	//When the arrow is spawned, the tank hasn't move yet, so we have to
	//reset to 0 its number of moves:
	ActingTanks[SelectedTank]->movesDone = 0;
}

void ATankzGameModeBase::DeleteArrowAndPlaceMovToken()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::DeleteArrowAndPlaceMovToken()"));
	arrow->Destroy();
	arrow = nullptr;

	//TODO:  Place movement token!!!!!!!!!!!!!
}

void ATankzGameModeBase::PositionArrowBase()
{
	auto border = ActingTanks[SelectedTank]->BorderPath;
	auto newPosition = border->GetLocationAtDistanceAlongSpline(positionInSplineBorderOfArrowBase, ESplineCoordinateSpace::World);
	newPosition.Z = 0.8; //Put arrow above ground
	arrow->SetActorLocation(newPosition);
}

void ATankzGameModeBase::AdjustArrowBase(const EvMove&ev)
{
	//UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AdjustArrowBase(%.2f,%.2f)"), ev.valueX, ev.valueY);
	auto border = ActingTanks[SelectedTank]->BorderPath;
	float border_length = border->GetSplineLength();

	//We treat X and Y movement in the same way: incrementing/decrementing the position of the arrow base in the BorderPath
	positionInSplineBorderOfArrowBase += ev.valueX;
	positionInSplineBorderOfArrowBase += ev.valueY;

	//Adjust positionInSplineBorderOfArrowBase in case it goes out of the path:
	if(positionInSplineBorderOfArrowBase >= border_length) {
		positionInSplineBorderOfArrowBase -= border_length;
	} else if(positionInSplineBorderOfArrowBase<0) {
		positionInSplineBorderOfArrowBase += border_length;
	}
}

void ATankzGameModeBase::AdjustArrowHead(const EvPan&ev)
{
	//UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AdjustArrowHead(%.2f,%.2f)"), ev.valueX, ev.valueY);
	float z = arrowRotation.GetComponentForAxis(EAxis::Z);
	z += ev.valueX;
	arrowRotation.SetComponentForAxis(EAxis::Z, z);
	arrow->SetActorRotation(arrowRotation);
}

//----------------------------------------------------------------------
// FSM Functions for tank placement in arrow
//----------------------------------------------------------------------
void ATankzGameModeBase::CalculateInitialTankPositionAlongArrow(const EvSelect&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::CalculateInitialTankPositionAlongArrow()"));
	positionInArrowInfo = PositionInArrowInfo();
	
	ActingTanks[SelectedTank]->movesDone++;
}

void ATankzGameModeBase::PlaceTankOnArrowSide()
{
	//UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::PlaceTankOnArrowSide()"));
	FTransform oldTransform = ActingTanks[SelectedTank]->GetActorTransform();
	float x = arrow->GetActorTransform().GetLocation().X;
	float y = arrow->GetActorTransform().GetLocation().Y;
	float z = oldTransform.GetLocation().Z;
	FVector newLocation{x,y,z};
	FTransform newTransform{arrowRotation, newLocation, oldTransform.GetScale3D()};
	ActingTanks[SelectedTank]->SetActorTransform(newTransform);
	ActingTanks[SelectedTank]->AddActorLocalOffset(positionInArrowInfo.getArrowOffset());
	ActingTanks[SelectedTank]->AddActorLocalRotation(positionInArrowInfo.getRotation());
}

void ATankzGameModeBase::AdjustTankPosition(const EvMove&ev)
{
	//UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::AdjustTankPosition(%.2f,%.2f)"), ev.valueX, ev.valueY);
	if(ev.valueY < 0) positionInArrowInfo.ToSide(Side::Left);
	if(ev.valueY > 0) positionInArrowInfo.ToSide(Side::Right);
	
	positionInArrowInfo.ChangeDeltaX(ev.valueX);
}

bool ATankzGameModeBase::MoreMovesLeft()
{
	if(ActingTanks[SelectedTank]->movesDone >= 2) {
		UE_LOG(LogTemp, Log, TEXT("Returning false from MoreMovesLeft()"));
		return false;
	}
	positionInSplineBorderOfArrowBase = 60.f;
	UE_LOG(LogTemp, Log, TEXT("Returning true from MoreMovesLeft()"));
	return true;
}

void ATankzGameModeBase::TurnTank90Degrees(const EvCycle&ev)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::TurnTank90Degrees()"));
	positionInArrowInfo.ChangeRotation(ev.forward);
}

//----------------------------------------------------------------------
// FSM Functions for ending MOVING phase
//----------------------------------------------------------------------
bool ATankzGameModeBase::MoreTanksInGroup()
{
	if(ActingTanks.size() > 1) {
		UE_LOG(LogTemp, Log, TEXT("Returning true from MoreTanksInGroup()"));
		return true;
	}
	UE_LOG(LogTemp, Log, TEXT("Returning false from MoreTanksInGroup()"));
	return false;
}

void ATankzGameModeBase::MarkTankHasActed(const EvEsc&)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::MarkTankHasActed()"));
	ActingTanks[SelectedTank]->hasActed = true;
}

bool ATankzGameModeBase::MoreTanksToAct()
{
	for (auto tank : GameState->Attackers) {
		if (!tank->hasActed) {
			UE_LOG(LogTemp, Log, TEXT("Returning true from MoreTanksToAct()"));
			return true;
		}
	}
	for (auto tank : GameState->Defenders) {
		if (!tank->hasActed) {
			UE_LOG(LogTemp, Log, TEXT("Returning true from MoreTanksToAct()"));
			return true;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Returning false from MoreTanksToAct()"));
	return false;
}

void ATankzGameModeBase::SwitchPhase(const EvEndPhase&ev)
{
	if(terminating) return;
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::SwitchPhase()"));
	GameState->CurrentPhase = ev.newPhase;
	OnPhaseChange.Broadcast(ev.newPhase);
}

//----------------------------------------------------------------------
// FSM Functions for attack phase
//----------------------------------------------------------------------
void ATankzGameModeBase::SelectObjectivesGroup()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::SelectObjectivesGroup()"));
	ObjectiveTanks.clear();
	SelectedObjectiveTank = 0;
	if(currentPlayerIsAttacker) {
		for(auto a: GameState->Attackers) //TODO: check tank is visible
			ObjectiveTanks.push_back(a);
	} else {
		for(auto a: GameState->Defenders)
			ObjectiveTanks.push_back(a);
	}
}

void ATankzGameModeBase::IncSelectedObjective(const EvCycle&ev)
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::IncSelectedObjective()"));
}
void ATankzGameModeBase::HighlightSelectedObjective()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::HighlightSelectedObjective()"));
}
void ATankzGameModeBase::UnhighlightSelectedObjective()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::UnhighlightSelectedObjective()"));
}

//----------------------------------------------------------------------
// FSM Functions TBD
//----------------------------------------------------------------------

void ATankzGameModeBase::UnselectObjectivesGroup()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::UnselectObjectivesGroup()"));
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
void ATankzGameModeBase::HighlightSelectedAction()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::HighlightSelectedAction()"));
}
void ATankzGameModeBase::UnhighlightSelectedAction()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::UnhighlightSelectedAction()"));
}

bool ATankzGameModeBase::ASideHasWon()
{
	UE_LOG(LogTemp, Log, TEXT("ATankzGameModeBase::ASideHasWon()"));
	return false;
}
