// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationTestingCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AutomationTesting.h"
#include "Components/InventoryComponent.h"

AAutomationTestingCharacter::AAutomationTestingCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AAutomationTestingCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthData.MaxHealth > 0.0f);
	Health = HealthData.MaxHealth;

	OnTakeAnyDamage.AddDynamic(this, &AAutomationTestingCharacter::OnAnyDamageRecieved);
}


void AAutomationTestingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &AAutomationTestingCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &AAutomationTestingCharacter::Look);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &AAutomationTestingCharacter::Look);
	}
	else
	{
		UE_LOG(LogAutomationTesting, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AAutomationTestingCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AAutomationTestingCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AAutomationTestingCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AAutomationTestingCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AAutomationTestingCharacter::DoJumpStart()
{
	Jump();
}

void AAutomationTestingCharacter::DoJumpEnd()
{
	StopJumping();
}

float AAutomationTestingCharacter::GetHealthPercent() const
{
	return Health / HealthData.MaxHealth;
}

void AAutomationTestingCharacter::OnAnyDamageRecieved(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	const auto IsAlive = [&]() { return Health > 0.0f; };

	if (Damage <= 0.0f || !IsAlive()) return;

	Health = FMath::Clamp(Health - Damage, 0.0f, HealthData.MaxHealth);

	if (IsAlive())
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &AAutomationTestingCharacter::OnHealing,
		                                HealthData.HealRate, true, -1.0f);
	}
	else
	{
		OnDeath();
	}
}

void AAutomationTestingCharacter::OnHealing()
{
	Health = FMath::Clamp(Health + HealthData.HealModifier, 0.0f, HealthData.MaxHealth);
	if (FMath::IsNearlyEqual(Health, HealthData.MaxHealth))
	{
		Health = HealthData.MaxHealth;
		GetWorldTimerManager().ClearTimer(HealTimerHandle);
	}
}

void AAutomationTestingCharacter::OnDeath()
{
	GetWorldTimerManager().ClearTimer(HealTimerHandle);

	check(GetCharacterMovement());
	check(GetCapsuleComponent());
	check(GetMesh());

	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	SetLifeSpan(HealthData.LifeSpan);
}
