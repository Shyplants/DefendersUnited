#include "DUAIController.h"
#include "DUEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ADUAIController::ADUAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (AIPerceptionComponent)
	{
		SetPerceptionComponent(*AIPerceptionComponent);

		if (SightConfig)
		{
			SightConfig->SightRadius = 400.f;
			SightConfig->LoseSightRadius = 500.f;
			SightConfig->PeripheralVisionAngleDegrees = 135.f;
			SightConfig->DetectionByAffiliation = FAISenseAffiliationFilter();
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;


			AIPerceptionComponent->ConfigureSense(*SightConfig);
			AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

			AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ADUAIController::OnPlayerDetected);
		}
	}

	SetGenericTeamId(FGenericTeamId(5));
}

ETeamAttitude::Type ADUAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if (OtherTeamID != 5) return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Friendly;
}

void ADUAIController::OnPlayerDetected(const TArray<AActor*>& DetectedPawn)
{
	if (BlackboardComponent && SightConfig)
	{
		if (GetPawn()->GetDistanceTo(DetectedPawn[0]) > SightConfig->SightRadius)
		{
			BlackboardComponent->SetValueAsObject(FName("PlayerActorKey"), NULL);
		}
		else
		{
			BlackboardComponent->SetValueAsObject(FName("PlayerActorKey"), DetectedPawn[0]);
		}
	}
}

void ADUAIController::BeginPlay()
{
	Super::BeginPlay();


	if (BehaviorTree != nullptr && BlackboardData != nullptr)
	{
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	
	RunBehaviorTree(BehaviorTree);
	InitTargetPoint();
}

void ADUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	DUEnemy = Cast<ADUEnemy>(InPawn);
}

void ADUAIController::InitTargetPoint()
{
	DUEnemy = DUEnemy == nullptr ? Cast<ADUEnemy>(GetPawn()) : DUEnemy;
	if (DUEnemy && BlackboardComponent)
	{
		FVector TargetPointLocation = DUEnemy->GetTargetPointLocation();
		BlackboardComponent->SetValueAsVector(FName("TargetPoint"), TargetPointLocation);
	}
}