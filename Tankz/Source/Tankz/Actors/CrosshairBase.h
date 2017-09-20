// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrosshairBase.generated.h"

UCLASS()
class TANKZ_API ACrosshairBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrosshairBase();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tankz")
	void SetSelected(bool isSelected);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
