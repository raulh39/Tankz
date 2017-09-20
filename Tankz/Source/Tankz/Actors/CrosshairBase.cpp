// No copyright notice because I don't like copyright

#include "CrosshairBase.h"


// Sets default values
ACrosshairBase::ACrosshairBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACrosshairBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACrosshairBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrosshairBase::SetSelected_Implementation(bool isSelected)
{
	UE_LOG(LogTemp, Error, TEXT("ACrosshairBase::SetSelected_Implementation called. The BP should have overrided this."));
}
