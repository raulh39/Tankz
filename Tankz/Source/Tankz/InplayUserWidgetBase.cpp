// No copyright notice because I don't like copyright

#include "InplayUserWidgetBase.h"

UPanelWidget* UInplayUserWidgetBase::ActionsListPW_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("ActionsListPW_Implementation called. The BP should have overrided this."));
	return nullptr;
}
