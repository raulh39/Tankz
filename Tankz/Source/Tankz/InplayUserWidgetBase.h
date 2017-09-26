// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InplayUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TANKZ_API UInplayUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Current phase of the game (Moving, attacking, ...) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tankz")
	FString PhaseText;

	/** Where to store the widgets taht lists the actions a selected tank can do in COMMAND phase. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tankz")
	UPanelWidget* ActionsListPW();
};
