// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ShooterGun.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"
class AShooterGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	AShooterCharacter();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	UFUNCTION(BlueprintPure)
    float GetHealthPercent() const;

	UPROPERTY(BlueprintReadOnly)
	bool bCanShoot = true;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void PullTrigger();

	void Switch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	void Jump();
	
	void HealthRegen();
	void CanShootAfterReload();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShooterGun> PrimaryClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShooterGun> SecondaryClass;

	UPROPERTY()
	AShooterGun* Gun;

	UPROPERTY()
	bool bIsPrimaryEquipped;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY()
	FTimerHandle HealthRegenTimer;

	UPROPERTY(EditAnywhere)
	UAnimSequence* SwitchWeaponAnim;

	UPROPERTY()
	FTimerHandle SwitchAnimationTimer;
	
};
