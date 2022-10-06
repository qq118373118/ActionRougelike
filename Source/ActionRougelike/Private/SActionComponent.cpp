// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"
#include "SAction.h"
#include "../ActionRougelike.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"


DECLARE_CYCLE_STAT(TEXT("StartActionByName"), STAT_StartActionByName, STATGROUP_STANFORD);

USActionComponent::USActionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	//设置同步，如果不设置同步，技能就没办法同步
	SetIsReplicatedByDefault(true);
	
}



void USActionComponent::BeginPlay(){

	Super::BeginPlay();

	//只在服务器上进行技能的注册
	if (GetOwner()->HasAuthority())
	{

		for (TSubclassOf<USAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}




void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	TArray<USAction*> ActiosCopy = Actions;
	for (USAction* Action : ActiosCopy)
	{
		if (Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	for (USAction* Action : Actions)
	{
		FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()),*GetNameSafe(Action));

		LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	}
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	//
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddActuib.[Class:%s]"), * GetNameSafe(ActionClass));
		return;
	}



	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);

	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
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

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}

	return nullptr;
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{

	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	//遍历组件中存在的技能
	for (USAction* Action : Actions)
	{
		//判断这个技能是否存在
		if (Action && Action->ActionName == ActionName)
		{

			//如果不能释放技能，打印提示信息
			if (!Action->CanStart(Instigator))
			{

				FString FailedMeg = FString::Printf(TEXT("Failed to run:%s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMeg);
				continue;
			}

			//如果是客户端就执行
			if (!GetOwner()->HasAuthority())
			{
				//这个函数是个RPC调用，因用Server修饰，所以客户端请求服务器执行该函数，客户端本身不执行。
				ServerStartAction(Instigator, ActionName);
			}

			TRACE_BOOKMARK(TEXT("StartAction:%s"), *GetNameSafe(Action));

			//客户端会在本地执行一次
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

				//如果是客户端就执行
				if (!GetOwner()->HasAuthority())
				{
					//这个函数是个RPC调用，因用Server修饰，所以客户端请求服务器执行该函数，客户端本身不执行。
					ServerStopAction(Instigator, ActionName);
				}


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


void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}



bool USActionComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}


void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent, Actions);
}

