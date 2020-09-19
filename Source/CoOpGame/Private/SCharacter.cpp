// Copyright© 2020-2021 AlemdarLabs, All Rights Reserved

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoOpGame/Public/Components/SHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SWeapon.h"

ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	WeaponSocketName = TEXT("WeaponSocket");
	ZoomedFOV = 65.f;
	ZoomInterpSpeed = 20.f;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(WeaponClass);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketName);
	CurrentWeapon->SetOwner(this); // then we can reach the Shooter Character from Weapon

	DefaultFOV = CameraComp->FieldOfView;
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AdjustFieldOfView(DeltaTime);
}

#pragma region Movements
void ASCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASCharacter::LookUp(float Value)
{
	if (Value != 0.f)
	{
		AddControllerPitchInput(Value);
	}
}

void ASCharacter::LookRight(float Value)
{
	if (Value != 0.f)
	{
		AddControllerYawInput(Value);
	}
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}
#pragma endregion

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ASCharacter::LookRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASCharacter::BeginFire);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ASCharacter::EndFire);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);
}

float ASCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComp)
	{
		DamageToApply = FMath::Min(HealthComp->CurrentHealth, DamageToApply);
		HealthComp->CurrentHealth -= DamageToApply;
	}

	if (IsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

void ASCharacter::BeginFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ASCharacter::EndFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->EndFire();
	}
}

bool ASCharacter::IsDead() const
{
	if (HealthComp == nullptr)
	{
		return true;
	}
	return HealthComp->CurrentHealth <= 0;
}

void ASCharacter::AdjustFieldOfView(float DeltaTime)
{
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);
}
