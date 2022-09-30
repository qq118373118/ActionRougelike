// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include <SActionComponent.h>
#include <GameFramework/GameStateBase.h>


USActionEffect::USActionEffect()
{
	bAutoStart = true;
}


float USActionEffect::GetTimeRemaining() const
{

	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//当Duration(持续时间)大于0时，设置一个时间句柄，在持续时间结束后触发 StopAction 函数
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);

	}

	//Period(周期) 表示两次攻击间的时间间隔
	//以 Period 为周期间隔不断循环触发委托
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}


}


//停止并清除句柄
void USActionEffect::StopAction_Implementation(AActor* Instigator)
{

	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Comp = GetOwningComponent();

	if (Comp)
	{
		Comp->RemoveAction(this);
	}


}

