// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox * ParentSizeBox;


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
public:

	UPROPERTY(EditAnywhere,Category = "UI")
	FVector WorldOffset;

	//mark?ExposeOnSpawn：在生成时暴露。当修饰变量的时候，在生成的时，蓝图里的create widget就会多一个参数节点.
	//mate的作用,（猜测）修饰属性，与细节面板上的内容相同
	UPROPERTY(BlueprintReadWrite, Category = "UI",meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;

};
