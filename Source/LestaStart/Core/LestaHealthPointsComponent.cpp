// Fill out your copyright notice in the Description page of Project Settings.


#include "LestaHealthPointsComponent.h"

#include "LestaCharacter.h"
#include "LestaGameMode.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"


ULestaHealthPointsComponent::ULestaHealthPointsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void ULestaHealthPointsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		HealthPoints = MaxHealthPoints;
	}

	HealthPointsTextRenderComponent = NewObject<UTextRenderComponent>(GetOwner());
	HealthPointsTextRenderComponent->RegisterComponent();
	HealthPointsTextRenderComponent->SetText(FText::AsNumber(HealthPoints));
	HealthPointsTextRenderComponent->SetWorldSize(50.0f);
	HealthPointsTextRenderComponent->SetTextRenderColor(FColor::Green);
	HealthPointsTextRenderComponent->SetHorizontalAlignment(EHTA_Center);
	HealthPointsTextRenderComponent->SetVerticalAlignment(EVRTA_TextCenter);
	HealthPointsTextRenderComponent->SetRelativeLocation(GetOwner()->GetActorLocation() + FVector(0, 0, 100));
}

float ULestaHealthPointsComponent::GetHealthPoints() const
{
	return HealthPoints;
}

void ULestaHealthPointsComponent::SetMaxHealthPoints(const float NewMaxHealthPoints)
{
	MaxHealthPoints = NewMaxHealthPoints;
}

void ULestaHealthPointsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULestaHealthPointsComponent, HealthPoints);
	DOREPLIFETIME(ULestaHealthPointsComponent, MaxHealthPoints);
}

void ULestaHealthPointsComponent::DecreaseHealthPoints(const float DeltaHealthPoints)
{
	if (GetOwner()->HasAuthority())
	{
		HealthPoints -= DeltaHealthPoints;
	}
}

void ULestaHealthPointsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (HealthPointsTextRenderComponent)
	{
		const FString HealthString = FString::Printf(TEXT("%0.0f"), HealthPoints);
		HealthPointsTextRenderComponent->SetText(FText::FromString(HealthString));
		HealthPointsTextRenderComponent->SetRelativeLocation(GetOwner()->GetActorLocation() + FVector(0, 0, 300));
	}
	
	if (HealthPoints <= 0)
	{
		AActor* Owner = GetOwner();
		const ALestaGameMode* GameMode = Cast<ALestaGameMode>(GetWorld()->GetAuthGameMode());
		ALestaCharacter* DeadCharacter = Cast<ALestaCharacter>(Owner);
		if (Owner && GameMode && DeadCharacter)
		{
			// Notify the game mode that the character has died
			GameMode->CharacterDied(DeadCharacter);
		}

		Owner->Destroy();
	}
}

