// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

void ABasePlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUD->RemoveFromViewport();
	
	if (bIsWinner) // if player is winner show WinScreen
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();	
		}
	}
	else // if player is NOT winner show LoseScreen
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}
	
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

int32 ABasePlayerController::GetEnemiesCount() const
{
	int32 Count = 0;

	for (AAIController* Element : TActorRange<AAIController>(GetWorld()))
	{
		Count++;
	}
	
	return Count;
}

