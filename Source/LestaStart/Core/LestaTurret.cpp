// Fill out your copyright notice in the Description page of Project Settings.


#include "LestaTurret.h"

#include "LestaHealthPointsComponent.h"
#include "TurretWeaponComponent.h"


// Sets default values
ALestaTurret::ALestaTurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Static Mesh Component"));
	Mesh->SetupAttachment(RootComponent);

	HealthPointsComponent = CreateDefaultSubobject<ULestaHealthPointsComponent>(TEXT("Box Health Points Component"));
	HealthPointsComponent->SetMaxHealthPoints(1000);
	
	WeaponComponent = CreateDefaultSubobject<UTurretWeaponComponent>(TEXT("Box Laser Component"));
}

// Called when the game starts or when spawned
void ALestaTurret::BeginPlay()
{
	Super::BeginPlay();
}

