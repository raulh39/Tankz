// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
