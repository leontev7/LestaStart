// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LestaSphereComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API ULestaSphereComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULestaSphereComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	float Radius;

	UPROPERTY(Replicated)
	bool IsCharging;
	
	FColor Color;

	UPROPERTY(Replicated)
	FVector Center;

	UPROPERTY(Replicated)
	float DamageCoefficient;
	
	float MinDamageCoefficient;
	float MaxDamageCoefficient;
	float DeltaDamageCoefficient;
	float DefaultDamage;
	
	float LineThickness;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetIsCharging(const bool IsCharging);
	void SetCenter(const FVector Center);
	[[nodiscard]] FVector GetCenter() const;
	[[nodiscard]] float GetFullRadius() const;
	[[nodiscard]] float GetFullDamage() const;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
