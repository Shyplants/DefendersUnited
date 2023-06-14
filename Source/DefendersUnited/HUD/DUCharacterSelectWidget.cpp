#include "DUCharacterSelectWidget.h"
#include "Engine/StaticMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "DefendersUnited/GameState/DUGameInstance.h"
#include "DefendersUnited/PlayerController/DULobbyController.h"


void UDUCharacterSelectWidget::NextCharacter(bool bForward)
{
	WeaponImages[CurrentIndex]->SetVisibility(ESlateVisibility::Collapsed);
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	WeaponImages[CurrentIndex]->SetVisibility(ESlateVisibility::Visible);
}

void UDUCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	NameTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	RifleImage = Cast<UImage>(GetWidgetFromName(TEXT("imgRifle")));
	WeaponImages.Add(RifleImage);
	RocketImage = Cast<UImage>(GetWidgetFromName(TEXT("imgRocket")));
	WeaponImages.Add(RocketImage);
	SniperImage = Cast<UImage>(GetWidgetFromName(TEXT("imgSniper")));
	WeaponImages.Add(SniperImage);
	SMGImage = Cast<UImage>(GetWidgetFromName(TEXT("imgSMG")));
	WeaponImages.Add(SMGImage);

	for (UImage* WeaponImage : WeaponImages)
	{
		WeaponImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	CurrentIndex = 0;
	MaxIndex = WeaponImages.Num();
	WeaponImages[CurrentIndex]->SetVisibility(ESlateVisibility::Visible);
	
	

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
	FString PlayerName = NameTextBox->GetText().ToString();
	if (PlayerName.Len() <= 0 || PlayerName.Len() > 10) return;

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

	ADULobbyController* DULobbyPlayerController = Cast<ADULobbyController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DULobbyPlayerController)
	{
		DULobbyPlayerController->OnConfirmClicked(PlayerName, WeaponType);
	}


	// UDUGameInstance* DUGameInstance = Cast<UDUGameInstance>(GetGameInstance());
	/*
	if (DUGameInstance)
	{
		DUGameInstance->WeaponType = WeaponType;
		DUGameInstance->SetDUPlayerName(PlayerName);
	}
	*/
}