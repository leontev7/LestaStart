// Fill out your copyright notice in the Description page of Project Settings.


#include "LestaSphereComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
ULestaSphereComponent::ULestaSphereComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	IsCharging = false;
	Radius = 50;
	Color = FColor::Green;
	
	LineThickness = 1;

	MinDamageCoefficient = 0;
	MaxDamageCoefficient = 5;
	DeltaDamageCoefficient = 0.1;
	DamageCoefficient = MinDamageCoefficient;
	DefaultDamage = 100;
}


// Called when the game starts
void ULestaSphereComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULestaSphereComponent::SetIsCharging(const bool NewIsCharging)
{
	IsCharging = NewIsCharging;
}

void ULestaSphereComponent::SetCenter(const FVector NewCenter)
{
	Center = NewCenter;
}

FVector ULestaSphereComponent::GetCenter() const
{
	return Center;
}

float ULestaSphereComponent::GetFullRadius() const
{
	return Radius * DamageCoefficient;
}

float ULestaSphereComponent::GetFullDamage() const
{
	return DefaultDamage * DamageCoefficient;
}

void ULestaSphereComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ULestaSphereComponent, Center);
	DOREPLIFETIME(ULestaSphereComponent, DamageCoefficient);
	DOREPLIFETIME(ULestaSphereComponent, IsCharging);
}


// Called every frame
void ULestaSphereComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsCharging)
	{
		DamageCoefficient += DeltaDamageCoefficient;
		if (DamageCoefficient > MaxDamageCoefficient)
			DamageCoefficient = MaxDamageCoefficient;

		DrawDebugSphere(GetWorld(), Center, Radius * DamageCoefficient, 50, Color, false, DeltaTime / 2, 0, LineThickness * 2 * DamageCoefficient);
	}
	else
	{
		DamageCoefficient = MinDamageCoefficient;
	}
}

