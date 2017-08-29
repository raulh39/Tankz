// No copyright notice because I don't like copyright

#include "TankzPlayerController.h"
#include "Actors/TankBase.h"

void ATankzPlayerController::ResetStatus()
{
	UE_LOG(LogTemp, Log, TEXT("-------------ATankzPlayerController::BeginPlay-------------"));
	auto world = GetWorld();
	if(!world) {
		UE_LOG(LogTemp, Error, TEXT("GetWorld() is null in ATankzPlayerController::BeginPlay!!"));
		return;
	}
	auto gs = world->GetGameState();
	if(!gs) {
		UE_LOG(LogTemp, Error, TEXT("GetWorld()->GetGameState() is null in ATankzPlayerController::BeginPlay!!"));
		return;
	}
	gameState = Cast<ATankzGameState>(gs);
	if(!gameState) {
		UE_LOG(LogTemp, Error, TEXT("GetWorld()->GetGameState() is not a ATankzGameState!!"));
		return;
	}

	NextGroupOfTanksToAct = gameState->GetNextGroupOfTanksToAct();

	for(auto tank: NextGroupOfTanksToAct) {
		UE_LOG(LogTemp, Log, TEXT("The tank '%s' will act"), *tank->InitialState.name);
	}
	CurrentlySelectedTank = 0;
	if(NextGroupOfTanksToAct.size()) {
		NextGroupOfTanksToAct[CurrentlySelectedTank]->SetSelected(true);
	}
}

void ATankzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("CycleUp",   IE_Pressed, this, &ATankzPlayerController::SelectNext);
	InputComponent->BindAction("CycleDown", IE_Pressed, this, &ATankzPlayerController::SelectPrev);

}

void ATankzPlayerController::SelectNext()
{
	if(NextGroupOfTanksToAct.size()<2) return;
	
	NextGroupOfTanksToAct[CurrentlySelectedTank]->SetSelected(false);

	if(CurrentlySelectedTank==NextGroupOfTanksToAct.size()-1)
		CurrentlySelectedTank=0;
	else
		CurrentlySelectedTank++;

	NextGroupOfTanksToAct[CurrentlySelectedTank]->SetSelected(true);
}

void ATankzPlayerController::SelectPrev()
{
	if(NextGroupOfTanksToAct.size()<2) return;
	
	NextGroupOfTanksToAct[CurrentlySelectedTank]->SetSelected(false);
	
	if(CurrentlySelectedTank==0)
		CurrentlySelectedTank=NextGroupOfTanksToAct.size()-1;
	else
		CurrentlySelectedTank--;

	NextGroupOfTanksToAct[CurrentlySelectedTank]->SetSelected(true);
}
