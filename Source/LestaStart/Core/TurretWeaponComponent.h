// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurretWeaponComponent.generated.h"

class ULestaLaserComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UTurretWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float DamagePerSecond;
	UPROPERTY(EditAnywhere)
	float MaxDamageDistance;
	
public:
	// Sets default values for this component's properties
	UTurretWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TObjectPtr<ULestaLaserComponent> LaserComponent;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
