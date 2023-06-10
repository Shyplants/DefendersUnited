// Fill out your copyright notice in the Description page of Project Settings.


#include "DUEnemy.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TargetPoint.h"
#include "DUEnemyAnimInstance.h"

// Sets default values
ADUEnemy::ADUEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	UE_LOG(LogTemp, Warning, TEXT("SetMode: %d\n"), Mode);
}

void ADUEnemy::ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	// UpdateHUDHealth();
	// PlayHitReactMontage();

	if (Health == 0.0f)
	{
		/*
		ADUGameMode* DUGameMode = GetWorld()->GetAuthGameMode<ADUGameMode>();
		if (DUGameMode)
		{
			DUPlayerController = DUPlayerController == nullptr ? Cast<ADUPlayerController>(Controller) : DUPlayerController;
			ADUPlayerController* AttackerController = Cast<ADUPlayerController>(InstigatorController);
			DUGameMode->PlayerEliminated(this, DUPlayerController, AttackerController);
		}
		*/
	}
}

void ADUEnemy::OnRep_Mode()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Mode: %d\n"), Mode);
	if(Mode == 1)
		PlayFireMontage();
}


void ADUEnemy::OnRep_Health()
{
	// UpdateHUDHealth();
	// PlayHitReactMontage();
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
	UE_LOG(LogTemp, Warning, TEXT("PlayFireMontage Called\n"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UE_LOG(LogTemp, Warning, TEXT("AnimInstance Valid : %d\n"), AnimInstance != NULL);
	if (AnimInstance && FireMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireMontage Valid \n"));
		AnimInstance->Montage_Play(FireMontage);
	}
}