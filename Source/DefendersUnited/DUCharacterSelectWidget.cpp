#include "DUCharacterSelectWidget.h"
#include "Engine/StaticMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Kismet/GameplayStatics.h"

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

	USkeletalMesh* NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Character/CompleteCharacters/SK_CharM_Barbarous.SK_CharM_Barbarous"));
	SkeletalMeshArray.Add(NewSkeletalMesh);
	NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Character/CompleteCharacters/SK_CharM_Robo.SK_CharM_Robo"));
	SkeletalMeshArray.Add(NewSkeletalMesh);
	NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Character/CompleteCharacters/SK_CharM_Tusk.SK_CharM_Tusk"));
	SkeletalMeshArray.Add(NewSkeletalMesh);
	NewSkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/Character/CompleteCharacters/SK_CharM_Golden.SK_CharM_Golden"));
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
	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
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
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::OpenLevel(GetWorld(), FName("Gameplay"));
}