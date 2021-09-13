// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AShooterGun::AShooterGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AShooterGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShooterGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AController* AShooterGun::GetOwnerController() const
{
	APawn* OwnerPawm = Cast<APawn>(GetOwner());
	if (OwnerPawm == nullptr)
	{
		return nullptr;
	};
	return OwnerPawm->GetController();
}

bool AShooterGun::GunTrace(OUT FHitResult& Hit,OUT FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	return GetWorld()->LineTraceSingleByChannel(OUT Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AShooterGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound,  Mesh, TEXT("MuzzleFlashSocket"));

	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return;
	}
	
	FHitResult Hit;
	FVector ShotDirection;
	
	if (GunTrace(OUT Hit,OUT ShotDirection))
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sparkles, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SparklesSound, Hit.Location);
		DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
	
		AActor* ActorHit =  Hit.GetActor();
		if (ActorHit != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

