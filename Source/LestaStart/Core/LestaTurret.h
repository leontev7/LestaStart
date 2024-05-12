// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LestaTurret.generated.h"

class UTurretWeaponComponent;
class ULestaHealthPointsComponent;

UCLASS()
class LESTASTART_API ALestaTurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALestaTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTurretWeaponComponent> WeaponComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULestaHealthPointsComponent> HealthPointsComponent;
};
