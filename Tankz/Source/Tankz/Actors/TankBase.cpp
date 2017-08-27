// No copyright notice because I don't like copyright

#include "TankBase.h"


// Sets default values
ATankBase::ATankBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATankBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATankBase::SetSelected_Implementation(bool isSelected)
{
	UE_LOG(LogTemp, Error, TEXT("SetSelected_Implementation"));
}
