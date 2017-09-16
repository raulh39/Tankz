// No copyright notice because I don't like copyright

#include "TankzPlayerController.h"
#include "Actors/TankBase.h"
#include "TankzGameModeBase.h"
#include "TankzGameState.h"
#include "InplayUserWidgetBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ATankzPlayerController::ATankzPlayerController(): floatMode(false)
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

void ATankzPlayerController::BeginPlay()
{
	pawn = Cast<ADefaultPawn>(GetPawn());
	if(pawn == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Controlled pawn not found"));
	}
}

void ATankzPlayerController::OnCycleUp()
{
	gameMode->process_event( EvCycle(true) );
}

void ATankzPlayerController::OnCycleDown()
{
	gameMode->process_event( EvCycle(false) );
}

void ATankzPlayerController::OnSelect()
{
	gameMode->process_event( EvSelect() );
}

void ATankzPlayerController::OnEsc()
{
	gameMode->process_event( EvEsc() );
}

void ATankzPlayerController::OnMoveForward(float value)
{
	if(value<.01f && value > -0.1f) return;
	if(floatMode) {
#if 0
		pawn->MoveForward(value);
#else
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		auto Direction = FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::X );
		Direction.Z = 0;
		pawn->AddMovementInput( Direction, value );
#endif
	} else {
		gameMode->process_event( EvMove(value, 0.f) );
	}
}

void ATankzPlayerController::OnMoveRight(float value)
{
	if(value<.01f && value > -0.1f) return;
	if(floatMode) {
#if 0
		pawn->MoveRight(value);			
#else
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		auto Direction = FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::Y );
		Direction.Z = 0;
		pawn->AddMovementInput( Direction, value );
#endif
	} else {
		gameMode->process_event( EvMove(0.f, value) );
	}
}

void ATankzPlayerController::OnMoveUp(float value)
{
	if(value<.01f && value > -0.1f) return;
	if(pawn->GetActorLocation().Z<200 && value < 0.f) return;
	if(pawn->GetActorLocation().Z>1000 && value > 0.f) return;
	pawn->MoveUp_World(value);
}

void ATankzPlayerController::OnPanX(float value)
{
	if(value<.01f && value > -0.1f) return;
	if(floatMode) {
			this->AddYawInput(value);
	} else {
		gameMode->process_event( EvPan(value, 0.f) );
	}
}

void ATankzPlayerController::OnPanY(float value)
{
	if(value<.01f && value > -0.1f) return;
	if(floatMode) {
			this->AddPitchInput(-value);
	} else {
		gameMode->process_event( EvPan(0.f, value) );
	}
}

void ATankzPlayerController::ActivateFloatMode()
{
	floatMode = true;
}

void ATankzPlayerController::DeactivateFloatMode()
{
	floatMode = false;
}

void ATankzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	gameMode = Cast<ATankzGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!gameMode) {
		UE_LOG(LogTemp, Error, TEXT("GameMode is not ATankzGameModeBase class. Aborting."));
		return;
	}
	InputComponent->BindAction("CycleUp",   IE_Pressed, this, &ATankzPlayerController::OnCycleUp);
	InputComponent->BindAction("CycleDown", IE_Pressed, this, &ATankzPlayerController::OnCycleDown);
	InputComponent->BindAction("Select",    IE_Pressed, this, &ATankzPlayerController::OnSelect);
	InputComponent->BindAction("Esc",   IE_Pressed, this, &ATankzPlayerController::OnEsc);
	InputComponent->BindAction("FloatMode", IE_Pressed, this, &ATankzPlayerController::ActivateFloatMode);
	InputComponent->BindAction("FloatMode", IE_Released, this, &ATankzPlayerController::DeactivateFloatMode);

	InputComponent->BindAxis("MoveForward", this, &ATankzPlayerController::OnMoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankzPlayerController::OnMoveRight);
	InputComponent->BindAxis("MoveUp", this, &ATankzPlayerController::OnMoveUp);
	InputComponent->BindAxis("PanX", this, &ATankzPlayerController::OnPanX);
	InputComponent->BindAxis("PanY", this, &ATankzPlayerController::OnPanY);
}
