#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"
#include "JsonObjectConverter.h"
#include "Engine/Blueprint.h"

ATankzGameModeBase::ATankzGameModeBase() {
	{
		auto panzerMeshLocator = TEXT("StaticMesh'/Game/Meshes/TankMesh.TankMesh'");

		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(panzerMeshLocator);
		if (MeshAsset.Object == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("Mesh Asset not found: %s"), panzerMeshLocator);
		}
		TankMesh = MeshAsset.Object;
	}
	{
		auto shermanMeshLocator = TEXT("StaticMesh'/Game/Meshes/M4Sherman.M4Sherman'");
		auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(shermanMeshLocator);
		if (MeshAsset.Object == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("Mesh Asset not found: %s"), shermanMeshLocator);
		}
		ShermanMesh = MeshAsset.Object;
	}

	auto matLocator = TEXT("MaterialInstanceConstant'/Game/Materials/TankMaterialInstance.TankMaterialInstance'");
	auto MaterialInstance = ConstructorHelpers::FObjectFinder<UMaterialInterface>(matLocator);
	if (MaterialInstance.Object == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Material Asset not found: %s"), matLocator);
	}

	
}

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	{
		FVector translation{0.f, 0.f, 0.f};
		FVector Axis{0,0,1};
		FRotator rotation{FQuat(Axis, 45*PI/180)};

		ATankBase* myTest = GetWorld()->SpawnActor<ATankBase>(TankTypes[0].Blueprint, translation, rotation);
		myTest->SetSelected(true);
	}

	FTankzMapData TankzMapData = LoadJson();

	for(auto tank: TankzMapData.attacker) {
		Spawn(tank, true);
	}
	for(auto tank: TankzMapData.defender) {
		Spawn(tank, false);
	}


}

void ATankzGameModeBase::Spawn(FTankData tank, bool isAttacker) {
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, FString::Printf(TEXT("Tank '%s' spawning"), *tank.name));

		float zpos = 0.11259f;
		if(isAttacker) zpos = 10.831337f;
		FVector translation{float(tank.position_x), float(tank.position_y), zpos};
		FVector Axis{0,0,1};
		FRotator rotation{FQuat(Axis, tank.rotation*PI/180)};

		auto newTank = GetWorld()->SpawnActor<ATank>(translation, rotation);
		
		if(isAttacker) {
			newTank->SetTankMesh(TankMesh);
			newTank->SetBaseColor(FLinearColor{.5,.05,.05});
		} else {
			newTank->SetTankMesh(ShermanMesh);
			newTank->SetBaseColor(FLinearColor{.05,.05,.5});
		}
}

FTankzMapData ATankzGameModeBase::LoadJson()
{
	FTankzMapData TankzMapData;

	{
		FString JsonString;
		const FString fileName = FPaths::Combine(FPaths::GameConfigDir(), TEXT("map.json"));
		UE_LOG(LogTemp, Log, TEXT("fileName: %s"),*fileName);
		FFileHelper::LoadFileToString(JsonString, *fileName);

		FJsonObjectConverter::JsonObjectStringToUStruct<FTankzMapData>(JsonString, &TankzMapData, 0, 0);
	}

	return TankzMapData;
}
