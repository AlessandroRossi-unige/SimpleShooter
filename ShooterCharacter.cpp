// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "BasePlayerController.h"
#include "EngineUtils.h"
#include "ShooterGun.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 
	// static ConstructorHelpers::FObjectFinder<UAnimSequence> anim(TEXT("AnimSequence'/Game/AssetPacks/ParagonWraith/Characters/Heroes/Wraith/Animations/Respawn.Respawn'"));
	// SwitchWeaponAnim = anim.Object;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	
	Gun = GetWorld()->SpawnActor<AShooterGun>(PrimaryClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	
	bIsPrimaryEquipped = true; // Primary weapon equipped by default
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	
	Health -= DamageToApply;
	
	if (IsDead())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Remove collision from dead body
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy(); // Destroy dead actor
	}
	
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		GetWorldTimerManager().SetTimer(HealthRegenTimer, this, &AShooterCharacter::HealthRegen, 1.5f, true,  5.f); // if hasn't taken damage in 5 seconds, regen health
	}
	return DamageToApply;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Setup keybindings
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PullTrigger);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Switch);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::Jump()
{
	ACharacter::Jump();
}

void AShooterCharacter::PullTrigger()
{
	if (bCanShoot) // Can't shoot while switching weapon
	{
		Gun->PullTrigger();
	}
}

void AShooterCharacter::Switch()
{
	bCanShoot = false; // Can't shoot while switching weapon
	GetWorldTimerManager().SetTimer(SwitchAnimationTimer, this, &AShooterCharacter::CanShootAfterReload, 2.f, false); // Can shoot after animation is done 
	
	if (bIsPrimaryEquipped) // Switch from primaty to secondary
	{
		Gun = GetWorld()->SpawnActor<AShooterGun>(SecondaryClass);
		bIsPrimaryEquipped = false;
	}
	else // Switch from secondary to primary
	{
		Gun = GetWorld()->SpawnActor<AShooterGun>(PrimaryClass);
		bIsPrimaryEquipped = true;
	}
	// Attach gun to character
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

void AShooterCharacter::CanShootAfterReload()
{
	bCanShoot = true;
}

bool AShooterCharacter::IsDead() const
{
	return Health == 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::HealthRegen()
{
	while (Health < MaxHealth)
	{
		Health++;
	}
}
