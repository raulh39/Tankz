// No copyright notice because I don't like copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionElementWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TANKZ_API UActionElementWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tankz")
	void SetHighlight(bool activate);
};
