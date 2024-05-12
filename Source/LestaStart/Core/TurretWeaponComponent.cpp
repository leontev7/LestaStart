// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretWeaponComponent.h"

#include "LestaCharacter.h"
#include "LestaHealthPointsComponent.h"
#include "LestaLaserComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTurretWeaponComponent::UTurretWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);

	LaserComponent = CreateDefaultSubobject<ULestaLaserComponent>(TEXT("Box Laser Component"));
	LaserComponent->SetColor(FColor::Red);
}


// Called when the game starts
void UTurretWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	LaserComponent->SetLineStart(GetOwner()->GetActorLocation());
    LaserComponent->SetDamagePerSecond(DamagePerSecond);
    LaserComponent->SetMaxDamageDistance(MaxDamageDistance);
}


// Called every frame
void UTurretWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector MyLocation = GetOwner()->GetActorLocation();
    float SearchRadius = LaserComponent->GetMaxDamageDistance();
    TArray<AActor*> FoundActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        MyLocation,
        SearchRadius,
        ObjectTypes,
        ALestaCharacter::StaticClass(),
        ActorsToIgnore,
        FoundActors
    );
    
    FoundActors.Sort([&](const AActor& A, const AActor& B) {
        return FVector::Dist(MyLocation, A.GetActorLocation()) < FVector::Dist(MyLocation, B.GetActorLocation());
    });

    ACharacter* NearestCharacter = nullptr;

    for (AActor* Actor : FoundActors)
    {
        if (ACharacter* Character = Cast<ACharacter>(Actor))
        {
            FVector CharacterLocation = Character->GetActorLocation();

            FHitResult Hit;
            FCollisionQueryParams Params(FName(TEXT("LaserTrace")), true, GetOwner());
            Params.bTraceComplex = true;
            Params.AddIgnoredActor(GetOwner());

            if (!GetWorld()->LineTraceSingleByChannel(
                Hit,
                MyLocation,
                CharacterLocation,
                ECC_Visibility,
                Params))
            {
                NearestCharacter = Character;
                break;
            }
        }
    }

    if (NearestCharacter)
    {
        ULestaHealthPointsComponent* HealthComponent = Cast<ULestaHealthPointsComponent>(NearestCharacter->GetComponentByClass(ULestaHealthPointsComponent::StaticClass()));
	
        if (HealthComponent)
            HealthComponent->DecreaseHealthPoints(LaserComponent->GetDamagePerSecond() * DeltaTime);
        
        LaserComponent->SetIsOn(true);
        LaserComponent->SetLineEnd(NearestCharacter->GetActorLocation());
    }
    else
    {
        LaserComponent->SetIsOn(false);
    }
}


