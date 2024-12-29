// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

}

void AAuraPlayerController::PlayerTick(float DeltaTime) 
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// assert: cannot run without input
	check(AuraContext) 
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(AuraContext, 0);
	} // else other players

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;

	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);


	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ConmtrolledPawn = GetPawn<APawn>() ) {
		ConmtrolledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ConmtrolledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();	// Cast<> not required

	/** scenarios:
	 * A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 * B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor (first time hover)
	 * C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor (no longer hovering)
	 * D. LastActor is valid && ThisActor is valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, Highlight ThisActor
	 * E. LastActor is valid && ThisActor is valid, but LastActor == ThisActor
	 *		- Do nothing (already hovering and highlighted)
	 */

	 if (LastActor == nullptr) {
		// A or B
		if (ThisActor != nullptr) {
			// B
			ThisActor->HighLightActor();
		}
		else {
			// A
		}
	 }
	 else {
		// C, D, E
		 if (ThisActor == nullptr) {
			//
			LastActor->UnHighLightActor();
		 }
		 else {
			// D or E
			if (LastActor != ThisActor) {
				// D
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
			else {
				// E
			}
		 }
	 }
}