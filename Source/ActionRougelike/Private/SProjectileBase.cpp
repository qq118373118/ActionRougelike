// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"

ASProjectileBase::ASProjectileBase()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
    SphereComp->SetCollisionProfileName("Projectile");
    RootComponent = SphereComp;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetupAttachment(RootComponent);

    AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
    AudioComp->SetupAttachment(RootComponent);



    EffectComp->OnSystemFinished.AddDynamic(this,&ASProjectileBase::OnDeactivated);

    MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComp");
    MoveComp->bRotationFollowsVelocity = true;
    MoveComp->bInitialVelocityInLocalSpace = true;
    MoveComp->ProjectileGravityScale = 0.0f;
    MoveComp->InitialSpeed = 800.f;

    //SetReplicates(true);
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Explode();
}

void ASProjectileBase::OnDeactivated(class UParticleSystemComponent* PSystem)
{
    Destroy();
}

void ASProjectileBase::Explode_Implementation()
{
    if (ensure(!IsPendingKill()))
    {
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		EffectComp->DeactivateSystem();

		MoveComp->StopMovementImmediately();
		SetActorEnableCollision(false);

		Destroy();
        
    }
}

void ASProjectileBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}
