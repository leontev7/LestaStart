// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LestaGameMode.generated.h"

class ALestaCharacter;

/** Base Game Mode class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void CharacterDied(ALestaCharacter* DeadCharacter) const;
};
