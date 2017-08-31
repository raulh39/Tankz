// No copyright notice because I don't like copyright

#include "TankzPlayerController.h"
#include "Actors/TankBase.h"
#include "TankzGameModeBase.h"


void ATankzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ATankzGameModeBase *gameMode = Cast<ATankzGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!gameMode) {
		UE_LOG(LogTemp, Error, TEXT("GameMode is not ATankzGameModeBase class. Aborting."));
		return;
	}
	InputComponent->BindAction("CycleUp",   IE_Pressed, gameMode, &ATankzGameModeBase::SelectNextTank);
	InputComponent->BindAction("CycleDown", IE_Pressed, gameMode, &ATankzGameModeBase::SelectPrevTank);
	InputComponent->BindAction("Select", IE_Pressed, gameMode, &ATankzGameModeBase::MarkThatTheSelectedTankHasActed);
}
