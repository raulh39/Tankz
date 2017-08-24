#include "TankzGameModeBase.h"
#include "Paths.h"
#include "Engine.h"
#include "Actors/Tank.h"

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
	UE_LOG(LogTemp, Log, TEXT("JsonString: %s"),*JsonString);

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject)) {
		UE_LOG(LogTemp, Error, TEXT("Error loading map.json"));
		return;
	}

	auto version = JsonObject->GetStringField("version");
	UE_LOG(LogTemp, Log, TEXT("version: %s"),*version);
}
