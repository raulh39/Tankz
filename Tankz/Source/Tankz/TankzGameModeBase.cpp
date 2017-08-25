#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"
#include "Data/TankData.h"
#include "JsonObjectConverter.h"

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LoadJson();

	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, TEXT("Tank Spawning"));
	FTransform SpawnTransform;

	SpawnTransform.SetTranslation(FVector(0.f, 0.f, 10.831337f));
	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);

	SpawnTransform.SetTranslation(FVector(80.f, 80.f, 10.831337f));
	GetWorld()->SpawnActor<ATank>(ATank::StaticClass(), SpawnTransform);
}

void ATankzGameModeBase::LoadJson()
{
	FString JsonString;
	const FString fileName = FPaths::Combine(FPaths::GameConfigDir(), TEXT("map.json"));
	UE_LOG(LogTemp, Log, TEXT("fileName: %s"),*fileName);
	FFileHelper::LoadFileToString(JsonString, *fileName);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject)) {
		UE_LOG(LogTemp, Error, TEXT("Error loading map.json:\n%s"), *JsonString);
		return;
	}

	auto version = JsonObject->GetStringField(TEXT("version"));
	UE_LOG(LogTemp, Log, TEXT("version: %s"),*version);

	auto attacker = JsonObject->GetArrayField(TEXT("attacker"));
	auto first = attacker[0].Get()->AsObject();

	FTankData TankData;
	FString JsonStr{TEXT("{\"name\": \"Panzer IV (1)\", \"mesh\": \"panzer\", \"initiative\": 5, \"attack\": 4, \"defence\": 1, \"damage_capacity\": 5, \"habilities\": [ \"Blitzkrieg\" ], \"position_x\": 0, \"position_y\": 0, \"rotation\": 0 }")};

	FJsonObjectConverter::JsonObjectStringToUStruct<FTankData>(JsonStr, &TankData, 0, 0);
	
	UE_LOG(LogTemp, Log, TEXT("TankData: %s"),*(TankData.ToString()));
}
