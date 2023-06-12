#include "DUCharacterSelectWidget.h"
#include "Engine/StaticMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "DefendersUnited/GameState/DUGameInstance.h"
#include "DefendersUnited/PlayerState/DUPlayerState.h"
#include "DefendersUnited/PlayerController/DUPlayerController.h"

void UDUCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	TargetComponent->SetSkeletalMesh(SkeletalMeshArray[CurrentIndex]);
}

void UDUCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 0 번 AR
	USkeletalMesh* NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	SkeletalMeshArray.Add(NewSkeletalMesh);
	// 1 번 RL
	NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/MilitaryWeapSilver/Weapons/Rocket_Launcher_A.Rocket_Launcher_A'"));
	SkeletalMeshArray.Add(NewSkeletalMesh);
	// 2 번 SR
	NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/MilitaryWeapSilver/Weapons/Sniper_Rifle_A.Sniper_Rifle_A'"));
	SkeletalMeshArray.Add(NewSkeletalMesh);
	// 3 번 SMG
	NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Nostock_Y.SK_SMG11_Nostock_Y'"));
	SkeletalMeshArray.Add(NewSkeletalMesh);


	CurrentIndex = 0;
	MaxIndex = SkeletalMeshArray.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	NameTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));

	PrevButton->OnClicked.AddDynamic(this, &UDUCharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UDUCharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UDUCharacterSelectWidget::OnConfirmClicked);
}

void UDUCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UDUCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UDUCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = NameTextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	switch (CurrentIndex)
	{
	case 0:
		WeaponType = EWeaponType::EWT_AssaultRifle;
		break;
	case 1:
		WeaponType = EWeaponType::EWT_RocketLauncher;
		break;
	case 2:
		WeaponType = EWeaponType::EWT_SniperRifle;
		break;
	case 3:
		WeaponType = EWeaponType::EWT_SubmachineGun;
		break;
	default:
		WeaponType = EWeaponType::EWT_MAX;
		break;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
	ADUPlayerState* DUPlayerState = PlayerController->GetPlayerState<ADUPlayerState>();

	if (DUGameInstance)
	{
		DUGameInstance->WeaponType = WeaponType;
	}

	if (DUPlayerState)
	{
		DUPlayerState->SetPlayerName(CharacterName);
	}


	PlayerController->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::OpenLevel(GetWorld(), FName("TestMap"));

}