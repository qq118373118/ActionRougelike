// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{

	SphereComp->SetSphereRadius(20.0f);

	MoveComp->InitialSpeed = 8000.f;

	DamageValue = -20.0f;

}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hotreload)
	// PostInitializeComponent is the preferred way of binding any events.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor!= GetInstigator())
	{
		static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");


		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

		//·À·´£¨·´µ¯£©
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}


		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageValue, SweepResult))
		{
			Explode(); 


			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(),BurningActionClass);
			}

		}

	}

}


