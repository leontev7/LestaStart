// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LestaLaserComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API ULestaLaserComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULestaLaserComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated)
	bool IsOn;
	UPROPERTY(Replicated)
	bool IsReloading;
	UPROPERTY(Replicated)
	FVector LineStart;
	UPROPERTY(Replicated)
	FVector LineEnd;
	UPROPERTY(Replicated)
	int NumOfBullets;
	
	FColor Color;

	float CurTime;

	UPROPERTY(EditAnywhere)
	float DamagePerSecond;
	UPROPERTY(EditAnywhere)
	int MaxNumOfBullets;
	UPROPERTY(EditAnywhere)
	float MaxDamageDistance;
	UPROPERTY(EditAnywhere)
	float ReloadTime;
	UPROPERTY(EditAnywhere)
	float LineThickness = 1.5f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	[[nodiscard]] FVector GetLineStart() const;

	void SetLineStart(const FVector LineStart);

	[[nodiscard]] FVector GetLineEnd() const;

	void SetLineEnd(const FVector LineEnd);

	[[nodiscard]] float GetIsOn() const;
	
	void SetIsOn(bool IsOn);

	void SetColor(const FColor Color);

	[[nodiscard]] float GetMaxDamageDistance() const;

	void SetDamagePerSecond(const float Damage);

	[[nodiscard]] float GetDamagePerSecond() const;
	
	void SetMaxDamageDistance(const float Distance);

	void ReloadWeapon();

	[[nodiscard]] float GetNumOfBullets() const;
	
	[[nodiscard]] float GetIsReloading() const;

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
