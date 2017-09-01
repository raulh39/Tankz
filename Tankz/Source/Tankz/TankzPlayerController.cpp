// No copyright notice because I don't like copyright

#include "TankzPlayerController.h"
#include "Actors/TankBase.h"
#include "TankzGameModeBase.h"
#include "TankzGameState.h"
#include "InplayUserWidgetBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ATankzPlayerController::ATankzPlayerController()
{
	auto u = LoadObject<UBlueprint>(nullptr, TEXT("/Game/InPlayWidgetBP.InPlayWidgetBP"));
	if(u == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UUserWidget NOT found"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("UUserWidget found"));
		dialogueHUD = CreateWidget<UInplayUserWidgetBase>(GetWorld(), u->GeneratedClass);
		if(dialogueHUD) {
			UE_LOG(LogTemp, Log, TEXT("UInplayUserWidgetBase created"));
			dialogueHUD->AddToViewport();
			dialogueHUD->PhaseText = TEXT("Moving");
		} else {
			UE_LOG(LogTemp, Error, TEXT("UInplayUserWidgetBase NOT created"));
		}
	}
}

void ATankzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	gameMode = Cast<ATankzGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!gameMode) {
		UE_LOG(LogTemp, Error, TEXT("GameMode is not ATankzGameModeBase class. Aborting."));
		return;
	}
	InputComponent->BindAction("CycleUp",   IE_Pressed, gameMode, &ATankzGameModeBase::SelectNextTank);
	InputComponent->BindAction("CycleDown", IE_Pressed, gameMode, &ATankzGameModeBase::SelectPrevTank);
	InputComponent->BindAction("Select", IE_Pressed, this, &ATankzPlayerController::OnSelect);
}

void ATankzPlayerController::OnSelect() {
	auto hasBeenAPhaseChange = gameMode->MakeTheSelectedTankAct();
	if(!hasBeenAPhaseChange) return;
	auto GameState = GetWorld()->GetGameState<ATankzGameState>();
	if(TankzPhase_Moving == GameState->CurrentPhase) {
		dialogueHUD->PhaseText = TEXT("Moving");
	} else if(TankzPhase_Attacking == GameState->CurrentPhase) {
		dialogueHUD->PhaseText = TEXT("Attacking");
	} else {
		dialogueHUD->PhaseText = TEXT("Command");
	}
}
