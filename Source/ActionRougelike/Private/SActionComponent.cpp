// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"

USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}




void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<USAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(),ActionClass);
	}

}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);


}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}

	if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)) )
	{
		NewAction->StartAction(Instigator);
	}

}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	//写的这么复杂是为了以下情况报错的时候会给个提示，同时又不影响这个函数逻辑和运行
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{

			if (!Action->CanStart(Instigator))
			{

				FString FailedMeg = FString::Printf(TEXT("Failed to run:%s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMeg);
				continue;
			}

			//客户端会触发下面的函数，让其在服务端显示客户端发出的动作。
			if (!GetOwner()->HasAuthority()) {
				ServerStartAction(Instigator, ActionName);
			}
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{

			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}


void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}