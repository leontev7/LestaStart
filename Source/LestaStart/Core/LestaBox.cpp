// Fill out your copyright notice in the Description page of Project Settings.


#include "LestaBox.h"

#include "LestaHealthPointsComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALestaBox::ALestaBox()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Box Scene Component"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Static Mesh Component"));
	Mesh->SetupAttachment(RootComponent);

	HealthPointsComponent = CreateDefaultSubobject<ULestaHealthPointsComponent>(TEXT("Box Health Points Component"));
	HealthPointsComponent->SetMaxHealthPoints(100);
}


