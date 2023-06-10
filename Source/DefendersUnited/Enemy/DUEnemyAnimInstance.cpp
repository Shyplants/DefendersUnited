#include "DUEnemyAnimInstance.h"
#include "DUEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"


void UDUEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<ADUEnemy>(TryGetPawnOwner());
}

void UDUEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Enemy == nullptr)
	{
		Enemy = Cast<ADUEnemy>(TryGetPawnOwner());
	}
	if (Enemy == nullptr) return;


	bAttack = Enemy->IsAttack();

	FVector Velocity = Enemy->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Enemy->GetCharacterMovement()->IsFalling();
	bIsAccelerating = Enemy->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bElimmed = Enemy->IsElimmed();

}