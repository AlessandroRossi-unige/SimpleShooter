// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterShotgun.h"


#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

void AShooterShotgun::PullTrigger() // Overriden
{
	if (bCanFireAgain)
	{
		UGameplayStatics::SpawnEmitterAttached(Super::MuzzleFlash, Super::Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(Super::MuzzleSound,  Super::Mesh, TEXT("MuzzleFlashSocket"));

		AController* OwnerController = GetOwnerController();
		if (OwnerController == nullptr)
		{
			return;
		}

		for (int32 i=0; i<ShotsPerPellet; i++) // Shotguns shoots multiple shots per pellet
		{
			FHitResult Hit;
			FVector ShotDirection;
	
			if (GunTrace(OUT Hit,OUT ShotDirection))
			{
		
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Super::Sparkles, Hit.Location, ShotDirection.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Super::SparklesSound, Hit.Location);
				DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
	
				AActor* ActorHit =  Hit.GetActor();
				if (ActorHit != nullptr)
				{
					FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
					ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
				}
			}
		}
		
		bCanFireAgain = false;
		GetWorldTimerManager().SetTimer(ShotTimer, this, &AShooterShotgun::SetCanFireAgain, ShotDelay, false);
	}
	
}

bool AShooterShotgun::GunTrace(OUT FHitResult& Hit,OUT FVector& ShotDirection)
{

	AController* OwnerController = Super::GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	FVector Spread {FMath::RandRange(0.f, SpreadAmount), FMath::RandRange(0.f, SpreadAmount), FMath::RandRange(0.f, SpreadAmount)}; // Spread of shots is configurable
	FVector End = Location + Spread + Rotation.Vector() * MaxRange;
	
	return GetWorld()->LineTraceSingleByChannel(OUT Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AShooterShotgun::SetCanFireAgain() // Wait between shotgun shots
{
	bCanFireAgain = true;
}