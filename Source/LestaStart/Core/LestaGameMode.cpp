// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaGameMode.h"

#include "LestaCharacter.h"
#include "Kismet/GameplayStatics.h"


void ALestaGameMode::CharacterDied(ALestaCharacter* DeadCharacter) const
{

	TArray<ALestaCharacter*> Characters;
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALestaCharacter::StaticClass(), FoundCharacters);
    
	for (AActor* Actor : FoundCharacters)
	{
		ALestaCharacter* LestaCharacter = Cast<ALestaCharacter>(Actor);
		if (LestaCharacter)
		{
			Characters.AddUnique(LestaCharacter);
		}
	}
	
	if (DeadCharacter)
	{
		Characters.Remove(DeadCharacter);

		if (Characters.Num() == 0)
		{

			check(GEngine != nullptr);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Restarting level..."));
			
			if (UWorld* World = GetWorld())
			{
				FString CurrentLevelName = World->GetMapName();
				CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);
				World->ServerTravel(FString::Printf(TEXT("%s?reload"), *CurrentLevelName));
			}
		}
	}
}

