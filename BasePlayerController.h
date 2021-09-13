// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure)
	int32 GetEnemiesCount() const;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	UPROPERTY()
	FTimerHandle RestartTimer;

	UPROPERTY()
	UUserWidget* HUD;
	
};
