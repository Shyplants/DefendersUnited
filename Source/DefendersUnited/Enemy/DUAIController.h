// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DUAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEFENDERSUNITED_API ADUAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADUAIController();
	virtual void BeginPlay() override;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION() 
	void OnPlayerDetected(const TArray<AActor*>& DetectedPawn);

protected:
	
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = AI)
	UBlackboardData* BlackboardData;

	class UAIPerceptionComponent* AIPerceptionComponent;
	class UAISenseConfig_Sight* SightConfig;

	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* BlackboardComponent;

private:
	UPROPERTY()
	class ADUEnemy* DUEnemy;

	void InitTargetPoint();
	
};
