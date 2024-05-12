// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LestaBox.generated.h"

class UStaticMeshComponent;
class ULestaHealthPointsComponent;

UCLASS()
class LESTASTART_API ALestaBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALestaBox();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULestaHealthPointsComponent> HealthPointsComponent;
};
