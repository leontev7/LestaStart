// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "LestaCharacter.generated.h"

class UCameraComponent;
class UCharacterWeaponComponent;
class ULestaHealthPointsComponent;

/** Base Character class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaCharacter : public ACharacter
{
	GENERATED_BODY()

	// You can extend this class if required
	// Blueprint Character class is derived from the ALestaCharacter

public:
	ALestaCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	FRotator GetOffset() const;

	UFUNCTION(Server, Reliable)
	void SetOffset();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:

	UPROPERTY(Replicated)
	FRotator Offset;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> PistolMeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	/** Input action assigned to movement. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	/** Input action assigned to camera movement. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	/** Input action assigned to left mouse button */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackInputAction;

	/** Input action assigned to key 1 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Key1InputAction;

	/** Input action assigned to key 2 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Key2InputAction;

	/** Input action assigned to scroll mouse wheel */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MouseWheelInputAction;

	/** Input action assigned to key R */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> KeyRInputAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterWeaponComponent> WeaponComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULestaHealthPointsComponent> HealthPointsComponent;

	virtual void OnMoveInput(const FInputActionInstance& InputActionInstance);
	
	virtual void OnLookInput(const FInputActionInstance& InputActionInstance);
	
	virtual void OnAttackInput(const FInputActionInstance& InputActionInstance);
	
	virtual void OnAttackCompletedInput(const FInputActionInstance& InputActionInstance);
	
	virtual void OnKey1Input(const FInputActionInstance& InputActionInstance);
	
	virtual void OnKey2Input(const FInputActionInstance& InputActionInstance);
	
	virtual void OnMouseWheelInput(const FInputActionInstance& InputActionInstance);

	virtual void OnKeyRInput(const FInputActionInstance& InputActionInstance);
	

public:
	USkeletalMeshComponent* GetPistolMeshComponent() const { return PistolMeshComponent; }
};
