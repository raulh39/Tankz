// No copyright notice because I don't like copyright

#include "TankzPlayerController.h"
#include "Actors/TankBase.h"
#include "TankzGameModeBase.h"
#include "InplayUserWidgetBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ATankzPlayerController::ATankzPlayerController()
{
	auto InplayUserWidget = ConstructorHelpers::FObjectFinder<UUserWidget>(TEXT("WidgetBlueprint'/Game/InPlayWidgetBP.InPlayWidgetBP'"));
	if(InplayUserWidget.Object == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UUserWidget NOT found"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("UUserWidget found"));
	}
}

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

void ATankzPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}
