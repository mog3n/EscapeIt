// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/Public/DrawDebugHelpers.h"
#define OUT // does nothing, just for understanding

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber ready for action boisssssss"));
	FindComponents();
	SetupInputComponent();
}

void UGrabber::FindComponents() {
	// Find Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Grabber in '%s' could not find Phyics Handler component"), *GetOwner()->GetName());
	}
	// Find Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("Grabber in '%s' could not find Input Component component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

void UGrabber::UpdatePhysicsHandle()
{
	// Get the player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	if (PhysicsHandle->GetGrabbedComponent() != NULL) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyHit()
{
	// Get the player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	/// Draw a line trace
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	/*
	DrawDebugLine(GetWorld(),PlayerViewPointLocation,LineTraceEnd,FColor(255, 0, 0), // red linefalse,0.f,0.f,10.0f);
	*/
	/// Setup query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	/// Line Trace
	FHitResult Hit;
	/// Ray cast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed!"));
	/// Try and reach any actors
	FHitResult Hit = GetFirstPhysicsBodyHit();
	UPrimitiveComponent* HitComp = Hit.GetComponent();
	// Check if any components were hit
	if (HitComp) {
		// Attatch the component to the physics handle
		PhysicsHandle->GrabComponent(HitComp, "", Hit.GetActor()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdatePhysicsHandle();
}