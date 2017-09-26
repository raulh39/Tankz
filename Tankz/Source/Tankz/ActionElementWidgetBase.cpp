// No copyright notice because I don't like copyright

#include "ActionElementWidgetBase.h"

void UActionElementWidgetBase::SetHighlight_Implementation(bool activate)
{
	UE_LOG(LogTemp, Error, TEXT("SetHighlight_Implementation called. The BP should have overrided this."));
}

void UActionElementWidgetBase::SetText_Implementation(const FString& text)
{
	UE_LOG(LogTemp, Error, TEXT("SetText_Implementation called. The BP should have overrided this."));
}
