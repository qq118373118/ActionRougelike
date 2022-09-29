// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;

	bIsActive = true;

	//当服务器生成一个对象时会通知其他的客户端也生成一个。
	SetReplicates(true);
}


void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{

}


void ASPowerupActor::ShowPowerup()
{
    SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
    SetPowerupState(false);

    GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive) 
{

	bIsActive = bNewIsActive;
	OnRep_IsActive();

}


void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// 设置可见性
	RootComponent->SetVisibility(bIsActive, true);
}


void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}
