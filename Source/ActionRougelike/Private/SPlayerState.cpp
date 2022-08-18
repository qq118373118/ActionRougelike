// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f))
	{
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);

}

bool ASPlayerState::RemoveCredits(int32 Delta)
{

	//这是我的写法

	if (Credits > Delta&&ensure(Delta>0.0f)) {

		Credits -= Delta;

		OnCreditsChanged.Broadcast(this, Credits, -Delta);

		return true;
	}
	
	return false;

	/*
	* 
	* 以下代码为视频教程中的写法：
	* 
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
	*/
	
}
