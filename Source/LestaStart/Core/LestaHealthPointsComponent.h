// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LestaHealthPointsComponent.generated.h"

class UTextRenderComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API ULestaHealthPointsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULestaHealthPointsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	TObjectPtr<UTextRenderComponent> HealthPointsTextRenderComponent;
	
	UPROPERTY(Replicated)
	float HealthPoints = 0;

	UPROPERTY(Replicated, EditAnywhere)
	float MaxHealthPoints = 0;
	
	[[nodiscard]] float GetHealthPoints() const;

	void SetMaxHealthPoints(const float NewMaxHealthPoints);
	
	void DecreaseHealthPoints(const float DeltaHealthPoints);

private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
