// Fill out your copyright notice in the Description page of Project Settings.

#include "TankzGameModeBase.h"
#include "Paths.h"

void ATankzGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("LaunchDir(): %s"), *FPaths::LaunchDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineDir(): %s"), *FPaths::EngineDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineUserDir(): %s"), *FPaths::EngineUserDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineVersionAgnosticUserDir(): %s"), *FPaths::EngineVersionAgnosticUserDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineContentDir(): %s"), *FPaths::EngineContentDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineConfigDir(): %s"), *FPaths::EngineConfigDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineIntermediateDir(): %s"), *FPaths::EngineIntermediateDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineSavedDir(): %s"), *FPaths::EngineSavedDir());
	UE_LOG(LogTemp, Warning, TEXT("EnginePluginsDir(): %s"), *FPaths::EnginePluginsDir());
	UE_LOG(LogTemp, Warning, TEXT("RootDir(): %s"), *FPaths::RootDir());
	UE_LOG(LogTemp, Warning, TEXT("GameDir(): %s"), *FPaths::GameDir());
	UE_LOG(LogTemp, Warning, TEXT("GameUserDir(): %s"), *FPaths::GameUserDir());
	UE_LOG(LogTemp, Warning, TEXT("GameContentDir(): %s"), *FPaths::GameContentDir());
	UE_LOG(LogTemp, Warning, TEXT("GameConfigDir(): %s"), *FPaths::GameConfigDir());
	UE_LOG(LogTemp, Warning, TEXT("GameSavedDir(): %s"), *FPaths::GameSavedDir());
	UE_LOG(LogTemp, Warning, TEXT("GameIntermediateDir(): %s"), *FPaths::GameIntermediateDir());
	UE_LOG(LogTemp, Warning, TEXT("GamePluginsDir(): %s"), *FPaths::GamePluginsDir());
	UE_LOG(LogTemp, Warning, TEXT("GamePersistentDownloadDir(): %s"), *FPaths::GamePersistentDownloadDir());
	UE_LOG(LogTemp, Warning, TEXT("SourceConfigDir(): %s"), *FPaths::SourceConfigDir());
	UE_LOG(LogTemp, Warning, TEXT("GeneratedConfigDir(): %s"), *FPaths::GeneratedConfigDir());
	UE_LOG(LogTemp, Warning, TEXT("SandboxesDir(): %s"), *FPaths::SandboxesDir());
	UE_LOG(LogTemp, Warning, TEXT("ProfilingDir(): %s"), *FPaths::ProfilingDir());
	UE_LOG(LogTemp, Warning, TEXT("ScreenShotDir(): %s"), *FPaths::ScreenShotDir());
	UE_LOG(LogTemp, Warning, TEXT("BugItDir(): %s"), *FPaths::BugItDir());
	UE_LOG(LogTemp, Warning, TEXT("VideoCaptureDir(): %s"), *FPaths::VideoCaptureDir());
	UE_LOG(LogTemp, Warning, TEXT("GameLogDir(): %s"), *FPaths::GameLogDir());
	UE_LOG(LogTemp, Warning, TEXT("AutomationDir(): %s"), *FPaths::AutomationDir());
	UE_LOG(LogTemp, Warning, TEXT("AutomationTransientDir(): %s"), *FPaths::AutomationTransientDir());
	UE_LOG(LogTemp, Warning, TEXT("AutomationLogDir(): %s"), *FPaths::AutomationLogDir());
	UE_LOG(LogTemp, Warning, TEXT("CloudDir(): %s"), *FPaths::CloudDir());
	UE_LOG(LogTemp, Warning, TEXT("GameDevelopersDir(): %s"), *FPaths::GameDevelopersDir());
	UE_LOG(LogTemp, Warning, TEXT("GameUserDeveloperDir(): %s"), *FPaths::GameUserDeveloperDir());
	UE_LOG(LogTemp, Warning, TEXT("DiffDir(): %s"), *FPaths::DiffDir());
	UE_LOG(LogTemp, Warning, TEXT("GameAgnosticSavedDir(): %s"), *FPaths::GameAgnosticSavedDir());
	UE_LOG(LogTemp, Warning, TEXT("EngineSourceDir(): %s"), *FPaths::EngineSourceDir());
	UE_LOG(LogTemp, Warning, TEXT("GameSourceDir(): %s"), *FPaths::GameSourceDir());
	UE_LOG(LogTemp, Warning, TEXT("FeaturePackDir(): %s"), *FPaths::FeaturePackDir());
}
