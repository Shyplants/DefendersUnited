// Fill out your copyright notice in the Description page of Project Settings.


#include "DUEnemy.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TargetPoint.h"
#include "DUEnemyAnimInstance.h"
#include "DefendersUnited/DefendersUnited.h"
#include "DefendersUnited/GameMode/DUGameMode.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"
#include "TimerManager.h"

// Sets default values
ADUEnemy::ADUEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

}

void ADUEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADUEnemy, Health);
	DOREPLIFETIME(ADUEnemy, Mode);
}


// Called when the game starts or when spawned
void ADUEnemy::BeginPlay()
{
	Super::BeginPlay();

	// UpdateHUDHealth();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ADUEnemy::ReceiveDamage);
	}
	
}


void ADUEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ADUEnemy::Destroyed()
{
	Super::Destroyed();

	/*
	if (ElimBotComponent)
	{
		ElimBotComponent->DestroyComponent();
	}

	ADUGameMode* DUGameMode = Cast<ADUGameMode>(UGameplayStatics::GetGameMode(this));
	bool bMatchNotInProgress = DUGameMode && DUGameMode->GetMatchState() != MatchState::InProgress;

	if (Combat && Combat->EquippedWeapon && bMatchNotInProgress)
	{
		Combat->EquippedWeapon->Destroy();
	}
	*/
}

void ADUEnemy::SetMode(int InputMode)
{
	Mode = InputMode;
}

void ADUEnemy::ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f \n"), Health);
	// UpdateHUDHealth();
	// PlayHitReactMontage();

	if (Health == 0.0f)
	{
		
		ADUGameMode* DUGameMode = GetWorld()->GetAuthGameMode<ADUGameMode>();
		if (DUGameMode)
		{
			ADUPlayerController* AttackerController = Cast<ADUPlayerController>(InstigatorController);
			DUGameMode->EnemyEliminated(this, AttackerController);
		}
		
	}
}

void ADUEnemy::OnRep_Mode()
{
	if(Mode == 1)
		PlayFireMontage();
}


void ADUEnemy::OnRep_Health()
{
	// UpdateHUDHealth();
	// PlayHitReactMontage();
}

void ADUEnemy::Elim()
{
	MulticastElim();
	GetWorldTimerManager().SetTimer(
		ElimTimer,
		this,
		&ADUEnemy::ElimTimerFinished,
		ElimDelay
	);
}

void ADUEnemy::ElimTimerFinished()
{
	ADUGameMode* DUGameMode = GetWorld()->GetAuthGameMode<ADUGameMode>();
	if (DUGameMode)
	{
		DUGameMode->RequestRemoveEnemy(this);
	}
}

void ADUEnemy::MulticastElim_Implementation()
{
	bElimmed = true;
	PlayElimMontage();

	/*
	// Start dissolve effect
	if (DissolveMaterialInstances.Num() > 0)
	{
		int index = 0;
		for (UMaterialInstance* DissolveMaterialInstance : DissolveMaterialInstances)
		{
			UMaterialInstanceDynamic* DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
			GetMesh()->SetMaterial(index, DynamicDissolveMaterialInstance);
			DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), 0.55f);
			DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Glow"), 200.f);
			DynamicDissolveMaterialInstances.Add(DynamicDissolveMaterialInstance);

			index++;
		}
	}
	StartDissolve();
	*/

	// Disable character movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*
	// Spawn elim bot
	if (ElimBotEffect)
	{
		FVector ElimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f);
		ElimBotComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			ElimBotEffect,
			ElimBotSpawnPoint,
			GetActorRotation()
		);
	}
	if (ElimBotSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			ElimBotSound,
			GetActorLocation()
		);
	}
	*/
}

FVector ADUEnemy::GetTargetPointLocation()
{
	if (!TargetPoint)
	{
		UE_LOG(LogTemp, Error, TEXT("TargetPoint Empty"));
	}

	return TargetPoint->GetActorLocation();
}

void ADUEnemy::PlayFireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

void ADUEnemy::PlayElimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ElimMontage)
	{
		AnimInstance->Montage_Play(ElimMontage);
	}
}