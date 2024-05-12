// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWeaponComponent.h"

#include "LestaCharacter.h"
#include "LestaHealthPointsComponent.h"
#include "LestaLaserComponent.h"
#include "LestaSphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UCharacterWeaponComponent::UCharacterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	LaserComponent = CreateDefaultSubobject<ULestaLaserComponent>(TEXT("Box Laser Component"));
	LaserComponent->SetColor(FColor::Green);

	SphereComponent = CreateDefaultSubobject<ULestaSphereComponent>(TEXT("Box Sphere Component"));

	IsLaser = true;
}


// Called when the game starts
void UCharacterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterWeaponComponent::SetWeapon_Implementation(const int PressedKey)
{
	if (!IsAttacking && !LaserComponent->GetIsReloading()) // Can not change the active weapon when attacking
	{
		if (PressedKey == 1)
			IsLaser = true;
		else if (PressedKey == 2)
		{
			IsLaser = false;
			LaserComponent->SetIsOn(false);
		}

		check(GEngine != nullptr);

		if (IsLaser)
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("The laser is installed by the main weapon"));
		else
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("The sphere is installed by the main weapon"));
	}
}

void UCharacterWeaponComponent::ChangeWeapon_Implementation()
{
	if (!IsLaser) SetWeapon(1);
	else SetWeapon(2);
}

void UCharacterWeaponComponent::StartAttack_Implementation()
{
	if (IsLaser)
	{
		if (LaserComponent->GetNumOfBullets() > 0)
		{
			LaserComponent->SetIsOn(true);
			IsAttacking = true;
		}
		else
		{
			check(GEngine != nullptr);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You need to reload your weapon. Press R."));
		}
	}
	else
	{
		SphereComponent->SetIsCharging(true);
		IsAttacking = true;
	}
}

void UCharacterWeaponComponent::EndAttack_Implementation()
{
	if (IsLaser)
		LaserComponent->SetIsOn(false);
	else
	{
		SphereComponent->SetIsCharging(false);

		TArray<AActor*> OverlappedActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner()); 
		
		UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			GetOwner()->GetActorLocation(),
			SphereComponent->GetFullRadius(),
			ObjectTypes,
			nullptr,
			ActorsToIgnore,
			OverlappedActors
		);
		
		for (const AActor* Actor : OverlappedActors)
		{
			if (!Actor->IsA(ALestaCharacter::StaticClass()))
			{
				if (ULestaHealthPointsComponent* HealthComponent = Cast<ULestaHealthPointsComponent>(Actor->GetComponentByClass(ULestaHealthPointsComponent::StaticClass())))
				{
					HealthComponent->DecreaseHealthPoints(SphereComponent->GetFullDamage());
				}
			}
		}
	}

	IsAttacking = false;
}

void UCharacterWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterWeaponComponent, IsLaser);
	DOREPLIFETIME(UCharacterWeaponComponent, IsAttacking);
}

void UCharacterWeaponComponent::ServerProcessLaserAttack_Implementation(FVector StartPoint, FVector EndPoint,
	float DeltaTime)
{
	if (!GetOwner()->HasAuthority())
		return;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool BHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);
	if (BHit && HitResult.GetActor())
	{
		if (ULestaHealthPointsComponent* HealthComponent = Cast<ULestaHealthPointsComponent>(HitResult.GetActor()->GetComponentByClass(ULestaHealthPointsComponent::StaticClass())))
		{
			HealthComponent->DecreaseHealthPoints(LaserComponent->GetDamagePerSecond() * GetWorld()->DeltaTimeSeconds);
		}
	}
}

void UCharacterWeaponComponent::ReloadWeapon_Implementation()
{
	if (IsLaser && !IsAttacking)
	{
		LaserComponent->ReloadWeapon();
	}
}


// Called every frame
void UCharacterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ALestaCharacter* OwnerCharacter = Cast<ALestaCharacter>(GetOwner()))
	{
		USkeletalMeshComponent* Pistol = OwnerCharacter->GetPistolMeshComponent();

		if (!OwnerCharacter->HasAuthority() && IsLaser && LaserComponent->GetIsOn())
		{
			const FVector StartPoint = Pistol->GetComponentLocation();
			const FVector EndPoint = StartPoint - Pistol->GetForwardVector() * LaserComponent->GetMaxDamageDistance();

			LaserComponent->SetLineStart(StartPoint);
			LaserComponent->SetLineEnd(EndPoint);

			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.bTraceComplex = true;
			CollisionParams.AddIgnoredActor(GetOwner());

			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);
			if (bHit && HitResult.GetActor())
			{
				LaserComponent->SetLineEnd(HitResult.ImpactPoint);
			}

			ServerProcessLaserAttack(StartPoint, EndPoint, DeltaTime);
		}
		else
		{
			SphereComponent->SetCenter(Pistol->GetComponentLocation() + FVector(0, 0, 8));
		}
	}
}


