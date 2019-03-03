// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorOpen.h"

// Sets default values for this component's properties
UDoorOpen::UDoorOpen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Open the door
void UDoorOpen::OpenDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Door opened"))
	FRotator openRotation = FRotator(0.0f, OpenDoorAngle, 0.0f); // Create rotation
}

// Close the door
void UDoorOpen::CloseDoor()
{
	FRotator openRotation = FRotator(0.0f, -180.f, 0.0f); // Create rotation
	Owner->SetActorRotation(openRotation); // Close the door
}

// Called when the game starts
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner(); // Get the owner of the class
	Player = GetWorld()->GetFirstPlayerController()->GetPawn(); // get the player's pawn
}


// Called every frame
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if actor is standing on trigger volume
	if (TriggerVolume->IsOverlappingActor(Player)) {
		OpenDoor();
		DoorLastOpenedInSeconds = GetWorld()->GetTimeSeconds();
	}

	float timeDelta = GetWorld()->GetTimeSeconds() - DoorLastOpenedInSeconds;
	if (timeDelta>CloseDoorDelayInSeconds) {
		CloseDoor();
	}
}

