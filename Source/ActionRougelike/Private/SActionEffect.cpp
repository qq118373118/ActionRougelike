// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"
#include <SActionComponent.h>


USActionEffect::USActionEffect()
{
	bAutoStart = true;
}


void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//��Duration(����ʱ��)����0ʱ������һ��ʱ�������ڳ���ʱ������󴥷� StopAction ����
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);

	}

	//Period(����) ��ʾ���ι������ʱ����
	//�� Period Ϊ���ڼ������ѭ������ί��
	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}


}


//ֹͣ��������
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
