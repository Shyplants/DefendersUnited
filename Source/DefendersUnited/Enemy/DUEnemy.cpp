// Fill out your copyright notice in the Description page of Project Settings.


#include "DUEnemy.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TargetPoint.h"
#include "DUEnemyAnimInstance.h"
#include "DefendersUnited/DefendersUnited.h"
#include "DefendersUnited/GameMode/DUGameMode.h"
#include "DefendersUnited/Character/DUCharacter.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"
#include "DefendersUnited/HUD/EnemyOverlay.h"
#include "DUAIController.h"
#include "TimerManager.h"

// Sets default values
ADUEnemy::ADUEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);


	EnemyOverlayComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyOverlayComponent"));
	EnemyOverlayComponent->SetupAttachment(RootComponent);
	
	/*
	EnemyOverlayComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyOverlayComponent"));
	EnemyOverlayComponent->SetupAttachment(RootComponent);
	if (EnemyOverlayComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyOverlay Casted"));
		EnemyOverlayComponent.WidgetCl
		EnemyOverlay = EnemyOverlayComponent->GetWidgetClass();
		UE_LOG(LogTemp, Warning, TEXT("EnemyOverlay State: %d"), EnemyOverlayComponent->GetWidgetClass()->IsValidLowLevel());
		SetHUDHealth();
	}

	*/
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

	if (EnemyOverlayClass)
	{
		EnemyOverlay = CreateWidget<UEnemyOverlay>(GetWorld(), EnemyOverlayClass);
	}
	if (EnemyOverlay)
	{
		FVector EnemyLocation = GetActorLocation();
		
		if (EnemyOverlayComponent)
		{
			
			// EnemyOverlayComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			EnemyOverlayComponent->SetWidget(EnemyOverlay);
			// EnemyOverlayComponent->SetWorldLocation(FVector(0.f, 0.f, 134.f)); // 적 오브젝트 머리 위로 위치 조정
			EnemyOverlayComponent->SetVisibility(true);
		}


		// EnemyOverlay->SetPositionInViewport(FVector2D(0, 5.f));
		// EnemyOverlay->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyOverlay Not Casted!"));
	}
	
}


void ADUEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	ADUCharacter* DUCharacter = Cast<ADUCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	FRotator LookAtRotator;

	if (EnemyOverlayComponent && DUCharacter)
	{
		// UE_LOG(LogTemp, Warning, TEXT("EnemyHealthWidget && DUCharacter Valid"));
		LookAtRotator = UKismetMathLibrary::FindLookAtRotation(EnemyOverlayComponent->GetComponentLocation(), DUCharacter->GetActorLocation());

		EnemyOverlayComponent->SetWorldRotation(LookAtRotator);
	}
	
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
	if (Health == 0.0f) return;

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health: %f \n"), Health);
	SetHUDHealth();
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
	SetHUDHealth();
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

void ADUEnemy::SetHUDHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("SetHUDHealth Called"));
	bool bHUDValid = EnemyOverlay &&
		EnemyOverlay->HealthBar;

	/*
	UE_LOG(LogTemp, Warning, TEXT("EnemyOverlay Valid"), EnemyOverlay);
	UE_LOG(LogTemp, Warning, TEXT("HealthBar Valid"), EnemyOverlay->HealthBar);
	*/

	if (bHUDValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyOverlay && EnemyOverlay->HealthBar"));
		const float HealthPercent = Health / MaxHealth;
		UE_LOG(LogTemp, Warning, TEXT("HealthPercent : %f"), HealthPercent);
		EnemyOverlay->HealthBar->SetPercent(HealthPercent);
	}
	
}