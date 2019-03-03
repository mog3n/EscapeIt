// Fill out your copyright notice in the Description page of Project Settings.
#include "DoorOpen.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"

UDoorOpen::UDoorOpen()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UDoorOpen::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner(); // Get the owner of the class
}
void UDoorOpen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdatePlateMass();
}




// Opens the Door
void UDoorOpen::OpenDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Door opened"));
	FRotator openRotation = FRotator(0.0f, OpenDoorAngle, 0.0f); // Create rotation
	Owner->SetActorRotation(openRotation); // Open the door
}

// Closes the door
void UDoorOpen::CloseDoor()
{
	FRotator openRotation = FRotator(0.0f, -180.f, 0.0f); // Create rotation
	Owner->SetActorRotation(openRotation); // Close the door
}

// Returns total mass in KG
float UDoorOpen::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> Actors; // extract actors
	TriggerVolume->GetOverlappingActors(Actors);
	for (const auto& Actor : Actors) {
		float ActorMass = Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		TotalMass += ActorMass;
		//DEBUG
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor->GetName());
	}
	return TotalMass;
}

void UDoorOpen::UpdatePlateMass() {
	if (GetTotalMassOfActorsOnPlate() >= TriggerMass) {
		OpenDoor();
		DoorLastOpenedInSeconds = GetWorld()->GetTimeSeconds();
	}
	// Open the door for n seconds
	float timeDelta = GetWorld()->GetTimeSeconds() - DoorLastOpenedInSeconds;
	if (timeDelta > CloseDoorDelayInSeconds) {
		CloseDoor();
	}
}