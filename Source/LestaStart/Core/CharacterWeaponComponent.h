// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterWeaponComponent.generated.h"

class ULestaLaserComponent;
class ULestaSphereComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UCharacterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(Replicated)
	bool IsLaser;
	
	UPROPERTY(Replicated)
	bool IsAttacking;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Unreliable)
	void ServerProcessLaserAttack(FVector StartPoint, FVector EndPoint, float DeltaTime);

public:
	// Sets default values for this component's properties
	UCharacterWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULestaLaserComponent> LaserComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULestaSphereComponent> SphereComponent;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Server, Unreliable)
	void SetWeapon(const int PressedKey);

	UFUNCTION(Server, Unreliable)
	void ChangeWeapon();

	UFUNCTION(Server, Unreliable)
	void StartAttack();

	UFUNCTION(Server, Unreliable)
	void EndAttack();

	UFUNCTION(Server, Unreliable)
	void ReloadWeapon();
};
