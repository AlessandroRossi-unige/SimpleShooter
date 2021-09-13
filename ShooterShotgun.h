// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGun.h"
#include "ShooterShotgun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterShotgun : public AShooterGun
{
	GENERATED_BODY()

public:
	
	void PullTrigger() override;
	
private:

	UPROPERTY()
	FTimerHandle ShotTimer;

	UPROPERTY(EditAnywhere)
	float ShotDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float SpreadAmount = 30.f;

	UPROPERTY(EditAnywhere)
	int32 ShotsPerPellet = 5;
	
	UPROPERTY()
	bool bCanFireAgain = true;

	bool GunTrace(OUT FHitResult& Hit, OUT FVector& ShotDirection) override;

	void SetCanFireAgain();
	
};
