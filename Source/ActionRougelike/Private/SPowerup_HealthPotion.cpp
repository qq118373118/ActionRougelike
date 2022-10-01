// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{


	CreditsCost = 50;

}


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{

		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditsCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}

FText ASPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return NSLOCTEXT("InteractableActors", "HealthPoint_FullHealthWarning","Already at full health.");
	}

	return FText::Format(NSLOCTEXT("InteractableActors", "HealthPoint_InteractMessage", "Cost{0} Credits.Restores health to maximum."),CreditsCost);

}



