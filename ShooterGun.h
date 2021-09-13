// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterGun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterGun : public AActor
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	AShooterGun();

	virtual void PullTrigger();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AController* GetOwnerController() const;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* Sparkles;
	
	UPROPERTY(EditAnywhere)
	USoundBase* SparklesSound;

private:

	virtual bool GunTrace(OUT FHitResult& Hit, OUT FVector& ShotDirection);

};