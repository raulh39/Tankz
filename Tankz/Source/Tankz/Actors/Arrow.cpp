// No copyright notice because I don't like copyright

#include "Arrow.h"
#include <UObject/ConstructorHelpers.h>

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("ArrowMeshComponent");
	RootComponent = Mesh;
	auto MeshAsset =  ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/MeshesAndTextures/Tokens/Arrow.Arrow'"));
	if (MeshAsset.Object != nullptr) {
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();	
}
