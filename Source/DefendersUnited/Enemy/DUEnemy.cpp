#include "DUEnemy.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
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

ADUEnemy::ADUEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	EnemyOverlayComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyOverlayComponent"));
	EnemyOverlayComponent->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(RootComponent);

	TargetPointLocation = FVector(1540.f, -3380.f, 40.f);
}

void ADUEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADUEnemy, Health);
	DOREPLIFETIME(ADUEnemy, Mode);
}


void ADUEnemy::BeginPlay()
{
	Super::BeginPlay();

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
			EnemyOverlayComponent->SetWidget(EnemyOverlay);
			EnemyOverlayComponent->SetVisibility(true);
		}
	}

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ADUEnemy::OnHit);
}

void ADUEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADUCharacter* DUCharacter = Cast<ADUCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	FRotator LookAtRotator;

	if (EnemyOverlayComponent && DUCharacter)
	{
		LookAtRotator = UKismetMathLibrary::FindLookAtRotation(EnemyOverlayComponent->GetComponentLocation(), DUCharacter->GetActorLocation());

		EnemyOverlayComponent->SetWorldRotation(LookAtRotator);
	}
}

void ADUEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	ADUCharacter* DUCharacter = Cast<ADUCharacter>(Hit.GetActor());
	AController* OwnerController = GetController();
	if (OwnerController && DUCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage Call"));
		UGameplayStatics::ApplyDamage(DUCharacter, 5.f, OwnerController, this, UDamageType::StaticClass());
	}
}

void ADUEnemy::Destroyed()
{
	Super::Destroyed();
}

void ADUEnemy::SetMode(int InputMode)
{
	Mode = InputMode;
}

void ADUEnemy::ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if (Health == 0.0f) return;

	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	SetHUDHealth();

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

	// Disable character movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ADUEnemy::GetTargetPointLocation()
{
	return TargetPointLocation;
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

	if (bHUDValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyOverlay && EnemyOverlay->HealthBar"));
		const float HealthPercent = Health / MaxHealth;
		UE_LOG(LogTemp, Warning, TEXT("HealthPercent : %f"), HealthPercent);
		EnemyOverlay->HealthBar->SetPercent(HealthPercent);
	}
	
}
