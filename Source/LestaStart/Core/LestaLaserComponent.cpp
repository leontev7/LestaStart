// Fill out your copyright notice in the Description page of Project Settings.


#include "LestaLaserComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
ULestaLaserComponent::ULestaLaserComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	LineStart = FVector(0, 0 ,0);
	LineEnd = FVector(0, 0, 0);
	Color = FColor::Blue;
	IsOn = false;
	DamagePerSecond = 100;
	MaxDamageDistance = 1000;
	ReloadTime = 3;
	MaxNumOfBullets = 5;
	NumOfBullets = MaxNumOfBullets;
}


// Called when the game starts
void ULestaLaserComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector ULestaLaserComponent::GetLineStart() const
{
	return LineStart;
}

void ULestaLaserComponent::SetLineStart(const FVector NewLineStart)
{
	LineStart = NewLineStart;
}

FVector ULestaLaserComponent::GetLineEnd() const
{
	return LineEnd;
}

void ULestaLaserComponent::SetLineEnd(const FVector NewLineEnd)
{
	LineEnd = NewLineEnd;
}

float ULestaLaserComponent::GetIsOn() const
{
	return IsOn;
}

void ULestaLaserComponent::SetIsOn(bool NewIsOn)
{
	IsOn = NewIsOn;

	if (IsOn == false)
		NumOfBullets -= 1;
}

void ULestaLaserComponent::SetColor(const FColor NewColor)
{
	Color = NewColor;
}

float ULestaLaserComponent::GetMaxDamageDistance() const
{
	return MaxDamageDistance;
}

void ULestaLaserComponent::SetDamagePerSecond(const float Damage)
{
	DamagePerSecond = Damage;
}

float ULestaLaserComponent::GetDamagePerSecond() const
{
	return DamagePerSecond;
}

void ULestaLaserComponent::SetMaxDamageDistance(const float Distance)
{
	MaxDamageDistance = Distance;
}

void ULestaLaserComponent::ReloadWeapon()
{
	IsReloading = true;

	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, ReloadTime, FColor::Yellow, TEXT("Reloading weapon..."));
}

float ULestaLaserComponent::GetNumOfBullets() const
{
	return NumOfBullets;
}

float ULestaLaserComponent::GetIsReloading() const
{
	return IsReloading;
}

void ULestaLaserComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULestaLaserComponent, IsOn);
	DOREPLIFETIME(ULestaLaserComponent, LineStart);
	DOREPLIFETIME(ULestaLaserComponent, LineEnd);
	DOREPLIFETIME(ULestaLaserComponent, NumOfBullets);
}

// Called every frame
void ULestaLaserComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Drawing laser line
	if (IsOn)
		DrawDebugLine(GetWorld(), LineStart, LineEnd, Color, false, DeltaTime / 2, 0, LineThickness);

	if (IsReloading)
	{
		CurTime += DeltaTime;
		if (CurTime > ReloadTime)
		{
			check(GEngine != nullptr);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Weapon Reloaded"));
			IsReloading = false;
			NumOfBullets = MaxNumOfBullets;
			CurTime = 0;
		}
	}
		
}

