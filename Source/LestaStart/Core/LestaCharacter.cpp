// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaCharacter.h"

#include "CharacterWeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "LestaHealthPointsComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

ALestaCharacter::ALestaCharacter()
{
	NetUpdateFrequency = 20.f;
	bReplicates = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh());

	WeaponComponent = CreateDefaultSubobject<UCharacterWeaponComponent>(TEXT("Character Weapon"));

	HealthPointsComponent = CreateDefaultSubobject<ULestaHealthPointsComponent>(TEXT("Box Health Points Component"));
	HealthPointsComponent->SetMaxHealthPoints(1000);

	PistolMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Weapon"));
	PistolMeshComponent->SetupAttachment(GetMesh(), TEXT("hand_r"));
	PistolMeshComponent->SetIsReplicated(true);
}

void ALestaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC) // should validate component because developers can change input component class through Project Settings
	{
		EIC->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput);
		EIC->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookInput);
		EIC->BindAction(AttackInputAction, ETriggerEvent::Started, this, &ThisClass::OnAttackInput);
		EIC->BindAction(AttackInputAction, ETriggerEvent::Completed, this, &ThisClass::OnAttackCompletedInput);
		EIC->BindAction(Key1InputAction, ETriggerEvent::Started, this, &ThisClass::OnKey1Input);
		EIC->BindAction(Key2InputAction, ETriggerEvent::Started, this, &ThisClass::OnKey2Input);
		EIC->BindAction(MouseWheelInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMouseWheelInput);
		EIC->BindAction(KeyRInputAction, ETriggerEvent::Started, this, &ThisClass::OnKeyRInput);
	}
	else
	{
		// Print error message into log
		// You can read more here: https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine
		UE_LOG(LogInput, Error, TEXT("Unexpected input component class: %s"), *GetFullNameSafe(PlayerInputComponent))
	}
}

FRotator ALestaCharacter::GetOffset() const
{
	return IsLocallyControlled() ? GetControlRotation() : Offset;
}

void ALestaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALestaCharacter, Offset);
}

void ALestaCharacter::SetOffset_Implementation()
{
	if (HasAuthority())
		Offset = GetControlRotation();
}

void ALestaCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALestaCharacter::OnMoveInput(const FInputActionInstance& InputActionInstance)
{
	// Controller rotation Yaw determines which direction Character is facing
	// so MoveForward = along that direction AND MoveRight = to the right of that direction

	const float YawDegree = GetControlRotation().Yaw; // controller rotation Yaw (in degrees)
	const float YawRadian = FMath::DegreesToRadians(YawDegree); // controller rotation Yaw (in radians)
	const FVector ForwardDirection = FVector(FMath::Cos(YawRadian), FMath::Sin(YawRadian), 0.f);
	const FVector RightDirection = FVector(FMath::Cos(YawRadian + UE_HALF_PI), FMath::Sin(YawRadian + UE_HALF_PI), 0.f);

	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddMovementInput(ForwardDirection * Input2D.X + RightDirection * Input2D.Y);
}

void ALestaCharacter::OnLookInput(const FInputActionInstance& InputActionInstance)
{
	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Input2D.X);
	AddControllerPitchInput(Input2D.Y);
	SetOffset();
}

void ALestaCharacter::OnAttackInput(const FInputActionInstance& InputActionInstance)
{
	WeaponComponent->StartAttack();
}

void ALestaCharacter::OnAttackCompletedInput(const FInputActionInstance& InputActionInstance)
{
	WeaponComponent->EndAttack();
}

void ALestaCharacter::OnKey1Input(const FInputActionInstance& InputActionInstance)
{
	WeaponComponent->SetWeapon(1);
}

void ALestaCharacter::OnKey2Input(const FInputActionInstance& InputActionInstance)
{
	WeaponComponent->SetWeapon(2);
}

void ALestaCharacter::OnMouseWheelInput(const FInputActionInstance& InputActionInstance)
{
	WeaponComponent->ChangeWeapon();
}

void ALestaCharacter::OnKeyRInput(const FInputActionInstance& InputActionInstance)
{
	WeaponComponent->ReloadWeapon();
}
