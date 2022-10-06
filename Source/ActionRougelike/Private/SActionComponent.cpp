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

	//����ͬ�������������ͬ�������ܾ�û�취ͬ��
	SetIsReplicatedByDefault(true);
	
}



void USActionComponent::BeginPlay(){

	Super::BeginPlay();

	//ֻ�ڷ������Ͻ��м��ܵ�ע��
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
	//д����ô������Ϊ��������������ʱ��������ʾ��ͬʱ�ֲ�Ӱ����������߼�������
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

	//��������д��ڵļ���
	for (USAction* Action : Actions)
	{
		//�ж���������Ƿ����
		if (Action && Action->ActionName == ActionName)
		{

			//��������ͷż��ܣ���ӡ��ʾ��Ϣ
			if (!Action->CanStart(Instigator))
			{

				FString FailedMeg = FString::Printf(TEXT("Failed to run:%s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMeg);
				continue;
			}

			//����ǿͻ��˾�ִ��
			if (!GetOwner()->HasAuthority())
			{
				//��������Ǹ�RPC���ã�����Server���Σ����Կͻ������������ִ�иú������ͻ��˱���ִ�С�
				ServerStartAction(Instigator, ActionName);
			}

			TRACE_BOOKMARK(TEXT("StartAction:%s"), *GetNameSafe(Action));

			//�ͻ��˻��ڱ���ִ��һ��
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

				//����ǿͻ��˾�ִ��
				if (!GetOwner()->HasAuthority())
				{
					//��������Ǹ�RPC���ã�����Server���Σ����Կͻ������������ִ�иú������ͻ��˱���ִ�С�
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

