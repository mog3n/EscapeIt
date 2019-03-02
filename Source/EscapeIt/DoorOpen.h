// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/PlayerController.h"

#include "Engine/World.h"
#include "DoorOpen.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEIT_API UDoorOpen : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpen();
	// open/close door
	void OpenDoor();
	void CloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* TriggerVolume;
	UPROPERTY(EditAnywhere)
		float OpenDoorAngle = 90.f;
	UPROPERTY(EditAnywhere)
		float CloseDoorDelayInSeconds = 1.0f;
	
	AActor* Owner; // store the owner of the class
	AActor* Player; // store player
	float DoorLastOpenedInSeconds;
};
