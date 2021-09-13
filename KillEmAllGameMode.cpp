// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"


#include "BaseAIController.h"
#include "BasePlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr) 
	{
		EndGame(false); // if the pawn killed is the player EndGame, player lost
	}

	for (ABaseAIController* AIController : TActorRange<ABaseAIController>(GetWorld()))
	{
		if(!AIController->IsDead()) // if at least an enemy is alive return
		{
			return; 
		}
	}

	EndGame(true); // if every enemy is dead EndGame
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsPlayerController = Controller->IsPlayerController();
		if (bIsPlayerWinner)
		{
			Controller->GameHasEnded(Controller->GetPawn(), bIsPlayerController);
		}
		else
		{
			Controller->GameHasEnded(Controller->GetPawn(), !bIsPlayerController);
		}
	}
}
