// Fill out your copyright notice in the Description page of Project Settings.


#include "DUAnimInstance.h"
#include "DUCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DefendersUnited/Weapon/Weapon.h"

void UDUAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ADUCharacter>(TryGetPawnOwner());
}

void UDUAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Character == nullptr)
	{
		Character = Cast<ADUCharacter>(TryGetPawnOwner());
	}
	if (Character == nullptr) return;

	FVector Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Character->GetCharacterMovement()->IsFalling();
	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = Character->IsWeaponEquipped();
	EquippedWeapon = Character->GetEquippedWeapon();
	bIsCrouched = Character->bIsCrouched;
	bAiming = Character->IsAiming();
	TurningInPlace = Character->GetTurningInPlace();
	bRotateRootBone = Character->ShouldRotateRootBone();
	bElimmed = Character->IsElimmed();

	// Offset Yaw for Strafing
	FRotator AimRotation = Character->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Character->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = Character->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = Character->GetAO_Yaw();
	AO_Pitch = Character->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && Character->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		Character->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (Character->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			FRotator LookAtRocation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - Character->GetHitTarget()));
			RightHandRotation = FMath::RInterpTo(RightHandRotation, LookAtRocation, DeltaTime, 30.f);

			/*
			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World); 
			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), MuzzleTipTransform.GetLocation() + MuzzleX * 1000.f, FColor::Red);
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), Character->GetHitTarget(), FColor::Orange);
			*/
		}
	}
}
