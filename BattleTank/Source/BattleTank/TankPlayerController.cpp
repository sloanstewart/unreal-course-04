// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "BattleTank.h"
#include "Public/Tank.h"

// Tick
// Super
// AimTowardsCrosshair();

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank *ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank())
	{
		return;
	}
	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector &OutHitLocation) const
{
	// Find crosshair position

	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

	// Deproject screen position of crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// line trace along that direction - look direction; see what we hit
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector &LookDirection) const
{
	FVector CameraWorldLocation; // to be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector &OutHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
					HitResult,
					StartLocation,
					EndLocation,
					ECollisionChannel::ECC_Visibility))
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	OutHitLocation = FVector(0);
	return false;
}