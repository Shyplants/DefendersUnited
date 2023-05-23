#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class DEFENDERSUNITED_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CasingMesh;

protected:
	virtual void BeginPlay() override;

};
