// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class ASWeapon;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Shoot();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDead() const { return Health <= 0; }

protected:
	virtual void BeginPlay() override;

	#pragma region Movements
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void BeginCrouch();
	void EndCrouch();
	void BeginZoom();
	void EndZoom();
	#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASWeapon> WeaponClass;

	UPROPERTY()
	ASWeapon* CurrentWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponSocketName;

	UPROPERTY(EditAnywhere, Category = "Player")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.f, ClampMax = 100.f))
	float ZoomInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	float DefaultFOV;

	bool bWantsToZoom;

private:	
	void AdjustFieldOfView(float DeltaTime);
	
};
