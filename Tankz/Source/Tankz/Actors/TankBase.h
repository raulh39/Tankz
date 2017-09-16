// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/TankData.h"
#include <Components/SplineComponent.h>
#include "TankBase.generated.h"

UCLASS()
class TANKZ_API ATankBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankBase();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tankz")
	void SetSelected(bool isSelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tankz")
	void SetBaseColor(FLinearColor newBaseColor);

	UPROPERTY(BlueprintReadOnly, Category = "Tankz")
	FTankState InitialState;
	UPROPERTY(BlueprintReadOnly, Category = "Tankz")
	FTankState CurrentState;
	
	UPROPERTY(EditAnywhere, Category = "Tankz")
	USplineComponent* BorderPath;

	void InitializeState(FTankData &state) {
		InitialState = state;
		CurrentState = state;
	}

	bool hasActed;
	bool isAlive;
	int movesDone;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
