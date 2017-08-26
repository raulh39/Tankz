#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Tank.generated.h"

UCLASS()
class TANKZ_API ATank : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATank();

	UPROPERTY()
	UStaticMeshComponent* MainTankBodyMesh;

	UPROPERTY()
	USplineComponent* BorderPath;

	UPROPERTY()
	UMaterialInstanceDynamic* MainTankBodyMaterialDynamicInstance;

	UPROPERTY()
	bool isSelected;

	UFUNCTION()
	void toggleSelected();

	UFUNCTION()
	void SetBaseColor(FLinearColor newBaseColor);
	
	UFUNCTION()
	void SetTankMesh(UStaticMesh* NewMesh);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void CreateComponents();
	void CreateMaterials();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
